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

#define private public
#define protected public
#include "array_wrapper.h"
#include "base_obj.h"
#include "bool_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "want_params.h"
#include "want_params_wrapper.h"
#include "zchar_wrapper.h"
#undef private
#undef protected

using namespace testing::ext;
using namespace OHOS::AAFwk;
using OHOS::Parcel;

namespace OHOS {
namespace AAFwk {
class WantParamsBaseTest : public testing::Test {
public:
    WantParamsBaseTest()
    {}
    ~WantParamsBaseTest()
    {
    }
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();

    std::shared_ptr<WantParams> wantParamsIn_ = nullptr;
    std::shared_ptr<WantParams> wantParamsOut_ = nullptr;
};

void WantParamsBaseTest::SetUpTestCase(void)
{}

void WantParamsBaseTest::TearDownTestCase(void)
{}

void WantParamsBaseTest::SetUp(void)
{
    wantParamsIn_ = std::make_shared<WantParams>();
    wantParamsOut_ = std::make_shared<WantParams>();
}

void WantParamsBaseTest::TearDown(void)
{
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0100
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling WantParams, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0100, Function | MediumTest | Level1)
{
    std::string keyStr = "12345667";
    std::string valueStr = "sdasdfdsffdgfdg";
    wantParamsIn_->SetParam(keyStr, String::Box(valueStr));

    Parcel in;
    if (wantParamsOut_ != nullptr) {
        wantParamsIn_->Marshalling(in);
        std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
        EXPECT_EQ(valueStr, String::Unbox(IString::Query(wantParamsOut_->GetParam(keyStr))));
    }
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0200
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling WantParams, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0200, Function | MediumTest | Level1)
{
    std::string keyStr = "12345667";
    bool valueBool = true;
    wantParamsIn_->SetParam(keyStr, Boolean::Box(valueBool));

    Parcel in;
    if (wantParamsOut_ != nullptr) {
        wantParamsIn_->Marshalling(in);
        std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
        EXPECT_EQ(valueBool, Boolean::Unbox(IBoolean::Query(wantParamsOut_->GetParam(keyStr))));
    }
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0300
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling WantParams, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0300, Function | MediumTest | Level1)
{
    std::string keyStr = "12345667";
    int valueInteger = 12345;
    wantParamsIn_->SetParam(keyStr, Integer::Box(valueInteger));
    int right = Integer::Unbox(IInteger::Query(wantParamsIn_->GetParam(keyStr)));

    Parcel in;
    wantParamsIn_->Marshalling(in);
    std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
    if (wantParamsOut_ != nullptr) {
        right = Integer::Unbox(IInteger::Query(wantParamsOut_->GetParam(keyStr)));
        EXPECT_EQ(valueInteger, right);

        wantParamsOut_ = nullptr;
    }
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0400
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling WantParams, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0400, Function | MediumTest | Level1)
{
    std::string keyStr = "12345667";
    long valueLong = 1234567;
    wantParamsIn_->SetParam(keyStr, Long::Box(valueLong));

    Parcel in;
    wantParamsIn_->Marshalling(in);
    std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
    long retLong = Long::Unbox(ILong::Query(wantParamsOut_->GetParam(keyStr)));
    EXPECT_EQ(retLong, valueLong);
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0500
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling nested WantParams inside array, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0500, Function | MediumTest | Level1)
{
    sptr<AAFwk::IArray> ao = new (std::nothrow) AAFwk::Array(2, AAFwk::g_IID_IWantParams);
    for (size_t i = 0; i < 2; i++) {
        WantParams wp;
        wp.SetParam("hello", String::Box("World"));
        ao->Set(i, AAFwk::WantParamWrapper::Box(wp));
    }
    WantParams l1;
    l1.SetParam("l1", ao);
    WantParams l2;
    l2.SetParam("l2", AAFwk::WantParamWrapper::Box(l1));
    wantParamsIn_->SetParam("l3",  AAFwk::WantParamWrapper::Box(l2));
    wantParamsIn_->DumpInfo(0);

    Parcel in;
    wantParamsIn_->Marshalling(in);
    std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
    wantParamsOut_->DumpInfo(0);

    WantParams l2Out = WantParamWrapper::Unbox(IWantParams::Query(wantParamsOut_->GetParam("l3")));
    WantParams l1Out = WantParamWrapper::Unbox(IWantParams::Query(l2Out.GetParam("l2")));

    IArray *aoOut = IArray::Query(l1Out.GetParam("l1"));
    long size;
    aoOut->GetLength(size);
    EXPECT_EQ(size, 2);
    sptr<IInterface> ww0;
    sptr<IInterface> ww1;
    aoOut->Get(0, ww0);
    aoOut->Get(1, ww1);

    WantParams wp0 = WantParamWrapper::Unbox(static_cast<IWantParams *>(ww0->Query(g_IID_IWantParams)));
    WantParams wp1 = WantParamWrapper::Unbox(static_cast<IWantParams *>(ww1->Query(g_IID_IWantParams)));

    std::string hello0(String::Unbox(IString::Query(wp0.GetParam("hello"))));
    EXPECT_EQ(hello0, "World");
    std::string hello1(String::Unbox(IString::Query(wp1.GetParam("hello"))));
    EXPECT_EQ(hello1, "World");
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0600
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling nested WantParams, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0600, Function | MediumTest | Level1)
{
    WantParams wp;

    wp.SetParam("hello", String::Box("World"));
    wp.SetParam("welcome", String::Box("NY"));

    WantParams l1;
    l1.SetParam("l1",  AAFwk::WantParamWrapper::Box(wp));
    wantParamsIn_->SetParam("l2",  AAFwk::WantParamWrapper::Box(l1));
    wantParamsIn_->DumpInfo(0);
    Parcel in;
    wantParamsIn_->Marshalling(in);
    std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
    wantParamsOut_->DumpInfo(0);
    WantParams l1Out = WantParamWrapper::Unbox(IWantParams::Query(wantParamsOut_->GetParam("l2")));
    WantParams wpOut = WantParamWrapper::Unbox(IWantParams::Query(l1.GetParam("l1")));
    std::string hello(String::Unbox(IString::Query(wpOut.GetParam("hello"))));
    EXPECT_EQ(hello, "World");
    std::string welcome(String::Unbox(IString::Query(wpOut.GetParam("welcome"))));
    EXPECT_EQ(welcome, "NY");
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0700
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: marshalling array, and then check result.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0700, Function | MediumTest | Level1)
{
    sptr<AAFwk::IArray> ao = new (std::nothrow) AAFwk::Array(2, AAFwk::g_IID_IString);

    std::string valueStr0 = "TestValue0";
    ao->Set(0, String::Box(valueStr0));
    std::string valueStr1 = "TestValue1";
    ao->Set(1, String::Box(valueStr1));

    WantParams l1;
    l1.SetParam("l1", ao);
    wantParamsIn_->SetParam("l2",  AAFwk::WantParamWrapper::Box(l1));
    wantParamsIn_->DumpInfo(0);

    Parcel in;
    wantParamsIn_->Marshalling(in);
    std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
    wantParamsOut_->DumpInfo(0);

    WantParams l1Out = WantParamWrapper::Unbox(IWantParams::Query(wantParamsOut_->GetParam("l2")));
    IArray *aoOut = IArray::Query(l1Out.GetParam("l1"));

    long size;
    aoOut->GetLength(size);
    EXPECT_EQ(size, 2);
    sptr<IInterface> str0;
    sptr<IInterface> str1;
    aoOut->Get(0, str0);
    aoOut->Get(1, str1);

    EXPECT_EQ(valueStr0, String::Unbox(IString::Query(str0)));
    EXPECT_EQ(valueStr1, String::Unbox(IString::Query(str1)));
}
/**
 * @brief Helper function to generate a wantParam with desired nested depth.
 * @param depth the desired depth.
 */
WantParams MaliciousRecursionGenerator(int depth)
{
    WantParams wp;
    if (depth <= 1) {
        wp.SetParam("hello", String::Box("World"));
        wp.SetParam("welcome", String::Box("NY"));
        return wp;
    } else {
        wp.SetParam(std::to_string(depth), AAFwk::WantParamWrapper::Box(MaliciousRecursionGenerator(depth-1)));
    }
    return wp;
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0800
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: Test within the recursion depth limits when marshalling with nested wantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0800, Function | MediumTest | Level1)
{
    WantParams wp = MaliciousRecursionGenerator(98);
    wantParamsIn_->SetParam("l99",  AAFwk::WantParamWrapper::Box(wp));
    Parcel in;
    EXPECT_EQ(wantParamsIn_->Marshalling(in), true);
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_0900
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: Test exceeding the recursion depth limits when marshalling with nested wantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_0900, Function | MediumTest | Level1)
{
    WantParams wp = MaliciousRecursionGenerator(99);
    wantParamsIn_->SetParam("l100",  AAFwk::WantParamWrapper::Box(wp));
    Parcel in;
    EXPECT_EQ(wantParamsIn_->Marshalling(in), false);
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_1000
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: Test exceeding the recursion depth limits when marshalling with nested wantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_1000, Function | MediumTest | Level1)
{
    WantParams wp = MaliciousRecursionGenerator(100);
    wantParamsIn_->SetParam("l101",  AAFwk::WantParamWrapper::Box(wp));
    Parcel in;
    EXPECT_EQ(wantParamsIn_->Marshalling(in), false);
}

/**
 * @tc.number: AaFwk_WantParams_Parcelable_1100
 * @tc.name: DeepCopy
 * @tc.desc: Test copy constructor of wantParam uses DeepCopy.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_Parcelable_1100, Function | MediumTest | Level1)
{
    WantParams wp = MaliciousRecursionGenerator(0);
    sptr<IWantParams> wrapper = AAFwk::WantParamWrapper::Box(wp);
    WantParams wp2;
    wp2.SetParam("l1",  wrapper);
    // DeepCopy
    WantParams wp3(wp2);
    // The wrapper ptr should not be the same.
    EXPECT_NE(wp3.GetParam("l1"), wp2.GetParam("l1"));
    // checks if inner values remain the same, wp1 should contain the values in wp
    WantParams wp1 = WantParamWrapper::Unbox(IWantParams::Query(wp3.GetParam("l1")));
    EXPECT_EQ(String::Unbox(IString::Query(wp1.GetParam("hello"))), "World");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0100
 * @tc.name: GetStringByType
 * @tc.desc: Test get string with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0100, Function | MediumTest | Level1)
{
    WantParams wp = MaliciousRecursionGenerator(0);
    sptr<IWantParams> wrapper = AAFwk::WantParamWrapper::Box(wp);
    std::string value = WantParams::GetStringByType(wp.GetParam("l1"), WantParams::VALUE_TYPE_NULL);
    EXPECT_EQ(value, "");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0200
 * @tc.name: GetStringByType
 * @tc.desc: Test get bool with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0200, Function | MediumTest | Level1)
{
    const std::string value = "true";
    sptr<IInterface> boolObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BOOLEAN, value);
    std::string stringVal = WantParams::GetStringByType(boolObj, WantParams::VALUE_TYPE_BOOLEAN);
    EXPECT_EQ(stringVal, "true");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0300
 * @tc.name: GetStringByType
 * @tc.desc: Test get string with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0300, Function | MediumTest | Level1)
{
    const std::string value = "string";
    sptr<IInterface> stringObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_STRING, value);
    std::string stringVal = WantParams::GetStringByType(stringObj, WantParams::VALUE_TYPE_STRING);
    EXPECT_EQ(stringVal, "string");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0400
 * @tc.name: GetStringByType
 * @tc.desc: Test get byte with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0400, Function | MediumTest | Level1)
{
    const std::string value = "129";
    sptr<IInterface> byteObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BYTE, value);
    std::string byteVal = WantParams::GetStringByType(byteObj, WantParams::VALUE_TYPE_BYTE);
    EXPECT_EQ(byteVal, "129");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0500
 * @tc.name: GetStringByType
 * @tc.desc: Test get char with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0500, Function | MediumTest | Level1)
{
    const std::string value = "I";
    sptr<IInterface> charObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_CHAR, value);
    std::string charVal = WantParams::GetStringByType(charObj, WantParams::VALUE_TYPE_CHAR);
    EXPECT_EQ(charVal, "I");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0600
 * @tc.name: GetStringByType
 * @tc.desc: Test get short with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0600, Function | MediumTest | Level1)
{
    const std::string value = "123";
    sptr<IInterface> shortObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_SHORT, value);
    std::string shortVal = WantParams::GetStringByType(shortObj, WantParams::VALUE_TYPE_SHORT);
    EXPECT_EQ(shortVal, "123");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0700
 * @tc.name: GetStringByType
 * @tc.desc: Test get int with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0700, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> intObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_INT, value);
    std::string intVal = WantParams::GetStringByType(intObj, WantParams::VALUE_TYPE_INT);
    EXPECT_EQ(intVal, "-1");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0800
 * @tc.name: GetStringByType
 * @tc.desc: Test get long with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0800, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> longObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_LONG, value);
    std::string longVal = WantParams::GetStringByType(longObj, WantParams::VALUE_TYPE_LONG);
    EXPECT_EQ(longVal, "-1");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_0900
 * @tc.name: GetStringByType
 * @tc.desc: Test get float with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_0900, Function | MediumTest | Level1)
{
    const std::string value = "-1.0004";
    sptr<IInterface> floatObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_FLOAT, value);
    std::string floatVal = WantParams::GetStringByType(floatObj, WantParams::VALUE_TYPE_FLOAT);
    EXPECT_EQ(floatVal, "-1.000400");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_1000
 * @tc.name: GetStringByType
 * @tc.desc: Test get float with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_1000, Function | MediumTest | Level1)
{
    const std::string value = "-1.00000004";
    sptr<IInterface> doubleObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_DOUBLE, value);
    std::string doubleVal = WantParams::GetStringByType(doubleObj, WantParams::VALUE_TYPE_DOUBLE);
    EXPECT_EQ(doubleVal, "-1.000000");
}

/**
 * @tc.number: AaFwk_WantParams_GetStringByType_1100
 * @tc.name: GetStringByType
 * @tc.desc: Test get float with invalid type in the WantParam.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringByType_1100, Function | MediumTest | Level1)
{
    const std::string value = "I5{2,3,5,7,11}";
    sptr<IInterface> arrayObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, value);
    std::string arrayVal = WantParams::GetStringByType(arrayObj, WantParams::VALUE_TYPE_ARRAY);
    EXPECT_EQ(arrayVal, "I5{2,3,5,7,11}");
}

/**
 * @tc.number: AaFwk_WantParams_NewArrayData_0100
 * @tc.name: NewArrayData
 * @tc.desc: Test NewArrayData with invalid sptr<IArray> parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_NewArrayData_0100, Function | MediumTest | Level1)
{
    sptr<AAFwk::IArray> array = new (std::nothrow) AAFwk::Array(0, AAFwk::g_IID_IWantParams);
    WantParams wp;
    wp.SetParam("param1", array);
    IArray *iarray = IArray::Query(array);
    sptr<IArray> destAO = nullptr;
    bool result = wp.NewArrayData(iarray, destAO);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0100
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with invalid parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0100, Function | MediumTest | Level1)
{
    int type = WantParams::GetDataType(nullptr);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_NULL);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0200
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with bool parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0200, Function | MediumTest | Level1)
{
    const std::string value = "true";
    sptr<IInterface> boolObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BOOLEAN, value);
    int type = WantParams::GetDataType(boolObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_BOOLEAN);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0300
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with byte parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0300, Function | MediumTest | Level1)
{
    const std::string value = "129";
    sptr<IInterface> byteObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BYTE, value);
    int type = WantParams::GetDataType(byteObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_BYTE);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0400
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with char parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0400, Function | MediumTest | Level1)
{
    const std::string value = "I";
    sptr<IInterface> charObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_CHAR, value);
    int type = WantParams::GetDataType(charObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_CHAR);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0500
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with short parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0500, Function | MediumTest | Level1)
{
    const std::string value = "123";
    sptr<IInterface> shortObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_SHORT, value);
    int type = WantParams::GetDataType(shortObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_SHORT);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0600
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with int parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0600, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> intObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_INT, value);
    int type = WantParams::GetDataType(intObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_INT);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0700
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with long parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0700, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> longObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_LONG, "-1");
    int type = WantParams::GetDataType(longObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_LONG);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0800
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with float parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0800, Function | MediumTest | Level1)
{
    const std::string value = "-1.0004";
    sptr<IInterface> floatObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_FLOAT, "-1.0004");
    int type = WantParams::GetDataType(floatObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_FLOAT);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_0900
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with double parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_0900, Function | MediumTest | Level1)
{
    const std::string value = "-1.00000004";
    sptr<IInterface> doubleObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_DOUBLE, "-1.00000004");
    int type = WantParams::GetDataType(doubleObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_DOUBLE);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_1000
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with string parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_1000, Function | MediumTest | Level1)
{
    const std::string value = "hello";
    sptr<IInterface> stringObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_STRING, "hello");
    int type = WantParams::GetDataType(stringObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_STRING);
}

/**
 * @tc.number: AaFwk_WantParams_GetDataType_1100
 * @tc.name: GetDataType
 * @tc.desc: Test GetDataType with array parameter.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetDataType_1100, Function | MediumTest | Level1)
{
    const std::string value = "I5{2,3,5,7,11}";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, value);
    int type = WantParams::GetDataType(interfaceObj);
    EXPECT_EQ(type, WantParams::VALUE_TYPE_ARRAY);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0100
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with boolean type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0100, Function | MediumTest | Level1)
{
    const std::string value = "true";
    sptr<IInterface> boolObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BOOLEAN, value);
    EXPECT_TRUE(Boolean::Unbox(IBoolean::Query(boolObj)));
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0200
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with byte type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0200, Function | MediumTest | Level1)
{
    const std::string value = "129";
    sptr<IInterface> byteObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BYTE, value);
    EXPECT_EQ(Byte::Unbox(IByte::Query(byteObj)), 129);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0300
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with char type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0300, Function | MediumTest | Level1)
{
    const std::string value = "I";
    sptr<IInterface> charObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_CHAR, value);
    EXPECT_EQ(Char::Unbox(IChar::Query(charObj)), 'I');
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0400
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with short type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0400, Function | MediumTest | Level1)
{
    const std::string value = "123";
    sptr<IInterface> shortObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_SHORT, value);
    EXPECT_EQ(Short::Unbox(IShort::Query(shortObj)), 123);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0500
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with integer type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0500, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> intObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_INT, value);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(intObj)), -1);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0600
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with long type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0600, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> longObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_LONG, "-1");
    EXPECT_EQ(Long::Unbox(ILong::Query(longObj)), -1);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0700
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with float type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0700, Function | MediumTest | Level1)
{
    const std::string value = "-1.0004";
    sptr<IInterface> floatObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_FLOAT, "-1.0004");
    EXPECT_FLOAT_EQ(Float::Unbox(IFloat::Query(floatObj)), -1.0004);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0800
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with double type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0800, Function | MediumTest | Level1)
{
    const std::string value = "-1.00000004";
    sptr<IInterface> doubleObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_DOUBLE, "-1.00000004");
    EXPECT_DOUBLE_EQ(Double::Unbox(IDouble::Query(doubleObj)), -1.00000004);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_0900
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with string type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_0900, Function | MediumTest | Level1)
{
    const std::string value = "hello";
    sptr<IInterface> stringObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_STRING, "hello");
    EXPECT_EQ(String::Unbox(IString::Query(stringObj)), std::string("hello"));
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_1000
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with array type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_1000, Function | MediumTest | Level1)
{
    const std::string value = "I5{2,3,5,7,11}";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, value);
    Array* arrayObj = static_cast<Array *>(IArray::Query(interfaceObj));
    sptr<IInterface> valueObj;
    arrayObj->Get(0, valueObj);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(valueObj)), 2);
    arrayObj->Get(1, valueObj);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(valueObj)), 3);
    arrayObj->Get(2, valueObj);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(valueObj)), 5);
    arrayObj->Get(3, valueObj);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(valueObj)), 7);
    arrayObj->Get(4, valueObj);
    EXPECT_EQ(Integer::Unbox(IInteger::Query(valueObj)), 11);
}

/**
 * @tc.number: AaFwk_WantParams_GetInterfaceByType_1100
 * @tc.name: GetInterfaceByType
 * @tc.desc: Test GetInterfaceByType with invalid type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetInterfaceByType_1100, Function | MediumTest | Level1)
{
    const std::string value = "123";
    sptr<IInterface> arrayObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_NULL, value);
    EXPECT_EQ(arrayObj, nullptr);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0100
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with bool type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0100, Function | MediumTest | Level1)
{
    const std::string value = "true";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BOOLEAN, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_BOOLEAN);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0200
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with byte type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0200, Function | MediumTest | Level1)
{
    const std::string value = "129";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_BYTE, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_BYTE);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0300
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with char type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0300, Function | MediumTest | Level1)
{
    const std::string value = "I";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_CHAR, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_CHAR);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0400
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with short type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0400, Function | MediumTest | Level1)
{
    const std::string value = "123";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_SHORT, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_SHORT);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0500
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with integer type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0500, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_INT, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_INT);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0600
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with long type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0600, Function | MediumTest | Level1)
{
    const std::string value = "-1";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_LONG, "-1");
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_LONG);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0700
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with float type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0700, Function | MediumTest | Level1)
{
    const std::string value = "-1.0004";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_FLOAT, "-1.0004");
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_FLOAT);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0800
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with double type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0800, Function | MediumTest | Level1)
{
    const std::string value = "-1.00000004";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_DOUBLE, "-1.00000004");
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_DOUBLE);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_0900
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with string type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_0900, Function | MediumTest | Level1)
{
    const std::string value = "hello";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_STRING, "hello");
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_STRING);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_1000
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with array type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_1000, Function | MediumTest | Level1)
{
    const std::string value = "I5{2,3,5,7,11}";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, WantParams::VALUE_TYPE_ARRAY);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_CompareInterface_1002
 * @tc.name: CompareInterface
 * @tc.desc: Test CompareInterface with array type.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_CompareInterface_1002, Function | MediumTest | Level1)
{
    const std::string value = "I5{2,3,5,7,11}";
    sptr<IInterface> interfaceObj = WantParams::GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, value);
    bool result = WantParams::CompareInterface(interfaceObj, interfaceObj, 102);
    EXPECT_TRUE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelString_0100
 * @tc.name: WriteArrayToParcelString
 * @tc.desc: Test WriteArrayToParcelString string content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelString_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelString(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelBool_0100
 * @tc.name: WriteArrayToParcelBool
 * @tc.desc: Test WriteArrayToParcelBool bool content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelBool_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelBool(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelByte_0100
 * @tc.name: WriteArrayToParcelByte
 * @tc.desc: Test WriteArrayToParcelByte byte content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelByte_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelByte(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelChar_0100
 * @tc.name: WriteArrayToParcelChar
 * @tc.desc: Test WriteArrayToParcelChar char content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelChar_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelChar(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelShort_0100
 * @tc.name: WriteArrayToParcelShort
 * @tc.desc: Test WriteArrayToParcelShort short content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelShort_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelShort(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelInt_0100
 * @tc.name: WriteArrayToParcelInt
 * @tc.desc: Test WriteArrayToParcelInt int content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelInt_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelInt(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelLong_0100
 * @tc.name: WriteArrayToParcelLong
 * @tc.desc: Test WriteArrayToParcelLong long content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelLong_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelLong(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelFloat_0100
 * @tc.name: WriteArrayToParcelFloat
 * @tc.desc: Test WriteArrayToParcelFloat float content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelFloat_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelFloat(parcel, nullptr);
    EXPECT_FALSE(result);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelDouble_0100
 * @tc.name: WriteArrayToParcelDouble
 * @tc.desc: Test WriteArrayToParcelDouble double content.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelDouble_0100, Function | MediumTest | Level1)
{
    Parcel parcel;
    WantParams wp;
    bool result = wp.WriteArrayToParcelDouble(parcel, nullptr);
    EXPECT_FALSE(result);
}

class UnsupportedDataTest : public testing::Test {
public:
    UnsupportedDataTest()
    {}
    ~UnsupportedDataTest()
    {
    }
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();

    std::shared_ptr<UnsupportedData> unsupportedData_ = nullptr;
};

void UnsupportedDataTest::SetUpTestCase(void)
{}

void UnsupportedDataTest::TearDownTestCase(void)
{}

void UnsupportedDataTest::SetUp(void)
{
    unsupportedData_ = std::make_shared<UnsupportedData>();
}

void UnsupportedDataTest::TearDown(void)
{
}

/**
 * @tc.number: AaFwk_WantParams_GetStringParam_1000
 * @tc.name: GetStringParam
 * @tc.desc: Test GetStringParam.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetStringParam_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    std::string key = "";
    std::string result = wantParams.GetStringParam(key);
    EXPECT_EQ(result, "");
}

/**
 * @tc.number: AaFwk_WantParams_WriteToParcelFD_1000
 * @tc.name: WriteToParcelFD
 * @tc.desc: Test WriteToParcelFD.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteToParcelFD_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    WantParams value;
    bool result = wantParams.WriteToParcelFD(parcel, value);
    EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_WantParams_WriteToParcelRemoteObject_1000
 * @tc.name: WriteToParcelRemoteObject
 * @tc.desc: Test WriteToParcelRemoteObject.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteToParcelRemoteObject_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    WantParams value;
    bool result = wantParams.WriteToParcelRemoteObject(parcel, value);
    EXPECT_EQ(result, false);

    std::string keyStr = "vlan";
    std::string valueStr = "vlan";
    wantParamsIn_->SetParam(keyStr, String::Box(valueStr));
    bool result1 = wantParams.WriteToParcelRemoteObject(parcel, value);
    EXPECT_EQ(result1, false);
}

/**
 * @tc.number: AaFwk_WantParams_WriteArrayToParcelWantParams_1000
 * @tc.name: WriteArrayToParcelWantParams
 * @tc.desc: Test WriteArrayToParcelWantParams.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_WriteArrayToParcelWantParams_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    int depth = 1;
    bool result = wantParams.WriteArrayToParcelWantParams(parcel, nullptr, depth);
    EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_WantParams_ReadArrayToParcel_1000
 * @tc.name: ReadArrayToParcel
 * @tc.desc: Test ReadArrayToParcel.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_ReadArrayToParcel_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    int type = 1;
    sptr<IArray> destAO = nullptr;
    bool result = wantParams.ReadArrayToParcel(parcel, type, destAO, 1);
    EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_WantParams_ReadFromParcelFD_1000
 * @tc.name: ReadFromParcelFD
 * @tc.desc: Test ReadFromParcelFD.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_ReadFromParcelFD_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    std::string key = "this is key";
    bool result = wantParams.ReadFromParcelFD(parcel, key);
    EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_WantParams_ReadFromParcelRemoteObject_1000
 * @tc.name: ReadFromParcelRemoteObject
 * @tc.desc: Test ReadFromParcelRemoteObject.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_ReadFromParcelRemoteObject_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    std::string key = "this is key";
    bool result = wantParams.ReadFromParcelRemoteObject(parcel, key);
    EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_WantParams_ReadUnsupportedData_1000
 * @tc.name: ReadUnsupportedData
 * @tc.desc: Test ReadUnsupportedData.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_ReadUnsupportedData_1000, Function | MediumTest | Level1)
{
    WantParams wantParams;
    Parcel parcel;
    std::string key = "this is key";
    int type = 1;
    bool result = wantParams.ReadUnsupportedData(parcel, key, type);
    EXPECT_EQ(result, false);

    parcel.WriteInt32(-1);
    bool result1 = wantParams.ReadUnsupportedData(parcel, key, type);
    EXPECT_EQ(result1, false);

    int type1 = 50;
    bool result2 = wantParams.ReadFromParcelParam(parcel, key, type1, 1);
    EXPECT_EQ(result2, false);
    UnsupportedData other;
    std::shared_ptr<UnsupportedData> unsupportedData = std::make_shared<UnsupportedData>(other);
}

/**
 * @tc.number: AaFwk_WantParams_GetCachedUnsupportedData_1000
 * @tc.name: GetCachedUnsupportedData
 * @tc.desc: Test GetCachedUnsupportedData.
 * @tc.require: issueI648W6
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_GetCachedUnsupportedData_1000, Function | MediumTest | Level1)
{
    OHOS::AAFwk::WantParams wantParams;
    AAFwk::UnsupportedData data;
    data.key = Str8ToStr16("dataKey2");
    data.type = 2;
    data.size = 1;
    int32_t bufferSize = 256;
    data.buffer = new (std::nothrow) uint8_t[bufferSize];
    std::vector<AAFwk::UnsupportedData> cacheData;
    cacheData.emplace_back(std::move(data));
    wantParams.SetCachedUnsupportedData(cacheData);
    std::vector<AAFwk::UnsupportedData> getCacheData;
    wantParams.GetCachedUnsupportedData(getCacheData);
    EXPECT_EQ(1, getCacheData.size());
    for (auto item : getCacheData) {
        EXPECT_EQ(item.key, Str8ToStr16("dataKey2"));
        EXPECT_EQ(item.type, 2);
        EXPECT_EQ(item.size, 1);
    }
}

/**
 * @tc.number: AaFwk_WantParams_DupAllFd_1000
 * @tc.name: DupAllFd
 * @tc.desc: Test DupAllFd.
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_DupAllFd_1000, Function | MediumTest | Level1)
{
    std::string keyStr = "FD";
    std::string valueStr = "sdasdfdsffdgfdg";
    WantParams wantParams;
    wantParams.DupAllFd();
    wantParamsIn_->SetParam(keyStr, String::Box(valueStr));
    Parcel in;
    if (wantParamsOut_ != nullptr) {
        wantParamsIn_->Marshalling(in);
        std::shared_ptr<WantParams> wantParamsOut_(WantParams::Unmarshalling(in));
        EXPECT_EQ(valueStr, String::Unbox(IString::Query(wantParamsOut_->GetParam(keyStr))));
    }
}

/**
 * @tc.number: AaFwk_WantParams_FromJson_0100
 * @tc.name: FromJson
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_FromJson_0100, Function | MediumTest | Level1)
{
    WantParams params;
    nlohmann::json js;
    js["strKey"] = "hello";

    params.FromJson(js);

    EXPECT_EQ(params.GetStringParam("strKey"), "hello");
}

/**
 * @tc.number: AaFwk_WantParams_ToJson_0100
 * @tc.name: ToJson
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_ToJson_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("intKey", OHOS::AAFwk::Integer::Box(123456));
    params.SetParam("strKey", OHOS::AAFwk::String::Box("aaaaa"));

    nlohmann::json js;
    params.ToJson(js);

    EXPECT_TRUE(js.contains("intKey"));
    EXPECT_TRUE(js.contains("strKey"));
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsByte_0100
 * @tc.name: InnerWrapWantParamsByte
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsByte_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("byteKey", OHOS::AAFwk::Byte::Box(66)); // 66 = 'B'
    nlohmann::json js;
    params.InnerWrapWantParamsByte(js, "byteKey");
    ASSERT_TRUE(js.contains("byteKey"));
    EXPECT_EQ(js["byteKey"], 66);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsChar_0100
 * @tc.name: InnerWrapWantParamsChar
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsChar_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("charKey", OHOS::AAFwk::Char::Box('A'));

    nlohmann::json js;
    params.InnerWrapWantParamsChar(js, "charKey");

    ASSERT_TRUE(js.contains("charKey"));
    EXPECT_EQ(js["charKey"], "A");
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsShort_0100
 * @tc.name: InnerWrapWantParamsShort
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsShort_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("shortKey", OHOS::AAFwk::Short::Box(12345));

    nlohmann::json js;
    params.InnerWrapWantParamsShort(js, "shortKey");

    ASSERT_TRUE(js.contains("shortKey"));
    EXPECT_EQ(js["shortKey"], 12345);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsBool_0100
 * @tc.name: InnerWrapWantParamsBool
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsBool_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("boolKey", OHOS::AAFwk::Boolean::Box(true));

    nlohmann::json js;
    params.InnerWrapWantParamsBool(js, "boolKey");

    ASSERT_TRUE(js.contains("boolKey"));
    EXPECT_EQ(js["boolKey"], true);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsString_0100
 * @tc.name: InnerWrapWantParamsString
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsString_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("strKey", OHOS::AAFwk::String::Box("OpenHarmony"));

    nlohmann::json js;
    params.InnerWrapWantParamsString(js, "strKey");

    ASSERT_TRUE(js.contains("strKey"));
    EXPECT_EQ(js["strKey"], "OpenHarmony");
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsInt32_0100
 * @tc.name: InnerWrapWantParamsInt32
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsInt32_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("int32Key", OHOS::AAFwk::Integer::Box(123456));
    nlohmann::json js;
    params.InnerWrapWantParamsInt32(js, "int32Key");
    ASSERT_TRUE(js.contains("int32Key"));
    EXPECT_EQ(js["int32Key"], 123456);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsInt64_0100
 * @tc.name: InnerWrapWantParamsInt64
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsInt64_0100, Function | MediumTest | Level1)
{
    WantParams params;
    long value = 12345678;
    params.SetParam("int64Key", OHOS::AAFwk::Long::Box(value));
    nlohmann::json js;
    params.InnerWrapWantParamsInt64(js, "int64Key");
    ASSERT_TRUE(js.contains("int64Key"));
    EXPECT_EQ(js["int64Key"], 12345678);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsFloat_0100
 * @tc.name: InnerWrapWantParamsFloat
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsFloat_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("floatKey", OHOS::AAFwk::Float::Box(3.14f));
    nlohmann::json js;
    params.InnerWrapWantParamsFloat(js, "floatKey");
    ASSERT_TRUE(js.contains("floatKey"));
    EXPECT_FLOAT_EQ(js["floatKey"], 3.14f);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsDouble_0100
 * @tc.name: InnerWrapWantParamsDouble
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsDouble_0100, Function | MediumTest | Level1)
{
    WantParams params;
    class DoubleInterface : public IInterface {
    public:
        explicit DoubleInterface(double v) : value(v) {}
        double value;
    };
    params.SetParam("doubleKey", OHOS::AAFwk::Double::Box(6.28));
    nlohmann::json js;
    params.InnerWrapWantParamsDouble(js, "doubleKey");
    ASSERT_TRUE(js.contains("doubleKey"));
    EXPECT_DOUBLE_EQ(js["doubleKey"], 6.28);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsWantParams_0100
 * @tc.name: InnerWrapWantParamsWantParams
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsWantParams_0100, Function | MediumTest | Level1)
{
    WantParams params;
    WantParams inner;
    inner.SetParam("innerKey", nullptr);
    params.SetParam("wantParamsKey", AAFwk::WantParamWrapper::Box(inner));
    nlohmann::json js;
    params.InnerWrapWantParamsWantParams(js, "wantParamsKey");
    ASSERT_TRUE(js.contains("wantParamsKey"));
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayString_0100
 * @tc.name: InnerWrapWantParamsArrayString
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayString_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IString);
    ao->Set(0, OHOS::AAFwk::String::Box("a"));
    ao->Set(1, OHOS::AAFwk::String::Box("b"));
    ao->Set(2, OHOS::AAFwk::String::Box("c"));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayString(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayBool_0100
 * @tc.name: InnerWrapWantParamsArrayBool
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayBool_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IBoolean);
    ao->Set(0, OHOS::AAFwk::Boolean::Box(true));
    ao->Set(1, OHOS::AAFwk::Boolean::Box(false));
    ao->Set(2, OHOS::AAFwk::Boolean::Box(true));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayBool(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayShort_0100
 * @tc.name: InnerWrapWantParamsArrayShort
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayShort_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IShort);
    ao->Set(0, OHOS::AAFwk::Short::Box(1));
    ao->Set(1, OHOS::AAFwk::Short::Box(2));
    ao->Set(2, OHOS::AAFwk::Short::Box(3));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayShort(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayByte_0100
 * @tc.name: InnerWrapWantParamsArrayByte
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayByte_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IByte);
    ao->Set(0, OHOS::AAFwk::Byte::Box(1));
    ao->Set(1, OHOS::AAFwk::Byte::Box(2));
    ao->Set(2, OHOS::AAFwk::Byte::Box(3));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayByte(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayInt32_0100
 * @tc.name: InnerWrapWantParamsArrayInt32
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayInt32_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IInteger);
    ao->Set(0, OHOS::AAFwk::Integer::Box(10));
    ao->Set(1, OHOS::AAFwk::Integer::Box(20));
    ao->Set(2, OHOS::AAFwk::Integer::Box(30));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayInt32(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayInt64_0100
 * @tc.name: InnerWrapWantParamsArrayInt64
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayInt64_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_ILong);
    ao->Set(0, OHOS::AAFwk::Long::Box(100));
    ao->Set(1, OHOS::AAFwk::Long::Box(200));
    ao->Set(2, OHOS::AAFwk::Long::Box(300));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayInt64(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayFloat_0100
 * @tc.name: InnerWrapWantParamsArrayFloat
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayFloat_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IFloat);
    ao->Set(0, OHOS::AAFwk::Float::Box(1.1f));
    ao->Set(1, OHOS::AAFwk::Float::Box(2.2f));
    ao->Set(2, OHOS::AAFwk::Float::Box(3.3f));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayFloat(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayWantParams_0100
 * @tc.name: InnerWrapWantParamsArrayWantParams
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayWantParams_0100, Function | MediumTest | Level1)
{
    WantParams params;
    WantParams wp1, wp2;
    wp1.SetParam("k1", nullptr);
    wp2.SetParam("k2", nullptr);
    sptr<IArray> wantParamsArray = new Array(2, g_IID_IWantParams);
    wantParamsArray->Set(0, AAFwk::WantParamWrapper::Box(wp1));
    wantParamsArray->Set(1, AAFwk::WantParamWrapper::Box(wp2));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayWantParams(js, "arrKey", wantParamsArray);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 2);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayChar_0100
 * @tc.name: InnerWrapWantParamsArrayChar
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayChar_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IChar);
    ao->Set(0, OHOS::AAFwk::Char::Box('x'));
    ao->Set(1, OHOS::AAFwk::Char::Box('y'));
    ao->Set(2, OHOS::AAFwk::Char::Box('z'));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayChar(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArrayDouble_0100
 * @tc.name: InnerWrapWantParamsArrayDouble
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArrayDouble_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IDouble);
    ao->Set(0, OHOS::AAFwk::Double::Box(1.11));
    ao->Set(1, OHOS::AAFwk::Double::Box(2.22));
    ao->Set(2, OHOS::AAFwk::Double::Box(3.33));
    nlohmann::json js;
    params.InnerWrapWantParamsArrayDouble(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_InnerWrapWantParamsArray_0100
 * @tc.name: InnerWrapWantParamsArray
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_InnerWrapWantParamsArray_0100, Function | MediumTest | Level1)
{
    WantParams params;
    sptr<IArray> ao = new Array(3, g_IID_IString);
    ao->Set(0, OHOS::AAFwk::String::Box("a"));
    ao->Set(1, OHOS::AAFwk::String::Box("b"));
    ao->Set(2, OHOS::AAFwk::String::Box("c"));
    nlohmann::json js;
    params.InnerWrapWantParamsArray(js, "arrKey", ao);
    ASSERT_TRUE(js.contains("arrKey"));
    EXPECT_EQ(js["arrKey"].size(), 3);
}

/**
 * @tc.number: AaFwk_WantParams_from_json_0100
 * @tc.name: from_json
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_from_json_0100, Function | MediumTest | Level1)
{
    nlohmann::json js;
    js["intKey"] = 123;
    js["strKey"] = "OpenHarmony";
    WantParams params;
    from_json(js, params);
    EXPECT_EQ(params.GetIntParam("intKey", 0), 123);
    EXPECT_EQ(params.GetStringParam("strKey"), "OpenHarmony");
}

/**
 * @tc.number: AaFwk_WantParams_to_json_0100
 * @tc.name: to_json
 * @tc.desc:
 */
HWTEST_F(WantParamsBaseTest, AaFwk_WantParams_to_json_0100, Function | MediumTest | Level1)
{
    WantParams params;
    params.SetParam("strKey", OHOS::AAFwk::String::Box("OpenHarmony"));
    nlohmann::json js;
    to_json(js, params);
    ASSERT_TRUE(js.contains("strKey"));
    EXPECT_EQ(js["strKey"], "OpenHarmony");
}
}
}
