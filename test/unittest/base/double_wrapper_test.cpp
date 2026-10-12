/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#define private public
#define protected public
#include "double_wrapper.h"
#undef private
#undef protected

#include <cmath>
#include <limits>

#include "int_wrapper.h"

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAfWKDoubleWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: DoubleWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_GetValue_001, TestSize.Level1)
{
  double value = 1000;
  Double db(value);
  ErrCode result = db.GetValue(value);
  EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: DoubleWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_ToString_001, TestSize.Level1)
{
  double value = 1000;
  Double db(value);
  EXPECT_EQ("1000.000000", db.ToString());
}

/**
 * @tc.number: DoubleWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Box_001, TestSize.Level1)
{
  double value = 1000;
  Double db(value);
  double result = db.Unbox(db.Box(value));
  EXPECT_EQ(1000, result);
}

/**
 * @tc.number: DoubleWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Parse_001, TestSize.Level1)
{
  double value = 1000;
  Double db(value);
  double result = db.Unbox(db.Parse(db.ToString()));
  EXPECT_EQ(1000, result);
}

/**
 * @tc.number: DoubleWrapperTest_Parse_002
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Parse_002, TestSize.Level1)
{
    double value = 3.14;
    Double db(value);
    double result = db.Unbox(db.Parse(db.ToString()));
    EXPECT_EQ(3.14, result);
}

/**
 * @tc.number: DoubleWrapperTest_Parse_003
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Parse_003, TestSize.Level1)
{
    sptr<IDouble> result = Double::Parse("");
    EXPECT_EQ(result, nullptr);
}

/**
 * @tc.number: DoubleWrapperTest_Parse_004
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Parse_004, TestSize.Level1)
{
    sptr<IDouble> result = Double::Parse("abc123");
    EXPECT_EQ(result, nullptr);
}

/**
 * @tc.number: DoubleWrapperTest_Parse_005
 * @tc.name: Parse
 * @tc.desc: partially consumed input is rejected (full-string consumption required).
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Parse_005, TestSize.Level1)
{
    EXPECT_EQ(Double::Parse("1.5.2"), nullptr);
    EXPECT_EQ(Double::Parse("3.14 "), nullptr);
}

/**
 * @tc.number: DoubleWrapperTest_Equals_001
 * @tc.name: Equals
 * @tc.desc: mathematically equal doubles computed by different paths are equal within relative epsilon.
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Equals_001, TestSize.Level1)
{
    Double left(0.1 + 0.2);
    Double right(0.3);
    EXPECT_TRUE(left.Equals(right));
}

/**
 * @tc.number: DoubleWrapperTest_Equals_002
 * @tc.name: Equals
 * @tc.desc: distinct near-zero doubles are not equal (no absolute tolerance floor).
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Equals_002, TestSize.Level1)
{
    Double zero(0.0);
    Double small(1e-300);
    Double small2(2e-300);
    EXPECT_FALSE(zero.Equals(small));
    EXPECT_FALSE(small.Equals(small2));
}

/**
 * @tc.number: DoubleWrapperTest_Equals_003
 * @tc.name: Equals
 * @tc.desc: NaN is not equal to anything; same infinities are equal; infinity differs from finite.
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Equals_003, TestSize.Level1)
{
    Double nanLeft(std::numeric_limits<double>::quiet_NaN());
    Double nanRight(std::numeric_limits<double>::quiet_NaN());
    EXPECT_FALSE(nanLeft.Equals(nanRight));
    Double infLeft(HUGE_VAL);
    Double infRight(HUGE_VAL);
    EXPECT_TRUE(infLeft.Equals(infRight));
    Double finite(1.0);
    EXPECT_FALSE(infLeft.Equals(finite));
}

/**
 * @tc.number: DoubleWrapperTest_Equals_004
 * @tc.name: Equals
 * @tc.desc: comparing with a different wrapper type returns false.
 */
HWTEST_F(AAfWKDoubleWrapperTest, DoubleWrapperTest_Equals_004, TestSize.Level1)
{
    Double doubleObj(1.5);
    Integer intObj(1);
    EXPECT_FALSE(doubleObj.Equals(intObj));
}
}
}