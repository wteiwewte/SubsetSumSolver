//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
#define SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H

#include "SubsetSumImpl.h"

template<typename T>
class DummyImplementation : public SubsetSumImpl<T> {
public:
    void solve() override;
private:
    using SubsetSumImpl<T>::_tab;
    using SubsetSumImpl<T>::_size;
    using SubsetSumImpl<T>::_target;
    using SubsetSumImpl<T>::_result;

    static T getRandomPrimeFromRange(std::pair<T, T> range);
    void compute(std::size_t l, std::size_t r);
    void init() override;

    std::vector<T> _inversions_mod_p;
    std::vector<T> _ln;
    std::vector<T> _g;
    T _p;
};


#endif //SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
