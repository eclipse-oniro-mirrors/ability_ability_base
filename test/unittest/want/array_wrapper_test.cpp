/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include "array_wrapper.h"
#undef private
#undef protected

using namespace testing::ext;

namespace OHOS {
namespace AAFwk {
class ArrayWrapperBaseTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: AaFwk_Array_Wrapper_Get_0100
 * @tc.name: Get
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Get_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  sptr<IInterface> value;
  ErrCode result = arr->Get(index, value);
  EXPECT_EQ(result, ERR_INVALID_VALUE);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Get_0200
 * @tc.name: Get
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Get_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  sptr<IInterface> value;
  long key = 1;
  ErrCode result = arr->Get(key, value);
  EXPECT_EQ(result, ERR_OK);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_GetLength_0100
 * @tc.name: GetLength
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_GetLength_0100, Function | MediumTest | Level1)
{
  long size = 1;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(size, typeId);
  ErrCode result = arr->GetLength(size);
  EXPECT_EQ(result, ERR_OK);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_GetType_0100
 * @tc.name: GetType
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_GetType_0100, Function | MediumTest | Level1)
{
  long size = 1;
  InterfaceID typeId = g_IID_IString;
  sptr<IArray> arr = new Array(size, typeId);
  ErrCode result = arr->GetType(typeId);
  EXPECT_EQ(result, ERR_OK);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Set_0100
 * @tc.name: Set
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Set_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  sptr<IInterface> value;
  long key = 1;
  ErrCode result = arr->Set(key, value);
  EXPECT_EQ(result, ERR_OK);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Set_0200
 * @tc.name: Set
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Set_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  sptr<IInterface> value;
  long key = -1;
  ErrCode result = arr->Set(key, value);
  EXPECT_EQ(result, ERR_INVALID_VALUE);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsBooleanArray_0100
 * @tc.name: IsBooleanArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsBooleanArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsBooleanArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsBooleanArray_0200
 * @tc.name: IsBooleanArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsBooleanArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IBoolean;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsBooleanArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsCharArray_0100
 * @tc.name: IsCharArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsCharArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsCharArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsCharArray_0200
 * @tc.name: IsCharArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsCharArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IChar;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsCharArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsByteArray_0100
 * @tc.name: IsByteArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsByteArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsByteArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsByteArray_0200
 * @tc.name: IsByteArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsByteArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IByte;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsByteArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsShortArray_0100
 * @tc.name: IsShortArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsShortArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsShortArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsShortArray_0200
 * @tc.name: IsShortArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsShortArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IShort;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsShortArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsIntegerArray_0100
 * @tc.name: IsIntegerArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsIntegerArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsIntegerArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsIntegerArray_0200
 * @tc.name: IsIntegerArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsIntegerArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IInteger;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsIntegerArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsLongArray_0100
 * @tc.name: IsLongArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsLongArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsLongArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsLongArray_0200
 * @tc.name: IsLongArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsLongArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_ILong;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsLongArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsFloatArray_0100
 * @tc.name: IsFloatArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsFloatArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsFloatArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsFloatArray_0200
 * @tc.name: IsFloatArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsFloatArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IFloat;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsFloatArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsDoubleArray_0100
 * @tc.name: IsDoubleArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsDoubleArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsDoubleArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsDoubleArray_0200
 * @tc.name: IsDoubleArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsDoubleArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IDouble;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsDoubleArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsStringArray_0100
 * @tc.name: IsStringArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsStringArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsStringArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsStringArray_0200
 * @tc.name: IsStringArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsStringArray_0200, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId = g_IID_IString;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsStringArray(arr);
  EXPECT_EQ(result, true);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_IsWantParamsArray_0100
 * @tc.name: IsWantParamsArray
 * @tc.desc: 
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_IsWantParamsArray_0100, Function | MediumTest | Level1)
{
  long index = 5;
  InterfaceID typeId;
  sptr<IArray> arr = new Array(index, typeId);
  bool result = Array::IsWantParamsArray(arr);
  EXPECT_EQ(result, false);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Parse_0100
 * @tc.name: Parse test
 * @tc.desc:
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Parse_0100, Function | MediumTest | Level1)
{
    std::function<sptr<IArray>(const std::string)> arrFunction = [](const std::string& arrayStr) -> sptr<IArray> {
        long index = 5;
        InterfaceID typeId;
        sptr<Array> arr = new Array(index, typeId);
        if (arr) {
            return arr->Parse(arrayStr);
        } else {
            return nullptr;
        }
    };
    EXPECT_EQ(arrFunction(""), nullptr);
    EXPECT_EQ(arrFunction("Test"), nullptr);
    EXPECT_EQ(arrFunction("{Test"), nullptr);
    EXPECT_EQ(arrFunction("{Test}"), nullptr);
    EXPECT_NE(arrFunction("C3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("T3{1,2,3}"), nullptr);
    EXPECT_EQ(arrFunction("Z3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("B3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("S3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("I3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("J3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("F3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("D3{1,2,3}"), nullptr);
    EXPECT_EQ(arrFunction("[3{1,2,3}"), nullptr);
    EXPECT_NE(arrFunction("W3{1,2,3}"), nullptr);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Parse_0200
 * @tc.name: Parse with oversized size returns nullptr
 * @tc.desc: Verify Array::Parse rejects an oversized size field up front. The claimed size
 *           2,000,000,000 far exceeds MAX_ARRAY_SIZE and the real element count; without the
 *           cap, ParseElement would loop ~2e9 times re-parsing the residual ("3") -- a DoS hang.
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Parse_0200, Function | MediumTest | Level1)
{
    // Claimed size (2,000,000,000) > MAX_ARRAY_SIZE; must return nullptr immediately instead
    // of driving a multi-billion-iteration ParseElement loop on the 3-element payload.
    sptr<IArray> result = Array::Parse("I2000000000{1,2,3}");
    EXPECT_EQ(result, nullptr);
}

/**
 * @tc.number: AaFwk_Array_Wrapper_Parse_0300
 * @tc.name: Parse stops once real elements are exhausted
 * @tc.desc: Verify ParseElement stops after the last real element instead of re-parsing the
 *           residual. "I5{1,2,3}" claims 5 Integer elements but only 3 are present; slots 3
 *           and 4 must keep their default nullptr rather than be filled with the residual "3".
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Parse_0300, Function | MediumTest | Level1)
{
    // size (5) exceeds the real element count (3); after "3" is consumed there is no more
    // data, so the loop must stop. Residual slots 3 and 4 stay nullptr.
    sptr<IArray> result = Array::Parse("I5{1,2,3}");
    ASSERT_NE(result, nullptr);
    long len = 0;
    EXPECT_EQ(result->GetLength(len), ERR_OK);
    EXPECT_EQ(len, 5);
    sptr<IInterface> value;
    EXPECT_EQ(result->Get(0, value), ERR_OK);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(result->Get(1, value), ERR_OK);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(result->Get(2, value), ERR_OK);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(result->Get(3, value), ERR_OK);
    EXPECT_EQ(value, nullptr);
    EXPECT_EQ(result->Get(4, value), ERR_OK);
    EXPECT_EQ(value, nullptr);
}
/**
 * @tc.number: AaFwk_Array_Wrapper_Parse_0400
 * @tc.name: Parse rejects an inflated size within MAX_ARRAY_SIZE
 * @tc.desc: Verify the size-vs-content DoS guard rejects a huge declared size with tiny content.
 */
HWTEST_F(ArrayWrapperBaseTest, AaFwk_Array_Wrapper_Parse_0400, Function | MediumTest | Level1)
{
    sptr<IArray> result = Array::Parse("[52428800{[52428800{T52428800{}}}");
    EXPECT_EQ(result, nullptr);

    sptr<IArray> ok = Array::Parse("I3{1,2,3}");
    ASSERT_NE(ok, nullptr);
    long len = 0;
    EXPECT_EQ(ok->GetLength(len), ERR_OK);
    EXPECT_EQ(len, 3);
}
}
}
