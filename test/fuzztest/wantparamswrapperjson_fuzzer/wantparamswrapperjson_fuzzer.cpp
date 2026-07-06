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

#include "wantparamswrapperjson_fuzzer.h"

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "array_wrapper.h"
#include "bool_wrapper.h"
#include "double_wrapper.h"
#include "int_wrapper.h"
#include "string_wrapper.h"
#include "want_params.h"
#include "want_params_wrapper.h"
#include "want_params_wrapper_json.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t MAX_INPUT_SIZE = 4096;
constexpr size_t MAX_FRAGMENT_SIZE = 256;
constexpr int MAX_FUZZ_DEPTH = 104;
constexpr uint32_t BYTE_SHIFT = CHAR_BIT;
constexpr unsigned char HIGH_NIBBLE_SHIFT = CHAR_BIT / 2;
constexpr unsigned char NIBBLE_MASK = (1U << HIGH_NIBBLE_SHIFT) - 1U;
constexpr unsigned char JSON_CONTROL_CHAR_LIMIT = 0x20;
constexpr unsigned char ASCII_DELETE = 0x7F;
constexpr int ARRAY_STRING_SIZE = 2;
constexpr char ENVELOPE_KEY[] = "ohos.want.paramsStringEnvelope";

std::string MakeString(const uint8_t *data, size_t size, size_t maxSize = MAX_FRAGMENT_SIZE)
{
    size_t realSize = std::min(size, maxSize);
    return std::string(reinterpret_cast<const char *>(data), realSize);
}

int32_t MakeInt32(const uint8_t *data, size_t size)
{
    uint32_t value = 0;
    size_t realSize = std::min<size_t>(size, sizeof(value));
    for (size_t i = 0; i < realSize; ++i) {
        value = (value << BYTE_SHIFT) | data[i];
    }
    return static_cast<int32_t>(value);
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

void ExerciseSchemaMutations(const std::string &raw)
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
        "{\"x\":{\"ohos.want.params.json\":{}}}",
        "{\"ohos.want.params.json.extra\":{}}",
        "{\"ohos.want.params.json\":{\"k\":{\"9\":\"v\"}}}{\"ohos.want.params.json\":{}}",
        "   {\"'\"y\"102\"{\"[8888888[0{ m}\"!};}",
    };

    for (const auto &candidate : candidates) {
        ExerciseParse(candidate);
    }
}

void ExerciseSerializeRoundTrip(const uint8_t *data, size_t size)
{
    std::string value = MakeString(data, size);
    std::string key = "k_" + std::to_string(static_cast<unsigned int>(size % 17));

    WantParams child;
    child.SetParam("child", String::Box(value));
    child.SetParam("flag", Boolean::Box((size & 1U) != 0));

    WantParams wp;
    wp.SetParam(key, String::Box(value));
    wp.SetParam("int", Integer::Box(MakeInt32(data, size)));
    wp.SetParam("double", Double::Box(static_cast<double>(MakeInt32(data, size))));
    wp.SetParam("nested", WantParamWrapper::Box(std::move(child)));

    sptr<IArray> array = new Array(ARRAY_STRING_SIZE, g_IID_IString);
    if (array != nullptr) {
        array->Set(0, String::Box(value));
        array->Set(1, String::Box(EscapeJsonString(value)));
        wp.SetParam("array", array);
    }

    std::string serialized;
    if (!WantParamWrapperJson::Serialize(wp, serialized)) {
        return;
    }
    ExerciseParse(serialized);
}

void ExerciseDepth(const uint8_t *data, size_t size)
{
    int depth = size == 0 ? 0 : static_cast<int>(data[0] % (MAX_FUZZ_DEPTH + 1));
    WantParams inner;
    inner.SetParam("leaf", String::Box(MakeString(data, size)));
    for (int i = 0; i < depth; ++i) {
        WantParams outer;
        outer.SetParam("lv", WantParamWrapper::Box(std::move(inner)));
        inner = std::move(outer);
    }

    std::string serialized;
    if (WantParamWrapperJson::Serialize(inner, serialized)) {
        ExerciseParse(serialized);
    }
}

} // namespace

bool DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size > MAX_INPUT_SIZE) {
        return false;
    }

    std::string raw = MakeString(data, size, MAX_INPUT_SIZE);
    ExerciseParse(raw);
    ExerciseSchemaMutations(raw);
    ExerciseSerializeRoundTrip(data, size);
    ExerciseDepth(data, size);
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
