//
// Created by jan on 20.04.19.
//

#ifndef SUBSET_SUM_SOLVER_DYNAMICPROGRAMMINGIMPL_H
#define SUBSET_SUM_SOLVER_DYNAMICPROGRAMMINGIMPL_H

#include "SubsetSumImpl.h"

#include <memory>

template<typename T>
class DynamicProgrammingImpl : public SubsetSumImpl<T> {
public:
    explicit DynamicProgrammingImpl(const SubsetSumImpl<T>& otherImpl);
    void solve() override;
    ~DynamicProgrammingImpl();
private:
    using SubsetSumImpl<T>::_tab;
    using SubsetSumImpl<T>::_size;
    using SubsetSumImpl<T>::_target;
    using SubsetSumImpl<T>::_result;

    void init() override;

    bool** _doesSumExist;
};




#endif //SUBSET_SUM_SOLVER_DYNAMICPROGRAMMINGIMPL_H
