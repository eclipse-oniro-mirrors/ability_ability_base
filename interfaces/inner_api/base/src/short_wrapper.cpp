/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "short_wrapper.h"

#include <cerrno>
#include <climits>
#include <cstdlib>

#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AAFwk {
IINTERFACE_IMPL_1(Short, Object, IShort);

ErrCode Short::GetValue(short &value) /* [out] */
{
    VALIDATE_NOT_NULL(&value);

    value = value_;
    return ERR_OK;
}

bool Short::Equals(IObject &other) /* [in] */
{
    Short *otherObj = static_cast<Short *>(IShort::Query(&other));
    return otherObj != nullptr && otherObj->value_ == value_;
}

std::string Short::ToString()
{
    return std::to_string(value_);
}

sptr<IShort> Short::Box(short value) /* [in] */
{
    sptr<IShort> object = sptr<Short>::MakeSptr(value);
    return object;
}

short Short::Unbox(IShort *object) /* [in] */
{
    short value = 0;
    if (object == nullptr) {
        return value;
    }
    object->GetValue(value);
    return value;
}

sptr<IShort> Short::Parse(const std::string &str)
{
    sptr<IShort> object;
    if (str.empty()) {
        return object;
    }
    char *end = nullptr;
    errno = 0;
    long value = strtol(str.c_str(), &end, 10);
    if (errno != 0 || end == str.c_str() || *end != '\0' ||
        value < SHRT_MIN || value > SHRT_MAX) {
        ABILITYBASE_LOGE("failed to convert to short: %{public}s", str.c_str());
        return object;
    }
    object = sptr<Short>::MakeSptr(static_cast<short>(value));
    return object;
}
}  // namespace AAFwk
}  // namespace OHOS
