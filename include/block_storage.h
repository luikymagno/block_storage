#ifndef BLOCK_STORAGE_H
#define BLOCK_STORAGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "storage_functions.h"

template <class T>
class BlockStorage {

public:
    BlockStorage(const std::string &path):
        path_(path), num_of_blocks_(0) {}

    virtual ~BlockStorage() {}

    virtual BlkStgStatus GetBlockSize(uint32_t &size_out) const = 0;

    virtual BlkStgStatus ClearAndAlloc(uint32_t size_gb) = 0;

    virtual BlkStgStatus ReadBlocks(uint32_t offset,
                                    uint32_t num_of_blocks,
                                    std::vector<T> &blocks_out) const = 0;

    virtual BlkStgStatus WriteBlocks(uint32_t offset,
                                     const std::vector<T> &blocks) = 0;

    uint32_t num_of_blocks() const;

protected:
    BlkStgStatus ClearFile() {
        if ((storage_functions::FileExists(path_)) &&
             storage_functions::RemoveFile(path_) != kNoError) {
            return kFileError;
        }
        return kNoError;
    }

    std::string path_;
    uint32_t num_of_blocks_;
};


#endif // BLOCK_STORAGE_H
