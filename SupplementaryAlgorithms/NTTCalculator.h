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

template<typename T>
class NTTCalculator
{
    inline static Int MOD = 0;
    inline static Int ROOT = 0;
    inline static Int ORD = 0;
    constexpr static Int MOD_INT = 2013265921;
    constexpr static Int ROOT_INT = 440564289;
    constexpr static Int ORD_INT = 1<<27;
    constexpr static Int MOD_LONG_LONG = 9223372006790004737;
    constexpr static Int ROOT_LONG_LONG = 2379743102665616301;
    constexpr static Int ORD_LONG_LONG = 1<<30;

    constexpr static Int N_MAX = static_cast<const Int>(2e6);
    static inline T A[N_MAX], B[N_MAX];

    template<NTTKind kind>
    static Int getRoot(std::size_t);

    static void divide(T* tab, std::size_t size);
    template<NTTKind kind>
    static void ntt(T* tab, std::size_t size);
    static void initModRootOrd(std::size_t bound);
public:
    static Int getMod(std::size_t bound) { return (bound <= std::numeric_limits<int>::max()) ? MOD_INT : MOD_LONG_LONG; }
//    constexpr static Int getMod() { return (sizeof(T) <= sizeof(int)) ? MOD_INT : MOD_LONG_LONG; }
//    constexpr static Int getRoot() { return (sizeof(T) <= sizeof(int)) ? ROOT_INT : ROOT_LONG_LONG; }
//    constexpr static Int getOrd() { return (sizeof(T) <= sizeof(int)) ? ORD_INT : ORD_LONG_LONG; }
    static void multiply(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result, std::size_t bound);
};


#endif //SUBSET_SUM_SOLVER_NTTCALCULATOR_H
