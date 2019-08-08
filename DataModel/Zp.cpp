//
// Created by jan on 20.04.19.
//

#include "Zp.h"

#include "Common/Util.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <cassert>

template<typename T>
void Zp<T>::init(T p, std::size_t inversionsToInit) {
    static_assert(std::is_integral_v<T>, "T isn't integral type!");
    assert(PrimeDecider<T>::isPrime(p));
    prime = p;
    initialized = true;
    inversions_mod_p.resize(inversionsToInit, T(0));
    for(std::size_t i =  1 ; i <= inversionsToInit; ++i)
        inversions_mod_p[i] = fast_exp<T>(i, prime - 2, prime);
}

template<typename T>
Zp<T> Zp<T>::operator+() const {
    return Zp<T>(_value);
}

template<typename T>
Zp<T> Zp<T>::operator-() const {
    return Zp<T>(prime - _value);
}

//Incrementing/decrementing functions not taking remainder mod prime due to performance reasons
template<typename T>
Zp<T>& Zp<T>::operator++() {
    _value++;
    return *this;
}

template<typename T>
Zp<T> &Zp<T>::operator--() {
    _value--;
    return *this;
}

template<typename T>
Zp<T> Zp<T>::operator++(int) {
    return Zp<T>(_value++);
}

template<typename T>
Zp<T> Zp<T>::operator--(int) {
    return Zp<T>(_value--);
}

template<typename T>
constexpr Zp<T> operator+(const Zp<T> &a, const Zp<T> &b) {
    return Zp<T>((a._value + b._value) % Zp<T>::prime);
}

template<typename T>
constexpr Zp<T> operator%(const Zp<T> &a, const Zp<T> &b) {
    return Zp<T>((a._value % b._value) % Zp<T>::prime);
}

template<typename T>
constexpr Zp<T> operator/(const Zp<T> &a, const Zp<T> &b) {
    return Zp<T>((a._value * Zp<T>::getInversion(b)) % Zp<T>::prime);
}

template<typename T>
constexpr Zp<T> operator*(const Zp<T> &a, const Zp<T> &b) {
    return Zp<T>((a._value * b._value) % Zp<T>::prime);
}

template<typename T>
constexpr Zp<T> operator-(const Zp<T> &a, const Zp<T> &b) {
    return Zp<T>((a._value + (-b._value)) % Zp<T>::prime);
}

template<typename T>
Zp<T> Zp<T>::getInversion(Zp<T> value) {
    return inversions_mod_p[value._value];
}

template<typename U>
constexpr bool operator<(const Zp<U> &a, const Zp<U> &b) {
    return a._value < b._value;
}

template<typename U>
constexpr bool operator==(const Zp<U> &a, const Zp<U> &b) {
    return a._value == b._value;
}

template<typename U>
constexpr bool operator!=(const Zp<U> &a, const Zp<U> &b) {
    return !(a == b);
}


template class Zp<Int>;
