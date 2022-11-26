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
#include "float_wrapper.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAfWKFloatWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: FloatWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKFloatWrapperTest, FloatWrapperTest_GetValue_001, TestSize.Level1)
{
  float value = 1.001;
  Float ft(value);
  ErrCode result = ft.GetValue(value);
  EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: FloatWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKFloatWrapperTest, FloatWrapperTest_ToString_001, TestSize.Level1)
{
  float value = 1.001;
  Float ft(value);
  EXPECT_EQ("1.001000", ft.ToString());
}

/**
 * @tc.number: FloatWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKFloatWrapperTest, FloatWrapperTest_Box_001, TestSize.Level1)
{
  float value = 1.0;
  Float ft(value);
  float result = ft.Unbox(ft.Box(value));
  EXPECT_EQ(1.0, result);
}

/**
 * @tc.number: FloatWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKFloatWrapperTest, FloatWrapperTest_Parse_001, TestSize.Level1)
{
  float value = 1.0;
  Float ft(value);
  float result = ft.Unbox(ft.Parse(ft.ToString()));
  EXPECT_EQ(1.0, result);
}
}
}