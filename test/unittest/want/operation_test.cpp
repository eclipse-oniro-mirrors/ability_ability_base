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
#include "operation.h"
#include "operation_builder.h"
#undef private
#undef protected

using namespace testing::ext;
using namespace OHOS::AAFwk;
using OHOS::Parcel;
using Uri = OHOS::Uri;
class OperationBaseTest : public testing::Test {
public:
    OperationBaseTest()
    {}
    ~OperationBaseTest()
    {}
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();

    std::shared_ptr<OperationBuilder> operationbuilder_ = nullptr;
};

void OperationBaseTest::SetUpTestCase(void)
{}

void OperationBaseTest::TearDownTestCase(void)
{}

void OperationBaseTest::SetUp(void)
{
    operationbuilder_ = std::make_shared<OperationBuilder>();
}

void OperationBaseTest::TearDown(void)
{}

/**
 * @tc.number: AaFwk_Operation_GetAbilityName_0100
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAbilityName_0100, Function | MediumTest | Level1)
{
    std::string value = "enter";
    GTEST_LOG_(INFO) << "AaFwk_Operation_GetAbilityName_0100 start";

    operationbuilder_->WithAbilityName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetAbilityName().c_str());

    GTEST_LOG_(INFO) << "AaFwk_Operation_GetAbilityName_0100 end";
}

/**
 * @tc.number: AaFwk_Operation_GetAbilityName_0200
 * @tc.name: WithAbilityName/GetAbilityName.
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAbilityName_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithAbilityName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();

    EXPECT_STREQ(value.c_str(), operation->GetAbilityName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetAbilityNameRef_0100
 * @tc.name: WithAbilityName/GetAbilityNameRef
 * @tc.desc: Verify GetAbilityNameRef returns a const reference with the correct value.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAbilityNameRef_0100, Function | MediumTest | Level1)
{
    std::string value = "enter";
    operationbuilder_->WithAbilityName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetAbilityNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->abilityName_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetAbilityNameRef_0200
 * @tc.name: WithAbilityName/GetAbilityNameRef
 * @tc.desc: Verify GetAbilityNameRef works when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAbilityNameRef_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithAbilityName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetAbilityNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->abilityName_), &ref);
}

/**
 * @tc.number:  AaFwk_Operation_GetBundleName_0100
 * @tc.name: WithBundleName/GetBundleName
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetBundleName_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    operationbuilder_->WithBundleName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetBundleName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetBundleNameRef_0100
 * @tc.name: WithBundleName/GetBundleNameRef
 * @tc.desc: Verify GetBundleNameRef returns a const reference with the correct value.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetBundleNameRef_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    operationbuilder_->WithBundleName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetBundleNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->bundleName_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetBundleNameRef_0200
 * @tc.name: WithBundleName/GetBundleNameRef
 * @tc.desc: Verify GetBundleNameRef works when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetBundleNameRef_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithBundleName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetBundleNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->bundleName_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetBundleName_0200
 * @tc.name: WithBundleName/GetBundleName
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetBundleName_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithBundleName(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetBundleName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetDeviceId_0100
 * @tc.name: WithDeviceId/GetDeviceId
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetDeviceId_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    operationbuilder_->WithDeviceId(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetDeviceId().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetDeviceId_0200
 * @tc.name: WithDeviceId/GetDeviceId
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetDeviceId_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithDeviceId(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetDeviceId().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetDeviceIdRef_0100
 * @tc.name: WithDeviceId/GetDeviceIdRef
 * @tc.desc: Verify GetDeviceIdRef returns a const reference with the correct value.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetDeviceIdRef_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    operationbuilder_->WithDeviceId(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetDeviceIdRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->deviceId_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetDeviceIdRef_0200
 * @tc.name: WithDeviceId/GetDeviceIdRef
 * @tc.desc: Verify GetDeviceIdRef works when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetDeviceIdRef_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithDeviceId(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    const std::string &ref = operation->GetDeviceIdRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->deviceId_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetAction_0100
 * @tc.name: WithAction/GetAction
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAction_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    operationbuilder_->WithAction(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetAction().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetAction_0200
 * @tc.name: WithAction/GetAction
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetAction_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    operationbuilder_->WithAction(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_STREQ(value.c_str(), operation->GetAction().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetModuleName_0100
 * @tc.name: SetModuleName/GetModuleName
 * @tc.desc: Verify GetModuleName returns the correct value.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetModuleName_0100, Function | MediumTest | Level1)
{
    std::string value = "module01";
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    operation->SetModuleName(value);
    EXPECT_STREQ(value.c_str(), operation->GetModuleName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetModuleName_0200
 * @tc.name: SetModuleName/GetModuleName
 * @tc.desc: Verify GetModuleName works when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetModuleName_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    operation->SetModuleName(value);
    EXPECT_STREQ(value.c_str(), operation->GetModuleName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_GetModuleNameRef_0100
 * @tc.name: SetModuleName/GetModuleNameRef
 * @tc.desc: Verify GetModuleNameRef returns a const reference with the correct value.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetModuleNameRef_0100, Function | MediumTest | Level1)
{
    std::string value = "module01";
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    operation->SetModuleName(value);
    const std::string &ref = operation->GetModuleNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->moduleName_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetModuleNameRef_0200
 * @tc.name: SetModuleName/GetModuleNameRef
 * @tc.desc: Verify GetModuleNameRef works when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetModuleNameRef_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    operation->SetModuleName(value);
    const std::string &ref = operation->GetModuleNameRef();
    EXPECT_STREQ(value.c_str(), ref.c_str());
    EXPECT_EQ(&(operation->moduleName_), &ref);
}

/**
 * @tc.number: AaFwk_Operation_GetEntities_0100
 * @tc.name: WithEntities/GetEntities
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetEntities_0100, Function | MediumTest | Level1)
{
    std::vector<std::string> value;
    value.push_back("string1");
    operationbuilder_->WithEntities(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();

    std::vector<std::string> revValue = operation->GetEntities();

    if (value.size() > 0 && revValue.size() > 0) {
        EXPECT_STREQ(value.at(0).c_str(), operation->GetEntities().at(0).c_str());
    } else {
        EXPECT_EQ(true, revValue.size() > 0);
    }
}

/**
 * @tc.number: AaFwk_Operation_GetEntities_0200
 * @tc.name: WithEntities/GetEntities
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetEntities_0200, Function | MediumTest | Level3)
{
    std::vector<std::string> value;
    operationbuilder_->WithEntities(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_EQ(true, operation->GetEntities().size() == 0);
}

/**
 * @tc.number: AaFwk_Operation_GetFlags_0100
 * @tc.name: WithFlags/GetFlags
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetFlags_0100, Function | MediumTest | Level1)
{
    unsigned int value = 1;
    operationbuilder_->WithFlags(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_EQ(value, operation->GetFlags());
}

/**
 * @tc.number: AaFwk_Operation_GetFlags_0200
 * @tc.name: WithFlags/GetFlags
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetFlags_0200, Function | MediumTest | Level3)
{
    unsigned int value = 0;
    operationbuilder_->WithFlags(value);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_EQ(value, operation->GetFlags());
}

/**
 * @tc.number: AaFwk_Operation_GetUri_0100
 * @tc.name: WithUri/GetUri
 * @tc.desc: Verify the function when the input string contains special characters.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetUri_0100, Function | MediumTest | Level1)
{
    std::string value = "scheme://authority/path1/path2/path3?id = 1&name = mingming&old#fragment";
    OHOS::Uri uri(value);
    operationbuilder_->WithUri(uri);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();

    EXPECT_EQ(uri, operation->GetUri());
}

/**
 * @tc.number: AaFwk_Operation_GetUri_0200
 * @tc.name: WithUri/GetUri
 * @tc.desc: Verify the function when the input string is empty.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_GetUri_0200, Function | MediumTest | Level3)
{
    std::string value = "";
    OHOS::Uri uri(value);
    operationbuilder_->WithUri(uri);
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    EXPECT_EQ(uri, operation->GetUri());
}

/**
 * @tc.number: AaFwk_Operation_build_0100
 * @tc.name: build
 * @tc.desc: Verify that the parameters are correct.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_build_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    OHOS::Uri uri(value);
    std::vector<std::string> columns;
    columns.push_back("string1");
    operationbuilder_->WithUri(uri);
    operationbuilder_->WithAction(value);
    operationbuilder_->WithEntities(columns);
    operationbuilder_->WithDeviceId(value);
    operationbuilder_->WithBundleName(value);
    operationbuilder_->WithAbilityName(value);

    std::shared_ptr<Operation> operation = operationbuilder_->Build();

    EXPECT_EQ(uri, operation->GetUri());
    EXPECT_STREQ(value.c_str(), operation->GetAction().c_str());

    std::vector<std::string> revValue = operation->GetEntities();

    if (columns.size() > 0 && revValue.size() > 0) {
        EXPECT_STREQ(columns.at(0).c_str(), operation->GetEntities().at(0).c_str());
    } else {
        EXPECT_EQ(true, revValue.size() > 0);
    }
    EXPECT_STREQ(value.c_str(), operation->GetDeviceId().c_str());
    EXPECT_STREQ(value.c_str(), operation->GetBundleName().c_str());
    EXPECT_STREQ(value.c_str(), operation->GetAbilityName().c_str());
}

/**
 * @tc.number: AaFwk_Operation_Marshalling_0100
 * @tc.name: Marshalling/Unmarshalling
 * @tc.desc: Validation serialization.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_Marshalling_0100, Function | MediumTest | Level1)
{
    std::string value = "value";
    OHOS::Uri uri(value);
    std::vector<std::string> columns;
    columns.push_back("string1");
    operationbuilder_->WithUri(uri);
    operationbuilder_->WithAction(value);
    operationbuilder_->WithEntities(columns);
    operationbuilder_->WithDeviceId(value);
    operationbuilder_->WithBundleName(value);
    operationbuilder_->WithAbilityName(value);

    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    Parcel in;
    operation->Marshalling(in);

    Operation *pOperation = operation->Unmarshalling(in);
    if (pOperation != nullptr) {
        EXPECT_EQ(true, *pOperation == *(operation.get()));
    } else {
        EXPECT_EQ(true, pOperation != nullptr);
    }
}

/**
 * @tc.number: AaFwk_Operation_Operator_0100
 * @tc.name: Operator
 * @tc.desc: Verify string overload.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_Operator_0100, Function | MediumTest | Level1)
{
    Operation operation_;
    std::string value = "value";
    OHOS::Uri uri(value);
    std::vector<std::string> columns;
    columns.push_back("string1");
    operationbuilder_->WithUri(uri);
    operationbuilder_->WithAction(value);
    operationbuilder_->WithEntities(columns);
    operationbuilder_->WithDeviceId(value);
    operationbuilder_->WithBundleName(value);
    operationbuilder_->WithAbilityName(value);

    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    operation_ = *(operation.get());

    EXPECT_EQ(true, operation_ == *(operation.get()));
}

/**
 * @tc.number: AaFwk_Operation_DumpInfo_0100
 * @tc.name: DumpInfo
 * @tc.desc: Verify string overload.
 */
HWTEST_F(OperationBaseTest, AaFwk_Operation_DumpInfo_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO) << "AaFwk_Operation_DumpInfo_0100 start";
    std::shared_ptr<Operation> operation = operationbuilder_->Build();
    int level = 10;
    operation->entities_.push_back("a");
    operation->entities_.push_back("b");
    operation->DumpInfo(level);
    EXPECT_EQ(true, operation->GetEntities().size() == 2);
    GTEST_LOG_(INFO) << "AaFwk_Operation_DumpInfo_0100 end";
}
