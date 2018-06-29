#ifndef BLKSTORAGE_H
#define BLKSTORAGE_H

#include <memory>
#include <string>

#include <fcntl.h>
#include <unistd.h>

namespace blkstorage {

const std::string kAllocFailed = "Aligned memory allocation failed.";
const std::string kOpenStorageFileFailed = "Couldn't open storage device file: ";
const std::string kSizeInvalid = "Invalid block size.";
const std::string kStorageDeviceFilePath = "/dev/sda";

const int kBytesInGigaByte = 1000000000;
const int kDefautFileFlags = O_RDWR | O_SYNC | O_DIRECT | O_CREAT;

enum BlkStgStatus {kFileError, kAllocError, kNoError};

std::shared_ptr<int> MakeFileRAII(const std::string &path, uint32_t flags) {
    int fd = open(path.c_str(), flags, S_IRWXU);
    if (fd < 0) {
        return nullptr;
    }
    return std::shared_ptr<int>(new int(fd), [](int *fd) {
        close(*fd);
        delete fd;
    });
}

bool FileExists(const std::string &path) {
    return access(path.c_str(), F_OK ) != -1;
}

BlkStgStatus RemoveFile(const std::string &path) {
    return (remove(path.c_str()) != -1) ? kNoError : kFileError;
}

}

#endif // BLKSTORAGE_H
