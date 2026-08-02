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
constexpr int EXERCISE_ROUTE_COUNT = 4;
constexpr int EXERCISE_ROUTE_PARSE = 0;
constexpr int EXERCISE_ROUTE_SCHEMA_MUTATIONS = 1;
constexpr int EXERCISE_ROUTE_SERIALIZE_ROUND_TRIP = 2;
constexpr size_t ROUTE_SELECTOR_INDEX = 0;
constexpr size_t SCENARIO_SELECTOR_INDEX = 1;
constexpr size_t PAYLOAD_OFFSET = 2;
constexpr size_t KEY_NAME_VARIANT = 17;
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

    // Run only 1-2 candidates per input, selected by the fuzz byte, so each
    // invocation stays cheap while coverage-guided fuzzing still reaches every
    // candidate across the corpus.
    size_t firstIdx = selector % candidates.size();
    ExerciseParse(candidates[firstIdx]);
    size_t secondIdx = (firstIdx + 1 + (selector >> HIGH_NIBBLE_SHIFT)) % candidates.size();
    if (secondIdx != firstIdx) {
        ExerciseParse(candidates[secondIdx]);
    }
}

void ExerciseSerializeRoundTrip(const uint8_t *data, size_t size)
{
    std::string value = MakeString(data, size);
    std::string key = "k_" + std::to_string(static_cast<unsigned int>(size % KEY_NAME_VARIANT));

    WantParams child;
    child.SetParam("child", String::Box(value));
    child.SetParam("flag", Boolean::Box((size & 1U) != 0));

    WantParams wp;
    wp.SetParam(key, String::Box(value));
    wp.SetParam("int", Integer::Box(MakeInt32(data, size)));
    wp.SetParam("double", Double::Box(static_cast<double>(MakeInt32(data, size))));
    wp.SetParam("nested", WantParamWrapper::Box(std::move(child)));

    WantParams arrayChild;
    arrayChild.SetParam("value", String::Box(value));
    sptr<IArray> wantParamsArray = new Array(1, g_IID_IWantParams);
    if (wantParamsArray != nullptr) {
        wantParamsArray->Set(0, WantParamWrapper::Box(std::move(arrayChild)));
        wp.SetParam("wantParamsArray", wantParamsArray);

        sptr<IArray> nestedArray = new Array(1, g_IID_IArray);
        if (nestedArray != nullptr) {
            nestedArray->Set(0, wantParamsArray);
            wp.SetParam("nestedArray", nestedArray);
        }
    }

    sptr<IArray> unsupportedArray = new Array(ARRAY_STRING_SIZE, g_IID_IString);
    if (unsupportedArray != nullptr) {
        unsupportedArray->Set(0, String::Box(value));
        unsupportedArray->Set(1, String::Box(EscapeJsonString(value)));
        WantParams unsupportedParams;
        unsupportedParams.SetParam("array", unsupportedArray);
        std::string unchanged = "unchanged";
        bool serializeResult = WantParamWrapperJson::Serialize(unsupportedParams, unchanged);
        if (serializeResult || unchanged != "unchanged") {
            __builtin_trap();
        }
    }

    std::string serialized;
    if (!WantParamWrapperJson::Serialize(wp, serialized)) {
        return;
    }
    ExerciseParse(serialized);
}

void ExerciseDepth(const uint8_t *data, size_t size, uint8_t selector)
{
    int depth = static_cast<int>(selector % (MAX_FUZZ_DEPTH + 1));
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
    if (data == nullptr || size == 0) {
        return false;
    }

    // Let corpus files contain a complete JSON document. Without this fast
    // path, the two selector bytes would remove the leading characters from a
    // valid JSON seed before it reaches Parse().
    if (data[0] == '{' && size <= MAX_INPUT_SIZE) {
        ExerciseParse(MakeString(data, size, MAX_INPUT_SIZE));
        return true;
    }

    if (size < PAYLOAD_OFFSET) {
        return false;
    }

    const uint8_t *payload = data + PAYLOAD_OFFSET;
    size_t payloadSize = size - PAYLOAD_OFFSET;
    if (payloadSize > MAX_INPUT_SIZE) {
        return false;
    }
    std::string raw = MakeString(payload, payloadSize, MAX_INPUT_SIZE);

    // Keep routing and scenario selection separate from the payload so control
    // bytes do not constrain the first byte of JSON or serialized values.
    switch (data[ROUTE_SELECTOR_INDEX] % EXERCISE_ROUTE_COUNT) {
        case EXERCISE_ROUTE_PARSE:
            ExerciseParse(raw);
            break;
        case EXERCISE_ROUTE_SCHEMA_MUTATIONS:
            ExerciseSchemaMutations(raw, data[SCENARIO_SELECTOR_INDEX]);
            break;
        case EXERCISE_ROUTE_SERIALIZE_ROUND_TRIP:
            ExerciseSerializeRoundTrip(payload, payloadSize);
            break;
        default:
            ExerciseDepth(payload, payloadSize, data[SCENARIO_SELECTOR_INDEX]);
            break;
    }
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
