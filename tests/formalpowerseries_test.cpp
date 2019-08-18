#include "DataModel/FormalPowerSeries.h"
#include "DataModel/Zp.h"
#include "SupplementaryAlgorithms/NTTCalculator.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <boost/container/small_vector.hpp>
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
    using integer_type = typename T::underlying_type;
    const integer_type p = NTTCalculator<integer_type>::initModRootOrd();
    T::init(p, 0);
  }
};

using FormalPowerSeriesTypes = ::testing::Types<Zp<int64_t>, Zp<__int128>>;
TYPED_TEST_SUITE(FormalPowerSeriesTest, FormalPowerSeriesTypes);

TYPED_TEST(FormalPowerSeriesTest, SeriesMultZeroSize)
{
  FormalPowerSeries<TypeParam> a, b;
  auto c = a * b;
  EXPECT_EQ(c.size(), a.size());
}
