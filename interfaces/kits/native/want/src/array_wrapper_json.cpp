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
#include "array_wrapper_json.h"

#include <climits>
#include <string>
#include <utility>

#include "ability_base_log_wrapper.h"
#include "want_params_wrapper.h"
#include "want_params_wrapper_json_internal.h"

namespace OHOS {
namespace AAFwk {
namespace WantParamWrapperJson {
namespace Internal {
namespace ArrayWrapperJson {
namespace {
constexpr int TYPE_WANT_PARAMS = 101;
constexpr int TYPE_ARRAY = 102;
constexpr char TYPE_WANT_PARAMS_KEY[] = "101";
constexpr char TYPE_ARRAY_KEY[] = "102";

using SerializeElementHandler = bool (*)(const sptr<IInterface> &, Json &, uint32_t, long);
using ParseElementHandler = bool (*)(const Json &, sptr<IInterface> &, uint32_t, size_t);

bool GetSupportedType(const InterfaceID &interfaceId, int &typeId)
{
    if (interfaceId == g_IID_IWantParams) {
        typeId = TYPE_WANT_PARAMS;
        return true;
    }
    if (interfaceId == g_IID_IArray) {
        typeId = TYPE_ARRAY;
        return true;
    }
    return false;
}

bool GetSupportedType(const std::string &typeKey, int &typeId, InterfaceID &interfaceId)
{
    if (typeKey == TYPE_WANT_PARAMS_KEY) {
        typeId = TYPE_WANT_PARAMS;
        interfaceId = g_IID_IWantParams;
        return true;
    }
    if (typeKey == TYPE_ARRAY_KEY) {
        typeId = TYPE_ARRAY;
        interfaceId = g_IID_IArray;
        return true;
    }
    return false;
}

bool SerializeWantParamsElement(
    const sptr<IInterface> &element, Json &itemJson, uint32_t depth, long index)
{
    IWantParams *wantParams = IWantParams::Query(element);
    if (wantParams == nullptr) {
        ABILITYBASE_LOGW("serialize failed, array element is not WantParams, index=%{public}ld", index);
        return false;
    }

    WantParams child;
    ErrCode errCode = wantParams->GetValue(child);
    if (errCode != ERR_OK) {
        ABILITYBASE_LOGW("serialize failed, get WantParams element failed, index=%{public}ld, err=%{public}d",
            index, errCode);
        return false;
    }
    return BuildParamsJson(child, itemJson, depth + 1);
}

bool SerializeArrayElement(
    const sptr<IInterface> &element, Json &itemJson, uint32_t depth, long index)
{
    IArray *nestedArray = IArray::Query(element);
    if (nestedArray == nullptr) {
        ABILITYBASE_LOGW("serialize failed, array element is not Array, index=%{public}ld", index);
        return false;
    }
    return Serialize(nestedArray, itemJson, depth + 1);
}

bool ParseWantParamsElement(const Json &itemJson, sptr<IInterface> &element, uint32_t depth, size_t index)
{
    if (!itemJson.is_object()) {
        ABILITYBASE_LOGW("parse failed, WantParams array item is not object, index=%{public}zu", index);
        return false;
    }

    WantParams child;
    if (!ParseParamsJson(itemJson, child, depth + 1)) {
        return false;
    }
    sptr<IWantParams> boxed = WantParamWrapper::Box(std::move(child));
    if (boxed == nullptr) {
        ABILITYBASE_LOGE("parse failed, box WantParams array item failed, index=%{public}zu", index);
        return false;
    }
    element = boxed;
    return true;
}

bool ParseArrayElement(const Json &itemJson, sptr<IInterface> &element, uint32_t depth, size_t index)
{
    if (!itemJson.is_object()) {
        ABILITYBASE_LOGW("parse failed, nested Array item is not object, index=%{public}zu", index);
        return false;
    }

    sptr<IArray> nestedArray;
    if (!Parse(itemJson, nestedArray, depth + 1)) {
        ABILITYBASE_LOGW("parse failed, parse nested array item failed, index=%{public}zu", index);
        return false;
    }
    element = nestedArray;
    return true;
}
}  // namespace

bool Serialize(IArray *array, Json &out, uint32_t depth)
{
    if (array == nullptr || depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("serialize failed, invalid array, depth=%{public}u", depth);
        return false;
    }

    InterfaceID interfaceId;
    int typeId = 0;
    if (array->GetType(interfaceId) != ERR_OK || !GetSupportedType(interfaceId, typeId)) {
        ABILITYBASE_LOGW("serialize failed, unsupported array element type");
        return false;
    }

    long length = 0;
    if (array->GetLength(length) != ERR_OK || length < 0 ||
        static_cast<size_t>(length) > MAX_JSON_ARRAY_LENGTH) {
        ABILITYBASE_LOGW("serialize failed, invalid array length=%{public}ld", length);
        return false;
    }

    SerializeElementHandler serializeElement = nullptr;
    switch (typeId) {
        case TYPE_WANT_PARAMS:
            serializeElement = SerializeWantParamsElement;
            break;
        case TYPE_ARRAY:
            serializeElement = SerializeArrayElement;
            break;
        default:
            ABILITYBASE_LOGW("serialize failed, unsupported array element typeId=%{public}d", typeId);
            return false;
    }

    Json items = Json::array();
    for (long index = 0; index < length; ++index) {
        sptr<IInterface> element;
        if (array->Get(index, element) != ERR_OK || element == nullptr) {
            ABILITYBASE_LOGW("serialize failed, invalid array element, index=%{public}ld", index);
            return false;
        }

        Json itemJson;
        if (!serializeElement(element, itemJson, depth, index)) {
            return false;
        }
        items.push_back(std::move(itemJson));
    }

    Json arrayJson = Json::object();
    arrayJson[std::to_string(typeId)] = std::move(items);
    out = std::move(arrayJson);
    return true;
}

bool Parse(const Json &jsonObject, sptr<IArray> &out, uint32_t depth)
{
    if (depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("parse failed, array depth %{public}u exceeds max depth %{public}u",
            depth, MAX_RECURSION_DEPTH);
        return false;
    }
    if (!jsonObject.is_object() || jsonObject.size() != 1) {
        size_t memberCount = jsonObject.is_object() ? jsonObject.size() : 0;
        ABILITYBASE_LOGW("parse failed, invalid array node, memberCount=%{public}zu", memberCount);
        return false;
    }

    auto item = jsonObject.begin();
    int typeId = 0;
    InterfaceID interfaceId;
    if (!GetSupportedType(item.key(), typeId, interfaceId) || !item.value().is_array() ||
        item.value().size() > MAX_JSON_ARRAY_LENGTH || item.value().size() > static_cast<size_t>(LONG_MAX)) {
        ABILITYBASE_LOGW("parse failed, invalid or unsupported array schema, typeKeyLen=%{public}zu",
            item.key().size());
        return false;
    }

    ParseElementHandler parseElement = nullptr;
    switch (typeId) {
        case TYPE_WANT_PARAMS:
            parseElement = ParseWantParamsElement;
            break;
        case TYPE_ARRAY:
            parseElement = ParseArrayElement;
            break;
        default:
            ABILITYBASE_LOGW("parse failed, unsupported array element typeId=%{public}d", typeId);
            return false;
    }

    const Json &items = item.value();
    sptr<IArray> parsedArray = sptr<Array>::MakeSptr(static_cast<long>(items.size()), interfaceId);
    if (parsedArray == nullptr) {
        ABILITYBASE_LOGE("parse failed, allocate array failed, itemCount=%{public}zu", items.size());
        return false;
    }

    for (size_t index = 0; index < items.size(); ++index) {
        sptr<IInterface> element;
        if (!parseElement(items[index], element, depth, index) || element == nullptr ||
            parsedArray->Set(static_cast<long>(index), element) != ERR_OK) {
            ABILITYBASE_LOGE("parse failed, set array item failed, index=%{public}zu", index);
            return false;
        }
    }

    out = std::move(parsedArray);
    return true;
}
}  // namespace ArrayWrapperJson
}  // namespace Internal
}  // namespace WantParamWrapperJson
}  // namespace AAFwk
}  // namespace OHOS
