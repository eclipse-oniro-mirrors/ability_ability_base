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

#include <gtest/gtest.h>

#define private public
#define protected public
#include "zchar_wrapper.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
class AAfWKZcharWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: ZcharWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetValue_001, TestSize.Level1)
{
  zchar value = 0;
  Char zcharValue(value);
  ErrCode result = zcharValue.GetValue(value);
  EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: ZcharWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_Box_001, TestSize.Level1)
{
  char value = 0;
  Char charValue(value);
  char result = charValue.Unbox(charValue.Box(value));
  EXPECT_EQ(0, result);
}

/**
 * @tc.number: ZcharWrapperTest_GetByteSize_001
 * @tc.name: GetByteSize
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetByteSize_001, TestSize.Level1)
{
    zchar value = 0xD801;
    Char zcharValue(value);
    EXPECT_EQ(0, zcharValue.GetByteSize(value));
}

/**
 * @tc.number: ZcharWrapperTest_GetByteSize_002
 * @tc.name: GetByteSize
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetByteSize_002, TestSize.Level1)
{
    zchar value = 0x10FFFF + 1;
    Char zcharValue(value);
    EXPECT_EQ(0, zcharValue.GetByteSize(value));
}

/**
 * @tc.number: ZcharWrapperTest_GetByteSize_003
 * @tc.name: GetByteSize
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetByteSize_003, TestSize.Level1)
{
    zchar value = 0x00000800 - 1;
    Char zcharValue(value);
    EXPECT_EQ(2, zcharValue.GetByteSize(value));
}

/**
 * @tc.number: ZcharWrapperTest_GetByteSize_004
 * @tc.name: GetByteSize
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetByteSize_004, TestSize.Level1)
{
    zchar value = 0x00010000 - 1;
    Char zcharValue(value);
    EXPECT_EQ(3, zcharValue.GetByteSize(value));
}

/**
 * @tc.number: ZcharWrapperTest_GetByteSize_005
 * @tc.name: GetByteSize
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetByteSize_005, TestSize.Level1)
{
    zchar value = 0x00010000 + 1;
    Char zcharValue(value);
    EXPECT_EQ(4, zcharValue.GetByteSize(value));
}

/**
 * @tc.number: ZcharWrapperTest_WriteUTF8Bytes_001
 * @tc.name: WriteUTF8Bytes
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_WriteUTF8Bytes_001, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    char *dst = new char [4];
    int32_t size = 4;
    zcharValue.WriteUTF8Bytes(dst, value, size);
}

/**
 * @tc.number: ZcharWrapperTest_WriteUTF8Bytes_002
 * @tc.name: WriteUTF8Bytes
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_WriteUTF8Bytes_002, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    char *dst = new char [4];
    int32_t size = 3;
    zcharValue.WriteUTF8Bytes(dst, value, size);
}

/**
 * @tc.number: ZcharWrapperTest_WriteUTF8Bytes_003
 * @tc.name: WriteUTF8Bytes
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_WriteUTF8Bytes_003, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    char *dst = new char [4];
    int32_t size = 2;
    zcharValue.WriteUTF8Bytes(dst, value, size);
}

/**
 * @tc.number: ZcharWrapperTest_GetChar_001
 * @tc.name: GetChar
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetChar_001, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    std::string str = "";
    int32_t index = 2;
    EXPECT_EQ(Char::INVALID_CHAR, zcharValue.GetChar(str, index));
}

/**
 * @tc.number: ZcharWrapperTest_GetChar_002
 * @tc.name: GetChar
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetChar_002, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    std::string str = "string";
    int32_t index = -1;
    EXPECT_EQ(Char::INVALID_CHAR, zcharValue.GetChar(str, index));
}

/**
 * @tc.number: ZcharWrapperTest_GetChar_003
 * @tc.name: GetChar
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetChar_003, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    std::string str = "string";
    int32_t index = 10;
    EXPECT_EQ(Char::INVALID_CHAR, zcharValue.GetChar(str, index));
}

/**
 * @tc.number: ZcharWrapperTest_GetCharInternal_001
 * @tc.name: GetCharInternal
 * @tc.desc:
 */
HWTEST_F(AAfWKZcharWrapperTest, ZcharWrapperTest_GetCharInternal_001, TestSize.Level1)
{
    zchar value = 0;
    Char zcharValue(value);
    const char* str = u8"你好";
    unsigned char* cur = reinterpret_cast<unsigned char*>(const_cast<char*>(str));
    int32_t size = 0;
    zcharValue.GetCharInternal(cur, size);
    EXPECT_EQ(3, size);
}
}
}