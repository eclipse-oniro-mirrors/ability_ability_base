/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_ABILITY_SESSION_INFO_H
#define OHOS_ABILITY_SESSION_INFO_H

#include <typeinfo>
#include <unistd.h>

#include "ability_info.h"
#include "iremote_object.h"
#include "parcel.h"
#include "session_info_constants.h"
#include "want.h"

namespace OHOS {
namespace AAFwk {
class AbilityStartSetting;
class ProcessOptions;
class StartWindowOption;

class SessionInfo : public Parcelable {
public:
    SessionInfo() = default;
    virtual ~SessionInfo() = default;
    bool Marshalling(Parcel& parcel) const override;
    static SessionInfo* Unmarshalling(Parcel& parcel);

    sptr<IRemoteObject> sessionToken = nullptr;
    sptr<IRemoteObject> callerSession = nullptr;
    sptr<IRemoteObject> callerToken = nullptr;
    sptr<IRemoteObject> parentToken = nullptr;
    std::string identityToken;
    int32_t persistentId = 0;
    uint32_t hostWindowId = 0;
    int32_t realHostWindowId = 0;
    uint32_t parentWindowType = 1;
    // Status of the UIAbility invoked by the StartABilityByCall
    CallToState state = CallToState::UNKNOW;
    // Returned resultCode of terminateSelfWithResult
    int32_t resultCode = -1;
    // requestCode of StartAbilityForResult
    int32_t requestCode = -1;
    std::string errorReason;
    int32_t errorCode = -1;
    int64_t uiAbilityId = 0;
    std::shared_ptr<AbilityStartSetting> startSetting = nullptr;
    std::shared_ptr<ProcessOptions> processOptions = nullptr;
    std::shared_ptr<StartWindowOption> startWindowOption = nullptr;
    std::vector<AppExecFwk::SupportWindowMode> supportWindowModes;
    Want want;
    int32_t userId = -1;
    bool isNewWant = true;
    bool isClearSession = false;
    uint32_t callingTokenId = 0;
    bool reuse = false;
    bool canStartAbilityFromBackground = false;
    int32_t collaboratorType = 0;
    std::string sessionName = "";
    bool isAsyncModalBinding = false;
    bool isAtomicService = false;
    bool isBackTransition = false;
    bool isSkipErms = false;
    // The following attributes are used for UIExtension
    // To ensure security, this attribute must be rewritten on the server-side.
    UIExtensionUsage uiExtensionUsage = UIExtensionUsage::MODAL;
    uint64_t uiExtensionComponentId = 0;
    uint64_t displayId = 0;
    float density = 0.0f;
    int32_t orientation = 0;
    bool needClearInNotShowRecent = false;
    std::string instanceKey = "";
    bool isMinimizedDuringFreeInstall = false;
    bool isFromIcon = false;
    int32_t tmpSpecifiedId = 0;

private:
    bool DoMarshallingOne(Parcel& parcel) const;
    bool DoMarshallingTwo(Parcel& parcel) const;
    bool DoMarshallingThree(Parcel& parcel) const;
    bool DoMarshallingFour(Parcel& parcel) const;
    bool DoMarshallingFive(Parcel& parcel) const;
    static SessionInfo* ReadParcelOne(SessionInfo* info, Parcel& parcel);
};
} // namespace AAFwk
} // namespace OHOS
#endif // OHOS_ABILITY_SESSION_INFO_H
