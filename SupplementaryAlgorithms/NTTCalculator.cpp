#include "NTTCalculator.h"
#include "Common/Util.h"

#include <algorithm>

#include <boost/safe_numerics/safe_integer.hpp>

namespace
{

template<typename T>
T MOD = T(0);
template<typename T>
T ROOT = T(0);
template<typename T>
T ORD = T(0);

template<typename T, NTTKind kind>
T getRoot(std::size_t size) {
    T w_n = fast_exp(ROOT<T>, ORD<T>/size, MOD<T>);
    if constexpr(kind == NTTKind::INVERSE)
        w_n = fast_exp(w_n, MOD<T> - 2, MOD<T>);
    return w_n;
}

template<typename T>
void divide(T* tab, std::size_t size)
{
    auto* odd_elements = new T[size/2];
    for(std::size_t i = 0; i < size/2; i++)
        odd_elements[i] = tab[i*2 + 1];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i] = tab[i*2];
    for(std::size_t i = 0; i < size/2; i++)
        tab[i + size/2] = odd_elements[i];
    delete[] odd_elements;
}

template<typename T, NTTKind kind>
void ntt(T* tab, std::size_t size)
{
    if( size < 2 ) return;
    Int w_n = getRoot<T, kind>(size);
    Int exponent = 1;
    divide(tab, size);
    ntt<T, kind>(tab, size/2);
    ntt<T, kind>(tab + size/2, size/2);
    for(std::size_t i = 0; i < size/2; i++ ) {
        Int t = tab[i];
        tab[i] = (tab[i] + ((exponent * tab[i+size/2]) % MOD<T>)) % MOD<T>;
        tab[i + size/2] = (t - ((exponent * tab[i + size/2]) % MOD<T>) + MOD<T>) % MOD<T>;
        exponent = (exponent * w_n) % MOD<T>;
    }
}

template<typename T>
void initModRootOrd(std::size_t bound) {
    if(bound < MOD_INT)
    {
        MOD<T> = MOD_INT;
        ROOT<T> = ROOT_INT;
        ORD<T> = ORD_INT;
    }
    else
    {
        MOD<T> = MOD_LONG_LONG;
        ROOT<T> = ROOT_LONG_LONG;
        ORD<T> = ORD_LONG_LONG;
    }
}
}

template<typename T>
void NTTCalculator<T>::multiply(const std::vector<T>& x, const std::vector<T> &y, std::vector<T> &result, std::size_t bound)
{
    initModRootOrd<T>(bound);
    std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

    std::fill(std::begin(A), std::begin(A) + adjustedLength, 0);
    std::fill(std::begin(B), std::begin(B) + adjustedLength, 0);

    std::copy(x.begin(), x.end(), std::begin(A));
    std::copy(y.begin(), y.end(), std::begin(B));

    ntt<T, NTTKind::NORMAL>(A, adjustedLength);
    ntt<T, NTTKind::NORMAL>(B, adjustedLength);

    T inversionLength = fast_exp<T>(adjustedLength, MOD<T> - 2, MOD<T>);
    for(std::size_t i = 0 ; i < adjustedLength; ++i )
        A[i] = ((((Int) A[i] * B[i]) % MOD<T>) * inversionLength) % MOD<T>;

    ntt<T, NTTKind::INVERSE>(A, adjustedLength);

    result.resize(x.size() + y.size() - 1);
    for(std::size_t i = 0 ; i < result.size(); ++i )
        result[i] = A[i];
}

template<typename T>
void
NTTCalculator<T>::multiplyWithOverflowCheck(const std::vector<T> &x, const std::vector<T> &y, std::vector<T> &result,
                                            std::size_t bound) {
    using safeT = boost::safe_numerics::safe<T>;
    initModRootOrd<safeT>(bound);
    std::size_t adjustedLength = getPowerOf2GreaterThan(x.size() + y.size());

    std::vector<safeT> Asafe(adjustedLength);
    std::vector<safeT> Bsafe(adjustedLength);

    std::fill(std::begin(Asafe), std::end(Asafe), 0);
    std::fill(std::begin(Bsafe), std::end(Bsafe), 0);

    std::copy(x.begin(), x.end(), std::begin(Asafe));
    std::copy(y.begin(), y.end(), std::begin(Bsafe));

    std::cout << (long long int) MOD<safeT> << " MOD\n";
    ntt<safeT, NTTKind::NORMAL>(Asafe.data(), adjustedLength);
    ntt<safeT, NTTKind::NORMAL>(Bsafe.data(), adjustedLength);

    auto inversionLength = fast_exp<safeT>(adjustedLength, MOD<safeT> - 2, MOD<safeT>);
    for(std::size_t i = 0 ; i < adjustedLength; ++i )
        Asafe[i] = (((Asafe[i] * Bsafe[i]) % MOD<safeT>) * inversionLength) % MOD<safeT>;

    ntt<safeT, NTTKind::INVERSE>(Asafe.data(), adjustedLength);
//    result.resize(x.size() + y.size());
//    for(std::size_t i = 0 ; i < result.size(); ++i )
//        result[i] = Asafe[i];
}

template class NTTCalculator<Int>;