//
// Created by jan on 07.04.19.
//
#include "Common/ConstantsAndTypes.h"
#include "SubsetSumImpl.h"

#include <cassert>
#include <iterator>


template<typename T>
SubsetSumImpl<T>::SubsetSumImpl(const SubsetSumImpl<T>& other) :
    _tab(other._tab),
    _target(other._target),
    _size(other._size)
    {}

template<typename T>
std::istream& operator>>(std::istream& istr, SubsetSumImpl<T>& ss)
{
    long long int temp1;
    istr >> ss._size;
    istr >> temp1;
    ss._target = temp1;
//    istr >> ss._target;
    if(OUTPUT_TEST_SIZE)
        std::cout << "\nTest size : n - " << (long long int) ss._size << ", t - " << (long long int) ss._target << '\n';
    ss._tab.resize(ss._size + 1);
    for (std::size_t i = 1; i <= ss._size ; ++i)
    {
        long long int temp;
        istr >> temp;
//        istr >> ss._tab[i];
        ss._tab[i] = temp;
    }
    ss.init();
    return istr;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const SubsetSumImpl<T>& ss)
{
    ostr << ss._size << ' ' << (long long int) ss._target << std::endl;
    for (std::size_t i = 1; i <= ss._size ; ++i)
    {
        ostr << (long long int) ss._tab[i] << ' ';
    }
    ostr << std::endl;
    ostr << "RESULT - " << ss._result << std::endl;
    return ostr;
}

template class SubsetSumImpl<Int>;

template std::istream& operator>>(std::istream&, SubsetSumImpl<Int>&);
template std::ostream& operator<<(std::ostream&, const SubsetSumImpl<Int>&);
