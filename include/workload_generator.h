#ifndef WORKLOAD_GENERATOR_H
#define WORKLOAD_GENERATOR_H

#include <vector>
#include <set>

class WorkloadGenerator
{
public:
    WorkloadGenerator() {}
    virtual ~WorkloadGenerator() {}
    std::vector<std::set<int>> Generate(int num_entries, double skew_factor,
                                        int skew_window, int tx_size);
private:

};

#endif // WORKLOAD_GENERATOR_H
