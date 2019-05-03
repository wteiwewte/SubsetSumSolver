//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
#define SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H

#include <boost/multiprecision/cpp_int.hpp>

//using Int = long long int;
//using Int = boost::multiprecision::int128_t;
using Int = __int128;

constexpr bool OUTPUT_TIME = true;
constexpr bool OUTPUT_TEST_SIZE = true;
constexpr bool CHECK_RECIPROCAL = true;

enum class ExponentCalculationPolicy {
    DIVIDE_AND_CONQUER = 0,
    NEWTONS_ITERATIVE_METHOD
};


enum class FormalSeriesMultiplicationPolicy{
    FFT = 0,
    MODULAR_FFT,
    TRIVIAL,
};

#endif //SUBSET_SUM_SOLVER_CONSTANTSANDTYPES_H
