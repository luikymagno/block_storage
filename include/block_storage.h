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
const std::string kSizeInvalid = "Invalid block size";
const std::string kStorageDeviceFilePath = "/dev/sda";

const int kBytesInGigaByte = 1000000000;
const int kDefautFileFlags = O_CREAT| O_RDWR | O_SYNC | O_DIRECT;
const int kLinuxMaxRWCount = 2147479552;

template <class T> class Block;

template <class T> class BlockStorage {

public:
    BlockStorage(const std::string &path):
        path_(path), num_of_blocks_(0), fd_raii_(nullptr){
        size_t blk_size;
        if ((GetBlockSize(blk_size) != kNoError) && (sizeof(T) != blk_size)) {
            throw std::logic_error(kSizeInvalid);
        }

        size_t sizeoft = sizeof(T);

        fd_raii_ = storage_functions::MakeFileRAII(path, kDefautFileFlags);
        if (!fd_raii_) {
            throw std::runtime_error(kOpenStorageFileFailed +
                                     std::string(std::strerror(errno)));
        }
    }

    virtual ~BlockStorage() {}

    BlkStgStatus GetBlockSize(size_t &size_out) const {
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

    BlkStgStatus ReadBlock(uint32_t offset_block, Block<T> &block) const {
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_reads = pread(*fd_raii_, block.GetRawPointer(),
                                 sizeof(T), offset_bytes);

        if (sizeof(T) != num_reads) {
            return kFileError;
        }

        return kNoError;
    }

    BlkStgStatus WriteBlock(uint32_t offset_block, const Block<T> &block) {
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_writes = pwrite(*fd_raii_, block.GetRawPointer(),
                                   sizeof(T), offset_bytes);

        if (sizeof(T) != num_writes) {
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


template <class T> class Block {
    friend class BlockStorage<T>;

public:
    Block(uint32_t block_size):
    data_(nullptr){
        if ((sizeof(T) != block_size)) {
            throw std::logic_error(kSizeInvalid);
        }

        data_ = std::shared_ptr<T>((T*) memalign(sizeof(T), block_size),
                                  [](T *p){free(p);});
        if (!data_.get()) {
            throw std::runtime_error(kAllocFailed);
        }
    }

    T data() const {return (*data_);}

    void SetData(const T &data) {*data_ = data;}

private:
    T* GetRawPointer() const {return data_.get();}
    std::shared_ptr<T> data_;
};

#endif // BLOCK_STORAGE_H
