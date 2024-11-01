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

#include "wantseventh_fuzzer.h"

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
constexpr uint8_t ENABLE = 2;
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
    want->AddFlags(flags);
    want->GetFlags();
    std::string bundleName(data, size);
    std::string abilityName(data, size);
    want->SetElementName(bundleName, abilityName);
    want->GetElement();
    want->GetEntities();
    want->GetBundle();
    std::string type(data, size);
    want->SetType(type);
    want->GetType();
    std::string action(data, size);
    want->SetAction(action);
    want->GetAction();
    WantParams wantparams;
    want->SetParams(wantparams);
    want->GetParams();
    std::string key(data, size);
    bool state = *data % ENABLE;
    want->SetParam(key, state);
    want->GetBoolParam(key, state);
    int iValue = static_cast<int>(GetU32Data(data));
    want->GetIntParam(key, iValue);
    want->SetParam(key, iValue);
    long lValue = static_cast<long>(GetU32Data(data));
    want->GetLongParam(key, lValue);
    want->SetParam(key, lValue);
    long long llValue = static_cast<long long>(GetU32Data(data));
    want->SetParam(key, llValue);
    want->GetStringParam(key);
    want->GetStringArrayParam(key);
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
