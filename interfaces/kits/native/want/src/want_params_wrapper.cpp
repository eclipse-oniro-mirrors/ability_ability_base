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
#include "want_params_wrapper.h"

#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AAFwk {
namespace {
size_t FindMatchingBrackets(const std::string &str, size_t leftIndex)
{
    int count = 0;
    for (size_t i = leftIndex + 1; i < str.size(); ++i) {
        if (str[i] == '{') {
            count++;
        } else if (str[i] == '}') {
            if (count == 0) {
                return i;
            }
            count--;
        }
    }
    return -1;
}

bool FindNextQuote(const std::string &str, size_t &strnum, size_t &pos, const char *func)
{
    strnum++;
    pos = str.find('"', strnum);
    if (pos == std::string::npos) {
        ABILITYBASE_LOGE("%{public}s: unmatched quote at pos %{public}zu", func, strnum);
        return false;
    }
    return true;
}

bool ParseTypeId(const std::string &token, int &typeId)
{
    errno = 0;
    char *end = nullptr;
    long parsed = strtol(token.c_str(), &end, 10);
    if (errno == ERANGE || end == token.c_str() || *end != '\0' || parsed > INT_MAX || parsed < INT_MIN) {
        return false;
    }
    typeId = static_cast<int>(parsed);
    if (!WantParams::IsKnownTypeId(typeId)) {
        return false;
    }
    return true;
}
}
constexpr int32_t WANT_PARAM_WRAPPER_TWO = 2;

IINTERFACE_IMPL_1(WantParamWrapper, Object, IWantParams);
const InterfaceID g_IID_IWantParams = {
    0xa75b9db6, 0x9813, 0x4371, 0x8848, {0xd, 0x2, 0x9, 0x6, 0x6, 0xc, 0xe, 0x6, 0xe, 0xc, 0x6, 0x8}
};
ErrCode WantParamWrapper::GetValue(WantParams &value)
{
    value = wantParams_;
    return ERR_OK;
}

bool WantParamWrapper::Equals(IObject &other)
{
    WantParamWrapper *otherObj = static_cast<WantParamWrapper *>(IWantParams::Query(&other));
    return otherObj != nullptr && otherObj->wantParams_ == wantParams_;
}

std::string WantParamWrapper::ToString()
{
    std::string result;
    if (wantParams_.Size() != 0) {
        result += "{";
        for (auto it : wantParams_.GetParams()) {
            int typeId = WantParams::GetDataType(it.second);
            result = result + "\"" + it.first + "\":{\"" + std::to_string(typeId) + "\":";
            if (IWantParams::Query(it.second) != nullptr) {
                result = result + static_cast<WantParamWrapper *>(IWantParams::Query(it.second))->ToString();
            } else {
                result = result + "\"" + WantParams::GetStringByType(it.second, typeId) + "\"";
            }
            if (it == *wantParams_.GetParams().rbegin()) {
                result += "}";
            } else {
                result += "},";
            }
        }
        result += "}";
    } else {
        result = "{}";
    }
    return result;
}

sptr<IWantParams> WantParamWrapper::Box(const WantParams &value)
{
    sptr<IWantParams> object = new (std::nothrow)WantParamWrapper(value);
    return object;
}

sptr<IWantParams> WantParamWrapper::Box(WantParams &&value)
{
    sptr<IWantParams> object = new (std::nothrow) WantParamWrapper(std::move(value));
    return object;
}

WantParams WantParamWrapper::Unbox(IWantParams *object)
{
    WantParams value;
    if (object != nullptr) {
        object->GetValue(value);
    }
    return value;
}
bool WantParamWrapper::ValidateStr(const std::string &str)
{
    if (str == "" || str == "{}" || str == "{\"\"}") {
        return false;
    }
    if (count(str.begin(), str.end(), '\"') % WANT_PARAM_WRAPPER_TWO != 0) {
        return false;
    }
    if (count(str.begin(), str.end(), '{') != count(str.begin(), str.end(), '}')) {
        return false;
    }
    int count = 0;
    for (auto it : str) {
        if (it == '{') {
            count++;
        }
        if (it == '}') {
            count--;
        }
        if (count < 0) {
            return false;
        }
    }
    return true;
}

sptr<IWantParams> WantParamWrapper::Parse(const std::string &str)
{
    WantParams wantParams;
    std::string key = "";
    int typeId = 0;
    if (ValidateStr(str)) {
        for (size_t strnum = 0; strnum < str.size(); strnum++) {
            if (str[strnum] == '{' && key != "" && typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
                size_t num;
                int count = 0;
                for (num = strnum; num < str.size(); num++) {
                    if (str[num] == '{') {
                        count++;
                    } else if (str[num] == '}') {
                        count--;
                    }
                    if (count == 0) {
                        break;
                    }
                }
                sptr<IWantParams> nested = WantParamWrapper::Parse(str.substr(strnum, num - strnum + 1));
                if (WantParamWrapper::Unbox(nested).Size() == 0) {
                    wantParams = WantParams();
                    break;
                }
                wantParams.SetParam(key, nested);
                key = "";
                typeId = 0;
                strnum = num + 1;
            } else if (str[strnum] == '"') {
                if (key == "") {
                    size_t pos = 0;
                    if (!FindNextQuote(str, strnum, pos, "Parse")) {
                        wantParams = WantParams();
                        break;
                    }
                    key = str.substr(strnum, pos - strnum);
                    strnum = pos;
                } else if (typeId == 0) {
                    size_t pos = 0;
                    if (!FindNextQuote(str, strnum, pos, "Parse")) {
                        wantParams = WantParams();
                        break;
                    }
                    std::string typeIdStr = str.substr(strnum, pos - strnum);
                    if (!ParseTypeId(typeIdStr, typeId)) {
                        wantParams = WantParams();
                        break;
                    }
                    strnum = pos;
                } else {
                    size_t pos = 0;
                    if (!FindNextQuote(str, strnum, pos, "Parse")) {
                        wantParams = WantParams();
                        break;
                    }
                    wantParams.SetParam(key,
                        WantParams::GetInterfaceByType(typeId, str.substr(strnum, pos - strnum)));
                    strnum = pos;
                    typeId = 0;
                    key = "";
                }
            }
        }
    }
    // Final-state guard: a clean parse leaves no pending key/typeId. A leftover
    // key/typeId means the input was truncated mid-value, so reset to empty. Catches
    // a STRING with an internal double-quote (e.g. {"9":"a"b"c"}), where the quote
    // parser stops early and strands the next key. Does not catch trailing garbage
    // (}}X) or multi-object merge (}},{), which exit in a clean state.
    if (!key.empty() || typeId != 0) {
        wantParams = WantParams();
    }
    sptr<IWantParams> iwantParams = new (std::nothrow) WantParamWrapper(wantParams);
    return iwantParams;
}

WantParams WantParamWrapper::ParseWantParams(const std::string &str)
{
    WantParams wantParams;
    std::string key = "";
    int typeId = 0;
    if (!ValidateStr(str)) {
        return wantParams;
    }
    for (size_t strnum = 0; strnum < str.size(); strnum++) {
        if (str[strnum] == '{' && key != "" && typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
            size_t num;
            int count = 0;
            for (num = strnum; num < str.size(); num++) {
                if (str[num] == '{') {
                    count++;
                } else if (str[num] == '}') {
                    count--;
                }
                if (count == 0) {
                    break;
                }
            }
            sptr<IWantParams> nested = WantParamWrapper::Parse(str.substr(strnum, num - strnum + 1));
            if (WantParamWrapper::Unbox(nested).Size() == 0) {
                wantParams = WantParams();
                break;
            }
            wantParams.SetParam(key, nested);
            key = "";
            typeId = 0;
            strnum = num + 1;
        } else if (str[strnum] == '"') {
            if (key == "") {
                size_t pos = 0;
                if (!FindNextQuote(str, strnum, pos, "ParseWantParams")) {
                    wantParams = WantParams();
                    break;
                }
                key = str.substr(strnum, pos - strnum);
                strnum = pos;
            } else if (typeId == 0) {
                size_t pos = 0;
                if (!FindNextQuote(str, strnum, pos, "ParseWantParams")) {
                    wantParams = WantParams();
                    break;
                }
                std::string typeIdStr = str.substr(strnum, pos - strnum);
                if (!ParseTypeId(typeIdStr, typeId)) {
                    wantParams = WantParams();
                    break;
                }
                strnum = pos;
            } else {
                size_t pos = 0;
                if (!FindNextQuote(str, strnum, pos, "ParseWantParams")) {
                    wantParams = WantParams();
                    break;
                }
                wantParams.SetParam(key,
                    WantParams::GetInterfaceByType(typeId, str.substr(strnum, pos - strnum)));
                strnum = pos;
                typeId = 0;
                key = "";
            }
        }
    }
    // Final-state guard -- see Parse for which truncation cases it catches.
    if (!key.empty() || typeId != 0) {
        wantParams = WantParams();
    }
    return wantParams;
}

WantParams WantParamWrapper::ParseWantParamsWithBrackets(const std::string &str)
{
    WantParams wantParams;
    std::string key = "";
    int typeId = 0;
    size_t type_index_before = 0;
    if (!ValidateStr(str)) {
        return wantParams;
    }
    for (size_t strnum = 0; strnum < str.size(); strnum++) {
        if (str[strnum] == '{' && key != "" && typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
            size_t num;
            int count = 0;
            for (num = strnum; num < str.size(); num++) {
                if (str[num] == '{') {
                    count++;
                } else if (str[num] == '}') {
                    count--;
                }
                if (count == 0) {
                    break;
                }
            }
            sptr<IWantParams> nested = WantParamWrapper::Parse(str.substr(strnum, num - strnum + 1));
            if (WantParamWrapper::Unbox(nested).Size() == 0) {
                wantParams = WantParams();
                break;
            }
            wantParams.SetParam(key, nested);
            key = "";
            typeId = 0;
            strnum = num + 1;
        } else if (str[strnum] == '"') {
            if (key == "") {
                size_t pos = 0;
                if (!FindNextQuote(str, strnum, pos, "ParseWantParamsWithBrackets")) {
                    wantParams = WantParams();
                    break;
                }
                key = str.substr(strnum, pos - strnum);
                strnum = pos;
            } else if (typeId == 0) {
                type_index_before = strnum;
                size_t pos = 0;
                if (!FindNextQuote(str, strnum, pos, "ParseWantParamsWithBrackets")) {
                    wantParams = WantParams();
                    break;
                }
                std::string typeIdStr = str.substr(strnum, pos - strnum);
                if (!ParseTypeId(typeIdStr, typeId)) {
                    wantParams = WantParams();
                    break;
                }
                strnum = pos;
            } else {
                strnum++;
                auto index = FindMatchingBrackets(str, type_index_before - 1);
                if (index == std::string::npos) {
                    wantParams = WantParams();
                    break;
                }
                wantParams.SetParam(key,
                    WantParams::GetInterfaceByType(typeId, str.substr(strnum, index - 1 - strnum)));
                strnum = index + 1;
                typeId = 0;
                key = "";
            }
        }
    }
    // Final-state guard -- see Parse for which truncation cases it catches.
    if (!key.empty() || typeId != 0) {
        wantParams = WantParams();
    }
    return wantParams;
}
}  // namespace AAFwk
}  // namespace OHOS