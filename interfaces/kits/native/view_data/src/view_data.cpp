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

#include "view_data.h"

#include "ability_base_log_wrapper.h"
#include "nlohmann/json.hpp"

namespace OHOS {
namespace AbilityBase {
constexpr size_t NODES_SIZE_LIMIT = 20;
constexpr const char* VIEW_DATA_BUNDLE_NAME = "bundleName";
constexpr const char* VIEW_DATA_MODULE_NAME = "moduleName";
constexpr const char* VIEW_DATA_ABILITY_NAME = "abilityName";
constexpr const char* VIEW_DATA_PAGE_URL = "pageUrl";
constexpr const char* VIEW_DATA_NODES = "nodes";

void ViewData::FromJsonString(const std::string& jsonStr)
{
    nlohmann::json jsonObject = nlohmann::json::parse(jsonStr, nullptr, false);
    if (jsonObject.is_discarded()) {
        ABILITYBASE_LOGE("Failed to parse json string.");
        return;
    }
    if (jsonObject.contains(VIEW_DATA_BUNDLE_NAME) && jsonObject[VIEW_DATA_BUNDLE_NAME].is_string()) {
        bundleName = jsonObject.at(VIEW_DATA_BUNDLE_NAME).get<std::string>();
    }
    if (jsonObject.contains(VIEW_DATA_MODULE_NAME) && jsonObject[VIEW_DATA_MODULE_NAME].is_string()) {
        moduleName = jsonObject.at(VIEW_DATA_MODULE_NAME).get<std::string>();
    }
    if (jsonObject.contains(VIEW_DATA_ABILITY_NAME) && jsonObject[VIEW_DATA_ABILITY_NAME].is_string()) {
        abilityName = jsonObject.at(VIEW_DATA_ABILITY_NAME).get<std::string>();
    }
    if (jsonObject.contains(VIEW_DATA_PAGE_URL) && jsonObject[VIEW_DATA_PAGE_URL].is_string()) {
        pageUrl = jsonObject.at(VIEW_DATA_PAGE_URL).get<std::string>();
    }
    if (jsonObject.contains(VIEW_DATA_NODES) && jsonObject[VIEW_DATA_NODES].is_array()) {
        nodes.clear();
        auto size = jsonObject[VIEW_DATA_NODES].size() > NODES_SIZE_LIMIT ? NODES_SIZE_LIMIT :
            jsonObject[VIEW_DATA_NODES].size();
        for (size_t i = 0; i < size; i++) {
            if (jsonObject[VIEW_DATA_NODES][i].is_string()) {
                PageNodeInfo pageNodeInfo;
                pageNodeInfo.FromJsonString(jsonObject[VIEW_DATA_NODES][i]);
                nodes.emplace_back(pageNodeInfo);
            }
        }
    }
}

std::string ViewData::ToJsonString() const
{
    nlohmann::json jsonNodes = nlohmann::json::array();
    auto size = nodes.size() > NODES_SIZE_LIMIT ? NODES_SIZE_LIMIT : nodes.size();
    for (size_t i = 0; i < size; i++) {
        jsonNodes.emplace_back(nodes[i].ToJsonString());
    }
    nlohmann::json jsonObject {
        {VIEW_DATA_BUNDLE_NAME, bundleName},
        {VIEW_DATA_MODULE_NAME, moduleName},
        {VIEW_DATA_ABILITY_NAME, abilityName},
        {VIEW_DATA_PAGE_URL, pageUrl},
        {VIEW_DATA_NODES, jsonNodes}
    };
    return jsonObject.dump();
}
}  // namespace AbilityBase
}  // namespace OHOS