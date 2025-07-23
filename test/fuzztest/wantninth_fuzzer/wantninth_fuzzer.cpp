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

#include "wantninth_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "want.h"
#undef private
#include "array_wrapper.h"
#include "bool_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "securec.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "uri.h"
#include "zchar_wrapper.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t FOO_MAX_LEN = 1024;
constexpr size_t U32_AT_SIZE = 4;
const uint32_t BYTE0_SHIFT = 24;
const uint32_t BYTE1_SHIFT = 16;
const uint32_t BYTE2_SHIFT = 8;
} // namespace
uint32_t GetU32Data(const char* ptr, size_t size)
{
    if (ptr == nullptr || size > OHOS::FOO_MAX_LEN || size < OHOS::U32_AT_SIZE) {
        return 0;
    }
    return (static_cast<uint32_t>(ptr[0]) << BYTE0_SHIFT) | (static_cast<uint32_t>(ptr[1]) << BYTE1_SHIFT) |
           (static_cast<uint32_t>(ptr[2]) << BYTE2_SHIFT) | static_cast<uint32_t>(ptr[3]);
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type GetData(const char* ptr, size_t size)
{
    if (ptr == nullptr || size > OHOS::FOO_MAX_LEN || size < sizeof(T)) {
        return static_cast<T>(0);
    }

    T result = 0;
    if (memcpy_s(&result, sizeof(T), ptr, std::min(sizeof(T), size)) != 0) {
        std::cout << "copy failed." << std::endl;
        return static_cast<T>(0);
    }
    return result;
}

void DoFromString(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string emptyString;
    want->FromString(emptyString);
    std::string values(data, size);
    want->FromString(values);
    std::string jsonWant = want->ToString();
    want->FromString(jsonWant);
}

void DoCheckUri(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string values(data, size);
    std::string emptyString;
    want->CheckUri(emptyString);
    want->CheckUri(values);
    values.insert(0, Want::WANT_HEADER);
    want->CheckUri(values);
    values.append(Want::WANT_END);
    want->CheckUri(values);
}

void DoWriteAndReadUri(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string values(data, size);
    Parcel uriParcel;
    want->WriteUri(uriParcel);
    Uri uri(values);
    want->SetUri(uri);
    want->WriteUri(uriParcel);
    want->ReadUri(uriParcel);
}

void DoWriteAndReadEntities(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string values(data, size);
    Parcel entitiesParcel;
    want->WriteEntities(entitiesParcel);
    want->AddEntity(values);
    want->WriteEntities(entitiesParcel);
    want->ReadEntities(entitiesParcel);
}

void DoWriteAndReadElement(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string values(data, size);
    Parcel elementParcel;
    want->WriteElement(elementParcel);
    want->SetElementName(values, values);
    want->WriteElement(elementParcel);
    want->ReadElement(elementParcel);
}

void DoWriteAndReadParameters(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    Parcel parameters;
    want->WriteParameters(parameters);
    WantParams wantParams;
    auto longValue = static_cast<long>(U32_AT_SIZE);
    std::shared_ptr<Array> wantArray = std::make_shared<Array>(longValue, g_IID_IString);
    std::string value(data, size);
    sptr<IInterface> stringValue = String::Box(value);
    for (size_t i = 0; i < longValue; i++) {
        wantArray->Set(i, stringValue);
    }
    sptr<IInterface> wantArrayParam = Array::Parse(wantArray->ToString());
    std::string wantArrayKey(1, Array::SIGNATURE);
    wantParams.SetParam(wantArrayKey, wantArrayParam);
    want->SetParams(wantParams);
    want->WriteParameters(parameters);
    want->ReadParameters(parameters);
}


bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    DoFromString(data, size);
    DoCheckUri(data, size);
    DoWriteAndReadUri(data, size);
    DoWriteAndReadEntities(data, size);
    DoWriteAndReadElement(data, size);
    DoWriteAndReadParameters(data, size);
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
    if (size < OHOS::U32_AT_SIZE) {
        return 0;
    }

    char* ch = reinterpret_cast<char *>(malloc(size + 1));
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
