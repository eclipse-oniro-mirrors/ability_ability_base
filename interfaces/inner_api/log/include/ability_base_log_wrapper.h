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

#ifndef OHOS_ABILITY_BASE_ABILITY_BASE_HILOG_WRAPPER_H
#define OHOS_ABILITY_BASE_ABILITY_BASE_HILOG_WRAPPER_H

#define ABILITYBASE_CONFIG_HILOG
#ifdef ABILITYBASE_CONFIG_HILOG
#include "hilog/log.h"

#ifdef ABILITYBASE_LOGF
#undef ABILITYBASE_LOGF
#endif

#ifdef ABILITYBASE_LOGE
#undef ABILITYBASE_LOGE
#endif

#ifdef ABILITYBASE_LOGW
#undef ABILITYBASE_LOGW
#endif

#ifdef ABILITYBASE_LOGI
#undef ABILITYBASE_LOGI
#endif

#ifdef ABILITYBASE_LOGD
#undef ABILITYBASE_LOGD
#endif

#ifndef ABILITYBASE_LOG_DOMAIN
#define ABILITYBASE_LOG_DOMAIN 0xD001305
#endif

#ifndef ABILITYBASE_LOG_TAG
#define ABILITYBASE_LOG_TAG "Base"
#endif

namespace OHOS::AbilityBase {
[[maybe_unused]] static constexpr const char* ConvertFileName(const char* fileName)
{
#define ABILITYBASE_FILENAME_MAPPING \
    ABILITYBASE_FILENAME_PAIR("array_wrapper.cpp", "AW") \
    ABILITYBASE_FILENAME_PAIR("array_wrapper_json.cpp", "AWJ") \
    ABILITYBASE_FILENAME_PAIR("byte_wrapper.cpp", "BW") \
    ABILITYBASE_FILENAME_PAIR("configuration.cpp", "CFG") \
    ABILITYBASE_FILENAME_PAIR("configuration_convertor.cpp", "CC") \
    ABILITYBASE_FILENAME_PAIR("double_wrapper.cpp", "DW") \
    ABILITYBASE_FILENAME_PAIR("element_name.cpp", "EN") \
    ABILITYBASE_FILENAME_PAIR("extractor.cpp", "EXT") \
    ABILITYBASE_FILENAME_PAIR("file_mapper.cpp", "FM") \
    ABILITYBASE_FILENAME_PAIR("float_wrapper.cpp", "FW") \
    ABILITYBASE_FILENAME_PAIR("int_wrapper.cpp", "IW") \
    ABILITYBASE_FILENAME_PAIR("long_wrapper.cpp", "LW") \
    ABILITYBASE_FILENAME_PAIR("operation.cpp", "OP") \
    ABILITYBASE_FILENAME_PAIR("pac_map.cpp", "PM") \
    ABILITYBASE_FILENAME_PAIR("page_node_info.cpp", "PNI") \
    ABILITYBASE_FILENAME_PAIR("patterns_matcher.cpp", "PTM") \
    ABILITYBASE_FILENAME_PAIR("rect.cpp", "RECT") \
    ABILITYBASE_FILENAME_PAIR("session_info.cpp", "SI") \
    ABILITYBASE_FILENAME_PAIR("short_wrapper.cpp", "SW") \
    ABILITYBASE_FILENAME_PAIR("view_data.cpp", "VD") \
    ABILITYBASE_FILENAME_PAIR("want.cpp", "WANT") \
    ABILITYBASE_FILENAME_PAIR("want_manager.cpp", "WM") \
    ABILITYBASE_FILENAME_PAIR("want_params.cpp", "WP") \
    ABILITYBASE_FILENAME_PAIR("want_params_wrapper.cpp", "WPW") \
    ABILITYBASE_FILENAME_PAIR("want_params_wrapper_json.cpp", "WPWJ") \
    ABILITYBASE_FILENAME_PAIR("zip_file.cpp", "ZF") \
    ABILITYBASE_FILENAME_PAIR("zip_file_reader.cpp", "ZFR") \
    ABILITYBASE_FILENAME_PAIR("zip_file_reader_io.cpp", "ZFRIO")
#define ABILITYBASE_FILENAME_PAIR(name, abbr) \
    if (__builtin_strcmp(fileName, name) == 0) { return abbr; }
    ABILITYBASE_FILENAME_MAPPING
#undef ABILITYBASE_FILENAME_PAIR
#undef ABILITYBASE_FILENAME_MAPPING
    return fileName;
}
} // namespace OHOS::AbilityBase

#ifndef ABILITYBASE_FILE_NAME
#ifdef __FILE_NAME__
#define ABILITYBASE_FILE_NAME __FILE_NAME__
#else
#define ABILITYBASE_FILE_NAME \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#endif

#ifndef ABILITYBASE_FUNC_FMT
#define ABILITYBASE_FUNC_FMT "[%{public}s%{public}d]"
#endif

#define ABILITYBASE_LOGF(fmt, ...) \
    ((void)HILOG_IMPL(LOG_CORE, LOG_FATAL, ABILITYBASE_LOG_DOMAIN, ABILITYBASE_LOG_TAG, \
        ABILITYBASE_FUNC_FMT fmt, \
        OHOS::AbilityBase::ConvertFileName(ABILITYBASE_FILE_NAME), \
        __LINE__, ##__VA_ARGS__))
#define ABILITYBASE_LOGE(fmt, ...) \
    ((void)HILOG_IMPL(LOG_CORE, LOG_ERROR, ABILITYBASE_LOG_DOMAIN, ABILITYBASE_LOG_TAG, \
        ABILITYBASE_FUNC_FMT fmt, \
        OHOS::AbilityBase::ConvertFileName(ABILITYBASE_FILE_NAME), \
        __LINE__, ##__VA_ARGS__))
#define ABILITYBASE_LOGW(fmt, ...) \
    ((void)HILOG_IMPL(LOG_CORE, LOG_WARN, ABILITYBASE_LOG_DOMAIN, ABILITYBASE_LOG_TAG, \
        ABILITYBASE_FUNC_FMT fmt, \
        OHOS::AbilityBase::ConvertFileName(ABILITYBASE_FILE_NAME), \
        __LINE__, ##__VA_ARGS__))
#define ABILITYBASE_LOGI(fmt, ...) \
    ((void)HILOG_IMPL(LOG_CORE, LOG_INFO, ABILITYBASE_LOG_DOMAIN, ABILITYBASE_LOG_TAG, \
        ABILITYBASE_FUNC_FMT fmt, \
        OHOS::AbilityBase::ConvertFileName(ABILITYBASE_FILE_NAME), \
        __LINE__, ##__VA_ARGS__))
#define ABILITYBASE_LOGD(fmt, ...) \
    ((void)HILOG_IMPL(LOG_CORE, LOG_DEBUG, ABILITYBASE_LOG_DOMAIN, ABILITYBASE_LOG_TAG, \
        ABILITYBASE_FUNC_FMT fmt, \
        OHOS::AbilityBase::ConvertFileName(ABILITYBASE_FILE_NAME), \
        __LINE__, ##__VA_ARGS__))
#else

#define ABILITYBASE_LOGF(...)
#define ABILITYBASE_LOGE(...)
#define ABILITYBASE_LOGW(...)
#define ABILITYBASE_LOGI(...)
#define ABILITYBASE_LOGD(...)
#endif // ABILITYBASE_CONFIG_HILOG

#endif // OHOS_ABILITY_BASE_ABILITY_BASE_HILOG_WRAPPER_H
