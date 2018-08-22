#ifndef DATA_BASE_PMEM_H
#define DATA_BASE_PMEM_H

#include "data_base.h"

#include "pmem_log.h"
#include <map>
#include <set>

class DataBasePmem: public DataBase  {
private:
    std::map<int,double> dataBase;
    const char* path;
    PmemLog pmemLog;
    int tx_id;

public:
    DataBasePmem(const char* path_);
    void executeTransaction(const std::set<double> &tx);
    std::map<int,double> getDataBase();
    void recover();
};
#endif // DATA_BASE_PMEM_H
