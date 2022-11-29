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
#include "user_object_wrapper.h"
#include "user_object_base.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
namespace {
const std::string SPLIT = "#";
};
class AAfWKUserObjectWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: UserObjectWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_GetValue_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    ErrCode result = useObjectValue.GetValue(value);
    EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: UserObjectWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_ToString_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ("", useObjectValue.ToString());
}

/**
 * @tc.number: UserObjectWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Box_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ(nullptr, useObjectValue.Box(value));
}

/**
 * @tc.number: UserObjectWrapperTest_Unbox_001
 * @tc.name: Unbox
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unbox_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    sptr<IUserObject> object = new (std::nothrow) UserObject(value);
    EXPECT_EQ(value, useObjectValue.Unbox(object));
}

/**
 * @tc.number: UserObjectWrapperTest_Unbox_002
 * @tc.name: Unbox
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unbox_002, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    sptr<IUserObject> object = new (std::nothrow) UserObject(value);
    object = nullptr;
    EXPECT_EQ(nullptr, useObjectValue.Unbox(object));
}

/**
 * @tc.number: UserObjectWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Parse_001, TestSize.Level1)
{   
    std::string str = "";
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ(nullptr, useObjectValue.Parse(str));
}
}
}