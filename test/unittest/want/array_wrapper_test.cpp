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
}
}