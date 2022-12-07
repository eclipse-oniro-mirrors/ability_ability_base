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
#include "extra_params.h"
#undef private
#undef protected

using namespace testing::ext;

namespace OHOS {
namespace AppExecFwk {
class ExtraParamsBaseTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: AppExecFwk_ExtraParams_SetDevType_0100
 * @tc.name: SetDevType
 * @tc.desc: 
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_SetDevType_0100, Function | MediumTest | Level1)
{
    std::vector<string> devType;
    ExtraParams extraParams;
    extraParams.SetDevType(devType);
    std::vector<string> result = extraParams.GetDevType();
    EXPECT_EQ(result, devType);
}

/**
 * @tc.number: AppExecFwk_ExtraParams_SetTargetBundleName_0100
 * @tc.name: SetTargetBundleName
 * @tc.desc: 
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_SetTargetBundleName_0100, Function | MediumTest | Level1)
{
    string targetBundleName;
    ExtraParams extraParams;
    extraParams.SetTargetBundleName(targetBundleName);
    string result = extraParams.GetTargetBundleName();
    EXPECT_EQ(result, targetBundleName);
}

/**
 * @tc.number: AppExecFwk_ExtraParams_SetJsonParams_0100
 * @tc.name: SetJsonParams
 * @tc.desc: 
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_SetJsonParams_0100, Function | MediumTest | Level1)
{
    string jsonParams;
    ExtraParams extraParams;
    extraParams.SetJsonParams(jsonParams);
    string result = extraParams.GetJsonParams();
    EXPECT_EQ(result, jsonParams);
}

/**
 * @tc.number: AppExecFwk_ExtraParams_SetDescription_0100
 * @tc.name: SetDescription
 * @tc.desc: 
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_SetDescription_0100, Function | MediumTest | Level1)
{
    string description;
    ExtraParams extraParams;
    extraParams.SetDescription(description);
    string result = extraParams.GetDescription();
    EXPECT_EQ(result, description);
}

/**
 * @tc.number: AppExecFwk_ExtraParams_Marshallin_0100
 * @tc.name: Marshallin
 * @tc.desc: 
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_Marshallin_0100, Function | MediumTest | Level1)
{
    ExtraParams extraParams;
    std::vector<string> devType;
    extraParams.SetDevType(devType);
    string targetBundleName;
    extraParams.SetTargetBundleName(targetBundleName);
    string jsonParams;
    extraParams.SetJsonParams(jsonParams);
    string description;
    extraParams.SetDescription(description);
    Parcel parcel;
    bool result = extraParams.Marshalling(parcel);
    EXPECT_EQ(result, true);
}

/**
 * @tc.number: AppExecFwk_ExtraParams_Unmarshalling_0100
 * @tc.name: Unmarshalling
 * @tc.desc: Test Unmarshalling.
 * @tc.require: issueI653GZ
 */
HWTEST_F(ExtraParamsBaseTest, AppExecFwk_ExtraParams_Unmarshalling_0100, Function | MediumTest | Level1)
{
    ExtraParams other;
    ExtraParams oextraParams(other);
    oextraParams.GetTargetBundleName();
    std::vector<string> devType;
    string targetBundleName = "TargetBundleName";
    string description = "Description";
    string jsonParams;
    ExtraParams extraParams(devType, targetBundleName, description, jsonParams);
    Parcel parcel;
    extraParams.Unmarshalling(parcel);
    string result = extraParams.GetTargetBundleName();
    EXPECT_EQ(result, targetBundleName);
}
}
}
