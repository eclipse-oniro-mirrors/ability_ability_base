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

#include "page_node_info.h"

#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AbilityBase {
constexpr const char* PAGE_NODE_INFO_ID = "id";
constexpr const char* PAGE_NODE_INFO_DEPTH = "depth";
constexpr const char* PAGE_NODE_INFO_AUTO_FILL_TYPE = "autoFillType";
constexpr const char* PAGE_NODE_INFO_TAG = "tag";
constexpr const char* PAGE_NODE_INFO_VALUE = "value";
constexpr const char* PAGE_NODE_INFO_PLACEHOLDER = "placeholder";
constexpr const char* PAGE_NODE_INFO_PASSWORD_RULES = "passwordRules";
constexpr const char* PAGE_NODE_INFO_ENABLE_AUTO_FILL = "enableAutoFill";
constexpr const char* PAGE_NODE_INFO_RECT = "rect";
constexpr const char* PAGE_NODE_INFO_IS_FOCUS = "isFocus";

void PageNodeInfo::FromJsonString(const std::string& jsonStr)
{
    nlohmann::json jsonObject = nlohmann::json::parse(jsonStr, nullptr, false);
    if (jsonObject.is_discarded()) {
        ABILITYBASE_LOGE("Failed to parse json string.");
        return;
    }
    if (jsonObject.contains(PAGE_NODE_INFO_ID) && jsonObject[PAGE_NODE_INFO_ID].is_number()) {
        id = jsonObject.at(PAGE_NODE_INFO_ID).get<int32_t>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_DEPTH) && jsonObject[PAGE_NODE_INFO_DEPTH].is_number()) {
        depth = jsonObject.at(PAGE_NODE_INFO_DEPTH).get<int32_t>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_AUTO_FILL_TYPE) && jsonObject[PAGE_NODE_INFO_AUTO_FILL_TYPE].is_number()) {
        autoFillType = static_cast<AutoFillType>(jsonObject.at(PAGE_NODE_INFO_AUTO_FILL_TYPE).get<int32_t>());
    }
    if (jsonObject.contains(PAGE_NODE_INFO_TAG) && jsonObject[PAGE_NODE_INFO_TAG].is_string()) {
        tag = jsonObject.at(PAGE_NODE_INFO_TAG).get<std::string>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_VALUE) && jsonObject[PAGE_NODE_INFO_VALUE].is_string()) {
        value = jsonObject.at(PAGE_NODE_INFO_VALUE).get<std::string>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_PLACEHOLDER) && jsonObject[PAGE_NODE_INFO_PLACEHOLDER].is_string()) {
        placeholder = jsonObject.at(PAGE_NODE_INFO_PLACEHOLDER).get<std::string>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_PASSWORD_RULES) && jsonObject[PAGE_NODE_INFO_PASSWORD_RULES].is_string()) {
        passwordRules = jsonObject.at(PAGE_NODE_INFO_PASSWORD_RULES).get<std::string>();
    }
    if (jsonObject.contains(PAGE_NODE_INFO_ENABLE_AUTO_FILL) &&
        jsonObject[PAGE_NODE_INFO_ENABLE_AUTO_FILL].is_boolean()) {
        enableAutoFill = jsonObject.at(PAGE_NODE_INFO_ENABLE_AUTO_FILL).get<bool>();
    }
    ParseJsonToPageNodeInfo(jsonObject);
}

void PageNodeInfo::ParseJsonToPageNodeInfo(const nlohmann::json& jsonObject)
{
    if (jsonObject.is_discarded()) {
        ABILITYBASE_LOGE("Failed to parse json string.");
        return;
    }
    if (jsonObject.contains(PAGE_NODE_INFO_RECT)) {
        rect.FromJsonString(jsonObject[PAGE_NODE_INFO_RECT]);
    }
    if (jsonObject.contains(PAGE_NODE_INFO_IS_FOCUS) && jsonObject[PAGE_NODE_INFO_IS_FOCUS].is_boolean()) {
        isFocus = jsonObject.at(PAGE_NODE_INFO_IS_FOCUS).get<bool>();
    }
}

std::string PageNodeInfo::ToJsonString() const
{
    nlohmann::json jsonObject {
        {PAGE_NODE_INFO_ID, id},
        {PAGE_NODE_INFO_DEPTH, depth},
        {PAGE_NODE_INFO_AUTO_FILL_TYPE, static_cast<int32_t>(autoFillType)},
        {PAGE_NODE_INFO_TAG, tag},
        {PAGE_NODE_INFO_VALUE, value},
        {PAGE_NODE_INFO_PLACEHOLDER, placeholder},
        {PAGE_NODE_INFO_PASSWORD_RULES, passwordRules},
        {PAGE_NODE_INFO_ENABLE_AUTO_FILL, enableAutoFill},
        {PAGE_NODE_INFO_RECT, rect.ToJsonString()},
        {PAGE_NODE_INFO_IS_FOCUS, isFocus}
    };
    return jsonObject.dump();
}
}  // namespace AbilityBase
}  // namespace OHOS