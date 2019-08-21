//
// Created by jan on 20.04.19.
//

#include "Zp.h"

#include "Common/Util.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <cassert>

namespace
{
constexpr std::size_t INVERSIONS_THRESHOLD = 2 * 1000 * 1000;
}

template <typename T>
void
Zp<T>::init(const T prime, const std::size_t inversionsToInit)
{
  static_assert(std::is_integral_v<T>, "T isn't integral type!");
  assert(PrimeDecider<T>::isPrime(prime));
  p = prime;
  initialized = true;
  if (inversionsToInit < INVERSIONS_THRESHOLD)
  {
    inversions_mod_p.resize(inversionsToInit + 1, T(0));
    for (std::size_t i = 1; i <= inversionsToInit; ++i)
      inversions_mod_p[i] = fast_exp<T>(i, p - 2, p);
  }
}

template <typename T>
Zp<T>
Zp<T>::getInversion(const Zp<T> value)
{
  return value._value < (T) inversions_mod_p.size()
             ? Zp<T>(inversions_mod_p[value._value], init_straightforward_value)
             : Zp<T>(fast_exp(value._value, p - 2, p), init_straightforward_value);
}

template class Zp<int32_t>;
template class Zp<int64_t>;
template class Zp<__int128>;
