
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
#include <algorithm>

#include "extra_params.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
/**
 * @brief A default constructor used to create an empty {@code ExtraParams} instance.
 * @param none
 */
ExtraParams::ExtraParams()
{}
/**
 * @brief A copy constructor used to create an empty {@code ExtraParams} instance.
 * @param other indicates object instance.
 */
ExtraParams::ExtraParams(const ExtraParams &other)
{
    devType_ = other.devType_;
    targetBundleName_ = other.targetBundleName_;
    description_ = other.description_;
    jsonParams_ = other.jsonParams_;
}
/**
 * @brief Overloading assignment operators to create the same object instance.
 * @param other Indicates object instance.
 */
ExtraParams &ExtraParams::operator=(const ExtraParams &other)
{
    if (this != &other) {
        devType_ = other.devType_;
        targetBundleName_ = other.targetBundleName_;
        description_ = other.description_;
        jsonParams_ = other.jsonParams_;
    }
    return *this;
}
/**
 * @brief Judge whether the parameter of extraparam instance is equal to that of other instance parameter
 * @param other Indicates object instance.
 * @return returns true the parameter of extraparam instance is equal to that of other instance parameter
 * otherwise false
 */
bool ExtraParams::operator==(const ExtraParams &other) const
{
    if (targetBundleName_ != other.targetBundleName_ || description_ != other.description_ ||
        jsonParams_ != other.jsonParams_) {
        return false;
    }
    if (devType_.size() != other.devType_.size()) {
        return false;
    }
    std::vector<string> left = devType_;
    std::vector<string> right = other.devType_;
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    return left == right;
}
/**
 * @brief A constructor used to create an {@code ExtraParams} instance based on the input parameters{@code devType},
 * {@code targetBundleName}, and {@code description}.
 *
 * @param devType Indicates the type of devices to be matched. This parameter can be any combination of
 * {@link #DEVICETYPE_SMART_PHONE}, {@link #DEVICETYPE_SMART_PAD}, {@link #DEVICETYPE_SMART_WATCH}, and
 * {@link #DEVICETYPE_SMART_TV}.
 *
 * @param targetBundleName Indicates the bundle name of the target application where the ability will be migrated.
 *
 * @param description Indicates the description used for device filtering.
 *
 * @param jsonParams Indicates the custom JSON parameters to be used as filter conditions.
 *
 * @return none
 */
ExtraParams::ExtraParams(const std::vector<string> &devType, const string &targetBundleName, const string &description,
    const string &jsonParams)
{
    devType_ = devType;
    targetBundleName_ = targetBundleName;
    description_ = description;
    jsonParams_ = jsonParams;
}

/**
 * @brief A destructor release an empty {@code ExtraParams} instance.
 * @param none
 */
ExtraParams::~ExtraParams()
{}

/**
 * @brief Sets the list of device types.
 *
 * @param devType Indicates the type of devices to be matched. This parameter can be any combination of
 * {@link #DEVICETYPE_SMART_PHONE}, {@link #DEVICETYPE_SMART_PAD}, {@link #DEVICETYPE_SMART_WATCH}, and
 * {@link #DEVICETYPE_SMART_TV}.
 *
 * @return none
 */
void ExtraParams::SetDevType(const std::vector<string> &devType)
{
    devType_ = devType;
}

/**
 * @brief Obtains the list of device types.
 *
 * @param none
 *
 * @return Returns the list of device types.
 */
std::vector<string> ExtraParams::GetDevType() const
{
    return devType_;
}

/**
 * @brief Sets the bundle name of the target application where ability will be migrated.
 *
 * @param targetBundleName Indicates the bundle name of the target application to set.
 *
 * @return none
 */
void ExtraParams::SetTargetBundleName(const string &targetBundleName)
{
    targetBundleName_ = targetBundleName;
}

/**
 * @brief Obtains the bundle name of the target application where the ability will be migrated.
 *
 * @return Returns the bundle name of the target application.
 */
string ExtraParams::GetTargetBundleName() const
{
    return targetBundleName_;
}

/**
 * @brief Sets the description used for device filtering.
 *
 * @param jsonParams Indicates the device description to set.
 *
 * @return none
 */
void ExtraParams::SetJsonParams(const string &jsonParams)
{
    jsonParams_ = jsonParams;
}

/**
 * @brief Obtains the custom JSON parameters used as filter conditions.
 *
 * @param none
 *
 * @return Returns the custom JSON parameters.
 */
string ExtraParams::GetJsonParams() const
{
    return jsonParams_;
}

/**
 * @brief Sets the custom JSON parameters to be used as filter conditions.
 *
 * @param description Indicates the custom JSON parameters to set.
 *
 */
void ExtraParams::SetDescription(const string &description)
{
    description_ = description;
}

/**
 * @brief Obtains the description used for device filtering.
 *
 * @param none
 *
 * @return Returns the description used for device filtering.
 */
string ExtraParams::GetDescription() const
{
    return description_;
}

/**
 * @brief Marshals this {@code ExtraParams} object into a {@link ohos.utils.Parcel} object.
 *
 * @param parcel Indicates the {@code Parcel} object for marshalling.
 *
 * @return Returns {@code true} if the marshalling is successful; returns {@code false} otherwise.
 */
bool ExtraParams::Marshalling(Parcel &parcel) const
{
    // devType
    bool writeDevType = parcel.WriteStringVector(devType_);

    // targetBundleName
    bool writeTargetBundleName = parcel.WriteString16(Str8ToStr16(targetBundleName_));

    // description
    bool writeDescription = parcel.WriteString16(Str8ToStr16(description_));

    // jsonParams
    bool writeJsonParams = parcel.WriteString16(Str8ToStr16(jsonParams_));

    return writeDevType && writeTargetBundleName && writeDescription && writeJsonParams;
}

/**
 * @brief Unmarshals this {@code ExtraParams} object from a {@link ohos.utils.Parcel} object.
 *
 * @param parcel Indicates the {@code Parcel} object for unmarshalling.
 *
 * @return Returns {@code true} if the unmarshalling is successful; returns {@code false} otherwise.
 */
ExtraParams *ExtraParams::Unmarshalling(Parcel &parcel)
{
    // devType.
    std::vector<string> devtype;
    if (!parcel.ReadStringVector(&devtype)) {
        return nullptr;
    }

    // targetBundleName
    std::u16string targetBundleName16;
    if (!parcel.ReadString16(targetBundleName16)) {
        return nullptr;
    }
    string targetBundleName = Str16ToStr8(targetBundleName16);

    // description
    std::u16string description16;
    if (!parcel.ReadString16(description16)) {
        return nullptr;
    }
    string description = Str16ToStr8(description16);

    // jsonParams
    std::u16string jsonParams16;
    if (!parcel.ReadString16(jsonParams16)) {
        return nullptr;
    }
    string jsonParams = Str16ToStr8(jsonParams16);

    ExtraParams *extraParams = new (std::nothrow) ExtraParams(devtype, targetBundleName, description, jsonParams);

    return extraParams;
}
}  // namespace AppExecFwk
}  // namespace OHOS
