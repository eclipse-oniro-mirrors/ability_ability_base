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
#include <utility>

#define private public
#include "want_params_wrapper.h"
#undef private
#include "array_wrapper.h"
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
constexpr int WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH = 100;

std::string BuildNestedArrayString(int depth)
{
    std::string str = "I1{1}";
    for (int i = 0; i < depth; i++) {
        str = "[1{" + str + "}";
    }
    return str;
}

std::string BuildWantParamsWithArrayString(int arrayDepth)
{
    return "{\"array\":{\"102\":\"" + BuildNestedArrayString(arrayDepth) + "\"}}";
}

std::string BuildNestedWantParamsString(int depth)
{
    std::string str = "{\"leaf\":{\"9\":\"value\"}}";
    for (int i = 0; i < depth; i++) {
        str = "{\"key" + std::to_string(i) + "\":{\"101\":" + str + "}}";
    }
    return str;
}

sptr<IWantParams> BuildNestedWantParamWrapper(int depth)
{
    WantParams leaf;
    leaf.SetParam("leaf", String::Box("value"));
    sptr<IWantParams> current = WantParamWrapper::Box(leaf);
    for (int i = 0; i < depth; i++) {
        WantParams outer;
        outer.SetParam("key" + std::to_string(i), current);
        current = WantParamWrapper::Box(outer);
    }
    return current;
}

sptr<IArray> BuildNestedArrayObject(int depth)
{
    sptr<IArray> current = sptr<Array>::MakeSptr(1, g_IID_IInteger);
    current->Set(0, Integer::Box(1));
    for (int i = 0; i < depth; i++) {
        sptr<IArray> outer = sptr<Array>::MakeSptr(1, g_IID_IArray);
        outer->Set(0, current);
        current = outer;
    }
    return current;
}

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

/**
 * @tc.number: Want_Param_Wrapper_2200
 * @tc.name: Parse with unmatched quote returns empty
 * @tc.desc: Verify Parse does not infinite loop on malformed input with stray quote.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2200, Function | MediumTest | Level1)
{
    // Colon format: {"k":{"101":{"x"}"a":{"1":"v"}}}
    // After nested {"x"}, strnum=17 then loop inc->18, skipping the " at pos 17.
    // Remaining effective quotes from pos 18: 5 (odd) — last " triggers npos.
    std::string malformed = "{\"k\":{\"101\":{\"x\"}\"a\":{\"1\":\"v\"}}}";
    auto result = WantParamWrapper::Parse(malformed);
    WantParams wantParams = WantParamWrapper::Unbox(result);
    EXPECT_EQ(wantParams.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2300
 * @tc.name: ParseWantParams with unmatched quote returns empty
 * @tc.desc: Verify ParseWantParams does not infinite loop on malformed input with stray quote.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2300, Function | MediumTest | Level1)
{
    std::string malformed = "{\"k\":{\"101\":{\"x\"}\"a\":{\"1\":\"v\"}}}";
    WantParams wantParams = WantParamWrapper::ParseWantParams(malformed);
    EXPECT_EQ(wantParams.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2400
 * @tc.name: ParseWantParamsWithBrackets with unmatched quote returns empty
 * @tc.desc: Verify ParseWantParamsWithBrackets does not infinite loop on malformed input with stray quote.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2400, Function | MediumTest | Level1)
{
    std::string malformed = "{\"k\":{\"101\":{\"x\"}\"a\":{\"1\":\"v\"}}}";
    WantParams wantParams = WantParamWrapper::ParseWantParamsWithBrackets(malformed);
    EXPECT_EQ(wantParams.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2500
 * @tc.name: Parse with overflowing typeId returns empty
 * @tc.desc: A typeId out of int range must be rejected and the whole result reset to empty.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2500, Function | MediumTest | Level1)
{
    // typeId "2147483648" equals INT_MAX + 1, out of int range on every platform.
    std::string malformed = "{\"k\":{\"2147483648\":\"v\"}}";

    auto result = WantParamWrapper::Parse(malformed);
    WantParams wantParams = WantParamWrapper::Unbox(result);
    EXPECT_EQ(wantParams.Size(), 0);

    WantParams wantParamsB = WantParamWrapper::ParseWantParams(malformed);
    EXPECT_EQ(wantParamsB.Size(), 0);

    WantParams wantParamsC = WantParamWrapper::ParseWantParamsWithBrackets(malformed);
    EXPECT_EQ(wantParamsC.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2600
 * @tc.name: Parse with non-numeric typeId returns empty
 * @tc.desc: A non-numeric typeId must be rejected and the whole result reset to empty.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2600, Function | MediumTest | Level1)
{
    // typeId "abc" is non-numeric; it must not silently fall back to 0 and misalign parsing.
    std::string malformed = "{\"k\":{\"abc\":\"v\"}}";

    auto result = WantParamWrapper::Parse(malformed);
    WantParams wantParams = WantParamWrapper::Unbox(result);
    EXPECT_EQ(wantParams.Size(), 0);

    WantParams wantParamsB = WantParamWrapper::ParseWantParams(malformed);
    EXPECT_EQ(wantParamsB.Size(), 0);

    WantParams wantParamsC = WantParamWrapper::ParseWantParamsWithBrackets(malformed);
    EXPECT_EQ(wantParamsC.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2700
 * @tc.name: ParseWantParamsWithBrackets with FindMatchingBrackets npos returns empty
 * @tc.desc: Verify ParseWantParamsWithBrackets does not infinite loop when the value branch
 *           hits FindMatchingBrackets returning npos. With no unmatched '}' after the typeId
 *           token, FindMatchingBrackets returns npos; without the npos guard the assignment
 *           strnum = index + 1 wraps (npos + 1 == 0) and the loop restarts forever. Input
 *           "a\"a\"\"1\"\"\"" reads key="a", typeId=1, then the value branch triggers.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2700, Function | MediumTest | Level1)
{
    // No unmatched '}' after the typeId -> FindMatchingBrackets returns npos in the value
    // branch -> strnum = npos + 1 wraps around to 0 -> infinite loop without the npos guard.
    std::string malformed = "a\"a\"\"1\"\"\"";
    WantParams wantParams = WantParamWrapper::ParseWantParamsWithBrackets(malformed);
    EXPECT_EQ(wantParams.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_2800
 * @tc.name: ParseWantParams parses nested WantParams
 * @tc.desc: Verify ParseWantParams attaches a nested WantParams under key "k". Before the
 *           num-strnum+1 fix the nested substring was passed without its closing '}', got
 *           rejected by ValidateStr, and the entry was lost; after the fix it parses.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2800, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"101\":{\"i\":{\"9\":\"v\"}}}}";
    WantParams wp = WantParamWrapper::ParseWantParams(s);
    auto kVal = wp.GetParam("k");
    ASSERT_NE(kVal, nullptr);
    WantParams inner = WantParamWrapper::Unbox(IWantParams::Query(kVal));
    EXPECT_EQ(inner.Size(), 1u);
}

/**
 * @tc.number: Want_Param_Wrapper_2900
 * @tc.name: Nested parse failure propagates to empty result
 * @tc.desc: Verify that when a nested WantParams fails to parse ("{}" is rejected by
 *           ValidateStr) the failure propagates and the WHOLE result is empty -- the earlier
 *           successfully-parsed entry "good" is discarded too, not just the failing key.
 *           Applies to all three parsers.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_2900, Function | MediumTest | Level1)
{
    // "good" parses fine first; then "k"'s nested {} fails -> whole result empty (Size 0),
    // i.e. "good" is also dropped. Proves it's not a per-key clear but a full reset.
    std::string s = "{\"good\":{\"9\":\"v\"},\"k\":{\"101\":{}}}";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 0u);
}

/**
 * @tc.number: Want_Param_Wrapper_3000
 * @tc.name: Parse rejects unknown typeId
 * @tc.desc: Verify an unknown typeId (999) is rejected -- the whole result is empty instead
 *           of storing a null entry from GetInterfaceByType. Without the whitelist 999 would
 *           pass ParseTypeId, GetInterfaceByType would return nullptr, and SetParam would
 *           store a null-valued entry (Size 1). Applies to all three parsers.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3000, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"999\":\"v\"}}";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 0u);
}

/**
 * @tc.number: Want_Param_Wrapper_3100
 * @tc.name: Parse rejects typeId 0 (sentinel collision)
 * @tc.desc: typeId 0 is not a real type and collides with the parser's "not-yet-read"
 *           sentinel (typeId == 0). It must be rejected so the state machine does not
 *           re-enter the typeId branch and misread the value as another typeId.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3100, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"0\":\"v\"}}";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 0u);
}

/**
 * @tc.number: Want_Param_Wrapper_3200
 * @tc.name: Parse rejects typeId with trailing garbage
 * @tc.desc: A typeId token like "5abc" has trailing non-numeric chars. ParseTypeId's
 *           *end != '\0' check must reject it so parsing does not silently accept 5 and
 *           misalign the stream on the residual "abc". Covers the only ParseTypeId branch
 *           not exercised by 2500/2600/3000/3100. Applies to all three parsers.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3200, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"5abc\":\"v\"}}";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 0u);
}

/**
 * @tc.number: Want_Param_Wrapper_3300
 * @tc.name: ParseWantParamsWithBrackets parses a standard string entry
 * @tc.desc: Verify the WithBrackets value branch (ParseBracketValue success path through
 *           FindMatchingBrackets) attaches a string value. This is the sole POSITIVE
 *           coverage for ParseWantParamsWithBrackets -- every other case feeds malformed
 *           input expecting Size 0, so the success path and the typeIndexBefore recording
 *           in ParseBracketQuotedToken are otherwise untested.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3300, Function | MediumTest | Level1)
{
    std::string s = "{\"key01\":{\"9\":\"value01\"}}";
    WantParams wp = WantParamWrapper::ParseWantParamsWithBrackets(s);
    EXPECT_EQ(wp.Size(), 1u);
    auto val = wp.GetParam("key01");
    ASSERT_NE(val, nullptr);
    IString *iStr = IString::Query(val);
    ASSERT_NE(iStr, nullptr);
    EXPECT_EQ(String::Unbox(iStr), "value01");
}

/**
 * @tc.number: Want_Param_Wrapper_3400
 * @tc.name: Final-state guard catches value-internal-quote truncation
 * @tc.desc: Value "a"b"c" contains internal quotes. On the QUOTE path
 *           (Parse / ParseWantParams) FindNextQuote stops at the first inner quote,
 *           stores the truncated value "a", and the residual strands a dangling
 *           key="c"; the final-state guard (ctx.key non-empty) resets the whole
 *           result to empty (Size 0). This is the primary truncation shape the
 *           guard intercepts: truncation that leaves the parser mid-cycle. The
 *           BRACKET path (WithBrackets) matches on '}' not '"', so it extracts the
 *           full value -- no truncation, guard does not fire, Size stays 1.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3400, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"9\":\"a\"b\"c\"}}";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 1u);
}

/**
 * @tc.number: Want_Param_Wrapper_3500
 * @tc.name: Final-state guard does not catch trailing garbage
 * @tc.desc: Boundary of the guard. Trailing chars after a complete object (...}}X)
 *           are silently skipped and the parser exits in a CLEAN state (key empty,
 *           typeId 0), so the guard does not fire and the entry survives (Size 1).
 *           The same applies to multi-object merge (}},{) and to truncation whose
 *           residual completes full cycles -- those need separate detection, not
 *           the final-state guard.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3500, Function | MediumTest | Level1)
{
    std::string s = "{\"k\":{\"9\":\"v\"}}X";
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 1u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 1u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 1u);
}

/**
 * @tc.number: Want_Param_Wrapper_3600
 * @tc.name: Nested parse within max depth succeeds
 * @tc.desc: Verify the three parse entrances accept nested WantParams before the depth limit.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3600, Function | MediumTest | Level1)
{
    std::string s = BuildNestedWantParamsString(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH);
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 1u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 1u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 1u);
}

/**
 * @tc.number: Want_Param_Wrapper_3700
 * @tc.name: Nested parse over max depth returns empty
 * @tc.desc: Verify the three parse entrances propagate recursive Parse depth failures.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3700, Function | MediumTest | Level1)
{
    std::string s = BuildNestedWantParamsString(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH + 1);
    EXPECT_EQ(WantParamWrapper::Unbox(WantParamWrapper::Parse(s)).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParams(s).Size(), 0u);
    EXPECT_EQ(WantParamWrapper::ParseWantParamsWithBrackets(s).Size(), 0u);
}

/**
 * @tc.number: Want_Param_Wrapper_3800
 * @tc.name: ToString within max depth succeeds
 * @tc.desc: Verify ToString serializes nested WantParams before the depth limit. A wrapper
 *           nested to the max depth (100) must still emit a non-empty string containing the
 *           leaf value -- the depth guard rejects only depth > MAX, not depth == MAX, and
 *           none of the recursive calls short-circuit. ToString pairs with Parse's 3600.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3800, Function | MediumTest | Level1)
{
    auto nested = BuildNestedWantParamWrapper(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH);
    WantParamWrapper wrapper(WantParamWrapper::Unbox(nested));

    auto result = wrapper.ToString();

    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("value"), std::string::npos);
}

/**
 * @tc.number: Want_Param_Wrapper_3900
 * @tc.name: ToString over max depth returns empty
 * @tc.desc: Verify ToString propagates recursive depth failures. A wrapper nested one level
 *           beyond the max depth (101) makes the innermost call hit ToString(MAX + 1), which
 *           the depth guard returns as ""; each ancestor then short-circuits on that empty
 *           child result and also returns "", so the whole serialization collapses to "" and
 *           recursion cannot run away into a stack overflow. ToString pairs with Parse's
 *           3700. The over-limit object is built directly because Parse itself enforces the
 *           same cap and would return empty first.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_3900, Function | MediumTest | Level1)
{
    auto nested = BuildNestedWantParamWrapper(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH + 1);
    WantParamWrapper wrapper(WantParamWrapper::Unbox(nested));

    auto result = wrapper.ToString();

    EXPECT_TRUE(result.empty());
}

/**
 * @tc.number: Want_Param_Wrapper_4000
 * @tc.name: Type 102 Array nesting within max depth succeeds
 * @tc.desc: Verify Parse stores a type-102 Array before WantParams by-value copy.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_4000, Function | MediumTest | Level1)
{
    std::string s = BuildWantParamsWithArrayString(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH - 1);
    sptr<IWantParams> parsed = WantParamWrapper::Parse(s);
    ASSERT_NE(parsed, nullptr);
    auto wrapper = static_cast<WantParamWrapper *>(parsed.GetRefPtr());
    ASSERT_NE(wrapper, nullptr);
    EXPECT_NE(IArray::Query(wrapper->wantParams_.GetParam("array")), nullptr);
}

/**
 * @tc.number: Want_Param_Wrapper_4100
 * @tc.name: Type 102 Array nesting over max depth fails
 * @tc.desc: Verify Parse rejects type-102 Array when nested Array depth crosses the limit.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_4100, Function | MediumTest | Level1)
{
    std::string s = BuildWantParamsWithArrayString(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH);
    sptr<IWantParams> parsed = WantParamWrapper::Parse(s);
    ASSERT_NE(parsed, nullptr);
    auto wrapper = static_cast<WantParamWrapper *>(parsed.GetRefPtr());
    ASSERT_NE(wrapper, nullptr);
    EXPECT_EQ(IArray::Query(wrapper->wantParams_.GetParam("array")), nullptr);
}

/**
 * @tc.number: Want_Param_Wrapper_4200
 * @tc.name: Type 102 Array ToString depth is shared
 * @tc.desc: Verify WantParams serialization propagates Array depth failures.
 */
HWTEST_F(WantParamWrapperBaseTest, Want_Param_Wrapper_4200, Function | MediumTest | Level1)
{
    WantParams okParams;
    okParams.SetParam("array", BuildNestedArrayObject(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH - 1));
    WantParamWrapper okWrapper(std::move(okParams));
    EXPECT_FALSE(okWrapper.ToString().empty());

    WantParams overLimit;
    overLimit.SetParam("array", BuildNestedArrayObject(WANT_PARAMS_WRAPPER_PARSE_MAX_DEPTH));
    WantParamWrapper overLimitWrapper(std::move(overLimit));
    EXPECT_TRUE(overLimitWrapper.ToString().empty());
}
