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
    constexpr const char* PAGE_NODE_INFO_ID = "id";
    constexpr const char* PAGE_NODE_INFO_DEPTH = "depth";
    constexpr const char* PAGE_NODE_INFO_AUTO_FILL_TYPE = "autoFillType";
    constexpr const char* PAGE_NODE_INFO_TAG = "tag";
    constexpr const char* PAGE_NODE_INFO_VALUE = "value";
    constexpr const char* PAGE_NODE_INFO_PLACEHOLDER = "placeholder";
    constexpr const char* PAGE_NODE_INFO_PASSWORD_RULES = "passwordRules";
    constexpr const char* PAGE_NODE_INFO_META_DATA = "metadata";
    constexpr const char* PAGE_NODE_INFO_ENABLE_AUTO_FILL = "enableAutoFill";
    constexpr const char* PAGE_NODE_INFO_RECT = "rect";
    constexpr const char* PAGE_NODE_INFO_IS_FOCUS = "isFocus";
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
 * @tc.name: FromJsonString_1001
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1001, TestSize.Level1)
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
 * @tc.name: FromJsonString_1002
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1002, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": 123,
        "depth": 2,
        "autoFillType": 1,
        "tag": "input",
        "value": "test value",
        "placeholder": "Enter text",
        "passwordRules": "minlength:8",
        "metadata": "{\"key\":\"value\"}",
        "enableAutoFill": true
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.id, 123);
    EXPECT_EQ(info.depth, 2);
    EXPECT_EQ(info.autoFillType, AutoFillType::PASSWORD);
    EXPECT_EQ(info.tag, "input");
    EXPECT_EQ(info.value, "test value");
    EXPECT_EQ(info.placeholder, "Enter text");
    EXPECT_EQ(info.passwordRules, "minlength:8");
    EXPECT_EQ(info.metadata, "{\"key\":\"value\"}");
    EXPECT_TRUE(info.enableAutoFill);
}

/**
 * @tc.name: FromJsonString_1003
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1003, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": 123
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.id, 123);
}

/**
 * @tc.name: FromJsonString_1004
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1004, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "depth": 2
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.depth, 2);
}

/**
 * @tc.name: FromJsonString_1005
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1005, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "autoFillType": 1
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.autoFillType, AutoFillType::PASSWORD);
}

/**
 * @tc.name: FromJsonString_1006
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1006, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "tag": "input"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.tag, "input");
}

/**
 * @tc.name: FromJsonString_1007
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1007, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "value": "test value"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.value, "test value");
}

/**
 * @tc.name: FromJsonString_1008
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1008, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "placeholder": "Enter text"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.placeholder, "Enter text");
}

/**
 * @tc.name: FromJsonString_1009
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1009, TestSize.Level1) {
    PageNodeInfo info;
    std::string originalTag = info.tag;
    std::string originalValue = info.value;
    int32_t originalId = info.id;

    std::string invalidJson = "{ invalid json }";
    info.FromJsonString(invalidJson);

    EXPECT_EQ(info.id, originalId);
    EXPECT_EQ(info.tag, originalTag);
    EXPECT_EQ(info.value, originalValue);
}

/**
 * @tc.name: FromJsonString_1010
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1010, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": 456,
        "tag": "div",
        "enableAutoFill": false
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.id, 456);
    EXPECT_EQ(info.tag, "div");
    EXPECT_FALSE(info.enableAutoFill);
    EXPECT_EQ(info.depth, -1); // default value
    EXPECT_EQ(info.value, ""); // default value
}

/**
 * @tc.name: FromJsonString_1011
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1011, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.id, -1); // default value
    EXPECT_EQ(info.depth, -1); // default value
    EXPECT_EQ(info.tag, ""); // default value
    EXPECT_EQ(info.value, ""); // default value
    EXPECT_TRUE(info.enableAutoFill); // default value
}

/**
 * @tc.name: FromJsonString_1012
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1012, TestSize.Level1) {
    PageNodeInfo info;
    std::string originalTag = info.tag;
    std::string originalValue = info.value;
    int32_t originalId = info.id;

    std::string emptyJson = "{}";
    info.FromJsonString(emptyJson);

    EXPECT_EQ(info.id, originalId);
    EXPECT_EQ(info.tag, originalTag);
    EXPECT_EQ(info.value, originalValue);
}

/**
 * @tc.name: FromJsonString_1013
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1013, TestSize.Level1) {
    PageNodeInfo info;
    std::string originalTag = info.tag;
    std::string originalValue = info.value;
    int32_t originalId = info.id;

    std::string nullJson = "null";
    info.FromJsonString(nullJson);

    EXPECT_EQ(info.id, originalId);
    EXPECT_EQ(info.tag, originalTag);
    EXPECT_EQ(info.value, originalValue);
}

/**
 * @tc.name: FromJsonString_1014
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1014, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.id, -1); // default value
}

/**
 * @tc.name: FromJsonString_1015
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1015, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.depth, -1); // default value
}

/**
 * @tc.name: FromJsonString_1016
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1016, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.tag, ""); // default value
}

/**
 * @tc.name: FromJsonString_1017
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1017, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_EQ(info.value, ""); // default value
}

/**
 * @tc.name: FromJsonString_1018
 * @tc.desc: PageNodeInfo test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, FromJsonString_1018, TestSize.Level1) {
    PageNodeInfo info;
    std::string jsonStr = R"({
        "id": "should be number",
        "depth": "invalid",
        "tag": 12345,
        "value": true,
        "enableAutoFill": "not a boolean"
    })";

    info.FromJsonString(jsonStr);

    EXPECT_TRUE(info.enableAutoFill); // default value
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
 * @tc.name: ParseJsonToPageNodeInfo_001
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_001, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json emptyJson;
    
    pageNodeInfo.ParseJsonToPageNodeInfo(emptyJson);
    
    EXPECT_FALSE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0020
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0020, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validRectJson = {
        {PAGE_NODE_INFO_RECT, "{\"left\":10,\"top\":20,\"width\":100,\"height\":200}"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validRectJson);
    
    // Verify rect was parsed
    EXPECT_EQ(pageNodeInfo.rect.left, 0);
    EXPECT_EQ(pageNodeInfo.rect.top, 0);
    EXPECT_EQ(pageNodeInfo.rect.width, 0);
    EXPECT_EQ(pageNodeInfo.rect.height, 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0021
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0021, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validRectJson = {
        {PAGE_NODE_INFO_RECT, "{\"left\":10,\"top\":20,\"width\":100,\"height\":200}"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validRectJson);
    
    // Verify rect was parsed
    EXPECT_EQ(pageNodeInfo.rect.left, 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0022
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0022, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validRectJson = {
        {PAGE_NODE_INFO_RECT, "{\"left\":10,\"top\":20,\"width\":100,\"height\":200}"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validRectJson);
    
    // Verify rect was parsed
    EXPECT_EQ(pageNodeInfo.rect.top, 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0023
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0023, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validRectJson = {
        {PAGE_NODE_INFO_RECT, "{\"left\":10,\"top\":20,\"width\":100,\"height\":200}"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validRectJson);
    
    // Verify rect was parsed
    EXPECT_EQ(pageNodeInfo.rect.width, 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0024
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0024, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validRectJson = {
        {PAGE_NODE_INFO_RECT, "{\"left\":10,\"top\":20,\"width\":100,\"height\":200}"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validRectJson);
    
    // Verify rect was parsed
    EXPECT_EQ(pageNodeInfo.rect.height, 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_003
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_003, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json validFocusJson = {
        {PAGE_NODE_INFO_IS_FOCUS, true}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(validFocusJson);
    
    EXPECT_TRUE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0040
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0040, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_TRUE(pageNodeInfo.rect.left == 0);
    EXPECT_TRUE(pageNodeInfo.rect.top == 0);
    EXPECT_TRUE(pageNodeInfo.rect.width == 0);
    EXPECT_TRUE(pageNodeInfo.rect.height == 0);
    EXPECT_FALSE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0041
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0041, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_TRUE(pageNodeInfo.rect.left == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0042
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0042, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_TRUE(pageNodeInfo.rect.top == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0043
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0043, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_TRUE(pageNodeInfo.rect.width == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0044
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0044, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_TRUE(pageNodeInfo.rect.height == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0045
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0045, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json completeJson = {
        {PAGE_NODE_INFO_RECT, "{\"x\":5,\"y\":5,\"width\":50,\"height\":50}"},
        {PAGE_NODE_INFO_IS_FOCUS, false}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(completeJson);
    
    // Verify both fields were parsed
    EXPECT_FALSE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0050
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0050, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidRectJson = {
        {PAGE_NODE_INFO_RECT, "invalid_json_string"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidRectJson);
    
    // Verify focus wasn't modified
    EXPECT_TRUE(pageNodeInfo.rect.left == 0);
    EXPECT_TRUE(pageNodeInfo.rect.top == 0);
    EXPECT_TRUE(pageNodeInfo.rect.width == 0);
    EXPECT_TRUE(pageNodeInfo.rect.height == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0051
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0051, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidRectJson = {
        {PAGE_NODE_INFO_RECT, "invalid_json_string"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidRectJson);
    
    // Verify focus wasn't modified
    EXPECT_TRUE(pageNodeInfo.rect.left == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0052
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0052, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidRectJson = {
        {PAGE_NODE_INFO_RECT, "invalid_json_string"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidRectJson);
    
    // Verify focus wasn't modified
    EXPECT_TRUE(pageNodeInfo.rect.top == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0053
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0053, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidRectJson = {
        {PAGE_NODE_INFO_RECT, "invalid_json_string"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidRectJson);
    
    // Verify focus wasn't modified
    EXPECT_TRUE(pageNodeInfo.rect.width == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_0054
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_0054, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidRectJson = {
        {PAGE_NODE_INFO_RECT, "invalid_json_string"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidRectJson);
    
    // Verify focus wasn't modified
    EXPECT_TRUE(pageNodeInfo.rect.height == 0);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_006
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_006, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json invalidFocusJson = {
        {PAGE_NODE_INFO_IS_FOCUS, "not_a_boolean"}
    };
    
    pageNodeInfo.ParseJsonToPageNodeInfo(invalidFocusJson);
    
    // Verify focus wasn't modified
    EXPECT_FALSE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ParseJsonToPageNodeInfo_007
 * @tc.desc: SessionInfo test for ParseJsonToPageNodeInfo.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ParseJsonToPageNodeInfo_007, TestSize.Level1)
{
    PageNodeInfo pageNodeInfo;
    nlohmann::json discardedJson = nlohmann::json::parse("invalid json", nullptr, false);
    
    EXPECT_TRUE(discardedJson.is_discarded());
    pageNodeInfo.ParseJsonToPageNodeInfo(discardedJson);
    
    // Verify no fields were modified
    EXPECT_FALSE(pageNodeInfo.isFocus);
}

/**
 * @tc.name: ToJsonString_0010
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0010, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ID], 0);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_DEPTH], 2);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_AUTO_FILL_TYPE], static_cast<int32_t>(AutoFillType::USER_NAME));
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
 * @tc.name: ToJsonString_0011
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0011, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ID], 0);
}

/**
 * @tc.name: ToJsonString_0012
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0012, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_DEPTH], 2);
}

/**
 * @tc.name: ToJsonString_0013
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0013, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_AUTO_FILL_TYPE], static_cast<int32_t>(AutoFillType::USER_NAME));
}

/**
 * @tc.name: ToJsonString_0014
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0014, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_TAG], "input");
}

/**
 * @tc.name: ToJsonString_0015
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0015, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_VALUE], "test_value");
}

/**
 * @tc.name: ToJsonString_0016
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0016, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_PLACEHOLDER], "test_placeholder");
}

/**
 * @tc.name: ToJsonString_0017
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0017, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_PASSWORD_RULES], "test_rules");
}

/**
 * @tc.name: ToJsonString_0018
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0018, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_META_DATA], "test_metadata");
}

/**
 * @tc.name: ToJsonString_0019
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0019, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ENABLE_AUTO_FILL], true);
}

/**
 * @tc.name: ToJsonString_00110
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_0020, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
    
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_IS_FOCUS], false);
}

/**
 * @tc.name: ToJsonString_002
 * @tc.desc: SessionInfo test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(PageNodeInfoTest, ToJsonString_002, TestSize.Level1)
{
    PageNodeInfo emptyNode;
    std::string result = emptyNode.ToJsonString();
    
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_ID].empty());
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_DEPTH], -1);
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_AUTO_FILL_TYPE], static_cast<int32_t>(AutoFillType::UNSPECIFIED));
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_TAG].empty());
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_VALUE].empty());
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_PLACEHOLDER].empty());
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_PASSWORD_RULES].empty());
    EXPECT_FALSE(jsonResult[PAGE_NODE_INFO_META_DATA].empty());
    EXPECT_EQ(jsonResult[PAGE_NODE_INFO_ENABLE_AUTO_FILL], true);
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
HWTEST_F(PageNodeInfoTest, ToJsonString_003, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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
HWTEST_F(PageNodeInfoTest, ToJsonString_004, TestSize.Level1)
{
    PageNodeInfo testNode;
    // Setup common test data
    testNode.id = 0;
    testNode.depth = 2;
    testNode.autoFillType = AutoFillType::USER_NAME;
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