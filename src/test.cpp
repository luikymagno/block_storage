#include "test.h"

#include "data_base_disk.h"
#include "data_base_pmem.h"
#include "disk_pmem.h"
#include "workload_generator.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>

void Test::performTransactions(const char* path, bool use_pmem, size_t num_txs) {
    if(use_pmem) {
        DataBasePmem base(path);
        WorkloadGenerator<DataBasePmem, double> wg(0.005, 10, disk_pmem::kTxSize, &base);
        auto start = std::chrono::high_resolution_clock::now();

        for(size_t i = 0; i < num_txs; i++) {
                wg.GenerateTx();
                std::cout << ((double)i*100.0f)/((double)num_txs) << "%" << std::endl;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        std::cout << "Time due to the write in pmem: " << elapsed.count()
                  << std::endl;
    }
    else {
        DataBaseDisk base(path);
        WorkloadGenerator<DataBaseDisk, double> wg(0.005, 10, disk_pmem::kTxSize, &base);
        auto start = std::chrono::high_resolution_clock::now();

        for(size_t i = 0; i < num_txs; i++) {
                wg.GenerateTx();
                std::cout << ((double)i*100.0f)/((double)num_txs) << "%" << std::endl;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        std::cout << "Time due to the write in disk: " << elapsed.count()
                  << std::endl;
    }
}
