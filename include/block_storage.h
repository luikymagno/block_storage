#ifndef BLOCK_STORAGE_H
#define BLOCK_STORAGE_H

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <linux/fs.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "storage_functions.h"

const std::string kAllocFailed = "Aligned memory allocation failed";
const std::string kOpenStorageFileFailed = "Couldn't open storage device file: ";
const std::string kOuOfRangeInvalid = "Out of range";
const std::string kSizeZeroInvalid = "Size equals to zero";
const std::string kStorageDeviceFilePath = "/dev/sda";

const int kBytesInGigaByte = 1000000000;
const int kDefautFileFlags = O_CREAT| O_RDWR | O_SYNC | O_DIRECT;

template <class T> class BlockArray;

template <class T> class BlockStorage {

public:
    BlockStorage(const std::string &path):
        path_(path), num_of_blocks_(0), fd_raii_(nullptr){
        fd_raii_ = storage_functions::MakeFileRAII(path,
                                                             kDefautFileFlags);
        if (!fd_raii_) {
            throw std::runtime_error(kOpenStorageFileFailed +
                                     std::string(std::strerror(errno)));
        }
    }

    virtual ~BlockStorage() {}

    BlkStgStatus GetBlockSize(uint32_t &size_out) const {
        auto fd_raii = storage_functions::MakeFileRAII(kStorageDeviceFilePath,
                                                       O_RDONLY);
        uint32_t block_size;
        if ((fd_raii) && (ioctl(*fd_raii, BLKBSZGET, &block_size) >= 0)) {
            size_out = block_size;
            return kNoError;
        }
        return kFileError;
    }

    BlkStgStatus ClearFile() {
        if ((storage_functions::FileExists(path_)) &&
             storage_functions::RemoveFile(path_) != kNoError) {
            return kFileError;
        }
        return kNoError;
    }

    BlkStgStatus ClearAndAlloc(uint32_t size_gb) {
        fd_raii_.reset();
        BlkStgStatus status = this->ClearFile();

        if (kNoError != status) {
            return status;
        }

        fd_raii_ = storage_functions::MakeFileRAII(this->path_,
                                                             kDefautFileFlags);

        if (!fd_raii_) {
            return kFileError;
        }

        if (fallocate(*fd_raii_, FALLOC_FL_ZERO_RANGE,
                      0, kBytesInGigaByte*size_gb) != 0) {
            return kAllocError;
        }

        return kNoError;
    }

    BlkStgStatus ReadBlocks(uint32_t offset_block,
                            BlockArray<T> &request) const {
        size_t read_size = request.size()*sizeof(T);
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_reads = pread(*fd_raii_, request.GetRawData(),
                                 read_size, offset_bytes);

        if (num_reads != read_size) {
            return kFileError;
        }


        return kNoError;
    }

    BlkStgStatus WriteBlocks(uint32_t offset_block,
                             const BlockArray<T> &request) {
        size_t write_size = request.size()*sizeof(T);
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_writes = pwrite(*fd_raii_, request.GetRawData(),
                                   write_size, offset_bytes);

        if (num_writes != write_size) {
            return kFileError;
        }

        return kNoError;
    }

    uint32_t num_of_blocks() const {return num_of_blocks_;}

private:
    std::shared_ptr<int> fd_raii_;
    std::string path_;
    uint32_t num_of_blocks_;

};


template <class T> class BlockArray {
    friend class BlockStorage<T>;

public:
    BlockArray(uint32_t block_size, uint32_t block_count):
    size_(block_count),
    buffer_(nullptr){
        if ((0 == block_size) || (0 == block_count)) {
            throw std::logic_error(kSizeZeroInvalid);
        }

        buffer_ = std::shared_ptr<T>((T*) memalign(size_*sizeof(T), block_size),
                                  [](T *p){free(p);});
        if (!buffer_) {
            throw std::runtime_error(kAllocFailed);
        }
    }

    T& operator[](uint32_t i) {
        if (i < size_ ) {
            return buffer_.get()[i];
        }
        throw std::logic_error(kOuOfRangeInvalid);
    }

    uint32_t size() const {return size_;}

private:
    T* GetRawData() const { return buffer_.get(); }
    uint32_t size_;
    std::shared_ptr<T> buffer_;
};

#endif // BLOCK_STORAGE_H
