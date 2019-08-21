#include "SupplementaryAlgorithms/NTTCalculator.h"

#include "Common/Util.h"

#if __has_include(<boost/safe_numerics/safe_integer.hpp>)
#include <boost/safe_numerics/safe_integer.hpp>
#define have_safe_numerics 1
#else
#define have_safe_numerics 0
#endif

#include <algorithm>

namespace
{
template <typename T>
T MOD = T(0);
template <typename T>
T ROOT = T(0);
template <typename T>
T ORD = T(0);

enum class NTTKind
{
  NORMAL = 0,
  INVERSE
};

template <typename T, NTTKind kind>
T
getRoot(std::size_t size)
{
  T w_n = fast_exp<T>(ROOT<T>, ORD<T> / size, MOD<T>);
  if constexpr (kind == NTTKind::INVERSE)
    w_n = fast_exp(w_n, MOD<T> - 2, MOD<T>);
  return w_n;
}

template <typename T>
void
divide(T* tab, std::size_t size)
{
  auto* odd_elements = new T[size / 2];
  for (std::size_t i = 0; i < size / 2; i++)
    odd_elements[i] = tab[i * 2 + 1];
  for (std::size_t i = 0; i < size / 2; i++)
    tab[i] = tab[i * 2];
  for (std::size_t i = 0; i < size / 2; i++)
    tab[i + size / 2] = odd_elements[i];
  delete[] odd_elements;
}

template <typename T, NTTKind kind>
void
ntt(T* tab, std::size_t size)
{
  if (size < 2)
    return;
  T w_n = getRoot<T, kind>(size);
  T exponent = 1;
  divide(tab, size);
  ntt<T, kind>(tab, size / 2);
  ntt<T, kind>(tab + size / 2, size / 2);
  for (std::size_t i = 0; i < size / 2; i++)
  {
    T t = tab[i];
    tab[i] = (tab[i] + (((bigger_t<T>) exponent * tab[i + size / 2]) % MOD<T>) ) % MOD<T>;
    tab[i + size / 2] = (t - (((bigger_t<T>) exponent * tab[i + size / 2]) % MOD<T>) +MOD<T>) % MOD<T>;
    exponent = ((bigger_t<T>) exponent * w_n) % MOD<T>;
  }
}
} // namespace

template <typename T>
T
NTTCalculator<T>::initModRootOrd()
{
  if constexpr (sizeof(T) == sizeof(int32_t))
  {
    MOD<T> = MOD_INT;
    ROOT<T> = ROOT_INT;
    ORD<T> = ORD_INT;
  }
  else
  {
    MOD<T> = MOD_LONG_LONG;
    ROOT<T> = ROOT_LONG_LONG;
    ORD<T> = ORD_LONG_LONG;
  }
  initialized = true;
  return MOD<T>;
}

template <typename T>
FormalPowerSeries<Zp<T>>
NTTCalculator<T>::multiply(const FormalPowerSeries<Zp<T>>& x, const FormalPowerSeries<Zp<T>>& y)
{
  assert(initialized && MOD<smaller_t<T>> == Zp<T>::p);
  std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());
  std::vector<smaller_t<T>> X(adjustedLength);
  std::vector<smaller_t<T>> Y(adjustedLength);

  std::fill(std::begin(X), std::begin(X) + adjustedLength, 0);
  std::fill(std::begin(Y), std::begin(Y) + adjustedLength, 0);

  std::copy(x.begin(), x.end(), std::begin(X));
  std::copy(y.begin(), y.end(), std::begin(Y));

  ntt<smaller_t<T>, NTTKind::NORMAL>(X.data(), adjustedLength);
  ntt<smaller_t<T>, NTTKind::NORMAL>(Y.data(), adjustedLength);

  smaller_t<T> inversionLength = fast_exp<smaller_t<T>>(adjustedLength, MOD<smaller_t<T>> - 2, MOD<smaller_t<T>>);
  for (std::size_t i = 0; i < adjustedLength; ++i)
    X[i] = ((T) (((T) X[i] * Y[i]) % MOD<smaller_t<T>>) *inversionLength) % MOD<smaller_t<T>>;

  ntt<smaller_t<T>, NTTKind::INVERSE>(X.data(), adjustedLength);

  FormalPowerSeries<Zp<T>> result(x.size() + y.size() - 1);
  for (std::size_t i = 0; i < result.size(); ++i)
    result[i] = Zp<T>(X[i]);
  return result;
}


template <typename T>
FormalPowerSeries<T>
NTTCalculator<T>::multiply(const FormalPowerSeries<T>& x, const FormalPowerSeries<T>& y)
{
  assert(initialized);
  FormalPowerSeries<T> result;
  std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

  std::vector<T> X(adjustedLength);
  std::vector<T> Y(adjustedLength);

  std::fill(std::begin(X), std::begin(X) + adjustedLength, 0);
  std::fill(std::begin(Y), std::begin(Y) + adjustedLength, 0);

  std::copy(x.begin(), x.end(), std::begin(X));
  std::copy(y.begin(), y.end(), std::begin(Y));

  ntt<T, NTTKind::NORMAL>(X.data(), adjustedLength);
  ntt<T, NTTKind::NORMAL>(Y.data(), adjustedLength);

  T inversionLength = fast_exp<T>(adjustedLength, MOD<T> - 2, MOD<T>);
  for (std::size_t i = 0; i < adjustedLength; ++i)
    X[i] = ((bigger_t<T>) (((bigger_t<T>) X[i] * Y[i]) % MOD<T>) * inversionLength) % MOD<T>;

  ntt<T, NTTKind::INVERSE>(X.data(), adjustedLength);

  result.resize(x.size() + y.size() - 1);
  for (std::size_t i = 0; i < result.size(); ++i)
    result[i] = X[i];
  return result;
}

#if have_safe_numerics == 1

template <>
struct smaller<boost::safe_numerics::safe<int32_t>> { typedef boost::safe_numerics::safe<int32_t> type; };

template <>
struct smaller<boost::safe_numerics::safe<int64_t>> { typedef boost::safe_numerics::safe<int32_t> type; };

template <>
struct smaller<boost::safe_numerics::safe<__int128>> { typedef boost::safe_numerics::safe<int64_t> type; };


template <>
struct bigger<boost::safe_numerics::safe<int32_t>> { typedef boost::safe_numerics::safe<int64_t> type; };

template <>
struct bigger<boost::safe_numerics::safe<int64_t>> { typedef boost::safe_numerics::safe<__int128> type; };

template <>
struct bigger<boost::safe_numerics::safe<__int128>> { typedef boost::safe_numerics::safe<__int128> type; };


template <typename T>
FormalPowerSeries<Zp<T>>
NTTCalculator<T>::multiplyWithOverflowCheck(const FormalPowerSeries<Zp<T>>& x,
                                            const FormalPowerSeries<Zp<T>>& y)
{
  using safeT = boost::safe_numerics::safe<T>;
  NTTCalculator<smaller_t<safeT>>::initModRootOrd();
  assert(initialized);
  const std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

  std::vector<smaller_t<safeT>> Asafe(adjustedLength);
  std::vector<smaller_t<safeT>> Bsafe(adjustedLength);

  std::fill(std::begin(Asafe), std::end(Asafe), 0);
  std::fill(std::begin(Bsafe), std::end(Bsafe), 0);

  std::copy(x.begin(), x.end(), std::begin(Asafe));
  std::copy(y.begin(), y.end(), std::begin(Bsafe));

  ntt<smaller_t<safeT>, NTTKind::NORMAL>(Asafe.data(), adjustedLength);
  ntt<smaller_t<safeT>, NTTKind::NORMAL>(Bsafe.data(), adjustedLength);

  auto inversionLength = fast_exp<smaller_t<safeT>>(adjustedLength, MOD<smaller_t<safeT>> - 2, MOD<smaller_t<safeT>>);
  for (std::size_t i = 0; i < adjustedLength; ++i)
    Asafe[i] = ((safeT) (((safeT) Asafe[i] * Bsafe[i]) % MOD<smaller_t<safeT>>) *inversionLength) % MOD<smaller_t<safeT>>;

  ntt<smaller_t<safeT>, NTTKind::INVERSE>(Asafe.data(), adjustedLength);
  FormalPowerSeries<Zp<T>> result;
  return result;
}
#else
#endif

template class NTTCalculator<int32_t>;
template class NTTCalculator<int64_t>;
template class NTTCalculator<__int128>;
