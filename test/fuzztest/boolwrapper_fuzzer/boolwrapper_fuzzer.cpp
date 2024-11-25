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

#include "boolwrapper_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>
#include <iostream>

#include "bool_wrapper.h"
#include "securec.h"

using namespace OHOS::AAFwk;

namespace OHOS {

bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fdp)
{
    bool boolValue = fdp->ConsumeBool();
    Boolean boolObj(boolValue);
    boolObj.GetValue(boolValue);
    Boolean otherObj(boolValue);
    boolObj.Equals(otherObj);
    boolObj.ToString();
    sptr<IBoolean> boolPtr = Boolean::Box(boolValue);
    Boolean::Unbox(boolPtr);
    std::string str("true");
    boolPtr = Boolean::Parse(str);
    return true;
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    FuzzedDataProvider fdp(data, size);
    OHOS::DoSomethingInterestingWithMyAPI(&fdp);
    return 0;
}