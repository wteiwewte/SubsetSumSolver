//
// Created by jan on 07.04.19.
//
#include "MainImpl.h"

#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "SupplementaryAlgorithms/FFTCalculator.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <cassert>
#include <functional>
#include <random>

template<typename T, ExponentCalculationPolicy expPolicy>
T MainImpl<T, expPolicy>::getRandomPrimeFromRange(std::pair<T, T> range)
{
    static std::random_device rd;
    static std::mt19937 eng(rd());
    static std::uniform_int_distribution<T> distr(range.first, range.second);

    T prime;
    while(!PrimeDecider<T>::isPrime((prime = distr(eng))));

    return prime;
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::solve()
{
    computeLog();
    computeExp();
    _result = ((_g[_target] % _p) != 0);
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::computeLog() {
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

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::init() {
    _inversions_mod_p.resize(_target + 1);
    _ln.resize(_target + 1, 0);
    _g.resize(_target + 1, 0);
    _g[0] = 1;
    _p = getRandomPrimeFromRange({_target + 1, fast_exp<T>(_target + _size, 3)});
    std::cout << (long long int) _p << "<- prime\n";

    for(std::size_t i =  1 ; i <= _target; ++i)
        _inversions_mod_p[i] = fast_exp<T>(i, _p - 2, _p);
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::computeExp() {
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

                FFTCalculator<T>::multiply(F, G, H);

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
        exp(_ln, _g);
    }
}

template<typename T, ExponentCalculationPolicy expPolicy>
template<FormalSeriesMultiplicationPolicy policy>
void MainImpl<T, expPolicy>::multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result)
{
    if constexpr(policy == FormalSeriesMultiplicationPolicy::TRIVIAL)
    {
        T sum_lens = static_cast<T>(a.size() + b.size());
        result.resize(sum_lens - 1, 0);
        for(std::size_t i = 0 ; i < a.size(); ++i) {
            for (std::size_t j = 0; j < b.size(); ++j) {
                result[i + j] += (a[i] * b[j]);
                result[i + j] %= _p;
            }
        }
    }
    else if constexpr(policy == FormalSeriesMultiplicationPolicy::FFT)
        FFTCalculator<T>::multiply(a, b, result);
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::reciprocal(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] != 0);
    std::size_t bound = input.size();
    std::vector<T> b(1, _inversions_mod_p[input[0]]);

    for(std::size_t i = 0; /*2 * i + 2 < bound*/; i = (2 * i + 1))
    {
        std::vector<T> squareB;
        multiply<FormalSeriesMultiplicationPolicy::TRIVIAL>(b, b, squareB);
        std::transform(squareB.begin(), squareB.end(),
                       squareB.begin(), [this](T& value) { return value % _p; });

        std::vector<T> inputPrefix(input.begin(), input.begin() + std::min(input.size(), 2 * i + 2));
        inputPrefix.resize(2 * i + 2, T(1));

        std::vector<T> partialResult;
        multiply<FormalSeriesMultiplicationPolicy::TRIVIAL>(inputPrefix, squareB, partialResult);

        std::transform(std::make_move_iterator(partialResult.begin() + i + 1), std::make_move_iterator(partialResult.begin() + 2 * i + 2),
                       std::back_inserter(b), [this](T&& value) { return (_p - (value % _p)) % _p; });

        assert(b.size() == 2 * i + 2);
        if(2 * i + 2 >= bound)
            break;
    }
    assert(b.size() >= bound);
    std::move(b.begin(), b.begin() + bound, std::back_inserter(result));

    if constexpr (CHECK_RECIPROCAL)
    {
        std::vector<T> test;
        multiply<FormalSeriesMultiplicationPolicy::TRIVIAL>(input, result, test);
        test.resize(input.size());


        bool correct = true;
        correct &= ((test[0] % _p) == 1);
        correct &= std::all_of(test.begin() + 1, test.end(),
                               [this](const T& val) { return ((val % _p) == 0); });
        assert(correct);
    }
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::log(const std::vector<T> &input, std::vector<T> &result) {
    if(input.size() == 1)
    {
        result.resize(1, 0);
        return;
    }


    auto coefficientGenerator = [this, &input, i = 0]() mutable {
        i++;
        return (i * input[i]) % _p;
    };
    std::vector<T> derivative(input.size() - 1);
    std::generate(derivative.begin(), derivative.end(), coefficientGenerator);

    std::vector<T> reciproc;
    reciprocal(input, reciproc);
    assert(reciproc.size() == input.size());

    std::vector<T> shiftedResult;
    multiply<FormalSeriesMultiplicationPolicy::TRIVIAL>(derivative, reciproc, shiftedResult);
    if(shiftedResult.size() < input.size())
        std::cerr << shiftedResult.size() << " " << input.size() << '\n';
    assert(shiftedResult.size() >= input.size());

    result.resize(input.size(), 0); // assuming result[0] = 0, because input[0] = 1
    auto resultCoefficientGenerator = [this, &shiftedResult, i = 0]() mutable {
        i++;
        return (shiftedResult[i - 1] * _inversions_mod_p[i]) % _p;
    };

    std::generate(result.begin(), result.end(), resultCoefficientGenerator);
}

template<typename T, ExponentCalculationPolicy expPolicy>
void MainImpl<T, expPolicy>::exp(const std::vector<T> &input, std::vector<T> &result) {
    result.resize(1, 1);
    for(std::size_t i = 1 ; i < input.size(); i *= 2)
    {
        assert(i == result.size());
        std::vector<T> logRl;
        log(result, logRl);
        std::vector<T> diffRlandP(2 * result.size());
        auto gen = [this, &logRl, &input, j = 0]() mutable {
            j++;
            auto diff = ((j - 1) < logRl.size() ? logRl[j - 1] : 0) - ((j - 1) < input.size() ? input[j - 1] : 0);
            return (diff < 0) ? (_p - diff) : diff;
        };

        std::generate(diffRlandP.begin(), diffRlandP.end(), gen);

        std::vector<T> partialResult;
        multiply<FormalSeriesMultiplicationPolicy::TRIVIAL>(result, diffRlandP, partialResult);

        assert(partialResult.size() >= 2 * result.size());
        std::transform(std::make_move_iterator(partialResult.begin() + result.size()), std::make_move_iterator(partialResult.begin() + 2 * result.size()),
                       std::back_inserter(result), [this](T&& value) { return (_p - (value % _p)) % _p; });
    }
}


template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD>;