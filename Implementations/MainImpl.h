//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
#define SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H

#include "Common/ConstantsAndTypes.h"
#include "SubsetSumImpl.h"

template<typename T, ExponentCalculationPolicy expPolicy>
class MainImpl : public SubsetSumImpl<T> {
public:
    void solve() override;
private:
    using SubsetSumImpl<T>::_tab;
    using SubsetSumImpl<T>::_size;
    using SubsetSumImpl<T>::_target;
    using SubsetSumImpl<T>::_result;

    static T getRandomPrimeFromRange(std::pair<T, T> range);
    void reciprocal(const std::vector<T>& input, std::vector<T>& result);
    void log(const std::vector<T>& input, std::vector<T>& result);
    void exp(const std::vector<T>& input, std::vector<T>& result);
    void computeLog();
    void computeExp();
    template<FormalSeriesMultiplicationPolicy policy>
    void multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result);
    void init() override;

    std::vector<T> _inversions_mod_p;
    std::vector<T> _ln;
    std::vector<T> _g;
    T _p;
};


#endif //SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
