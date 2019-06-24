//
// Created by jan on 19.04.19.
//

#ifndef SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H
#define SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H

#include <vector>

template<typename T>
class FormalPowerSeries : public std::vector<T> {
};

template<typename T>
void reciprocal(const FormalPowerSeries<T>& input, FormalPowerSeries<T>& result);

#endif //SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H
