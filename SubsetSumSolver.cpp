//
// Created by jan on 07.04.19.
//

#include "Common/ConstantsAndTypes.h"
#include "SubsetSumSolver.h"

template<typename T>
std::istream& operator>>(std::istream& istr, SubsetSumSolver<T>& solver)
{
    return istr >> *solver._impl;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const SubsetSumSolver<T>& solver)
{
    return ostr << *solver._impl;
}

template<typename T>
SubsetSumSolver<T>::SubsetSumSolver(std::unique_ptr<SubsetSumImpl<T>>&& impl) : _impl(std::move(impl))
{

}

template<typename T>
void SubsetSumSolver<T>::solve()
{
    _impl->solve();
}

template<typename T>
bool SubsetSumSolver<T>::result() const
{
    return _impl->result();
}

template class SubsetSumSolver<Int>;

template std::istream& operator>>(std::istream&, SubsetSumSolver<Int>&);
template std::ostream& operator<<(std::ostream&, const SubsetSumSolver<Int>&);