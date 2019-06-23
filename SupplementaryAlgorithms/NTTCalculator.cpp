#include "NTTCalculator.h"
#include "Common/Util.h"

#include <algorithm>

template<typename T>
void NTTCalculator<T>::multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result, std::size_t bound)
{
    initModRootOrd(bound);
//    std::cout << (long long int) MOD << " MOD\n";
    std::size_t max_len = getPowerOf2GreaterThan(a.size() + b.size());
    for(std::size_t i = 0 ; i < max_len ; ++i ) {
        A[i] = B[i] = 0;
    }

    for(std::size_t i = 0 ; i < a.size(); ++i ){
        A[i] = a[i];
    }
    for(std::size_t i = 0 ; i < b.size(); ++i ){
        B[i] = b[i];
    }

    ntt<NTTKind::NORMAL>(A, max_len);
    ntt<NTTKind::NORMAL>(B, max_len);

    T inv_n = fast_exp<T>(max_len, MOD - 2, MOD);
    for(std::size_t i = 0 ; i < max_len; ++i )
        A[i] = ((((Int) A[i] * B[i]) % MOD) * inv_n) % MOD;

    ntt<NTTKind::INVERSE>(A, max_len);

    std::size_t sum_lengths = a.size() + b.size();
    result.resize(sum_lengths - 1);
    for(std::size_t i = 0 ; i < sum_lengths - 1; ++i )
        result[i] = A[i];
}

template<typename T>
void NTTCalculator<T>::divide(T* tab, std::size_t size)
{
    auto* odd_elements = new T[size/2];
    for(int i = 0; i < size/2; i++)
        odd_elements[i] = tab[i*2 + 1];
    for(int i = 0; i < size/2; i++)
        tab[i] = tab[i*2];
    for(int i = 0; i < size/2; i++)
        tab[i + size/2] = odd_elements[i];
    delete[] odd_elements;
}

template<typename T>
template<NTTKind kind>
void NTTCalculator<T>::ntt(T* tab, std::size_t size)
{
    if( size < 2 ) return;
    Int w_n = getRoot<kind>(size);
    Int exponent = 1;
    divide(tab, size);
    ntt<kind>(tab, size/2);
    ntt<kind>(tab + size/2, size/2);
    for(int i = 0; i < size/2; i++ ) {
        Int t = tab[i];
        tab[i] = (tab[i] + ((exponent * tab[i+size/2]) % MOD)) % MOD;
        tab[i + size/2] = (t - ((exponent * tab[i + size/2]) % MOD) + MOD) % MOD;
        exponent = (exponent * w_n) % MOD;
    }
}

template<typename T>
template<NTTKind kind>
Int NTTCalculator<T>::getRoot(std::size_t size) {
    Int w_n = fast_exp(ROOT, ORD/size, MOD);
    if constexpr(kind == NTTKind::INVERSE)
        w_n = fast_exp(w_n, MOD - 2, MOD);
    return w_n;
}

template<typename T>
void NTTCalculator<T>::initModRootOrd(std::size_t bound) {
    if(bound < MOD_INT)
    {
        MOD = MOD_INT;
        ROOT = ROOT_INT;
        ORD = ORD_INT;
    }
    else
    {
        MOD = MOD_LONG_LONG;
        ROOT = ROOT_LONG_LONG;
        ORD = ORD_LONG_LONG;
    }
}

template class NTTCalculator<Int>;