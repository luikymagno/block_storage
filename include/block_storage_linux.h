#ifndef BLOCK_STORAGE_LINUX_H
#define BLOCK_STORAGE_LINUX_H

#include <linux/fs.h>
#include <memory>
#include <sys/ioctl.h>

#include "block_storage.h"

const std::string kStorageDeviceFilePath = "/dev/sda";


template <class T>
class BlockStorageLinux: public BlockStorage<T> {

public:
    BlockStorageLinux(const std::string &path):
        BlockStorage<T>(path) {}

    virtual ~BlockStorageLinux() {}

    virtual BlkStgStatus GetBlockSize(uint32_t &size_out) const {
        auto fd_raii = storage_functions::MakeFileRAII(kStorageDeviceFilePath);
        uint32_t block_size;
        if ((fd_raii) && (ioctl(*fd_raii, BLKBSZGET, &block_size) >= 0)) {
            size_out = block_size;
            return kNoError;
        }
        return kFileError;
    }

    virtual BlkStgStatus ClearAndAlloc(uint32_t size_gb) {
        BlkStgStatus status = this->ClearFile();

        if (kNoError != status) {
            return status;
        }

        return kNoError;
    }

    virtual BlkStgStatus ReadBlocks(uint32_t offset,
                                    uint32_t num_of_blocks,
                                    std::vector<T> &blocks_out) const {
        return kNoError;
    }

    virtual BlkStgStatus WriteBlocks(uint32_t offset,
                                     const std::vector<T> &blocks) {
        return kNoError;
    }

};

#endif // BLOCK_STORAGE_LINUX_H
