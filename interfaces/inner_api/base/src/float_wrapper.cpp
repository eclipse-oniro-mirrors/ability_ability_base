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

#include "float_wrapper.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AAFwk {
IINTERFACE_IMPL_1(Float, Object, IFloat);

ErrCode Float::GetValue(float &value) /* [out] */
{
    value = value_;
    return ERR_OK;
}

bool Float::Equals(IObject &other) /* [in] */
{
    Float *otherObj = static_cast<Float *>(IFloat::Query(&other));
    if (otherObj == nullptr) {
        return false;
    }
    if (value_ == otherObj->value_) {
        return true;
    }
    if (!std::isfinite(value_) || !std::isfinite(otherObj->value_)) {
        return false;
    }
    constexpr float EPSILON = std::numeric_limits<float>::epsilon();
    float diff = std::abs(otherObj->value_ - value_);
    float maxVal = std::max(std::abs(value_), std::abs(otherObj->value_));
    return diff <= EPSILON * maxVal;
}

std::string Float::ToString()
{
    return std::to_string(value_);
}

sptr<IFloat> Float::Box(float value) /* [in] */
{
    sptr<IFloat> object = sptr<Float>::MakeSptr(value);
    return object;
}

float Float::Unbox(IFloat *object) /* [in] */
{
    float value = 0;
    if (object == nullptr) {
        return value;
    }
    object->GetValue(value);
    return value;
}

sptr<IFloat> Float::Parse(const std::string &str) /* [in] */
{
    sptr<IFloat> object;
    std::size_t idx;
    try {
        float value = std::stof(str, &idx);
        if (idx == str.length()) {
            object = sptr<Float>::MakeSptr(value);
        }
    } catch (...) {
        ABILITYBASE_LOGE("failed to convert to float: %{public}s", str.c_str());
    }
    return object;
}
}  // namespace AAFwk
}  // namespace OHOS
