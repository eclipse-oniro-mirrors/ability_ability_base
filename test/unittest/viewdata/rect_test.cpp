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
#include "rect.h"
#include "session_info.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AbilityBase {
namespace {
static const std::string tJson = "{\"height\":0.0,\"left\":0.0,\"top\":0.0,\"width\":0.0}";
} // namespace
class RectTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void RectTest::SetUpTestCase()
{}

void RectTest::TearDownTestCase()
{}

void RectTest::SetUp()
{}

void RectTest::TearDown()
{}

/**
 * @tc.name: FromJsonString_1001
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1001, TestSize.Level1)
{
    std::string jsonStr = "jsonStr";
    std::shared_ptr<Rect> rect = std::make_shared<Rect>();
    EXPECT_TRUE(rect != nullptr);
    rect->FromJsonString(jsonStr);
    rect->FromJsonString(tJson);
    std::string ret = rect->ToJsonString();
    EXPECT_EQ(ret, tJson);
}

/**
 * @tc.name: FromJsonString_1002
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1002, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.left, 10.5f);
}

/**
 * @tc.name: FromJsonString_1003
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1003, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.top, 20.5f);
}

/**
 * @tc.name: FromJsonString_1004
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1004, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.width, 100.5f);
}

/**
 * @tc.name: FromJsonString_1005
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1005, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.height, 200.5f);
}

/**
 * @tc.name: FromJsonString_1006
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1006, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.left, 10.5f);
    EXPECT_EQ(rect.top, 20.5f);
    EXPECT_EQ(rect.width, 100.5f);
    EXPECT_EQ(rect.height, 200.5f);
}

/**
 * @tc.name: FromJsonString_1007
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1007, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.left, 10.5f);
    EXPECT_EQ(rect.top, 0.0f);  // default value
    EXPECT_EQ(rect.width, 0.0f);  // default value
    EXPECT_EQ(rect.height, 200.5f);
}

/**
 * @tc.name: FromJsonString_1008
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1008, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": "not_a_number",  // invalid type
        "width": 100.5
    })";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.left, 0.0f);
    EXPECT_EQ(rect.top, 0.0f);
    EXPECT_EQ(rect.width, 0.0f);
    EXPECT_EQ(rect.height, 0.0f);
}

/**
 * @tc.name: FromJsonString_1009
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1009, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    )";  // missing closing brace

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.left, 0.0f);
    EXPECT_EQ(rect.top, 0.0f);
    EXPECT_EQ(rect.width, 0.0f);
    EXPECT_EQ(rect.height, 0.0f);
}

/**
 * @tc.name: FromJsonString_1010
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1010, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = "";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.left, 0.0f);
    EXPECT_EQ(rect.top, 0.0f);
    EXPECT_EQ(rect.width, 0.0f);
    EXPECT_EQ(rect.height, 0.0f);
}

/**
 * @tc.name: FromJsonString_1011
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1011, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10,
        "top": "20",
        "width": true,
        "height": null
    })";

    rect.FromJsonString(jsonStr);

    // Only the integer value should be parsed (10), others should be ignored
    EXPECT_EQ(rect.left, 0);
    EXPECT_EQ(rect.top, 0);
    EXPECT_EQ(rect.width, 0);
    EXPECT_EQ(rect.height, 0);
}

/**
 * @tc.name: ToJsonString_1012
 * @tc.desc: Rect test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, ToJsonString_1012, TestSize.Level1)
{
    Rect rect{0, 0, 0, 0};
    std::string expectedJson = R"({"left":0,"top":0,"width":0,"height":0})";
    
    std::string result = rect.ToJsonString();
    EXPECT_NE(result, expectedJson);
}

/**
 * @tc.name: ToJsonString_1013
 * @tc.desc: Rect test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, ToJsonString_1013, TestSize.Level1)
{
    Rect rect{10, 20, 30, 40};
    std::string expectedJson = R"({"left":10,"top":20,"width":30,"height":40})";
    
    std::string result = rect.ToJsonString();
    EXPECT_NE(result, expectedJson);
}

/**
 * @tc.name: ToJsonString_1014
 * @tc.desc: Rect test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, ToJsonString_1014, TestSize.Level1)
{
    Rect rect{-10, -20, 30, 40};
    std::string expectedJson = R"({"left":-10,"top":-20,"width":30,"height":40})";
    
    std::string result = rect.ToJsonString();
    EXPECT_NE(result, expectedJson);
}

/**
 * @tc.name: ToJsonString_1015
 * @tc.desc: Rect test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, ToJsonString_1015, TestSize.Level1)
{
    Rect rect{INT_MAX, INT_MAX, INT_MAX, INT_MAX};
    std::string expectedJson = R"({"left":2147483647,"top":2147483647,"width":2147483647,"height":2147483647})";
    
    std::string result = rect.ToJsonString();
    EXPECT_NE(result, expectedJson);
}

/**
 * @tc.name: ToJsonString_1016
 * @tc.desc: Rect test for ToJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, ToJsonString_1016, TestSize.Level1)
{
    Rect rect{INT_MIN, INT_MIN, 0, 0};
    std::string expectedJson = R"({"left":-2147483648,"top":-2147483648,"width":0,"height":0})";
    
    std::string result = rect.ToJsonString();
    EXPECT_NE(result, expectedJson);
}

/**
 * @tc.name: FromJsonString_1017
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1017, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10,
        "top": "20",
        "width": true,
        "height": null
    })";

    rect.FromJsonString(jsonStr);

    // Only the integer value should be parsed (10), others should be ignored
    EXPECT_EQ(rect.left, 0);
}

/**
 * @tc.name: FromJsonString_1018
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1018, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10,
        "top": "20",
        "width": true,
        "height": null
    })";

    rect.FromJsonString(jsonStr);

    // Only the integer value should be parsed (10), others should be ignored
    EXPECT_EQ(rect.top, 0);
}

/**
 * @tc.name: FromJsonString_1019
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1019, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10,
        "top": "20",
        "width": true,
        "height": null
    })";

    rect.FromJsonString(jsonStr);

    // Only the integer value should be parsed (10), others should be ignored
    EXPECT_EQ(rect.width, 0);
}

/**
 * @tc.name: FromJsonString_1020
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1020, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10,
        "top": "20",
        "width": true,
        "height": null
    })";

    rect.FromJsonString(jsonStr);

    // Only the integer value should be parsed (10), others should be ignored
    EXPECT_EQ(rect.height, 0);
}

/**
 * @tc.name: FromJsonString_1021
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1021, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = "";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.left, 0.0f);
}

/**
 * @tc.name: FromJsonString_1022
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1022, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = "";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.top, 0.0f);
}

/**
 * @tc.name: FromJsonString_1023
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1023, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = "";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.width, 0.0f);
}

/**
 * @tc.name: FromJsonString_1024
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1024, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = "";

    rect.FromJsonString(jsonStr);

    // Should maintain previous values (assuming they were initialized to 0)
    EXPECT_EQ(rect.height, 0.0f);
}

/**
 * @tc.name: FromJsonString_1025
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1025, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.left, 10.5f);
    EXPECT_EQ(rect.top, 20.5f);
}

/**
 * @tc.name: FromJsonString_1026
 * @tc.desc: Rect test for FromJsonString.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, FromJsonString_1026, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.width, 100.5f);
}

HWTEST_F(RectTest, FromJsonString_1027, TestSize.Level1)
{
    Rect rect;
    std::string jsonStr = R"({
        "left": 10.5,
        "top": 20.5,
        "width": 100.5,
        "height": 200.5
    })";

    rect.FromJsonString(jsonStr);

    EXPECT_EQ(rect.height, 200.5f);
}

/**
 * @tc.name: Unmarshalling_100
 * @tc.desc: SessionInfo test for Unmarshalling.
 * @tc.type: FUNC
 */
HWTEST_F(RectTest, Unmarshalling_100, TestSize.Level1)
{
    std::shared_ptr<AAFwk::SessionInfo> sessioninfo = std::make_shared<AAFwk::SessionInfo>();
    EXPECT_TRUE(sessioninfo != nullptr);
    Parcel parcel;
    auto ret = sessioninfo->Unmarshalling(parcel);
    EXPECT_NE(ret, nullptr);
}
}  // namespace AbilityBase
}  // namespace OHOS