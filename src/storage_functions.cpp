#include "storage_functions.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


std::shared_ptr<int> storage_functions::MakeFileRAII(const std::string &path) {
    return MakeFileRAIILinux(path);
}

bool storage_functions::FileExists(const std::string &path) {
    return FileExistsLinux(path);
}

BlkStgStatus storage_functions::RemoveFile(const std::string &path) {
    return RemoveFileLinux(path);
}

std::shared_ptr<int> storage_functions::MakeFileRAIILinux(const std::string &path) {
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        return nullptr;
    }
    return std::shared_ptr<int>(new int(fd), [](int *fd) {
        close(*fd);
        delete fd;
    });
}

bool storage_functions::FileExistsLinux(const std::string &path) {
    return access(path.c_str(), F_OK ) != -1;
}

BlkStgStatus storage_functions::RemoveFileLinux(const std::string &path) {
    return (remove(path.c_str()) != -1) ? kNoError : kFileError;
}


