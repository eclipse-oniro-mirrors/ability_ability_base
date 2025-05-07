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
#include "bool_wrapper.h"
#include "double_wrapper.h"
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
    want.SetUri(cwant.uri);
    for (auto it : cwant.params) {
        ABILITYBASE_LOGD("params key: %{public}s, value: %{public}s",
            it.first.c_str(), it.second.c_str());
        wantParams.SetParam(it.first, String::Box(it.second));
    }

    for (auto it : cwant.intParams) {
        ABILITYBASE_LOGD("int key: %{public}s, value: %{public}d", it.first.c_str(), it.second);
        wantParams.SetParam(it.first, Integer::Box(it.second));
    }

    for (auto it : cwant.boolParams) {
        ABILITYBASE_LOGD("bool key: %{public}s, value: %{public}d", it.first.c_str(), it.second);
        wantParams.SetParam(it.first, Boolean::Box(it.second));
    }

    for (auto it : cwant.doubleParams) {
        ABILITYBASE_LOGD("bool key: %{public}s, value: %{public}f", it.first.c_str(), it.second);
        wantParams.SetParam(it.first, Double::Box(it.second));
    }

    for (auto it : cwant.fds) {
        ABILITYBASE_LOGD("fd key: %{public}s, value: %{public}d", it.first.c_str(), it.second);
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

constexpr int32_t FD_NULL = -1;
constexpr int32_t FD_SIZE = 2;

std::pair<std::string, int> GetFD(std::pair<std::string, sptr<IInterface>> fdparams)
{
    std::pair<std::string, int> fd = {"", FD_NULL};
    ABILITYBASE_LOGD("params key: %{public}s", fdparams.first.c_str());
    auto wantParams = WantParamWrapper::Unbox(IWantParams::Query(fdparams.second));
    auto params = wantParams.GetParams();
    if (params.size() != FD_SIZE) {
        ABILITYBASE_LOGD("params key: %{public}s not fd", fdparams.first.c_str());
        return fd;
    }

    auto typeIt = params.find(TYPE_PROPERTY);
    if (typeIt == params.end()) {
        ABILITYBASE_LOGD("params key: %{public}s not find FD_TYPE", fdparams.first.c_str());
        return fd;
    }
    IString *strValue = IString::Query(typeIt->second);
    if (strValue == nullptr) {
        ABILITYBASE_LOGD("params key: %{public}s FD_TYPE not string", fdparams.first.c_str());
        return fd;
    }
    std::string typeString = String::Unbox(strValue);
    if (typeString != FD) {
        ABILITYBASE_LOGD("params key: %{public}s not FD", fdparams.first.c_str());
        return fd;
    }

    auto valueIt = params.find(VALUE_PROPERTY);
    if (valueIt == params.end()) {
        ABILITYBASE_LOGD("params key: %{public}s not find FD_VALUE", fdparams.first.c_str());
        return fd;
    }
    IInteger *intValue = IInteger::Query(valueIt->second);
    if (intValue == nullptr) {
        ABILITYBASE_LOGD("params key: %{public}s FD_VALUE not int", fdparams.first.c_str());
        return fd;
    }
    ABILITYBASE_LOGD("params key: %{public}s, value: %{public}d", fdparams.first.c_str(), Integer::Unbox(intValue));
    fd = {fdparams.first, Integer::Unbox(intValue)};
    return fd;
}

[[maybe_unused]] AbilityBase_ErrorCode CWantManager::TransformToCWantWithoutElement(
    Want want, bool isDup, AbilityBase_Want &cwant)
{
    cwant.uri = want.GetUriString();
    WantParams wantParams = want.GetParams();
    auto params = wantParams.GetParams();
    for (const auto& iter : params) {
        if (IString::Query(iter.second) != nullptr) {
            ABILITYBASE_LOGD("params key: %{public}s, value: %{public}s",
                iter.first.c_str(),
                String::Unbox(IString::Query(iter.second)).c_str());
            cwant.params.insert(
                std::pair<std::string, std::string>(iter.first, String::Unbox(IString::Query(iter.second))));
        } else if (IInteger::Query(iter.second) != nullptr) {
            ABILITYBASE_LOGD("params key: %{public}s, value: %{public}d",
                iter.first.c_str(),
                Integer::Unbox(IInteger::Query(iter.second)));
            cwant.intParams.insert(
                std::pair<std::string, int>(iter.first, Integer::Unbox(IInteger::Query(iter.second))));
        } else if (IBoolean::Query(iter.second) != nullptr) {
            ABILITYBASE_LOGD("params key: %{public}s, value: %{public}d",
                iter.first.c_str(),
                Boolean::Unbox(IBoolean::Query(iter.second)));
            cwant.boolParams.insert(
                std::pair<std::string, bool>(iter.first, Boolean::Unbox(IBoolean::Query(iter.second))));
        } else if (IDouble::Query(iter.second) != nullptr) {
            ABILITYBASE_LOGD("params key: %{public}s, value: %{public}lf",
                iter.first.c_str(),
                Double::Unbox(IDouble::Query(iter.second)));
            cwant.doubleParams.insert(
                std::pair<std::string, double>(iter.first, Double::Unbox(IDouble::Query(iter.second))));
        } else if (IWantParams::Query(iter.second) != nullptr) {
            auto fd = GetFD(iter);
            if (fd.first != iter.first || fd.second == FD_NULL) {
                continue;
            }
            cwant.fds.insert(fd);
        }
    }

    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}
} // namespace AAFwk
} // namespace OHOS