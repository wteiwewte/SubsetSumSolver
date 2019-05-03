//
// Created by jan on 06.04.19.
//

#ifndef SUBSET_SUM_SOLVER_PRIMER_H
#define SUBSET_SUM_SOLVER_PRIMER_H

#include "Common/ConstantsAndTypes.h"

#include <array>
#include <type_traits>
#include <limits>
#include <iostream>


template <typename T = Int>
class PrimeDecider {
    static constexpr T witnesses [] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

public:
    PrimeDecider<T> () {
//        static_assert(std::is_integral_v<T>, "T isn't integral type!");
    }
    static bool isPrime(T x);
private:
    static std::pair<T, T> decouple(T x);
    static bool isComposite(T x, std::pair<T, T> decoupled, T witness);
};

#endif //SUBSET_SUM_SOLVER_PRIMER_H
