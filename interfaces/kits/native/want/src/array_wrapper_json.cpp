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
constexpr int TYPE_BOOLEAN = 1;
constexpr int TYPE_BYTE = 2;
constexpr int TYPE_CHAR = 3;
constexpr int TYPE_SHORT = 4;
constexpr int TYPE_INT = 5;
constexpr int TYPE_LONG = 6;
constexpr int TYPE_FLOAT = 7;
constexpr int TYPE_DOUBLE = 8;
constexpr int TYPE_STRING = 9;
constexpr int TYPE_WANT_PARAMS = 101;
constexpr int TYPE_ARRAY = 102;

struct SupportedType {
    const InterfaceID *interfaceId;
    int typeId;
};

struct ElementContext {
    int typeId;
    uint32_t depth;
    UnsupportedTypePolicy policy;
};

const SupportedType SUPPORTED_TYPES[] = {
    {&g_IID_IBoolean, TYPE_BOOLEAN},
    {&g_IID_IByte, TYPE_BYTE},
    {&g_IID_IChar, TYPE_CHAR},
    {&g_IID_IShort, TYPE_SHORT},
    {&g_IID_IInteger, TYPE_INT},
    {&g_IID_ILong, TYPE_LONG},
    {&g_IID_IFloat, TYPE_FLOAT},
    {&g_IID_IDouble, TYPE_DOUBLE},
    {&g_IID_IString, TYPE_STRING},
    {&g_IID_IWantParams, TYPE_WANT_PARAMS},
    {&g_IID_IArray, TYPE_ARRAY},
};

bool IsScalarTypeId(int typeId)
{
    return typeId >= TYPE_BOOLEAN && typeId <= TYPE_STRING;
}

bool GetSupportedType(const InterfaceID &interfaceId, int &typeId)
{
    for (const auto &supportedType : SUPPORTED_TYPES) {
        if (interfaceId == *supportedType.interfaceId) {
            typeId = supportedType.typeId;
            return true;
        }
    }
    return false;
}

bool GetSupportedType(int typeId, InterfaceID &interfaceId)
{
    for (const auto &supportedType : SUPPORTED_TYPES) {
        if (typeId == supportedType.typeId) {
            interfaceId = *supportedType.interfaceId;
            return true;
        }
    }
    return false;
}

ConvertResult SerializeScalarElement(
    const sptr<IInterface> &element, Json &itemJson, int typeId, long index)
{
    if (WantParams::GetDataType(element) != typeId) {
        ABILITYBASE_LOGW("serialize failed, scalar array element type mismatch, index=%{public}ld, "
            "typeId=%{public}d", index, typeId);
        return ConvertResult::ERROR;
    }
    itemJson = WantParams::GetStringByType(element, typeId);
    return ConvertResult::SUCCESS;
}

ConvertResult SerializeWantParamsElement(const sptr<IInterface> &element, Json &itemJson,
    uint32_t depth, long index, UnsupportedTypePolicy policy)
{
    IWantParams *wantParams = IWantParams::Query(element);
    if (wantParams == nullptr) {
        ABILITYBASE_LOGW("serialize failed, array element is not WantParams, index=%{public}ld", index);
        return ConvertResult::ERROR;
    }

    WantParams child;
    ErrCode errCode = wantParams->GetValue(child);
    if (errCode != ERR_OK) {
        ABILITYBASE_LOGW("serialize failed, get WantParams element failed, index=%{public}ld, err=%{public}d",
            index, errCode);
        return ConvertResult::ERROR;
    }
    if (!BuildParamsJson(child, itemJson, depth + 1, policy)) {
        return ConvertResult::ERROR;
    }
    return ConvertResult::SUCCESS;
}

ConvertResult SerializeArrayElement(const sptr<IInterface> &element, Json &itemJson,
    uint32_t depth, long index, UnsupportedTypePolicy policy)
{
    IArray *nestedArray = IArray::Query(element);
    if (nestedArray == nullptr) {
        ABILITYBASE_LOGW("serialize failed, array element is not Array, index=%{public}ld", index);
        return ConvertResult::ERROR;
    }
    return Serialize(nestedArray, itemJson, depth + 1, policy);
}

ConvertResult SerializeElement(const sptr<IInterface> &element, Json &itemJson,
    const ElementContext &context, long index)
{
    if (IsScalarTypeId(context.typeId)) {
        return SerializeScalarElement(element, itemJson, context.typeId, index);
    }
    if (context.typeId == TYPE_WANT_PARAMS) {
        return SerializeWantParamsElement(element, itemJson, context.depth, index, context.policy);
    }
    if (context.typeId == TYPE_ARRAY) {
        return SerializeArrayElement(element, itemJson, context.depth, index, context.policy);
    }
    return ConvertResult::UNSUPPORTED;
}

ConvertResult ParseScalarElement(
    const Json &itemJson, sptr<IInterface> &element, int typeId, size_t index)
{
    if (!ParseScalarValueJson(typeId, itemJson, element)) {
        ABILITYBASE_LOGW("parse failed, invalid scalar array element, index=%{public}zu, "
            "typeId=%{public}d", index, typeId);
        return ConvertResult::ERROR;
    }
    return ConvertResult::SUCCESS;
}

ConvertResult ParseWantParamsElement(const Json &itemJson, sptr<IInterface> &element,
    uint32_t depth, size_t index, UnsupportedTypePolicy policy)
{
    if (!itemJson.is_object()) {
        ABILITYBASE_LOGW("parse failed, WantParams array item is not object, index=%{public}zu", index);
        return ConvertResult::ERROR;
    }

    WantParams child;
    if (!ParseParamsJson(itemJson, child, depth + 1, policy)) {
        return ConvertResult::ERROR;
    }
    sptr<IWantParams> boxed = WantParamWrapper::Box(std::move(child));
    if (boxed == nullptr) {
        ABILITYBASE_LOGE("parse failed, box WantParams array item failed, index=%{public}zu", index);
        return ConvertResult::ERROR;
    }
    element = boxed;
    return ConvertResult::SUCCESS;
}

ConvertResult ParseArrayElement(const Json &itemJson, sptr<IInterface> &element,
    uint32_t depth, size_t index, UnsupportedTypePolicy policy)
{
    if (!itemJson.is_object()) {
        ABILITYBASE_LOGW("parse failed, nested Array item is not object, index=%{public}zu", index);
        return ConvertResult::ERROR;
    }

    sptr<IArray> nestedArray;
    ConvertResult result = Parse(itemJson, nestedArray, depth + 1, policy);
    if (result != ConvertResult::SUCCESS) {
        if (result == ConvertResult::ERROR) {
            ABILITYBASE_LOGW("parse failed, parse nested array item failed, index=%{public}zu", index);
        }
        return result;
    }
    element = nestedArray;
    return ConvertResult::SUCCESS;
}

ConvertResult ParseElement(const Json &itemJson, sptr<IInterface> &element,
    const ElementContext &context, size_t index)
{
    if (IsScalarTypeId(context.typeId)) {
        return ParseScalarElement(itemJson, element, context.typeId, index);
    }
    if (context.typeId == TYPE_WANT_PARAMS) {
        return ParseWantParamsElement(itemJson, element, context.depth, index, context.policy);
    }
    if (context.typeId == TYPE_ARRAY) {
        return ParseArrayElement(itemJson, element, context.depth, index, context.policy);
    }
    return ConvertResult::UNSUPPORTED;
}
}  // namespace

ConvertResult Serialize(IArray *array, Json &out, uint32_t depth, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("serialize array enter, depth=%{public}u", depth);
    if (array == nullptr || depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("serialize failed, invalid array, depth=%{public}u", depth);
        return ConvertResult::ERROR;
    }

    InterfaceID interfaceId;
    int typeId = 0;
    if (array->GetType(interfaceId) != ERR_OK) {
        ABILITYBASE_LOGW("serialize failed, get array element type failed");
        return ConvertResult::ERROR;
    }
    if (!GetSupportedType(interfaceId, typeId)) {
        ABILITYBASE_LOGD("serialize array exit, depth=%{public}u, result=unsupported", depth);
        return ConvertResult::UNSUPPORTED;
    }

    long length = 0;
    if (array->GetLength(length) != ERR_OK || length < 0 ||
        static_cast<size_t>(length) > MAX_JSON_ARRAY_LENGTH) {
        ABILITYBASE_LOGW("serialize failed, invalid array length=%{public}ld", length);
        return ConvertResult::ERROR;
    }
    ABILITYBASE_LOGD("serialize array metadata, depth=%{public}u, typeId=%{public}d, length=%{public}ld",
        depth, typeId, length);

    const ElementContext context = {typeId, depth, policy};
    Json items = Json::array();
    for (long index = 0; index < length; ++index) {
        sptr<IInterface> element;
        if (array->Get(index, element) != ERR_OK || element == nullptr) {
            ABILITYBASE_LOGW("serialize failed, invalid array element, index=%{public}ld", index);
            return ConvertResult::ERROR;
        }

        Json itemJson;
        ConvertResult result = SerializeElement(element, itemJson, context, index);
        if (result != ConvertResult::SUCCESS) {
            ABILITYBASE_LOGD("serialize array exit, depth=%{public}u, typeId=%{public}d, "
                "index=%{public}ld, result=%{public}d", depth, typeId, index, static_cast<int>(result));
            return result;
        }
        items.push_back(std::move(itemJson));
    }

    Json arrayJson = Json::object();
    arrayJson[std::to_string(typeId)] = std::move(items);
    out = std::move(arrayJson);
    ABILITYBASE_LOGD("serialize array exit, depth=%{public}u, typeId=%{public}d, "
        "length=%{public}ld, result=success", depth, typeId, length);
    return ConvertResult::SUCCESS;
}

ConvertResult Parse(const Json &jsonObject, sptr<IArray> &out,
    uint32_t depth, UnsupportedTypePolicy policy)
{
    ABILITYBASE_LOGD("parse array enter, depth=%{public}u, memberCount=%{public}zu",
        depth, jsonObject.is_object() ? jsonObject.size() : 0);
    if (depth > MAX_RECURSION_DEPTH) {
        ABILITYBASE_LOGW("parse failed, array depth %{public}u exceeds max depth %{public}u",
            depth, MAX_RECURSION_DEPTH);
        return ConvertResult::ERROR;
    }
    if (!jsonObject.is_object() || jsonObject.size() != 1) {
        size_t memberCount = jsonObject.is_object() ? jsonObject.size() : 0;
        ABILITYBASE_LOGW("parse failed, invalid array node, memberCount=%{public}zu", memberCount);
        return ConvertResult::ERROR;
    }

    auto item = jsonObject.begin();
    int typeId = 0;
    InterfaceID interfaceId;
    if (!ParseTypeId(item.key(), typeId)) {
        ABILITYBASE_LOGW("parse failed, invalid array element typeId, typeId=%{public}s",
            item.key().c_str());
        return ConvertResult::ERROR;
    }
    if (!item.value().is_array() || item.value().size() > MAX_JSON_ARRAY_LENGTH ||
        item.value().size() > static_cast<size_t>(LONG_MAX)) {
        ABILITYBASE_LOGW("parse failed, invalid array items, typeId=%{public}d", typeId);
        return ConvertResult::ERROR;
    }
    if (!GetSupportedType(typeId, interfaceId)) {
        ABILITYBASE_LOGD("parse array exit, depth=%{public}u, typeId=%{public}d, result=unsupported",
            depth, typeId);
        return ConvertResult::UNSUPPORTED;
    }
    ABILITYBASE_LOGD("parse array metadata, depth=%{public}u, typeId=%{public}d, itemCount=%{public}zu",
        depth, typeId, item.value().size());

    const ElementContext context = {typeId, depth, policy};
    const Json &items = item.value();
    sptr<IArray> parsedArray = sptr<Array>::MakeSptr(static_cast<long>(items.size()), interfaceId);
    if (parsedArray == nullptr) {
        ABILITYBASE_LOGE("parse failed, allocate array failed, itemCount=%{public}zu", items.size());
        return ConvertResult::ERROR;
    }

    for (size_t index = 0; index < items.size(); ++index) {
        sptr<IInterface> element;
        ConvertResult result = ParseElement(items[index], element, context, index);
        if (result != ConvertResult::SUCCESS) {
            ABILITYBASE_LOGD("parse array exit, depth=%{public}u, typeId=%{public}d, "
                "index=%{public}zu, result=%{public}d", depth, typeId, index, static_cast<int>(result));
            return result;
        }
        if (element == nullptr || parsedArray->Set(static_cast<long>(index), element) != ERR_OK) {
            ABILITYBASE_LOGE("parse failed, set array item failed, index=%{public}zu", index);
            return ConvertResult::ERROR;
        }
    }

    out = std::move(parsedArray);
    ABILITYBASE_LOGD("parse array exit, depth=%{public}u, typeId=%{public}d, "
        "itemCount=%{public}zu, result=success", depth, typeId, items.size());
    return ConvertResult::SUCCESS;
}
}  // namespace ArrayWrapperJson
}  // namespace Internal
}  // namespace WantParamWrapperJson
}  // namespace AAFwk
}  // namespace OHOS
