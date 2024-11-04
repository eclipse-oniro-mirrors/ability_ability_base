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

#include "wantsecond_fuzzer.h"

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

void DoCheckAndSetParameters(std::shared_ptr<Want> want, const char* data, size_t size)
{
    if (want == nullptr) {
        return;
    }

    std::string prop(data, size);
    std::string value(data, size);
    if (prop.length() > TEST_TYPE_TAG_SIZE) {
        prop[1] = '.';
        prop[0] = ' ';
        Want::CheckAndSetParameters(*want, "other", prop, value);

        prop[0] = String::SIGNATURE;
        Want::CheckAndSetParameters(*want, "string", prop, value);

        prop[0] = Boolean::SIGNATURE;
        bool flag = *data % ENABLE;
        Want::CheckAndSetParameters(*want, "bool", prop, std::to_string(flag));

        prop[0] = Char::SIGNATURE;
        auto val = std::to_string(static_cast<char>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "char", prop, val);

        prop[0] = Byte::SIGNATURE;
        val = std::to_string(static_cast<byte>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "byte", prop, val);

        prop[0] = Short::SIGNATURE;
        val = std::to_string(static_cast<short>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "short", prop, val);

        prop[0] = Integer::SIGNATURE;
        val = std::to_string(static_cast<int>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "integer", prop, val);

        prop[0] = Long::SIGNATURE;
        val = std::to_string(static_cast<long>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "long", prop, val);

        prop[0] = Float::SIGNATURE;
        val = std::to_string(static_cast<float>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "float", prop, val);

        prop[0] = Double::SIGNATURE;
        val = std::to_string(static_cast<double>(GetU32Data(data)));
        Want::CheckAndSetParameters(*want, "double", prop, val);

        prop[0] = Array::SIGNATURE;
        sptr<Array> stringArray = new Array(ARRAY_SIZE, g_IID_IString);
        for (int i =0; i < ARRAY_SIZE; ++i) {
            stringArray->Set(i, String::Box(value));
        }
        val = stringArray->ToString();
        Want::CheckAndSetParameters(*want, "array", prop, val);

        prop[0] = Array::SIGNATURE;
        sptr<Array> array = new Array(ARRAY_SIZE, g_IID_IString);
        for (int i =0; i < ARRAY_SIZE; ++i) {
            array->Set(i, String::Box(value));
        }
        val = array->ToString();
        val = "." + val;
        Want::CheckAndSetParameters(*want, "invalidarray", prop, value);
    }
}

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<Want> want = std::make_shared<Want>();
    std::string type(data, size);
    want->SetType(type);
    want->FormatType(type);
    Uri uri(type);
    want->SetUri(uri);
    want->SetUriAndType(uri, type);
    want->FormatUri(uri);
    want->FormatUri(type);
    char* charUri;
    want->WantParseUri(charUri);
    want->GetLowerCaseScheme(uri);
    want->FormatUriAndType(uri, type);
    want->FormatMimeType(type);
    DoCheckAndSetParameters(want, data, size);
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
