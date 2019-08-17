//
// Created by jan on 19.04.19.
//

#ifndef SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H
#define SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H

#include "Common/ConstantsAndTypes.h"

#include <vector>

template<typename T>
class FormalPowerSeries : public std::vector<T> {
public:
  using std::vector<T>::operator[];
  using std::vector<T>::size;
  using std::vector<T>::resize;
  using std::vector<T>::vector;

  void releaseMemory();

  template <typename U>
  friend FormalPowerSeries<U> operator*(const FormalPowerSeries<U>& a,
                                        const FormalPowerSeries<U>& b);
  template <typename U>
  friend FormalPowerSeries<U> operator-(const FormalPowerSeries<U>& a,
                                        const FormalPowerSeries<U>& b);
};

template<typename T>
FormalPowerSeries<T> derivative(const FormalPowerSeries<T>& input);

template<typename T>
FormalPowerSeries<T> reciprocal(const FormalPowerSeries<T>& input);

template<typename T>
FormalPowerSeries<T> log(const FormalPowerSeries<T>& input);

template<typename T, ExponentCalculationPolicy expPolicy>
FormalPowerSeries<T> exp(const FormalPowerSeries<T>& input);

#endif //SUBSET_SUM_SOLVER_FORMALPOWERSERIES_H
