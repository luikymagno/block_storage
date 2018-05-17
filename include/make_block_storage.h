#ifndef MAKE_BLOCK_STORAGE_H
#define MAKE_BLOCK_STORAGE_H

#include <memory>
#include <exception>

//#ifdef LINUX
#include "block_storage_linux.h"
//#elif WINDOWS
//TODO
//#endif

const std::string kGetBlkSizeError = "Could not get block size during storage construction";
const std::string kTypeSizeError = "Type and block size must be equal";

template <class T>
std::unique_ptr<BlockStorage<T>> MakeBlockStorage(const std::string &path) {
//#ifdef LINUX
    std::unique_ptr<BlockStorage<T>> storage_ptr(new BlockStorageLinux<T>(path));
//#elif WINDOWS
    //TODO
//#endif

    uint32_t block_size = 0;
    if (kNoError != storage_ptr->GetBlockSize(block_size)) {
        throw std::runtime_error(kGetBlkSizeError);
    }

    if (sizeof(T) != block_size) {
        throw std::logic_error(kTypeSizeError);
    }

    return std::move(storage_ptr);
}


#endif // MAKE_BLOCK_STORAGE_H
