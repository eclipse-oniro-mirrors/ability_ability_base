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
#include "bool_wrapper.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAfWKBoolWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: BoolWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKBoolWrapperTest, BoolWrapperTest_GetValue_001, TestSize.Level1)
{
  bool value = true;
  Boolean bl(value);
  ErrCode result = bl.GetValue(value);
  EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: BoolWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKBoolWrapperTest, BoolWrapperTest_ToString_001, TestSize.Level1)
{
  bool value = true;
  Boolean bl(value);
  EXPECT_EQ("true", bl.ToString());
}

/**
 * @tc.number: BoolWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKBoolWrapperTest, BoolWrapperTest_Box_001, TestSize.Level1)
{
  bool value = true;
  Boolean bl(value);
  bool result = bl.Unbox(bl.Box(value));
  EXPECT_EQ(true, result);
}

/**
 * @tc.number: BoolWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKBoolWrapperTest, BoolWrapperTest_Parse_001, TestSize.Level1)
{
  bool value = true;
  Boolean bl(value);
  bool result = bl.Unbox(bl.Parse("true"));
  EXPECT_EQ(true, result);
}

/**
 * @tc.number: BoolWrapperTest_Parse_002
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKBoolWrapperTest, BoolWrapperTest_Parse_002, TestSize.Level1)
{
  bool value = true;
  Boolean bl(value);
  bool result = bl.Unbox(bl.Parse("false"));
  EXPECT_EQ(false, result);
}
}
}