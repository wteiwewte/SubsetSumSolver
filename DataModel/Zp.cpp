//
// Created by jan on 20.04.19.
//

#include "Zp.h"

#include "Common/Util.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <cassert>

template<typename T>
void Zp<T>::init(const T prime, const std::size_t inversionsToInit) {
    static_assert(std::is_integral_v<T>, "T isn't integral type!");
    assert(PrimeDecider<T>::isPrime(prime));
    p = prime;
    initialized = true;
    inversions_mod_p.resize(inversionsToInit + 1, T(0));
    for(std::size_t i =  1 ; i <= inversionsToInit; ++i)
        inversions_mod_p[i] = fast_exp<T>(i, p - 2, p);
}

template<typename T>
Zp<T> Zp<T>::getInversion(const Zp<T> value) {
//    return Zp(inversions_mod_p[value._value]);
    return Zp(fast_exp(value._value, p - 2, p));
}

template<typename T>
Zp<T> Zp<T>::getInversion(const T value) {
//    return Zp(inversions_mod_p[value]);
    return Zp(fast_exp(value, p - 2, p));
}

template class Zp<Int>;
