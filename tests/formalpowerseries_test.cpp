#include "DataModel/FormalPowerSeries.h"
#include "DataModel/Zp.h"
#include "SupplementaryAlgorithms/NTTCalculator.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <gtest/gtest.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

template <typename T>
class FormalPowerSeriesTest : public ::testing::Test
{
public:
  FormalPowerSeriesTest() = default;

protected:
  void SetUp() override
  {
    static_assert(FORMAL_SERIES_MULT_POLICY == NTT);
    if constexpr(is_Zp<T>::value)
    {
//      using integer_type = typename T::underlying_type;
//      const integer_type p = NTTCalculator<smaller_t<integer_type>>::initModRootOrd();
      T::init(2013265921, 0);
//      Zp<smaller_t<integer_type>>::init(p, 0);
    }
    else
      NTTCalculator<T>::initModRootOrd();
  }
  static FormalPowerSeries<T> _a, _b;
};

using FormalPowerSeriesTypes = ::testing::Types<int32_t, int64_t, __int128
    /*Zp<int32_t>, Zp<int64_t>, Zp<__int128>*/>;
TYPED_TEST_SUITE(FormalPowerSeriesTest, FormalPowerSeriesTypes);

template <>
FormalPowerSeries<int32_t> FormalPowerSeriesTest<int32_t>::_a{{5437, 9123, 92, 0, 1111, 3223}};

template <>
FormalPowerSeries<int32_t> FormalPowerSeriesTest<int32_t>::_b{{3312, 17, 123}};

template <>
FormalPowerSeries<int64_t> FormalPowerSeriesTest<int64_t>::_a{{5437, 72731723, 92, 200000000, 1111, 0, 3223}};

template <>
FormalPowerSeries<int64_t> FormalPowerSeriesTest<int64_t>::_b{{91923, 170000, 3414441}};

template <>
FormalPowerSeries<__int128> FormalPowerSeriesTest<__int128>::_a{{96073614, 91293, 621636, 0, 325614337, 0, 1}};

template <>
FormalPowerSeries<__int128> FormalPowerSeriesTest<__int128>::_b{{215562753, 12055053109, 162753109, 412819}};

//template <>
//FormalPowerSeries<Zp<int32_t>> FormalPowerSeriesTest<Zp<int32_t>>::_a{{Zp<int32_t>(547, init_straightforward_value)}};
//
//template <>
//FormalPowerSeries<Zp<int32_t>> FormalPowerSeriesTest<Zp<int32_t>>::_b{{Zp<int32_t>(376, init_straightforward_value),
//  Zp<int32_t>(39, init_straightforward_value), Zp<int32_t>(19, init_straightforward_value)}};
//

TYPED_TEST(FormalPowerSeriesTest, SeriesMultiplicationZeroSize)
{
  FormalPowerSeries<TypeParam> a, b;
  auto c = a * b;
  EXPECT_EQ(c.size(), a.size());
}

TYPED_TEST(FormalPowerSeriesTest, SeriesMultiplication)
{
  if constexpr(is_Zp<TypeParam>::value)
      std::cerr << TypeParam::p << " PPPP\n";
  EXPECT_EQ(TestFixture::_a * TestFixture::_b,
            bruteForceMultiply(TestFixture::_a, TestFixture::_b));
}

TYPED_TEST(FormalPowerSeriesTest, Reciprocal)
{
  if constexpr(is_Zp<TypeParam>::value)
  {
    const FormalPowerSeries<TypeParam> a_reciproc = reciprocal(TestFixture::_a);
    FormalPowerSeries<TypeParam> one = a_reciproc * TestFixture::_a;

    one.resize(TestFixture::_a.size());
    EXPECT_TRUE(one[0] == TypeParam(1));
    EXPECT_TRUE(std::all_of(one.begin() + 1, one.end(), [](const TypeParam val){
      return val == TypeParam(0);
    }));
  }
}
