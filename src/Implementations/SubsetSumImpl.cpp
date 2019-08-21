//
// Created by jan on 07.04.19.
//
#include "SubsetSumImpl.h"

#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"

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
  istr >> ss._size;
  istr >> ss._target;
  if constexpr (OUTPUT_TEST_SIZE)
    std::cout << "\nTest size : n - "
              << ss._size << ", t - "
              << ss._target << '\n';
  ss._tab.resize(ss._size + 1);
  for (T i = 1; i <= ss._size; ++i)
    istr >> ss._tab[i];
  ss.init();
  return istr;
}

template <typename T>
std::ostream&
operator<<(std::ostream& ostr, const SubsetSumImpl<T>& ss)
{
  ostr << ss._size << ' ' << ss._target << std::endl;
  for (T i = 1; i <= ss._size; ++i)
    ostr << ss._tab[i] << ' ';
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
