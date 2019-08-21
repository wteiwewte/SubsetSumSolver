//
// Created by jan on 20.04.19.
//

#include "DynamicProgrammingImpl.h"

#include "Common/ConstantsAndTypes.h"

template <typename T>
DynamicProgrammingImpl<T>::DynamicProgrammingImpl(const SubsetSumImpl<T>& otherImpl)
  : SubsetSumImpl<T>(otherImpl), _allocationSucceded(true)
{
  init();
}

template <typename T>
void
DynamicProgrammingImpl<T>::solve()
{
  if(!_allocationSucceded)
  {
    std::cerr << "Allocation failure!\n";
    return;
  }

  for (T i = 1; i <= _size; ++i)
    for (T j = 1; j <= _target; ++j)
    {
      if (_tab[i] > j)
        _doesSumExist[i][j] = _doesSumExist[i - 1][j];
      else
        _doesSumExist[i][j] = _doesSumExist[i - 1][j] || _doesSumExist[i - 1][j - _tab[i]];
    }

  _result = _doesSumExist[_size][_target];
}

template <typename T>
void
DynamicProgrammingImpl<T>::init()
{
  _doesSumExist = new(std::nothrow) bool*[_size + 1];
  if(_doesSumExist)
  {
    for (T i = 0; i <= _size; ++i)
    {
      _doesSumExist[i] = new(std::nothrow) bool[_target + 1];
      if(_doesSumExist[i])
      {
        for (T j = 0; j <= _target; ++j)
          _doesSumExist[i][j] = false;
      }
      else
      {
        _allocationSucceded = false;
        return;
      }
    }

    for (T i = 0; i <= _size; ++i)
      _doesSumExist[i][0] = true;
  }
  else
    _allocationSucceded = false;
}

template <typename T>
DynamicProgrammingImpl<T>::~DynamicProgrammingImpl()
{
  for (T i = 0; i <= _size; ++i)
    delete[] _doesSumExist[i];

  delete[] _doesSumExist;
}

template class DynamicProgrammingImpl<Int>;
