//
// Created by jan on 07.04.19.
//

#include "SubsetSumSolver.h"

#include "Common/ConstantsAndTypes.h"
#include "DataModel/Zp.h"

template <typename T>
std::istream&
operator>>(std::istream& istr, SubsetSumSolver<T>& solver)
{
  return istr >> *solver._impl;
}

template <typename T>
std::ostream&
operator<<(std::ostream& ostr, const SubsetSumSolver<T>& solver)
{
  return ostr << *solver._impl;
}

template <typename T>
SubsetSumSolver<T>::SubsetSumSolver(std::unique_ptr<SubsetSumImpl<T>>&& impl)
  : _impl(std::move(impl))
{
}

template <typename T>
void
SubsetSumSolver<T>::solve()
{
  _impl->solve();
}

template <typename T>
bool
SubsetSumSolver<T>::result() const
{
  return _impl->result();
}

#define INSTANTIATE_TEMPLATE_SOLVER(type) \
  template class SubsetSumSolver<type>; \
  template std::istream& operator>>(std::istream&, SubsetSumSolver<type>&); \
  template std::ostream& operator<<(std::ostream&, const SubsetSumSolver<type>&);

INSTANTIATE_TEMPLATE_SOLVER(int32_t)
INSTANTIATE_TEMPLATE_SOLVER(int64_t)
INSTANTIATE_TEMPLATE_SOLVER(__int128)
