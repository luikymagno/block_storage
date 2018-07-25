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

std::shared_ptr<int> MakeFileRAII(const std::string &path, uint32_t flags);

bool FileExists(const std::string &path);

BlkStgStatus RemoveFile(const std::string &path);

}

#endif // BLKSTORAGE_H
