//
// Created by jan on 06.04.19.
//

#include "PrimeDecider.h"

#include "Common/Util.h"

#include <algorithm>
#include <iostream>

template <typename T>
std::pair<T, T>
PrimeDecider<T>::decouple(T x)
{
  T powerOf2 = 1;
  T i = 0;
  while (x % 2 == 0)
  {
    powerOf2 *= 2;
    x /= 2;
    ++i;
  }
  return {x, i};
}

template <typename T>
bool
PrimeDecider<T>::isComposite(T x, std::pair<T, T> decoupled, T witness)
{
  T temp = fast_exp(witness, decoupled.first, x);
  if (temp == 1 || temp == x - 1)
  {
    return false;
  }
  for (T i = decoupled.second - 2; i >= 0; --i)
  {
    temp = ((__int128) temp * temp) % x;
    if (temp == x - 1)
      return false;
  }
  return true;
}

template <typename T>
bool
PrimeDecider<T>::isPrime(T x)
{
  std::pair<T, T> decoupled = decouple(x - 1);
  //witnessesSmall/Big
  return std::any_of(std::begin(witnesses), std::end(witnesses), [x](T a) { return x == a; }) ||
         std::none_of(std::begin(witnesses), std::end(witnesses), [x, decoupled](T a) {
           return isComposite(x, decoupled, a);
         });
}

template class PrimeDecider<int32_t>;
template class PrimeDecider<int64_t>;
template class PrimeDecider<__int128>;
