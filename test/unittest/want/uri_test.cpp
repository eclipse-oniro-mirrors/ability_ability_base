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
#include "uri.h"
#undef private
#undef protected
using namespace OHOS;
using namespace testing::ext;
namespace OHOS {
    namespace {
        const string EMPTY = "";
        const int PORT_NONE = -1;
    }
class UriTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
    std::shared_ptr<Uri> uri_ = nullptr;
};

void UriTest::SetUpTestCase(void)
{}

void UriTest::TearDownTestCase(void)
{}

void UriTest::SetUp(void)
{
    uri_ = std::make_shared<Uri>(" ");
}

void UriTest::TearDown(void)
{}

/**
 * @tc.number: Uri_GetSchemeSpecificPart_0100
 * @tc.name: GetSchemeSpecificPart
 * @tc.desc: Verify the function when the input string get string specific part.
 */
HWTEST_F(UriTest, Uri_GetSchemeSpecificPart_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0100 start";
    auto result = uri_->GetSchemeSpecificPart();
    EXPECT_EQ(result, " ");
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0100 end";
}

/**
 * @tc.number: Uri_GetSchemeSpecificPart_0200
 * @tc.name: GetSchemeSpecificPart
 * @tc.desc: Verify the function when the input string get string specific part.
 */
HWTEST_F(UriTest, Uri_GetSchemeSpecificPart_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0200 start";
    uri_->uriString_ = "abc";
    auto result = uri_->GetSchemeSpecificPart();
    EXPECT_EQ(result, "abc");
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0200 end";
}

/**
 * @tc.number: Uri_GetAuthority_0100
 * @tc.name: GetAuthority
 * @tc.desc: Verify the function to get authority.
 */
HWTEST_F(UriTest, Uri_GetAuthority_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0100 start";
    uri_->uriString_ = "abc";
    auto result = uri_->GetAuthority();
    EXPECT_TRUE(result == "");
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0100 end";
}

/**
 * @tc.number: Uri_GetAuthority_0200
 * @tc.name: GetAuthority
 * @tc.desc: Verify the function to get authority.
 */
HWTEST_F(UriTest, Uri_GetAuthority_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0200 start";
    uri_->uriString_ = "abc";
    uri_->cachedSsi_ = 1;
    auto result = uri_->GetAuthority();
    EXPECT_TRUE(result == "");
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0200 end";
}

/**
 * @tc.number: Uri_GetAuthority_0300
 * @tc.name: GetAuthority
 * @tc.desc: Verify the function to get authority.
 */
HWTEST_F(UriTest, Uri_GetAuthority_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0200 start";
    uri_->uriString_ = "://abc";
    auto result = uri_->GetAuthority();
    EXPECT_EQ(result, "abc");
    GTEST_LOG_(INFO) << "Uri_GetAuthority_0200 end";
}

/**
 * @tc.number: Uri_GetHost_0100
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input authority get host.
 */
HWTEST_F(UriTest, Uri_GetHost_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetHost_0100 start";
    uri_->uriString_ = "abc";
    uri_->authority_ = "abcd";
    auto result = uri_->GetHost();
    EXPECT_EQ("abcd", result);
    GTEST_LOG_(INFO) << "Uri_GetHost_0100 end";
}

/**
 * @tc.number: Uri_GetHost_0200
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input authority get host.
 */
HWTEST_F(UriTest, Uri_GetHost_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetHost_0200 start";
    auto result = uri_->GetAuthority();
    EXPECT_TRUE(result == "");
    GTEST_LOG_(INFO) << "Uri_GetHost_0200 end";
}

/**
 * @tc.number: Uri_GetPort_0100
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input port get port.
 */
HWTEST_F(UriTest, Uri_GetPort_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetPort_0100 start";
    uri_->uriString_ = "abc";
    uri_->port_ = -1;
    auto result = uri_->GetPort();
    EXPECT_EQ(uri_->port_, result);
    GTEST_LOG_(INFO) << "Uri_GetPort_0100 end";
}

/**
 * @tc.number: Uri_GetPort_0200
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input port get port..
 */
HWTEST_F(UriTest, Uri_GetPort_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetPort_0200 start";
    auto result = uri_->GetPort();
    EXPECT_TRUE(result == -1);
    GTEST_LOG_(INFO) << "Uri_GetPort_0200 end";
}

/**
 * @tc.number: Uri_GetPort_0300
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input port get port.
 */
HWTEST_F(UriTest, Uri_GetPort_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetPort_0300 start";
    uri_->uriString_ = "://:567";
    auto result = uri_->GetPort();
    EXPECT_EQ(567, result);
    GTEST_LOG_(INFO) << "Uri_GetPort_0300 end";
}

/**
 * @tc.number: Uri_GetPort_0400
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input port get port.
 */
HWTEST_F(UriTest, Uri_GetPort_0400, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetPort_0400 start";
    uri_->uriString_ = ":abc";
    uri_->port_ = 5;
    auto result = uri_->GetPort();
    EXPECT_EQ(5, result);
    GTEST_LOG_(INFO) << "Uri_GetPort_0400 end";
}

/**
 * @tc.number: Uri_GetUserInfo_0100
 * @tc.name: GetUserInfo
 * @tc.desc: Verify the function when the input userInfo get userInfo.
 */
HWTEST_F(UriTest, Uri_GetUserInfo_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetUserInfo_0100 start";
    uri_->uriString_ = "abc";
    auto result = uri_->GetUserInfo();
    EXPECT_TRUE(result == "");
    GTEST_LOG_(INFO) << "Uri_GetUserInfo_0100 end";
}

/**
 * @tc.number: Uri_GetUserInfo_0200
 * @tc.name: GetUserInfo
 * @tc.desc: Verify the function when the input userInfo get userInfo.
 */
HWTEST_F(UriTest, Uri_GetUserInfo_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetUserInfo_0200 start";
    uri_->uriString_ = "abcde@";
    uri_->userInfo_ = "abc";
    auto result = uri_->GetUserInfo();
    EXPECT_EQ(result, "abc");
    GTEST_LOG_(INFO) << "Uri_GetUserInfo_0200 end";
}

/**
 * @tc.number: Uri_GetFragment_0100
 * @tc.name: GetFragment
 * @tc.desc: Verify the function to get fragment.
 */
HWTEST_F(UriTest, Uri_GetFragment_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetFragment_0100 start";
    uri_->uriString_ = "abc";
    auto result = uri_->GetFragment();
    EXPECT_TRUE(result == "");
    GTEST_LOG_(INFO) << "Uri_GetFragment_0100 end";
}

/**
 * @tc.number: Uri_IsHierarchical_0100
 * @tc.name: IsHierarchical
 * @tc.desc: Verify the function is hierarchical.
 */
HWTEST_F(UriTest, Uri_IsHierarchical_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0100 start";
    uri_->uriString_ = "abc";
    uri_->cachedSsi_ = std::string::npos;
    auto result = uri_->IsHierarchical();
    EXPECT_TRUE(result);
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0100 end";
}

/**
 * @tc.number: Uri_IsHierarchical_0200
 * @tc.name: IsHierarchical
 * @tc.desc: Verify the function is hierarchical.
 */
HWTEST_F(UriTest, Uri_IsHierarchical_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0200 start";
    uri_->uriString_ = ":abc";
    auto result = uri_->IsHierarchical();
    EXPECT_FALSE(result);
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0200 end";
}

/**
 * @tc.number: Uri_IsOpaque_0100
 * @tc.name: IsOpaque
 * @tc.desc: Verify the function is opaque.
 */
HWTEST_F(UriTest, Uri_IsOpaque_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsOpaque_0100 start";
    auto result = uri_->IsOpaque();
    EXPECT_FALSE(result);
    GTEST_LOG_(INFO) << "Uri_IsOpaque_0100 end";
}

/**
 * @tc.number: Uri_IsOpaque_0200
 * @tc.name: IsOpaque
 * @tc.desc: Verify the function is opaque.
 */
HWTEST_F(UriTest, Uri_IsOpaque_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsOpaque_0200 start";
    uri_->uriString_ = "abc";
    uri_->cachedSsi_ = 2;
    auto result = uri_->IsOpaque();
    EXPECT_TRUE(result);
    GTEST_LOG_(INFO) << "Uri_IsOpaque_0200 end";
}

/**
 * @tc.number: Uri_IsAbsolute_0100
 * @tc.name: IsAbsolute
 * @tc.desc: Verify the function is absolute.
 */
HWTEST_F(UriTest, Uri_IsAbsolute_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsAbsolute_0100 start";
    uri_->uriString_ = "ab:c";
    auto result = uri_->IsAbsolute();
    EXPECT_TRUE(result);
    GTEST_LOG_(INFO) << "Uri_IsAbsolute_0100 end";
}

/**
 * @tc.number: Uri_IsAbsolute_0200
 * @tc.name: IsAbsolute
 * @tc.desc: Verify the function is absolute.
 */
HWTEST_F(UriTest, Uri_IsAbsolute_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsAbsolute_0200 start";
    auto result = uri_->IsAbsolute();
    EXPECT_TRUE(!result);
    GTEST_LOG_(INFO) << "Uri_IsAbsolute_0200 end";
}

/**
 * @tc.number: Uri_IsRelative_0100
 * @tc.name: IsRelative
 * @tc.desc: Verify the function is relative.
 */
HWTEST_F(UriTest, Uri_IsRelative_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsRelative_0100 start";
    uri_->uriString_ = "abc";
    uri_->cachedSsi_ = std::string::npos;
    auto result = uri_->IsRelative();
    EXPECT_TRUE(result);
    GTEST_LOG_(INFO) << "Uri_IsRelative_0100 end";
}

/**
 * @tc.number: Uri_IsRelative_0200
 * @tc.name: IsRelative
 * @tc.desc: Verify the function is relative.
 */
HWTEST_F(UriTest, Uri_IsRelative_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsRelative_0200 start";
    uri_->uriString_ = "a:bc";
    auto result = uri_->IsRelative();
    EXPECT_TRUE(!result);
    GTEST_LOG_(INFO) << "Uri_IsRelative_0200 end";
}

/**
 * @tc.number: Uri_Equals_0100
 * @tc.name: Equals
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(UriTest, Uri_Equals_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_Equals_0100 start";
    Parcel parcel;
    auto other = uri_->Unmarshalling(parcel);
    auto result = uri_->Equals(*other);
    EXPECT_FALSE(result);
    GTEST_LOG_(INFO) << "Uri_Equals_0100 end";
}

/**
 * @tc.number: Uri_CompareTo_0100
 * @tc.name: Equals
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(UriTest, Uri_CompareTo_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_CompareTo_0100 start";
    Parcel parcel;
    auto other = uri_->Unmarshalling(parcel);
    auto result = uri_->CompareTo(*other);
    EXPECT_EQ(result, 1);
    GTEST_LOG_(INFO) << "Uri_CompareTo_0100 end";
}

/**
 * @tc.number: Uri_GetScheme_0100
 * @tc.name: GetScheme
 * @tc.desc: Verify the function when the uriString_ is not empty.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_GetScheme_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetScheme_0100 start";
    string uriString = "this is uriString";
    uri_ = std::make_shared<Uri>(uriString);
    string result = uri_->GetScheme();
    string result1 = uri_->ParseScheme();
    EXPECT_EQ(result, result1);
    GTEST_LOG_(INFO) << "Uri_GetScheme_0100 end";
}

/**
 * @tc.number: Uri_GetSchemeSpecificPart_0300
 * @tc.name: GetSchemeSpecificPart
 * @tc.desc: Verify the function when the uriString_ is empty.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_GetSchemeSpecificPart_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0300 start";
    auto result = uri_->GetSchemeSpecificPart();
    auto result1 = uri_->GetAuthority();
    auto result2 = uri_->GetUserInfo();
    auto result5 = uri_->GetHost();
    auto result6 = uri_->ParseHost();
    auto result7 = uri_->GetPort();
    auto result8 = uri_->GetQuery();
    auto result9 = uri_->GetPath();
    auto result10 = uri_->GetFragment();
    auto result12 = uri_->IsOpaque();
    auto result13 = uri_->IsAbsolute();
    EXPECT_EQ(result, " ");
    EXPECT_EQ(result1, EMPTY);
    EXPECT_EQ(result2, EMPTY);
    EXPECT_EQ(result5, EMPTY);
    EXPECT_EQ(result6, EMPTY);
    EXPECT_EQ(result7, PORT_NONE);
    EXPECT_EQ(result8, EMPTY);
    EXPECT_EQ(result9, " ");
    EXPECT_EQ(result10, EMPTY);
    EXPECT_EQ(result12, false);
    EXPECT_EQ(result13, false);
    GTEST_LOG_(INFO) << "Uri_GetSchemeSpecificPart_0300 end";
}

/**
 * @tc.number: Uri_IsHierarchical_0300
 * @tc.name: IsHierarchical
 * @tc.desc: Verify the function when the uriString_ is empty.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_IsHierarchical_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0300 start";
    uri_->uriString_ = "";
    auto result1 = uri_->IsHierarchical();
    auto result2 = uri_->IsRelative();
    EXPECT_EQ(result1, false);
    EXPECT_EQ(result2, false);
    GTEST_LOG_(INFO) << "Uri_IsHierarchical_0300 end";
}

/**
 * @tc.number: Uri_ParseUserInfo_0100
 * @tc.name: ParseUserInfo
 * @tc.desc: Verify the function when the authority is not empty.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_ParseUserInfo_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_ParseUserInfo_0100 start";
    string uriString = "this is uriString";
    uri_ = std::make_shared<Uri>(uriString);
    auto result = uri_->GetAuthority();
    auto result1 = uri_->ParseUserInfo();
    auto result2 = uri_->ParseAuthority();
    auto result3 = uri_->ParsePort();
    EXPECT_EQ(result, result2);
    EXPECT_EQ(result3, PORT_NONE);
    GTEST_LOG_(INFO) << "Uri_ParseUserInfo_0100 end";
}

/**
 * @tc.number: Uri_ParseQuery_0100
 * @tc.name: ParseQuery
 * @tc.desc: Verify the function when the fsi < qsi.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_ParseQuery_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_ParseQuery_0100 start";
    string uriString = "this is uriString";
    uri_ = std::make_shared<Uri>(uriString);
    uri_->cachedSsi_ = 1;
    uri_->cachedFsi_ = 2;
    auto result = uri_->ParseQuery();
    EXPECT_EQ(result, EMPTY);
    GTEST_LOG_(INFO) << "Uri_ParseQuery_0100 end";
}

/**
 * @tc.number: Uri_ParseQuery_0200
 * @tc.name: ParseQuery
 * @tc.desc: Verify the function when the fsi > qsi.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_ParseQuery_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_ParseQuery_0200 start";
    string uriString = "this is uriString";
    uri_ = std::make_shared<Uri>(uriString);
    uri_->cachedSsi_ = 3;
    uri_->cachedFsi_ = 2;
    auto result = uri_->ParseQuery();
    EXPECT_EQ(result, EMPTY);
    GTEST_LOG_(INFO) << "Uri_ParseQuery_0200 end";
}

/**
 * @tc.number: Uri_ParsePath_0100
 * @tc.name: ParsePath
 * @tc.desc: Verify the function when the fsi = uriString.length.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_ParsePath_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_ParsePath_0100 start";
    string uriString = "uriString";
    uri_ = std::make_shared<Uri>(uriString);
    uri_->cachedSsi_ = 8;
    auto result = uri_->ParsePath();
    EXPECT_EQ(result, EMPTY);
    GTEST_LOG_(INFO) << "Uri_ParsePath_0100 end";
}

/**
 * @tc.number: Uri_ParsePath_0200
 * @tc.name: ParsePath
 * @tc.desc: Verify the function.
 * @tc.require: issueI6415N
 */
HWTEST_F(UriTest, Uri_ParsePath_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "Uri_ParsePath_0200 start";
    string uriString = "uriString";
    uri_ = std::make_shared<Uri>(uriString);
    uri_->cachedSsi_ = 2;
    auto result = uri_->ParsePath();
    EXPECT_EQ(result, EMPTY);
    GTEST_LOG_(INFO) << "Uri_ParsePath_0200 end";
}
}