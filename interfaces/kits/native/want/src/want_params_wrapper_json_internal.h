/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#ifndef OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_JSON_INTERNAL_H
#define OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_JSON_INTERNAL_H

#include <cstddef>
#include <cstdint>
#include <string>

#include "nlohmann/json.hpp"
#include "want_params_wrapper_json.h"

namespace OHOS {
namespace AAFwk {
namespace WantParamWrapperJson {
namespace Internal {
using Json = nlohmann::json;

// Maximum supported combined WantParams/Array JSON recursion depth. The
// top-level WantParams object starts at depth 0.
constexpr uint32_t MAX_RECURSION_DEPTH = 100;
constexpr size_t MAX_JSON_ARRAY_LENGTH = 50 * 1024 * 1024;

// Parses a canonical decimal typeId without redundant leading zeros, a leading plus sign, or whitespace.
bool ParseTypeId(const std::string &value, int &typeId);
// Restores a scalar JSON string and verifies that it already uses the canonical representation.
bool ParseScalarValueJson(int typeId, const Json &valueJson, sptr<IInterface> &value);
bool BuildParamsJson(
    const WantParams &params, Json &out, uint32_t depth, UnsupportedTypePolicy policy);
bool ParseParamsJson(
    const Json &jsonObject, WantParams &out, uint32_t depth, UnsupportedTypePolicy policy);
}  // namespace Internal
}  // namespace WantParamWrapperJson
}  // namespace AAFwk
}  // namespace OHOS

#endif  // OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_JSON_INTERNAL_H
