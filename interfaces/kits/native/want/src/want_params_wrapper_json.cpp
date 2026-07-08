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
#include "array_wrapper.h"
#include "nlohmann/json.hpp"
#include "want_params_wrapper.h"

namespace OHOS {
namespace AAFwk {
namespace WantParamWrapperJson {
namespace {
using Json = nlohmann::json;

constexpr int TYPE_WANT_PARAMS = 101;
constexpr int TYPE_ARRAY = 102;
constexpr int TYPE_NULL = -1;

// Maximum supported nested WantParams depth. The top-level WantParams object
// starts at depth 0.
constexpr uint32_t MAX_DEPTH = 100;

bool ParseTypeId(const std::string &token, int &typeId)
{
    errno = 0;
    char *end = nullptr;
    long parsed = strtol(token.c_str(), &end, 10);
    if (errno == ERANGE || end == token.c_str() || *end != '\0' || parsed > INT_MAX || parsed < INT_MIN) {
        return false;
    }
    typeId = static_cast<int>(parsed);
    return typeId != TYPE_NULL && WantParams::IsKnownTypeId(typeId);
}

bool BuildParamsJson(const WantParams &wp, Json &out, uint32_t depth)
{
    if (depth > MAX_DEPTH) {
        ABILITYBASE_LOGW("serialize failed, depth %{public}u exceeds max depth %{public}u", depth, MAX_DEPTH);
        return false;
    }

    // Each parameter is encoded as {"typeId": value}. Nested WantParams are
    // encoded as JSON objects, while scalar values are encoded as strings.
    Json params = Json::object();
    for (const auto &it : wp.GetParams()) {
        int typeId = WantParams::GetDataType(it.second);
        if (typeId == TYPE_NULL) {
            continue;
        }

        Json typedValue = Json::object();
        IWantParams *nested = IWantParams::Query(it.second);
        IArray *nestedArray = IArray::Query(it.second);
        if (nested != nullptr) {
            WantParams child;
            ErrCode errCode = nested->GetValue(child);
            if (errCode != ERR_OK) {
                ABILITYBASE_LOGW("serialize failed, get nested WantParams failed, keyLen=%{public}zu, err=%{public}d",
                    it.first.size(), errCode);
                return false;
            }
            Json childJson;
            if (!BuildParamsJson(child, childJson, depth + 1)) {
                return false;
            }
            typedValue[std::to_string(typeId)] = std::move(childJson);
        } else if (nestedArray != nullptr) {
            std::string value = static_cast<Array *>(nestedArray)->ToString(depth + 1);
            if (value.empty()) {
                ABILITYBASE_LOGW("serialize failed, array depth exceeds max depth, keyLen=%{public}zu",
                    it.first.size());
                return false;
            }
            typedValue[std::to_string(typeId)] = std::move(value);
        } else {
            typedValue[std::to_string(typeId)] = WantParams::GetStringByType(it.second, typeId);
        }
        params[it.first] = std::move(typedValue);
    }

    out = std::move(params);
    return true;
}

bool ParseParamsJson(const Json &jsonObject, WantParams &out, uint32_t depth);

sptr<IInterface> RestoreScalarValueJson(int typeId, const Json &valueJson, uint32_t depth)
{
    std::string valueStr = valueJson.get<std::string>();
    if (typeId == TYPE_ARRAY) {
        return Array::Parse(valueStr, depth + 1);
    }
    return WantParams::GetInterfaceByType(typeId, valueStr);
}

bool ParseTypedValueJson(const Json &typedValue, WantParams &parsed, const std::string &key, uint32_t depth)
{
    // A typed-value object represents one logical WantParams value, so it must
    // contain exactly one typeId member.
    if (!typedValue.is_object() || typedValue.size() != 1) {
        size_t memberCount = typedValue.is_object() ? typedValue.size() : 0;
        ABILITYBASE_LOGW("parse failed, invalid typed value, keyLen=%{public}zu, memberCount=%{public}zu",
            key.size(), memberCount);
        return false;
    }

    auto item = typedValue.begin();
    int typeId = 0;
    if (!ParseTypeId(item.key(), typeId)) {
        ABILITYBASE_LOGW("parse failed, invalid typeId, keyLen=%{public}zu, typeIdLen=%{public}zu",
            key.size(), item.key().size());
        return false;
    }

    if (typeId == TYPE_WANT_PARAMS) {
        WantParams child;
        if (!ParseParamsJson(item.value(), child, depth + 1)) {
            return false;
        }
        sptr<IWantParams> value = WantParamWrapper::Box(std::move(child));
        if (value == nullptr) {
            ABILITYBASE_LOGE("parse failed, box nested WantParams failed, keyLen=%{public}zu", key.size());
            return false;
        }
        parsed.SetParam(key, value);
        return true;
    }

    if (!item.value().is_string()) {
        ABILITYBASE_LOGW("parse failed, scalar value is not string, keyLen=%{public}zu, typeId=%{public}d",
            key.size(), typeId);
        return false;
    }
    sptr<IInterface> value = RestoreScalarValueJson(typeId, item.value(), depth);
    if (value == nullptr) {
        ABILITYBASE_LOGW("parse failed, restore value failed, keyLen=%{public}zu, typeId=%{public}d",
            key.size(), typeId);
        return false;
    }
    parsed.SetParam(key, value);
    return true;
}

bool ParseParamsJson(const Json &jsonObject, WantParams &out, uint32_t depth)
{
    if (depth > MAX_DEPTH) {
        ABILITYBASE_LOGW("parse failed, depth %{public}u exceeds max depth %{public}u", depth, MAX_DEPTH);
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
        if (!ParseTypedValueJson(item.value(), parsed, item.key(), depth)) {
            return false;
        }
    }
    out = std::move(parsed);
    return true;
}

bool ParseEnvelopeJson(const Json &jsonObject, WantParams &out)
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
    if (!ParseParamsJson(jsonObject.at(ENVELOPE_KEY), parsed, 0)) {
        return false;
    }
    out = std::move(parsed);
    return true;
}
}  // namespace

bool Parse(const std::string &text, WantParams &out)
{
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
        if (!ParseEnvelopeJson(jsonObject, parsed)) {
            return false;
        }
        out = std::move(parsed);
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
    const std::string envelopePrefix = "{\"" + std::string(ENVELOPE_KEY) + "\":";
    return text.compare(0, envelopePrefix.size(), envelopePrefix) == 0;
}

bool Serialize(const WantParams &wp, std::string &out)
{
    try {
        // Build the complete JSON tree before assigning to out.
        Json params;
        if (!BuildParamsJson(wp, params, 0)) {
            return false;
        }

        Json envelope = Json::object();
        envelope[ENVELOPE_KEY] = std::move(params);
        out = envelope.dump();
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
