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

#define private public
#include "session_info.h"
#undef private

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AAFwk {
class SessionInfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void SessionInfoTest::SetUpTestCase()
{
}

void SessionInfoTest::TearDownTestCase()
{
}

void SessionInfoTest::SetUp()
{
}

void SessionInfoTest::TearDown()
{
}

HWTEST_F(SessionInfoTest, SessionInfo_Marshalling_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.Marshalling(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_Unmarshalling_001, TestSize.Level1)
{
    Parcel parcel;
    SessionInfo* info = SessionInfo::Unmarshalling(parcel);
    ASSERT_TRUE(info != nullptr);
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingTwo_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingTwo(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingFive_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingFive(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSix_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSix(parcel));
}
} // namespace AAFwk
} // namespace OHOS