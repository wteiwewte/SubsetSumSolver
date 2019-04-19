//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_SUBSETSUMSOLVER_H
#define SUBSET_SUM_SOLVER_SUBSETSUMSOLVER_H

#include "SubsetSumImpl.h"

#include <iostream>
#include <memory>
#include <experimental/propagate_const>

template<typename T>
class SubsetSumSolver;

template<typename T>
std::istream& operator>>(std::istream&, SubsetSumSolver<T>&);

template<typename T>
std::ostream& operator<<(std::ostream&, const SubsetSumSolver<T>&);

template<typename T>
class SubsetSumSolver
{
public:
    SubsetSumSolver(std::unique_ptr<SubsetSumImpl<T>>&&);
    void solve();
    bool verify() const;
    bool result() const;
private:
    friend std::istream& operator>> <T>(std::istream&, SubsetSumSolver<T>&);
    friend std::ostream& operator<< <T>(std::ostream&, const SubsetSumSolver<T>&);

    std::experimental::propagate_const<std::unique_ptr<SubsetSumImpl<T>>> _impl;
};

#endif //SUBSET_SUM_SOLVER_SUBSETSUMSOLVER_H
