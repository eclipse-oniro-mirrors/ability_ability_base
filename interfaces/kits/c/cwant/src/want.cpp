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

#include "want.h"
#include <map>
#include <string>
#include <unistd.h>
#include "ability_base_log_wrapper.h"
#include "securec.h"
#include "want/include/want.h"

struct AbilityBase_Want {
    AbilityBase_Element element;
    std::map<std::string, std::string> params;
    std::map<std::string, int32_t> fds;
};

AbilityBase_Want* OH_AbilityBase_CreateWant(AbilityBase_Element element)
{
    std::unique_ptr<AbilityBase_Want> want = std::make_unique<AbilityBase_Want>();
    want->element.bundleName = element.bundleName;
    want->element.moduleName = element.moduleName;
    want->element.abilityName = element.abilityName;
    return want.release();
}

AbilityBase_ErrorCode OH_AbilityBase_DestroyWant(AbilityBase_Want* want)
{
    if (want == nullptr) {
        ABILITYBASE_LOGE("null want");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }

    if (!want->params.empty()) {
        want->params.clear();
    }

    delete want;
    want = nullptr;
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_SetWantElement(AbilityBase_Want* want, AbilityBase_Element element)
{
    if (want == nullptr) {
        ABILITYBASE_LOGE("null want");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    want->element.bundleName = element.bundleName;
    want->element.moduleName = element.moduleName;
    want->element.abilityName = element.abilityName;
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_GetWantElement(AbilityBase_Want* want, AbilityBase_Element* element)
{
    if (want == nullptr || element == nullptr) {
        ABILITYBASE_LOGE("null arg");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    element->bundleName = want->element.bundleName;
    element->moduleName = want->element.moduleName;
    element->abilityName = want->element.abilityName;
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_SetWantCharParam(AbilityBase_Want* want, const char* key, const char* value)
{
    if (want == nullptr || key == nullptr || value == nullptr) {
        ABILITYBASE_LOGE("null want");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    want->params[std::string(key)] = std::string(value);
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_GetWantCharParam(AbilityBase_Want* want, const char* key,
    char* value, size_t valueSize)
{
    if (want == nullptr || key == nullptr || value == nullptr) {
        ABILITYBASE_LOGE("null arg");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    auto it = want->params.find(key);
    if (it == want->params.end()) {
        ABILITYBASE_LOGE("not found key");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    const std::string& foundValue = it->second;
    if (foundValue.size() >= valueSize) {
        ABILITYBASE_LOGE("no enough buffer");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    if (strcpy_s(value, valueSize, it->second.c_str()) != EOK) {
        ABILITYBASE_LOGE("strcpy_s err");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_AddWantFd(AbilityBase_Want* want, const char* key, int32_t fd)
{
    if (want == nullptr || key == nullptr) {
        ABILITYBASE_LOGE("null want");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    want->fds[std::string(key)] = fd;
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}

AbilityBase_ErrorCode OH_AbilityBase_GetWantFd(AbilityBase_Want* want, const char* key, int32_t* fd)
{
    if (want == nullptr || key == nullptr || fd == nullptr) {
        ABILITYBASE_LOGE("null arg");
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    auto it = want->fds.find(key);
    if (it == want->fds.end()) {
        ABILITYBASE_LOGE("not found key: %{public}s", key);
        return ABILITY_BASE_ERROR_CODE_PARAM_INVALID;
    }
    *fd = it->second;
    return ABILITY_BASE_ERROR_CODE_NO_ERROR;
}