/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "configuration.h"
#undef private

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AppExecFwk {
class BaseConfigurationTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void BaseConfigurationTest::SetUpTestCase()
{
}

void BaseConfigurationTest::TearDownTestCase()
{
}

void BaseConfigurationTest::SetUp()
{
}

void BaseConfigurationTest::TearDown()
{
}

/*
 * Feature: Configuration
 * Function: FilterDuplicates
 * SubFunction: NA
 * FunctionPoints:FilterDuplicates Configuration
 * EnvConditions: NA
 * CaseDescription: FilterDuplicates Configuratio, call FilterDuplicates function.
 */
HWTEST_F(BaseConfigurationTest, Configuration_FilterDuplicates_001, TestSize.Level1)
{
    AppExecFwk::Configuration config;
    ASSERT_TRUE(config.configParameter_.empty());
    auto configuration = std::make_shared<AppExecFwk::Configuration>();
    configuration->FilterDuplicates(config);
    config.configParameter_.emplace(std::make_pair("key1", "value1"));
    configuration->configParameter_.emplace(std::make_pair("key1", "value1"));
    configuration->FilterDuplicates(config);
    EXPECT_EQ(configuration->configParameter_.size(), 0);
}
} // namespace AppExecFwk
} // namespace OHOS