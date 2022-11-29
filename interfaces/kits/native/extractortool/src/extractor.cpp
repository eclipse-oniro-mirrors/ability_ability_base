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

#include "extractor.h"

#include <fstream>
#include <memory>
#include <regex>
#include <sstream>

#include "ability_base_log_wrapper.h"
#include "constants.h"
#include "file_path_utils.h"
#include "string_ex.h"

namespace OHOS {
namespace AbilityBase {
Extractor::Extractor(const std::string &source) : sourceFile_(source), zipFile_(source)
{
    hapPath_ = source;
}

Extractor::~Extractor()
{}

bool Extractor::Init()
{
    if (!zipFile_.Open()) {
        ABILITYBASE_LOGE("open zip file failed");
        return false;
    }
    ZipEntry zipEntry;
    initial_ = true;
    return true;
}

std::shared_ptr<Extractor> Extractor::Create(const std::string& hapPath)
{
    if (hapPath.empty()) {
        ABILITYBASE_LOGE("source is nullptr");
        return nullptr;
    }

    std::string loadPath;
    if (StringStartWith(hapPath, Constants::ABS_CODE_PATH, std::string(Constants::ABS_CODE_PATH).length())) {
        loadPath = GetLoadPath(hapPath);
    } else {
        loadPath = hapPath;
    }
    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(loadPath);
    if (!extractor->Init()) {
        ABILITYBASE_LOGE("Extractor create failed for %{public}s", loadPath.c_str());
        return nullptr;
    }

    return extractor;
}

bool Extractor::GetFileBuffer(const std::string& srcPath, std::ostringstream& dest)
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }

    if (srcPath.empty()) {
        ABILITYBASE_LOGE("GetFileBuffer::srcPath is nullptr");
        return false;
    }

    std::string relativePath = GetRelativePath(srcPath);
    if (!ExtractByName(relativePath, dest)) {
        ABILITYBASE_LOGE("GetFileBuffer::Extract file failed");
        return false;
    }

    return true;
}

bool Extractor::GetFileList(const std::string& srcPath, std::vector<std::string>& assetList)
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }

    if (srcPath.empty()) {
        ABILITYBASE_LOGE("GetFileList::srcPath is nullptr");
        return false;
    }

    std::vector<std::string> fileList;
    if (!GetZipFileNames(fileList)) {
        ABILITYBASE_LOGE("GetFileList::Get file list failed");
        return false;
    }

    std::regex replacePattern(srcPath);
    for (auto value : fileList) {
        if (StringStartWith(value, srcPath.c_str(), srcPath.length())) {
            assetList.emplace_back(value);
        }
    }

    return true;
}

bool Extractor::HasEntry(const std::string &fileName) const
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }

    return zipFile_.HasEntry(fileName);
}

bool Extractor::IsDirExist(const std::string &dir) const
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }
    if (dir.empty()) {
        ABILITYBASE_LOGE("param dir empty");
        return false;
    }
    return zipFile_.IsDirExist(dir);
}

bool Extractor::ExtractByName(const std::string &fileName, std::ostream &dest) const
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }
    if (!zipFile_.ExtractFile(fileName, dest)) {
        ABILITYBASE_LOGE("extractor is not ExtractFile");
        return false;
    }
    return true;
}

bool Extractor::ExtractFile(const std::string &fileName, const std::string &targetPath) const
{
    std::ofstream fileStream;
    fileStream.open(targetPath, std::ios_base::out | std::ios_base::binary);
    if (!fileStream.is_open()) {
        ABILITYBASE_LOGE("fail to open %{private}s file to write", targetPath.c_str());
        return false;
    }
    if ((!ExtractByName(fileName, fileStream)) || (!fileStream.good())) {
        ABILITYBASE_LOGE("fail to extract %{public}s zip file into stream", fileName.c_str());
        fileStream.clear();
        fileStream.close();
        if (remove(targetPath.c_str()) != 0) {
            ABILITYBASE_LOGE("fail to remove %{private}s file which writes stream error", targetPath.c_str());
        }
        return false;
    }
    fileStream.clear();
    fileStream.close();
    return true;
}

bool Extractor::GetZipFileNames(std::vector<std::string> &fileNames)
{
    auto &entryMap = zipFile_.GetAllEntries();
    if (entryMap.empty()) {
        ABILITYBASE_LOGE("Zip file is empty");
        return false;
    }

    for (auto &entry : entryMap) {
        fileNames.emplace_back(entry.first);
    }
    return true;
}

void Extractor::GetSpecifiedTypeFiles(std::vector<std::string> &fileNames, const std::string &suffix)
{
    auto &entryMap = zipFile_.GetAllEntries();
    for (const auto &entry : entryMap) {
        std::string fileName = entry.first;
        auto position = fileName.rfind('.');
        if (position != std::string::npos) {
            std::string suffixStr = fileName.substr(position);
            if (LowerStr(suffixStr) == suffix) {
                fileNames.emplace_back(fileName);
            }
        }
    }
}

bool Extractor::IsStageBasedModel(std::string abilityName)
{
    auto &entryMap = zipFile_.GetAllEntries();
    std::vector<std::string> splitStrs;
    OHOS::SplitStr(abilityName, ".", splitStrs);
    std::string name = splitStrs.empty() ? abilityName : splitStrs.back();
    std::string entry = "assets/js/" + name + "/" + name + ".js";
    bool isStageBasedModel = entryMap.find(entry) != entryMap.end();
    return isStageBasedModel;
}

bool Extractor::IsSameHap(const std::string& hapPath) const
{
    return !hapPath_.empty() && !hapPath.empty() && hapPath_ == hapPath;
}

void Extractor::SetRuntimeFlag(bool isRuntime)
{
    zipFile_.SetIsRuntime(isRuntime);
}
}  // namespace AbilityBase
}  // namespace OHOS
