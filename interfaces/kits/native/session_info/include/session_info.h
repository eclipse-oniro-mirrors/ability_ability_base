/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "iremote_object.h"
#include "parcel.h"
#include "session/host/include/zidl/session_interface.h"
#include "ui/rs_surface_node.h"

namespace OHOS {
namespace AAFwk {

class SessionInfo : public Parcelable {
public:
    SessionInfo() {};
    virtual ~SessionInfo() {};
    virtual bool Marshalling(Parcel& parcel) const override;
    static SessionInfo* Unmarshalling(Parcel& parcel);

    sptr<Rosen::ISession> sessionToken = nullptr;
    sptr<Rosen::ISession> callerSession = nullptr;
    sptr<IRemoteObject> callerToken = nullptr;
    std::shared_ptr<Rosen::RSSurfaceNode> surfaceNode = nullptr;
    uint64_t persistentId = 0;
};
} // namespace AAFwk
} // namespace OHOS
#endif // OHOS_ABILITY_SESSION_INFO_H
