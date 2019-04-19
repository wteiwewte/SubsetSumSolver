//
// Created by jan on 07.04.19.
//

#include <random>
#include "ConstantsAndTypes.h"
#include "DummyImplementation.h"
#include "PrimeDecider.h"
#include "Util.h"
#include "FFTSolver.h"

template<typename T>
T DummyImplementation<T>::getRandomPrimeFromRange(std::pair<T, T> range)
{
    static std::random_device rd;
    static std::mt19937 eng(rd());
    static std::uniform_int_distribution<T> distr(range.first, range.second);

    T prime;
    while(!PrimeDecider<T>::isPrime((prime = distr(eng))));

    return prime;
}

template<typename T>
void DummyImplementation<T>::solve()
{
//    std::cout << "AJ\n";
//    for(std::size_t i = 1 ; i <= _size; ++i)
//        std::cout << _tab[i] << ' ';
//    std::cout << '\n';
//
    std::vector<T> powersOfA(_target + 1, 0);
    for(std::size_t i =  1 ; i <= _size; ++i)
        if(1 <= _tab[i] && _tab[i] <= _target)
            powersOfA[_tab[i]]++;

//    std::cout << "BJ\n";
//    for(std::size_t i = 1 ; i <= _target; ++i)
//        std::cout << powersOfA[i] << ' ';
//    std::cout << '\n';
    for(std::size_t k = 1 ; k <= _target; ++k)
        powersOfA[k] %= _p;

    for(std::size_t k = 1 ; k <= _target; ++k)
        for(std::size_t j =  1 ; j <= _target / k; ++j)
        {
            _ln[j * k] += ((powersOfA[k] * (((j - 1) % 2 == 1) ? (_p - _inversions_mod_p[j]) : _inversions_mod_p[j])) % _p);
            _ln[j * k] %= _p;
        }

    compute(0, _target);
    _result = ((_g[_target] % _p) != 0);
}

template<typename T>
void DummyImplementation<T>::compute(std::size_t l, std::size_t r)
{
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

        FFTSolver<T>::multiply(F, G, H);

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
}

template<typename T>
void DummyImplementation<T>::init() {
    _inversions_mod_p.resize(_target + 1);
    _ln.resize(_target + 1, 0);
    _g.resize(_target + 1, 0);
    _g[0] = 1;
    _p = getRandomPrimeFromRange({_target + 1, fast_exp<T>(_target + _size, 3)});
//    std::cout << _p << "<- prime\n";

    for(std::size_t i =  1 ; i <= _target; ++i)
        _inversions_mod_p[i] = fast_exp<T>(i, _p - 2, _p);
}


template class DummyImplementation<Int>;