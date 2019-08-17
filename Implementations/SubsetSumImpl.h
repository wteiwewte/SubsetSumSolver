//
// Created by jan on 07.04.19.
//

#ifndef SUBSET_SUM_SOLVER_SUBSETSUMIMPL_H
#define SUBSET_SUM_SOLVER_SUBSETSUMIMPL_H

#include <iostream>
#include <vector>

template<typename T>
class SubsetSumImpl;

template<typename T>
std::istream& operator>>(std::istream&, SubsetSumImpl<T>&);

template<typename T>
std::ostream& operator<<(std::ostream&, const SubsetSumImpl<T>&);

template<typename T>
class SubsetSumImpl
{
public:
    SubsetSumImpl() = default;
    SubsetSumImpl(const SubsetSumImpl<T>& otherImpl);
    virtual void solve() = 0;
    virtual ~SubsetSumImpl() {};
    bool result() const { return _result; }
protected:
    std::vector<T> _tab;
    T _target;
//    std::make_unsigned_t<T> _size;
    int64_t _size;
    bool _result = false;
private:
    virtual void init() = 0;
    friend std::istream& operator>> <T>(std::istream&, SubsetSumImpl<T>&);
    friend std::ostream& operator<< <T>(std::ostream&, const SubsetSumImpl<T>&);
};


#endif //SUBSET_SUM_SOLVER_SUBSETSUMIMPL_H
