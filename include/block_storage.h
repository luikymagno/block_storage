#ifndef BLOCK_STORAGE_H
#define BLOCK_STORAGE_H

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <linux/fs.h>
#include <malloc.h>
#include <sys/ioctl.h>

#include "blkstorage.h"

template <class T> class Block;

template <class T> class BlockStorage {

public:
    /**
     * @brief Tries to construct a block storage device by matching the hardware
     * block size and the size of the structure passed by template.
     * @param path is the path to a file (existing or not) where the data
     * is going to be written/read to.
     * @throws std::logic_error
     * @throws std::runtime_error
     */
    BlockStorage(const std::string &path):
        path_(path), fd_raii_(nullptr){
        size_t blk_size;
        if ((GetBlockSize(blk_size) != blkstorage::kNoError) ||
                (sizeof(T) != blk_size)) {
            throw std::logic_error(blkstorage::kSizeInvalid);
        }

        fd_raii_ = blkstorage::MakeFileRAII(path, blkstorage::kDefautFileFlags);
        if (!fd_raii_) {
            throw std::runtime_error(blkstorage::kOpenStorageFileFailed +
                                     std::string(std::strerror(errno)));
        }
    }

    /**
     * @brief empty destructor
     */
    virtual ~BlockStorage() {}

    blkstorage::BlkStgStatus GetBlockSize(size_t &size_out) const {
        auto fd_raii = blkstorage::MakeFileRAII(
                    blkstorage::kStorageDeviceFilePath, O_RDONLY);
        uint32_t block_size;
        if ((fd_raii) && (ioctl(*fd_raii, BLKBSZGET, &block_size) >= 0)) {
            size_out = block_size;
            return blkstorage::kNoError;
        }
        return blkstorage::kFileError;
    }

    /**
     * @brief Clears the contents of the current block storage file used to
     * store data.
     * @return kNoError if it succeeds or kFileError if the file cannot be
     * cleared
     */
    blkstorage::BlkStgStatus ClearFile() {
        if ((blkstorage::FileExists(path_)) &&
                (blkstorage::RemoveFile(path_) != blkstorage::kNoError)) {
            return blkstorage::kFileError;
        }

        fd_raii_ = blkstorage::MakeFileRAII(this->path_,
                                            blkstorage::kDefautFileFlags);

        if (!fd_raii_) {
            return blkstorage::kFileError;
        }

        return blkstorage::kNoError;
    }

    /**
     * @brief Clears the contents of the current block storage file used to
     * store data and tries to allocate a certain ammount of storage for the
     * file.
     * @param size_gb is the ammount of storage to be allocated
     * @return kNoError if it succeeds or kFileError if the file cannot be
     * cleared or kAllocError if the sotrage allocation fails.
     */
    blkstorage::BlkStgStatus ClearAndAlloc(uint32_t size_gb) {
        fd_raii_.reset();
        blkstorage::BlkStgStatus status = this->ClearFile();

        if (blkstorage::kNoError != status) {
            return status;
        }

        fd_raii_ = blkstorage::MakeFileRAII(this->path_,
                                            blkstorage::kDefautFileFlags);

        if (!fd_raii_) {
            return blkstorage::kFileError;
        }

        if (fallocate(*fd_raii_, FALLOC_FL_ZERO_RANGE,
                      0, blkstorage::kBytesInGigaByte*size_gb) != 0) {
            return blkstorage::kAllocError;
        }

        return blkstorage::kNoError;
    }

    /**
     * @brief reads a block from the storage device.
     * @param offset_block is the address (in nuber of blocks) of the block to
     * be read into block_out.
     * @param block_out is the output parameter where the block read by this
     * member function if going to be written to.
     * @return kNoError if the function succeeds or kFileError otherwise.
     */
    blkstorage::BlkStgStatus ReadBlock(uint32_t offset_block,
                                       Block<T> &block_out) const {
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_reads = pread(*fd_raii_, block_out.GetRawPointer(),
                                 sizeof(T), offset_bytes);

        if (sizeof(T) != num_reads) {
            return blkstorage::kFileError;
        }

        return blkstorage::kNoError;
    }

    /**
     * @brief writes a block into the storage device.
     * @param offset_block is the address (in nuber of blocks) of the block to
     * be written.
     * @param block_in is the block that will be written nto the storage device.
     * @return kNoError if the function succeeds or kFileError otherwise.
     */
    blkstorage::BlkStgStatus WriteBlock(uint32_t offset_block,
                                        const Block<T> &block_in) {
        size_t offset_bytes = offset_block*sizeof(T);

        size_t num_writes = pwrite(*fd_raii_, block_in.GetRawPointer(),
                                   sizeof(T), offset_bytes);

        if (sizeof(T) != num_writes) {
            return blkstorage::kFileError;
        }

        return blkstorage::kNoError;
    }

private:
    std::shared_ptr<int> fd_raii_;
    std::string path_;
};


template <class T> class Block {
    friend class BlockStorage<T>;

public:

    /**
     * @brief Tries to construct a block by allocating memory alligned space
     * of the size of the structure passed by template.
     * @throws std::runtime_error
     */
    Block():
    data_(nullptr){
        data_ = std::shared_ptr<T>((T*) memalign(sizeof(T), sizeof(T)),
                                  [](T *p){free(p);});
        if (!data_.get()) {
            throw std::runtime_error(blkstorage::kAllocFailed);
        }
    }

    /**
     * @brief gets a copy of the data from block.
     * @return a copy of data content.
     */
    T data() const {return (*data_);}

    /**
     * @brief SetData
     * @param data
     */
    void SetData(const T &data) {*data_ = data;}

private:
    T* GetRawPointer() const {return data_.get();}
    std::shared_ptr<T> data_;
};

#endif // BLOCK_STORAGE_H
