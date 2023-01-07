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

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include "extractor.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace AbilityBase {
namespace {
const std::string TEST_HAP_PATH("/system/app/com.ohos.settings/Settings.hap");
const std::string TEST_HAP_SEETINGS_PATH("/system/app/Settings/Settings.hap");
const std::string ERROR_HAP_PATH("/system/app/com.ohos.settings/XXX.hap");
const std::string TEST_THIRD_HAP_PATH("/data/app/el1/bundle/public/com.ohos.settings/Settings.hap");
const std::string MODULE_JSON_PATH("module.json");
const std::string CONFIG_JSON_PATH("config.json");
const std::string OUT_PATH("/data/module.json");
const std::string MAIN_ABILITY_PATH("ets/MainAbility");
const std::string FA_MAIN_ABILITY_PATH("assets/js/default");
const std::string ERROR_PATH("ets/MainAbilityXXX");
const std::string MAIN_ABILITY_FILENAME("ets/MainAbility/MainAbility.abc");
const std::string ERROR_FILENAME("ets/MainAbility/XXX.abc");
} // namespace
class ExtractorTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    std::string testPath_;
    static bool addToMap_;
};

bool ExtractorTest::addToMap_ = false;

void ExtractorTest::SetUpTestCase()
{}

void ExtractorTest::TearDownTestCase()
{}

void ExtractorTest::SetUp()
{
    std::ifstream ifs(TEST_HAP_PATH.c_str());
    if (ifs.good()) {
        testPath_ = TEST_HAP_PATH;
    } else {
        testPath_ = TEST_HAP_SEETINGS_PATH;
    }
}

void ExtractorTest::TearDown()
{}

/*
 * Feature: Extractor
 * Function: Init
 * SubFunction: NA
 * FunctionPoints:Init extractor
 * EnvConditions: NA
 * CaseDescription: Create extractor, call Init function.
 */
HWTEST_F(ExtractorTest, ExtractorInit_001, TestSize.Level1)
{
    std::string loadPath;
    std::shared_ptr<Extractor> extractor1 = std::make_shared<Extractor>(loadPath);
    EXPECT_FALSE(extractor1->Init());

    std::shared_ptr<Extractor> extractor2 = std::make_shared<Extractor>(testPath_);
    EXPECT_TRUE(extractor2->Init());
}

/*
 * Feature: Extractor
 * Function: GetExtractor
 * SubFunction: NA
 * FunctionPoints:Create extractor
 * EnvConditions: NA
 * CaseDescription: Create extractor.
 */
HWTEST_F(ExtractorTest, ExtractorCreate_001, TestSize.Level1)
{
    bool newCreate = false;
    std::string loadPath;
    std::shared_ptr<Extractor> extractor1 = ExtractorUtil::GetExtractor(loadPath, newCreate);
    EXPECT_TRUE(extractor1 == nullptr);
    EXPECT_FALSE(newCreate);

    loadPath = ERROR_HAP_PATH;
    std::shared_ptr<Extractor> extractor2 = ExtractorUtil::GetExtractor(loadPath, newCreate);
    EXPECT_TRUE(extractor2 == nullptr);
    EXPECT_FALSE(newCreate);

    std::shared_ptr<Extractor> extractor3 = ExtractorUtil::GetExtractor(testPath_, newCreate);
    EXPECT_TRUE(extractor3 != nullptr);

    loadPath = TEST_THIRD_HAP_PATH;
    std::shared_ptr<Extractor> extractor4 = ExtractorUtil::GetExtractor(loadPath, newCreate);
    EXPECT_TRUE(extractor4 == nullptr);
    EXPECT_FALSE(newCreate);
}

/*
 * Feature: Extractor
 * Function: AddExtractor
 * SubFunction: NA
 * FunctionPoints:Add extractor
 * EnvConditions: NA
 * CaseDescription: Add extractor.
 */
HWTEST_F(ExtractorTest, ExtractorAdd_001, TestSize.Level1)
{
    bool newCreate = false;
    std::string loadPath;
    std::shared_ptr<Extractor> extractor1 = ExtractorUtil::GetExtractor(loadPath, newCreate);
    EXPECT_TRUE(extractor1 == nullptr);
    EXPECT_FALSE(newCreate);
    bool addNullExtrator1 = ExtractorUtil::AddExtractor(loadPath, extractor1);
    EXPECT_FALSE(addNullExtrator1);

    loadPath = ERROR_HAP_PATH;
    std::shared_ptr<Extractor> extractor2 = ExtractorUtil::GetExtractor(loadPath, newCreate);
    EXPECT_TRUE(extractor2 == nullptr);
    EXPECT_FALSE(newCreate);
    bool addNullExtrator2 = ExtractorUtil::AddExtractor(loadPath, extractor2);
    EXPECT_FALSE(addNullExtrator2);

    std::shared_ptr<Extractor> extractor3 = ExtractorUtil::GetExtractor(testPath_, newCreate);
    EXPECT_TRUE(extractor3 != nullptr);
    if (!addToMap_) {
        EXPECT_TRUE(newCreate);
        bool addExtractor = ExtractorUtil::AddExtractor(testPath_, extractor3);
        EXPECT_TRUE(addExtractor);
        addToMap_ = true;
    } else {
        EXPECT_FALSE(newCreate);
    }

    loadPath = "";
    bool addExtractorWithEmptyPath = ExtractorUtil::AddExtractor(loadPath, extractor3);
    EXPECT_FALSE(addExtractorWithEmptyPath);
}

/*
 * Feature: Extractor
 * Function: GetLoadFilePath
 * SubFunction: NA
 * FunctionPoints:Get load file path
 * EnvConditions: NA
 * CaseDescription: Get load file path.
 */
HWTEST_F(ExtractorTest, GetLoadFilePath_001, TestSize.Level1)
{
    std::string loadPath;
    std::string loadFilePath = ExtractorUtil::GetLoadFilePath(loadPath);
    EXPECT_TRUE(loadPath == loadFilePath);

    loadPath = ERROR_HAP_PATH;
    loadFilePath = ExtractorUtil::GetLoadFilePath(loadPath);
    EXPECT_TRUE(loadPath == loadFilePath);

    loadPath = testPath_;
    loadFilePath = ExtractorUtil::GetLoadFilePath(loadPath);
    EXPECT_TRUE(loadPath == loadFilePath);

    loadPath = TEST_THIRD_HAP_PATH;
    loadFilePath = ExtractorUtil::GetLoadFilePath(loadPath);
    EXPECT_TRUE(loadPath != loadFilePath);
}

/*
 * Feature: Extractor
 * Function: GetFileBuffer
 * SubFunction: NA
 * FunctionPoints:Get file buffer
 * EnvConditions: NA
 * CaseDescription: Create extractor, call get file buffer function.
 */
HWTEST_F(ExtractorTest, GetFileBuffer_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::ostringstream outStream;
    std::string srcPath = MODULE_JSON_PATH;
    EXPECT_FALSE(extractor->GetFileBuffer(srcPath, outStream));

    extractor->Init();
    EXPECT_FALSE(extractor->GetFileBuffer("", outStream));
    EXPECT_FALSE(extractor->GetFileBuffer(CONFIG_JSON_PATH, outStream));
    EXPECT_TRUE(extractor->GetFileBuffer(srcPath, outStream));
    EXPECT_TRUE(sizeof(outStream) > 0);
}

/*
 * Feature: Extractor
 * Function: GetFileList
 * SubFunction: NA
 * FunctionPoints:Get file list
 * EnvConditions: NA
 * CaseDescription: Create extractor, call get file list function.
 */
HWTEST_F(ExtractorTest, GetFileList_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::vector<std::string> fileList;
    std::string srcPath = MAIN_ABILITY_PATH;
    EXPECT_FALSE(extractor->GetFileList(srcPath, fileList));

    extractor->Init();
    EXPECT_FALSE(extractor->GetFileList("", fileList));
    EXPECT_TRUE(extractor->GetFileList(FA_MAIN_ABILITY_PATH, fileList));
    EXPECT_TRUE(fileList.size() == 0);
    EXPECT_TRUE(extractor->GetFileList(srcPath, fileList));
    EXPECT_TRUE(fileList.size() > 0);
}

/*
 * Feature: Extractor
 * Function: HasEntry
 * SubFunction: NA
 * FunctionPoints:Has entry
 * EnvConditions: NA
 * CaseDescription: Create extractor, call has entry function.
 */
HWTEST_F(ExtractorTest, HasEntry_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::string fileName = MAIN_ABILITY_FILENAME;
    EXPECT_FALSE(extractor->HasEntry(fileName));

    extractor->Init();
    EXPECT_FALSE(extractor->HasEntry(""));
    EXPECT_FALSE(extractor->HasEntry(ERROR_FILENAME));
    EXPECT_TRUE(extractor->HasEntry(fileName));
}

/*
 * Feature: Extractor
 * Function: IsDirExist
 * SubFunction: NA
 * FunctionPoints:Is dir exist
 * EnvConditions: NA
 * CaseDescription: Create extractor, call is dir exist function.
 */
HWTEST_F(ExtractorTest, IsDirExist_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::string srcPath = MAIN_ABILITY_PATH;
    EXPECT_FALSE(extractor->IsDirExist(srcPath));

    extractor->Init();
    EXPECT_FALSE(extractor->IsDirExist(""));
    EXPECT_FALSE(extractor->IsDirExist(ERROR_PATH));
    EXPECT_TRUE(extractor->IsDirExist(srcPath));
}

/*
 * Feature: Extractor
 * Function: ExtractByName
 * SubFunction: NA
 * FunctionPoints:Extract by name
 * EnvConditions: NA
 * CaseDescription: Create extractor, call extract by name function.
 */
HWTEST_F(ExtractorTest, ExtractByName_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::ostringstream outStream;
    std::string srcPath = MODULE_JSON_PATH;
    EXPECT_FALSE(extractor->ExtractByName(srcPath, outStream));

    extractor->Init();
    EXPECT_FALSE(extractor->ExtractByName("", outStream));
    EXPECT_TRUE(extractor->ExtractByName(srcPath, outStream));
    EXPECT_TRUE(sizeof(outStream) > 0);
}

/*
 * Feature: Extractor
 * Function: ExtractFile
 * SubFunction: NA
 * FunctionPoints:Extract file
 * EnvConditions: NA
 * CaseDescription: Create extractor, call extract file function.
 */
HWTEST_F(ExtractorTest, ExtractFile_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::string outPath = OUT_PATH;
    std::string srcPath = MODULE_JSON_PATH;
    EXPECT_FALSE(extractor->ExtractFile(srcPath, outPath));

    extractor->Init();
    EXPECT_FALSE(extractor->ExtractFile("", outPath));
    EXPECT_FALSE(extractor->ExtractFile(srcPath, ""));
    EXPECT_TRUE(extractor->ExtractFile(srcPath, outPath));
    std::ifstream f(outPath.c_str());
    EXPECT_TRUE(f.good());
}

/*
 * Feature: Extractor
 * Function: GetZipFileNames
 * SubFunction: NA
 * FunctionPoints:Get zip file names
 * EnvConditions: NA
 * CaseDescription: Create extractor, call get zip file names function.
 */
HWTEST_F(ExtractorTest, GetZipFileNames_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::vector<std::string> fileList;
    EXPECT_FALSE(extractor->GetZipFileNames(fileList));
    EXPECT_TRUE(fileList.size() == 0);

    extractor->Init();
    EXPECT_TRUE(extractor->GetZipFileNames(fileList));
    EXPECT_TRUE(fileList.size() > 0);
}

/*
 * Feature: Extractor
 * Function: GetSpecifiedTypeFiles
 * SubFunction: NA
 * FunctionPoints:Get specified type files
 * EnvConditions: NA
 * CaseDescription: Create extractor, call get specified type files function.
 */
HWTEST_F(ExtractorTest, GetSpecifiedTypeFiles_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::vector<std::string> fileList;
    extractor->GetSpecifiedTypeFiles(fileList, ".abc");
    EXPECT_TRUE(fileList.size() == 0);
    extractor->Init();
    extractor->GetSpecifiedTypeFiles(fileList, ".abc");
    EXPECT_TRUE(fileList.size() > 0);
}

/*
 * Feature: Extractor
 * Function: IsStageBasedModel
 * SubFunction: NA
 * FunctionPoints:Is stage based model
 * EnvConditions: NA
 * CaseDescription: Create extractor, call is stage based model function.
 */
HWTEST_F(ExtractorTest, IsStageBasedModel_001, TestSize.Level1)
{
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(testPath_);
    std::vector<std::string> fileList;
    EXPECT_FALSE(extractor->IsStageBasedModel("MainAbility"));

    extractor->Init();
    EXPECT_FALSE(extractor->IsStageBasedModel("MainAbility"));
}

/*
 * Feature: Extractor
 * Function: IsSameHap
 * SubFunction: NA
 * FunctionPoints:Is same hap
 * EnvConditions: NA
 * CaseDescription: Create extractor, call is same hap function.
 */
HWTEST_F(ExtractorTest, IsSameHap_001, TestSize.Level1)
{
    std::string loadPath;
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(loadPath);
    EXPECT_FALSE(extractor->IsSameHap(""));

    std::shared_ptr<Extractor> extractor1 = std::make_shared<Extractor>(testPath_);
    extractor1->Init();
    EXPECT_FALSE(extractor1->IsSameHap(""));
    EXPECT_FALSE(extractor1->IsSameHap(ERROR_HAP_PATH));
    EXPECT_TRUE(extractor1->IsSameHap(testPath_));
}
}  // namespace AbilityBase
}  // namespace OHOS
