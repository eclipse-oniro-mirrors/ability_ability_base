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
#include "want_params_wrapper_json.h"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <new>
#include <string>
#include <utility>

#include "ability_base_log_wrapper.h"
#include "array_wrapper_json.h"
#include "nlohmann/json.hpp"
#include "want_params_wrapper.h"
#include "want_params_wrapper_json_internal.h"

namespace OHOS {
namespace AAFwk {
namespace WantParamWrapperJson {
namespace {
using Json = nlohmann::json;

constexpr int TYPE_WANT_PARAMS = 101;
constexpr int TYPE_ARRAY = 102;
constexpr int TYPE_NULL = -1;
constexpr int TYPE_SCALAR_MIN = 1;
constexpr int TYPE_SCALAR_MAX = 9;

bool IsSupportedTypeId(int typeId)
{
    // Support is defined by this JSON codec, not by the wider WantParams type registry.
    return (typeId >= TYPE_SCALAR_MIN && typeId <= TYPE_SCALAR_MAX) ||
        typeId == TYPE_WANT_PARAMS || typeId == TYPE_ARRAY;
}
}  // namespace

namespace Internal {
bool ParseTypeId(const std::string &value, int &typeId)
{
    errno = 0;
    char *end = nullptr;
    long parsed = strtol(value.c_str(), &end, 10);
    if (errno == ERANGE || end == value.c_str() || *end != '\0' || parsed > INT_MAX || parsed < INT_MIN) {
        return false;
    }
    int parsedTypeId = static_cast<int>(parsed);
    if (value != std::to_string(parsedTypeId)) {
        return false;
    }
    typeId = parsedTypeId;
    return true;
}

bool ParseScalarValueJson(int typeId, const Json &valueJson, sptr<IInterface> &value)
{
    if (typeId < TYPE_SCALAR_MIN || typeId > TYPE_SCALAR_MAX || !valueJson.is_string()) {
        return false;
    }
    std::string valueStr = valueJson.get<std::string>();
    sptr<IInterface> parsedValue = WantParams::GetInterfaceByType(typeId, valueStr);
    if (parsedValue == nullptr || WantParams::GetStringByType(parsedValue, typeId) != valueStr) {
        return false;
    }
    value = std::move(parsedValue);
    return true;
}

bool BuildParamsJson(const WantParams &wp, Json &out, uint32_t depth, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("serialize params enter, depth=%{public}u, inputCount=%{public}d",
        depth, wp.Size());
    if (depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("serialize failed, depth %{public}u exceeds max depth %{public}u",
            depth, MAX_RECURSION_DEPTH);
        return false;
    }

    // Each parameter is encoded as {"typeId": value}. Nested WantParams and
    // Array values are encoded as JSON objects, while scalar values are encoded
    // as strings.
    Json params = Json::object();
    for (const auto &it : wp.GetParams()) {
        if (it.second == nullptr) {
            continue;
        }
        int typeId = WantParams::GetDataType(it.second);
        if (typeId == TYPE_NULL) {
            if (policy == UnsupportedTypePolicy::SKIP) {
                ABILITYBASE_LOGW("serialize skipped unsupported parameter, key=%{public}s", it.first.c_str());
                continue;
            }
            ABILITYBASE_LOGW("serialize failed, unsupported parameter, key=%{public}s", it.first.c_str());
            return false;
        }

        Json typedValue = Json::object();
        IWantParams *nested = IWantParams::Query(it.second);
        IArray *nestedArray = IArray::Query(it.second);
        if (nested != nullptr) {
            WantParams child;
            ErrCode errCode = nested->GetValue(child);
            if (errCode != ERR_OK) {
                ABILITYBASE_LOGW("serialize failed, get nested WantParams failed, key=%{public}s, err=%{public}d",
                    it.first.c_str(), errCode);
                return false;
            }
            Json childJson;
            if (!BuildParamsJson(child, childJson, depth + 1, policy)) {
                ABILITYBASE_LOGD("serialize nested WantParams failed, key=%{public}s, depth=%{public}u",
                    it.first.c_str(), depth);
                return false;
            }
            typedValue[std::to_string(typeId)] = std::move(childJson);
        } else if (nestedArray != nullptr) {
            Json arrayJson;
            ArrayWrapperJson::ConvertResult result =
                ArrayWrapperJson::Serialize(nestedArray, arrayJson, depth + 1, policy);
            if (result == ArrayWrapperJson::ConvertResult::UNSUPPORTED &&
                policy == UnsupportedTypePolicy::SKIP) {
                ABILITYBASE_LOGW("serialize skipped unsupported array parameter, key=%{public}s",
                    it.first.c_str());
                continue;
            }
            if (result != ArrayWrapperJson::ConvertResult::SUCCESS) {
                if (result == ArrayWrapperJson::ConvertResult::UNSUPPORTED) {
                    ABILITYBASE_LOGW("serialize failed, unsupported array parameter, key=%{public}s",
                        it.first.c_str());
                } else {
                    ABILITYBASE_LOGD("serialize array parameter failed, key=%{public}s, depth=%{public}u",
                        it.first.c_str(), depth);
                }
                return false;
            }
            typedValue[std::to_string(typeId)] = std::move(arrayJson);
        } else {
            typedValue[std::to_string(typeId)] = WantParams::GetStringByType(it.second, typeId);
        }
        params[it.first] = std::move(typedValue);
    }

    out = std::move(params);
    ABILITYBASE_LOGD("serialize params exit, depth=%{public}u, result=success, "
        "inputCount=%{public}d, outputCount=%{public}zu", depth, wp.Size(), out.size());
    return true;
}
}  // namespace Internal

namespace {
bool ParseTypedValueJson(const Json &typedValue, WantParams &parsed, const std::string &key,
    uint32_t depth, UnsupportedTypePolicy policy)
{
    // A typed-value object represents one logical WantParams value, so it must
    // contain exactly one typeId member.
    if (!typedValue.is_object() || typedValue.size() != 1) {
        size_t memberCount = typedValue.is_object() ? typedValue.size() : 0;
        ABILITYBASE_LOGW("parse failed, invalid typed value, key=%{public}s, memberCount=%{public}zu",
            key.c_str(), memberCount);
        return false;
    }

    auto item = typedValue.begin();
    int typeId = 0;
    if (!Internal::ParseTypeId(item.key(), typeId)) {
        ABILITYBASE_LOGW("parse failed, invalid typeId, key=%{public}s, typeId=%{public}s",
            key.c_str(), item.key().c_str());
        return false;
    }
    if (!IsSupportedTypeId(typeId)) {
        if (policy == UnsupportedTypePolicy::SKIP) {
            ABILITYBASE_LOGW("parse skipped unsupported parameter, key=%{public}s, typeId=%{public}d",
                key.c_str(), typeId);
            return true;
        }
        ABILITYBASE_LOGW("parse failed, unsupported parameter, key=%{public}s, typeId=%{public}d",
            key.c_str(), typeId);
        return false;
    }

    if (typeId == TYPE_WANT_PARAMS) {
        WantParams child;
        if (!Internal::ParseParamsJson(item.value(), child, depth + 1, policy)) {
            ABILITYBASE_LOGD("parse nested WantParams failed, key=%{public}s, depth=%{public}u",
                key.c_str(), depth);
            return false;
        }
        sptr<IWantParams> value = WantParamWrapper::Box(std::move(child));
        if (value == nullptr) {
            ABILITYBASE_LOGE("parse failed, box nested WantParams failed, key=%{public}s", key.c_str());
            return false;
        }
        parsed.SetParam(key, value);
        return true;
    }

    if (typeId == TYPE_ARRAY) {
        sptr<IArray> value;
        Internal::ArrayWrapperJson::ConvertResult result =
            Internal::ArrayWrapperJson::Parse(item.value(), value, depth + 1, policy);
        if (result == Internal::ArrayWrapperJson::ConvertResult::UNSUPPORTED &&
            policy == UnsupportedTypePolicy::SKIP) {
            ABILITYBASE_LOGW("parse skipped unsupported array parameter, key=%{public}s", key.c_str());
            return true;
        }
        if (result != Internal::ArrayWrapperJson::ConvertResult::SUCCESS) {
            if (result == Internal::ArrayWrapperJson::ConvertResult::UNSUPPORTED) {
                ABILITYBASE_LOGW("parse failed, unsupported array parameter, key=%{public}s", key.c_str());
            } else {
                ABILITYBASE_LOGD("parse array parameter failed, key=%{public}s, depth=%{public}u",
                    key.c_str(), depth);
            }
            return false;
        }
        parsed.SetParam(key, value);
        return true;
    }

    if (!item.value().is_string()) {
        ABILITYBASE_LOGW("parse failed, scalar value is not string, key=%{public}s, typeId=%{public}d",
            key.c_str(), typeId);
        return false;
    }
    sptr<IInterface> value;
    if (!Internal::ParseScalarValueJson(typeId, item.value(), value)) {
        ABILITYBASE_LOGW("parse failed, restore value failed, key=%{public}s, typeId=%{public}d",
            key.c_str(), typeId);
        return false;
    }
    parsed.SetParam(key, value);
    return true;
}
}  // namespace

namespace Internal {
bool ParseParamsJson(
    const Json &jsonObject, WantParams &out, uint32_t depth, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("parse params enter, depth=%{public}u, inputCount=%{public}zu",
        depth, jsonObject.is_object() ? jsonObject.size() : 0);
    if (depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("parse failed, depth %{public}u exceeds max depth %{public}u",
            depth, MAX_RECURSION_DEPTH);
        return false;
    }
    if (!jsonObject.is_object()) {
        ABILITYBASE_LOGW("parse failed, params node is not object, depth=%{public}u", depth);
        return false;
    }

    // Commit only after the whole subtree is valid, so callers keep their
    // previous output value when parsing fails.
    WantParams parsed;
    for (const auto &item : jsonObject.items()) {
        if (!ParseTypedValueJson(item.value(), parsed, item.key(), depth, policy)) {
            ABILITYBASE_LOGD("parse params exit, depth=%{public}u, result=failed, parsedCount=%{public}d",
                depth, parsed.Size());
            return false;
        }
    }
    out = std::move(parsed);
    ABILITYBASE_LOGD("parse params exit, depth=%{public}u, result=success, "
        "inputCount=%{public}zu, outputCount=%{public}d", depth, jsonObject.size(), out.Size());
    return true;
}
}  // namespace Internal

namespace {
bool ParseEnvelopeJson(const Json &jsonObject, WantParams &out, UnsupportedTypePolicy policy)
{
    // The envelope is a fixed wrapper object and must not contain extra members.
    if (!jsonObject.is_object()) {
        ABILITYBASE_LOGW("parse failed, envelope is not object");
        return false;
    }
    if (jsonObject.size() != 1 || !jsonObject.contains(ENVELOPE_KEY)) {
        ABILITYBASE_LOGW("parse failed, invalid envelope, memberCount=%{public}zu", jsonObject.size());
        return false;
    }

    WantParams parsed;
    if (!Internal::ParseParamsJson(jsonObject.at(ENVELOPE_KEY), parsed, 0, policy)) {
        return false;
    }
    out = std::move(parsed);
    return true;
}
}  // namespace

bool Parse(const std::string &text, WantParams &out)
{
    return Parse(text, out, UnsupportedTypePolicy::SKIP);
}

bool Parse(const std::string &text, WantParams &out, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("parse envelope enter, inputLength=%{public}zu, skipUnsupported=%{public}d",
        text.size(), static_cast<int>(policy == UnsupportedTypePolicy::SKIP));
    if (!HasEnvelope(text)) {
        ABILITYBASE_LOGW("parse failed, missing exact envelope, length=%{public}zu", text.size());
        return false;
    }

    try {
        Json jsonObject = Json::parse(text, nullptr, false);
        if (jsonObject.is_discarded()) {
            ABILITYBASE_LOGW("parse failed, invalid json, length=%{public}zu", text.size());
            return false;
        }

        WantParams parsed;
        if (!ParseEnvelopeJson(jsonObject, parsed, policy)) {
            ABILITYBASE_LOGD("parse envelope exit, result=failed, inputLength=%{public}zu", text.size());
            return false;
        }
        out = std::move(parsed);
        ABILITYBASE_LOGD("parse envelope exit, result=success, inputLength=%{public}zu, "
            "outputCount=%{public}d", text.size(), out.Size());
        return true;
    } catch (const Json::exception &e) {
        ABILITYBASE_LOGE("parse failed, json exception id=%{public}d, length=%{public}zu", e.id, text.size());
        return false;
    } catch (const std::bad_alloc &) {
        ABILITYBASE_LOGE("parse failed, no memory, length=%{public}zu", text.size());
        return false;
    }
}

bool HasEnvelope(const std::string &text)
{
    // Match the exact envelope prefix emitted by Serialize. No whitespace is
    // allowed before '{' or between '{', the envelope key, and ':'.
    static const std::string envelopePrefix = "{\"" + std::string(ENVELOPE_KEY) + "\":";
    return text.compare(0, envelopePrefix.size(), envelopePrefix) == 0;
}

bool Serialize(const WantParams &wp, std::string &out)
{
    return Serialize(wp, out, UnsupportedTypePolicy::SKIP);
}

bool Serialize(const WantParams &wp, std::string &out, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("serialize envelope enter, inputCount=%{public}d, skipUnsupported=%{public}d",
        wp.Size(), static_cast<int>(policy == UnsupportedTypePolicy::SKIP));
    try {
        // Build the complete JSON tree before assigning to out.
        Json params;
        if (!Internal::BuildParamsJson(wp, params, 0, policy)) {
            ABILITYBASE_LOGD("serialize envelope exit, result=failed, inputCount=%{public}d", wp.Size());
            return false;
        }

        Json envelope = Json::object();
        envelope[ENVELOPE_KEY] = std::move(params);
        out = envelope.dump();
        ABILITYBASE_LOGD("serialize envelope exit, result=success, inputCount=%{public}d, "
            "outputLength=%{public}zu", wp.Size(), out.size());
        return true;
    } catch (const Json::exception &e) {
        ABILITYBASE_LOGE("serialize failed, json exception id=%{public}d", e.id);
        return false;
    } catch (const std::bad_alloc &) {
        ABILITYBASE_LOGE("serialize failed, no memory");
        return false;
    }
}
}  // namespace WantParamWrapperJson
}  // namespace AAFwk
}  // namespace OHOS
