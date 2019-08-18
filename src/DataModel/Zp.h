//
// Created by jan on 20.04.19.
//

#ifndef SUBSET_SUM_SOLVER_ZPFIELD_H
#define SUBSET_SUM_SOLVER_ZPFIELD_H

#include "Common/ConstantsAndTypes.h"

#include <vector>

struct take_remainder_t
{
  explicit take_remainder_t() = default;
};

inline constexpr take_remainder_t take_remainder{};

struct take_remainder_and_fix_negatives_t
{
  explicit take_remainder_and_fix_negatives_t() = default;
};

inline constexpr take_remainder_and_fix_negatives_t take_remainder_and_fix_negatives{};

struct init_straightforward_value_t
{
  explicit init_straightforward_value_t() = default;
};

inline constexpr init_straightforward_value_t init_straightforward_value{};

template <typename T>
class Zp
{
public:
  using underlying_type = T;

  static inline T p = -1;
  static void init(const T p, const std::size_t inversionsToInit = 0);
  constexpr Zp<T>() = default;
  constexpr Zp<T>(const Zp<T>& other) = default;

  explicit constexpr Zp<T>(T value,
                           take_remainder_t,
                           typename std::enable_if_t<std::is_integral_v<T>, int> = 0)
    : _value(value % p)
  {
    assert(value >= 0);
  }

  explicit constexpr Zp<T>(T value,
                           take_remainder_and_fix_negatives_t,
                           typename std::enable_if_t<std::is_integral_v<T>, int> = 0)
    : _value(value % p + (value < 0 ? p : 0))
  {
  }

  explicit constexpr Zp<T>(T value,
                           init_straightforward_value_t,
                           typename std::enable_if_t<std::is_integral_v<T>, int> = 0)
    : _value(value)
  {
  }

  explicit constexpr Zp<T>(T value, typename std::enable_if_t<std::is_integral_v<T>, int> = 0)
    : Zp<T>(value, take_remainder_and_fix_negatives)
  {
  }

  constexpr operator T() const { return _value; }
  constexpr T value() const { return _value; }

  constexpr Zp<T> operator+() const { return Zp<T>(_value); }
  constexpr Zp<T> operator-() const { return Zp<T>(p - _value); }

  //Incrementing/decrementing functions not taking remainder mod p due to performance reasons
  constexpr Zp<T>& operator++()
  {
    ++_value;
    return *this;
  }
  constexpr Zp<T>& operator--()
  {
    --_value;
    return *this;
  }

  constexpr Zp<T> operator++(int) { return Zp<T>(_value++); }
  constexpr Zp<T> operator--(int) { return Zp<T>(_value--); }

  constexpr Zp<T>& operator+=(const Zp<T> other)
  {
    _value = (_value + p + other._value) % p;
    return *this;
  }
  constexpr Zp<T>& operator-=(const Zp<T> other)
  {
    _value = (_value + p - other._value) % p;
    return *this;
  }
  constexpr Zp<T>& operator*=(const Zp<T> other)
  {
    _value = (_value * other._value) % p;
    return *this;
  }
  constexpr Zp<T>& operator/=(const Zp<T> other)
  {
    _value = (_value * Zp<T>::getInversion(other).value()) % p;
    return *this;
  }
  constexpr Zp<T>& operator%=(const Zp<T> other)
  {
    _value %= other._value;
    return *this;
  }

  template <typename U>
  constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>& operator+=(const U u)
  {
    _value = (_value + u) % p;
    return *this;
  }
  template <typename U>
  constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>& operator-=(const U u)
  {
    _value = (_value - u) % p;
    _value += _value < 0 ? p : 0;
    return *this;
  }
  template <typename U>
  constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>& operator*=(const U u)
  {
    _value = (_value * u) % p;
    return *this;
  }
  template <typename U>
  constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>& operator/=(const U u)
  {
    _value = (_value / u) % p;
    return *this;
  }
  template <typename U>
  constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>& operator%=(const U u)
  {
    _value = (_value % u) % p;
    return *this;
  }

private:
  static inline bool initialized = false;
  static inline std::vector<T> inversions_mod_p{};

  static Zp<T> getInversion(const Zp<T> value);

  template <typename U>
  friend constexpr bool operator<(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr bool operator<=(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr bool operator>(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr bool operator>=(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr bool operator==(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr bool operator!=(const Zp<U> a, const Zp<U> b);

  template <typename U>
  friend constexpr Zp<U> operator+(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr Zp<U> operator-(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr Zp<U> operator*(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr Zp<U> operator/(const Zp<U> a, const Zp<U> b);
  template <typename U>
  friend constexpr Zp<U> operator%(const Zp<U> a, const Zp<U> b);

  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator+(const U a, const Zp<T1> b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator-(const U a, const Zp<T1> b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator*(const U a, const Zp<T1> b);

  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator+(const Zp<T1> a, const U b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator-(const Zp<T1> a, const U b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator*(const Zp<T1> a, const U b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator/(const Zp<T1> a, const U b);
  template <typename T1, typename U>
  friend constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T1>>
  operator%(const Zp<T1> a, const U b);

  T _value;
};

template <typename U>
constexpr bool
operator<(const Zp<U> a, const Zp<U> b)
{
  return a._value < b._value;
}
template <typename U>
constexpr bool
operator<=(const Zp<U> a, const Zp<U> b)
{
  return a._value <= b._value;
}
template <typename U>
constexpr bool
operator>(const Zp<U> a, const Zp<U> b)
{
  return a._value > b._value;
}
template <typename U>
constexpr bool
operator>=(const Zp<U> a, const Zp<U> b)
{
  return a._value >= b._value;
}
template <typename U>
constexpr bool
operator==(const Zp<U> a, const Zp<U> b)
{
  return a._value == b._value;
}
template <typename U>
constexpr bool
operator!=(const Zp<U> a, const Zp<U> b)
{
  return a._value != b._value;
}

template <typename U>
constexpr Zp<U>
operator+(const Zp<U> a, const Zp<U> b)
{
  return Zp<U>(a._value + b._value);
}
template <typename U>
constexpr Zp<U>
operator-(const Zp<U> a, const Zp<U> b)
{
  return Zp<U>(a._value + Zp<U>::p - b._value);
}
template <typename U>
constexpr Zp<U> operator*(const Zp<U> a, const Zp<U> b)
{
  return Zp<U>(a._value * b._value);
}
template <typename U>
constexpr Zp<U>
operator/(const Zp<U> a, const Zp<U> b)
{
  return a * Zp<U>::getInversion(b);
}
template <typename U>
constexpr Zp<U>
operator%(const Zp<U> a, const Zp<U> b)
{
  return Zp<U>(a._value % b._value);
}

template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator+(const U a, const Zp<T> b)
{
  return Zp<T>(a + b._value);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator-(const U a, const Zp<T> b)
{
  return Zp<T>(a - b._value, take_remainder_and_fix_negatives);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>> operator*(const U a, const Zp<T> b)
{
  return Zp<T>(a * b._value);
}

template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator+(const Zp<T> a, const U b)
{
  return Zp<T>(a._value + b);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator-(const Zp<T> a, const U b)
{
  return Zp<T>(a._value - b, true);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>> operator*(const Zp<T> a, const U b)
{
  return Zp<T>(a._value * b);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator%(const Zp<T> a, const U b)
{
  return Zp<T>(a._value % b);
}
template <typename T, typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Zp<T>>
operator/(const Zp<T> a, const U b)
{
  return Zp<T>(a._value / b);
}

template <class T>
Zp(T)->Zp<T>;
template <class T>
Zp(T, take_remainder_t)->Zp<T>;
template <class T>
Zp(T, take_remainder_and_fix_negatives_t)->Zp<T>;
template <class T>
Zp(T, init_straightforward_value_t)->Zp<T>;

template <typename>
struct is_Zp : std::false_type
{
};

template <typename T>
struct is_Zp<Zp<T>> : std::true_type
{
};

#endif //SUBSET_SUM_SOLVER_ZPFIELD_H
