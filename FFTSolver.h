//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_FFTSOLVER_H
#define SUBSET_SUM_SOLVER_FFTSOLVER_H

#include "ConstantsAndTypes.h"

#include <complex>
#include <vector>

template<typename T>
class FFTSolver
{
    static constexpr Int N_MAX = static_cast<const Int>(2e6);
    static constexpr double PI = 3.14159265358979323;

    static inline std::complex<double> A[N_MAX], B[N_MAX], C[N_MAX];

    static void divide(std::complex<double>* tab, T size);
    static void fft(std::complex<double>* tab, T size);
    static void ifft(std::complex<double>* tab, T size);
public:
    static void multiply(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result);
};


#endif //SUBSET_SUM_SOLVER_FFTSOLVER_H
