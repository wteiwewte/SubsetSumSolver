//
// Created by jan on 06.04.19.
//

#ifndef SUBSET_SUM_SOLVER_UTIL_H
#define SUBSET_SUM_SOLVER_UTIL_H

#include <chrono>
#include <type_traits>

template<typename T>
T getPowerOf2GreaterThan(T atleast)
{
    T temp = 1;
    while(temp < atleast)
        temp <<= 1;
    return temp;
}

template<typename T>
T fast_exp(T base, T exp, T modulo)
{
//    static_assert(std::is_integral_v<T>, "T is not integral!");
    if(exp == 0)
        return 1;

    if(exp % 2 == 0)
    {
        T half = fast_exp(base, exp/2, modulo);
        return (half * half) % modulo;
    }
    else
    {
        T half = fast_exp(base, exp/2, modulo);
        return (((half * half) % modulo) * base) % modulo;
    }
}

template<typename T>
T fast_exp(T base, T exp)
{
//    static_assert(std::is_integral_v<T>, "T is not integral!");
    if(exp == 0)
        return 1;

    if(exp % 2 == 0)
    {
        T half = fast_exp(base, exp/2);
        return (half * half);
    }
    else
    {
        T half = fast_exp(base, exp/2);
        return (((half * half)) * base);
    }
}

template<typename Func, typename TimeRep = std::chrono::milliseconds>
std::enable_if_t<!std::is_same_v<std::decay_t <typename std::invoke_result_t<Func>>, void>,
        std::pair<typename std::invoke_result_t<Func>, std::chrono::duration<double>>> executeAndMeasureTime(Func f)
{
    auto start = std::chrono::steady_clock::now();
    auto result = f();
    auto end = std::chrono::steady_clock::now();
    return {result, std::chrono::duration_cast<TimeRep>(end - start)};
}

template<typename Func, typename TimeRep = std::chrono::milliseconds>
std::enable_if_t<std::is_same_v<std::decay_t <typename std::invoke_result_t<Func>>, void>,
        std::chrono::duration<double>> executeAndMeasureTime(Func f)
{
    auto start = std::chrono::steady_clock::now();
    f();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<TimeRep>(end - start);
}

#endif //SUBSET_SUM_SOLVER_UTIL_H
