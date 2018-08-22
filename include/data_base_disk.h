#ifndef DATA_BASE_DISK_H
#define DATA_BASE_DISK_H

#include "data_base.h"
#include "disk_log.h"

#include <memory>
#include <string>

class DataBaseDisk: public DataBase {
private:

    std::map<int,double> dataBase;
    DiskLog diskLog;
    int indexBlock;
    disk_pmem::LogBlock logB;
    int tx_id;

public:

    DataBaseDisk(std::string path);
    void executeTransaction(const std::set<double> &tx);
    std::map<int,double> getDataBase();
    void recover();
};

#endif // DATA_BASE_DISK_H
