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
#include "array_wrapper.h"

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
    return std::string::npos;
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
constexpr int32_t MAX_PARSE_RECURSION_DEPTH = 100;

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
    return ToString(0);
}

std::string WantParamWrapper::ToString(int depth)
{
    std::string result;
    if (depth < 0 || depth > MAX_PARSE_RECURSION_DEPTH) {
        ABILITYBASE_LOGE("WantParamWrapper::ToString invalid recursion depth %{public}d", depth);
        return result;
    }
    if (wantParams_.Size() != 0) {
        result += "{";
        for (auto it : wantParams_.GetParams()) {
            int typeId = WantParams::GetDataType(it.second);
            result = result + "\"" + it.first + "\":{\"" + std::to_string(typeId) + "\":";
            if (!AppendParamValueString(it.second, typeId, depth, result)) {
                return "";
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

bool WantParamWrapper::AppendParamValueString(IInterface *value, int typeId, int depth, std::string &result)
{
    if (IWantParams::Query(value) != nullptr) {
        std::string valueStr = static_cast<WantParamWrapper *>(IWantParams::Query(value))->ToString(depth + 1);
        if (valueStr.empty()) {
            return false;
        }
        result += valueStr;
        return true;
    }
    if (IArray::Query(value) != nullptr) {
        std::string valueStr = static_cast<Array *>(IArray::Query(value))->ToString(depth + 1);
        if (valueStr.empty()) {
            return false;
        }
        result = result + "\"" + valueStr + "\"";
        return true;
    }
    result = result + "\"" + WantParams::GetStringByType(value, typeId) + "\"";
    return true;
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
        ABILITYBASE_LOGE("ValidateStr: invalid content, length=%{public}zu", str.size());
        return false;
    }
    size_t quoteCount = count(str.begin(), str.end(), '\"');
    if (quoteCount % WANT_PARAM_WRAPPER_TWO != 0) {
        ABILITYBASE_LOGE("ValidateStr: odd quote count=%{public}zu, length=%{public}zu", quoteCount, str.size());
        return false;
    }
    size_t leftBrace = count(str.begin(), str.end(), '{');
    size_t rightBrace = count(str.begin(), str.end(), '}');
    if (leftBrace != rightBrace) {
        ABILITYBASE_LOGE("ValidateStr: brace count mismatch, left=%{public}zu right=%{public}zu",
            leftBrace, rightBrace);
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
            ABILITYBASE_LOGE("ValidateStr: brace order invalid, length=%{public}zu", str.size());
            return false;
        }
    }
    return true;
}

void WantParamWrapper::FindNestedWantParamsEnd(const std::string &str, size_t strnum, size_t &num)
{
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
}

bool WantParamWrapper::ParseNestedWantParams(const std::string &str, size_t &strnum, int depth, ParseState &state)
{
    size_t num;
    FindNestedWantParamsEnd(str, strnum, num);
    std::string nestedStr = str.substr(strnum, num - strnum + 1);
    bool isEmptyNested = nestedStr == "{}";
    sptr<IWantParams> nested;
    if (isEmptyNested) {
        nested = WantParamWrapper::Box(WantParams());
    } else {
        nested = WantParamWrapper::Parse(nestedStr, depth + 1);
    }
    if (nested == nullptr || (!isEmptyNested && WantParamWrapper::Unbox(nested).Size() == 0)) {
        ABILITYBASE_LOGE("ParseNestedWantParams: nested parse failed, key=%{public}s depth=%{public}d",
            state.key.c_str(), depth);
        state.wantParams = WantParams();
        return false;
    }
    state.wantParams.SetParam(state.key, nested);
    state.key = "";
    state.typeId = 0;
    strnum = num + 1;
    return true;
}

sptr<IInterface> WantParamWrapper::ParseValueByType(int typeId, const std::string &value, int depth)
{
    if (typeId == WantParams::VALUE_TYPE_ARRAY) {
        return Array::Parse(value, depth + 1);
    }
    return WantParams::GetInterfaceByType(typeId, value);
}

bool WantParamWrapper::ParseQuotedParam(const std::string &str, size_t &strnum, int depth, ParseState &state,
    const char *func)
{
    size_t pos = 0;
    if (state.key == "") {
        if (!FindNextQuote(str, strnum, pos, func)) {
            state.wantParams = WantParams();
            return false;
        }
        state.key = str.substr(strnum, pos - strnum);
        strnum = pos;
        return true;
    }
    if (state.typeId == 0) {
        if (!FindNextQuote(str, strnum, pos, func)) {
            state.wantParams = WantParams();
            return false;
        }
        std::string typeIdStr = str.substr(strnum, pos - strnum);
        if (!ParseTypeId(typeIdStr, state.typeId)) {
            ABILITYBASE_LOGE("%{public}s: ParseTypeId failed, typeIdStr=%{public}s", func, typeIdStr.c_str());
            state.wantParams = WantParams();
            return false;
        }
        strnum = pos;
        return true;
    }
    if (!FindNextQuote(str, strnum, pos, func)) {
        state.wantParams = WantParams();
        return false;
    }
    sptr<IInterface> value = ParseValueByType(state.typeId, str.substr(strnum, pos - strnum), depth);
    state.wantParams.SetParam(state.key, value);
    strnum = pos;
    state.typeId = 0;
    state.key = "";
    return true;
}

bool WantParamWrapper::ParseQuotedParamWithBrackets(const std::string &str, size_t &strnum, int depth,
    ParseState &state, const char *func)
{
    size_t pos = 0;
    if (state.key == "") {
        if (!FindNextQuote(str, strnum, pos, func)) {
            state.wantParams = WantParams();
            return false;
        }
        state.key = str.substr(strnum, pos - strnum);
        strnum = pos;
        return true;
    }
    if (state.typeId == 0) {
        state.typeIndexBefore = strnum;
        if (!FindNextQuote(str, strnum, pos, func)) {
            state.wantParams = WantParams();
            return false;
        }
        std::string typeIdStr = str.substr(strnum, pos - strnum);
        if (!ParseTypeId(typeIdStr, state.typeId)) {
            ABILITYBASE_LOGE("%{public}s: ParseTypeId failed, typeIdStr=%{public}s", func, typeIdStr.c_str());
            state.wantParams = WantParams();
            return false;
        }
        strnum = pos;
        return true;
    }
    strnum++;
    auto index = FindMatchingBrackets(str, state.typeIndexBefore - 1);
    if (index == std::string::npos) {
        ABILITYBASE_LOGE("%{public}s: unmatched brackets, strnum=%{public}zu", func, strnum);
        state.wantParams = WantParams();
        return false;
    }

    if (index <= strnum) {
        ABILITYBASE_LOGE("%{public}s: malformed bracketed value, index=%{public}zu strnum=%{public}zu",
            func, index, strnum);
        state.wantParams = WantParams();
        return false;
    }
    sptr<IInterface> value = ParseValueByType(state.typeId, str.substr(strnum, index - 1 - strnum), depth);
    state.wantParams.SetParam(state.key, value);
    strnum = index + 1;
    state.typeId = 0;
    state.key = "";
    return true;
}

void WantParamWrapper::ResetIfParseIncomplete(ParseState &state)
{
    if (!state.key.empty() || state.typeId != 0) {
        ABILITYBASE_LOGE("ResetIfParseIncomplete: truncated input, key=%{public}s typeId=%{public}d",
            state.key.c_str(), state.typeId);
        state.wantParams = WantParams();
    }
}

sptr<IWantParams> WantParamWrapper::Parse(const std::string &str)
{
    return Parse(str, 0);
}

sptr<IWantParams> WantParamWrapper::Parse(const std::string &str, int depth)
{
    ParseState state;
    if (depth < 0 || depth > MAX_PARSE_RECURSION_DEPTH) {
        ABILITYBASE_LOGE("Parse: invalid recursion depth %{public}d", depth);
        return new (std::nothrow) WantParamWrapper(state.wantParams);
    }
    if (!ValidateStr(str)) {
        return new (std::nothrow) WantParamWrapper(state.wantParams);
    }
    for (size_t strnum = 0; strnum < str.size(); strnum++) {
        if (str[strnum] == '{' && state.key != "" && state.typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
            if (!ParseNestedWantParams(str, strnum, depth, state)) {
                break;
            }
        } else if (str[strnum] == '"') {
            if (!ParseQuotedParam(str, strnum, depth, state, "Parse")) {
                break;
            }
        }
    }
    // Final-state guard: a clean parse leaves no pending key/typeId. A leftover
    // key/typeId means the input was truncated mid-value, so reset to empty. Catches
    // a STRING with an internal double-quote (e.g. {"9":"a"b"c"}), where the quote
    // parser stops early and strands the next key. Does not catch trailing garbage
    // (}}X) or multi-object merge (}},{), which exit in a clean state.
    ResetIfParseIncomplete(state);
    sptr<IWantParams> iwantParams = new (std::nothrow) WantParamWrapper(state.wantParams);
    return iwantParams;
}

WantParams WantParamWrapper::ParseWantParams(const std::string &str)
{
    return ParseWantParams(str, 0);
}

WantParams WantParamWrapper::ParseWantParams(const std::string &str, int depth)
{
    ParseState state;
    if (depth < 0 || depth > MAX_PARSE_RECURSION_DEPTH) {
        ABILITYBASE_LOGE("ParseWantParams: invalid recursion depth %{public}d", depth);
        return state.wantParams;
    }
    if (!ValidateStr(str)) {
        return state.wantParams;
    }
    for (size_t strnum = 0; strnum < str.size(); strnum++) {
        if (str[strnum] == '{' && state.key != "" && state.typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
            if (!ParseNestedWantParams(str, strnum, depth, state)) {
                break;
            }
        } else if (str[strnum] == '"') {
            if (!ParseQuotedParam(str, strnum, depth, state, "ParseWantParams")) {
                break;
            }
        }
    }
    // Final-state guard -- see Parse for which truncation cases it catches.
    ResetIfParseIncomplete(state);
    return state.wantParams;
}

WantParams WantParamWrapper::ParseWantParamsWithBrackets(const std::string &str)
{
    return ParseWantParamsWithBrackets(str, 0);
}

WantParams WantParamWrapper::ParseWantParamsWithBrackets(const std::string &str, int depth)
{
    ParseState state;
    if (depth < 0 || depth > MAX_PARSE_RECURSION_DEPTH) {
        ABILITYBASE_LOGE("ParseWantParamsWithBrackets: invalid recursion depth %{public}d", depth);
        return state.wantParams;
    }
    if (!ValidateStr(str)) {
        return state.wantParams;
    }
    for (size_t strnum = 0; strnum < str.size(); strnum++) {
        if (str[strnum] == '{' && state.key != "" && state.typeId == WantParams::VALUE_TYPE_WANTPARAMS) {
            if (!ParseNestedWantParams(str, strnum, depth, state)) {
                break;
            }
        } else if (str[strnum] == '"') {
            if (!ParseQuotedParamWithBrackets(str, strnum, depth, state, "ParseWantParamsWithBrackets")) {
                break;
            }
        }
    }
    // Final-state guard -- see Parse for which truncation cases it catches.
    ResetIfParseIncomplete(state);
    return state.wantParams;
}
}  // namespace AAFwk
}  // namespace OHOS
