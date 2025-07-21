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

#include "wantthird_fuzzer.h"

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
constexpr size_t U32_AT_SIZE = 4;
}
uint32_t GetU32Data(const char* ptr)
{
    // convert fuzz input data to an integer
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

void DoReadFromJsonFirst(std::shared_ptr<Want> want)
{
    nlohmann::json noBundleName = "{"
        "\"deviceId\": \"teset_deviceId\""
    "}";

    nlohmann::json noAbilityName = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\""
    "}";

    nlohmann::json noUri = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\""
    "}";

    nlohmann::json noType = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\""
    "}";

    nlohmann::json noFlags = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\""
    "}";

    nlohmann::json noAction = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\""
    "}";
    if (want != nullptr) {
        want->ReadFromJson(noBundleName);
        want->ReadFromJson(noAbilityName);
        want->ReadFromJson(noUri);
        want->ReadFromJson(noType);
        want->ReadFromJson(noFlags);
        want->ReadFromJson(noAction);
    }
}

void DoReadFromJsonSecond(std::shared_ptr<Want> want)
{
    nlohmann::json noParameters = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\""
    "}";
    nlohmann::json noEntities = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\""
    "}";
    nlohmann::json noStringDeviceId = "{"
        "\"deviceId\": {},"
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";
    nlohmann::json noStringBundleName = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": {},"
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";
    if (want != nullptr) {
        want->ReadFromJson(noParameters);
        want->ReadFromJson(noEntities);
        want->ReadFromJson(noStringDeviceId);
        want->ReadFromJson(noStringBundleName);
    }
}

void DoReadFromJsonThird(std::shared_ptr<Want> want)
{
    nlohmann::json noStringAbilityName = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": {},"
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";

    nlohmann::json noStringUri = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": {},"
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";

    nlohmann::json noStringType = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": {},"
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";

    if (want != nullptr) {
        want->ReadFromJson(noStringAbilityName);
        want->ReadFromJson(noStringUri);
        want->ReadFromJson(noStringType);
    }
}

void DoReadFromJsonFourth(std::shared_ptr<Want> want)
{
    nlohmann::json noStringFlags = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": {},"
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";

    nlohmann::json noStringAction = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": {},"
        "\"parameters\": \"teset_parameters\","
        "\"entities\": \"teset_entities\""
    "}";

    nlohmann::json noStringParameters = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": {},"
        "\"entities\": \"teset_entities\""
    "}";

    if (want != nullptr) {
        want->ReadFromJson(noStringFlags);
        want->ReadFromJson(noStringAction);
        want->ReadFromJson(noStringParameters);
    }
}

void DoReadFromJsonFifth(std::shared_ptr<Want> want)
{
    nlohmann::json nullEntities = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": "
    "}";

    nlohmann::json arraryEntitiesItemNotString = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": ["
            "{},"
        "]"
    "}";

    nlohmann::json stringJson = "{"
        "\"deviceId\": \"teset_deviceId\","
        "\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\","
        "\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\","
        "\"parameters\": \"teset_parameters\","
        "\"entities\": ["
            "\"entities\""
        "]"
    "}";

    if (want != nullptr) {
        want->ReadFromJson(nullEntities);
        want->ReadFromJson(arraryEntitiesItemNotString);
        want->ReadFromJson(stringJson);
    }
}

void DoReadFromJson(std::shared_ptr<Want> want)
{
    DoReadFromJsonFirst(want);
    DoReadFromJsonSecond(want);
    DoReadFromJsonThird(want);
    DoReadFromJsonFourth(want);
    DoReadFromJsonFifth(want);
}

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    want->CountEntities();
    Want *wantptr = nullptr;
    want->GetScheme();
    AppExecFwk::ElementName elementName;
    wantptr = want->MakeMainAbility(elementName);
    if (wantptr) {
        delete wantptr;
        wantptr = nullptr;
    }
    Operation operation;
    want->SetOperation(operation);
    wantptr = want->CloneOperation();
    if (wantptr) {
        delete wantptr;
        wantptr = nullptr;
    }
    std::string key(data, size);
    want->HasParameter(key);
    std::string content(data, size);
    std::string prop(data, size);
    std::string value(data, size);
    want->ParseContent(content, prop, value);
    std::string str(data, size);
    want->Decode(str);
    want->Encode(str);
    int level = static_cast<int>(GetU32Data(data));
    want->DumpInfo(level);
    nlohmann::json wantJson;
    std::string jsonStr(data, size);
    size_t pos = jsonStr.find("\"jsonStr\"");
    if (pos != std::string::npos) {
        std::string jsonString = jsonStr.substr(pos + 8);
        wantJson = nlohmann::json::parse(jsonString, nullptr, false);
        if (!wantJson.is_discarded()) {
            std::cout << "json parse success:" << wantJson.dump() << std::endl;
            want->ReadFromJson(wantJson);
        }
    }
    DoReadFromJson(want);
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
