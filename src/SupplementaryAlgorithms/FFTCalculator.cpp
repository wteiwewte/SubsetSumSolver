//
// Created by jan on 07.04.19.
//

#include "FFTCalculator.h"

#include "Common/Util.h"

#include <algorithm>

namespace
{
template <typename FloatingType>
constexpr bool
less(FloatingType a, FloatingType b)
{
  return a - b > std::numeric_limits<FloatingType>::epsilon();
}
} // namespace

template <typename T>
FormalPowerSeries<T>
FFTCalculator<T>::multiply(const FormalPowerSeries<T>& x, const FormalPowerSeries<T>& y)
{
  FormalPowerSeries<T> result;
  const std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

  std::fill(std::begin(A), std::begin(A) + adjustedLength, 0.0);
  std::fill(std::begin(B), std::begin(B) + adjustedLength, 0.0);

  std::copy(x.begin(), x.end(), std::begin(A));
  std::copy(y.begin(), y.end(), std::begin(B));

  fft(A.data(), adjustedLength);
  fft(B.data(), adjustedLength);

  for (std::size_t i = 0; i < adjustedLength; ++i)
    A[i] = A[i] * B[i];

  ifft(A.data(), adjustedLength);

  result.resize(x.size() + y.size() - 1);
  for (std::size_t i = 0; i < result.size(); ++i)
    result[i] = T(std::round(A[i].real()));
  return result;
}

template <typename T>
FormalPowerSeries<Zp<T>>
FFTCalculator<T>::multiply(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y)
{
  FormalPowerSeries<Zp<T>> result;
  const std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());
  std::fill(std::begin(A), std::begin(A) + adjustedLength, 0.0);
  std::fill(std::begin(B), std::begin(B) + adjustedLength, 0.0);

  std::copy(x.begin(), x.end(), std::begin(A));
  std::copy(y.begin(), y.end(), std::begin(B));

  fft(A.data(), adjustedLength);
  fft(B.data(), adjustedLength);

  for (std::size_t i = 0; i < adjustedLength; ++i)
    A[i] = A[i] * B[i];

  ifft(A.data(), adjustedLength);

  result.resize(x.size() + y.size() - 1);
  for (std::size_t i = 0; i < result.size(); ++i)
    result[i] = Zp(T(std::round(A[i].real())));
  return result;
}

template <typename T>
FormalPowerSeries<Zp<T>>
FFTCalculator<T>::multiplyLessOverflow(const FormalPowerSeries<Zp<T>>& x,
                                       const FormalPowerSeries<Zp<T>>& y)
{
  FormalPowerSeries<Zp<T>> result;

  const std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

  std::vector<FloatingType> tempResult(x.size() + y.size() - 1, 0.0);

  const FloatingType sqrtMOD = std::sqrt((FloatingType) Zp<T>::p);
  assert(std::round(sqrtMOD * sqrtMOD) == Zp<T>::p);

  auto addMult2SubVectors =
      [&x, &y, &tempResult, adjustedLength](FloatingType factor, auto xCheck, auto yCheck) {
        std::fill(std::begin(A), std::begin(A) + adjustedLength, 0.0);
        std::fill(std::begin(B), std::begin(B) + adjustedLength, 0.0);

        for (std::size_t i = 0; i < x.size(); ++i)
          A[i] = xCheck(x[i]);

        for (std::size_t i = 0; i < y.size(); ++i)
          B[i] = yCheck(y[i]);

        fft(A.data(), adjustedLength);
        fft(B.data(), adjustedLength);

        for (std::size_t i = 0; i < adjustedLength; ++i)
          A[i] = A[i] * B[i];

        ifft(A.data(), adjustedLength);

        for (std::size_t i = 0; i < tempResult.size(); ++i)
          tempResult[i] += (factor * A[i].real());
      };

  addMult2SubVectors(
      1.0,
      [sqrtMOD](const Zp<T> x) {
        return less(static_cast<FloatingType>(x.value()), sqrtMOD) ? x.value() : 0.0;
      },
      [sqrtMOD](const Zp<T> x) {
        return less(static_cast<FloatingType>(x.value()), sqrtMOD) ? x.value() : 0.0;
      });

  addMult2SubVectors(
      sqrtMOD,
      [sqrtMOD](const Zp<T> x) {
        return !less(static_cast<FloatingType>(x.value()), sqrtMOD)
                   ? static_cast<FloatingType>(x.value()) / sqrtMOD
                   : 0.0;
      },
      [sqrtMOD](const Zp<T> x) {
        return less(static_cast<FloatingType>(x.value()), sqrtMOD) ? x.value() : 0.0;
      });

  addMult2SubVectors(
      sqrtMOD,
      [sqrtMOD](const Zp<T> x) {
        return less(static_cast<FloatingType>(x.value()), sqrtMOD) ? x.value() : 0.0;
      },
      [sqrtMOD](const Zp<T> x) {
        return !less(static_cast<FloatingType>(x.value()), sqrtMOD)
                   ? static_cast<FloatingType>(x.value()) / sqrtMOD
                   : 0.0;
      });

  addMult2SubVectors(
      sqrtMOD * sqrtMOD,
      [sqrtMOD](const Zp<T> x) {
        return !less(static_cast<FloatingType>(x.value()), sqrtMOD)
                   ? static_cast<FloatingType>(x.value()) / sqrtMOD
                   : 0.0;
      },
      [sqrtMOD](const Zp<T> x) {
        return !less(static_cast<FloatingType>(x.value()), sqrtMOD)
                   ? static_cast<FloatingType>(x.value()) / sqrtMOD
                   : 0.0;
      });

  result.resize(x.size() + y.size() - 1, Zp<T>(0, init_straightforward_value));
  for (std::size_t i = 0; i < result.size(); ++i)
    result[i] = Zp<T>((T) std::round(tempResult[i]), take_remainder_and_fix_negatives);
  return result;
}

template <typename T>
void
FFTCalculator<T>::divide(complex_type* tab, const std::size_t size)
{
  auto* odd_elements = new complex_type[size / 2];
  for (std::size_t i = 0; i < size / 2; i++)
    odd_elements[i] = tab[i * 2 + 1];
  for (std::size_t i = 0; i < size / 2; i++)
    tab[i] = tab[i * 2];
  for (std::size_t i = 0; i < size / 2; i++)
    tab[i + size / 2] = odd_elements[i];
  delete[] odd_elements;
}

template <typename T>
void
FFTCalculator<T>::fft(complex_type* tab, const std::size_t size)
{
  if (size < 2)
    return;
  auto w_n = exp(complex_type(0.0, 2.0 * PI / static_cast<FloatingType>(size)));
  auto exponent = complex_type(1.0, 0.0);
  divide(tab, size);
  fft(tab, size / 2);
  fft(tab + size / 2, size / 2);
  for (std::size_t i = 0; i < size / 2; i++)
  {
    auto t = tab[i];
    tab[i] += exponent * tab[i + size / 2];
    tab[i + size / 2] = t - exponent * tab[i + size / 2];
    exponent *= w_n;
  }
}

template <typename T>
void
FFTCalculator<T>::ifft(complex_type* tab, const std::size_t size)
{
  transform(tab, tab + size, tab, [](const complex_type& c) { return conj(c); });

  fft(tab, size);

  transform(tab, tab + size, tab, [&](const complex_type& c) {
    auto conj_c = conj(c);
    conj_c /= static_cast<FloatingType>(size);
    return conj_c;
  });
}

template class FFTCalculator<int32_t>;
template class FFTCalculator<int64_t>;
template class FFTCalculator<__int128>;
