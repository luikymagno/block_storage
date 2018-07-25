#include "blkstorage.h"

namespace blkstorage {

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
