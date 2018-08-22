#include "disk_log.h"

#include <iostream>
#include <vector>

DiskLog::DiskLog(std::string path_) :block({}), storage(path_),
    offset(0), path(path_)  {
    storage.ClearFile();
}

int DiskLog::getOffSet() {
    return offset;
}

Block<disk_pmem::LogBlock> DiskLog::read(int offset_) {
    storage.ReadBlock(offset_,block);
    return block;
}

void DiskLog::write(const disk_pmem::LogBlock &logB) {
        block.SetData(logB);
        if (storage.WriteBlock(offset++, block) != blkstorage::kNoError) {
            std::cout << "There was an error during WriteBlock()." << std::endl;
        }
}











