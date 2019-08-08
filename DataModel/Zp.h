//
// Created by jan on 20.04.19.
//

#ifndef SUBSET_SUM_SOLVER_ZPFIELD_H
#define SUBSET_SUM_SOLVER_ZPFIELD_H

#include <vector>

template<typename T>
class Zp {
public:
    static inline T prime = -1;
    static void init(T p, std::size_t inversionsToInit = 0);
    Zp<T> (T value = T(0)) : _value(value) {}

    static Zp<T> getInversion(Zp<T> value);

    Zp<T> operator+() const;
    Zp<T> operator-() const;

    Zp<T>& operator++();
    Zp<T>& operator--();

    Zp<T> operator++(int);
    Zp<T> operator--(int);
private:
    static inline bool initialized = false;
    static inline std::vector<T> inversions_mod_p {};

    template<typename U>
    friend constexpr bool operator<(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr bool operator==(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr bool operator!=(const Zp<U>& a, const Zp<U>& b);


    template<typename U>
    friend constexpr Zp<U> operator+(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr Zp<U> operator-(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr Zp<U> operator*(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr Zp<U> operator/(const Zp<U>& a, const Zp<U>& b);
    template<typename U>
    friend constexpr Zp<U> operator%(const Zp<U>& a, const Zp<U>& b);

    T _value;
};

template<class T> Zp(T) -> Zp<T>;


#endif //SUBSET_SUM_SOLVER_ZPFIELD_H
