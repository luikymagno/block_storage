#ifndef TEST_H
#define TEST_H

#include <set>
#include <vector>

class Test {
public:
    Test() {}
    void performTransactions(const char *path,
                             bool use_pmem, size_t num_txs);
};

#endif // TEST_H
