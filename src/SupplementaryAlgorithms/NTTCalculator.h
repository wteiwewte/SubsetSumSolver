//
// Created by jan on 22.06.19.
//

#ifndef SUBSET_SUM_SOLVER_NTTCALCULATOR_H
#define SUBSET_SUM_SOLVER_NTTCALCULATOR_H

#include "Common/ConstantsAndTypes.h"
#include "DataModel/FormalPowerSeries.h"
#include "DataModel/Zp.h"

#include <array>

constexpr int MOD_INT = 2013265921;
constexpr int ROOT_INT = 440564289;
constexpr int ORD_INT = 1 << 27;
constexpr long long int MOD_LONG_LONG = 9223372006790004737;
constexpr long long int ROOT_LONG_LONG = 2379743102665616301;
constexpr long long int ORD_LONG_LONG = 1 << 30;

template <typename T>
class NTTCalculator
{
  constexpr static Int N_MAX = static_cast<const Int>(2e6);
  static inline bool initialized = false;
public:
  static T initModRootOrd();
  static FormalPowerSeries<Zp<T>>
  multiply(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y);
  static FormalPowerSeries<T>
  multiply(const FormalPowerSeries<T>& x, const FormalPowerSeries<T>& y);
  static FormalPowerSeries<Zp<T>>
  multiplyWithOverflowCheck(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y);
};

#endif //SUBSET_SUM_SOLVER_NTTCALCULATOR_H
