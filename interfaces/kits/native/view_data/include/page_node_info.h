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

#ifndef OHOS_ABILITY_BASE_PAGE_NODE_INFO_H
#define OHOS_ABILITY_BASE_PAGE_NODE_INFO_H

#include <string>

#include "auto_fill_type.h"
#include "nlohmann/json.hpp"
#include "rect.h"

namespace OHOS {
namespace AbilityBase {
struct PageNodeInfo {
    void FromJsonString(const std::string& jsonStr);
    void ParseJsonToPageNodeInfo(const nlohmann::json& jsonObject);
    std::string ToJsonString() const;

    int32_t id = -1;
    int32_t depth = -1;
    AutoFillType autoFillType = AutoFillType::UNSPECIFIED;
    std::string tag;
    std::string value;
    std::string placeholder;
    std::string metadata;
    std::string passwordRules;
    bool enableAutoFill = true;
    Rect rect;
    bool isFocus = false;
};
}  // namespace AbilityBase
}  // namespace OHOS
#endif // OHOS_ABILITY_BASE_PAGE_NODE_INFO_H