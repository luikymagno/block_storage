#include "workload_generator.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>

std::vector<std::set<int>> WorkloadGenerator::Generate(int num_entries,
                                                        double skew_factor,
                                                        int skew_window,
                                                        int tx_size) {
    std::random_device r;
    std::mt19937 mt(r());
    std::exponential_distribution<double> exp(skew_factor);

    int advance_counter = 0, newest = 1;

    std::vector<std::set<int>> txs;
    for (int i=0; i < num_entries; i++) {
        std::set<int> tx;
        do {
            int obj_id = 0;
            do {
                obj_id = floor(-exp(mt))+ newest;
            } while (obj_id < 0);
            tx.insert(obj_id);
        } while (tx.size() < std::min(tx_size, newest));
        txs.push_back(tx);
        advance_counter++;
        if (advance_counter == skew_window) {
            advance_counter = 0;
            newest++;
        }
    }
    return txs;
}
