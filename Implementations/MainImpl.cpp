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
//    std::cout << "Ln: " << _ln << '\n';
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
    _inversions_mod_p.resize(_target + 1);
    _ln.resize(_target + 1, 0);
    _g.resize(_target + 1, 0);
    _g[0] = 1;
    _gDivideAndConquer.resize(_target + 1, 0);
    _gDivideAndConquer[0] = 1;
//    _p = getRandomPrimeFromRange({_target + 1, fast_exp<T>(_target + _size, 3)});
    _p = NTTCalculator<T>::getMod(_target + 1);
//    _p = 11;
    std::cout << (long long int) _p << "<- prime\n";

    for(std::size_t i =  1 ; i <= _target; ++i)
        _inversions_mod_p[i] = fast_exp<T>(i, _p - 2, _p);
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::computeExp() {
//    if constexpr(expPolicy == ExponentCalculationPolicy::DIVIDE_AND_CONQUER)
//    {
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
                    G[j] = _gDivideAndConquer[j + l];

                std::vector<T> H;
                multiply(F, G, H);


                for(std::size_t i = m + 1; i <= r; ++i)
                {
//                    _g[i] += ((_inversions_mod_p[i] * (H[i - l] % _p)) % _p);
//                    _g[i] %= _p;
                    _gDivideAndConquer[i] += ((_inversions_mod_p[i] * (H[i - l] % _p)) % _p);
                    _gDivideAndConquer[i] %= _p;

                }

                std::vector<T>().swap(F);
                std::vector<T>().swap(G);
                std::vector<T>().swap(H);

                compute(m + 1, r);
            }
        };

        compute(0, _target);
//    }
//    else
//    {
//        _g = exp(_ln);
//        std::cout << "Correct g: " << _gDivideAndConquer << '\n';
//        exp(_ln, _g);
        expV2(_ln, _g);
//        std::cout << "g: " << _g << '\n';
//        std::vector<T> gV2 = _g;
//        std::cout << "gV2: " << gV2 << '\n';
//    }
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::multiply(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result)
{
    if constexpr(multPolicy == FormalSeriesMultiplicationPolicy::TRIVIAL)
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
    else if constexpr(multPolicy == FormalSeriesMultiplicationPolicy::FFT)
        FFTCalculator<T>::multiply(a, b, result);
    else
        NTTCalculator<T>::multiply(a, b, result, _target + 1);
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::reciprocal(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] != 0);
    std::size_t bound = input.size();
    std::vector<T> b(1, _inversions_mod_p[input[0]]);

    for(std::size_t i = 0; /*2 * i + 2 < bound*/; i = (2 * i + 1))
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
        if(2 * i + 2 >= bound)
            break;
    }
    assert(b.size() >= bound);
    std::move(b.begin(), b.begin() + bound, std::back_inserter(result));

    if constexpr (CHECK_RECIPROCAL)
    {
        std::vector<T> test;
        multiply(input, result, test);
        test.resize(input.size());

        bool correct = true;
        correct &= ((test[0] % _p) == 1);
        correct &= std::all_of(test.begin() + 1, test.end(),
                               [this](const T& val) { return ((val % _p) == 0); });
        assert(correct);
    }
//    std::cout << "RECIPROC_INPUT " << input << '\n';
//    std::cout << "RECIPROC_RESULT " << result << '\n';
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::log(const std::vector<T> &input, std::vector<T> &result) {
//    std::cout << (LL) input[0] << " log_input[0]\n";
    assert(input[0] == 1);
//    std::cout << input << " LOG_INPUT\n";
    if(input.size() == 1)
    {
        result.resize(1, 0);
//        std::cout << result << " LOG_RESULT\n";
        return;
    }

    std::vector<T> derivative(input.size() - 1);
    for(std::size_t i = 0 ; i < input.size() - 1; ++i)
        derivative[i] = ((i + 1) * input[i + 1]) % _p;
//    std::cout << derivative << " LOG_DERI\n";

    std::vector<T> reciproc;
    reciprocal(input, reciproc);
    assert(reciproc.size() == input.size());
//    std::cout << reciproc << "LOG_RECIPROC\n";

    std::vector<T> shiftedResult;
    multiply(derivative, reciproc, shiftedResult);
    assert(shiftedResult.size() >= input.size());
//    std::cout << shiftedResult << " LOG_SHIFTED\n";


    result.resize(input.size(), 0); // assuming result[0] = 0, because input[0] = 1

    for(std::size_t i = 1 ; i < result.size(); ++i)
        result[i] = (_inversions_mod_p[i] * shiftedResult[i - 1]) % _p;
//    std::cout << result << " LOG_RESULT\n";
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::exp(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] == 0);
//    std::cout << input << " INPUT\n";
    // pomnożyć koncowy wynik przez input[0] ^{-1} % _p
//    std::cout << (LL) input[0] << " exp_input[0]\n";
//    T a0 = input[0];
//    T a0Exp = fast_exp()
//    const_cast<std::vector<T>&>(input)[0] = 0;
    result.resize(1, 1);
    for(std::size_t i = 1 ; i < input.size(); i *= 2)
    {
        std::cout << "I: " << i << '\n';
        assert(i == result.size());
        std::vector<T> logRl;
        log(result, logRl);
        assert(logRl.size() == result.size());
        std::vector<T> diffRlandP(2 * result.size());

        for(std::size_t j =  0 ; j < diffRlandP.size(); ++j)
        {
            diffRlandP[j] = (j < logRl.size() ? logRl[j] : 0) - (j < input.size() ? input[j] : 0);
            diffRlandP[j] = (diffRlandP[j] < 0) ? (diffRlandP[j] + _p) : diffRlandP[j];
            assert(diffRlandP[j] >= 0);
            assert(diffRlandP[j] < _p);
        }

        std::vector<T> partialResult;
        multiply(result, diffRlandP, partialResult);

        assert(partialResult.size() >= 2 * result.size());
//        std::cout << partialResult << " partialResult\n";
        std::transform(std::make_move_iterator(partialResult.begin() + result.size()),
                       std::make_move_iterator(partialResult.begin() + 2 * result.size()),
                       std::back_inserter(result), [this](T&& value) { return (_p - (value % _p)) % _p; });
//        std::cout << result << " Exp result\n";
    }
//    std::cout << result << " result\n";
}

template<typename T, ExponentCalculationPolicy expPolicy, FormalSeriesMultiplicationPolicy multPolicy>
void MainImpl<T, expPolicy, multPolicy>::expV2(const std::vector<T> &input, std::vector<T> &result) {
    assert(input[0] == 0);
    result.resize(input.size(), 0);
    result[0] = 1;
    for(std::size_t i = 1 ; i < input.size(); i *= 2)
    {
//        std::cout << "I: " << i << '\n';
        std::vector<T> logRl;
        log(result, logRl);
        assert(logRl.size() == result.size());
        std::vector<T> diffRlandP(result.size());

        for(std::size_t j =  0 ; j < diffRlandP.size(); ++j)
        {
            diffRlandP[j] = logRl[j] - input[j];
            diffRlandP[j] = (diffRlandP[j] < 0) ? (diffRlandP[j] + _p) : diffRlandP[j];
            assert(std::clamp<Int>(diffRlandP[j], 0, _p) == diffRlandP[j]);
//            assert(diffRlandP[j] >= 0);
//            assert(diffRlandP[j] < _p);
        }

        std::vector<T> partialResult;
        multiply(result, diffRlandP, partialResult);

//        std::cout << partialResult << " partialResult\n";
        for(std::size_t j = i; j < std::min(result.size(), 2 * i) ; ++j)
            result[j] = (result[j] - partialResult[j] + _p) % _p;
//        std::transform(std::make_move_iterator(partialResult.begin() + result.size()),
//                       std::make_move_iterator(partialResult.begin() + 2 * result.size()),
//                       std::back_inserter(result), [this](T&& value) { return (_p - (value % _p)) % _p; });
//        std::cout << result << " Exp result\n";
    }
}

template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::FFT>;
template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::NTT>;
template class MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::TRIVIAL>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::FFT>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::NTT>;
template class MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD, FormalSeriesMultiplicationPolicy::TRIVIAL>;
