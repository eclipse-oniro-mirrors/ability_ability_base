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

#include "wantparamsfour_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

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
constexpr size_t U32_AT_SIZE = 4;
constexpr size_t BOOLEAN_TO_STRING = 10;
constexpr int32_t ARRAY_SIZE = 5;
}

uint32_t GetU32Data(const char* ptr)
{
    // convert fuzz input data to an integer
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

void DoCoverageUnsupportedDataAndWantParams()
{
    std::vector<UnsupportedData> cachedUnsupportedData;
    UnsupportedData data1;
    cachedUnsupportedData.push_back(data1);
    UnsupportedData data2(std::move(data1));
    cachedUnsupportedData.push_back(data2);
    UnsupportedData data3;
    cachedUnsupportedData.push_back(data3);
    UnsupportedData data4 = data3;
    cachedUnsupportedData.push_back(data4);
    data3 = data4;
    data4 = std::move(data3);
    WantParams wantOther;
    std::shared_ptr<WantParams> wantParams = std::make_shared<WantParams>(std::move(wantOther));
    wantParams->SetCachedUnsupportedData(cachedUnsupportedData);
    wantParams->GetCachedUnsupportedData(cachedUnsupportedData);
}

void ConvertBasicDataTypes(const char* data, size_t size, std::map<std::string, sptr<IInterface>>& params)
{
    std::string stringValue(data, size);
    bool boolValue = static_cast<bool>(GetU32Data(data));
    uint8_t byteValue = static_cast<uint8_t>(GetU32Data(data));
    char charValue = static_cast<char>(GetU32Data(data));
    int16_t shortValue = static_cast<int16_t>(GetU32Data(data));
    int32_t intValue = static_cast<int32_t>(GetU32Data(data));
    long longValue = static_cast<long>(GetU32Data(data));
    float floatValue = static_cast<float>(GetU32Data(data));
    double doubleValue = static_cast<double>(GetU32Data(data));

    params["string"] = String::Box(stringValue);
    params["boolean"] = Boolean::Box(boolValue);
    params["byte"] = Byte::Box(byteValue);
    params["char"] = Char::Box(charValue);
    params["short"] = Short::Box(shortValue);
    params["integer"] = Integer::Box(intValue);
    params["long"] = Long::Box(longValue);
    params["float"] = Float::Box(floatValue);
    params["double"] = Double::Box(doubleValue);
}

void CreateArrays(const char* data, size_t size, std::map<std::string, sptr<IInterface>>& params)
{
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
    
    std::string stringValue(data, size);
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

    params["booleanArray"] = booleanArray;
    params["charArray"] = charArray;
    params["byteArray"] = byteArray;
    params["shortArray"] = shortArray;
    params["integerArray"] = integerArray;
    params["longArray"] = longArray;
    params["floatArray"] = floatArray;
    params["doubleArray"] = doubleArray;
    params["stringArray"] = stringArray;
    params["wantParamsArray"] = wantParamsArray;
    params["other"] = other;
}

void HandleWantParams(std::map<std::string, sptr<IInterface>>& params)
{
    WantParams source;
    source.params_ = params;
    WantParams dest(source);
    sptr<IArray> destNull = nullptr;
    if (source == dest) {}
    dest.NewArrayData(static_cast<IArray*>(params["booleanArray"].GetRefPtr()), destNull);
    
    Parcel parcel;
    UnsupportedData unsupportedData;
    dest.cachedUnsupportedData_.push_back(unsupportedData);
    dest.DoMarshalling(parcel, 0);
    
    WantParams paramsClose(dest);
    WantParams paramsRemove(dest);
    WantParams paramsDup(dest);
    paramsClose.CloseAllFd();
    paramsRemove.RemoveAllFd();
    paramsDup.DupAllFd();
}

void DoCoverageNewArrayData(const char* data, size_t size)
{
    std::map<std::string, sptr<IInterface>> params;
    ConvertBasicDataTypes(data, size, params);
    CreateArrays(data, size, params);
    HandleWantParams(params);
}

void DoCoverageGetInterfaceByType()
{
    std::string stringValue = std::to_string(BOOLEAN_TO_STRING);
    WantParams params;
    for (size_t i = 0; i < BOOLEAN_TO_STRING; i++) {
        params.GetInterfaceByType(i, stringValue);
    }
    params.GetInterfaceByType(WantParams::VALUE_TYPE_ARRAY, stringValue);
}

void DoCoverageWriteToParce(const char* data, size_t size)
{
    Parcel parcel;
    std::string stringValue(data, size);
    bool boolValue = static_cast<bool>(GetU32Data(data));
    int intValue = static_cast<int>(GetU32Data(data));
    long longValue = static_cast<long>(GetU32Data(data));
    sptr<IInterface> stringIt = String::Box(stringValue);
    sptr<IInterface> boolIt = Boolean::Box(boolValue);
    sptr<IInterface> intIt = Integer::Box(intValue);
    sptr<IInterface> longIt = Long::Box(longValue);
    WantParams params;
    params.WriteToParcelString(parcel, stringIt);
    params.WriteToParcelBool(parcel, boolIt);
    params.WriteToParcelInt(parcel, intIt);
    params.WriteToParcelLong(parcel, longIt);
}

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    DoCoverageUnsupportedDataAndWantParams();
    DoCoverageNewArrayData(data, size);
    DoCoverageGetInterfaceByType();
    DoCoverageWriteToParce(data, size);
    return true;
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr) {
        std::cout << "invalid data" << std::endl;
        return 0;
    }

    /* Validate the length of size */
    if (size < OHOS::U32_AT_SIZE) {
        return 0;
    }

    char* ch = (char *)malloc(size + 1);
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
