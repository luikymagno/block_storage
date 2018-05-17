#ifndef STORAGE_FUNCTIONS_H
#define STORAGE_FUNCTIONS_H

#include <memory>
#include <string>

enum BlkStgStatus {kOutOfRangeError, kFileError, kNoError};

namespace storage_functions {

std::shared_ptr<int> MakeFileRAII(const std::string &path);

bool FileExists(const std::string &path);

BlkStgStatus RemoveFile(const std::string &path);

std::shared_ptr<int> MakeFileRAIILinux(const std::string &path);

bool FileExistsLinux(const std::string &path);

BlkStgStatus RemoveFileLinux(const std::string &path);
}




#endif // STORAGE_FUNCTIONS_H
