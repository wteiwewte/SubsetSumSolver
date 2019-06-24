//
// Created by jan on 07.04.19.
//

#include "FFTCalculator.h"
#include "Common/Util.h"

#include <algorithm>

namespace
{
template<typename FloatingType>
constexpr bool less(FloatingType a, FloatingType b)
{
    return a - b > std::numeric_limits<FloatingType>::epsilon();
}
}

template<typename T>
void FFTCalculator<T>::multiply(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& result)
{
    std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

    std::fill(std::begin(A), std::begin(A) + adjustedLength, 0.0);
    std::fill(std::begin(B), std::begin(B) + adjustedLength, 0.0);

    std::copy(x.begin(), x.end(), std::begin(A));
    std::copy(y.begin(), y.end(), std::begin(B));

    fft(A, adjustedLength);
    fft(B, adjustedLength);

    for(std::size_t i = 0 ; i < adjustedLength; ++i)
        A[i] = A[i] * B[i];

    ifft(A, adjustedLength);

    result.resize(x.size() + y.size() - 1);
    for(std::size_t i = 0 ; i < result.size(); ++i )
        result[i] = std::round(A[i].real());
}


template<typename T>
void FFTCalculator<T>::multiplyNoOverflow(const std::vector<T> &x, const std::vector<T> &y, std::vector<T> &result, const T MOD) {
    std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

    std::vector<FloatingType > tempResult(x.size() + y.size() - 1, 0.0);

    const FloatingType sqrtMOD = std::sqrt((FloatingType) MOD);
//    assert(std::round(sqrtMOD * sqrtMOD) == MOD);

    auto addMult2SubVectors = [&x, &y, &tempResult, adjustedLength](FloatingType factor, auto xCheck, auto yCheck) {
        std::fill(std::begin(A), std::begin(A) + adjustedLength, 0.0);
        std::fill(std::begin(B), std::begin(B) + adjustedLength, 0.0);

        for (std::size_t i = 0; i < x.size(); ++i)
            A[i] = xCheck(x[i]);

        for (std::size_t i = 0; i < y.size(); ++i)
            B[i] = yCheck(y[i]);

        fft(A, adjustedLength);
        fft(B, adjustedLength);

        for (std::size_t i = 0; i < adjustedLength; ++i)
            A[i] = A[i] * B[i];

        ifft(A, adjustedLength);

        for (std::size_t i = 0; i < tempResult.size(); ++i)
            tempResult[i] += (factor * A[i].real());
    };

    addMult2SubVectors(1.0,
            [sqrtMOD](const T value){ return less(static_cast<FloatingType>(value), sqrtMOD) ? value : 0.0; },
           [sqrtMOD](const T value){ return less(static_cast<FloatingType>(value), sqrtMOD) ? value : 0.0; });

    addMult2SubVectors(sqrtMOD,
            [sqrtMOD](const T value){ return !less(static_cast<FloatingType>(value), sqrtMOD) ? static_cast<FloatingType>(value)/sqrtMOD : 0.0; },
           [sqrtMOD](const T value){ return less(static_cast<FloatingType>(value), sqrtMOD) ? value : 0.0; });

    addMult2SubVectors(sqrtMOD,
            [sqrtMOD](const T value){ return less(static_cast<FloatingType>(value), sqrtMOD) ? value : 0.0; },
           [sqrtMOD](const T value){ return !less(static_cast<FloatingType>(value), sqrtMOD) ? static_cast<FloatingType>(value)/sqrtMOD : 0.0; });

    addMult2SubVectors(sqrtMOD * sqrtMOD,
            [sqrtMOD](const T value){ return !less(static_cast<FloatingType>(value), sqrtMOD) ? static_cast<FloatingType>(value)/sqrtMOD : 0.0; },
           [sqrtMOD](const T value){ return !less(static_cast<FloatingType>(value), sqrtMOD) ? static_cast<FloatingType>(value)/sqrtMOD : 0.0; });

    result.resize(x.size() + y.size() - 1, 0);
    for(std::size_t i = 0 ; i < result.size(); ++i)
        result[i] = ((T) std::round(tempResult[i]) + MOD) % MOD;
}

template<typename T>
void FFTCalculator<T>::divide(complex_type *tab, std::size_t size)
{
    auto* odd_elements = new complex_type[size/2];
    for(std::size_t i = 0; i < size/2; i++)
        odd_elements[i] = tab[i*2 + 1];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i] = tab[i*2];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i + size/2] = odd_elements[i];
    delete[] odd_elements;
}

template<typename T>
void FFTCalculator<T>::fft(complex_type *tab, std::size_t size)
{
    if( size < 2 ) return;
    auto w_n =  exp(complex_type(0.0, 2.0* PI/static_cast<FloatingType>(size)));
    auto exponent  = complex_type(1.0, 0.0);
    divide(tab, size);
    fft(tab, size/2);
    fft(tab + size/2, size/2);
    for(std::size_t i = 0; i < size/2; i++) {
        auto t = tab[i];
        tab[i] += exponent* tab[i+size/2];
        tab[i + size/2] = t - exponent * tab[i + size/2];
        exponent *= w_n;
    }
}

template<typename T>
void FFTCalculator<T>::ifft(complex_type *tab, std::size_t size)
{
    transform(tab, tab + size, tab, [](const complex_type& c){
        return conj(c);
    } );

    fft(tab, size);

    transform(tab, tab + size, tab, [&](const complex_type& c){
        auto conj_c = conj(c);
        conj_c /= static_cast<FloatingType>(size);
        return conj_c;
    });
}


template class FFTCalculator<Int>;