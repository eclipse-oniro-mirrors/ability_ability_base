/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "view_data.h"
#include "session_info.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AbilityBase {
namespace {
    static const std::string TJSON =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_BUNDLE_NAME =
        "{\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_BUNDLE_NAME_NOT_STRING =
        "{\"bundleName\":0,\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_MODULE_NAME =
        "{\"bundleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_MODULE_NAME_NOT_STRING =
        "{\"bundleName\":\"\",\"moduleName\":0,\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_ABILITY_NAME =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_ABILITY_NAME_NOT_STRING =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":0,\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_PAGE_URL =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_PAGE_URL_NOT_STRING =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":0,\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_USER_SELECTED =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"nodes\":[],"
        "\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_USER_SELECTED_NOT_BOOL =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":0,\"nodes\":[],"
        "\"isUserSelected\":\"\",\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_OTHER_ACCOUNT =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_OTHER_ACCOUNT_NOT_BOOL =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":\"{\\\"left\\\":0,\\\"top\\\":0,\\\"width\\\":0,\\\"height\\\":0}\"}";

    static const std::string TJSON_WITHOUT_PAGE_RECT =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false";

    static const std::string TJSON_PAGE_RECT_NOT_STRING =
        "{\"bundleName\":\"\",\"moduleName\":\"\",\"abilityName\":\"\",\"pageUrl\":\"\",\"nodes\":[],"
        "\"isUserSelected\":false,\"isOtherAccount\":false,"
        "\"pageRect\":0";
} // namespace
class ViewDataTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void ViewDataTest::SetUpTestCase()
{}

void ViewDataTest::TearDownTestCase()
{}

void ViewDataTest::SetUp()
{}

void ViewDataTest::TearDown()
{}

/**
 * @tc.name: FromJsonString_100
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_100, TestSize.Level1)
{
    std::string jsonStr = "jsonStr";
    std::shared_ptr<ViewData> viewdata = std::make_shared<ViewData>();
    EXPECT_TRUE(viewdata != nullptr);
    viewdata->FromJsonString(jsonStr);
    viewdata->FromJsonString(TJSON);
    std::string ret = viewdata->ToJsonString();
    EXPECT_NE(ret, jsonStr);

    viewdata->FromJsonString(TJSON_WITHOUT_BUNDLE_NAME);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_BUNDLE_NAME);

    viewdata->FromJsonString(TJSON_BUNDLE_NAME_NOT_STRING);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_BUNDLE_NAME_NOT_STRING);

    viewdata->FromJsonString(TJSON_WITHOUT_MODULE_NAME);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_MODULE_NAME);

    viewdata->FromJsonString(TJSON_MODULE_NAME_NOT_STRING);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_MODULE_NAME_NOT_STRING);

    viewdata->FromJsonString(TJSON_WITHOUT_ABILITY_NAME);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_ABILITY_NAME);

    viewdata->FromJsonString(TJSON_ABILITY_NAME_NOT_STRING);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_ABILITY_NAME_NOT_STRING);
}

/**
 * @tc.name: FromJsonString_200
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_200, TestSize.Level1)
{
    std::string jsonStr = "jsonStr";
    std::shared_ptr<ViewData> viewdata = std::make_shared<ViewData>();
    EXPECT_TRUE(viewdata != nullptr);

    viewdata->FromJsonString(TJSON_WITHOUT_PAGE_URL);
    std::string ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_PAGE_URL);

    viewdata->FromJsonString(TJSON_PAGE_URL_NOT_STRING);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_PAGE_URL_NOT_STRING);

    viewdata->FromJsonString(TJSON_WITHOUT_USER_SELECTED);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_USER_SELECTED);

    viewdata->FromJsonString(TJSON_USER_SELECTED_NOT_BOOL);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_USER_SELECTED_NOT_BOOL);

    viewdata->FromJsonString(TJSON_WITHOUT_OTHER_ACCOUNT);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_OTHER_ACCOUNT);

    viewdata->FromJsonString(TJSON_OTHER_ACCOUNT_NOT_BOOL);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_OTHER_ACCOUNT_NOT_BOOL);

    viewdata->FromJsonString(TJSON_WITHOUT_PAGE_RECT);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_WITHOUT_PAGE_RECT);

    viewdata->FromJsonString(TJSON_PAGE_RECT_NOT_STRING);
    ret = viewdata->ToJsonString();
    EXPECT_NE(ret, TJSON_PAGE_RECT_NOT_STRING);
}

/**
 * @tc.name: Unmarshalling_100
 * @tc.desc: SessionInfo test for Unmarshalling.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, Unmarshalling_100, TestSize.Level1)
{
    std::shared_ptr<AAFwk::SessionInfo> sessioninfo = std::make_shared<AAFwk::SessionInfo>();
    EXPECT_TRUE(sessioninfo != nullptr);
    Parcel parcel;
    auto ret = sessioninfo->Unmarshalling(parcel);
    EXPECT_NE(ret, nullptr);
}
}  // namespace AbilityBase
}  // namespace OHOS
