//
// Created by jan on 19.04.19.
//

#include "DataModel/FormalPowerSeries.h"

#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "DataModel/Zp.h"
#include "SupplementaryAlgorithms/FFTCalculator.h"
#include "SupplementaryAlgorithms/NTTCalculator.h"

#include <mutex>
#include <thread>

template <typename T>
void
FormalPowerSeries<T>::releaseMemory()
{
  FormalPowerSeries<T>().swap(*this);
}

template <typename U>
FormalPowerSeries<U> operator*(const FormalPowerSeries<U>& a, const FormalPowerSeries<U>& b)
{
  if (a.empty() || b.empty())
    return {};
  if constexpr (FORMAL_SERIES_MULT_POLICY == TRIVIAL)
    return bruteForceMultiply(a, b);
  else if constexpr (FORMAL_SERIES_MULT_POLICY == NTT)
    if constexpr (is_Zp<U>::value)
    {
      if constexpr (NTT_CHECK_OVERFLOW_CHECK)
        NTTCalculator<typename U::underlying_type>::multiplyWithOverflowCheck(a, b);
      return NTTCalculator<typename U::underlying_type>::multiply(a, b);
    }
    else
      return NTTCalculator<U>::multiply(a, b);
  else if constexpr (is_Zp<U>::value)
    return FFTCalculator<typename U::underlying_type>::multiplyLessOverflow(a, b);
  else
    return FFTCalculator<U>::multiply(a, b);
}

template <typename U>
FormalPowerSeries<U>
bruteForceMultiply(const FormalPowerSeries<U>& a, const FormalPowerSeries<U>& b)
{
  if (a.empty() || b.empty())
    return {};

  FormalPowerSeries<U> result(a.size() + b.size() - 1, U(0));

  for (std::size_t i = 0; i < a.size(); ++i)
    for (std::size_t j = 0; j < b.size(); ++j)
      result[i + j] += a[i] * b[j];

  return result;
}

template <typename T>
FormalPowerSeries<T>
derivative(const FormalPowerSeries<T>& input)
{
  FormalPowerSeries<T> result(input.size() - 1);
  for (std::size_t i = 0; i < input.size() - 1; ++i)
    result[i] = (i + 1) * input[i + 1];
  return result;
}

template <typename T>
FormalPowerSeries<T>
reciprocal(const FormalPowerSeries<T>& input)
{
  assert(input[0] != T(0));
  std::size_t bound = input.size();
  FormalPowerSeries<T> b(1, T(1) / input[0]);

  for (std::size_t i = 0; i + 1 < bound; i = (2 * i + 1))
  {
    FormalPowerSeries<T> inputPrefix(input.begin(),
                                     input.begin() + std::min(input.size(), 2 * i + 2));
    //      inputPrefix.resize(2 * i + 2, T(1));
    // temp = makeSeriesFromRange(input.begin(), input.begin() + std::min(input.size(), 2 * i + 2)) * (b * b)
    //return makeSeriesFromRange(temp.begin() + i + 1, temp.begin() + 2 * i + 2)
    FormalPowerSeries<T> partialResult = inputPrefix * (b * b);

    std::transform(std::make_move_iterator(partialResult.begin() + i + 1),
                   std::make_move_iterator(partialResult.begin() + 2 * i + 2),
                   std::back_inserter(b),
                   [](T&& value) { return -value; });

    assert(b.size() == 2 * i + 2);
  }
  assert(b.size() >= bound);
  FormalPowerSeries<T> result;
  std::move(b.begin(), b.begin() + bound, std::back_inserter(result));

  if constexpr (CHECK_RECIPROCAL)
  {
    FormalPowerSeries<T> test = input * result;
    test.resize(input.size());

    bool correct = true;
    correct &= test[0] == T(1);
    correct &= std::all_of(test.begin() + 1, test.end(), [](const T& val) { return val == T(0); });
    assert(correct);
  }

  assert(input.size() == result.size());

  return result;
}

template <typename T>
FormalPowerSeries<T>
log(const FormalPowerSeries<T>& input)
{
  assert(input[0] == T(1));
  FormalPowerSeries<T> result;
  if (input.size() == 1)
  {
    result.resize(1, T(0));
    return result;
  }

  const FormalPowerSeries<T> shiftedResult = derivative(input) * reciprocal(input);
  assert(shiftedResult.size() >= input.size());

  result.resize(input.size(), T(0));

  //return shift_with_inversions(derivative(input) * reciprocal(input)) *
  //std::rotate ? transform ?
  for (std::size_t i = 1; i < result.size(); ++i)
    result[i] = (T(1) / T(i)) * shiftedResult[i - 1];
  assert(input.size() == result.size());
  return result;
}

template <typename T, ExponentCalculationPolicy expPolicy>
FormalPowerSeries<T>
exp(const FormalPowerSeries<T>& input)
{
  assert(input[0] == T(0));
  FormalPowerSeries<T> result;
  if constexpr (expPolicy == DIVIDE_AND_CONQUER)
  {
    result.resize(input.size(), T(0));
    result[0] = T(1);

    std::function<void(std::size_t, std::size_t)> compute =
        [&compute, &input, &result](const std::size_t l, const std::size_t r) {
          if (l < r)
          {
            const std::size_t m = (l + r) / 2;
            compute(l, m);

            FormalPowerSeries<T> F(r - l + 1);
            for (std::size_t k = 0; k <= r - l; ++k)
              F[k] = k * input[k];

            FormalPowerSeries<T> G(m - l + 1);
            for (std::size_t j = 0; j <= m - l; ++j)
              G[j] = result[j + l];

            FormalPowerSeries<T> H = F * G;

            for (std::size_t i = m + 1; i <= r; ++i)
              result[i] += (T(1) / T(i)) * H[i - l];

            F.releaseMemory();
            G.releaseMemory();
            H.releaseMemory();

            compute(m + 1, r);
          }
        };

    compute(0, input.size() - 1);
  }
  else
  {
    result.resize(1, T(1));
    for (std::size_t i = 1; i < input.size(); i *= 2)
    {
      result.resize(std::min(input.size(), 2 * i), T(0));
      FormalPowerSeries<T> logRl = log(result);
      FormalPowerSeries<T> diffRlandP(result.size());
      // result -= result * (log(result) - input);
      for (std::size_t j = 0; j < diffRlandP.size(); ++j)
      {
        diffRlandP[j] = logRl[j] - input[j];
      }

      FormalPowerSeries<T> partialResult = result * diffRlandP;

      assert(std::all_of(partialResult.begin(), partialResult.begin() + i, [](const T& value) {
        return value == T(0);
      }));
      for (std::size_t j = i; j < std::min(input.size(), 2 * i); ++j)
        result[j] = result[j] - partialResult[j];
    }
  }
  return result;
}

#define INSTANTIATE_TEMPLATE_SERIES(type) \
  template class FormalPowerSeries<type>; \
  template FormalPowerSeries<type> bruteForceMultiply(const FormalPowerSeries<type>& a, const FormalPowerSeries<type>& b); \
  template FormalPowerSeries<type> derivative(const FormalPowerSeries<type>&); \
  template FormalPowerSeries<type> log(const FormalPowerSeries<type>&); \
  template FormalPowerSeries<type> reciprocal(const FormalPowerSeries<type>&); \
  template FormalPowerSeries<type> exp<type, DIVIDE_AND_CONQUER>(const FormalPowerSeries<type>&); \
  template FormalPowerSeries<type> exp<type, NEWTONS_ITERATIVE_METHOD>( \
      const FormalPowerSeries<type>&);

INSTANTIATE_TEMPLATE_SERIES(int32_t)
INSTANTIATE_TEMPLATE_SERIES(int64_t)
INSTANTIATE_TEMPLATE_SERIES(__int128)

INSTANTIATE_TEMPLATE_SERIES(Zp<int32_t>)
INSTANTIATE_TEMPLATE_SERIES(Zp<int64_t>)
INSTANTIATE_TEMPLATE_SERIES(Zp<__int128>)
