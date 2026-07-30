/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <memory>
#include <string>
#include <vector>

#define private public
#include "uri.h"
#undef private

using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
namespace {
    const std::string EMPTY = "";
    const int PORT_NONE = -1;
    const std::string FULL_URI = "http://user@host:8080/path/sub?q=1#frag";
    const std::string SCHEME = "http";
    const std::string SSP = "//user@host:8080/path/sub?q=1";
    const std::string AUTHORITY = "user@host:8080";
    const std::string USER_INFO = "user";
    const std::string HOST = "host";
    const int PORT = 8080;
    const std::string QUERY = "q=1";
    const std::string PATH = "/path/sub";
    const std::string FRAGMENT = "frag";
}

class UriTddTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void UriTddTest::SetUpTestCase()
{
}

void UriTddTest::TearDownTestCase()
{
}

void UriTddTest::SetUp()
{
}

void UriTddTest::TearDown()
{
}

HWTEST_F(UriTddTest, UriTdd_ConstructEmpty_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
    EXPECT_EQ(u->GetScheme(), EMPTY);
    EXPECT_EQ(u->GetSchemeSpecificPart(), EMPTY);
    EXPECT_EQ(u->GetAuthority(), EMPTY);
    EXPECT_EQ(u->GetHost(), EMPTY);
    EXPECT_EQ(u->GetPort(), PORT_NONE);
    EXPECT_EQ(u->GetUserInfo(), EMPTY);
    EXPECT_EQ(u->GetQuery(), EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_EQ(u->GetFragment(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructWhitespace_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(" ");
    EXPECT_EQ(u->GetUriStringRef(), " ");
    EXPECT_EQ(u->GetScheme(), EMPTY);
    EXPECT_EQ(u->GetSchemeSpecificPart(), " ");
    EXPECT_FALSE(u->IsAbsolute());
    EXPECT_TRUE(u->IsRelative());
    EXPECT_TRUE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructSchemeOnly_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:");
    EXPECT_EQ(u->GetUriStringRef(), "http:");
    EXPECT_EQ(u->GetScheme(), SCHEME);
    EXPECT_EQ(u->GetSchemeSpecificPart(), EMPTY);
    EXPECT_EQ(u->GetAuthority(), EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_TRUE(u->IsAbsolute());
    EXPECT_FALSE(u->IsRelative());
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructSchemeUpper_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("HTTP:");
    EXPECT_EQ(u->GetUriStringRef(), "HTTP:");
    EXPECT_EQ(u->GetScheme(), "HTTP");
    EXPECT_TRUE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_ConstructSchemeWithPipe_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("ab|c:");
    EXPECT_EQ(u->GetUriStringRef(), "ab|c:");
    EXPECT_EQ(u->GetScheme(), "ab|c");
}

HWTEST_F(UriTddTest, UriTdd_ConstructSchemeWithDigits_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("a1+-.|:");
    EXPECT_EQ(u->GetUriStringRef(), "a1+-.|:");
    EXPECT_EQ(u->GetScheme(), "a1+-.|");
}

HWTEST_F(UriTddTest, UriTdd_ConstructInvalidFirstChar_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("1abc:");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
    EXPECT_EQ(u->GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructInvalidSubsequent_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("ab_c:");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
    EXPECT_EQ(u->GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructInvalidFirstChar_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("+abc:");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructInvalidFirstChar_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(".abc:");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructInvalidFirstChar_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("-abc:");
    EXPECT_EQ(u->GetUriStringRef(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ConstructRelative_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetUriStringRef(), "abc");
    EXPECT_EQ(u->GetScheme(), EMPTY);
    EXPECT_EQ(u->GetSchemeSpecificPart(), "abc");
    EXPECT_FALSE(u->IsAbsolute());
    EXPECT_TRUE(u->IsRelative());
    EXPECT_TRUE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructFullUri_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetUriStringRef(), FULL_URI);
    EXPECT_EQ(u->GetScheme(), SCHEME);
    EXPECT_EQ(u->GetSchemeSpecificPart(), SSP);
    EXPECT_EQ(u->GetAuthority(), AUTHORITY);
    EXPECT_EQ(u->GetUserInfo(), USER_INFO);
    EXPECT_EQ(u->GetHost(), HOST);
    EXPECT_EQ(u->GetPort(), PORT);
    EXPECT_EQ(u->GetQuery(), QUERY);
    EXPECT_EQ(u->GetPath(), PATH);
    EXPECT_EQ(u->GetFragment(), FRAGMENT);
    EXPECT_TRUE(u->IsAbsolute());
    EXPECT_FALSE(u->IsRelative());
    EXPECT_TRUE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructOpaque_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("mailto:foo@bar.com");
    EXPECT_EQ(u->GetScheme(), "mailto");
    EXPECT_EQ(u->GetSchemeSpecificPart(), "foo@bar.com");
    EXPECT_EQ(u->GetAuthority(), EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_TRUE(u->IsAbsolute());
    EXPECT_FALSE(u->IsRelative());
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructNonAscii_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("中文");
    Parcel parcel;
    EXPECT_EQ(u->GetUriStringRef(), "中文");
    EXPECT_EQ(u->GetScheme(), EMPTY);
    EXPECT_FALSE(u->Marshalling(parcel));
}

HWTEST_F(UriTddTest, UriTdd_CopyConstructor_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b(a);
    EXPECT_EQ(b.ToString(), FULL_URI);
    EXPECT_EQ(b.GetScheme(), SCHEME);
    EXPECT_EQ(b.GetAuthority(), AUTHORITY);
    EXPECT_EQ(b.GetHost(), HOST);
    EXPECT_EQ(b.GetPort(), PORT);
}

HWTEST_F(UriTddTest, UriTdd_CopyConstructorCached_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    (void)a.GetScheme();
    (void)a.GetHost();
    (void)a.GetPort();
    (void)a.GetQuery();
    (void)a.GetFragment();
    Uri b(a);
    EXPECT_EQ(b.GetScheme(), SCHEME);
    EXPECT_EQ(b.GetHost(), HOST);
    EXPECT_EQ(b.GetPort(), PORT);
    EXPECT_EQ(b.GetQuery(), QUERY);
    EXPECT_EQ(b.GetFragment(), FRAGMENT);
}

HWTEST_F(UriTddTest, UriTdd_CopyConstructorFromEmpty_001, TestSize.Level1)
{
    Uri a(EMPTY);
    Uri b(a);
    EXPECT_EQ(b.ToString(), EMPTY);
    EXPECT_EQ(b.GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_CopyAssign_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b("abc");
    b = a;
    EXPECT_EQ(b.ToString(), FULL_URI);
    EXPECT_EQ(b.GetScheme(), SCHEME);
    EXPECT_EQ(b.GetHost(), HOST);
    EXPECT_EQ(b.GetPort(), PORT);
}

HWTEST_F(UriTddTest, UriTdd_CopyAssignSelf_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri& ref = a;
    a = ref;
    EXPECT_EQ(a.ToString(), FULL_URI);
    EXPECT_EQ(a.GetScheme(), SCHEME);
}

HWTEST_F(UriTddTest, UriTdd_CopyAssignFromEmpty_001, TestSize.Level1)
{
    Uri a(EMPTY);
    Uri b(FULL_URI);
    b = a;
    EXPECT_EQ(b.ToString(), EMPTY);
    EXPECT_EQ(b.GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_MoveConstructor_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b(std::move(a));
    EXPECT_EQ(b.ToString(), FULL_URI);
    EXPECT_EQ(b.GetScheme(), SCHEME);
    EXPECT_EQ(b.GetHost(), HOST);
    EXPECT_EQ(b.GetPort(), PORT);
}

HWTEST_F(UriTddTest, UriTdd_MoveAssign_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b("abc");
    b = std::move(a);
    EXPECT_EQ(b.ToString(), FULL_URI);
    EXPECT_EQ(b.GetScheme(), SCHEME);
    EXPECT_EQ(b.GetHost(), HOST);
}

HWTEST_F(UriTddTest, UriTdd_MoveAssignSelf_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri& ref = a;
    a = std::move(ref);
    EXPECT_EQ(a.ToString(), FULL_URI);
    EXPECT_EQ(a.GetScheme(), SCHEME);
}

HWTEST_F(UriTddTest, UriTdd_GetUriStringRef_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetUriStringRef(), FULL_URI);
}

HWTEST_F(UriTddTest, UriTdd_GetUriStringRef_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetUriStringRef(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_GetScheme_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetScheme(), SCHEME);
}

HWTEST_F(UriTddTest, UriTdd_GetScheme_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetScheme_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetScheme_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->scheme_ = "preset";
    EXPECT_EQ(u->GetScheme(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetScheme_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetScheme(), SCHEME);
    EXPECT_EQ(u->GetScheme(), SCHEME);
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPart_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetSchemeSpecificPart(), SSP);
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPart_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetSchemeSpecificPart(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPart_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetSchemeSpecificPart(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPart_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->ssp_ = "preset";
    EXPECT_EQ(u->GetSchemeSpecificPart(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPart_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:opaque#frag");
    EXPECT_EQ(u->GetSchemeSpecificPart(), "opaque");
    EXPECT_EQ(u->GetFragment(), "frag");
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetAuthority(), AUTHORITY);
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetAuthority(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetAuthority(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("://abc");
    EXPECT_EQ(u->GetAuthority(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:opaque");
    EXPECT_EQ(u->GetAuthority(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->authority_ = "preset";
    EXPECT_EQ(u->GetAuthority(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetAuthority_007, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/path?q=1#frag");
    EXPECT_EQ(u->GetAuthority(), "host");
}

HWTEST_F(UriTddTest, UriTdd_GetHost_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetHost(), HOST);
}

HWTEST_F(UriTddTest, UriTdd_GetHost_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetHost(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetHost_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetHost(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetHost_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host:8080");
    EXPECT_EQ(u->GetHost(), "host");
}

HWTEST_F(UriTddTest, UriTdd_GetHost_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://user@host");
    EXPECT_EQ(u->GetHost(), "host");
}

HWTEST_F(UriTddTest, UriTdd_GetHost_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->host_ = "preset";
    EXPECT_EQ(u->GetHost(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetHost_007, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host");
    EXPECT_EQ(u->GetHost(), "host");
}

HWTEST_F(UriTddTest, UriTdd_GetPort_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetPort(), PORT);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetPort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetPort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("://:567");
    EXPECT_EQ(u->GetPort(), 567);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host");
    EXPECT_EQ(u->GetPort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host:abc");
    EXPECT_EQ(u->GetPort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_007, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->port_ = 5;
    EXPECT_EQ(u->GetPort(), 5);
}

HWTEST_F(UriTddTest, UriTdd_GetPort_008, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://user@host:12345");
    EXPECT_EQ(u->GetPort(), 12345);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetUserInfo(), USER_INFO);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetUserInfo(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetUserInfo(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host");
    EXPECT_EQ(u->GetUserInfo(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://user@host");
    EXPECT_EQ(u->GetUserInfo(), "user");
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfo_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->userInfo_ = "preset";
    EXPECT_EQ(u->GetUserInfo(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetQuery(), QUERY);
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetQuery(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetQuery(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://h?q=1");
    EXPECT_EQ(u->GetQuery(), "q=1");
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://h#frag?q=1");
    EXPECT_EQ(u->GetQuery(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetQuery_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->query_ = "preset";
    EXPECT_EQ(u->GetQuery(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetPath_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetPath(), PATH);
}

HWTEST_F(UriTddTest, UriTdd_GetPath_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetPath(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_GetPath_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetPath_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:opaque");
    EXPECT_EQ(u->GetPath(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetPath_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:");
    EXPECT_EQ(u->GetPath(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetPath_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/path?q=1#frag");
    EXPECT_EQ(u->GetPath(), "/path");
}

HWTEST_F(UriTddTest, UriTdd_GetPath_007, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("/a/b/c");
    EXPECT_EQ(u->GetPath(), "/a/b/c");
}

HWTEST_F(UriTddTest, UriTdd_GetPath_008, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->path_ = "preset";
    EXPECT_EQ(u->GetPath(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    EXPECT_TRUE(segs.empty());
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/a/b/c");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 3u);
    EXPECT_EQ(segs[0], "a");
    EXPECT_EQ(segs[1], "b");
    EXPECT_EQ(segs[2], "c");
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/a/b/");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 2u);
    EXPECT_EQ(segs[0], "a");
    EXPECT_EQ(segs[1], "b");
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/a//b");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 2u);
    EXPECT_EQ(segs[0], "a");
    EXPECT_EQ(segs[1], "b");
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    EXPECT_TRUE(segs.empty());
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_006, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/one");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 1u);
    EXPECT_EQ(segs[0], "one");
}

HWTEST_F(UriTddTest, UriTdd_GetPathSegments_007, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/a/b?q=1#frag");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 2u);
    EXPECT_EQ(segs[0], "a");
    EXPECT_EQ(segs[1], "b");
}

HWTEST_F(UriTddTest, UriTdd_GetFragment_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetFragment(), FRAGMENT);
}

HWTEST_F(UriTddTest, UriTdd_GetFragment_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->GetFragment(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetFragment_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->GetFragment(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_GetFragment_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://h#frag");
    EXPECT_EQ(u->GetFragment(), "frag");
}

HWTEST_F(UriTddTest, UriTdd_GetFragment_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    u->fragment_ = "preset";
    EXPECT_EQ(u->GetFragment(), "preset");
}

HWTEST_F(UriTddTest, UriTdd_IsHierarchical_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_TRUE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_IsHierarchical_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_TRUE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_IsHierarchical_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:opaque");
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_IsHierarchical_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:");
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_IsHierarchical_005, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_IsAbsolute_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_TRUE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_IsAbsolute_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_FALSE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_IsAbsolute_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_FALSE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_IsAbsolute_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http:");
    EXPECT_TRUE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_IsRelative_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_TRUE(u->IsRelative());
}

HWTEST_F(UriTddTest, UriTdd_IsRelative_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_FALSE(u->IsRelative());
}

HWTEST_F(UriTddTest, UriTdd_IsRelative_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_FALSE(u->IsRelative());
}

HWTEST_F(UriTddTest, UriTdd_IsRelative_004, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("a:bc");
    EXPECT_FALSE(u->IsRelative());
}

HWTEST_F(UriTddTest, UriTdd_Equals_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b(FULL_URI);
    EXPECT_TRUE(a.Equals(b));
}

HWTEST_F(UriTddTest, UriTdd_Equals_002, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b("abc");
    EXPECT_FALSE(a.Equals(b));
}

HWTEST_F(UriTddTest, UriTdd_Equals_003, TestSize.Level1)
{
    Uri a(EMPTY);
    Uri b(EMPTY);
    EXPECT_TRUE(a.Equals(b));
}

HWTEST_F(UriTddTest, UriTdd_Equals_004, TestSize.Level1)
{
    Uri a("http://host");
    Uri b("http://host");
    EXPECT_TRUE(a.Equals(b));
}

HWTEST_F(UriTddTest, UriTdd_CompareTo_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b(FULL_URI);
    EXPECT_EQ(a.CompareTo(b), 0);
}

HWTEST_F(UriTddTest, UriTdd_CompareTo_002, TestSize.Level1)
{
    Uri a("http://a");
    Uri b("http://b");
    EXPECT_LT(a.CompareTo(b), 0);
}

HWTEST_F(UriTddTest, UriTdd_CompareTo_003, TestSize.Level1)
{
    Uri a("http://b");
    Uri b("http://a");
    EXPECT_GT(a.CompareTo(b), 0);
}

HWTEST_F(UriTddTest, UriTdd_CompareTo_004, TestSize.Level1)
{
    Uri a("abc");
    Uri b("abd");
    EXPECT_LT(a.CompareTo(b), 0);
}

HWTEST_F(UriTddTest, UriTdd_ToString_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->ToString(), FULL_URI);
}

HWTEST_F(UriTddTest, UriTdd_ToString_002, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("abc");
    EXPECT_EQ(u->ToString(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_ToString_003, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(EMPTY);
    EXPECT_EQ(u->ToString(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_OperatorEqual_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b(FULL_URI);
    EXPECT_TRUE(a == b);
}

HWTEST_F(UriTddTest, UriTdd_OperatorEqual_002, TestSize.Level1)
{
    Uri a(FULL_URI);
    Uri b("abc");
    EXPECT_FALSE(a == b);
}

HWTEST_F(UriTddTest, UriTdd_OperatorEqual_003, TestSize.Level1)
{
    Uri a(EMPTY);
    Uri b(EMPTY);
    EXPECT_TRUE(a == b);
}

HWTEST_F(UriTddTest, UriTdd_Marshalling_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    Parcel parcel;
    EXPECT_TRUE(u.Marshalling(parcel));
}

HWTEST_F(UriTddTest, UriTdd_Marshalling_002, TestSize.Level1)
{
    Uri u("http://example.com/path");
    Parcel parcel;
    EXPECT_TRUE(u.Marshalling(parcel));
}

HWTEST_F(UriTddTest, UriTdd_Marshalling_003, TestSize.Level1)
{
    Uri u("中文");
    Parcel parcel;
    EXPECT_FALSE(u.Marshalling(parcel));
}

HWTEST_F(UriTddTest, UriTdd_Marshalling_004, TestSize.Level1)
{
    Uri u(EMPTY);
    Parcel parcel;
    EXPECT_TRUE(u.Marshalling(parcel));
}

HWTEST_F(UriTddTest, UriTdd_Unmarshalling_001, TestSize.Level1)
{
    Parcel parcel;
    Uri* u = Uri::Unmarshalling(parcel);
    ASSERT_NE(u, nullptr);
    EXPECT_EQ(u->ToString(), EMPTY);
    delete u;
}

HWTEST_F(UriTddTest, UriTdd_Unmarshalling_002, TestSize.Level1)
{
    Uri src(FULL_URI);
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->ToString(), FULL_URI);
    EXPECT_TRUE(src.Equals(*dst));
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_Unmarshalling_003, TestSize.Level1)
{
    Uri src("http://example.com/path");
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->GetScheme(), "http");
    EXPECT_EQ(dst->GetHost(), "example.com");
    EXPECT_EQ(dst->GetPath(), "/path");
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_Unmarshalling_004, TestSize.Level1)
{
    Uri src("http://user@host:9090/p?q=k#f");
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->GetScheme(), "http");
    EXPECT_EQ(dst->GetAuthority(), "user@host:9090");
    EXPECT_EQ(dst->GetUserInfo(), "user");
    EXPECT_EQ(dst->GetHost(), "host");
    EXPECT_EQ(dst->GetPort(), 9090);
    EXPECT_EQ(dst->GetQuery(), "q=k");
    EXPECT_EQ(dst->GetPath(), "/p");
    EXPECT_EQ(dst->GetFragment(), "f");
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_CheckScheme_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_TRUE(u.CheckScheme());
}

HWTEST_F(UriTddTest, UriTdd_CheckScheme_002, TestSize.Level1)
{
    Uri u("http://x");
    u.uriString_ = "1abc:";
    EXPECT_FALSE(u.CheckScheme());
}

HWTEST_F(UriTddTest, UriTdd_CheckScheme_003, TestSize.Level1)
{
    Uri u("http://x");
    u.uriString_ = "ab_c:";
    EXPECT_FALSE(u.CheckScheme());
}

HWTEST_F(UriTddTest, UriTdd_CheckScheme_004, TestSize.Level1)
{
    Uri u("http://x");
    u.uriString_ = "ab|c:";
    EXPECT_TRUE(u.CheckScheme());
}

HWTEST_F(UriTddTest, UriTdd_CheckScheme_005, TestSize.Level1)
{
    Uri u("no_scheme");
    EXPECT_TRUE(u.CheckScheme());
}

HWTEST_F(UriTddTest, UriTdd_ParseScheme_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseScheme(), SCHEME);
}

HWTEST_F(UriTddTest, UriTdd_ParseScheme_002, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseScheme(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParseSsp_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseSsp(), SSP);
}

HWTEST_F(UriTddTest, UriTdd_ParseSsp_002, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseSsp(), "abc");
}

HWTEST_F(UriTddTest, UriTdd_ParseSsp_003, TestSize.Level1)
{
    Uri u("http:opaque#frag");
    EXPECT_EQ(u.ParseSsp(), "opaque");
}

HWTEST_F(UriTddTest, UriTdd_ParseAuthority_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseAuthority(), AUTHORITY);
}

HWTEST_F(UriTddTest, UriTdd_ParseAuthority_002, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseAuthority(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParseAuthority_003, TestSize.Level1)
{
    Uri u("http://host/path?q=1#frag");
    EXPECT_EQ(u.ParseAuthority(), "host");
}

HWTEST_F(UriTddTest, UriTdd_ParseUserInfo_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseUserInfo(), USER_INFO);
}

HWTEST_F(UriTddTest, UriTdd_ParseUserInfo_002, TestSize.Level1)
{
    Uri u("http://host");
    EXPECT_EQ(u.ParseUserInfo(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParseUserInfo_003, TestSize.Level1)
{
    Uri u("http://user@host");
    EXPECT_EQ(u.ParseUserInfo(), "user");
}

HWTEST_F(UriTddTest, UriTdd_ParseHost_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseHost(), HOST);
}

HWTEST_F(UriTddTest, UriTdd_ParseHost_002, TestSize.Level1)
{
    Uri u("http://host");
    EXPECT_EQ(u.ParseHost(), "host");
}

HWTEST_F(UriTddTest, UriTdd_ParseHost_003, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseHost(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParsePort_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParsePort(), PORT);
}

HWTEST_F(UriTddTest, UriTdd_ParsePort_002, TestSize.Level1)
{
    Uri u("http://host");
    EXPECT_EQ(u.ParsePort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_ParsePort_003, TestSize.Level1)
{
    Uri u("://:567");
    EXPECT_EQ(u.ParsePort(), 567);
}

HWTEST_F(UriTddTest, UriTdd_ParsePort_004, TestSize.Level1)
{
    Uri u("http://host:abc");
    EXPECT_EQ(u.ParsePort(), PORT_NONE);
}

HWTEST_F(UriTddTest, UriTdd_ParsePathWithSsi_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    size_t ssi = u.uriString_.find(':');
    EXPECT_EQ(u.ParsePath(ssi), PATH);
}

HWTEST_F(UriTddTest, UriTdd_ParsePathWithSsi_002, TestSize.Level1)
{
    Uri u("http://host/path?q=1#frag");
    size_t ssi = u.uriString_.find(':');
    EXPECT_EQ(u.ParsePath(ssi), "/path");
}

HWTEST_F(UriTddTest, UriTdd_ParsePathNoArg_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParsePath(), PATH);
}

HWTEST_F(UriTddTest, UriTdd_ParsePathNoArg_002, TestSize.Level1)
{
    Uri u("http:opaque");
    EXPECT_EQ(u.ParsePath(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParsePathNoArg_003, TestSize.Level1)
{
    Uri u("http:");
    EXPECT_EQ(u.ParsePath(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParsePathNoArg_004, TestSize.Level1)
{
    Uri u("/a/b/c");
    EXPECT_EQ(u.ParsePath(), "/a/b/c");
}

HWTEST_F(UriTddTest, UriTdd_ParseQuery_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseQuery(), QUERY);
}

HWTEST_F(UriTddTest, UriTdd_ParseQuery_002, TestSize.Level1)
{
    Uri u("http://h?q=1");
    EXPECT_EQ(u.ParseQuery(), "q=1");
}

HWTEST_F(UriTddTest, UriTdd_ParseQuery_003, TestSize.Level1)
{
    Uri u("http://h#frag?q=1");
    EXPECT_EQ(u.ParseQuery(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParseQuery_004, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseQuery(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_ParseFragment_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.ParseFragment(), FRAGMENT);
}

HWTEST_F(UriTddTest, UriTdd_ParseFragment_002, TestSize.Level1)
{
    Uri u("http://h#frag");
    EXPECT_EQ(u.ParseFragment(), "frag");
}

HWTEST_F(UriTddTest, UriTdd_ParseFragment_003, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.ParseFragment(), EMPTY);
}

HWTEST_F(UriTddTest, UriTdd_FindSchemeSeparator_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.FindSchemeSeparator(), u.uriString_.find(':'));
}

HWTEST_F(UriTddTest, UriTdd_FindSchemeSeparator_002, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.FindSchemeSeparator(), std::string::npos);
}

HWTEST_F(UriTddTest, UriTdd_FindSchemeSeparator_003, TestSize.Level1)
{
    Uri u("http://host");
    EXPECT_EQ(u.FindSchemeSeparator(), 4u);
}

HWTEST_F(UriTddTest, UriTdd_FindSchemeSeparator_004, TestSize.Level1)
{
    Uri u(FULL_URI);
    size_t first = u.FindSchemeSeparator();
    size_t second = u.FindSchemeSeparator();
    EXPECT_EQ(first, second);
}

HWTEST_F(UriTddTest, UriTdd_FindFragmentSeparator_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    EXPECT_EQ(u.FindFragmentSeparator(), u.uriString_.find('#'));
}

HWTEST_F(UriTddTest, UriTdd_FindFragmentSeparator_002, TestSize.Level1)
{
    Uri u("abc");
    EXPECT_EQ(u.FindFragmentSeparator(), std::string::npos);
}

HWTEST_F(UriTddTest, UriTdd_FindFragmentSeparator_003, TestSize.Level1)
{
    Uri u("http://h#frag");
    EXPECT_EQ(u.FindFragmentSeparator(), u.uriString_.find('#'));
}

HWTEST_F(UriTddTest, UriTdd_FindFragmentSeparator_004, TestSize.Level1)
{
    Uri u(FULL_URI);
    size_t first = u.FindFragmentSeparator();
    size_t second = u.FindFragmentSeparator();
    EXPECT_EQ(first, second);
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetScheme();
    u.scheme_ = "changed";
    EXPECT_EQ(u.GetScheme(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetAuthorityCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetAuthority();
    u.authority_ = "changed";
    EXPECT_EQ(u.GetAuthority(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetHostCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetHost();
    u.host_ = "changed";
    EXPECT_EQ(u.GetHost(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetPortCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetPort();
    u.port_ = 999;
    EXPECT_EQ(u.GetPort(), 999);
}

HWTEST_F(UriTddTest, UriTdd_GetUserInfoCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetUserInfo();
    u.userInfo_ = "changed";
    EXPECT_EQ(u.GetUserInfo(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetQueryCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetQuery();
    u.query_ = "changed";
    EXPECT_EQ(u.GetQuery(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetPathCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetPath();
    u.path_ = "changed";
    EXPECT_EQ(u.GetPath(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetFragmentCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetFragment();
    u.fragment_ = "changed";
    EXPECT_EQ(u.GetFragment(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_GetSchemeSpecificPartCachedTwice_001, TestSize.Level1)
{
    Uri u(FULL_URI);
    u.GetSchemeSpecificPart();
    u.ssp_ = "changed";
    EXPECT_EQ(u.GetSchemeSpecificPart(), "changed");
}

HWTEST_F(UriTddTest, UriTdd_PortZero_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host:0");
    EXPECT_EQ(u->GetPort(), 0);
}

HWTEST_F(UriTddTest, UriTdd_PortLarge_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host:65535");
    EXPECT_EQ(u->GetPort(), 65535);
}

HWTEST_F(UriTddTest, UriTdd_QueryWithoutValue_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://h?q");
    EXPECT_EQ(u->GetQuery(), "q");
}

HWTEST_F(UriTddTest, UriTdd_FragmentOnly_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://h#frag");
    EXPECT_EQ(u->GetFragment(), "frag");
    EXPECT_EQ(u->GetPath(), "");
    EXPECT_EQ(u->GetQuery(), "");
}

HWTEST_F(UriTddTest, UriTdd_AuthorityWithBackslash_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host\\path");
    EXPECT_EQ(u->GetPath(), "\\path");
}

HWTEST_F(UriTddTest, UriTdd_AuthorityEndsAtQuery_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host?q=1");
    EXPECT_EQ(u->GetAuthority(), "host");
    EXPECT_EQ(u->GetPath(), "");
    EXPECT_EQ(u->GetQuery(), "q=1");
}

HWTEST_F(UriTddTest, UriTdd_AuthorityEndsAtFragment_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host#frag");
    EXPECT_EQ(u->GetAuthority(), "host");
    EXPECT_EQ(u->GetFragment(), "frag");
}

HWTEST_F(UriTddTest, UriTdd_AbsoluteWithPathQueryFragment_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("http://host/a/b?q=1#f");
    EXPECT_EQ(u->GetPath(), "/a/b");
    EXPECT_EQ(u->GetQuery(), "q=1");
    EXPECT_EQ(u->GetFragment(), "f");
    EXPECT_FALSE(u->IsRelative());
    EXPECT_TRUE(u->IsHierarchical());
    EXPECT_TRUE(u->IsAbsolute());
}

HWTEST_F(UriTddTest, UriTdd_OperatorEqualSelf_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    EXPECT_TRUE(a == a);
}

HWTEST_F(UriTddTest, UriTdd_CompareToSelf_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    EXPECT_EQ(a.CompareTo(a), 0);
}

HWTEST_F(UriTddTest, UriTdd_EqualsSelf_001, TestSize.Level1)
{
    Uri a(FULL_URI);
    EXPECT_TRUE(a.Equals(a));
}

HWTEST_F(UriTddTest, UriTdd_ToStringPreservesInput_001, TestSize.Level1)
{
    const std::string input = "scheme://u@h:1/p?q#f";
    auto u = std::make_shared<Uri>(input);
    EXPECT_EQ(u->ToString(), input);
}

HWTEST_F(UriTddTest, UriTdd_RoundTripViaParcel_001, TestSize.Level1)
{
    Uri src(FULL_URI);
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_TRUE(src == *dst);
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_RoundTripViaParcel_002, TestSize.Level1)
{
    const std::string input = "ftp://anonymous@files.example.org:21/dir/file.txt";
    Uri src(input);
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->ToString(), input);
    EXPECT_EQ(dst->GetScheme(), "ftp");
    EXPECT_EQ(dst->GetHost(), "files.example.org");
    EXPECT_EQ(dst->GetPort(), 21);
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_RoundTripViaParcel_003, TestSize.Level1)
{
    Uri src("mailto:someone@example.com");
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    Uri* dst = Uri::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->ToString(), "mailto:someone@example.com");
    EXPECT_EQ(dst->GetScheme(), "mailto");
    EXPECT_EQ(dst->GetPath(), "");
    delete dst;
}

HWTEST_F(UriTddTest, UriTdd_EmptyStringAllGetters_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("");
    EXPECT_EQ(u->GetScheme(), EMPTY);
    EXPECT_EQ(u->GetSchemeSpecificPart(), EMPTY);
    EXPECT_EQ(u->GetAuthority(), EMPTY);
    EXPECT_EQ(u->GetHost(), EMPTY);
    EXPECT_EQ(u->GetPort(), PORT_NONE);
    EXPECT_EQ(u->GetUserInfo(), EMPTY);
    EXPECT_EQ(u->GetQuery(), EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_EQ(u->GetFragment(), EMPTY);
    EXPECT_FALSE(u->IsHierarchical());
    EXPECT_FALSE(u->IsAbsolute());
    EXPECT_FALSE(u->IsRelative());
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    EXPECT_TRUE(segs.empty());
}

HWTEST_F(UriTddTest, UriTdd_FullUriAllGetters_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>(FULL_URI);
    EXPECT_EQ(u->GetScheme(), SCHEME);
    EXPECT_EQ(u->GetSchemeSpecificPart(), SSP);
    EXPECT_EQ(u->GetAuthority(), AUTHORITY);
    EXPECT_EQ(u->GetHost(), HOST);
    EXPECT_EQ(u->GetPort(), PORT);
    EXPECT_EQ(u->GetUserInfo(), USER_INFO);
    EXPECT_EQ(u->GetQuery(), QUERY);
    EXPECT_EQ(u->GetPath(), PATH);
    EXPECT_EQ(u->GetFragment(), FRAGMENT);
    EXPECT_TRUE(u->IsHierarchical());
    EXPECT_TRUE(u->IsAbsolute());
    EXPECT_FALSE(u->IsRelative());
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 2u);
    EXPECT_EQ(segs[0], "path");
    EXPECT_EQ(segs[1], "sub");
}

HWTEST_F(UriTddTest, UriTdd_ConstructDataUri_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("data:text/plain;base64,SGVsbG8=");
    EXPECT_EQ(u->GetScheme(), "data");
    EXPECT_EQ(u->GetSchemeSpecificPart(), "text/plain;base64,SGVsbG8=");
    EXPECT_EQ(u->GetAuthority(), EMPTY);
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_FALSE(u->IsHierarchical());
}

HWTEST_F(UriTddTest, UriTdd_ConstructContentUri_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("content://com.example.provider/table/1");
    EXPECT_EQ(u->GetScheme(), "content");
    EXPECT_EQ(u->GetAuthority(), "com.example.provider");
    EXPECT_EQ(u->GetPath(), "/table/1");
    std::vector<std::string> segs;
    u->GetPathSegments(segs);
    ASSERT_EQ(segs.size(), 2u);
    EXPECT_EQ(segs[0], "table");
    EXPECT_EQ(segs[1], "1");
}

HWTEST_F(UriTddTest, UriTdd_ConstructTelUri_001, TestSize.Level1)
{
    auto u = std::make_shared<Uri>("tel:+8610000000");
    EXPECT_EQ(u->GetScheme(), "tel");
    EXPECT_EQ(u->GetSchemeSpecificPart(), "+8610000000");
    EXPECT_EQ(u->GetPath(), EMPTY);
    EXPECT_FALSE(u->IsHierarchical());
}
} // namespace OHOS
