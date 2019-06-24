//
// Created by jan on 22.06.19.
//

#ifndef SUBSET_SUM_SOLVER_NTTCALCULATOR_H
#define SUBSET_SUM_SOLVER_NTTCALCULATOR_H

#include "Common/ConstantsAndTypes.h"
#include <vector>

enum class NTTKind{
    NORMAL = 0,
    INVERSE
};

constexpr int MOD_INT = 2013265921;
constexpr int ROOT_INT = 440564289;
constexpr int ORD_INT = 1<<27;
constexpr long long int MOD_LONG_LONG = 9223372006790004737;
constexpr long long int ROOT_LONG_LONG = 2379743102665616301;
constexpr long long int ORD_LONG_LONG = 1<<30;

template<typename T>
class NTTCalculator
{
    constexpr static Int N_MAX = static_cast<const Int>(2e6);
    static inline T A[N_MAX], B[N_MAX];
public:
    static Int getMod(std::size_t bound) { return (bound <= std::numeric_limits<int>::max()) ? MOD_INT : MOD_LONG_LONG; }
    static void multiply(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& result, std::size_t bound);
    static void multiplyWithOverflowCheck(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& result, std::size_t bound);
};


#endif //SUBSET_SUM_SOLVER_NTTCALCULATOR_H
