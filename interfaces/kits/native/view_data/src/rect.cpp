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

#include "rect.h"

#include "ability_base_log_wrapper.h"
#include "nlohmann/json.hpp"

namespace OHOS {
namespace AbilityBase {
constexpr const char* VIEW_DATA_RECT_LEFT = "left";
constexpr const char* VIEW_DATA_RECT_TOP = "top";
constexpr const char* VIEW_DATA_RECT_WIDTH = "width";
constexpr const char* VIEW_DATA_RECT_HEIGHT = "height";

void Rect::FromJsonString(const std::string& jsonStr)
{
    nlohmann::json jsonObject = nlohmann::json::parse(jsonStr, nullptr, false);
    if (jsonObject.is_discarded()) {
        ABILITYBASE_LOGE("Failed to parse json string.");
        return;
    }
    if (jsonObject.contains(VIEW_DATA_RECT_LEFT) && jsonObject[VIEW_DATA_RECT_LEFT].is_number_float()) {
        left = jsonObject.at(VIEW_DATA_RECT_LEFT).get<float>();
    }
    if (jsonObject.contains(VIEW_DATA_RECT_TOP) && jsonObject[VIEW_DATA_RECT_TOP].is_number_float()) {
        top = jsonObject.at(VIEW_DATA_RECT_TOP).get<float>();
    }
    if (jsonObject.contains(VIEW_DATA_RECT_WIDTH) && jsonObject[VIEW_DATA_RECT_WIDTH].is_number_float()) {
        width = jsonObject.at(VIEW_DATA_RECT_WIDTH).get<float>();
    }
    if (jsonObject.contains(VIEW_DATA_RECT_HEIGHT) && jsonObject[VIEW_DATA_RECT_HEIGHT].is_number_float()) {
        height = jsonObject.at(VIEW_DATA_RECT_HEIGHT).get<float>();
    }
}

std::string Rect::ToJsonString() const
{
    nlohmann::json jsonObject {
        {VIEW_DATA_RECT_LEFT, left},
        {VIEW_DATA_RECT_TOP, top},
        {VIEW_DATA_RECT_WIDTH, width},
        {VIEW_DATA_RECT_HEIGHT, height}
    };
    return jsonObject.dump();
}
}  // namespace AbilityBase
}  // namespace OHOS