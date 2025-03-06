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
#include "want/include/want.h"

using namespace testing::ext;


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
 * @tc.number:OH_AbilityBase_SetWantElement_001
 * @tc.desc: Function test OH_AbilityBase_SetWantElement_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantElement_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    AbilityBase_Element outElement;
    errCode = OH_AbilityBase_GetWantElement(want, &outElement);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ReleaseElementMemory(element);
    ASSERT_STREQ(outElement.bundleName, "testBundleName");
    ASSERT_STREQ(outElement.moduleName, "testModuleName");
    ASSERT_STREQ(outElement.abilityName, "testAbilityName");
}

/**
 * @tc.number:OH_AbilityBase_SetWantElement_002
 * @tc.desc: Function test OH_AbilityBase_SetWantElement_002
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantElement_002, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = nullptr;
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
}

/**
 * @tc.number:OH_AbilityBase_SetWantUri_001
 * @tc.desc: Function test OH_AbilityBase_SetWantUri_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantUri_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    const char * uri = "testUri";
    errCode = OH_AbilityBase_SetWantUri(want, uri);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    errCode = OH_AbilityBase_SetWantUri(want, nullptr);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
    errCode = OH_AbilityBase_SetWantUri(nullptr, nullptr);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
    char outUri[50];
    errCode = OH_AbilityBase_GetWantUri(want, outUri, sizeof(outUri));
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    ASSERT_STREQ(uri, outUri);
    errCode = OH_AbilityBase_GetWantUri(want, outUri, 1);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
}

/**
 * @tc.number:OH_AbilityBase_SetWantInt32Param_001
 * @tc.desc: Function test OH_AbilityBase_SetWantInt32Param_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantInt32Param_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    const char * key = "intKey";
    int32_t value = 10;
    errCode = OH_AbilityBase_SetWantInt32Param(want, nullptr, value);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
    errCode = OH_AbilityBase_SetWantInt32Param(want, key, value);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    int32_t outValue = 0;
    errCode = OH_AbilityBase_GetWantInt32Param(want, key, &outValue);
    ASSERT_EQ(outValue, value);
    errCode = OH_AbilityBase_GetWantInt32Param(want, "notFoundKey", &outValue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
}

/**
 * @tc.number:OH_AbilityBase_SetWantBoolParam_001
 * @tc.desc: Function test OH_AbilityBase_SetWantBoolParam_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantBoolParam_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    const char * key = "boolKey";
    bool value = true;
    errCode = OH_AbilityBase_SetWantBoolParam(want, key, value);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    errCode = OH_AbilityBase_SetWantBoolParam(want, key, value);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);
    bool outValue = false;
    errCode = OH_AbilityBase_GetWantBoolParam(want, key, &outValue);
    ASSERT_EQ(outValue, value);
    errCode = OH_AbilityBase_GetWantBoolParam(want, "notFoundKey", &outValue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
}

/**
 * @tc.number:OH_AbilityBase_SetWantDoubleParam_001
 * @tc.desc: Function test OH_AbilityBase_SetWantDoubleParam_001
 * @tc.type: FUNC
 */
HWTEST_F(CWantTest, OH_AbilityBase_SetWantDoubleParam_001, TestSize.Level0)
{
    AbilityBase_Element element;
    initElementWithDynamicMemory(element);

    AbilityBase_Want* want = OH_AbilityBase_CreateWant(element);
    AbilityBase_ErrorCode errCode = OH_AbilityBase_SetWantElement(want, element);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    const char * key = "boolKey";
    double value = 3.14f;
    errCode = OH_AbilityBase_SetWantDoubleParam(want, key, value);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_NO_ERROR);

    double outValue = 0.00;
    errCode = OH_AbilityBase_GetWantDoubleParam(want, key, &outValue);
    ASSERT_EQ(outValue, value);
    errCode = OH_AbilityBase_GetWantDoubleParam(want, "notFoundKey", &outValue);
    ASSERT_EQ(errCode, ABILITY_BASE_ERROR_CODE_PARAM_INVALID);
}
