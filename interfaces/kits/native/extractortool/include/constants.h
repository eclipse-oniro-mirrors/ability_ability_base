/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_ABILITY_BASE_CONSTANTS_H
#define OHOS_ABILITY_BASE_CONSTANTS_H

namespace OHOS {
namespace AbilityBase {
namespace Constants {
constexpr const char* ABS_CODE_PATH = "/data/app/el1/bundle/public";
constexpr const char* LOCAL_CODE_PATH = "/data/storage/el1/bundle";
constexpr const char* LOCAL_BUNDLES = "/data/bundles";
constexpr const char* FILE_SEPARATOR = "/";
constexpr const char* FILE_DOT = "\\.";

constexpr const char* COMPRESS_PROPERTY = "compress";

inline std::string GetProcPrefix()
{
    return "/proc/";
}
}  // namespace Constants
}  // namespace AbilityBase
}  // namespace OHOS
#endif  // OHOS_ABILITY_BASE_CONSTANTS_H