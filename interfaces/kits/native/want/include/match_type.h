/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_ABILITY_BASE_MATCH_TYPE_H
#define OHOS_ABILITY_BASE_MATCH_TYPE_H

#include <cstdint>
namespace OHOS {
namespace AAFwk {
enum class MatchType : uint8_t {
    DEFAULT = 0,
    PREFIX = 1,
    PATTERN = 2,
    GLOBAL = 3,
};
}
}  // namespace OHOS

#endif  // OHOS_ABILITY_BASE_MATCH_TYPE_H
