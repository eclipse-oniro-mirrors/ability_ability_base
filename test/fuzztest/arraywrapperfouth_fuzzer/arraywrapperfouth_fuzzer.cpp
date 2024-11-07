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

#include "arraywrapperfouth_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "array_wrapper.h"
#undef private
#include "long_wrapper.h"
#include "securec.h"
#include "string_wrapper.h"
#include "want_params.h"
#include "want_params_wrapper.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t U32_AT_SIZE = 4;
constexpr size_t MAX_ARRAY_SIZE = 50 * 1024 * 1024;
}
bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    long longSize = (long)(size);
    InterfaceID id;
    std::shared_ptr<Array> array = std::make_shared<Array>(longSize, id);
    std::shared_ptr<Array> otherArray = std::make_shared<Array>(longSize, id);
    std::string value(data, size);
    sptr<IInterface> stringValue = String::Box(value);
    sptr<IInterface> longValue = Long::Box(longSize);
    for (size_t i = 0; i < longSize; i++) {
        array->Set(i, stringValue);
        otherArray->Set(i, longValue);
    }
    array->Equals(*otherArray);
    array->Equals(*array);
    WantParams otherWantParams;
    std::shared_ptr<WantParams> wantParams = std::make_shared<WantParams>(otherWantParams);
    WantParamWrapper wantParamWrapper(*wantParams);
    array->Equals(wantParamWrapper);
    long lengthSize = (long)(MAX_ARRAY_SIZE + size);
    array->size_ = lengthSize;
    array->GetLength(lengthSize);
    otherArray->size_ = size;
    array->Equals(*otherArray);
    array->size_ = (long)(INT_MAX + size);
    otherArray->size_ = (long)(INT_MAX + size);
    array->Equals(*otherArray);
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
