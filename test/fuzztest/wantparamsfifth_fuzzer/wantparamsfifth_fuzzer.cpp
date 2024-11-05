/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <iostream>

#include "wantparamsfifth_fuzzer.h"

#define private public
#include "want_params.h"
#undef private
#include "array_wrapper.h"
#include "base_interfaces.h"
#include "bool_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "remote_object_wrapper.h"
#include "securec.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"
#include "zchar_wrapper.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t FOO_MAX_LEN = 1024;
constexpr size_t U32_AT_SIZE = 4;
constexpr size_t PARAMS_SIZE = 21;
constexpr int32_t OTHER = -2;
constexpr int32_t ARRAY_SIZE = 5;
} // namespace

uint32_t GetU32Data(const char* ptr)
{
    // convert fuzz input data to an integer
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

void WriteBasicTypesToParcel(Parcel& parcel, const char* data, size_t size)
{
    std::string stringValue(data, size);
    bool boolValue = static_cast<bool>(GetU32Data(data));
    int8_t byteValue = static_cast<int8_t>(GetU32Data(data));
    char charValue = static_cast<char>(GetU32Data(data));
    int16_t shortValue = static_cast<int16_t>(GetU32Data(data));
    int32_t intValue = static_cast<int32_t>(GetU32Data(data));
    long longValue = static_cast<long>(GetU32Data(data));
    float floatValue = static_cast<float>(GetU32Data(data));
    double doubleValue = static_cast<double>(GetU32Data(data));

    parcel.WriteInt32(PARAMS_SIZE);
    parcel.WriteString16(Str8ToStr16("booleanKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_BOOLEAN);
    parcel.WriteBool(boolValue);
    parcel.WriteString16(Str8ToStr16("byteKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_BYTE);
    parcel.WriteInt8(byteValue);
    parcel.WriteString16(Str8ToStr16("charKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_CHAR);
    parcel.WriteInt32(charValue);
    parcel.WriteString16(Str8ToStr16("shortKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_SHORT);
    parcel.WriteInt16(shortValue);
    parcel.WriteString16(Str8ToStr16("intKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_INT);
    parcel.WriteInt32(intValue);
    parcel.WriteString16(Str8ToStr16("longKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_LONG);
    parcel.WriteInt64(longValue);
    parcel.WriteString16(Str8ToStr16("floatKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_FLOAT);
    parcel.WriteFloat(floatValue);
    parcel.WriteString16(Str8ToStr16("doubleKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_DOUBLE);
    parcel.WriteDouble(doubleValue);
    parcel.WriteString16(Str8ToStr16("stringKey"));
    parcel.WriteInt32(WantParams::VALUE_TYPE_STRING);
    parcel.WriteString16(Str8ToStr16(stringValue));
}

void WriteArrayToParcel(
    Parcel& parcel, const std::string& key, int32_t type, sptr<IArray> array, std::shared_ptr<WantParams> wantParams)
{
    parcel.WriteString16(Str8ToStr16(key));
    parcel.WriteInt32(type);
    wantParams->WriteArrayToParcel(parcel, array, 0);
}

void WriteArraysToParcel(Parcel& parcel, const char* data, size_t size, std::shared_ptr<WantParams> wantParams)
{
    std::string stringValue(data, size);
    sptr<IArray> booleanArray = new Array(ARRAY_SIZE, g_IID_IBoolean);
    sptr<IArray> charArray = new Array(ARRAY_SIZE, g_IID_IChar);
    sptr<IArray> byteArray = new Array(ARRAY_SIZE, g_IID_IByte);
    sptr<IArray> shortArray = new Array(ARRAY_SIZE, g_IID_IShort);
    sptr<IArray> integerArray = new Array(ARRAY_SIZE, g_IID_IInteger);
    sptr<IArray> longArray = new Array(ARRAY_SIZE, g_IID_ILong);
    sptr<IArray> floatArray = new Array(ARRAY_SIZE, g_IID_IFloat);
    sptr<IArray> doubleArray = new Array(ARRAY_SIZE, g_IID_IDouble);
    sptr<IArray> stringArray = new Array(ARRAY_SIZE, g_IID_IString);
    sptr<IArray> wantParamsArray = new Array(ARRAY_SIZE, g_IID_IWantParams);
    sptr<IArray> other = new Array(ARRAY_SIZE, g_IID_IRemoteObjectWrap);
    for (int32_t i = 0; i < ARRAY_SIZE; i++) {
        booleanArray->Set(i, Boolean::Box(static_cast<bool>(GetU32Data(data))));
        charArray->Set(i, Char::Box(static_cast<char>(GetU32Data(data))));
        byteArray->Set(i, Byte::Box(static_cast<int8_t>(GetU32Data(data))));
        shortArray->Set(i, Short::Box(static_cast<int16_t>(GetU32Data(data))));
        integerArray->Set(i, Integer::Box(static_cast<int32_t>(GetU32Data(data))));
        longArray->Set(i, Long::Box(static_cast<long>(GetU32Data(data))));
        floatArray->Set(i, Float::Box(static_cast<float>(GetU32Data(data))));
        doubleArray->Set(i, Double::Box(static_cast<double>(GetU32Data(data))));
        stringArray->Set(i, String::Box(stringValue));
        other->Set(i, RemoteObjectWrap::Box(nullptr));
    }
    WriteArrayToParcel(parcel, "stringArray", WantParams::VALUE_TYPE_STRINGARRAY, stringArray, wantParams);
    WriteArrayToParcel(parcel, "booleanArray", WantParams::VALUE_TYPE_BOOLEANARRAY, booleanArray, wantParams);
    WriteArrayToParcel(parcel, "byteArray", WantParams::VALUE_TYPE_BYTEARRAY, byteArray, wantParams);
    WriteArrayToParcel(parcel, "charArray", WantParams::VALUE_TYPE_CHARARRAY, charArray, wantParams);
    WriteArrayToParcel(parcel, "shortArray", WantParams::VALUE_TYPE_SHORTARRAY, shortArray, wantParams);
    WriteArrayToParcel(parcel, "integerArray", WantParams::VALUE_TYPE_INTARRAY, integerArray, wantParams);
    WriteArrayToParcel(parcel, "longArray", WantParams::VALUE_TYPE_LONGARRAY, longArray, wantParams);
    WriteArrayToParcel(parcel, "floatArray", WantParams::VALUE_TYPE_FLOATARRAY, floatArray, wantParams);
    WriteArrayToParcel(parcel, "doubleArray", WantParams::VALUE_TYPE_DOUBLEARRAY, doubleArray, wantParams);
    WriteArrayToParcel(parcel, "wantParamsArray", WantParams::VALUE_TYPE_WANTPARAMSARRAY, wantParamsArray, wantParams);
    WriteArrayToParcel(parcel, "other", OTHER, other, wantParams);
    parcel.WriteInt32(WantParams::VALUE_TYPE_NULL);
}

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    WantParams wantOther;
    std::shared_ptr<WantParams> wantParams = std::make_shared<WantParams>(wantOther);

    Parcel parcel;
    WriteBasicTypesToParcel(parcel, data, size);
    WriteArraysToParcel(parcel, data, size, wantParams);

    auto wantPtr = wantParams->Unmarshalling(parcel, 0);
    if (wantPtr) {
        delete wantPtr;
        wantPtr = nullptr;
    }
    return true;
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr) {
        std::cout << "invalid data" << std::endl;
        return 0;
    }

    /* Validate the length of size */
    if (size > OHOS::FOO_MAX_LEN || size < OHOS::U32_AT_SIZE) {
        return 0;
    }

    char* ch = (char*)malloc(size + 1);
    if (ch == nullptr) {
        std::cout << "malloc failed." << std::endl;
        return 0;
    }

    (void)memset_s(ch, size + 1, 0x00, size + 1);
    if (memcpy_s(ch, size, data, size) != EOK) {
        std::cout << "copy failed." << std::endl;
        free(ch);
        ch = nullptr;
        return 0;
    }

    OHOS::DoSomethingInterestingWithMyAPI(ch, size);
    free(ch);
    ch = nullptr;
    return 0;
}
