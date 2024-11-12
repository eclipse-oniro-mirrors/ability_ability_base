/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

#include "wantfifth_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "want.h"
#undef private
#include "securec.h"
#include "string_wrapper.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
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
    std::string key(data, size);
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
    std::string action(data, size);
    want->SetAction(action);
    std::string uri(data, size);
    want->SetUri(uri);
    std::vector<std::string> entities;
    entities.push_back(key);
    want->SetEntities(entities);
    std::string deviceId(data, size);
    want->SetDeviceId(deviceId);
    std::string bundleName(data, size);
    std::string abilityName(data, size);
    want->SetElementName(bundleName, abilityName);
    unsigned int flags = static_cast<unsigned int>(GetU32Data(data));
    want->SetFlags(flags);
    std::string uriString(data, size);
    want->ToUriStringInner(uriString);
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
