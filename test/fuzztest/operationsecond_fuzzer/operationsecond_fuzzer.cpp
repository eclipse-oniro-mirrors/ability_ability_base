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

#include "operationsecond_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "operation.h"
#undef private
#include "securec.h"
#include "uri.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t U32_AT_SIZE = 4;
}
bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<Operation> operation = std::make_shared<Operation>();
    std::shared_ptr<Operation> otherOperation = std::make_shared<Operation>();
    std::string values(data, size);
    otherOperation->SetBundleName(values);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    otherOperation->SetDeviceId(values);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    otherOperation->SetModuleName(values);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    std::vector<std::string> otherEntities;
    otherEntities.insert(otherEntities.begin(), values);
    otherOperation->SetEntities(otherEntities);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    operation->SetEntities(otherEntities);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    otherOperation->SetFlags(U32_AT_SIZE);
    if (*operation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    Uri otherUri(values);
    otherOperation->SetUri(otherUri);
    if (*operation == *otherOperation) {};
    if (*otherOperation == *otherOperation) {};
    operation = std::make_shared<Operation>(*otherOperation);
    std::string entity(data, size);
    entity.append(values);
    otherEntities.insert(otherEntities.begin(), entity);
    otherOperation->SetEntities(otherEntities);
    std::vector<std::string> entities = otherEntities;
    entities[0] = values;
    operation->SetEntities(entities);
    if (*operation == *otherOperation) {};
    Parcel parcel;
    operation->Marshalling(parcel);
    operation->ReadFromParcel(parcel);
    operation->Unmarshalling(parcel);
    operation->GetUri(otherUri);
    operation->DumpInfo(size);
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
