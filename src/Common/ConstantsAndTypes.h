//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
#define SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H

#include <array>
#include <string_view>

enum ExponentCalculationPolicy : uint8_t
{
  DIVIDE_AND_CONQUER = 0,
  NEWTONS_ITERATIVE_METHOD
};

enum FormalSeriesMultiplicationPolicy : uint8_t
{
  FFT = 0,
  NTT,
  TRIVIAL,
};

constexpr std::array<std::string_view, 2> exponentPolicyName = {"DIVIDE_AND_CONQUER",
                                                                "NEWTONS_ITERATIVE_METHOD"};

constexpr std::array<std::string_view, 3> multPolicyName = {"FFT", "NTT", "TRIVIAL"};
using Int = int64_t;

constexpr bool OUTPUT_TIME = true;
constexpr bool OUTPUT_TEST_SIZE = true;
constexpr bool CHECK_RECIPROCAL = true;

constexpr FormalSeriesMultiplicationPolicy FORMAL_SERIES_MULT_POLICY = NTT;
constexpr bool NTT_CHECK_OVERFLOW_CHECK = false;

template <typename T>
struct smaller { };

template <>
struct smaller<int32_t> { typedef int32_t type; };

template <>
struct smaller<int64_t> { typedef int32_t type; };

template <>
struct smaller<__int128> { typedef int64_t type; };

template <typename T>
using smaller_t = typename smaller<T>::type;

template <typename T>
struct bigger { };

template <>
struct bigger<int32_t> { typedef int64_t type; };

template <>
struct bigger<int64_t> { typedef __int128 type; };

template <>
struct bigger<__int128> { typedef __int128 type; };

template <typename T>
using bigger_t = typename bigger<T>::type;

#endif //SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
