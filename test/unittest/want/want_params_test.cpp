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

#include "array_wrapper.h"
#include "string_wrapper.h"
#include "bool_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"

#include "want_params.h"
#include "want_params_wrapper.h"

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
    std::string outString(String::Unbox(IString::Query(wantParamsOut_->GetParam(keyStr))));
    EXPECT_STREQ(std::to_string(valueLong).c_str(), outString.c_str());
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
    sptr<AAFwk::IArray> ao = new (std::nothrow) AAFwk::Array(2, AAFwk::g_IID_IWantParams);
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
}
}
