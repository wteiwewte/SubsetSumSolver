//
// Created by jan on 07.04.19.
//
#include "SubsetSumImpl.h"

#include "Common/ConstantsAndTypes.h"

#include <cassert>
#include <iterator>

template <typename T>
SubsetSumImpl<T>::SubsetSumImpl(const SubsetSumImpl<T>& other)
  : _tab(other._tab), _target(other._target), _size(other._size)
{
}

template <typename T>
std::istream&
operator>>(std::istream& istr, SubsetSumImpl<T>& ss)
{
  long long int temp1;
  istr >> ss._size;
  istr >> temp1;
  ss._target = temp1;
  //    istr >> ss._target;
  if (OUTPUT_TEST_SIZE)
    std::cout << "\nTest size : n - " << (long long int) ss._size << ", t - "
              << (long long int) ss._target << '\n';
  ss._tab.resize(ss._size + 1);
  for (T i = 1; i <= ss._size; ++i)
  {
    long long int temp;
    istr >> temp;
    //        istr >> ss._tab[i];
    ss._tab[i] = temp;
  }
  ss.init();
  return istr;
}

template <typename T>
std::ostream&
operator<<(std::ostream& ostr, const SubsetSumImpl<T>& ss)
{
  ostr << ss._size << ' ' << (long long int) ss._target << std::endl;
  for (T i = 1; i <= ss._size; ++i)
  {
    ostr << (long long int) ss._tab[i] << ' ';
  }
  ostr << std::endl;
  ostr << "RESULT - " << ss._result << std::endl;
  return ostr;
}

#define INSTANTIATE_TEMPLATE_IMPL(type) \
  template class SubsetSumImpl<type>; \
  template std::istream& operator>>(std::istream&, SubsetSumImpl<type>&); \
  template std::ostream& operator<<(std::ostream&, const SubsetSumImpl<type>&);

INSTANTIATE_TEMPLATE_IMPL(int32_t)
INSTANTIATE_TEMPLATE_IMPL(int64_t)
INSTANTIATE_TEMPLATE_IMPL(__int128)
