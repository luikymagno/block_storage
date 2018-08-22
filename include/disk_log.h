#ifndef DISK_LOG_H
#define DISK_LOG_H

#include "block_storage.h"

#include "disk_pmem.h"

#include <string>
#include <map>



class DiskLog {
private:

    Block<disk_pmem::LogBlock> block;
    BlockStorage<disk_pmem::LogBlock> storage;
    int offset;
    std::string path;

public:

     DiskLog(std::string path_);
     int getOffSet();
     Block<disk_pmem::LogBlock> read(int offset_);
     void write(const disk_pmem::LogBlock &logB);
};

#endif // DISK_LOG_H







