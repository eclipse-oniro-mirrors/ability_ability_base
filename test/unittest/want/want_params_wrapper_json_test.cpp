/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "array_wrapper.h"
#include "bool_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "want.h"
#include "want_params.h"
#include "want_params_wrapper.h"
#include "want_params_wrapper_json.h"
#include "zchar_wrapper.h"

using namespace testing::ext;
using namespace OHOS;
using namespace AAFwk;

namespace {
// 信封标记，与实现端常量保持一致（逐字符核对，避免拼写差异导致用例空跑）。
constexpr const char *ENVELOPE_KEY = "ohos.want.paramsStringEnvelope";

// 在断言前打印序列化/解析后的字符串，便于失败时直接看到实际输出。
void Dump(const std::string &tag, const std::string &s)
{
    GTEST_LOG_(INFO) << "[WantParamsWrapperJsonTest] " << tag << " => [" << s << "]";
}

bool Validate(const std::string &text)
{
    WantParams ignored;
    return WantParamWrapperJson::Parse(text, ignored);
}

// 取 out 中 key 对应的 string 值（typeId=9）。取不到返回 "<null>" 便于断言区分。
std::string GetString9(const WantParams &out, const std::string &key)
{
    sptr<IInterface> item = out.GetParam(key);
    if (item == nullptr) {
        return "<null>";
    }
    return WantParams::GetStringByType(item, 9);  // 9 = VALUE_TYPE_STRING
}

// 构造嵌套层数为 nestedLevels 的带信封字符串，用于深度边界测试。
// 结构：{"ohos.want.paramsStringEnvelope":{"k":{"101":{...{"leaf":{"9":"v"}}...}}}}
// depth=0 为信封 value，每多一层 typeId=101 嵌套 depth 加 1，最深一层 depth=nestedLevels。
std::string BuildNestedEnvelope(int nestedLevels)
{
    std::string s = "{\"";
    s += ENVELOPE_KEY;
    s += "\":";
    for (int i = 0; i < nestedLevels; ++i) {
        s += "{\"k\":{\"101\":";  // 每层一个成员 k，typeId=101 指向下一层 wantParams
    }
    s += "{\"leaf\":{\"9\":\"v\"}}";  // 最内层 wantParams 含一个 string 成员
    for (int i = 0; i < nestedLevels; ++i) {
        s += "}}";  // 闭合每层 {"k":{...}}
    }
    s += "}";  // 闭合信封
    return s;
}

std::string BuildArrayEnvelope(int arrayDepth)
{
    nlohmann::json arrayNode = {{"101", nlohmann::json::array()}};
    for (int i = 1; i < arrayDepth; ++i) {
        arrayNode = {{"102", nlohmann::json::array({std::move(arrayNode)})}};
    }
    nlohmann::json root = {{ENVELOPE_KEY, {{"array", {{"102", std::move(arrayNode)}}}}}};
    return root.dump();
}

sptr<IArray> BuildNestedArrayObject(int arrayDepth)
{
    sptr<IArray> current = sptr<Array>::MakeSptr(0, g_IID_IWantParams);
    for (int i = 1; i < arrayDepth; ++i) {
        sptr<IArray> outer = sptr<Array>::MakeSptr(1, g_IID_IArray);
        outer->Set(0, current);
        current = outer;
    }
    return current;
}

sptr<IArray> BuildNestedArrayWithWantParamsLeaf(int arrayDepth)
{
    if (arrayDepth <= 0) {
        return nullptr;
    }

    WantParams leaf;
    leaf.SetParam("leaf", String::Box("value"));
    sptr<IWantParams> boxedLeaf = WantParamWrapper::Box(std::move(leaf));
    sptr<IArray> current = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    if (boxedLeaf == nullptr || current == nullptr || current->Set(0, boxedLeaf) != ERR_OK) {
        return nullptr;
    }

    for (int currentDepth = 1; currentDepth < arrayDepth; ++currentDepth) {
        sptr<IArray> outer = sptr<Array>::MakeSptr(1, g_IID_IArray);
        if (outer == nullptr || outer->Set(0, current) != ERR_OK) {
            return nullptr;
        }
        current = outer;
    }
    return current;
}

constexpr int FULL_MEMBER_COUNT = 1024;
constexpr int FULL_DEPTH = 100;
constexpr const char *NESTED_KEY = "lv";
constexpr const char *MAX_STRING_PREFIX = "max_string_";

std::string MakePerfKey(int level, int index)
{
    return "p_" + std::to_string(level) + "_" + std::to_string(index);
}

std::string MakePerfValue(int level, int index)
{
    return "v_" + std::to_string(level) + "_" + std::to_string(index);
}

std::string MakeMaxPerfValue(int level)
{
    std::string value = MAX_STRING_PREFIX + std::to_string(level) + "_";
    value.resize(static_cast<size_t>(Want::PARAM_WANT_CAPACITY_EXPANSION), 'x');
    return value;
}

void FillScalarMembers(WantParams &wp, int level, int count)
{
    for (int i = 0; i < count; ++i) {
        std::string value = (i == count - 1) ? MakeMaxPerfValue(level) : MakePerfValue(level, i);
        wp.SetParam(MakePerfKey(level, i), String::Box(value));
    }
}

WantParams BuildFullNestedWantParams(int depth, int memberCount)
{
    WantParams inner;
    FillScalarMembers(inner, depth, memberCount);
    for (int level = depth - 1; level >= 0; --level) {
        WantParams outer;
        FillScalarMembers(outer, level, memberCount - 1);
        outer.SetParam(NESTED_KEY, WantParamWrapper::Box(std::move(inner)));
        inner = std::move(outer);
    }
    return inner;
}

long long CostMs(const std::chrono::steady_clock::time_point &start,
    const std::chrono::steady_clock::time_point &end)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
}  // namespace

class WantParamWrapperJsonTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() override;
    void TearDown() override;
};

void WantParamWrapperJsonTest::SetUpTestCase(void)
{}

void WantParamWrapperJsonTest::TearDownTestCase(void)
{}

void WantParamWrapperJsonTest::SetUp(void)
{}

void WantParamWrapperJsonTest::TearDown(void)
{}

// ==================== A. 已知问题数据类型：转义往返 ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_0100
 * @tc.name: Serialize/Parse basic string round-trip
 * @tc.desc: 普通字符串 Serialize 后 Parse 应原样还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0100, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "hello";
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0100_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0200
 * @tc.name: value contains double quote
 * @tc.desc: value 含双引号应被转义为 \"，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0200, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "a\"b";  // 实际内容 a"b
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0200_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0300
 * @tc.name: value contains backslash
 * @tc.desc: value 含反斜杠应被转义为 \\，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0300, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "a\\b";  // 实际内容 a\b
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0300_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0400
 * @tc.name: value contains newline and tab
 * @tc.desc: value 含换行/制表应被转义为 \n \t，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0400, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "line1\nline2\tend";  // 实际含换行与制表
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0400_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0500
 * @tc.name: value contains JSON structural chars
 * @tc.desc: value 含 { } [ ] , : 不应被误切为结构符，往返后原样还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0500, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "{a:[1,2],b:3}";  // 含花括号/方括号/逗号/冒号
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0500_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0600
 * @tc.name: value contains Chinese (UTF-8)
 * @tc.desc: 非 ASCII 中文按规范 UTF-8 原样保留，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0600, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "你好，世界";  // 3 字节 UTF-8 序列
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0600_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0700
 * @tc.name: value contains 4-byte UTF-8 (emoji)
 * @tc.desc: 4 字节 UTF-8（补充平面，如 emoji）原样保留，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0700, TestSize.Level1)
{
    const std::string key = "k";
    const std::string val = "emoji😀end";  // 😀 = U+1F600，UTF-8 四字节
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0700_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0800
 * @tc.name: key contains special chars
 * @tc.desc: key 含特殊字符同样需要转义，往返后还原。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0800, TestSize.Level1)
{
    const std::string key = "k\"e\\y";  // 实际内容 k"e\y
    const std::string val = "v";
    WantParams wp;
    wp.SetParam(key, String::Box(val));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0800_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, key), val);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_0900
 * @tc.name: int type round-trip
 * @tc.desc: 整型参数 Serialize/Parse 往返，值与 typeId 均保持。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_0900, TestSize.Level1)
{
    const std::string key = "n";
    WantParams wp;
    wp.SetParam(key, Integer::Box(42));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("0900_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam(key);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 5), "42");  // 5 = VALUE_TYPE_INT
}

// ==================== B. 嵌套构造深度 ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_1000
 * @tc.name: nested wantParams round-trip
 * @tc.desc: 单层嵌套 wantParams 往返，结构与值保持。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1000, TestSize.Level1)
{
    WantParams inner;
    inner.SetParam("leaf", String::Box("v"));
    WantParams wp;
    wp.SetParam("k", WantParamWrapper::Box(inner));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("1000_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> kItem = out.GetParam("k");
    ASSERT_NE(kItem, nullptr);
    WantParams innerOut = WantParamWrapper::Unbox(IWantParams::Query(kItem));
    EXPECT_EQ(GetString9(innerOut, "leaf"), "v");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1100
 * @tc.name: multi-level nested round-trip
 * @tc.desc: 多层嵌套 wantParams 往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1100, TestSize.Level1)
{
    WantParams l3;
    l3.SetParam("leaf", String::Box("deep"));
    WantParams l2;
    l2.SetParam("c", WantParamWrapper::Box(l3));
    WantParams l1;
    l1.SetParam("b", WantParamWrapper::Box(l2));
    WantParams wp;
    wp.SetParam("a", WantParamWrapper::Box(l1));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("1100_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    auto aItem = out.GetParam("a");
    ASSERT_NE(aItem, nullptr);
    auto bOut = WantParamWrapper::Unbox(IWantParams::Query(aItem));    // = l1 = {b:...}
    auto bItem = bOut.GetParam("b");
    ASSERT_NE(bItem, nullptr);
    auto cOut = WantParamWrapper::Unbox(IWantParams::Query(bItem));    // = l2 = {c:...}
    auto cItem = cOut.GetParam("c");
    ASSERT_NE(cItem, nullptr);
    auto leafOut = WantParamWrapper::Unbox(IWantParams::Query(cItem)); // = l3 = {leaf:"deep"}
    EXPECT_EQ(GetString9(leafOut, "leaf"), "deep");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1200
 * @tc.name: depth boundary 100 should succeed
 * @tc.desc: 嵌套深度达 MAX_DEPTH(100) 仍应解析成功。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1200, TestSize.Level1)
{
    std::string s = BuildNestedEnvelope(100);  // 最深层 depth=100，未超限
    Dump("1200_input_len", std::to_string(s.size()));
    WantParams out;
    EXPECT_TRUE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1300
 * @tc.name: depth over limit 101 should fail
 * @tc.desc: 嵌套深度超过 MAX_DEPTH(101) 应解析失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1300, TestSize.Level1)
{
    std::string s = BuildNestedEnvelope(101);  // 最深层 depth=101，超限
    Dump("1300_input_len", std::to_string(s.size()));
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

// ==================== C. 畸形数据（应解析失败） ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_1400
 * @tc.name: non-envelope input rejected
 * @tc.desc: 无信封输入（老格式）HasEnvelope=false、Parse=false。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1400, TestSize.Level1)
{
    const std::string s = "{\"k\":{\"9\":\"v\"}}";  // 老格式，无信封
    Dump("1400_input", s);
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope(s));
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1500
 * @tc.name: envelope value not object
 * @tc.desc: 信封 value 不是 object 应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1500, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":123}";  // value 是裸整数
    Dump("1500_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1600
 * @tc.name: unterminated string
 * @tc.desc: 字符串未闭合应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1600, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"unterminated}}}";
    Dump("1600_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1700
 * @tc.name: invalid escape
 * @tc.desc: 非法转义 \x 应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1700, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"a\\xb\"}}}";
    Dump("1700_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1800
 * @tc.name: isolated low surrogate
 * @tc.desc: 孤立低 surrogate \uDC00 应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1800, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"\\uDC00\"}}}";
    Dump("1800_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_1900
 * @tc.name: invalid typeId variants
 * @tc.desc: 非法 typeId（非数字/未知/0/尾随字符）应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_1900, TestSize.Level1)
{
    const std::vector<std::string> bad = {
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"abc\":\"v\"}}}",   // 非数字
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"999\":\"v\"}}}",   // 未知 typeId
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"0\":\"v\"}}}",     // 0（哨兵/未知）
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"5abc\":\"v\"}}}",  // 尾随字符
    };
    for (const auto &s : bad) {
        Dump("1900_input", s);
        WantParams out;
        EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2000
 * @tc.name: typedValue duplicate key follows json library semantics
 * @tc.desc: duplicate typeId keys are non-protocol input; nlohmann keeps one value, while distinct members fail.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2000, TestSize.Level1)
{
    const std::string duplicate = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"a\",\"9\":\"b\"}}}";
    Dump("2000_duplicate_input", duplicate);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(duplicate, out));
    std::string parsedValue = GetString9(out, "k");
    EXPECT_TRUE(parsedValue == "a" || parsedValue == "b");

    const std::string multiple = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"5\":\"1\",\"9\":\"a\"}}}";
    Dump("2000_multiple_input", multiple);
    WantParams invalidOut;
    EXPECT_FALSE(WantParamWrapperJson::Parse(multiple, invalidOut));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2100
 * @tc.name: trailing garbage after envelope
 * @tc.desc: 信封结束后尾随非空白应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2100, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{}}X";  // 尾随 X
    Dump("2100_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2200
 * @tc.name: nested empty wantParams accepted
 * @tc.desc: 嵌套 wantParams 为空 {} 应可解析（与 Serialize 端空 child 输出 {} 对齐，保持往返自洽）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2200, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"101\":{}}}}";
    Dump("2200_input", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> kItem = out.GetParam("k");
    ASSERT_NE(kItem, nullptr);
    WantParams innerOut = WantParamWrapper::Unbox(IWantParams::Query(kItem));
    EXPECT_EQ(innerOut.Size(), 0);  // 嵌套空 wantParams 还原为空对象
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2250
 * @tc.name: nested empty wantParams full round-trip
 * @tc.desc: 内存构造含空嵌套 child 的 WantParams，Serialize→Parse→再 Serialize，
 *   两次序列化字符串应一致（回归 Serialize/Parse 对空嵌套的往返不对称）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2250, TestSize.Level1)
{
    WantParams inner;  // 空 wantParams
    WantParams wp;
    wp.SetParam("k", WantParamWrapper::Box(inner));
    std::string s1;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s1));
    Dump("2250_serialize_1", s1);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s1, out));
    std::string s2;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(out, s2));
    Dump("2250_serialize_2", s2);
    EXPECT_EQ(s1, s2);  // 往返后序列化输出必须一致
    EXPECT_EQ(s1, "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"101\":{}}}}");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2300
 * @tc.name: top-level empty wantParams accepted
 * @tc.desc: 信封内顶层空 wantParams 应成功（{"ohos.want.paramsStringEnvelope":{}}）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2300, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{}}";
    Dump("2300_input", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(out.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2400
 * @tc.name: typeId known but unconstructable
 * @tc.desc: typeId 已知但 GetInterfaceByType 无法构造（如 10）应失败。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2400, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"10\":\"v\"}}}";
    Dump("2400_input", s);
    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
}

// ==================== D. HasEnvelope / Validate ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_2500
 * @tc.name: Validate legitimate and malformed
 * @tc.desc: Validate 对合法信封返回 true，对畸形返回 false。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2500, TestSize.Level1)
{
    const std::string ok = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"v\"}}}";
    Dump("2500_ok", ok);
    EXPECT_TRUE(Validate(ok));
    const std::string bad = "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"999\":\"v\"}}}";
    Dump("2500_bad", bad);
    EXPECT_FALSE(Validate(bad));
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2600
 * @tc.name: HasEnvelope dispatch
 * @tc.desc: HasEnvelope 对带信封输入 true，对无信封输入 false。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2600, TestSize.Level1)
{
    const std::string withEnv = "{\"ohos.want.paramsStringEnvelope\":{}}";
    Dump("2600_with_env", withEnv);
    EXPECT_TRUE(WantParamWrapperJson::HasEnvelope(withEnv));
    const std::string withoutEnv = "{\"k\":{\"9\":\"v\"}}";
    Dump("2600_without_env", withoutEnv);
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope(withoutEnv));
    const std::string notObject = "\"just a string\"";
    Dump("2600_not_object", notObject);
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope(notObject));
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope(" {\"ohos.want.paramsStringEnvelope\":{}}"));
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope("{ \"ohos.want.paramsStringEnvelope\":{}}"));
    EXPECT_FALSE(WantParamWrapperJson::HasEnvelope("{\"ohos.want.paramsStringEnvelope\" :{}}"));

    WantParams out;
    EXPECT_FALSE(WantParamWrapperJson::Parse(" {\"ohos.want.paramsStringEnvelope\":{}}", out));
    EXPECT_FALSE(WantParamWrapperJson::Parse("{ \"ohos.want.paramsStringEnvelope\":{}}", out));
    EXPECT_FALSE(WantParamWrapperJson::Parse("{\"ohos.want.paramsStringEnvelope\" :{}}", out));
}

// ==================== E. 全数据类型往返（真实构造） ====================
// typeId 对照（WantParams 私有枚举值）：1=Bool 2=Byte 3=Char 4=Short 5=Int 6=Long 7=Float 8=Double
// 9=String 102=Array 101=WantParams。用字面量避免访问 private 枚举。

/**
 * @tc.number: Want_Param_Wrapper_Json_2700
 * @tc.name: boolean round-trip
 * @tc.desc: 布尔参数 Serialize/Parse 往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2700, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("b", Boolean::Box(true));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("2700_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("b");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 1), "true");  // 1 = VALUE_TYPE_BOOLEAN
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2800
 * @tc.name: byte round-trip
 * @tc.desc: 字节参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2800, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("by", Byte::Box('A'));  // 'A' = 65
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("2800_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("by");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 2), "65");  // 2 = VALUE_TYPE_BYTE
}

/**
 * @tc.number: Want_Param_Wrapper_Json_2900
 * @tc.name: char round-trip
 * @tc.desc: 字符参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_2900, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("c", Char::Box(static_cast<zchar>('Z')));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("2900_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("c");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 3), "Z");  // 3 = VALUE_TYPE_CHAR
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3000
 * @tc.name: short round-trip
 * @tc.desc: 短整型参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3000, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("sh", Short::Box(static_cast<short>(-7)));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3000_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("sh");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 4), "-7");  // 4 = VALUE_TYPE_SHORT
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3100
 * @tc.name: long round-trip
 * @tc.desc: 长整型参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3100, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("l", Long::Box64(9000000000LL));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3100_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("l");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 6), "9000000000");  // 6 = VALUE_TYPE_LONG
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3200
 * @tc.name: float round-trip
 * @tc.desc: 单精度浮点参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3200, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("f", Float::Box(3.5f));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3200_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("f");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 7), "3.500000");  // 7 = VALUE_TYPE_FLOAT（std::to_string 固定 6 位小数）
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3300
 * @tc.name: double round-trip
 * @tc.desc: 双精度浮点参数往返。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3300, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("d", Double::Box(2.71828));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3300_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    sptr<IInterface> item = out.GetParam("d");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(WantParams::GetStringByType(item, 8), "2.718280");  // 8 = VALUE_TYPE_DOUBLE（std::to_string 固定 6 位小数）
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3400
 * @tc.name: string array is not supported in phase one
 * @tc.desc: String arrays fail without falling back to the legacy Array string format.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3400, TestSize.Level1)
{
    sptr<IArray> arr = new Array(2, g_IID_IString);
    ASSERT_NE(arr, nullptr);
    ASSERT_EQ(arr->Set(0, String::Box("a")), ERR_OK);
    ASSERT_EQ(arr->Set(1, String::Box("b")), ERR_OK);
    WantParams wp;
    wp.SetParam("arr", arr);
    std::string serialized = "unchanged";
    EXPECT_FALSE(WantParamWrapperJson::Serialize(wp, serialized));
    EXPECT_EQ(serialized, "unchanged");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3500
 * @tc.name: integer array is not supported in phase one
 * @tc.desc: Integer arrays fail until their JSON element rules are defined.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3500, TestSize.Level1)
{
    sptr<IArray> arr = new Array(2, g_IID_IInteger);
    ASSERT_NE(arr, nullptr);
    ASSERT_EQ(arr->Set(0, Integer::Box(10)), ERR_OK);
    ASSERT_EQ(arr->Set(1, Integer::Box(20)), ERR_OK);
    WantParams wp;
    wp.SetParam("arr", arr);
    std::string serialized = "unchanged";
    EXPECT_FALSE(WantParamWrapperJson::Serialize(wp, serialized));
    EXPECT_EQ(serialized, "unchanged");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3600
 * @tc.name: empty primitive array is not supported in phase one
 * @tc.desc: Empty primitive arrays retain an unsupported element type and fail atomically.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3600, TestSize.Level1)
{
    sptr<IArray> arr = new Array(0, g_IID_IInteger);
    ASSERT_NE(arr, nullptr);
    WantParams wp;
    wp.SetParam("arr", arr);
    std::string serialized = "unchanged";
    EXPECT_FALSE(WantParamWrapperJson::Serialize(wp, serialized));
    EXPECT_EQ(serialized, "unchanged");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3610
 * @tc.name: WantParams array uses recursive JSON objects
 * @tc.desc: WantParams array items use type-key JSON and preserve delimiter-like string content.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3610, TestSize.Level1)
{
    const std::string specialValue = "XXXX\"XX\\tail,a{b}\n\t";
    WantParams first;
    first.SetParam("special\"key", String::Box(specialValue));
    WantParams second;

    sptr<IArray> array = sptr<Array>::MakeSptr(2, g_IID_IWantParams);
    ASSERT_NE(array, nullptr);
    ASSERT_EQ(array->Set(0, WantParamWrapper::Box(std::move(first))), ERR_OK);
    ASSERT_EQ(array->Set(1, WantParamWrapper::Box(std::move(second))), ERR_OK);

    WantParams params;
    params.SetParam("array", array);
    std::string serialized;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(params, serialized));
    Dump("3610_serialize", serialized);

    const nlohmann::json root = nlohmann::json::parse(serialized);
    const nlohmann::json &arrayJson = root.at(ENVELOPE_KEY).at("array").at("102");
    ASSERT_TRUE(arrayJson.is_object());
    ASSERT_TRUE(arrayJson.at("101").is_array());
    EXPECT_EQ(arrayJson.at("101").size(), 2u);
    EXPECT_EQ(arrayJson.at("101").at(0).at("special\"key").at("9").get<std::string>(), specialValue);

    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, out));
    IArray *parsedArray = IArray::Query(out.GetParam("array"));
    ASSERT_NE(parsedArray, nullptr);
    long length = 0;
    ASSERT_EQ(parsedArray->GetLength(length), ERR_OK);
    ASSERT_EQ(length, 2);

    sptr<IInterface> firstElement;
    ASSERT_EQ(parsedArray->Get(0, firstElement), ERR_OK);
    WantParams parsedFirst = WantParamWrapper::Unbox(IWantParams::Query(firstElement));
    EXPECT_EQ(GetString9(parsedFirst, "special\"key"), specialValue);

    sptr<IInterface> secondElement;
    ASSERT_EQ(parsedArray->Get(1, secondElement), ERR_OK);
    WantParams parsedSecond = WantParamWrapper::Unbox(IWantParams::Query(secondElement));
    EXPECT_EQ(parsedSecond.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3620
 * @tc.name: Empty WantParams array round-trip
 * @tc.desc: An empty typed WantParams array remains type 101 with zero JSON items.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3620, TestSize.Level1)
{
    sptr<IArray> array = sptr<Array>::MakeSptr(0, g_IID_IWantParams);
    ASSERT_NE(array, nullptr);
    WantParams params;
    params.SetParam("array", array);

    std::string serialized;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(params, serialized));
    const nlohmann::json root = nlohmann::json::parse(serialized);
    const nlohmann::json &arrayJson = root.at(ENVELOPE_KEY).at("array").at("102");
    EXPECT_TRUE(arrayJson.at("101").empty());

    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, out));
    IArray *parsedArray = IArray::Query(out.GetParam("array"));
    ASSERT_NE(parsedArray, nullptr);
    InterfaceID elementType;
    ASSERT_EQ(parsedArray->GetType(elementType), ERR_OK);
    EXPECT_EQ(elementType, g_IID_IWantParams);
    long length = -1;
    ASSERT_EQ(parsedArray->GetLength(length), ERR_OK);
    EXPECT_EQ(length, 0);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3630
 * @tc.name: Invalid native WantParams array fails atomically
 * @tc.desc: Null or wrong-type native elements fail serialization without changing output.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3630, TestSize.Level1)
{
    sptr<IArray> nullArray = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    sptr<IArray> wrongTypeArray = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    sptr<IArray> wrongNestedArray = sptr<Array>::MakeSptr(1, g_IID_IArray);
    ASSERT_NE(nullArray, nullptr);
    ASSERT_NE(wrongTypeArray, nullptr);
    ASSERT_NE(wrongNestedArray, nullptr);
    ASSERT_EQ(wrongTypeArray->Set(0, String::Box("wrong")), ERR_OK);
    ASSERT_EQ(wrongNestedArray->Set(0, String::Box("wrong")), ERR_OK);

    for (const auto &array : { nullArray, wrongTypeArray, wrongNestedArray }) {
        WantParams params;
        params.SetParam("array", array);
        std::string serialized = "unchanged";
        EXPECT_FALSE(WantParamWrapperJson::Serialize(params, serialized));
        EXPECT_EQ(serialized, "unchanged");
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3640
 * @tc.name: Invalid WantParams array JSON schema fails atomically
 * @tc.desc: Invalid members, element type, items shape, item type, or extra fields are rejected.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3640, TestSize.Level1)
{
    const std::string prefix = "{\"" + std::string(ENVELOPE_KEY) + "\":{\"array\":{\"102\":";
    const std::string suffix = "}}}";
    const std::vector<std::string> invalidValues = {
        "{}",
        "[]",
        "{\"101\":{}}",
        "{\"101\":[null]}",
        "{\"102\":[null]}",
        "{\"102\":[{\"9\":[]}]}",
        "{\"101\":[],\"102\":[]}",
        "{\"9\":[]}",
        "{\"elementType\":101,\"items\":[]}",
        "{\"items\":[]}",
    };

    for (const auto &value : invalidValues) {
        WantParams out;
        out.SetParam("sentinel", String::Box("keep"));
        const std::string serialized = prefix + value + suffix;
        EXPECT_FALSE(WantParamWrapperJson::Parse(serialized, out)) << "input=[" << serialized << "]";
        EXPECT_EQ(GetString9(out, "sentinel"), "keep");
        EXPECT_EQ(out.Size(), 1);
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3650
 * @tc.name: Legacy Array signature strings are rejected
 * @tc.desc: The new JSON interface does not accept any legacy Array signature string.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3650, TestSize.Level1)
{
    const std::vector<std::string> legacyValues = {
        "W1{{\\\"k\\\":{\\\"9\\\":\\\"v\\\"}}}",
        "I2{1,2}",
        "T2{a,b}",
    };
    for (const auto &legacyValue : legacyValues) {
        const std::string legacy = "{\"" + std::string(ENVELOPE_KEY) +
            "\":{\"array\":{\"102\":\"" + legacyValue + "\"}}}";
        WantParams out;
        out.SetParam("sentinel", String::Box("keep"));
        EXPECT_FALSE(WantParamWrapperJson::Parse(legacy, out));
        EXPECT_EQ(GetString9(out, "sentinel"), "keep");
        EXPECT_EQ(out.Size(), 1);
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3660
 * @tc.name: Nested WantParams arrays use the new JSON representation recursively
 * @tc.desc: A WantParams array inside an array item is serialized and parsed without the legacy W format.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3660, TestSize.Level1)
{
    WantParams leaf;
    leaf.SetParam("value", String::Box("XXXX\"XX,a{b}"));
    sptr<IArray> innerArray = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    ASSERT_NE(innerArray, nullptr);
    ASSERT_EQ(innerArray->Set(0, WantParamWrapper::Box(std::move(leaf))), ERR_OK);

    WantParams parent;
    parent.SetParam("children", innerArray);
    sptr<IArray> outerArray = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    ASSERT_NE(outerArray, nullptr);
    ASSERT_EQ(outerArray->Set(0, WantParamWrapper::Box(std::move(parent))), ERR_OK);

    WantParams params;
    params.SetParam("parents", outerArray);
    std::string serialized;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(params, serialized));

    const nlohmann::json root = nlohmann::json::parse(serialized);
    const nlohmann::json &leafJson =
        root.at(ENVELOPE_KEY).at("parents").at("102").at("101").at(0)
            .at("children").at("102").at("101").at(0);
    EXPECT_EQ(leafJson.at("value").at("9").get<std::string>(), "XXXX\"XX,a{b}");

    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, out));
    IArray *parsedOuter = IArray::Query(out.GetParam("parents"));
    ASSERT_NE(parsedOuter, nullptr);
    sptr<IInterface> parentElement;
    ASSERT_EQ(parsedOuter->Get(0, parentElement), ERR_OK);
    WantParams parsedParent = WantParamWrapper::Unbox(IWantParams::Query(parentElement));
    IArray *parsedInner = IArray::Query(parsedParent.GetParam("children"));
    ASSERT_NE(parsedInner, nullptr);
    sptr<IInterface> leafElement;
    ASSERT_EQ(parsedInner->Get(0, leafElement), ERR_OK);
    WantParams parsedLeaf = WantParamWrapper::Unbox(IWantParams::Query(leafElement));
    EXPECT_EQ(GetString9(parsedLeaf, "value"), "XXXX\"XX,a{b}");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3665
 * @tc.name: Array of arrays reaches WantParams array through the new codec
 * @tc.desc: An IArray<IArray> recursively uses type-key ArrayNode objects and preserves special strings.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3665, TestSize.Level1)
{
    const std::string specialValue = "outer\\inner\"value,a{b}";
    WantParams leaf;
    leaf.SetParam("value", String::Box(specialValue));

    sptr<IArray> wantParamsArray = sptr<Array>::MakeSptr(1, g_IID_IWantParams);
    ASSERT_NE(wantParamsArray, nullptr);
    ASSERT_EQ(wantParamsArray->Set(0, WantParamWrapper::Box(std::move(leaf))), ERR_OK);

    sptr<IArray> outerArray = sptr<Array>::MakeSptr(1, g_IID_IArray);
    ASSERT_NE(outerArray, nullptr);
    ASSERT_EQ(outerArray->Set(0, wantParamsArray), ERR_OK);

    WantParams params;
    params.SetParam("groups", outerArray);
    std::string serialized;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(params, serialized));

    const nlohmann::json root = nlohmann::json::parse(serialized);
    const nlohmann::json &leafJson =
        root.at(ENVELOPE_KEY).at("groups").at("102").at("102").at(0).at("101").at(0);
    EXPECT_EQ(leafJson.at("value").at("9").get<std::string>(), specialValue);

    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, out));
    IArray *parsedOuter = IArray::Query(out.GetParam("groups"));
    ASSERT_NE(parsedOuter, nullptr);
    sptr<IInterface> parsedInnerValue;
    ASSERT_EQ(parsedOuter->Get(0, parsedInnerValue), ERR_OK);
    IArray *parsedInner = IArray::Query(parsedInnerValue);
    ASSERT_NE(parsedInner, nullptr);
    sptr<IInterface> parsedLeafValue;
    ASSERT_EQ(parsedInner->Get(0, parsedLeafValue), ERR_OK);
    WantParams parsedLeaf = WantParamWrapper::Unbox(IWantParams::Query(parsedLeafValue));
    EXPECT_EQ(GetString9(parsedLeaf, "value"), specialValue);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3670
 * @tc.name: WantParams array depth boundary is shared by serialize and parse
 * @tc.desc: Array nesting followed by one WantParams item reaches depth 100 and round-trips;
 *   one more Array level reaches depth 101 and fails atomically.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3670, TestSize.Level1)
{
    constexpr int BOUNDARY_ARRAY_DEPTH = FULL_DEPTH - 1;
    sptr<IArray> boundaryArray = BuildNestedArrayWithWantParamsLeaf(BOUNDARY_ARRAY_DEPTH);
    ASSERT_NE(boundaryArray, nullptr);
    WantParams boundary;
    boundary.SetParam("array", boundaryArray);

    std::string serialized;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(boundary, serialized));
    WantParams parsed;
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, parsed));

    const nlohmann::json boundaryRoot = nlohmann::json::parse(serialized);
    const nlohmann::json &boundaryArrayJson = boundaryRoot.at(ENVELOPE_KEY).at("array").at("102");
    nlohmann::json tooDeepArray = {
        {"102", nlohmann::json::array({boundaryArrayJson})},
    };
    nlohmann::json tooDeepRoot = {
        {ENVELOPE_KEY, {{"array", {{"102", std::move(tooDeepArray)}}}}},
    };
    WantParams parseOut;
    parseOut.SetParam("sentinel", String::Box("keep"));
    EXPECT_FALSE(WantParamWrapperJson::Parse(tooDeepRoot.dump(), parseOut));
    EXPECT_EQ(GetString9(parseOut, "sentinel"), "keep");
    EXPECT_EQ(parseOut.Size(), 1);

    sptr<IArray> overLimitArray = BuildNestedArrayWithWantParamsLeaf(FULL_DEPTH);
    ASSERT_NE(overLimitArray, nullptr);
    WantParams overLimit;
    overLimit.SetParam("array", overLimitArray);
    std::string unchanged = "unchanged";
    EXPECT_FALSE(WantParamWrapperJson::Serialize(overLimit, unchanged));
    EXPECT_EQ(unchanged, "unchanged");
}

// ==================== F. WantParam 真实构造的深度嵌套 ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_3700
 * @tc.name: real nested wantParams depth 50
 * @tc.desc: 用 WantParamWrapper::Box 真实构造 50 层嵌套，Serialize 后 Parse 往返，
 *   验证最内层值可还原（与 1200/1300 手拼字符串的深度边界互补）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3700, TestSize.Level1)
{
    const int depth = 50;
    WantParams inner;
    inner.SetParam("leaf", String::Box("deepval"));
    for (int i = 0; i < depth; ++i) {
        WantParams outer;
        outer.SetParam("lv", WantParamWrapper::Box(std::move(inner)));
        inner = std::move(outer);
    }
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(inner, s));
    Dump("3700_serialize_len", std::to_string(s.size()));
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    // 逐层下钻到最内层 leaf。
    WantParams cur = std::move(out);
    for (int i = 0; i < depth; ++i) {
        sptr<IInterface> lv = cur.GetParam("lv");
        ASSERT_NE(lv, nullptr);
        cur = WantParamWrapper::Unbox(IWantParams::Query(lv));
    }
    EXPECT_EQ(GetString9(cur, "leaf"), "deepval");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3750
 * @tc.name: real nested depth at MAX_DEPTH boundary (100) round-trip
 * @tc.desc: 用 WantParamWrapper::Box 真实构造 100 层嵌套（= MAX_DEPTH 边界），Serialize→Parse 完整往返，
 *   补 1200/1300 手拼字符串不经 Serialize 的盲区，验证序列化方向 depth 边界不丢层、不栈溢出
 *   100 层未超限，应全程成功。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3750, TestSize.Level1)
{
    const int depth = 100;  // MAX_DEPTH 边界：100 层嵌套应全程成功（与 1200 边界一致）
    WantParams inner;
    inner.SetParam("leaf", String::Box("boundary"));
    for (int i = 0; i < depth; ++i) {
        WantParams outer;
        outer.SetParam("lv", WantParamWrapper::Box(std::move(inner)));
        inner = std::move(outer);
    }
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(inner, s));
    Dump("3750_serialize_len", std::to_string(s.size()));
    // 走完整 Serialize→Parse：覆盖 SerializeParams 递归路径（1200/1300 手拼字符串绕过了它）。
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    // 逐层下钻 100 次到最内层 leaf：任一层在序列化/解析中丢失，下钻会取不到 "lv" 而 ASSERT 失败。
    WantParams cur = std::move(out);
    for (int i = 0; i < depth; ++i) {
        sptr<IInterface> lv = cur.GetParam("lv");
        ASSERT_NE(lv, nullptr);
        cur = WantParamWrapper::Unbox(IWantParams::Query(lv));
    }
    EXPECT_EQ(GetString9(cur, "leaf"), "boundary");
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3755
 * @tc.name: full member count and max depth round-trip performance
 * @tc.desc: Build a depth-100 WantParams tree where every level has 1024 members
 *   and one max-size string payload.
 *   This mirrors WantParams parcel read limits (MAX_RECURSION_DEPTH=100 and
 *   maxAllowedSize=1024) plus Want IPC expansion capacity as a JSON codec
 *   pressure/performance regression test.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3755, TestSize.Level3)
{
    auto buildStart = std::chrono::steady_clock::now();
    WantParams wp = BuildFullNestedWantParams(FULL_DEPTH, FULL_MEMBER_COUNT);
    auto buildEnd = std::chrono::steady_clock::now();
    ASSERT_EQ(wp.Size(), FULL_MEMBER_COUNT);

    std::string serialized;
    auto serializeStart = std::chrono::steady_clock::now();
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, serialized));
    auto serializeEnd = std::chrono::steady_clock::now();
    ASSERT_FALSE(serialized.empty());

    WantParams out;
    auto parseStart = std::chrono::steady_clock::now();
    ASSERT_TRUE(WantParamWrapperJson::Parse(serialized, out));
    auto parseEnd = std::chrono::steady_clock::now();

    auto verifyStart = std::chrono::steady_clock::now();
    WantParams cur = std::move(out);
    for (int level = 0; level < FULL_DEPTH; ++level) {
        ASSERT_EQ(cur.Size(), FULL_MEMBER_COUNT) << "level=" << level;
        EXPECT_EQ(GetString9(cur, MakePerfKey(level, 0)), MakePerfValue(level, 0));
        EXPECT_EQ(GetString9(cur, MakePerfKey(level, FULL_MEMBER_COUNT - 2)),
            MakeMaxPerfValue(level));

        sptr<IInterface> next = cur.GetParam(NESTED_KEY);
        ASSERT_NE(next, nullptr) << "level=" << level;
        IWantParams *nested = IWantParams::Query(next);
        ASSERT_NE(nested, nullptr) << "level=" << level;
        cur = WantParamWrapper::Unbox(nested);
    }
    ASSERT_EQ(cur.Size(), FULL_MEMBER_COUNT);
    EXPECT_EQ(GetString9(cur, MakePerfKey(FULL_DEPTH, 0)), MakePerfValue(FULL_DEPTH, 0));
    EXPECT_EQ(GetString9(cur, MakePerfKey(FULL_DEPTH, FULL_MEMBER_COUNT - 1)),
        MakeMaxPerfValue(FULL_DEPTH));
    auto verifyEnd = std::chrono::steady_clock::now();

    GTEST_LOG_(INFO) << "[WantParamsWrapperJsonTest] 3755 depth=" << FULL_DEPTH
                     << ", membersPerLevel=" << FULL_MEMBER_COUNT
                     << ", maxStringBytes=" << Want::PARAM_WANT_CAPACITY_EXPANSION
                     << ", jsonBytes=" << serialized.size()
                     << ", buildMs=" << CostMs(buildStart, buildEnd)
                     << ", serializeMs=" << CostMs(serializeStart, serializeEnd)
                     << ", parseMs=" << CostMs(parseStart, parseEnd)
                     << ", verifyMs=" << CostMs(verifyStart, verifyEnd);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3760
 * @tc.name: real nested depth over MAX_DEPTH (101) serialize fails
 * @tc.desc: 用 WantParamWrapper::Box 真实构造 101 层嵌套（> MAX_DEPTH），Serialize 应返回 false，
 *   且 out 保持原值不变。与 3750 边界互补：解析端已有 depth 上限，序列化端同样应拒绝极深嵌套，
 *   避免生成自身无法解析的数据及栈溢出风险（对应 SerializeParams 的 depth 上限）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3760, TestSize.Level1)
{
    const int depth = 101;  // 超过 MAX_DEPTH(100)：序列化应失败
    WantParams inner;
    inner.SetParam("leaf", String::Box("overlimit"));
    for (int i = 0; i < depth; ++i) {
        WantParams outer;
        outer.SetParam("lv", WantParamWrapper::Box(std::move(inner)));
        inner = std::move(outer);
    }
    const std::string sentinel = "SENTINEL_UNCHANGED";
    std::string out = sentinel;  // 预置非空值，断言 Serialize 失败时 out 不被改写
    EXPECT_FALSE(WantParamWrapperJson::Serialize(inner, out));
    EXPECT_EQ(out, sentinel);  // 失败时 out 必须保持不变
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3800
 * @tc.name: mixed types in one wantParams
 * @tc.desc: 单个 wantParams 混合多种类型（string/int/double/嵌套 wantParams），往返后各值保持。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3800, TestSize.Level1)
{
    WantParams inner;
    inner.SetParam("in", String::Box("inner"));
    WantParams wp;
    wp.SetParam("s", String::Box("str"));
    wp.SetParam("i", Integer::Box(7));
    wp.SetParam("d", Double::Box(1.25));
    wp.SetParam("n", WantParamWrapper::Box(inner));
    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3800_serialize", s);
    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, "s"), "str");
    EXPECT_EQ(WantParams::GetStringByType(out.GetParam("i"), 5), "7");
    EXPECT_EQ(WantParams::GetStringByType(out.GetParam("d"), 8), "1.250000");
    auto nItem = out.GetParam("n");
    ASSERT_NE(nItem, nullptr);
    WantParams innerOut = WantParamWrapper::Unbox(IWantParams::Query(nItem));
    EXPECT_EQ(GetString9(innerOut, "in"), "inner");
}

// ==================== G. 畸形噪声/损坏输入批量拒绝 ====================

/**
 * @tc.number: Want_Param_Wrapper_Json_3810
 * @tc.name: serialize skips null values
 * @tc.desc: Null values are outside the envelope format and are silently dropped.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3810, TestSize.Level1)
{
    WantParams wp;
    wp.SetParam("keep", String::Box("v"));
    wp.SetParam("drop", nullptr);

    std::string s;
    ASSERT_TRUE(WantParamWrapperJson::Serialize(wp, s));
    Dump("3810_serialize", s);
    EXPECT_EQ(s.find("\"drop\""), std::string::npos);

    WantParams out;
    ASSERT_TRUE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, "keep"), "v");
    EXPECT_EQ(out.GetParam("drop"), nullptr);
    EXPECT_EQ(out.Size(), 1);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3820
 * @tc.name: parse rejects null typeId
 * @tc.desc: typeId -1 is not generated by Serialize, so it is rejected.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3820, TestSize.Level1)
{
    const std::string s = "{\"ohos.want.paramsStringEnvelope\":{\"keep\":{\"9\":\"v\"},\"drop\":{\"-1\":\"\"}}}";
    Dump("3820_input", s);

    WantParams out;
    out.SetParam("sentinel", String::Box("keep"));
    EXPECT_FALSE(WantParamWrapperJson::Parse(s, out));
    EXPECT_EQ(GetString9(out, "sentinel"), "keep");
    EXPECT_EQ(out.GetParam("drop"), nullptr);
    EXPECT_EQ(out.Size(), 1);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3900
 * @tc.name: non-envelope noise inputs rejected in batch
 * @tc.desc: 一批明显畸形/噪声输入（顶层首成员非信封 key）应全部解析失败，不抛异常、不卡死，
 *   且失败时不产出半成品结果（out 保持空）。噪声样本取自老 parser 用例 Want_Param_Wrapper_3600
 *   的同类数据，确认新 JSON parser 走"无信封→快速失败"路径，逐字符推进安全。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3900, TestSize.Level1)
{
    const std::vector<std::string> bad = {
        "{\"'\"y\"102\"{\"[8888888[0{ m}\"!};}",        // 引号/花括号交错噪声
        "{T\"'\"y\"102\"{\"[44444444 2{[0{ }}\"!};}",   // 首字符非合法 object 起始
        "{\"'\"y\"102\"{\"[8888888            -      08888 2{[0{ }}\"};}",  // 含超长空白与杂码
        "\"d\"\"101\":{  $     5P{        !   {\"a\"{\"102\"&W:n!{\"J  8888888\t}{        4,,,,,6 "
        "[    \"}  }   }}",  // 裸串起始 + 串内制表符（<0x20 控制字节）
        "{\"'\"y\"102\"{\"[888 d + n{[8888888X[2{``````````````d````````````[0{         `    C     "
        "}}\"!}!};}",  // 反引号噪声
        "",                 // 空输入
        "   \t\r\n  ",      // 仅空白
        "{",                // 孤立左花括号
        "123",              // 裸数字
        "[]",               // 裸数组
        "\"just a string\"",  // 裸字符串（非 object）
    };
    for (const auto &s : bad) {
        Dump("3900_input", s);
        WantParams out;
        EXPECT_FALSE(WantParamWrapperJson::Parse(s, out)) << "input=[" << s << "]";
        EXPECT_EQ(out.Size(), 0);  // 失败绝不产出半成品成员
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3950
 * @tc.name: envelope-present corruption rejected and out left unchanged
 * @tc.desc: 输入以合法信封 key 开头但其后结构损坏（value 非 object / 缺括号 / 缺逗号 /
 *   typeId 与 value 类型不符 / 串内裸控制字符 / 截断）应全部解析失败。与 3900 互补：本组越过
 *   信封 peek 进入 ParseParams/ParseTypedValue/ReadString 深层路径。同时验证失败原子性——
 *   预置哨兵成员在每次失败解析后必须仍存在、out 成员数不变（对应 Parse 契约"失败不修改 out"）。
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3950, TestSize.Level1)
{
    // p 为合法信封前缀，每个用例在其后拼接损坏的 value，确保越过信封判定后才失败。
    const std::string p = "{\"ohos.want.paramsStringEnvelope\":";
    std::vector<std::string> bad = {
        p + "garbage}",                                   // 信封 value 非 object
        p + "{\"k\":}}",                                  // typedValue 缺 object 体
        p + "{\"k\":{\"9\"}}",                            // typeId 后缺冒号/value
        p + "{\"k\":{\"9\":\"v\"",                        // typedValue/params/信封均未闭合
        p + "{\"k\":{\"9\":\"v\",}}",                     // 成员列表尾随逗号
        p + "{\"k\":{\"9\":\"v\" \"k2\":{\"9\":\"x\"}}}",  // 两成员间缺逗号
        p + "{\"k\":{\"101\":\"notobj\"}}",               // typeId=101 期望嵌套 object 却给字符串
        p + "{\"k\":{\"9\":\"",                           // value 字符串中途截断
    };
    // 串内裸控制字符 U+0001：JSON 禁止串内出现 U+0000-001F。显式 push_back 避免 \x 贪吃 hex 的字面量陷阱。
    std::string controlVal = p + "{\"k\":{\"9\":\"a";
    controlVal.push_back('\x01');
    controlVal += "b\"}}}";
    bad.push_back(controlVal);

    for (const auto &s : bad) {
        Dump("3950_input", s);
        WantParams out;
        out.SetParam("sentinel", String::Box("keep"));  // 预置哨兵
        EXPECT_FALSE(WantParamWrapperJson::Parse(s, out)) << "input=[" << s << "]";
        EXPECT_EQ(GetString9(out, "sentinel"), "keep");  // 失败不修改 out：哨兵仍在
        EXPECT_EQ(out.Size(), 1);                        // 仅剩预置哨兵，无半成品
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3960
 * @tc.name: legacy hierarchy noise variants rejected
 * @tc.desc: Old WantParamsWrapper malformed hierarchy strings and their variants are not JSON wrapper payloads.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3960, TestSize.Level1)
{
    const std::vector<std::string> bad = {
        "   {\"'\"y\"102\"{\"[8888888[0{ m}\"!};}",
        "{\"'\"y\"102\"{\"[8888888[0{ m}\"!};}{\"'\"y\"102\"{\"[8888888[0{ m}\"!};}",
        "{\"noise\":\"{'y'102{'[8888888[0{ m}'!};}\"}",
        "{\"x\":{\"ohos.want.paramsStringEnvelope\":{}}}",
        "{\"ohos.want.paramsStringEnvelope.extra\":{}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"v\"}}}{\"ohos.want.paramsStringEnvelope\":{}}",
        "[{\"ohos.want.paramsStringEnvelope\":{}}]",
    };

    for (const auto &s : bad) {
        Dump("3960_input", s);
        WantParams out;
        out.SetParam("sentinel", String::Box("keep"));
        EXPECT_FALSE(WantParamWrapperJson::Parse(s, out)) << "input=[" << s << "]";
        EXPECT_EQ(GetString9(out, "sentinel"), "keep");
        EXPECT_EQ(out.Size(), 1);
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3970
 * @tc.name: non-serialized envelope schema variants rejected
 * @tc.desc: Envelope strings not produced by Serialize are rejected and do not partially update output.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3970, TestSize.Level1)
{
    const std::vector<std::string> bad = {
        "{\"ohos.want.paramsStringEnvelope\":[]}",
        "{\"ohos.want.paramsStringEnvelope\":null}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":null}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":[]}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{}}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"101\":[]}}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"102\":{}}}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"102\":\"\"}}}",
        "{\"ohos.want.paramsStringEnvelope\":{\"k\":{\"9\":\"v\"}},\"extra\":{}}",
    };

    for (const auto &s : bad) {
        Dump("3970_input", s);
        WantParams out;
        out.SetParam("sentinel", String::Box("keep"));
        EXPECT_FALSE(WantParamWrapperJson::Parse(s, out)) << "input=[" << s << "]";
        EXPECT_EQ(GetString9(out, "sentinel"), "keep");
        EXPECT_EQ(out.Size(), 1);
    }
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3980
 * @tc.name: parse type-102 Array with shared depth
 * @tc.desc: Array depth is counted from the current JSON WantParams depth.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3980, TestSize.Level1)
{
    WantParams out;
    EXPECT_TRUE(WantParamWrapperJson::Parse(BuildArrayEnvelope(FULL_DEPTH), out));
    EXPECT_NE(IArray::Query(out.GetParam("array")), nullptr);

    out = WantParams();
    EXPECT_FALSE(WantParamWrapperJson::Parse(BuildArrayEnvelope(FULL_DEPTH + 1), out));
    EXPECT_EQ(out.Size(), 0);
}

/**
 * @tc.number: Want_Param_Wrapper_Json_3990
 * @tc.name: serialize type-102 Array with shared depth
 * @tc.desc: Recursive ArrayWrapperJson depth failures are propagated by JSON serialization.
 */
HWTEST_F(WantParamWrapperJsonTest, Want_Param_Wrapper_Json_3990, TestSize.Level1)
{
    WantParams okParams;
    okParams.SetParam("array", BuildNestedArrayObject(FULL_DEPTH));
    std::string serialized;
    EXPECT_TRUE(WantParamWrapperJson::Serialize(okParams, serialized));

    WantParams overLimit;
    overLimit.SetParam("array", BuildNestedArrayObject(FULL_DEPTH + 1));
    serialized = "unchanged";
    EXPECT_FALSE(WantParamWrapperJson::Serialize(overLimit, serialized));
    EXPECT_EQ(serialized, "unchanged");
}
