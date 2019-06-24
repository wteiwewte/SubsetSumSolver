//
// Created by jan on 07.04.19.
//
#include "MainImpl.h"

#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "SupplementaryAlgorithms/FFTCalculator.h"
#include "SupplementaryAlgorithms/NTTCalculator.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <cassert>
#include <functional>
#include <random>
#include <DataModel/Zp.h>

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
T MainImpl<T, expPolicy, multPolicy>::getRandomPrimeFromRange(std::pair<T, T> range)
{
    static std::random_device rd;
    static std::mt19937 eng(rd());
    static std::uniform_int_distribution<T> distr(range.first, range.second);

    T prime;
    while(!PrimeDecider<T>::isPrime((prime = distr(eng))));

    return prime;
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::solve()
{
    computeLog();
    assert(_g[_target] == 0);
    computeExp();
    _result = ((_g[_target] % _p) != 0);
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::computeLog() {
    std::vector<T> powersOfA(_target + 1, 0);
    for(std::size_t i =  1 ; i <= _size; ++i)
        if(1 <= _tab[i] && _tab[i] <= _target)
            powersOfA[_tab[i]]++;

    for(std::size_t k = 1 ; k <= _target; ++k)
        powersOfA[k] %= _p;

    for(std::size_t k = 1 ; k <= _target; ++k)
        for(std::size_t j =  1 ; j <= _target / k; ++j)
        {
            _ln[j * k] += ((powersOfA[k] * (((j - 1) % 2 == 1) ? (_p - _inversions_mod_p[j]) : _inversions_mod_p[j])) % _p);
            _ln[j * k] %= _p;
        }
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::init() {
    std::map<ExponentCalculationPolicy, std::string> expPolicyNames =
            {
                    {ExponentCalculationPolicy::DIVIDE_AND_CONQUER, "DIVIDE_AND_CONQUER"},
                    {ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, "NEWTONS_ITERATIVE_METHOD"}
            };
    std::map<FormalSeriesMultiplicationPolicy , std::string> multPolicyNames =
            {
                    {FormalSeriesMultiplicationPolicy::TRIVIAL, "TRIVIAL"},
                    {FormalSeriesMultiplicationPolicy::NTT, "NTT"},
                    {FormalSeriesMultiplicationPolicy::FFT, "FFT"}
            };

    _inversions_mod_p.resize(_target + 1);
    _ln.resize(_target + 1, 0);
    _g.resize(_target + 1, 0);
    _g[0] = 1;
//    _p = getRandomPrimeFromRange({_target + 1, fast_exp<T>(_target + _size, 3)});
    _p = NTTCalculator<T>::getMod(_size + _target + 1);
//    Zp<T>::init(_p, _target);
    _inversions_mod_p.resize(_target + 1, T(0));
    for(std::size_t i =  1 ; i <= _target; ++i)
        _inversions_mod_p[i] = fast_exp<T>(i, _p - 2, _p);

    std::cout << expPolicyNames[expPolicy] << " " << multPolicyNames[multPolicy] << " " << (long long int) _p << "<- prime\n";
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::computeExp() {
    if constexpr(expPolicy == ExponentCalculationPolicy::DIVIDE_AND_CONQUER)
    {
        std::function<void(std::size_t, std::size_t)> compute = [this, &compute](std::size_t l, std::size_t r) {
            if(l < r)
            {
                std::size_t m = (l + r)/2;
                compute(l, m);

                std::vector<T> F(r - l + 1);
                for(std::size_t k = 0; k <= r - l; ++k)
                    F[k] = (k * _ln[k]) % _p;

                std::vector<T> G(m - l + 1);
                for(std::size_t j = 0; j <= m - l; ++j)
                    G[j] = _g[j + l];

                std::vector<T> H;
                multiply(F, G, H);


                for(std::size_t i = m + 1; i <= r; ++i)
                {
                    _g[i] += ((_inversions_mod_p[i] * (H[i - l] % _p)) % _p);
                    _g[i] %= _p;
                }

                std::vector<T>().swap(F);
                std::vector<T>().swap(G);
                std::vector<T>().swap(H);

                compute(m + 1, r);
            }
        };

        compute(0, _target);
    }
    else
    {
//        std::cout << "HALO\n";
//        std::vector<T> result;
//        NTTCalculator<T>::multiplyWithOverflowCheck(_ln, _g, result, _target + 1);
//        std::cout << "HALO1\n";
        exp(_ln, _g);
    }
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result)
{
    if constexpr(multPolicy == FormalSeriesMultiplicationPolicy::TRIVIAL)
    {
        assert(a.size() > 0 && b.size() > 0);
        result.resize(a.size() + b.size() - 1, 0);
        for(std::size_t i = 0 ; i < a.size(); ++i) {
            for (std::size_t j = 0; j < b.size(); ++j) {
                result[i + j] += (a[i] * b[j]);
                result[i + j] %= _p;
            }
        }
    }
    else if constexpr(multPolicy == FormalSeriesMultiplicationPolicy::FFT)
        FFTCalculator<T>::multiply(a, b, result);
//        FFTCalculator<T>::multiplyNoOverflow(a, b, result, _p);
    else
        NTTCalculator<T>::multiply(a, b, result, _target + 1);
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::reciprocal(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] != 0);
    std::size_t bound = input.size();
    std::vector<T> b(1, _inversions_mod_p[input[0]]);

    for(std::size_t i = 0; i + 1 < bound; i = (2 * i + 1))
    {
        std::vector<T> squareB;
        multiply(b, b, squareB);
        std::transform(squareB.begin(), squareB.end(),
                       squareB.begin(), [this](T& value) { return value % _p; });

        std::vector<T> inputPrefix(input.begin(), input.begin() + std::min(input.size(), 2 * i + 2));
        inputPrefix.resize(2 * i + 2, T(1));

        std::vector<T> partialResult;
        multiply(inputPrefix, squareB, partialResult);

        std::transform(std::make_move_iterator(partialResult.begin() + i + 1), std::make_move_iterator(partialResult.begin() + 2 * i + 2),
                       std::back_inserter(b), [this](T&& value) { return (_p - (value % _p)) % _p; });

        assert(b.size() == 2 * i + 2);
    }
    assert(b.size() >= bound);
    std::move(b.begin(), b.begin() + bound, std::back_inserter(result));

    if constexpr (CHECK_RECIPROCAL)
    {
        std::vector<T> test;
        multiply(input, result, test);
        test.resize(input.size());

//        for(std::size_t i = 0 ; i < test.size(); ++i)
//            std::cout << (long long int) (test[i] % _p) << ' ';
//        std::cout << '\n';
        bool correct = true;
        correct &= ((test[0] % _p) == 1);
        correct &= std::all_of(test.begin() + 1, test.end(),
                               [this](const T& val) { return ((val % _p) == 0); });
        assert(correct);
    }
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::log(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] == 1);
    if(input.size() == 1)
    {
        result.resize(1, 0);
        return;
    }

    std::vector<T> derivative(input.size() - 1);
    for(std::size_t i = 0 ; i < input.size() - 1; ++i)
        derivative[i] = ((i + 1) * input[i + 1]) % _p;

    std::vector<T> reciproc;
    reciprocal(input, reciproc);
    assert(reciproc.size() == input.size());

    std::vector<T> shiftedResult;
    multiply(derivative, reciproc, shiftedResult);
    assert(shiftedResult.size() >= input.size());

    result.resize(input.size(), 0);

    for(std::size_t i = 1 ; i < result.size(); ++i)
        result[i] = (_inversions_mod_p[i] * shiftedResult[i - 1]) % _p;
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::exp(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] == 0);
    result.resize(1, 1);
    for (std::size_t i = 1; i < input.size(); i *= 2) {
        result.resize(std::min(input.size(), 2 * i), 0);
        std::vector<T> logRl;
        log(result, logRl);
        assert(logRl.size() == result.size());
        std::vector<T> diffRlandP(result.size());

        for (std::size_t j = 0; j < diffRlandP.size(); ++j) {
            diffRlandP[j] = logRl[j] - input[j];
            diffRlandP[j] = (diffRlandP[j] < 0) ? (diffRlandP[j] + _p) : diffRlandP[j];
            assert(std::clamp<T>(diffRlandP[j], 0, _p) == diffRlandP[j]);
        }

        std::vector<T> partialResult;
        multiply(result, diffRlandP, partialResult);

        for (std::size_t j = i; j < std::min(input.size(), 2 * i); ++j)
            result[j] = (result[j] - partialResult[j] + _p) % _p;
//        std::transform(std::make_move_iterator(partialResult.begin() + result.size()),
//                       std::make_move_iterator(partialResult.begin() + 2 * result.size()),
//                       std::back_inserter(result), [this](T&& value) { return (_p - (value % _p)) % _p; });
    }
}

template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::FFT>;
template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::NTT>;
template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::TRIVIAL>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::FFT>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::NTT>;
//template class MainImpl<Zp<Int>, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::NTT>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::TRIVIAL>;
