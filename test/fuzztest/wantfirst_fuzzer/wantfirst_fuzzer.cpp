/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "wantfirst_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "want.h"
#undef private
#include "securec.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t FOO_MAX_LEN = 1024;
constexpr size_t U32_AT_SIZE = 4;
}
uint32_t GetU32Data(const char* ptr)
{
    // convert fuzz input data to an integer
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}
bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    unsigned int flags = static_cast<unsigned int>(GetU32Data(data));
    want->SetFlags(flags);
    want->RemoveFlags(flags);
    std::string entity(data, size);
    want->AddEntity(entity);
    want->HasEntity(entity);
    want->RemoveEntity(entity);
    want->CountEntities();
    std::string bundleName(data, size);
    want->SetBundle(bundleName);
    std::string type(data, size);
    want->SetType(type);
    std::string deviceId(data, size);
    want->SetDeviceId(deviceId);
    std::string moduleName(data, size);
    want->SetModuleName(moduleName);
    want->FormatType(type);
    Uri uri(type);
    want->SetUri(uri);
    want->SetUriAndType(uri, type);
    want->FormatUri(uri);
    want->FormatUri(type);
    Parcel wantParcel;
    Want* wantptr = nullptr;
    if (wantParcel.WriteBuffer(data, size)) {
        wantptr = Want::Unmarshalling(wantParcel);
    }
    want->WantToUri(*wantptr);
    char* charUri;
    want->WantParseUri(charUri);
    want->GetLowerCaseScheme(uri);
    want->FormatUriAndType(uri, type);
    std::string mimeType(data, size);
    want->FormatMimeType(type);
    want->GetScheme();
    AppExecFwk::ElementName elementName;
    want->MakeMainAbility(elementName);
    std::string key(data, size);
    sptr<IRemoteObject> remoteObject;
    want->SetParam(key, remoteObject);
    want->GetRemoteObject(key);
    std::vector<bool> boolValue;
    want->SetParam(key, boolValue);
    want->GetBoolArrayParam(key);
    byte byteValue = '\0';
    want->SetParam(key, byteValue);
    want->GetByteParam(key, byteValue);
    std::vector<byte> byteVector;
    want->SetParam(key, byteVector);
    want->GetByteArrayParam(key);
    zchar charValue = U'\0';
    want->SetParam(key, charValue);
    want->GetCharParam(key, charValue);
    std::vector<zchar> charVector;
    want->SetParam(key, charVector);
    want->GetCharArrayParam(key);
    std::vector<int> intVector;
    want->SetParam(key, intVector);
    want->GetIntArrayParam(key);
    double doubleValue = 0.0;
    want->SetParam(key, doubleValue);
    want->GetDoubleParam(key, doubleValue);
    std::vector<double> doubleVector;
    want->SetParam(key, doubleVector);
    want->GetDoubleArrayParam(key);
    float floatValue = 0.0;
    want->SetParam(key, floatValue);
    want->GetFloatParam(key, floatValue);
    std::vector<float> floatVector;
    want->SetParam(key, floatVector);
    want->GetFloatArrayParam(key);
    std::vector<long> longVector;
    want->SetParam(key, longVector);
    want->GetLongArrayParam(key);
    short shortValue = 0;
    want->SetParam(key, shortValue);
    want->GetShortParam(key, shortValue);
    std::vector<short> shortVector;
    want->SetParam(key, shortVector);
    want->GetShortArrayParam(key);
    std::vector<std::string> stringVector;
    want->SetParam(key, stringVector);
    Operation operation;
    want->SetOperation(operation);
    want->CloneOperation();
    want->ReplaceParams(*wantptr);
    want->HasParameter(key);
    std::string content(data, size);
    want->ParseUriInternal(content, elementName, *wantptr);
    std::string prop(data, size);
    std::string value(data, size);
    want->CheckAndSetParameters(*wantptr, key, prop, value);
    want->ParseContent(content, prop, value);
    want->ParseFlag(content, *wantptr);
    std::string str(data, size);
    want->Decode(str);
    want->Encode(str);
    int level = static_cast<int>(GetU32Data(data));
    want->DumpInfo(level);
    nlohmann::json wantJson;
    want->ReadFromJson(wantJson);
    want->FromString(value);
    want->GetDeviceId();
    want->GetModuleName();
    want->ClearWant(wantptr);
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
    if (size > OHOS::FOO_MAX_LEN || size < OHOS::U32_AT_SIZE) {
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
