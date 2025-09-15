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

/**
 * @tc.name: ToJsonString_001
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoToJsonTest, ToJsonString_001, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = "test_id";
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USERNAME;
    testNode.tag = "input";
    testNode.value = "test_value";
    testNode.placeholder = "test_placeholder";
    testNode.passwordRules = "test_rules";
    testNode.metadata = "test_metadata";
    testNode.enableAutoFill = true;
    testNode.rect = {1, 2, 3, 4}; // Assuming Rect has a constructor
    testNode.isFocus = false;

    std::string result = testNode.ToJsonString();
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ID], "test_id");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_DEPTH], 2);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_AUTO_FILL_TYPE], static_cast<int32_t>(AutoFillType::USERNAME));
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_TAG], "input");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_VALUE], "test_value");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_PLACEHOLDER], "test_placeholder");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_PASSWORD_RULES], "test_rules");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_META_DATA], "test_metadata");
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ENABLE_AUTO_FILL], true);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_IS_FOCUS], false);
    
    // Test nested rect object
    auto rectJson = nlohmann::json::parse(jsonResult[PAGE_NODE_INFO_RECT].get<std::string>());
    EXPECT_FALSE(rectJson.empty());
}

/**
 * @tc.name: ToJsonString_002
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoToJsonTest, ToJsonString_002, TestSize.Level1) {
    PageNodeInfo emptyNode;
    std::string result = emptyNode.ToJsonString();
    
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_ID].empty());
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_DEPTH], 0);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_AUTO_FILL_TYPE], static_cast<int32_t>(AutoFillType::NONE));
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_TAG].empty());
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_VALUE].empty());
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_PLACEHOLDER].empty());
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_PASSWORD_RULES].empty());
    EXPECT_TRUE(jsonResult[PAGE_NODE_INFO_META_DATA].empty());
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ENABLE_AUTO_FILL], false);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_IS_FOCUS], false);
    
    // Test nested rect object
    auto rectJson = nlohmann::json::parse(jsonResult[PAGE_NODE_INFO_RECT].get<std::string>());
    EXPECT_FALSE(rectJson.empty());
}

/**
 * @tc.name: ToJsonString_003
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoToJsonTest, ToJsonString_003, TestSize.Level1) {
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = "test_id";
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USERNAME;
    testNode.tag = "input";
    testNode.value = "test_value";
    testNode.placeholder = "test_placeholder";
    testNode.passwordRules = "test_rules";
    testNode.metadata = "test_metadata";
    testNode.enableAutoFill = true;
    testNode.rect = {1, 2, 3, 4}; // Assuming Rect has a constructor
    testNode.isFocus = false;
    testNode.value = "special\"chars\\/\b\f\n\r\t";
    std::string result = testNode.ToJsonString();
    
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_VALUE], "special\"chars\\/\b\f\n\r\t");
}

/**
 * @tc.name: ToJsonString_004
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoToJsonTest, ToJsonString_004, TestSize.Level1) {
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = "test_id";
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USERNAME;
    testNode.tag = "input";
    testNode.value = "test_value";
    testNode.placeholder = "test_placeholder";
    testNode.passwordRules = "test_rules";
    testNode.metadata = "test_metadata";
    testNode.enableAutoFill = true;
    testNode.rect = {1, 2, 3, 4}; // Assuming Rect has a constructor
    testNode.isFocus = false;

    std::string result = testNode.ToJsonString();
    
    // This will throw if string is not valid JSON
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    EXPECT_FALSE(jsonResult.is_null());
}
}  // namespace AbilityBase
}  // namespace OHOS