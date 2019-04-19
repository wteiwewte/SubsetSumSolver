//
// Created by jan on 07.04.19.
//
#include "ConstantsAndTypes.h"
#include "SubsetSumImpl.h"

#include <cassert>
#include <iterator>

template<typename T>
bool SubsetSumImpl<T>::verify() const
{
    bool doesSumExist[_size + 2][_target + 2];

    for(std::size_t i = 0 ; i <= _size ; ++i)
        for(std::size_t j = 0; j <= _target; ++j)
            doesSumExist[i][j] = false;

    for(std::size_t i = 0; i <= _size; ++i)
        doesSumExist[i][0] = true;

    for(std::size_t i = 1 ; i <= _size ; ++i)
        for(std::size_t j = 1; j <= _target; ++j)
        {
            if(_tab[i] > j)
                doesSumExist[i][j] = doesSumExist[i - 1][j];
            else
                doesSumExist[i][j] = (doesSumExist[i - 1][j] || doesSumExist[i - 1][j - _tab[i]]);
        }
    return (doesSumExist[_size][_target] == _result);
}

template<typename T>
std::istream& operator>>(std::istream& istr, SubsetSumImpl<T>& ss)
{
    istr >> ss._size;
    istr >> ss._target;
    if(OUTPUT_TEST_SIZE)
        std::cout << "\nTest size : n - " << ss._size << ", t - " << ss._target << '\n';
    ss._tab.resize(ss._size + 1);
    for (std::size_t i = 1; i <= ss._size ; ++i)
    {
        istr >> ss._tab[i];
    }
    ss.init();
    return istr;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const SubsetSumImpl<T>& ss)
{
    ostr << ss._size << ' ' << ss._target << std::endl;
    for (std::size_t i = 1; i <= ss._size ; ++i)
    {
        ostr << ss._tab[i] << ' ';
    }
    ostr << std::endl;
    ostr << "RESULT - " << ss._result << std::endl;
    return ostr;
}

template class SubsetSumImpl<Int>;

template std::istream& operator>>(std::istream&, SubsetSumImpl<Int>&);
template std::ostream& operator<<(std::ostream&, const SubsetSumImpl<Int>&);
