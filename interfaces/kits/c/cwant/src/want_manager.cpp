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

#include "want_manager.h"

#include <map>
#include <string>
#include "ability_base_log_wrapper.h"
#include "int_wrapper.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"

namespace OHOS {
namespace AAFwk {

[[maybe_unused]] AbilityBase_ErrorCode CWantManager::TransformToWant(AbilityBase_Want cwant, bool isDup,
    Want &want)
{
    want.SetElementName("", cwant.element.bundleName, cwant.element.abilityName, cwant.element.moduleName);
    WantParams wantParams;
    wantParams.SetParam(Want::PARAM_MODULE_NAME, String::Box(cwant.element.moduleName));
    for (auto it : cwant.params) {
        ABILITYBASE_LOGI("params key: %{public}s, value: %{public}s",
            it.first.c_str(), it.second.c_str());
        wantParams.SetParam(it.first, String::Box(it.second));
    }

    for (auto it : cwant.fds) {
        ABILITYBASE_LOGI("fd key: %{public}s, value: %{public}d", it.first.c_str(), it.second);
        WantParams wp;
        wp.SetParam(TYPE_PROPERTY, String::Box(FD));
        int32_t new_fd = isDup ? dup(it.second) : it.second;
        wp.SetParam(VALUE_PROPERTY, Integer::Box(new_fd));
        OHOS::sptr<IWantParams> pWantParams = WantParamWrapper::Box(wp);
        if (pWantParams != nullptr) {
            wantParams.SetParam(it.first, pWantParams);
        }
    }
    want.SetParams(wantParams);
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}
} // namespace AAFwk
} // namespace OHOS