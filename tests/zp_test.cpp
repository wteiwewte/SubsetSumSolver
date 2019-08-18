#include "Common/Util.h"
#include "DataModel/Zp.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <gtest/gtest.h>

#include <cassert>

template <typename T>
class ZpTest : public ::testing::Test
{
public:
  void SetUp() override
  {
    Zp<T>::init(_p);
    assert((__int128) _p * _p < (__int128) std::numeric_limits<T>::max());
    _aZp = Zp(_a);
    _bZp = Zp(_b);
  }

  T calculateInversion(T val) { return fast_exp(val, _p - 2, _p); }

  static T _p, _a, _b;
  Zp<T> _aZp, _bZp;
};

using ZpTypes = ::testing::Types<int32_t, int64_t, __int128>;
TYPED_TEST_SUITE(ZpTest, ZpTypes);

template <>
int32_t ZpTest<int32_t>::_p = 5437;
template <>
int32_t ZpTest<int32_t>::_a = 32177;
template <>
int32_t ZpTest<int32_t>::_b = 910;

template <>
int64_t ZpTest<int64_t>::_p = 132172163;
template <>
int64_t ZpTest<int64_t>::_a = 231932190;
template <>
int64_t ZpTest<int64_t>::_b = 7912190;

template <>
__int128 ZpTest<__int128>::_p = 960355073614337;
template <>
__int128 ZpTest<__int128>::_a = 215500162753;
template <>
__int128 ZpTest<__int128>::_b = 1205500162753109;

TYPED_TEST(ZpTest, TestZpTakeRemainderTag)
{
  EXPECT_EQ((TypeParam) Zp(TestFixture::_a + TestFixture::_p), TestFixture::_a % TestFixture::_p);
  EXPECT_EQ((TypeParam) Zp(TestFixture::_a + TestFixture::_p, take_remainder),
            TestFixture::_a % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpTakeRemainderAndFixNegativesTag)
{
  EXPECT_EQ((TypeParam) Zp(-TestFixture::_a, take_remainder_and_fix_negatives),
            -TestFixture::_a % TestFixture::_p + TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpInitStraighforwardValueTag)
{
  EXPECT_EQ((TypeParam) Zp(TestFixture::_a % TestFixture::_p, init_straightforward_value),
            TestFixture::_a % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpOperatorT)
{
  EXPECT_EQ((TypeParam) this->_aZp, TestFixture::_a % TestFixture::_p);
  EXPECT_EQ(this->_aZp.value(), TestFixture::_a % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpOperatorPlus)
{
  EXPECT_EQ((TypeParam) this->_aZp, TestFixture::_a % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpOperatorMinus)
{
  EXPECT_EQ((TypeParam) - this->_aZp, TestFixture::_p - TestFixture::_a % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpPostIncrementing)
{
  EXPECT_EQ((TypeParam) this->_aZp++, TestFixture::_a % TestFixture::_p);
  EXPECT_EQ((TypeParam) this->_aZp, (TestFixture::_a + 1) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpPostDecrementing)
{
  EXPECT_EQ((TypeParam) this->_aZp--, TestFixture::_a % TestFixture::_p);
  EXPECT_EQ((TypeParam) this->_aZp, (TestFixture::_a - 1) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpPreIncrementing)
{
  EXPECT_EQ((TypeParam)++ this->_aZp, (TestFixture::_a + 1) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpPreDecrementing)
{
  EXPECT_EQ((TypeParam)-- this->_aZp, (TestFixture::_a - 1) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentSumZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp += this->_bZp),
            (TestFixture::_a + TestFixture::_b) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentDifferenceZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp -= this->_bZp),
            ((TestFixture::_a - TestFixture::_b) % TestFixture::_p + TestFixture::_p) %
                TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentMultiplicationZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp *= this->_bZp),
            (TestFixture::_a * TestFixture::_b) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentDivisionZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp /= this->_bZp),
            (TestFixture::_a * this->calculateInversion(TestFixture::_b)) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentRemainderZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp %= this->_bZp),
            (TestFixture::_a % TestFixture::_p) % (TestFixture::_b % TestFixture::_p));
}

TYPED_TEST(ZpTest, TestZpAssignmentSumInt)
{
  EXPECT_EQ((TypeParam)(this->_aZp += TestFixture::_b),
            (TestFixture::_a + TestFixture::_b) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentDifferenceInt)
{
  EXPECT_EQ((TypeParam)(this->_aZp -= TestFixture::_b),
            ((TestFixture::_a - TestFixture::_b) % TestFixture::_p + TestFixture::_p) %
                TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentMultiplicationInt)
{
  EXPECT_EQ((TypeParam)(this->_aZp *= TestFixture::_b),
            (TestFixture::_a * TestFixture::_b) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpAssignmentDivisionInt)
{
  EXPECT_EQ((TypeParam)(this->_aZp /= TestFixture::_b),
            (TestFixture::_a % TestFixture::_p) / TestFixture::_b);
}

TYPED_TEST(ZpTest, TestZpAssignmentRemainderInt)
{
  EXPECT_EQ((TypeParam)(this->_aZp %= TestFixture::_b),
            (TestFixture::_a % TestFixture::_p) % TestFixture::_b);
}

TYPED_TEST(ZpTest, TestZpInversion)
{
  EXPECT_EQ((this->_aZp / this->_bZp) * this->_bZp, Zp(1));
}

TYPED_TEST(ZpTest, TestZpSumZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp + this->_bZp),
            (TestFixture::_a + TestFixture::_b) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpDifferenceZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp - this->_bZp),
            ((TestFixture::_a - TestFixture::_b) % TestFixture::_p + TestFixture::_p) %
                TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpDivisionZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp / this->_bZp),
            (TestFixture::_a * this->calculateInversion(TestFixture::_b)) % TestFixture::_p);
}

TYPED_TEST(ZpTest, TestZpRemainderZp)
{
  EXPECT_EQ((TypeParam)(this->_aZp % this->_bZp),
            (TestFixture::_a % TestFixture::_p) % (TestFixture::_b % TestFixture::_p));
}
