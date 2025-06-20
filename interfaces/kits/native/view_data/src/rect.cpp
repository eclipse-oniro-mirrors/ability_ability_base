/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include "rect.h"

#include "ability_base_log_wrapper.h"
#include "cJSON.h"

namespace OHOS {
namespace AbilityBase {
constexpr const char* VIEW_DATA_RECT_LEFT = "left";
constexpr const char* VIEW_DATA_RECT_TOP = "top";
constexpr const char* VIEW_DATA_RECT_WIDTH = "width";
constexpr const char* VIEW_DATA_RECT_HEIGHT = "height";

void Rect::FromJsonString(const std::string& jsonStr)
{
    cJSON *jsonObject = cJSON_Parse(jsonStr.c_str());
    if (jsonObject == nullptr) {
        ABILITYBASE_LOGE("json parse failed");
        return;
    }
    cJSON *rectLeftItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_RECT_LEFT);
    if (rectLeftItem != nullptr && cJSON_IsNumber(rectLeftItem)) {
        left = static_cast<float>(rectLeftItem->valuedouble);
    }
    cJSON *rectTopItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_RECT_TOP);
    if (rectTopItem != nullptr && cJSON_IsNumber(rectTopItem)) {
        top = static_cast<float>(rectTopItem->valuedouble);
    }
    cJSON *rectWidthItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_RECT_WIDTH);
    if (rectWidthItem != nullptr && cJSON_IsNumber(rectWidthItem)) {
        width = static_cast<float>(rectWidthItem->valuedouble);
    }
    cJSON *rectHeightItem = cJSON_GetObjectItem(jsonObject, VIEW_DATA_RECT_HEIGHT);
    if (rectHeightItem != nullptr && cJSON_IsNumber(rectHeightItem)) {
        height = static_cast<float>(rectHeightItem->valuedouble);
    }
    cJSON_Delete(jsonObject);
}

std::string Rect::ToJsonString() const
{
    cJSON *jsonObject = cJSON_CreateObject();
    if (jsonObject == nullptr) {
        return "";
    }
    cJSON_AddNumberToObject(jsonObject, VIEW_DATA_RECT_LEFT, static_cast<double>(left));
    cJSON_AddNumberToObject(jsonObject, VIEW_DATA_RECT_TOP, static_cast<double>(top));
    cJSON_AddNumberToObject(jsonObject, VIEW_DATA_RECT_WIDTH, static_cast<double>(width));
    cJSON_AddNumberToObject(jsonObject, VIEW_DATA_RECT_HEIGHT, static_cast<double>(height));
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