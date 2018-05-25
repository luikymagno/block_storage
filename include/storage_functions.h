#ifndef STORAGE_FUNCTIONS_H
#define STORAGE_FUNCTIONS_H

#include <memory>
#include <string>

enum BlkStgStatus {kFileError, kAllocError, kNoError};

namespace storage_functions {

std::shared_ptr<int> MakeFileRAII(const std::string &path, uint32_t flags);

bool FileExists(const std::string &path);

BlkStgStatus RemoveFile(const std::string &path);
}




#endif // STORAGE_FUNCTIONS_H
