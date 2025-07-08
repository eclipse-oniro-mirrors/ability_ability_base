/*
* Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include <memory>
#include <regex>

#include "array_wrapper.h"
#include "bool_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#define private public
#define protected public
#include "pac_map.h"
#undef protected
#undef private
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "user_object_base.h"

namespace OHOS {
namespace AppExecFwk {
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::AppExecFwk;

#define PAC_MPA_TEST_INT 1000
#define PAC_MAP_TEST_LONG (-1000)
#define PAC_MAP_TEST_FLOAT 1.0f
#define PAC_MAP_TEST_DOUBLE 3.1415926

/*
* Description：Test for data type of base: like int, short, long std::string etc.
*/
class PacMapSecondTest : public testing::Test {
public:
    PacMapSecondTest() : pacmap_(nullptr)
    {}
    ~PacMapSecondTest()
    {}

    std::shared_ptr<PacMap> pacmap_ = nullptr;
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void PacMapSecondTest::SetUpTestCase(void)
{}

void PacMapSecondTest::TearDownTestCase(void)
{}

void PacMapSecondTest::SetUp()
{
    pacmap_ = std::make_shared<PacMap>();
}

void PacMapSecondTest::TearDown()
{}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0100
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0100 start";
    long size = 3;
    InterfaceID id = g_IID_IArray;
    AAFwk::Array array1(size, id);
    AAFwk::Array array2(size, id);
    AAFwk::IInterface *one_interface = static_cast<OHOS::AAFwk::IArray*>(&array1);
    AAFwk::IInterface *two_interface = static_cast<OHOS::AAFwk::IArray*>(&array2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, false);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0100 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0200
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0200 start";
    long size1 = 3;
    long size2 = 4;
    InterfaceID id = g_IID_IArray;
    AAFwk::Array array1(size1, id);
    AAFwk::Array array2(size2, id);
    AAFwk::IInterface *one_interface = static_cast<OHOS::AAFwk::IArray*>(&array1);
    AAFwk::IInterface *two_interface = static_cast<OHOS::AAFwk::IArray*>(&array2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, false);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0200 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0300
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0300 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IBoolean);
    if (ao1 != nullptr) {
        ao1->Set(0, Boolean::Box(true));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IBoolean);
    if (ao2 != nullptr) {
        ao2->Set(0, Boolean::Box(true));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0300 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0400
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0400, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0400 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IByte);
    if (ao1 != nullptr) {
        ao1->Set(0, Byte::Box('a'));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IByte);
    if (ao2 != nullptr) {
        ao2->Set(0, Byte::Box('a'));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0400 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0500
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0500, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0500 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IShort);
    if (ao1 != nullptr) {
        ao1->Set(0, Short::Box(PAC_MPA_TEST_INT));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IShort);
    if (ao2 != nullptr) {
        ao2->Set(0, Short::Box(PAC_MPA_TEST_INT));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0500 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0600
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0600, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0600 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IInteger);
    if (ao1 != nullptr) {
        ao1->Set(0, Integer::Box(PAC_MPA_TEST_INT));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IInteger);
    if (ao2 != nullptr) {
        ao2->Set(0, Integer::Box(PAC_MPA_TEST_INT));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0600 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0700
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0700, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0700 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_ILong);
    if (ao1 != nullptr) {
        ao1->Set(0, Long::Box(PAC_MAP_TEST_LONG));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_ILong);
    if (ao2 != nullptr) {
        ao2->Set(0, Long::Box(PAC_MAP_TEST_LONG));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_TRUE(pacmap_ != nullptr);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0700 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_0800
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_0800, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0800 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IFloat);
    if (ao1 != nullptr) {
        ao1->Set(0, Float::Box(PAC_MAP_TEST_FLOAT));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IFloat);
    if (ao2 != nullptr) {
        ao2->Set(0, Float::Box(PAC_MAP_TEST_FLOAT));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_0800 end";
}

/**
* @tc.number: AppExecFwk_PacMap_CompareArrayData_1000
* @tc.name: CompareArrayData
* @tc.desc: Verify CompareArrayData.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_CompareArrayData_1000, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_1000 start";
    sptr<AAFwk::IArray> ao1 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IString);
    if (ao1 != nullptr) {
        ao1->Set(0, String::Box("<~!@#$%^&*()_+>特殊字符"));
    }
    sptr<AAFwk::IArray> ao2 = new (std::nothrow) AAFwk::Array(1, AAFwk::g_IID_IString);
    if (ao2 != nullptr) {
        ao2->Set(0, String::Box("<~!@#$%^&*()_+>特殊字符"));
    }
    AAFwk::IInterface *one_interface = static_cast<AAFwk::IArray*>(ao1);
    AAFwk::IInterface *two_interface = static_cast<AAFwk::IArray*>(ao2);
    bool result = pacmap_->CompareArrayData(one_interface, two_interface);
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_CompareArrayData_1000 end";
}

/**
* @tc.number: AppExecFwk_PacMap_Equals_0100
* @tc.name: Equals
* @tc.desc: Verify Equals.
* @tc.require:
*/
HWTEST_F(PacMapSecondTest, AppExecFwk_PacMap_Equals_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_Equals_0100 start";
    Object other;
    EXPECT_EQ(pacmap_->Equals(other), false);
    GTEST_LOG_(INFO) << "AppExecFwk_PacMap_Equals_0100 end";
}
}  // namespace AppExecFwk
}  // namespace OHOS
