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

#include "operationfirst_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#define private public
#include "operation.h"
#undef private
#include "securec.h"

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

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<Operation> operation = std::make_shared<Operation>();
    std::string abilityName(data, size);
    operation->SetAbilityName(abilityName);
    operation->GetAbilityName();
    std::string action(data, size);
    operation->SetAction(action);
    operation->GetAction();
    std::string bundleName(data, size);
    operation->SetBundleName(bundleName);
    operation->GetBundleName();
    std::vector<std::string> entities;
    std::string entity(data, size);
    entities.push_back(entity);
    operation->SetEntities(entities);
    operation->GetEntities();
    unsigned int flags = static_cast<unsigned int>(GetU32Data(data, size));
    operation->SetFlags(flags);
    operation->GetFlags();
    operation->AddFlags(flags);
    std::shared_ptr<Operation> otherOperation = std::make_shared<Operation>(*operation);
    if (*operation == *otherOperation) {};
    std::string otherAbilityName(data, size);
    otherAbilityName.append(abilityName);
    otherOperation->SetAbilityName(otherAbilityName);
    if (*operation == *otherOperation) {};
    otherOperation->SetAbilityName(abilityName);
    std::string otherAction(data, size);
    otherAction.append(action);
    otherOperation->SetAction(otherAction);
    if (*operation == *otherOperation) {};
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