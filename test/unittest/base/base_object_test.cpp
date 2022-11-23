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
#include "light_refbase.h"
#include "base_def.h"
#include "base_object.h"
#include "bool_wrapper.h"
#include "zchar_wrapper.h"
#include "byte_wrapper.h"
#include "user_object_wrapper.h"
#include "short_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "float_wrapper.h"
#include "double_wrapper.h"
#include "string_wrapper.h"
#include "refbase.h"

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAFwkBaseObjectTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AAFwkBaseObjectTest::SetUpTestCase(void)
{}

void AAFwkBaseObjectTest::TearDownTestCase(void)
{}

void AAFwkBaseObjectTest::SetUp(void)
{}

void AAFwkBaseObjectTest::TearDown(void)
{}

/**
 * @tc.number: BaseObject_test_001
 * @tc.name: Query
 * @tc.desc: test Query function and iid == g_IID_IWeakReferenceSource.
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_001, TestSize.Level1)
{
    Object object;
    InterfaceID iid = {
        0xbc3f5250, 0x34d7, 0x42d2, 0x9b40, {0xf, 0xf, 0xc, 0xe, 0x8, 0x3, 0xf, 0xd, 0x4, 0x0, 0x6, 0x1}
    };
    EXPECT_TRUE(object.Query(iid));
}

/**
 * @tc.number: BaseObject_test_002
 * @tc.name: Query
 * @tc.desc: test Query function and iid == g_IID_IInterface.
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_002, TestSize.Level1)
{
    Object object;
    InterfaceID iid = {
        0x00000000, 0x0000, 0x0000, 0x0000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1}
    };
    EXPECT_TRUE(object.Query(iid));
}

/**
 * @tc.number: BaseObject_test_003
 * @tc.name: GetInterfaceID
 * @tc.desc: test GetInterfaceID function and object == static_cast<IObject *>(this).
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_003, TestSize.Level1)
{
    Object object;
    InterfaceID iid = {
        0x8321f710, 0xa0c0, 0x4cbe, 0xbfbc, {0x5, 0xa, 0x7, 0x8, 0xf, 0x1, 0x3, 0x1, 0x2, 0xb, 0x1, 0xb}
    };
    EXPECT_EQ(iid, object.GetInterfaceID(object.Query(iid)));
}

/**
 * @tc.number: BaseObject_test_004
 * @tc.name: GetInterfaceID
 * @tc.desc: test GetInterfaceID function and object == static_cast<IWeakReferenceSource *>(this).
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_004, TestSize.Level1)
{
    Object object;
    InterfaceID iid = {
        0xbc3f5250, 0x34d7, 0x42d2, 0x9b40, {0xf, 0xf, 0xc, 0xe, 0x8, 0x3, 0xf, 0xd, 0x4, 0x0, 0x6, 0x1}
    };
    EXPECT_EQ(iid, object.GetInterfaceID(object.Query(iid)));
}

/**
 * @tc.number: BaseObject_test_005
 * @tc.name: GetInterfaceID
 * @tc.desc: test GetInterfaceID function and object is Any value.
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_005, TestSize.Level1)
{
    Object object;
    InterfaceID iid = {
        0x26ab1978, 0x1d11, 0x4a4f, 0x826d, {0x6, 0x1, 0x7, 0x8, 0x5, 0xc, 0x0, 0x4, 0x8, 0xc, 0xc, 0xa}
    };
    EXPECT_EQ(InterfaceID::Empty, object.GetInterfaceID(object.Query(iid)));
}


/**
 * @tc.number: BaseObject_test_006
 * @tc.name: GetWeakReference
 * @tc.desc: test GetWeakReference function and addr is not nullptr.
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_006, TestSize.Level1)
{
    Object object;
    sptr<IWeakReference> weakRef = nullptr;
    EXPECT_EQ(ERR_OK, object.GetWeakReference(weakRef));
}

/**
 * @tc.number: BaseObject_test_007
 * @tc.name: ToString
 * @tc.desc: test ToString function.
 */
HWTEST_F(AAFwkBaseObjectTest, BaseObject_test_007, TestSize.Level1)
{
    Object object;
    object.ToString();
}

/**
 * @tc.number: UserObject_test_001
 * @tc.name: ToString
 * @tc.desc: test ToString function and value_ is nullptr.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_001, TestSize.Level1)
{
    UserObject userObject(nullptr);
    EXPECT_EQ("", userObject.ToString());
}

/**
 * @tc.number: UserObject_test_002
 * @tc.name: Box
 * @tc.desc: test Box function and value is nullptr.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_002, TestSize.Level1)
{
    UserObject userObject(nullptr);
    EXPECT_EQ(nullptr, userObject.Box(nullptr));
}

/**
 * @tc.number: UserObject_test_003
 * @tc.name: Unbox
 * @tc.desc: test Unbox function and object is nullptr.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_003, TestSize.Level1)
{
    UserObject userObject(nullptr);
    EXPECT_EQ(nullptr, userObject.Unbox(nullptr));
}

/**
 * @tc.number: UserObject_test_004
 * @tc.name: Parse
 * @tc.desc: test Parse function and str is empty.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_004, TestSize.Level1)
{
    UserObject userObject(nullptr);
    std::string str = "";
    EXPECT_EQ(nullptr, userObject.Parse(str));
}

/**
 * @tc.number: UserObject_test_005
 * @tc.name: Parse
 * @tc.desc: test Parse function and SPLIT is does not exist.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_005, TestSize.Level1)
{
    UserObject userObject(nullptr);
    std::string str = "acd";
    EXPECT_EQ(nullptr, userObject.Parse(str));
}

/**
 * @tc.number: UserObject_test_006
 * @tc.name: Parse
 * @tc.desc: test Parse function and SPLIT is exist.
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_006, TestSize.Level1)
{
    UserObject userObject(nullptr);
    std::string str = "a#cd";
    EXPECT_EQ(nullptr, userObject.Parse(str));
}

/**
 * @tc.number: UserObject_test_007
 * @tc.name: Parse
 * @tc.desc: test Parse function and str == "true".
 */
HWTEST_F(AAFwkBaseObjectTest, UserObject_test_007, TestSize.Level1)
{
    Boolean boolean(true);
    std::string str = "true";
    boolean.Parse(str);
    EXPECT_EQ("true", boolean.ToString());
}
}  // namespace AAFwk
}  // namespace OHOS
