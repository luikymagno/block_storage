#ifndef DATA_BASE_H
#define DATA_BASE_H

#include "disk_log.h"

#include <map>
#include <set>

class DataBase {
public:

    virtual void executeTransaction(const std::set<int> &tx) = 0;
    virtual std::map<int,double> getDataBase() = 0;
    virtual void recover() = 0;
};

#endif // DATA_BASE_H
