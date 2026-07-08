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
#ifndef OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_H
#define OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_H
#include <cstddef>

#include "base_def.h"
#include "base_obj.h"
#include "want_params.h"
namespace OHOS {
namespace AAFwk {
class Array;
class WantParams;
INTERFACE(IWantParams, a75b9db6 - 9813 - 4371 - 8848 - d2966ce6ec68)
{
    static IWantParams *Query(IInterface * object)
    {
        if (object == nullptr) {
            return nullptr;
        }
        return static_cast<IWantParams *>(object->Query(g_IID_IWantParams));
    }

    virtual ErrCode GetValue(WantParams & value) = 0;
};
class WantParamWrapper final : public Object, public IWantParams {
public:
    WantParamWrapper(const WantParams &value) : wantParams_(value)
    {}
    /**
     * @brief Constructor for move semantics
     */
    WantParamWrapper(WantParams &&value) : wantParams_(std::move(value))
    {}

    ~WantParamWrapper()
    {}

    IINTERFACE_DECL();

    ErrCode GetValue(WantParams &value) override;

    bool Equals(IObject &other) override;

    std::string ToString() override;

    static sptr<IWantParams> Box(const WantParams &value);
    /**
     * @brief Wrap a wantParam into a WantParamWrapper using move semantics and return the wrapper.
     *
     * @param value
     * @return sptr<IWantParams>
     */
    static sptr<IWantParams> Box(WantParams &&value);

    static WantParams Unbox(IWantParams *object);

    static bool ValidateStr(const std::string &str);

    static sptr<IWantParams> Parse(const std::string &str);

    static WantParams ParseWantParams(const std::string &str);

    static WantParams ParseWantParamsWithBrackets(const std::string &str);

    static constexpr char SIGNATURE = 'W';

private:
    friend class Array;

    struct ParseState {
        std::string key;
        int typeId = 0;
        size_t typeIndexBefore = 0;
        WantParams wantParams;
    };

    std::string ToString(int depth);

    static sptr<IWantParams> Parse(const std::string &str, int depth);

    static WantParams ParseWantParams(const std::string &str, int depth);

    static WantParams ParseWantParamsWithBrackets(const std::string &str, int depth);

    static bool AppendParamValueString(IInterface *value, int typeId, int depth, std::string &result);

    static void FindNestedWantParamsEnd(const std::string &str, size_t strnum, size_t &num);

    static bool ParseNestedWantParams(const std::string &str, size_t &strnum, int depth, ParseState &state);

    static sptr<IInterface> ParseValueByType(int typeId, const std::string &value, int depth);

    static bool ParseQuotedParam(const std::string &str, size_t &strnum, int depth, ParseState &state,
        const char *func);

    static bool ParseQuotedParamWithBrackets(const std::string &str, size_t &strnum, int depth, ParseState &state,
        const char *func);

    static void ResetIfParseIncomplete(ParseState &state);

    WantParams wantParams_;
};
}  // namespace AAFwk
}  // namespace OHOS
#endif  // OHOS_ABILITY_BASE_WANT_PARAMS_WRAPPER_H
