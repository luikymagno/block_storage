#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <set>
#include <vector>
#include <map>

#include "block_storage.h"
#include "blkstorage.h"
#include "data_base_disk.h"
#include "data_base_pmem.h"
#include "test.h"

constexpr size_t kPracticalPoolSize = 1800000000;
constexpr size_t kNumTx = kPracticalPoolSize/sizeof(disk_pmem::LogEntry);

using namespace std;

int main(int argc, char *argv[]) {
    Test test;
    test.performTransactions("/home/luiky/Documents/log", false, kNumTx);
    return 0;
}




