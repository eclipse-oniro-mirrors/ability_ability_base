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

#include <cstdint>

#include "ipc_object_stub.h"
#include "message_parcel.h"

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

HWTEST_F(SessionInfoTest, SessionInfo_ReadParcelTwo_001, TestSize.Level1)
{
    Parcel parcel;
    sptr<SessionInfo> sessionInfo = new (std::nothrow) SessionInfo();
    ASSERT_TRUE(sessionInfo != nullptr);
    SessionInfo* info = SessionInfo::ReadParcelTwo(sessionInfo, parcel);
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

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSeven_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSeven(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSeven_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.isTargetPlugin = true;
    sessionInfo.hostBundleName = "com.example.test";
    sessionInfo.frameNum = 50;
    sessionInfo.callerTypeForAnco = 1;
    Want want;
    want.SetAction("action.system.test");
    sessionInfo.want = want;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSeven(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSix_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.splitRatioPreference = 2;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSix(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingOne_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingOne(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingTwo_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.identityToken = "identity";
    sessionInfo.parentWindowType = 5;
    sessionInfo.hideStartWindow = true;
    sessionInfo.shouldSkipKillInStartup = true;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingTwo(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingThree_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingThree(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingThree_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.persistentId = 1;
    sessionInfo.hostWindowId = 2;
    sessionInfo.realHostWindowId = 3;
    sessionInfo.state = CallToState::FOREGROUND;
    sessionInfo.resultCode = 1;
    sessionInfo.requestCode = 2;
    sessionInfo.errorReason = "reason";
    sessionInfo.errorCode = 3;
    sessionInfo.uiAbilityId = 4;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingThree(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingFour_001, TestSize.Level1)
{
    SessionInfo sessionInfo;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingFour(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingFour_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.isNewWant = false;
    sessionInfo.isClearSession = true;
    sessionInfo.callingTokenId = 10;
    sessionInfo.reuse = true;
    sessionInfo.canStartAbilityFromBackground = true;
    sessionInfo.collaboratorType = 2;
    sessionInfo.sessionName = "name";
    sessionInfo.uiExtensionComponentId = 20;
    sessionInfo.isAsyncModalBinding = true;
    sessionInfo.uiExtensionUsage = UIExtensionUsage::EMBEDDED;
    sessionInfo.isAtomicService = true;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingFour(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingFive_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.isBackTransition = true;
    sessionInfo.displayId = 100;
    sessionInfo.density = 3.5f;
    sessionInfo.orientation = 180;
    sessionInfo.needClearInNotShowRecent = true;
    sessionInfo.instanceKey = "key";
    sessionInfo.requestId = 5;
    sessionInfo.scbRequestId = 6;
    sessionInfo.userId = 7;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingFive(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSix_003, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.dpiFollowStrategy = 1;
    sessionInfo.specifiedFlag = "specified";
    sessionInfo.reuseDelegatorWindow = true;
    sessionInfo.splitRatioPreference = 3;
    sessionInfo.scenarios = 4;
    sessionInfo.isPrelaunch = true;
    sessionInfo.targetGrantBundleName = "target";
    sessionInfo.specifiedReason = 8;
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSix(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSeven_003, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.supportWindowModes = {
        static_cast<AppExecFwk::SupportWindowMode>(0),
        static_cast<AppExecFwk::SupportWindowMode>(1),
    };
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSeven(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSeven_004, TestSize.Level1)
{
    SessionInfo sessionInfo;
    for (int i = 0; i < 10; ++i) {
        sessionInfo.supportWindowModes.emplace_back(
            static_cast<AppExecFwk::SupportWindowMode>(i));
    }
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSeven(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingSeven_005, TestSize.Level1)
{
    SessionInfo sessionInfo;
    for (int i = 0; i < 11; ++i) {
        sessionInfo.supportWindowModes.emplace_back(
            static_cast<AppExecFwk::SupportWindowMode>(i));
    }
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.DoMarshallingSeven(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_Marshalling_002, TestSize.Level1)
{
    SessionInfo sessionInfo;
    sessionInfo.persistentId = 1;
    sessionInfo.hostBundleName = "bundle";
    sessionInfo.frameNum = 10;
    sessionInfo.callerTypeForAnco = 1;
    sessionInfo.isTargetPlugin = true;
    sessionInfo.state = CallToState::BACKGROUND;
    sessionInfo.want.SetAction("test.action");
    Parcel parcel;
    ASSERT_TRUE(sessionInfo.Marshalling(parcel));
}

HWTEST_F(SessionInfoTest, SessionInfo_ReadParcelOne_001, TestSize.Level1)
{
    Parcel parcel;
    sptr<SessionInfo> sessionInfo = new (std::nothrow) SessionInfo();
    ASSERT_TRUE(sessionInfo != nullptr);
    SessionInfo* info = SessionInfo::ReadParcelOne(sessionInfo.GetRefPtr(), parcel);
    ASSERT_TRUE(info != nullptr);
    EXPECT_EQ(info->persistentId, 0);
    EXPECT_EQ(info->userId, 0);
    EXPECT_EQ(info->isNewWant, false);
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripDefaults_001, TestSize.Level1)
{
    SessionInfo src;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->persistentId, 0);
    EXPECT_EQ(dst->userId, -1);
    EXPECT_EQ(dst->resultCode, -1);
    EXPECT_EQ(dst->requestCode, -1);
    EXPECT_EQ(dst->errorCode, -1);
    EXPECT_EQ(dst->isNewWant, true);
    EXPECT_EQ(dst->hostBundleName, "");
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripPodFieldsOne_001, TestSize.Level1)
{
    SessionInfo src;
    src.persistentId = 100;
    src.hostWindowId = 200;
    src.realHostWindowId = -5;
    src.parentWindowType = 3;
    src.hideStartWindow = true;
    src.shouldSkipKillInStartup = true;
    src.state = CallToState::FOREGROUND;
    src.resultCode = 7;
    src.requestCode = 9;
    src.errorCode = 42;
    src.uiAbilityId = 123456;
    src.isNewWant = false;
    src.isClearSession = true;
    src.callingTokenId = 88;
    src.reuse = true;
    src.collaboratorType = 2;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->persistentId, 100);
    EXPECT_EQ(dst->hostWindowId, 200u);
    EXPECT_EQ(dst->realHostWindowId, -5);
    EXPECT_EQ(dst->parentWindowType, 3u);
    EXPECT_EQ(dst->hideStartWindow, true);
    EXPECT_EQ(dst->shouldSkipKillInStartup, true);
    EXPECT_EQ(dst->state, CallToState::FOREGROUND);
    EXPECT_EQ(dst->resultCode, 7);
    EXPECT_EQ(dst->requestCode, 9);
    EXPECT_EQ(dst->errorCode, 42);
    EXPECT_EQ(dst->uiAbilityId, 123456);
    EXPECT_EQ(dst->isNewWant, false);
    EXPECT_EQ(dst->isClearSession, true);
    EXPECT_EQ(dst->callingTokenId, 88u);
    EXPECT_EQ(dst->reuse, true);
    EXPECT_EQ(dst->collaboratorType, 2);
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripPodFieldsTwo_001, TestSize.Level1)
{
    SessionInfo src;
    src.uiExtensionComponentId = 999;
    src.uiExtensionUsage = UIExtensionUsage::EMBEDDED;
    src.isAtomicService = true;
    src.isBackTransition = true;
    src.displayId = 7;
    src.density = 2.5f;
    src.orientation = 90;
    src.requestId = 11;
    src.scbRequestId = 22;
    src.userId = 33;
    src.dpiFollowStrategy = 1;
    src.splitRatioPreference = 4;
    src.scenarios = 5;
    src.isPrelaunch = true;
    src.specifiedReason = 6;
    src.frameNum = 50;
    src.callerTypeForAnco = 1;
    src.isTargetPlugin = true;
    src.nativeHideWindow = true;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->uiExtensionComponentId, 999u);
    EXPECT_EQ(dst->uiExtensionUsage, UIExtensionUsage::EMBEDDED);
    EXPECT_EQ(dst->isAtomicService, true);
    EXPECT_EQ(dst->isBackTransition, true);
    EXPECT_EQ(dst->displayId, 7u);
    EXPECT_FLOAT_EQ(dst->density, 2.5f);
    EXPECT_EQ(dst->orientation, 90);
    EXPECT_EQ(dst->requestId, 11);
    EXPECT_EQ(dst->scbRequestId, 22);
    EXPECT_EQ(dst->userId, 33);
    EXPECT_EQ(dst->dpiFollowStrategy, 1);
    EXPECT_EQ(dst->splitRatioPreference, 4);
    EXPECT_EQ(dst->scenarios, 5);
    EXPECT_EQ(dst->isPrelaunch, true);
    EXPECT_EQ(dst->specifiedReason, 6);
    EXPECT_EQ(dst->frameNum, 50);
    EXPECT_EQ(dst->callerTypeForAnco, 1);
    EXPECT_EQ(dst->isTargetPlugin, true);
    EXPECT_EQ(dst->nativeHideWindow, true);
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripStringFields_001, TestSize.Level1)
{
    SessionInfo src;
    src.identityToken = "id_token";
    src.errorReason = "reason";
    src.sessionName = "session_name";
    src.instanceKey = "instance_key";
    src.specifiedFlag = "flag";
    src.targetGrantBundleName = "bundle_name";
    src.hostBundleName = "host_bundle";
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->identityToken, "id_token");
    EXPECT_EQ(dst->errorReason, "reason");
    EXPECT_EQ(dst->sessionName, "session_name");
    EXPECT_EQ(dst->instanceKey, "instance_key");
    EXPECT_EQ(dst->specifiedFlag, "flag");
    EXPECT_EQ(dst->targetGrantBundleName, "bundle_name");
    EXPECT_EQ(dst->hostBundleName, "host_bundle");
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripWindowModes_001, TestSize.Level1)
{
    SessionInfo src;
    src.supportWindowModes = {
        static_cast<AppExecFwk::SupportWindowMode>(0),
        static_cast<AppExecFwk::SupportWindowMode>(1),
        static_cast<AppExecFwk::SupportWindowMode>(2),
    };
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->supportWindowModes.size(), 3u);
    EXPECT_EQ(static_cast<int32_t>(dst->supportWindowModes[0]), 0);
    EXPECT_EQ(static_cast<int32_t>(dst->supportWindowModes[1]), 1);
    EXPECT_EQ(static_cast<int32_t>(dst->supportWindowModes[2]), 2);
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripWindowModesMax_001, TestSize.Level1)
{
    SessionInfo src;
    for (int i = 0; i < 10; ++i) {
        src.supportWindowModes.emplace_back(
            static_cast<AppExecFwk::SupportWindowMode>(i));
    }
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->supportWindowModes.size(), 10u);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(static_cast<int32_t>(dst->supportWindowModes[i]), i);
    }
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripWindowModesOverMax_001, TestSize.Level1)
{
    SessionInfo src;
    for (int i = 0; i < 11; ++i) {
        src.supportWindowModes.emplace_back(
            static_cast<AppExecFwk::SupportWindowMode>(i));
    }
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->supportWindowModes.size(), 0u);
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripWant_001, TestSize.Level1)
{
    SessionInfo src;
    src.want.SetAction("action.session.test");
    src.want.AddEntity("entity.one");
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->want.GetAction(), "action.session.test");
    EXPECT_TRUE(dst->want.HasEntity("entity.one"));
    delete dst;
}

HWTEST_F(SessionInfoTest, SessionInfo_RoundTripRemoteObjectsNull_001, TestSize.Level1)
{
    SessionInfo src;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->sessionToken, nullptr);
    EXPECT_EQ(dst->callerSession, nullptr);
    EXPECT_EQ(dst->callerToken, nullptr);
    EXPECT_EQ(dst->parentToken, nullptr);
    EXPECT_EQ(dst->startSetting, nullptr);
    EXPECT_EQ(dst->processOptions, nullptr);
    EXPECT_EQ(dst->windowCreateParams, nullptr);
    EXPECT_EQ(dst->showParams, nullptr);
    EXPECT_EQ(dst->startWindowOption, nullptr);
    delete dst;
}

/**
 * @tc.name: SessionInfo_DoMarshallingEight_001
 * @tc.desc: DoMarshallingEight writes presence-bool false and two fields in order (null token)
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingEight_001, TestSize.Level1)
{
    SessionInfo src;
    src.originalCallerInfo.callerToken = nullptr;
    src.originalCallerInfo.persistentId = 10;
    src.callerPersistentId = 20;
    Parcel parcel;
    ASSERT_TRUE(src.DoMarshallingEight(parcel));
    EXPECT_FALSE(parcel.ReadBool());
    EXPECT_EQ(parcel.ReadInt32(), 10);
    EXPECT_EQ(parcel.ReadInt32(), 20);
}

/**
 * @tc.name: SessionInfo_DoMarshallingEight_002
 * @tc.desc: DoMarshallingEight with a non-null callerToken writes the presence flag and the object
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_DoMarshallingEight_002, TestSize.Level1)
{
    SessionInfo src;
    src.originalCallerInfo.callerToken = new IPCObjectStub(u"session.info.test.token");
    src.originalCallerInfo.persistentId = 1;
    src.callerPersistentId = 3;
    MessageParcel parcel;
    ASSERT_TRUE(src.DoMarshallingEight(parcel));
    EXPECT_TRUE(parcel.ReadBool());
    EXPECT_NE(parcel.ReadRemoteObject(), nullptr);
    EXPECT_EQ(parcel.ReadInt32(), 1);
    EXPECT_EQ(parcel.ReadInt32(), 3);
}

/**
 * @tc.name: SessionInfo_RoundTripOriginalCallerInfo_001
 * @tc.desc: Migration declaration fields survive a full marshalling round trip
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_RoundTripOriginalCallerInfo_001, TestSize.Level1)
{
    SessionInfo src;
    src.persistentId = 10;
    src.originalCallerInfo.callerToken = nullptr;
    src.originalCallerInfo.persistentId = 10;
    src.callerPersistentId = 21;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.callerToken, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.persistentId, 10);
    EXPECT_EQ(dst->callerPersistentId, 21);
    delete dst;
}

/**
 * @tc.name: SessionInfo_RoundTripOriginalCallerInfo_002
 * @tc.desc: Migration declaration fields default to zero on round trip (plain request)
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_RoundTripOriginalCallerInfo_002, TestSize.Level1)
{
    SessionInfo src;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.callerToken, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.persistentId, 0);
    EXPECT_EQ(dst->callerPersistentId, 0);
    delete dst;
}

/**
 * @tc.name: SessionInfo_RoundTripOriginalCallerInfo_003
 * @tc.desc: Full round trip with a NON-NULL caller token — the WriteRemoteObject /
 *           ReadRemoteObject branches in DoMarshallingEight / ReadParcelThree are
 *           exercised end-to-end (BASE-05 fix)
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_RoundTripOriginalCallerInfo_003, TestSize.Level1)
{
    SessionInfo src;
    src.originalCallerInfo.callerToken = new IPCObjectStub(u"session.info.roundtrip.token");
    src.originalCallerInfo.persistentId = 42;
    src.callerPersistentId = 99;
    MessageParcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    // Token round-trips as a non-null binder proxy (remote object slot consumed).
    EXPECT_NE(dst->originalCallerInfo.callerToken, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.persistentId, 42);
    EXPECT_EQ(dst->callerPersistentId, 99);
    delete dst;
}

/**
 * @tc.name: SessionInfo_RoundTripOriginalCallerBoundary_001
 * @tc.desc: Boundary values of caller ids survive the round trip
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_RoundTripOriginalCallerBoundary_001, TestSize.Level1)
{
    SessionInfo src;
    src.originalCallerInfo.persistentId = INT32_MAX;
    src.callerPersistentId = INT32_MIN;
    Parcel parcel;
    ASSERT_TRUE(src.Marshalling(parcel));
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(dst->originalCallerInfo.persistentId, INT32_MAX);
    EXPECT_EQ(dst->callerPersistentId, INT32_MIN);
    delete dst;
}

/**
 * @tc.name: SessionInfo_UnmarshallingTruncatedParcel_001
 * @tc.desc: A truncated/garbage parcel makes Unmarshalling fail visibly (nullptr)
 *           instead of silently defaulting the migration fields
 * @tc.type: FUNC
 * @tc.require: issue
 */
HWTEST_F(SessionInfoTest, SessionInfo_UnmarshallingTruncatedParcel_001, TestSize.Level1)
{
    Parcel parcel;
    SessionInfo* dst = SessionInfo::Unmarshalling(parcel);
    EXPECT_EQ(dst, nullptr);
}

/**
 * @tc.name: SessionInfo_Clone_0100
 * @tc.desc: Clone returns an independent deep copy: later mutations of either want do not
 *           affect the other one
 * @tc.type: FUNC
 */
HWTEST_F(SessionInfoTest, SessionInfo_Clone_001, TestSize.Level1)
{
    SessionInfo src;
    src.want.SetAction("action.session.clone");
    src.want.SetParam("cloneIntKey", 100);
    src.persistentId = 42;
    src.instanceKey = "app_instance_0";
    auto cloned = src.Clone();
    ASSERT_NE(cloned, nullptr);
    EXPECT_NE(cloned.GetRefPtr(), &src);
    EXPECT_EQ(cloned->want.GetAction(), "action.session.clone");
    EXPECT_EQ(cloned->want.GetIntParam("cloneIntKey", -1), 100);
    EXPECT_EQ(cloned->persistentId, 42);
    EXPECT_EQ(cloned->instanceKey, "app_instance_0");

    // want is deep-copplied: mutating either copy must not affect the other one.
    src.want.SetParam("cloneIntKey", 200);
    src.want.SetParam("cloneSrcOnlyKey", 1);
    cloned->want.SetParam("cloneCloneOnlyKey", 2);
    EXPECT_EQ(cloned->want.GetIntParam("cloneIntKey", -1), 100);
    EXPECT_FALSE(cloned->want.HasParameter("cloneSrcOnlyKey"));
    EXPECT_EQ(src.want.GetIntParam("cloneIntKey", -1), 200);
    EXPECT_FALSE(src.want.HasParameter("cloneCloneOnlyKey"));
}

/**
 * @tc.name: SessionInfo_Clone_0200
 * @tc.desc: Clone keeps a null processOptions null; the deep copy of a non-null processOptions
 *           (distinct pointee, no in-place write leakage) is covered by
 *           ChangeAbilityVisibility_001 in ability_runtime's
 *           ui_ability_lifecycle_manager_third_test, where the process_options header is
 *           reachable
 * @tc.type: FUNC
 */
HWTEST_F(SessionInfoTest, SessionInfo_Clone_002, TestSize.Level1)
{
    SessionInfo empty;
    auto clonedEmpty = empty.Clone();
    ASSERT_NE(clonedEmpty, nullptr);
    EXPECT_NE(clonedEmpty.GetRefPtr(), &empty);
    EXPECT_EQ(clonedEmpty->processOptions, nullptr);
    EXPECT_EQ(clonedEmpty->want.GetAction(), "");
}
} // namespace AAFwk
} // namespace OHOS
