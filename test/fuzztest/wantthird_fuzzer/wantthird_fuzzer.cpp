/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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
    std::string noBundleNameStr = "{"
        "\"deviceId\": \"teset_deviceId\""
    "}";

    std::string noAbilityNameStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\""
    "}";

    std::string noUriStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\""
    "}";

    std::string noTypeStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\",\"uri\": \"teset_uri\""
    "}";

    std::string noFlagsStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\",\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\""
    "}";

    std::string noActionStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\","
        "\"abilityName\": \"teset_abilityName\",\"uri\": \"teset_uri\","
        "\"type\": \"teset_type\",\"flags\": \"teset_flags\""
    "}";

    if (want != nullptr) {
        cJSON *noBundleName = cJSON_Parse(noBundleNameStr.c_str());
        cJSON *noAbilityName = cJSON_Parse(noAbilityNameStr.c_str());
        cJSON *noUri = cJSON_Parse(noUriStr.c_str());
        cJSON *noType = cJSON_Parse(noTypeStr.c_str());
        cJSON *noFlags = cJSON_Parse(noFlagsStr.c_str());
        cJSON *noAction = cJSON_Parse(noActionStr.c_str());
        want->ReadFromJson(noBundleName);
        want->ReadFromJson(noAbilityName);
        want->ReadFromJson(noUri);
        want->ReadFromJson(noType);
        want->ReadFromJson(noFlags);
        want->ReadFromJson(noAction);
        cJSON_Delete(noBundleName);
        cJSON_Delete(noAbilityName);
        cJSON_Delete(noUri);
        cJSON_Delete(noType);
        cJSON_Delete(noFlags);
        cJSON_Delete(noAction);
    }
}

void DoReadFromJsonSecond(std::shared_ptr<Want> want)
{
    std::string noParametersStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\",\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\",\"type\": \"teset_type\",\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\",\"parameters\": \"teset_parameters\""
    "}";
    std::string noEntitiesStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": \"teset_bundleName\",\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\",\"type\": \"teset_type\",\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\",\"parameters\": \"teset_parameters\""
    "}";
    std::string noStringDeviceIdStr = "{"
        "\"deviceId\": {},\"bundleName\": \"teset_bundleName\",\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\",\"type\": \"teset_type\",\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\",\"parameters\": \"teset_parameters\",\"entities\": \"teset_entities\""
    "}";
    std::string noStringBundleNameStr = "{"
        "\"deviceId\": \"teset_deviceId\",\"bundleName\": {},\"abilityName\": \"teset_abilityName\","
        "\"uri\": \"teset_uri\",\"type\": \"teset_type\",\"flags\": \"teset_flags\","
        "\"action\": \"teset_action\",\"parameters\": \"teset_parameters\",\"entities\": \"teset_entities\""
    "}";

    if (want != nullptr) {
        cJSON *noParameters = cJSON_Parse(noParametersStr.c_str());
        cJSON *noEntities = cJSON_Parse(noEntitiesStr.c_str());
        cJSON *noStringDeviceId = cJSON_Parse(noStringDeviceIdStr.c_str());
        cJSON *noStringBundleName = cJSON_Parse(noStringBundleNameStr.c_str());
        want->ReadFromJson(noParameters);
        want->ReadFromJson(noEntities);
        want->ReadFromJson(noStringDeviceId);
        want->ReadFromJson(noStringBundleName);
        cJSON_Delete(noParameters);
        cJSON_Delete(noEntities);
        cJSON_Delete(noStringDeviceId);
        cJSON_Delete(noStringBundleName);
    }
}

void DoReadFromJsonThird(std::shared_ptr<Want> want)
{
    std::string noStringAbilityNameStr = "{"
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

    std::string noStringUriStr = "{"
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

    std::string noStringTypeStr = "{"
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
        cJSON *noStringAbilityName = cJSON_Parse(noStringAbilityNameStr.c_str());
        cJSON *noStringUri = cJSON_Parse(noStringUriStr.c_str());
        cJSON *noStringType = cJSON_Parse(noStringTypeStr.c_str());
        want->ReadFromJson(noStringAbilityName);
        want->ReadFromJson(noStringUri);
        want->ReadFromJson(noStringType);
        cJSON_Delete(noStringAbilityName);
        cJSON_Delete(noStringUri);
        cJSON_Delete(noStringType);
    }
}

void DoReadFromJsonFourth(std::shared_ptr<Want> want)
{
    std::string noStringFlagsStr = "{"
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

    std::string noStringActionStr = "{"
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

    std::string noStringParametersStr = "{"
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
        cJSON *noStringFlags = cJSON_Parse(noStringFlagsStr.c_str());
        cJSON *noStringAction = cJSON_Parse(noStringActionStr.c_str());
        cJSON *noStringParameters = cJSON_Parse(noStringParametersStr.c_str());
        want->ReadFromJson(noStringFlags);
        want->ReadFromJson(noStringAction);
        want->ReadFromJson(noStringParameters);
        cJSON_Delete(noStringFlags);
        cJSON_Delete(noStringAction);
        cJSON_Delete(noStringParameters);
    }
}

void DoReadFromJsonFifth(std::shared_ptr<Want> want)
{
    std::string nullEntitiesStr = "{"
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

    std::string arraryEntitiesItemNotStringStr = "{"
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

    std::string stringJsonStr = "{"
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
        cJSON *nullEntities = cJSON_Parse(nullEntitiesStr.c_str());
        cJSON *arraryEntitiesItemNotString = cJSON_Parse(arraryEntitiesItemNotStringStr.c_str());
        cJSON *stringJson = cJSON_Parse(stringJsonStr.c_str());
        want->ReadFromJson(nullEntities);
        want->ReadFromJson(arraryEntitiesItemNotString);
        want->ReadFromJson(stringJson);
        cJSON_Delete(nullEntities);
        cJSON_Delete(arraryEntitiesItemNotString);
        cJSON_Delete(stringJson);
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
    cJSON *wantJson = cJSON_CreateObject();
    want->ReadFromJson(wantJson);
    DoReadFromJson(want);
    cJSON_Delete(wantJson);
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
