/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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
#include "cJSON.h"

namespace OHOS {
namespace AbilityBase {
constexpr size_t NODES_SIZE_LIMIT = 20;
constexpr const char* VIEW_DATA_BUNDLE_NAME = "bundleName";
constexpr const char* VIEW_DATA_MODULE_NAME = "moduleName";
constexpr const char* VIEW_DATA_ABILITY_NAME = "abilityName";
constexpr const char* VIEW_DATA_PAGE_URL = "pageUrl";
constexpr const char* VIEW_DATA_NODES = "nodes";
constexpr const char* VIEW_DATA_PAGE_RECT = "pageRect";
constexpr const char* VIEW_DATA_USER_SELECTED = "isUserSelected";
constexpr const char* VIEW_DATA_OTHER_ACCOUNT = "isOtherAccount";

void ViewData::FromJsonString(const std::string& jsonStr)
{
    cJSON *jsonObject = cJSON_Parse(jsonStr.c_str());
    if (jsonObject == nullptr) {
        ABILITYBASE_LOGE("json parse failed");
        return;
    }
    cJSON *bundleNameItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_BUNDLE_NAME);
    if (bundleNameItem != nullptr && cJSON_IsString(bundleNameItem)) {
        bundleName = bundleNameItem->valuestring;
    }
    cJSON *moduleNameItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_MODULE_NAME);
    if (moduleNameItem != nullptr && cJSON_IsString(moduleNameItem)) {
        moduleName = moduleNameItem->valuestring;
    }
    cJSON *abilityNameItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_ABILITY_NAME);
    if (abilityNameItem != nullptr && cJSON_IsString(abilityNameItem)) {
        abilityName = abilityNameItem->valuestring;
    }
    cJSON *pageUrlItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_PAGE_URL);
    if (pageUrlItem != nullptr && cJSON_IsString(pageUrlItem)) {
        pageUrl = pageUrlItem->valuestring;
    }
    cJSON *userSelectedItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_USER_SELECTED);
    if (userSelectedItem != nullptr && cJSON_IsBool(userSelectedItem)) {
        isUserSelected = userSelectedItem->type == cJSON_True ? true : false;
    }
    cJSON *otherAccountItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_OTHER_ACCOUNT);
    if (otherAccountItem != nullptr && cJSON_IsBool(otherAccountItem)) {
        isOtherAccount = otherAccountItem->type == cJSON_True ? true : false;
    }
    cJSON *nodesItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_NODES);
    if (nodesItem != nullptr && cJSON_IsArray(nodesItem)) {
        nodes.clear();
        int size = cJSON_GetArraySize(nodesItem);
        if (size > (int)NODES_SIZE_LIMIT) {
            size = (int)NODES_SIZE_LIMIT;
        }
        for (int i = 0; i < size; i++) {
            cJSON *nodeItem = cJSON_GetArrayItem(nodesItem, i);
            if (nodeItem != nullptr && cJSON_IsString(nodeItem)) {
                PageNodeInfo pageNodeInfo;
                std::string pageNodeInfoStr = nodeItem->valuestring;
                pageNodeInfo.FromJsonString(pageNodeInfoStr);
                nodes.emplace_back(pageNodeInfo);
            }
        }
    }
    cJSON *pageRectItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_PAGE_RECT);
    if (pageRectItem != nullptr && cJSON_IsString(pageRectItem)) {
        std::string pageRectStr = pageRectItem->valuestring;
        pageRect.FromJsonString(pageRectStr);
    }
    cJSON_Delete(jsonObject);
}

std::string ViewData::ToJsonString() const
{
    cJSON *jsonObject = cJSON_CreateObject();
    if (jsonObject == nullptr) {
        return "";
    }
    cJSON *jsonNodes = cJSON_CreateArray();
    if (jsonNodes == nullptr) {
        cJSON_Delete(jsonObject);
        return "";
    }
    auto size = nodes.size() > NODES_SIZE_LIMIT ? NODES_SIZE_LIMIT : nodes.size();
    for (size_t i = 0; i < size; i++) {
        cJSON_AddItemToArray(jsonNodes, cJSON_CreateString(nodes[i].ToJsonString().c_str()));
    }
    cJSON_AddStringToObject(jsonObject, VIEW_DATA_BUNDLE_NAME, bundleName.c_str());
    cJSON_AddStringToObject(jsonObject, VIEW_DATA_MODULE_NAME, moduleName.c_str());
    cJSON_AddStringToObject(jsonObject, VIEW_DATA_ABILITY_NAME, abilityName.c_str());
    cJSON_AddStringToObject(jsonObject, VIEW_DATA_PAGE_URL, pageUrl.c_str());
    cJSON_AddItemToObject(jsonObject, VIEW_DATA_NODES, jsonNodes);
    cJSON_AddBoolToObject(jsonObject, VIEW_DATA_USER_SELECTED, isUserSelected);
    cJSON_AddBoolToObject(jsonObject, VIEW_DATA_OTHER_ACCOUNT, isOtherAccount);
    cJSON_AddStringToObject(jsonObject, VIEW_DATA_PAGE_RECT, pageRect.ToJsonString().c_str());
    char *str = cJSON_PrintUnformatted(jsonObject);
    cJSON_Delete(jsonObject);
    if (str == nullptr) {
        return "";
    }
    std::string jsonStr(str);
    cJSON_free(str);
    return jsonStr;
}
}  // namespace AbilityBase
}  // namespace OHOS