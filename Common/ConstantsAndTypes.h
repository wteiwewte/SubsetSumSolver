//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
#define SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H

#include <string_view>

#include <boost/multiprecision/cpp_int.hpp>

//using Int = long long int;
//using Int = boost::multiprecision::int128_t;
using Int = __int128;
//using Int = int64_t;
using LL = long long int;

constexpr bool OUTPUT_TIME = true;
constexpr bool OUTPUT_TEST_SIZE = true;
constexpr bool CHECK_RECIPROCAL = true;

enum ExponentCalculationPolicy : uint8_t
{
    DIVIDE_AND_CONQUER = 0,
    NEWTONS_ITERATIVE_METHOD
};

constexpr std::array<std::string_view, 2> exponentPolicyName = {"DIVIDE_AND_CONQUER", "NEWTONS_ITERATIVE_METHOD"};

enum FormalSeriesMultiplicationPolicy : uint8_t
{
    FFT = 0,
    NTT,
    TRIVIAL,
};

constexpr std::array<std::string_view, 3> multPolicyName = {"FFT", "NTT", "TRIVIAL"};

constexpr FormalSeriesMultiplicationPolicy FORMAL_SERIES_MULT_POLICY = NTT;

#endif //SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
