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

#include "wantparamswrapperjsonparse_fuzzer.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>

#include "string_wrapper.h"
#include "want_params.h"
#include "want_params_wrapper_json.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t MAX_INPUT_SIZE = 4096;

std::string MakeString(const uint8_t *data, size_t size)
{
    size_t realSize = std::min(size, MAX_INPUT_SIZE);
    return std::string(reinterpret_cast<const char *>(data), realSize);
}

void ExerciseParse(const std::string &text)
{
    WantParamWrapperJson::HasEnvelope(text);

    WantParams out;
    out.SetParam("sentinel", String::Box("keep"));
    if (!WantParamWrapperJson::Parse(text, out)) {
        return;
    }

    std::string serialized;
    if (!WantParamWrapperJson::Serialize(out, serialized)) {
        return;
    }

    WantParamWrapperJson::HasEnvelope(serialized);
    WantParams reparsed;
    WantParamWrapperJson::Parse(serialized, reparsed);
}
} // namespace

bool DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size > MAX_INPUT_SIZE) {
        return false;
    }

    ExerciseParse(MakeString(data, size));
    return true;
}
} // namespace OHOS

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (data == nullptr) {
        return 0;
    }
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
