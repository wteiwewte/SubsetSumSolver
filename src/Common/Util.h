//
// Created by jan on 06.04.19.
//

#ifndef SUBSET_SUM_SOLVER_UTIL_H
#define SUBSET_SUM_SOLVER_UTIL_H

#include "Common/ConstantsAndTypes.h"
#include "DataModel/Zp.h"

#include <chrono>
#include <iostream>
#include <type_traits>

template <typename T>
constexpr T
getPowerOf2GreaterThan(const T atleast)
{
  std::size_t temp = 1;
  while (temp < atleast)
    temp <<= 1;
  return temp;
}

template <typename T>
constexpr T
fast_exp(const T base, const T exp, const T modulo)
{
  if (exp == 0)
    return 1;

  if (exp % 2 == 0)
  {
    T half = fast_exp(base, exp / 2, modulo);
    return (half * half) % modulo;
  }
  else
  {
    T half = fast_exp(base, exp / 2, modulo);
    return ((((half * half) % modulo) * base)) % modulo;
  }
}

template <typename T>
constexpr T
fast_exp(const T base, const T exp)
{
  if (exp == 0)
    return 1;

  if (exp % 2 == 0)
  {
    T half = fast_exp(base, exp / 2);
    return ((__int128) half * half);
  }
  else
  {
    T half = fast_exp(base, exp / 2);
    return ((__int128) ((__int128) half * half) * base);
  }
}

template <typename T>
constexpr Zp<T>
fast_exp(const Zp<T> base, const Zp<T> exp)
{
  if (exp == 0)
    return Zp<T>(1);

  if (exp % 2 == 0)
  {
    Zp<T> half = fast_exp(base, exp / 2);
    return half * half;
  }
  else
  {
    Zp<T> half = fast_exp(base, exp / 2);
    return (half * half) * base;
  }
}

template <typename Func, typename TimeRep = std::chrono::milliseconds>
std::chrono::duration<double>
executeAndMeasureTime(Func f)
{
  auto start = std::chrono::steady_clock::now();
  f();
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<TimeRep>(end - start);
}

template <typename T>
std::ostream&
operator<<(std::ostream& out, const std::vector<T>& x)
{
  for (auto t : x)
    out << (long long int) t << ' ';
  return out;
}

template <typename T>
std::ostream&
operator<<(std::ostream& out, const __int128& x)
{
  out << (int64_t) x;
  return out;
}

#endif //SUBSET_SUM_SOLVER_UTIL_H
