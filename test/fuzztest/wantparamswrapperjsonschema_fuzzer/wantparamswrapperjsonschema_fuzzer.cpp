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

#include "wantparamswrapperjsonschema_fuzzer.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "string_wrapper.h"
#include "want_params.h"
#include "want_params_wrapper_json.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t MAX_INPUT_SIZE = 4096;
constexpr size_t MAX_FRAGMENT_SIZE = 256;
constexpr size_t SELECTOR_SIZE = 1;
constexpr unsigned char HIGH_NIBBLE_SHIFT = 4;
constexpr unsigned char NIBBLE_MASK = 0x0F;
constexpr unsigned char JSON_CONTROL_CHAR_LIMIT = 0x20;
constexpr unsigned char ASCII_DELETE = 0x7F;
constexpr char ENVELOPE_KEY[] = "ohos.want.paramsStringEnvelope";

std::string MakeString(const uint8_t *data, size_t size)
{
    size_t realSize = std::min(size, MAX_INPUT_SIZE);
    return std::string(reinterpret_cast<const char *>(data), realSize);
}

std::string HexByte(unsigned char value)
{
    constexpr char hex[] = "0123456789abcdef";
    std::string out;
    out.push_back(hex[(value >> HIGH_NIBBLE_SHIFT) & NIBBLE_MASK]);
    out.push_back(hex[value & NIBBLE_MASK]);
    return out;
}

std::string EscapeJsonString(const std::string &value)
{
    std::string out;
    for (unsigned char ch : value) {
        switch (ch) {
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case '\b':
                out += "\\b";
                break;
            case '\f':
                out += "\\f";
                break;
            case '\n':
                out += "\\n";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\t':
                out += "\\t";
                break;
            default:
                if (ch < JSON_CONTROL_CHAR_LIMIT || ch >= ASCII_DELETE) {
                    out += "\\u00";
                    out += HexByte(ch);
                } else {
                    out.push_back(static_cast<char>(ch));
                }
                break;
        }
    }
    return out;
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

void ExerciseSchemaMutations(const std::string &raw, uint8_t selector)
{
    std::string fragment = raw.substr(0, MAX_FRAGMENT_SIZE);
    std::string escaped = EscapeJsonString(fragment);
    std::string envelopePrefix = "{\"";
    envelopePrefix += ENVELOPE_KEY;
    envelopePrefix += "\":";

    std::vector<std::string> candidates = {
        raw,
        "   " + raw,
        envelopePrefix + fragment + "}",
        envelopePrefix + "{\"k\":" + fragment + "}}",
        envelopePrefix + "{\"k\":{\"9\":" + fragment + "}}}",
        envelopePrefix + "{\"k\":{\"101\":" + fragment + "}}}",
        envelopePrefix + "{\"" + escaped + "\":{\"9\":\"" + escaped + "\"}}}",
        envelopePrefix + "{\"k\":{\"9\":\"" + escaped + "\"}}}",
        envelopePrefix + "{\"k\":{\"101\":{\"child\":{\"9\":\"" + escaped + "\"}}}}}",
        envelopePrefix + "{\"k\":{\"102\":\"" + escaped + "\"}}}",
        envelopePrefix + "{\"k\":{\"102\":{\"101\":[{\"v\":{\"9\":\"" + escaped + "\"}}]}}}}",
        envelopePrefix + "{\"k\":{\"102\":{\"102\":[{\"101\":[]}]}}}}",
        envelopePrefix + "{\"k\":{\"102\":{\"101\":" + fragment + "}}}}",
        envelopePrefix + "{\"k\":{\"102\":{\"elementType\":101,\"items\":[]}}}}",
        "{\"x\":{\"ohos.want.params.json\":{}}}",
        "{\"ohos.want.params.json.extra\":{}}",
        "{\"ohos.want.params.json\":{\"k\":{\"9\":\"v\"}}}{\"ohos.want.params.json\":{}}",
        "   {\"'\"y\"102\"{\"[8888888[0{ m}\"!};}",
    };

    size_t firstIdx = selector % candidates.size();
    ExerciseParse(candidates[firstIdx]);
    size_t secondIdx = (firstIdx + 1 + (selector >> HIGH_NIBBLE_SHIFT)) % candidates.size();
    if (secondIdx != firstIdx) {
        ExerciseParse(candidates[secondIdx]);
    }
}
} // namespace

bool DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size < SELECTOR_SIZE) {
        return false;
    }

    const uint8_t *payload = data + SELECTOR_SIZE;
    size_t payloadSize = size - SELECTOR_SIZE;
    if (payloadSize > MAX_INPUT_SIZE) {
        return false;
    }

    ExerciseSchemaMutations(MakeString(payload, payloadSize), data[0]);
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
