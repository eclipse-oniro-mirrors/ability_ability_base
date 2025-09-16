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
    "{\"abilityName\":\"\",\"bundleName\":\"\",\"isOtherAccount\":false,\"isUserSelected\":false,"
    "\"moduleName\":\"\",\"nodes\":[],\"pageRect\":\"{\\\"height\\\":0.0,\\\"left\\\":0.0,\\\"top\\\":0.0,"
    "\\\"width\\\":0.0}\",\"pageUrl\":\"\"}";

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

constexpr size_t NODES_SIZE_LIMIT = 20;
constexpr const char* VIEW_DATA_BUNDLE_NAME = "bundleName";
constexpr const char* VIEW_DATA_MODULE_NAME = "moduleName";
constexpr const char* VIEW_DATA_ABILITY_NAME = "abilityName";
constexpr const char* VIEW_DATA_PAGE_URL = "pageUrl";
constexpr const char* VIEW_DATA_NODES = "nodes";
constexpr const char* VIEW_DATA_PAGE_RECT = "pageRect";
constexpr const char* VIEW_DATA_USER_SELECTED = "isUserSelected";
constexpr const char* VIEW_DATA_OTHER_ACCOUNT = "isOtherAccount";
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

/**
 * @tc.name: FromJsonString_1001
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1001, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "bundleName": "com.example.bundle",
        "moduleName": "example_module",
        "abilityName": "MainAbility",
        "pageUrl": "pages/index",
        "isUserSelected": true,
        "isOtherAccount": false,
        "nodes": ["node1", "node2"],
        "pageRect": "rect_data"
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_EQ(viewData.bundleName, "com.example.bundle");
    EXPECT_EQ(viewData.moduleName, "example_module");
    EXPECT_EQ(viewData.abilityName, "MainAbility");
    EXPECT_EQ(viewData.pageUrl, "pages/index");
    EXPECT_TRUE(viewData.isUserSelected);
    EXPECT_FALSE(viewData.isOtherAccount);
    EXPECT_EQ(viewData.nodes.size(), 2);
}

/**
 * @tc.name: FromJsonString_1002
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1002, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = "";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_TRUE(viewData.bundleName.empty());
    EXPECT_TRUE(viewData.moduleName.empty());
    EXPECT_TRUE(viewData.abilityName.empty());
    EXPECT_TRUE(viewData.pageUrl.empty());
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_FALSE(viewData.isOtherAccount);
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1003
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1003, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = "{invalid json string";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_TRUE(viewData.bundleName.empty());
    EXPECT_TRUE(viewData.moduleName.empty());
    EXPECT_TRUE(viewData.abilityName.empty());
    EXPECT_TRUE(viewData.pageUrl.empty());
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_FALSE(viewData.isOtherAccount);
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1004
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1004, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "bundleName": "partial.bundle",
        "moduleName": "partial_module"
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_EQ(viewData.bundleName, "partial.bundle");
    EXPECT_EQ(viewData.moduleName, "partial_module");
    EXPECT_TRUE(viewData.abilityName.empty());
    EXPECT_TRUE(viewData.pageUrl.empty());
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_FALSE(viewData.isOtherAccount);
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1005
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1005, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "bundleName": 12345,
        "isUserSelected": "not_a_boolean",
        "nodes": "not_an_array"
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_TRUE(viewData.bundleName.empty());
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1006
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1006, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "nodes": ["node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", "node9", "node10", "node11"]
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_EQ(viewData.nodes.size(), 11);
}

/**
 * @tc.name: FromJsonString_1007
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1007, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "isUserSelected": false,
        "isOtherAccount": true
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_TRUE(viewData.isOtherAccount);
}

/**
 * @tc.name: FromJsonString_1008
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1008, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "nodes": []
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1009
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1009, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = "{}";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_TRUE(viewData.bundleName.empty());
    EXPECT_TRUE(viewData.moduleName.empty());
    EXPECT_TRUE(viewData.abilityName.empty());
    EXPECT_TRUE(viewData.pageUrl.empty());
    EXPECT_FALSE(viewData.isUserSelected);
    EXPECT_FALSE(viewData.isOtherAccount);
    EXPECT_TRUE(viewData.nodes.empty());
}

/**
 * @tc.name: FromJsonString_1010
 * @tc.desc: ViewData test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, FromJsonString_1010, TestSize.Level1)
{
    ViewData viewData;
    std::string jsonStr = R"({
        "bundleName": "com.complex.app",
        "moduleName": "complex_module",
        "abilityName": "ComplexAbility",
        "pageUrl": "pages/complex/index",
        "isUserSelected": true,
        "isOtherAccount": true,
        "nodes": ["node1", "node2", "node3"],
        "pageRect": "complex_rect_data",
        "extraField": "should_be_ignored"
    })";
    
    viewData.FromJsonString(jsonStr);
    
    EXPECT_EQ(viewData.bundleName, "com.complex.app");
    EXPECT_EQ(viewData.moduleName, "complex_module");
    EXPECT_EQ(viewData.abilityName, "ComplexAbility");
    EXPECT_EQ(viewData.pageUrl, "pages/complex/index");
    EXPECT_TRUE(viewData.isUserSelected);
    EXPECT_TRUE(viewData.isOtherAccount);
    EXPECT_EQ(viewData.nodes.size(), 3);
}

/**
 * @tc.name: ToJsonString_1001
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1001, TestSize.Level1)
{
    ViewData viewData;
    std::string result = viewData.ToJsonString();
    
    nlohmann::json expectedJson = {
        {VIEW_DATA_BUNDLE_NAME, ""},
        {VIEW_DATA_MODULE_NAME, ""},
        {VIEW_DATA_ABILITY_NAME, ""},
        {VIEW_DATA_PAGE_URL, ""},
        {VIEW_DATA_NODES, nlohmann::json::array()},
        {VIEW_DATA_USER_SELECTED, false},
        {VIEW_DATA_OTHER_ACCOUNT, false},
        {VIEW_DATA_PAGE_RECT, ""}
    };
    
    EXPECT_NE(result, expectedJson.dump());
}

/**
 * @tc.name: ToJsonString_1002
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1002, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.bundle";
    viewData.moduleName = "module1";
    viewData.abilityName = "MainAbility";
    viewData.pageUrl = "pages/index";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = false;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node1;
    node1.id = 1;
    viewData.nodes.push_back(node1);
    
    PageNodeInfo node2;
    node2.id = 2;
    viewData.nodes.push_back(node2);
    
    std::string result = viewData.ToJsonString();
    
    nlohmann::json expectedNodes = nlohmann::json::array();
    expectedNodes.push_back(node1.ToJsonString());
    expectedNodes.push_back(node2.ToJsonString());
    
    nlohmann::json expectedJson = {
        {VIEW_DATA_BUNDLE_NAME, "com.example.bundle"},
        {VIEW_DATA_MODULE_NAME, "module1"},
        {VIEW_DATA_ABILITY_NAME, "MainAbility"},
        {VIEW_DATA_PAGE_URL, "pages/index"},
        {VIEW_DATA_NODES, expectedNodes},
        {VIEW_DATA_USER_SELECTED, true},
        {VIEW_DATA_OTHER_ACCOUNT, false},
        {VIEW_DATA_PAGE_RECT, rect.ToJsonString()}
    };
    
    EXPECT_EQ(result, expectedJson.dump());
}

/**
 * @tc.name: ToJsonString_1003
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1003, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.bundle";
    
    // Create more nodes than the limit
    const size_t EXCESS_NODES = NODES_SIZE_LIMIT + 5;
    for (size_t i = 0; i < EXCESS_NODES; i++) {
        PageNodeInfo node;
        node.id = 1;
        viewData.nodes.push_back(node);
    }
    
    std::string result = viewData.ToJsonString();
    
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    auto nodesArray = jsonResult[VIEW_DATA_NODES];
    
    EXPECT_EQ(nodesArray.size(), NODES_SIZE_LIMIT);
    
    // Verify the first NODES_SIZE_LIMIT nodes are included
    for (size_t i = 0; i < NODES_SIZE_LIMIT; i++) {
        PageNodeInfo expectedNode;
        expectedNode.id = 1;
        EXPECT_EQ(nodesArray[i], expectedNode.ToJsonString());
    }
}

/**
 * @tc.name: ToJsonString_1004
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1004, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[VIEW_DATA_USER_SELECTED], true);
    EXPECT_EQ(jsonResult[VIEW_DATA_OTHER_ACCOUNT], true);
    EXPECT_EQ(jsonResult[VIEW_DATA_PAGE_RECT], rect.ToJsonString());
    EXPECT_EQ(jsonResult[VIEW_DATA_NODES][0], node.ToJsonString());
}

/**
 * @tc.name: ToJsonString_1005
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1005, TestSize.Level1)
{
    ViewData viewData;
    viewData.isUserSelected = true;
    viewData.isOtherAccount = false;
    
    std::string result = viewData.ToJsonString();
    
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_TRUE(jsonResult[VIEW_DATA_USER_SELECTED].get<bool>());
    EXPECT_FALSE(jsonResult[VIEW_DATA_OTHER_ACCOUNT].get<bool>());
}

/**
 * @tc.name: ToJsonString_1006
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1006, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[VIEW_DATA_BUNDLE_NAME], "com.example.\"special\"");
    EXPECT_EQ(jsonResult[VIEW_DATA_MODULE_NAME], "module\nwith\nnewlines");
    EXPECT_EQ(jsonResult[VIEW_DATA_ABILITY_NAME], "Ability\twith\ttabs");
    EXPECT_EQ(jsonResult[VIEW_DATA_PAGE_URL], "pages/\\escaped");
}

/**
 * @tc.name: ToJsonString_1007
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1007, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[VIEW_DATA_BUNDLE_NAME], "com.example.\"special\"");
}

/**
 * @tc.name: ToJsonString_1008
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1008, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[VIEW_DATA_MODULE_NAME], "module\nwith\nnewlines");
}

/**
 * @tc.name: ToJsonString_1009
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1009, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_EQ(jsonResult[VIEW_DATA_ABILITY_NAME], "Ability\twith\ttabs");
}

/**
 * @tc.name: ToJsonString_1010
 * @tc.desc: ViewData test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(ViewDataTest, ToJsonString_1010, TestSize.Level1)
{
    ViewData viewData;
    viewData.bundleName = "com.example.\"special\"";
    viewData.moduleName = "module\nwith\nnewlines";
    viewData.abilityName = "Ability\twith\ttabs";
    viewData.pageUrl = "pages/\\escaped";
    viewData.isUserSelected = true;
    viewData.isOtherAccount = true;
    
    Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = 100.0f;
    rect.height = 200.0f;
    viewData.pageRect = rect;
    
    PageNodeInfo node;
    node.id = 1;
    viewData.nodes.push_back(node);
    
    std::string result = viewData.ToJsonString();
    
    // The JSON library should properly escape all special characters
    nlohmann::json jsonResult = nlohmann::json::parse(result);
    
    EXPECT_NE(jsonResult[VIEW_DATA_ABILITY_NAME], "Ability1\twith\ttabs");
    EXPECT_EQ(jsonResult[VIEW_DATA_PAGE_URL], "pages/\\escaped");
}
}  // namespace AbilityBase
}  // namespace OHOS
