/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "securec.h"
#include "want.h"
#include "want_params.h"
#include "want/include/want.h"
#include "want_manager.h"
#include "bool_wrapper.h"
#include "double_wrapper.h"
#include "int_wrapper.h"
#include "string_wrapper.h"

using namespace testing::ext;
using namespace OHOS::AAFwk;


class CWantTest : public testing::Test {
public:
    static void SetUpTestCase(){};

    static void TearDownTestCase(){};

    void SetUp(){};

    void TearDown(){};
};

void initElementWithDynamicMemory(AbilityBase_Element& element)
{
    const char bundle[] = "testBundleName";
    const char module[] = "testModuleName";
    const char ability[] = "testAbilityName";

    element.bundleName = new char[strlen(bundle) + 1];
    element.moduleName = new char[strlen(module) + 1];
    element.abilityName = new char[strlen(ability) + 1];

    strcpy_s(element.bundleName, strlen(bundle) + 1, bundle);
    strcpy_s(element.moduleName, strlen(module) + 1, module);
    strcpy_s(element.abilityName, strlen(ability) + 1, ability);
}

void ReleaseElementMemory(AbilityBase_Element& element)
{
    delete[] element.bundleName;
    delete[] element.moduleName;
    delete[] element.abilityName;
}

/**
 * @tc.number:OH_AbilityBase_TransformToWant_001
 * @tc.desc: Function test OH_AbilityBase_TransformToWant_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_TransformToWant_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    const char * intKey = "intKey";
    int32_t intvalue = 10;
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantInt32Param(want, intKey, intvalue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    
    const char * doubleKey = "doubleKey";
    double doublevalue = 3.14f;
    errCode = OH_AbilityBase_SetWantDoubleParam(want, doubleKey, doublevalue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    
    const char * boolKey = "boolKey";
    bool boolvalue = true;
    errCode = OH_AbilityBase_SetWantBoolParam(want, boolKey, boolvalue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    
    const char * strKey = "strKey";
    const char * strValue = "strValue";
    errCode = OH_AbilityBase_SetWantCharParam(want, strKey, strValue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    
    const char * fdKey = "fdKey";
    int32_t fd = 1;
    errCode = OH_AbilityBase_AddWantFd(want, fdKey, fd);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    OHOS::AAFwk::Want *want2 = new (std::nothrow) OHOS::AAFwk::Want();
    errCode = OHOS::AAFwk::CWantManager::TransformToWant(*want, true, *want2);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ReleaseElementMemory(element);
}

/**
 * @tc.number:OH_AbilityBase_TransformToWant_002
 * @tc.desc: Function test OH_AbilityBase_TransformToWant_002
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_TransformToWant_002, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    OHOS::AAFwk::Want *want2 = new (std::nothrow) OHOS::AAFwk::Want();
    OHOS::AAFwk::WantParams wantParams;
    std::string keyStr1 = "12345667";
    bool valueBool = true;
    wantParams.SetParam(keyStr1, OHOS::AAFwk::Boolean::Box(valueBool));
    
    std::string keyStr2 = "1234566";
    std::string valueString = "123";
    wantParams.SetParam(keyStr2, OHOS::AAFwk::String::Box(valueString));
    
    std::string keyStr3 = "123456";
    int valueInt = 111;
    wantParams.SetParam(keyStr3, OHOS::AAFwk::Integer::Box(valueInt));
    
    std::string keyStr4 = "12345";
    double valueDouble = 3.14f;
    wantParams.SetParam(keyStr4, OHOS::AAFwk::Double::Box(valueDouble));
    
    AbilityBase_ErrorCode errCode = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(*want2, false, *want);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ReleaseElementMemory(element);
}

HWTEST_F(CWantTest, ShouldTransformEmptyWant, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    EXPECT_TRUE(cwant.uri.empty());
    EXPECT_TRUE(cwant.params.empty());
    EXPECT_TRUE(cwant.intParams.empty());
    EXPECT_TRUE(cwant.boolParams.empty());
    EXPECT_TRUE(cwant.doubleParams.empty());
    EXPECT_TRUE(cwant.fds.empty());
}

HWTEST_F(CWantTest, ShouldTransformUriCorrectly, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    want.SetUri("test_uri");
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    EXPECT_EQ(cwant.uri, "test_uri");
}

HWTEST_F(CWantTest, ShouldTransformStringParams, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    OHOS::AAFwk::WantParams params;
    params.SetParam("str_key", String::Box("str_value"));
    want.SetParams(params);
    
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_EQ(cwant.params.size(), 1);
    EXPECT_EQ(cwant.params["str_key"], "str_value");
}

HWTEST_F(CWantTest, ShouldTransformIntParams, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    OHOS::AAFwk::WantParams params;
    params.SetParam("int_key", Integer::Box(123));
    want.SetParams(params);
    
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_EQ(cwant.intParams.size(), 1);
    EXPECT_EQ(cwant.intParams["int_key"], 123);
}

HWTEST_F(CWantTest, ShouldTransformBoolParams, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    OHOS::AAFwk::WantParams params;
    params.SetParam("bool_key", Boolean::Box(true));
    want.SetParams(params);
    
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_EQ(cwant.boolParams.size(), 1);
    EXPECT_EQ(cwant.boolParams["bool_key"], true);
}

HWTEST_F(CWantTest, ShouldTransformDoubleParams, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    OHOS::AAFwk::WantParams params;
    params.SetParam("double_key", Double::Box(3.14));
    want.SetParams(params);
    
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_EQ(cwant.doubleParams.size(), 1);
    EXPECT_DOUBLE_EQ(cwant.doubleParams["double_key"], 3.14);
}

HWTEST_F(CWantTest, ShouldTransformMultipleParams, TestSize.Level0)
{
    OHOS::AAFwk::Want want;
    OHOS::AAFwk::WantParams params;
    params.SetParam("str_key", String::Box("str_value"));
    params.SetParam("int_key", Integer::Box(123));
    params.SetParam("bool_key", Boolean::Box(true));
    params.SetParam("double_key", Double::Box(3.14));
    want.SetParams(params);
    
    AbilityBase_Want cwant;
    auto result = OHOS::AAFwk::CWantManager::TransformToCWantWithoutElement(want, false, cwant);

    EXPECT_EQ(result, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_EQ(cwant.params.size(), 1);
    ASSERT_EQ(cwant.intParams.size(), 1);
    ASSERT_EQ(cwant.boolParams.size(), 1);
    ASSERT_EQ(cwant.doubleParams.size(), 1);
    EXPECT_EQ(cwant.params["str_key"], "str_value");
    EXPECT_EQ(cwant.intParams["int_key"], 123);
    EXPECT_EQ(cwant.boolParams["bool_key"], true);
    EXPECT_DOUBLE_EQ(cwant.doubleParams["double_key"], 3.14);
}