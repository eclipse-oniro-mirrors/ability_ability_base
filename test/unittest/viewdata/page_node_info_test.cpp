/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include "page_node_info.h"
#include "session_info.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AbilityBase {
namespace {
    static const std::string TJSON =
        "{\"autoFillType\":0,\"depth\":-1,\"enableAutoFill\":true,\"id\":-1,\"isFocus\":false,"
        "\"metadata\":\"\",\"passwordRules\":\"\",\"placeholder\":\"\","
        "\"rect\":\"{\\\"height\\\":0.0,\\\"left\\\":0.0,\\\"top\\\":0.0,"
        "\\\"width\\\":0.0}\",\"tag\":\"\",\"value\":\"\"}";
} // namespace      

class PageNodeInfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void PageNodeInfoTest::SetUpTestCase()
{}

void PageNodeInfoTest::TearDownTestCase()
{}

void PageNodeInfoTest::SetUp()
{}

void PageNodeInfoTest::TearDown()
{}

/**
 * @tc.name: FromJsonString_100
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_100, TestSize.Level1)
{
    std::string jsonStr = "jsonStr";
    std::shared_ptr<PageNodeInfo> pageNodeInfo = std::make_shared<PageNodeInfo>();
    EXPECT_TRUE(pageNodeInfo != nullptr);
    pageNodeInfo->FromJsonString(jsonStr);
    pageNodeInfo->FromJsonString(TJSON);
    std::string ret = pageNodeInfo->ToJsonString();
    EXPECT_EQ(ret, TJSON);
}

/**
 * @tc.name: Unmarshalling_100
 * @tc.desc: SessionInfo test for Unmarshalling.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, Unmarshalling_100, TestSize.Level1)
{
    std::shared_ptr<AAFwk::SessionInfo> sessioninfo = std::make_shared<AAFwk::SessionInfo>();
    EXPECT_TRUE(sessioninfo != nullptr);
    Parcel parcel;
    auto ret = sessioninfo->Unmarshalling(parcel);
    EXPECT_NE(ret, nullptr);
}
}  // namespace AbilityBase
}  // namespace OHOS