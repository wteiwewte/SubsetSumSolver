//
// Created by jan on 07.04.19.
//
#include "MainImpl.h"

#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "DataModel/FormalPowerSeries.h"
#include "SupplementaryAlgorithms/FFTCalculator.h"
#include "SupplementaryAlgorithms/NTTCalculator.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <DataModel/Zp.h>

#include <cassert>
#include <functional>
#include <random>

template <typename T, ExponentCalculationPolicy expPolicy>
MainImpl<T, expPolicy>::MainImpl(const SubsetSumImpl<T>& otherImpl) : SubsetSumImpl<T>(otherImpl)
{
  init();
}

template <typename T, ExponentCalculationPolicy expPolicy>
T
MainImpl<T, expPolicy>::getRandomPrimeFromRange(const std::pair<T, T>& range)
{
  static std::random_device rd;
  static std::mt19937 eng(rd());
  std::uniform_int_distribution<T> distr(range.first, range.second);

  T prime;
  while (!PrimeDecider<T>::isPrime((prime = distr(eng))))
    ;

  return prime;
}

template <typename T, ExponentCalculationPolicy expPolicy>
void
MainImpl<T, expPolicy>::solve()
{
  computeLog();
  _g = exp<Zp<T>, expPolicy>(_ln);
  _result = _g[_target] != Zp<T>(0);
}

template <typename T, ExponentCalculationPolicy expPolicy>
void
MainImpl<T, expPolicy>::computeLog()
{
  FormalPowerSeries<Zp<T>> powersOfA(_target + 1, Zp<T>(0));
  for (T i = 1; i <= _size; ++i)
    if (1 <= _tab[i] && _tab[i] <= _target)
      powersOfA[_tab[i]]++;

  for (T k = 1; k <= _target; ++k)
    powersOfA[k] %= Zp<T>::p;

  _ln.resize(_target + 1, Zp<T>(0));
  for (T k = 1; k <= _target; ++k)
    for (T j = 1; j <= _target / k; ++j)
      _ln[j * k] +=
          (powersOfA[k] * (((j - 1) % 2 == 1) ? -(Zp<T>(1) / Zp<T>(j)) : (Zp<T>(1) / Zp<T>(j))));
}

template <typename T, ExponentCalculationPolicy expPolicy>
void
MainImpl<T, expPolicy>::init()
{
  if constexpr (FORMAL_SERIES_MULT_POLICY == NTT)
  {
    const T p = NTTCalculator<smaller_t<T>>::initModRootOrd();
    assert(_target < p);
    Zp<T>::init(p, _target);
  }
  else
  {
    const T upperBound = std::min<__int128>(std::floor(std::sqrt(std::numeric_limits<T>::max())),
                                            fast_exp<__int128>(_target + _size, 3));
    const T p = getRandomPrimeFromRange({_target + 1, upperBound});
    //    Zp<T>::init(36389, _target);
    Zp<T>::init(p, _target);
  }
  std::cout << exponentPolicyName[expPolicy] << " " << Zp<T>::p << "<- prime\n";
}

template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD>;
