#include "DataModel/Zp.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <boost/container/small_vector.hpp>
#include <gtest/gtest.h>

#include <cassert>
#include <memory>
#include <optional>
#include <unordered_map>

class ZpTest : public ::testing::Test
{
public:
  ZpTest() : _a(0) {}

  int _a;
};

//template void Zp<int>::init(int, std::size_t);
//
//TEST_F(ZpTest, TestZpSum)
//{
//  Zp<int> a(5), b(7);
//  Zp<int>::init(7, 6);
//  EXPECT_EQ(a + b, Zp(5));
//}
