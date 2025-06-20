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
constexpr const char* PAGE_NODE_INFO_META_DATA = "metadata";
constexpr const char* PAGE_NODE_INFO_ENABLE_AUTO_FILL = "enableAutoFill";
constexpr const char* PAGE_NODE_INFO_RECT = "rect";
constexpr const char* PAGE_NODE_INFO_IS_FOCUS = "isFocus";

void PageNodeInfo::FromJsonString(const std::string& jsonStr)
{
    cJSON *jsonObject = cJSON_Parse(jsonStr.c_str());
    if (jsonObject == nullptr) {
        ABILITYBASE_LOGE("json parse failed");
        return;
    }
    cJSON *idItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_ID);
    if (idItem != nullptr && cJSON_IsNumber(idItem)) {
        id = static_cast<int32_t>(idItem->valueint);
    }
    cJSON *depthItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_DEPTH);
    if (depthItem != nullptr && cJSON_IsNumber(depthItem)) {
        depth = static_cast<int32_t>(depthItem->valueint);
    }
    cJSON *autoFillTypeItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_AUTO_FILL_TYPE);
    if (autoFillTypeItem != nullptr && cJSON_IsNumber(autoFillTypeItem)) {
        autoFillType = static_cast<AutoFillType>(static_cast<int32_t>(autoFillTypeItem->valueint));
    }
    cJSON *tagItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_TAG);
    if (tagItem != nullptr && cJSON_IsString(tagItem)) {
        tag = tagItem->valuestring;
    }
    cJSON *valueItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_VALUE);
    if (valueItem != nullptr && cJSON_IsString(valueItem)) {
        value = valueItem->valuestring;
    }
    cJSON *placeholderItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_PLACEHOLDER);
    if (placeholderItem != nullptr && cJSON_IsString(placeholderItem)) {
        placeholder = placeholderItem->valuestring;
    }
    cJSON *passwordRulesItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_PASSWORD_RULES);
    if (passwordRulesItem != nullptr && cJSON_IsString(passwordRulesItem)) {
        passwordRules = passwordRulesItem->valuestring;
    }
    cJSON *metadataItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_META_DATA);
    if (metadataItem != nullptr && cJSON_IsString(metadataItem)) {
        metadata = metadataItem->valuestring;
    }
    cJSON *enableAutoFillItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_ENABLE_AUTO_FILL);
    if (enableAutoFillItem != nullptr && cJSON_IsBool(enableAutoFillItem)) {
        enableAutoFill = enableAutoFillItem->type == cJSON_True ? true : false;
    }
    ParseJsonToPageNodeInfo(jsonObject);
    cJSON_Delete(jsonObject);
}

void PageNodeInfo::ParseJsonToPageNodeInfo(const cJSON *jsonObject)
{
    if (jsonObject == nullptr) {
        ABILITYBASE_LOGE("json parse failed");
        return;
    }
    cJSON *rectItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_RECT);
    if (rectItem != nullptr && cJSON_IsString(rectItem)) {
        std::string rectStr = rectItem->valuestring;
        rect.FromJsonString(rectStr);
    }
    cJSON *isFocusItem = cJSON_GetObjectItem(jsonObject, PAGE_NODE_INFO_IS_FOCUS);
    if (isFocusItem != nullptr && cJSON_IsBool(isFocusItem)) {
        isFocus = isFocusItem->type == cJSON_True ? true : false;
    }
}

std::string PageNodeInfo::ToJsonString() const
{
    cJSON *jsonObject = cJSON_CreateObject();
    if (jsonObject == nullptr) {
        return "";
    }
    cJSON_AddNumberToObject(jsonObject, PAGE_NODE_INFO_ID, static_cast<double>(id));
    cJSON_AddNumberToObject(jsonObject, PAGE_NODE_INFO_DEPTH, static_cast<double>(depth));
    cJSON_AddNumberToObject(jsonObject, PAGE_NODE_INFO_AUTO_FILL_TYPE, static_cast<double>(autoFillType));
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_TAG, tag.c_str());
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_VALUE, value.c_str());
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_PLACEHOLDER, placeholder.c_str());
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_PASSWORD_RULES, passwordRules.c_str());
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_META_DATA, metadata.c_str());
    cJSON_AddBoolToObject(jsonObject, PAGE_NODE_INFO_ENABLE_AUTO_FILL, enableAutoFill);
    cJSON_AddStringToObject(jsonObject, PAGE_NODE_INFO_RECT, rect.ToJsonString().c_str());
    cJSON_AddBoolToObject(jsonObject, PAGE_NODE_INFO_IS_FOCUS, isFocus);
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