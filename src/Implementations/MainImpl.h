//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
#define SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H

#include "Common/ConstantsAndTypes.h"
#include "DataModel/FormalPowerSeries.h"
#include "DataModel/Zp.h"
#include "Implementations/SubsetSumImpl.h"

template <typename T, ExponentCalculationPolicy expPolicy>
class MainImpl : public SubsetSumImpl<T>
{
public:
  MainImpl() = default;
  explicit MainImpl(const SubsetSumImpl<T>& otherImpl);
  void solve() override;

private:
  using SubsetSumImpl<T>::_tab;
  using SubsetSumImpl<T>::_size;
  using SubsetSumImpl<T>::_target;
  using SubsetSumImpl<T>::_result;

  static T getRandomPrimeFromRange(const std::pair<T, T>& range);
  void computeLog();
  void init() override;

  FormalPowerSeries<Zp<T>> _ln;
  FormalPowerSeries<Zp<T>> _g;
};

#endif //SUBSET_SUM_SOLVER_DUMMYIMPLEMENTATION_H
