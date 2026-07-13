/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "array_wrapper.h"
#include <cstdint>
#include "ability_base_log_wrapper.h"
#include "bool_wrapper.h"
#include "zchar_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"

namespace OHOS {
namespace AAFwk {
IINTERFACE_IMPL_1(Array, Object, IArray);

constexpr int32_t MAX_ARRAY_SIZE = 50 * 1024 * 1024;
constexpr int32_t MAX_ARRAY_RECURSION_DEPTH = 100;

namespace {
bool IsValidArraySignature(char signature)
{
    return signature == String::SIGNATURE || signature == Boolean::SIGNATURE || signature == Byte::SIGNATURE ||
        signature == Short::SIGNATURE || signature == Integer::SIGNATURE || signature == Long::SIGNATURE ||
        signature == Float::SIGNATURE || signature == Double::SIGNATURE || signature == Array::SIGNATURE ||
        signature == Char::SIGNATURE || signature == WantParamWrapper::SIGNATURE;
}

bool ParseArraySize(const std::string &arrayStr, std::size_t idx, long &size)
{
    try {
        size = std::stol(arrayStr.substr(1, idx - 1));
    } catch (...) {
        ABILITYBASE_LOGE("failed to convert to long: %{public}s", arrayStr.substr(1, idx - 1).c_str());
        return false;
    }
    if (size < 0 || size > MAX_ARRAY_SIZE) {
        ABILITYBASE_LOGE("invalid array size: %{public}ld", size);
        return false;
    }
    return true;
}

bool IsInvalidArrayDepth(int depth, const char *func)
{
    if (depth < 0 || depth > MAX_ARRAY_RECURSION_DEPTH) {
        ABILITYBASE_LOGE("%{public}s: invalid recursion depth %{public}d", func, depth);
        return true;
    }
    return false;
}

bool ScanElement(const std::string &values, std::size_t beginIdx, bool stopAtComma, std::size_t &commaIdx)
{
    std::size_t braceDepth = 0;
    for (std::size_t i = beginIdx; i < values.length(); ++i) {
        char current = values[i];
        if (current == '{') {
            ++braceDepth;
        } else if (current == '}') {
            if (braceDepth == 0) {
                return false;
            }
            --braceDepth;
        } else if (stopAtComma && current == ',' && braceDepth == 0) {
            commaIdx = i;
            return true;
        }
    }
    commaIdx = std::string::npos;
    return braceDepth == 0;
}

// Extracts one element from `values` at `beginIdx`, advancing past the consumed segment.
// The last element takes the residual tail; an earlier one scans to the next top-level
// comma and sets `shouldBreak` when data runs out before the declared size is reached.
bool ExtractElementValue(const std::string &values, std::size_t &beginIdx, bool isLast,
    std::string &valueStr, bool &shouldBreak)
{
    shouldBreak = false;
    if (isLast) {
        valueStr = values.substr(beginIdx, values.length() - beginIdx);
        std::size_t endIdx = std::string::npos;
        if (!ScanElement(valueStr, 0, false, endIdx)) {
            return false;
        }
        return true;
    }
    std::size_t endIdx = std::string::npos;
    if (!ScanElement(values, beginIdx, true, endIdx)) {
        return false;
    }
    if (endIdx == std::string::npos) {
        valueStr = values.substr(beginIdx);
        shouldBreak = true;
        return true;
    }
    valueStr = values.substr(beginIdx, endIdx - beginIdx);
    beginIdx = endIdx + 1;
    return true;
}

bool GetArraySignature(const InterfaceID &typeId, char &signature)
{
    if (typeId == g_IID_IString) {
        signature = String::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IBoolean) {
        signature = Boolean::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IByte) {
        signature = Byte::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IShort) {
        signature = Short::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IInteger) {
        signature = Integer::SIGNATURE;
        return true;
    } else if (typeId == g_IID_ILong) {
        signature = Long::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IFloat) {
        signature = Float::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IDouble) {
        signature = Double::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IArray) {
        signature = Array::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IChar) {
        signature = Char::SIGNATURE;
        return true;
    } else if (typeId == g_IID_IWantParams) {
        signature = WantParamWrapper::SIGNATURE;
        return true;
    }
    ABILITYBASE_LOGE("failed to get array signature");
    return false;
}
}

Array::Array(long size, const InterfaceID &id) : typeId_(id)
{
    size_ = size > MAX_ARRAY_SIZE ? MAX_ARRAY_SIZE : size;
    for (long i = 0; i < size_; i++) {
        values_.push_back(nullptr);
    }
}

ErrCode Array::Get(long index, /* [in] */
    sptr<IInterface> &value)   /* [out] */
{
    if (index < 0 || index >= size_) {
        value = nullptr;
        return ERR_INVALID_VALUE;
    }
    value = values_[index];
    return ERR_OK;
}

ErrCode Array::GetLength(long &size) /* [out] */
{
    size = size_;
    if (size_ < 0 || size_ > MAX_ARRAY_SIZE) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

ErrCode Array::GetType(InterfaceID &id) /* [out] */
{
    VALIDATE_NOT_NULL(&id);

    id = typeId_;
    return ERR_OK;
}

ErrCode Array::Set(long index, /* [in] */
    IInterface *value)         /* [in] */
{
    if (index < 0 || index >= size_) {
        return ERR_INVALID_VALUE;
    }
    values_[index] = value;
    return ERR_OK;
}

bool Array::Equals(IObject &other) /* [in] */
{
    Array *otherObj = static_cast<Array *>(IArray::Query(&other));
    if (otherObj == nullptr) {
        return false;
    }

    if (this == otherObj) {
        return true;
    }

    if (otherObj->size_ != size_ || otherObj->typeId_ != typeId_) {
        return false;
    }

    if (size_ < 0 || size_ > INT_MAX) {
        return false;
    }

    for (long i = 0; i < size_; i++) {
        auto *left = values_[i].GetRefPtr();
        auto *right = otherObj->values_[i].GetRefPtr();
        if (left == nullptr && right == nullptr) {
            continue;
        }
        if (left == nullptr || right == nullptr) {
            return false;
        }
        if (!Object::Equals(*left, *right)) {
            return false;
        }
    }
    return true;
}

std::string Array::ToString()
{
    return ToString(0);
}

std::string Array::ToString(int depth)
{
    std::string result;
    if (IsInvalidArrayDepth(depth, "Array::ToString")) {
        return result;
    }
    char signature;
    if (GetArraySignature(typeId_, signature)) {
        result += signature;
    }

    result += std::to_string(size_) + "{";
    for (long i = 0; i < size_; i++) {
        if (values_[i].GetRefPtr() == nullptr) {
            return "";
        }
        if (!AppendValueString(values_[i].GetRefPtr(), depth, result)) {
            return "";
        }
        if (i < size_ - 1) {
            result += ",";
        }
    }
    result += "}";
    return result;
}

bool Array::AppendValueString(IInterface *value, int depth, std::string &result)
{
    if (value == nullptr) {
        ABILITYBASE_LOGE("AppendValueString: value is null");
        return false;
    }
    if (IArray::Query(value) != nullptr) {
        std::string valueStr = static_cast<Array *>(IArray::Query(value))->ToString(depth + 1);
        if (valueStr.empty()) {
            return false;
        }
        result += valueStr;
        return true;
    }
    if (IWantParams::Query(value) != nullptr) {
        std::string valueStr = static_cast<WantParamWrapper *>(IWantParams::Query(value))->ToString(depth + 1);
        if (valueStr.empty()) {
            return false;
        }
        result += valueStr;
        return true;
    }
    result += Object::ToString(*value);
    return true;
}

sptr<IArray> Array::ParseString(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IString);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return String::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseBoolean(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IBoolean);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Boolean::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseByte(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IByte);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Byte::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseShort(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IShort);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Short::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseInteger(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IInteger);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Integer::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseLong(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_ILong);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Long::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseFloat(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IFloat);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Float::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseDouble(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IDouble);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Double::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}
sptr<IArray> Array::ParseChar(const std::string &values, long size)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IChar);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Char::Parse(str); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}
sptr<IArray> Array::ParseArray(const std::string &values, long size)
{
    return ParseArray(values, size, 0);
}

sptr<IArray> Array::ParseArray(const std::string &values, long size, int depth)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IArray);
    if (array != nullptr) {
        auto func = [depth](const std::string &str) -> sptr<IInterface> { return Array::Parse(str, depth + 1); };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseWantParams(const std::string &values, long size)
{
    return ParseWantParams(values, size, 0);
}

sptr<IArray> Array::ParseWantParams(const std::string &values, long size, int depth)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(size, g_IID_IWantParams);
    if (array != nullptr) {
        auto func = [depth](const std::string &str) -> sptr<IInterface> {
            return WantParamWrapper::Parse(str, depth + 1);
        };
        if (!ParseElement(array, func, values, size)) {
            return nullptr;
        }
    }
    return array;
}

sptr<IArray> Array::ParseBySignature(char signature, const std::string &values, long size, int depth)
{
    switch (signature) {
        case Char::SIGNATURE:
            return ParseChar(values, size);
        case String::SIGNATURE:
            return ParseString(values, size);
        case Boolean::SIGNATURE:
            return ParseBoolean(values, size);
        case Byte::SIGNATURE:
            return ParseByte(values, size);
        case Short::SIGNATURE:
            return ParseShort(values, size);
        case Integer::SIGNATURE:
            return ParseInteger(values, size);
        case Long::SIGNATURE:
            return ParseLong(values, size);
        case Float::SIGNATURE:
            return ParseFloat(values, size);
        case Double::SIGNATURE:
            return ParseDouble(values, size);
        case Array::SIGNATURE:
            return ParseArray(values, size, depth);
        case WantParamWrapper::SIGNATURE:
            return ParseWantParams(values, size, depth);
        default:
            return nullptr;
    }
}

sptr<IArray> Array::Parse(const std::string &arrayStr) /* [in] */
{
    return Parse(arrayStr, 0);
}

sptr<IArray> Array::Parse(const std::string &arrayStr, int depth) /* [in] */
{
    if (arrayStr.empty()) {
        return nullptr;
    }
    if (IsInvalidArrayDepth(depth, "Array::Parse")) {
        return nullptr;
    }

    char signature = arrayStr[0];
    if (!IsValidArraySignature(signature)) {
        return nullptr;
    }

    std::size_t idx = arrayStr.find("{");
    if (idx == std::string::npos) {
        return nullptr;
    }
    if (arrayStr[arrayStr.length() - 1] != '}') {
        return nullptr;
    }
    long size = 0;
    if (!ParseArraySize(arrayStr, idx, size)) {
        return nullptr;
    }

    idx += 1;
    std::string values = arrayStr.substr(idx, arrayStr.length() - 1 - idx);
    // DoS guard: Attacker-controlled size.
    // Reject oversized values upfront to prevent mass empty slot pre-allocation in Array constructor.
    if (static_cast<size_t>(size) > values.length() + 1) {
        ABILITYBASE_LOGE("declared array size %{public}ld exceeds content length %{public}zu",
            size, values.length() + 1);
        return nullptr;
    }

    return ParseBySignature(signature, values, size, depth);
}

bool Array::ParseElement(IArray *array,                  /* [in] */
    std::function<sptr<IInterface>(std::string &)> func, /* [in] */
    const std::string &values,                           /* [in] */
    long size)                                           /* [in] */
{
    if (array == nullptr) {
        return false;
    }

    std::size_t beginIdx = 0;
    for (long i = 0; i < size; i++) {
        bool isLast = (i == size - 1);
        std::string valueStr;
        bool shouldBreak = false;
        if (!ExtractElementValue(values, beginIdx, isLast, valueStr, shouldBreak)) {
            return false;
        }
        auto element = func(valueStr);
        if (element == nullptr || array->Set(i, element) != ERR_OK) {
            return false;
        }
        if (shouldBreak) {
            break;
        }
    }
    return true;
}

bool Array::IsBooleanArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IBoolean;
}

bool Array::IsCharArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IChar;
}

bool Array::IsByteArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IByte;
}

bool Array::IsShortArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IShort;
}

bool Array::IsIntegerArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IInteger;
}

bool Array::IsLongArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_ILong;
}

bool Array::IsFloatArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IFloat;
}

bool Array::IsDoubleArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IDouble;
}

bool Array::IsStringArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IString;
}

bool Array::IsWantParamsArray(IArray *array) /* [in] */
{
    if (array == nullptr) {
        return false;
    }
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IWantParams;
}

void Array::ForEach(IArray *array,          /* [in] */
    std::function<void(IInterface *)> func) /* [in] */
{
    if (array == nullptr) {
        return;
    }
    long size;
    array->GetLength(size);
    for (long i = 0; i < size; i++) {
        sptr<IInterface> object;
        array->Get(i, object);
        func(object);
    }
}
}  // namespace AAFwk
}  // namespace OHOS
