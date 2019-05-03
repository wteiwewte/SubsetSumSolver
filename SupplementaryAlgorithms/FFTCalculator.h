//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_FFTSOLVER_H
#define SUBSET_SUM_SOLVER_FFTSOLVER_H

#include "Common/ConstantsAndTypes.h"

#include <complex>
#include <vector>

#include <boost/multiprecision/float128.hpp>
#include <boost/math/constants/constants.hpp>
// using boost::math::constants::pi;
#include <boost/multiprecision/cpp_dec_float.hpp>

//using FloatingType = boost::multiprecision::float128;
//using FloatingType = boost::multiprecision::cpp_dec_float_50;
//using FloatingType = boost::multiprecision::cpp_dec_float_100;
using FloatingType = long double;

template<typename T>
class FFTCalculator
{
    static constexpr Int N_MAX = static_cast<const Int>(2e6);
//    static constexpr FloatingType PI = 3.14159265358979323;
    static inline FloatingType PI = boost::math::constants::pi<FloatingType>();

    static inline std::complex<FloatingType> A[N_MAX], B[N_MAX], C[N_MAX];

    static void divide(std::complex<FloatingType>* tab, T size);
    static void fft(std::complex<FloatingType>* tab, T size);
    static void ifft(std::complex<FloatingType>* tab, T size);
public:
    static void multiply(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result);
};


#endif //SUBSET_SUM_SOLVER_FFTSOLVER_H
