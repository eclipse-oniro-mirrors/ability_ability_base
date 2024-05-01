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

#ifndef OHOS_ABILITY_BASE_VIEW_DATA_H
#define OHOS_ABILITY_BASE_VIEW_DATA_H

#include <string>
#include <vector>

#include "page_node_info.h"

namespace OHOS {
namespace AbilityBase {
struct ViewData {
    // nodes has a limit size
    void FromJsonString(const std::string& jsonStr);
    // nodes has a limit size
    std::string ToJsonString() const;

    std::string bundleName;
    std::string moduleName;
    std::string abilityName;
    std::string pageUrl;
    std::vector<PageNodeInfo> nodes;
    Rect pageRect;
    bool isUserSelected = false;
    bool isOtherAccount = false;
};
}  // namespace AbilityBase
}  // namespace OHOS
#endif // OHOS_ABILITY_BASE_VIEW_DATA_H
