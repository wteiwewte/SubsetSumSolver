//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_FFTSOLVER_H
#define SUBSET_SUM_SOLVER_FFTSOLVER_H

#include "Common/ConstantsAndTypes.h"
#include "DataModel/FormalPowerSeries.h"
#include "DataModel/Zp.h"

#include <boost/math/constants/constants.hpp>

#include <array>
#include <complex>

using FloatingType = long double;

template <typename T>
class FFTCalculator
{
  using complex_type = std::complex<FloatingType>;
  constexpr static Int N_MAX = static_cast<const Int>(2e6);
  constexpr static FloatingType PI = boost::math::constants::pi<FloatingType>();

  static inline std::array<complex_type, N_MAX> A, B;

  static void divide(complex_type* tab, const std::size_t size);
  static void fft(complex_type* tab, const std::size_t size);
  static void ifft(complex_type* tab, const std::size_t size);

public:
  static FormalPowerSeries<T>
  multiply(const FormalPowerSeries<T>& x, const FormalPowerSeries<T>& y);
  static FormalPowerSeries<Zp<T>>
  multiply(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y);
  static FormalPowerSeries<Zp<T>>
  multiplyLessOverflow(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y);
};

#endif //SUBSET_SUM_SOLVER_FFTSOLVER_H
