#include "data_base_disk.h"
#include "disk_pmem.h"

#include <iostream>

DataBaseDisk::DataBaseDisk(std::string path): diskLog(path), tx_id(0), indexBlock(0) {}

void DataBaseDisk::executeTransaction(const std::set<double> &tx) {
    int counter = 0;

    for (auto& obj:tx) {
        disk_pmem::LogUpdate logU;
        logU.obj_id = obj;
        logU.tx_id = tx_id;
        (logB.entries[indexBlock]).updates[counter++] = logU;
    }
    if(counter < disk_pmem::kTxSize) {
        for(int i = counter; i < disk_pmem::kTxSize; i++ ) {
            (logB.entries[indexBlock]).updates[i].obj_id = -1;
            (logB.entries[indexBlock]).updates[i].tx_id = -1;
        }
    }
    disk_pmem::LogCommit logC;

    logC.tx_id = tx_id;
    (logB.entries[indexBlock++]).commit = logC;
    tx_id++;
    if(indexBlock == disk_pmem::kNumLogEntries) {
        indexBlock = 0;
        diskLog.write(logB);
    }
}

void DataBaseDisk::recover() {
    dataBase.clear();

    int offsetNumber = diskLog.getOffSet();
    Block<disk_pmem::LogBlock> block;
    for(int i = 0; i< offsetNumber; i++) {
        block =  diskLog.read(i);
        logB = block.data();
        for(auto& entry:logB.entries) {
            for(auto& update:entry.updates) {
                if(update.tx_id != -1) {
//                    dataBase[update.obj_id] = update.img_after;
                }
            }
        }
    }

}

std::map<int,double> DataBaseDisk::getDataBase() {
    return dataBase;
}

















