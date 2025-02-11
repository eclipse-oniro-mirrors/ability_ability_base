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
#include <cstdint>

#define private public
#include "want_params_wrapper.h"
#undef protected
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "short_wrapper.h"
#include "string_wrapper.h"

using namespace testing::ext;
using namespace OHOS;
using namespace AAFwk;

namespace {
const std::string STRING_WANT_PARAMS_KEY_01 = "key01";
const std::string STRING_WANT_PARAMS_VALUE_01 = "value01";
const std::string STRING_WANT_PARAMS_STRING_01 = "{\"key01\":{\"9\":\"value01\"}}";

const std::string STRING_WANT_PARAMS_KEY_02 = "key02";
const std::string STRING_WANT_PARAMS_VALUE_02 = "value02";
const std::string STRING_WANT_PARAMS_STRING_0201 =
    "{\"key01\":{\"101\":{\"key02\":{\"9\":\"value02\"}}},\"key02\":{\"9\":\"value02\"}}";
}  // namespace

class WantParamWrapperBaseTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() override;
    void TearDown() override;

    WantParams wantParams_;
    std::shared_ptr<WantParamWrapper> wantParamWrapperPtr_;
};

void WantParamWrapperBaseTest::SetUpTestCase()
{}

void WantParamWrapperBaseTest::TearDownTestCase(void)
{}

void WantParamWrapperBaseTest::SetUp(void)
{
    wantParams_ = {};
    wantParams_.SetParam(STRING_WANT_PARAMS_KEY_01, String::Box(STRING_WANT_PARAMS_VALUE_01));
    wantParamWrapperPtr_ = std::make_shared<WantParamWrapper>(wantParams_);
}

void WantParamWrapperBaseTest::TearDown(void)
{}

/**
 * @tc.number: Want_Param_Wrapper_0100
 * @tc.name: GetValue
 * @tc.desc: Verify the "GetValue" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0100, Function | MediumTest | Level1)
{
    WantParams wantParams;
    ErrCode result = wantParamWrapperPtr_->GetValue(wantParams);

    EXPECT_EQ(result, ERR_OK);

    EXPECT_EQ(wantParams_ == wantParams, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0200
 * @tc.name: GetValue
 * @tc.desc: Verify the "GetValue" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0200, Function | MediumTest | Level1)
{
    WantParams wantParams;
    ErrCode result = wantParamWrapperPtr_->GetValue(wantParams);

    EXPECT_EQ(result, ERR_OK);

    // make another empty WantParams
    WantParams wantParamsEmpty = {};

    EXPECT_EQ(wantParams_ == wantParamsEmpty, false);
}

/**
 * @tc.number: Want_Param_Wrapper_0300
 * @tc.name: Equals
 * @tc.desc: Verify the "Equals" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0300, Function | MediumTest | Level1)
{
    WantParams wantParams;
    wantParams.SetParam(STRING_WANT_PARAMS_KEY_01, String::Box(STRING_WANT_PARAMS_VALUE_01));
    WantParamWrapper wantParamWrapper(wantParams);

    bool result = wantParamWrapperPtr_->Equals(wantParamWrapper);

    EXPECT_EQ(result, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0400
 * @tc.name: Equals
 * @tc.desc: Verify the "Equals" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0400, Function | MediumTest | Level1)
{
    WantParams wantParams;
    WantParamWrapper wantParamWrapper(wantParams);

    bool result = wantParamWrapperPtr_->Equals(wantParamWrapper);

    EXPECT_EQ(result, false);
}

/**
 * @tc.number: Want_Param_Wrapper_0500
 * @tc.name: Box
 * @tc.desc: Verify the "Box" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0500, Function | MediumTest | Level1)
{
    auto wantParamsPtr = WantParamWrapper::Box(wantParams_);

    WantParams wantParams;
    ErrCode result = wantParamsPtr->GetValue(wantParams);

    EXPECT_EQ(result, ERR_OK);

    EXPECT_EQ(wantParams_ == wantParams, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0600
 * @tc.name: Box
 * @tc.desc: Verify the "Box" function with am empty object.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0600, Function | MediumTest | Level1)
{
    auto wantParamsPtr = WantParamWrapper::Box({});

    EXPECT_NE(wantParamsPtr, nullptr);

    WantParams wantParams;
    ErrCode result = wantParamsPtr->GetValue(wantParams);

    EXPECT_EQ(result, ERR_OK);

    // make another empty WantParams
    WantParams wantParamsEmpty = {};

    EXPECT_EQ(wantParams == wantParamsEmpty, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0700
 * @tc.name: Unbox
 * @tc.desc: Verify the "Unbox" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0700, Function | MediumTest | Level1)
{
    auto wantParamsPtr = WantParamWrapper::Box(wantParams_);

    auto wantParams = WantParamWrapper::Unbox(wantParamsPtr);

    EXPECT_EQ(wantParams_ == wantParams, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0800
 * @tc.name: Unbox
 * @tc.desc: Verify the "Unbox" function with a nullptr
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0800, Function | MediumTest | Level1)
{
    auto wantParams = WantParamWrapper::Unbox(nullptr);

    // make another empty WantParams
    WantParams wantParamsEmpty = {};

    EXPECT_EQ(wantParams == wantParamsEmpty, true);
}

/**
 * @tc.number: Want_Param_Wrapper_0900
 * @tc.name: ValidateStr
 * @tc.desc: Verify the "ValidateStr" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_0900, Function | MediumTest | Level1)
{
    bool result = WantParamWrapper::ValidateStr("{");

    EXPECT_EQ(result, false);
}

/**
 * @tc.number: Want_Param_Wrapper_1000
 * @tc.name: ValidateStr
 * @tc.desc: Verify the "ValidateStr" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1000, Function | MediumTest | Level1)
{
    bool result = WantParamWrapper::ValidateStr("}");

    EXPECT_EQ(result, false);
}

/**
 * @tc.number: Want_Param_Wrapper_1100
 * @tc.name: ValidateStr
 * @tc.desc: Verify the "ValidateStr" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1100, Function | MediumTest | Level1)
{
    bool result = WantParamWrapper::ValidateStr("}{");

    EXPECT_EQ(result, false);
}

/**
 * @tc.number: Want_Param_Wrapper_1200
 * @tc.name: ValidateStr
 * @tc.desc: Verify the "ValidateStr" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1200, Function | MediumTest | Level1)
{
    bool result = WantParamWrapper::ValidateStr("{\"\"}");

    EXPECT_EQ(result, false);
}

/**
 * @tc.number: Want_Param_Wrapper_1300
 * @tc.name: Parse
 * @tc.desc: Verify the "Parse" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1300, Function | MediumTest | Level1)
{
    auto wantParamsPtr = WantParamWrapper::Parse("");

    auto wantParams = WantParamWrapper::Unbox(wantParamsPtr);

    // make another empty WantParams
    WantParams wantParamsEmpty = {};

    EXPECT_EQ(wantParams == wantParamsEmpty, true);
}

/**
 * @tc.number: Want_Param_Wrapper_1400
 * @tc.name: Parse
 * @tc.desc: Verify the "Parse" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1400, Function | MediumTest | Level1)
{
    auto wantParamsPtr = WantParamWrapper::Parse("{}");

    auto wantParams = WantParamWrapper::Unbox(wantParamsPtr);

    // make another empty WantParams
    WantParams wantParamsEmpty = {};

    EXPECT_EQ(wantParams == wantParamsEmpty, true);
}

/**
 * @tc.number: Want_Param_Wrapper_1500
 * @tc.name: ToString
 * @tc.desc: Verify the "ToString" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1500, Function | MediumTest | Level1)
{
    auto wantParamsString = wantParamWrapperPtr_->ToString();

    EXPECT_EQ(wantParamsString, STRING_WANT_PARAMS_STRING_01);
}

/**
 * @tc.number: Want_Param_Wrapper_1600
 * @tc.name: ToString
 * @tc.desc: Verify the "ToString" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1600, Function | MediumTest | Level1)
{
    WantParams wantParams = {};
    WantParamWrapper wantParamWrapper(wantParams);

    auto wantParamsString = wantParamWrapper.ToString();

    EXPECT_EQ(wantParamsString, "{}");
}

/**
 * @tc.number: Want_Param_Wrapper_1700
 * @tc.name: ToString
 * @tc.desc: Verify the "ToString" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1700, Function | MediumTest | Level1)
{
    WantParams wantParams = {};
    wantParams.SetParam(STRING_WANT_PARAMS_KEY_02, String::Box(STRING_WANT_PARAMS_VALUE_02));
    wantParams.SetParam(STRING_WANT_PARAMS_KEY_01, WantParamWrapper::Box(wantParams));
    WantParamWrapper wantParamWrapper(wantParams);

    auto wantParamsString = wantParamWrapper.ToString();

    EXPECT_EQ(wantParamsString, STRING_WANT_PARAMS_STRING_0201);
}

/**
 * @tc.number: Want_Param_Wrapper_1800
 * @tc.name: from ToString to Parse
 * @tc.desc: Verify the "from ToString to Parse" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1800, Function | MediumTest | Level1)
{
    auto wantParamsString = wantParamWrapperPtr_->ToString();

    EXPECT_EQ(wantParamsString, STRING_WANT_PARAMS_STRING_01);

    auto wantParamsPtr = WantParamWrapper::Parse(wantParamsString);

    auto wantParams = WantParamWrapper::Unbox(wantParamsPtr);

    EXPECT_EQ(wantParams_ == wantParams, true);
}

/**
 * @tc.number: Want_Param_Wrapper_1900
 * @tc.name: ParseWantParams
 * @tc.desc: Verify the "ParseWantParams" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_1900, Function | MediumTest | Level1)
{
    std::string testStr = "{\"byteTest1\":{\"2\":\"\"},\"byteTest2\":{\"2\":\"127\"},\"byteTest3\":{\"2\":\"abc123\"},"
        "\"doubleTest1\":{\"8\":\"\"},\"doubleTest2\":{\"8\":\"3.14\"},\"doubleTest3\":{\"8\":\"abc123\"},"
        "\"floatTest1\":{\"7\":\"\"},\"floatTest2\":{\"7\":\"3.14\"},\"floatTest3\":{\"7\":\"abc123\"},"
        "\"intTest1\":{\"5\":\"\"},\"intTest2\":{\"5\":\"2147483647\"},\"intTest3\":{\"5\":\"abc123\"},"
        "\"longTest1\":{\"6\":\"\"},\"longTest2\":{\"6\":\"2147483647\"},\"longTest3\":{\"6\":\"abc123\"},"
        "\"shortTest1\":{\"4\":\"\"},\"shortTest2\":{\"4\":\"32767\"},\"shortTest3\":{\"4\":\"abc123\"}}";
    WantParams wantParams = WantParamWrapper::ParseWantParams(testStr);

    auto byteTest1Value = wantParams.GetParam("byteTest1");
    EXPECT_EQ(byteTest1Value, nullptr);
    auto byteTest2Value = wantParams.GetParam("byteTest2");
    EXPECT_NE(byteTest2Value, nullptr);
    IByte *iByte = IByte::Query(byteTest2Value);
    EXPECT_NE(iByte, nullptr);
    byte byteVal = Byte::Unbox(iByte);
    EXPECT_EQ(byteVal, INT8_MAX);
    auto byteTest3Value = wantParams.GetParam("byteTest3");
    EXPECT_EQ(byteTest3Value, nullptr);

    auto shortTest1Value = wantParams.GetParam("shortTest1");
    EXPECT_EQ(shortTest1Value, nullptr);
    auto shortTest2Value = wantParams.GetParam("shortTest2");
    EXPECT_NE(shortTest2Value, nullptr);
    IShort *iShort = IShort::Query(shortTest2Value);
    EXPECT_NE(iShort, nullptr);
    short shortVal = Short::Unbox(iShort);
    EXPECT_EQ(shortVal, INT16_MAX);
    auto shortTest3Value = wantParams.GetParam("shortTest3");
    EXPECT_EQ(shortTest3Value, nullptr);
}

/**
 * @tc.number: Want_Param_Wrapper_2000
 * @tc.name: ParseWantParams
 * @tc.desc: Verify the "ParseWantParams" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2000, Function | MediumTest | Level1)
{
    std::string testStr = "{\"byteTest1\":{\"2\":\"\"},\"byteTest2\":{\"2\":\"127\"},\"byteTest3\":{\"2\":\"abc123\"},"
        "\"doubleTest1\":{\"8\":\"\"},\"doubleTest2\":{\"8\":\"3.14\"},\"doubleTest3\":{\"8\":\"abc123\"},"
        "\"floatTest1\":{\"7\":\"\"},\"floatTest2\":{\"7\":\"3.14\"},\"floatTest3\":{\"7\":\"abc123\"},"
        "\"intTest1\":{\"5\":\"\"},\"intTest2\":{\"5\":\"2147483647\"},\"intTest3\":{\"5\":\"abc123\"},"
        "\"longTest1\":{\"6\":\"\"},\"longTest2\":{\"6\":\"2147483647\"},\"longTest3\":{\"6\":\"abc123\"},"
        "\"shortTest1\":{\"4\":\"\"},\"shortTest2\":{\"4\":\"32767\"},\"shortTest3\":{\"4\":\"abc123\"}}";
    WantParams wantParams = WantParamWrapper::ParseWantParams(testStr);

    auto doubleTest1Value = wantParams.GetParam("doubleTest1");
    EXPECT_EQ(doubleTest1Value, nullptr);
    auto doubleTest2Value = wantParams.GetParam("doubleTest2");
    EXPECT_NE(doubleTest2Value, nullptr);
    IDouble *iDouble = IDouble::Query(doubleTest2Value);
    EXPECT_NE(iDouble, nullptr);
    double doubleVal = Double::Unbox(iDouble);
    EXPECT_FLOAT_EQ(doubleVal, 3.14f);
    auto doubleTest3Value = wantParams.GetParam("doubleTest3");
    EXPECT_EQ(doubleTest3Value, nullptr);

    auto floatTest1Value = wantParams.GetParam("floatTest1");
    EXPECT_EQ(floatTest1Value, nullptr);
    auto floatTest2Value = wantParams.GetParam("floatTest2");
    EXPECT_NE(floatTest2Value, nullptr);
    IFloat *iFloat = IFloat::Query(floatTest2Value);
    EXPECT_NE(iFloat, nullptr);
    float floatVal = Float::Unbox(iFloat);
    EXPECT_FLOAT_EQ(floatVal, 3.14f);
    auto floatTest3Value = wantParams.GetParam("floatTest3");
    EXPECT_EQ(floatTest3Value, nullptr);
}

/**
 * @tc.number: Want_Param_Wrapper_2100
 * @tc.name: ParseWantParams
 * @tc.desc: Verify the "ParseWantParams" function.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2100, Function | MediumTest | Level1)
{
    std::string testStr = "{\"byteTest1\":{\"2\":\"\"},\"byteTest2\":{\"2\":\"127\"},\"byteTest3\":{\"2\":\"abc123\"},"
        "\"doubleTest1\":{\"8\":\"\"},\"doubleTest2\":{\"8\":\"3.14\"},\"doubleTest3\":{\"8\":\"abc123\"},"
        "\"floatTest1\":{\"7\":\"\"},\"floatTest2\":{\"7\":\"3.14\"},\"floatTest3\":{\"7\":\"abc123\"},"
        "\"intTest1\":{\"5\":\"\"},\"intTest2\":{\"5\":\"2147483647\"},\"intTest3\":{\"5\":\"abc123\"},"
        "\"longTest1\":{\"6\":\"\"},\"longTest2\":{\"6\":\"2147483647\"},\"longTest3\":{\"6\":\"abc123\"},"
        "\"shortTest1\":{\"4\":\"\"},\"shortTest2\":{\"4\":\"32767\"},\"shortTest3\":{\"4\":\"abc123\"}}";
    WantParams wantParams = WantParamWrapper::ParseWantParams(testStr);

    auto intTest1Value = wantParams.GetParam("intTest1");
    EXPECT_EQ(intTest1Value, nullptr);
    auto intTest2Value = wantParams.GetParam("intTest2");
    EXPECT_NE(intTest2Value, nullptr);
    IInteger *iInteger = IInteger::Query(intTest2Value);
    EXPECT_NE(iInteger, nullptr);
    int intVal = Integer::Unbox(iInteger);
    EXPECT_EQ(intVal, INT32_MAX);
    auto intTest3Value = wantParams.GetParam("intTest3");
    EXPECT_EQ(intTest3Value, nullptr);

    auto longTest1Value = wantParams.GetParam("longTest1");
    EXPECT_EQ(longTest1Value, nullptr);
    auto longTest2Value = wantParams.GetParam("longTest2");
    EXPECT_NE(longTest2Value, nullptr);
    ILong *iLong = ILong::Query(longTest2Value);
    EXPECT_NE(iLong, nullptr);
    long longVal = Long::Unbox(iLong);
    EXPECT_EQ(longVal, INT32_MAX);
    auto longTest3Value = wantParams.GetParam("longTest3");
    EXPECT_EQ(longTest3Value, nullptr);
}
