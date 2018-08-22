#ifndef WORKLOAD_GENERATOR_H
#define WORKLOAD_GENERATOR_H

#include <vector>
#include <set>
#include <random>
#include <cmath>

template <typename DBType, typename DBObjType>
class WorkloadGenerator
{
public:
    WorkloadGenerator(double skew_factor,int skew_window,
                      int tx_size, DBType *db):
        skew_factor_(skew_factor),
        skew_window_(skew_window),
        tx_size_(tx_size),
        r_(),
        mt_(r_()),
        exp_(skew_factor_),
        advance_counter_(0),
        newest_(1),
        db_(db) {}

    virtual ~WorkloadGenerator() {}

    void GenerateTx() {

    std::set<DBObjType> tx;
    do {
        int obj_id = 0;
        do {
            obj_id = floor(-exp_(mt_)) + newest_;
        } while (obj_id < 0);
        tx.insert(obj_id);
    } while (tx.size() < std::min(tx_size_, newest_));
    advance_counter_++;
    if (advance_counter_ == skew_window_) {
        advance_counter_ = 0;
        newest_++;
    }
    db_->executeTransaction(tx);
}

private:
    int num_entries_;
    double skew_factor_;
    int skew_window_;
    int tx_size_;
    std::random_device r_;
    std::mt19937 mt_;
    std::exponential_distribution<double> exp_;
    int advance_counter_;
    int newest_;
    DBType *db_;
};

#endif // WORKLOAD_GENERATOR_H
