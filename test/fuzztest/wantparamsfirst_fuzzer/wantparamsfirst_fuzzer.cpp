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

#include "wantparamsfirst_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "want_params.h"
#undef private
#include "string_wrapper.h"
#include "byte_wrapper.h"
#include "want_params_wrapper.h"
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
    UnsupportedData other;
    std::shared_ptr<UnsupportedData> unsupportedData = std::make_shared<UnsupportedData>(other);
    WantParams wantOther;
    std::shared_ptr<WantParams> wantParams = std::make_shared<WantParams>(wantOther);
    std::string key(data, size);
    int8_t byteValue = static_cast<int8_t>(GetU32Data(data));
    sptr<IInterface> stringIt = String::Box(key);
    wantParams->SetParam(key, stringIt);
    wantParams->GetWantParams(key);
    wantParams->GetStringParam(key);
    int defaultValue = static_cast<int>(GetU32Data(data));
    wantParams->GetIntParam(key, defaultValue);
    wantParams->KeySet();
    wantParams->HasParam(key);
    wantParams->IsEmpty();
    int level = static_cast<int32_t>(GetU32Data(data));
    wantParams->DumpInfo(level);
    Parcel parcel;
    sptr<IInterface> byteIt = Byte::Box(byteValue);
    int depth = static_cast<int>(GetU32Data(data));
    wantParams->WriteToParcelWantParams(parcel, byteIt, depth);
    wantParams->WriteToParcelByte(parcel, byteIt);
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
