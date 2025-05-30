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

#include "wantparamssecond_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "want_params.h"
#undef private
#include "float_wrapper.h"
#include "securec.h"

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
    WantParams wantOther;
    std::shared_ptr<WantParams> wantParams = std::make_shared<WantParams>(wantOther);
    Parcel parcel;
    IArray* ao = nullptr;
    wantParams->WriteArrayToParcelChar(parcel, ao);
    wantParams->WriteArrayToParcelShort(parcel, ao);
    wantParams->WriteToParcelFD(parcel, wantOther);
    wantParams->WriteToParcelRemoteObject(parcel, wantOther);
    float floatValue = 0.0;
    sptr<IInterface> floatIt = Float::Box(floatValue);
    wantParams->WriteToParcelFloat(parcel, floatIt);
    int depth = static_cast<int>(GetU32Data(data));
    wantParams->WriteArrayToParcelWantParams(parcel, ao, depth);
    sptr<IArray> array;
    wantParams->ReadFromParcelArrayChar(parcel, array);
    wantParams->ReadFromParcelArrayShort(parcel, array);
    std::string key(data ,size);
    int type = static_cast<int>(GetU32Data(data));
    wantParams->ReadFromParcelWantParamWrapper(parcel, key, type, 1);
    wantParams->ReadFromParcelFD(parcel, key);
    wantParams->ReadFromParcelRemoteObject(parcel, key);
    wantParams->ReadFromParcelFloat(parcel, key);
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
