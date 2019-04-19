//
// Created by jan on 07.04.19.
//

#include "FFTSolver.h"
#include "Util.h"

#include <algorithm>

template<typename T>
void FFTSolver<T>::multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result)
{
    T max_len = getPowerOf2GreaterThan(static_cast<T>(a.size() + b.size()));
    for(std::size_t i = 0 ; i < max_len ; ++i ) {
        A[i] = B[i] = C[i] = 0;
    }

    for(std::size_t i = 0 ; i < a.size(); ++i ){
        A[i] = a[i];
    }
    for(std::size_t i = 0 ; i < b.size(); ++i ){
        B[i] = b[i];
    }

    fft(A, max_len);
    fft(B, max_len);

    for(std::size_t i = 0 ; i < max_len; ++i )
        C[i] = A[i] * B[i];

    ifft(C, max_len);

    T sum_lens = static_cast<T>(a.size() + b.size());
    result.resize(sum_lens - 1);
    for(std::size_t i = 0 ; i < sum_lens - 1; ++i )
        result[i] = round(C[i].real());
}

template<typename T>
void FFTSolver<T>::divide(std::complex<double> *tab, T size)
{
    auto* odd_elements = new std::complex<double>[size/2];
    for(std::size_t i = 0; i < size/2; i++)
        odd_elements[i] = tab[i*2 + 1];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i] = tab[i*2];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i + size/2] = odd_elements[i];
    delete[] odd_elements;
}

template<typename T>
void FFTSolver<T>::fft(std::complex<double> *tab, T size)
{
    if( size < 2 ) return;
    auto w_n =  exp(std::complex<double>(0.0, 2.0* PI/size));
    auto exponent  = std::complex<double>(1.0, 0.0);
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
void FFTSolver<T>::ifft(std::complex<double> *tab, T size)
{
    transform(tab, tab+size, tab, [](std::complex<double> const& c){
        return conj(c);
    } );

    fft(tab, size);

    transform(tab, tab+size, tab, [&](std::complex<double> const& c){
        auto conj_c = conj(c);
        conj_c /= size;
        return conj_c;
    } );
}

template class FFTSolver<Int>;