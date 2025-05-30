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
#include "short_wrapper.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAfWKShortWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: ZcharWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_GetValue_001, TestSize.Level1)
{
    short value = 0;
    Short shortValue(value);
    ErrCode result = shortValue.GetValue(value);
    EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: ZcharWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_ToString_001, TestSize.Level1)
{
    short value = 0;
    Short shortValue(value);
    EXPECT_EQ("0", shortValue.ToString());
}

/**
 * @tc.number: ZcharWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_Box_001, TestSize.Level1)
{
    short value = 0;
    Short shortValue(value);
    short result = shortValue.Unbox(shortValue.Box(value));
    EXPECT_EQ(0, result);
}

/**
 * @tc.number: ZcharWrapperTest_Ubox_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_Ubox_001, TestSize.Level1)
{
    
    short value = 0;
    Short shortValue(value);
    short result = shortValue.Unbox(shortValue.Box(value));
    EXPECT_EQ(0, result);
}

/**
 * @tc.number: ZcharWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_Parse_001, TestSize.Level1)
{
    short value = 0;
    Short shortValue(value);
    short result = shortValue.Unbox(shortValue.Parse(shortValue.ToString()));
    EXPECT_EQ(0, result);
}

/**
 * @tc.number: ShortWrapperTest_Parse_003
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_Parse_003, TestSize.Level1)
{
    sptr<IShort> result = Short::Parse("");
    EXPECT_EQ(result, nullptr);
}

/**
 * @tc.number: ShortWrapperTest_Parse_004
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKShortWrapperTest, ShortWrapperTest_Parse_004, TestSize.Level1)
{
    sptr<IShort> result = Short::Parse("abc123");
    EXPECT_EQ(result, nullptr);
}
}
}