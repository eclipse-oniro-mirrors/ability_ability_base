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

#include "configuration.h"

#include <mutex>
#include <nlohmann/json.hpp>

#include "ability_base_log_wrapper.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
constexpr int CYCLE_LIMIT = 1000;
}
using json = nlohmann::json;
Configuration::Configuration()
{}

Configuration::Configuration(const Configuration &other) : defaultDisplayId_(other.defaultDisplayId_)
{
    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    configParameter_ = other.configParameter_;
}

Configuration& Configuration::operator=(const Configuration &other)
{
    if (this == &other) {
        return *this;
    }

    defaultDisplayId_ = other.defaultDisplayId_;

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    configParameter_.clear();
    configParameter_ = other.configParameter_;
    return *this;
}

Configuration::~Configuration()
{}

bool Configuration::MakeTheKey(std::string &getKey, int id, const std::string &param) const
{
    if (param.empty()) {
        return false;
    }

    const std::vector<std::string> SystemConfigurationKeyStore {
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_LANGUAGE,
        OHOS::AAFwk::GlobalConfigurationKey::IS_PREFERRED_LANGUAGE,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_LOCALE,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_HOUR,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_COLORMODE,
        OHOS::AAFwk::GlobalConfigurationKey::INPUT_POINTER_DEVICE,
        OHOS::AAFwk::GlobalConfigurationKey::DEVICE_TYPE,
        OHOS::AAFwk::GlobalConfigurationKey::THEME,
        OHOS::AAFwk::GlobalConfigurationKey::THEME_ID,
        OHOS::AAFwk::GlobalConfigurationKey::THEME_ICON,
        OHOS::AAFwk::GlobalConfigurationKey::THEME_SKIN,
        OHOS::AAFwk::GlobalConfigurationKey::COLORMODE_IS_SET_BY_SA,
        OHOS::AAFwk::GlobalConfigurationKey::COLORMODE_IS_SET_BY_APP,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_FONT_ID,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_FONT_SIZE_SCALE,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_FONT_WEIGHT_SCALE,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_MCC,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_MNC,
        OHOS::AppExecFwk::ConfigurationInner::APPLICATION_DIRECTION,
        OHOS::AppExecFwk::ConfigurationInner::APPLICATION_DENSITYDPI,
        OHOS::AppExecFwk::ConfigurationInner::APPLICATION_DISPLAYID,
        OHOS::AppExecFwk::ConfigurationInner::APPLICATION_FONT,
        OHOS::AAFwk::GlobalConfigurationKey::APP_FONT_SIZE_SCALE,
        OHOS::AAFwk::GlobalConfigurationKey::APP_FONT_MAX_SCALE,
        OHOS::AAFwk::GlobalConfigurationKey::SYSTEM_SMART_GESTURE_SWITCH,
    };
    if (std::find(SystemConfigurationKeyStore.begin(), SystemConfigurationKeyStore.end(), param) ==
        SystemConfigurationKeyStore.end()) {
        return false;
    }

    getKey.clear();
    getKey += std::to_string(id);
    getKey += ConfigurationInner::CONNECTION_SYMBOL;
    getKey += param;
    ABILITYBASE_LOGD("key: %{public}s", getKey.c_str());

    return true;
}

bool Configuration::AddItem(int displayId, const std::string &key, const std::string &value)
{
    if (key.empty() || value.empty()) {
        return false;
    }

    std::string getKey;
    if (!MakeTheKey(getKey, displayId, key)) {
        return false;
    }

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    configParameter_[getKey] = value;
    return true;
}

std::string Configuration::GetItem(int displayId, const std::string &key) const
{
    if (key.empty()) {
        return ConfigurationInner::EMPTY_STRING;
    }

    std::string getKey;
    if (!MakeTheKey(getKey, displayId, key)) {
        return ConfigurationInner::EMPTY_STRING;
    }

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    auto iter = configParameter_.find(getKey);
    if (iter != configParameter_.end()) {
        return iter->second;
    }

    return ConfigurationInner::EMPTY_STRING;
}

int Configuration::GetItemSize() const
{
    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    return configParameter_.size();
}

void Configuration::GetAllKey(std::vector<std::string> &keychain) const
{
    keychain.clear();

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    for (const auto &it :configParameter_) {
        keychain.push_back(it.first);
    }
}

std::string Configuration::GetValue(const std::string &key) const
{
    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    auto iter = configParameter_.find(key);
    if (iter != configParameter_.end()) {
        return iter->second;
    }

    return ConfigurationInner::EMPTY_STRING;
}

void Configuration::CompareDifferent(std::vector<std::string> &diffKeyV, const Configuration &other) const
{
    diffKeyV.clear();
    if (other.GetItemSize() == 0) {
        return;
    }
    std::vector<std::string> otherk;
    other.GetAllKey(otherk);

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    for (const auto &iter : otherk) {
        ABILITYBASE_LOGD("iter:%{public}s,Val:%{public}s", iter.c_str(), other.GetValue(iter).c_str());
        auto otherItem = other.GetValue(iter);
        if (otherItem.empty()) {
            continue;
        }
        if (otherItem != GetValue(iter)) {
            diffKeyV.push_back(iter);
        }
    }
}

void Configuration::Merge(const std::vector<std::string> &diffKeyV, const Configuration &other)
{
    if (diffKeyV.empty()) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    for (const auto &mergeItemKey : diffKeyV) {
        auto myItem = GetValue(mergeItemKey);
        auto otherItem = other.GetValue(mergeItemKey);
        // myItem possible empty
        if (!otherItem.empty() && otherItem != myItem) {
            configParameter_[mergeItemKey] = otherItem;
        }
    }
}

int Configuration::RemoveItem(int displayId, const std::string &key)
{
    if (key.empty()) {
        return 0;
    }

    std::string getKey;
    if (!MakeTheKey(getKey, displayId, key)) {
        return 0;
    }

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    return configParameter_.erase(getKey);
}

bool Configuration::AddItem(const std::string &key, const std::string &value)
{
    return AddItem(defaultDisplayId_, key, value);
}

std::string Configuration::GetItem(const std::string &key) const
{
    return GetItem(defaultDisplayId_, key);
}

int Configuration::RemoveItem(const std::string &key)
{
    return RemoveItem(defaultDisplayId_, key);
}

const std::string Configuration::GetName() const
{
    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    json configArray(configParameter_);
    std::string configArrayStr = "";
    try {
        configArrayStr = configArray.dump();
    } catch (...) {
        ABILITYBASE_LOGE("failed to dump GetName");
    }
    return configArrayStr;
}

bool Configuration::ReadFromParcel(Parcel &parcel)
{
    ABILITYBASE_LOGD("ReadFromParcel");
    int32_t defaultDisplayId = parcel.ReadInt32();
    int32_t configSize = parcel.ReadInt32();
    std::vector<std::string> keys;
    std::vector<std::string> values;
    if (!parcel.ReadStringVector(&keys)) {
        ABILITYBASE_LOGE("read keys failed");
        return false;
    }
    if (!parcel.ReadStringVector(&values)) {
        ABILITYBASE_LOGE("read values failed");
        return false;
    }
    size_t keySize = keys.size();
    size_t valueSize = values.size();
    if (keySize != valueSize || configSize != (int32_t)valueSize || configSize > CYCLE_LIMIT) {
        ABILITYBASE_LOGE("invalid size");
        return false;
    }

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    defaultDisplayId_ = defaultDisplayId;
    configParameter_.clear();
    for (int32_t i = 0; i < configSize; i++) {
        configParameter_.emplace(keys.at(i), values.at(i));
    }
    return true;
}

Configuration *Configuration::Unmarshalling(Parcel &parcel)
{
    Configuration *Configuration = new (std::nothrow) OHOS::AppExecFwk::Configuration();
    if (Configuration && !Configuration->ReadFromParcel(parcel)) {
        delete Configuration;
        Configuration = nullptr;
    }
    return Configuration;
}

bool Configuration::Marshalling(Parcel &parcel) const
{
    ABILITYBASE_LOGD("called");
    if (!parcel.WriteInt32(defaultDisplayId_)) {
        ABILITYBASE_LOGE("write defaultDisplayId_ failed");
        return false;
    }

    std::vector<std::string> keys;
    std::vector<std::string> values;

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    if (!parcel.WriteInt32(configParameter_.size())) {
        ABILITYBASE_LOGE("write config size failed");
        return false;
    }
    for (const auto &config : configParameter_) {
        keys.emplace_back(config.first);
        values.emplace_back(config.second);
    }

    if (!parcel.WriteStringVector(keys)) {
        ABILITYBASE_LOGE("write keys failed");
        return false;
    }
    if (!parcel.WriteStringVector(values)) {
        ABILITYBASE_LOGE("write values failed");
        return false;
    }
    return true;
}

void Configuration::FilterDuplicates(const Configuration &other)
{
    if (other.GetItemSize() == 0) {
        return;
    }
    std::vector<std::string> otherk;
    other.GetAllKey(otherk);

    std::lock_guard<std::recursive_mutex> lock(configParameterMutex_);
    for (const auto &iter : otherk) {
        auto myItem = GetValue(iter);
        auto otherItem = other.GetValue(iter);
        // myItem possible empty
        if (!otherItem.empty() && !myItem.empty()) {
            configParameter_.erase(iter);
        }
    }
}
}  // namespace AppExecFwk
}  // namespace OHOS
