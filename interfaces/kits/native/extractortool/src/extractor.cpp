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
#include "securec.h"
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
    initial_ = true;
    return true;
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

std::unique_ptr<FileMapper> Extractor::GetData(const std::string &fileName, bool safeRegion) const
{
    int32_t fd = 0;
    ZipPos offset = 0;
    uint32_t len = 0;
    bool compress = false;

    std::string relativePath = GetRelativePath(fileName);
    if (!zipFile_.GetEntryInfoByName(relativePath, compress, fd, offset, len)) {
        ABILITYBASE_LOGE("Get entry info by name failed. fileName: %{public}s", fileName.c_str());
        return nullptr;
    }

    std::unique_ptr<FileMapper> fileMapper = std::make_unique<FileMapper>();
    if (!fileMapper->CreateFileMapper(relativePath, compress, fd,
        static_cast<int32_t>(offset), static_cast<size_t>(len), safeRegion)) {
        ABILITYBASE_LOGE("Create file mapper failed. fileName: %{public}s", fileName.c_str());
        return nullptr;
    }
    return fileMapper;
}

bool Extractor::UnzipData(std::unique_ptr<FileMapper> fileMapper,
    std::unique_ptr<uint8_t[]> &dataPtr, size_t &len) const
{
    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }

    if (!fileMapper) {
        ABILITYBASE_LOGE("Input fileMapper is nullptr.");
        return false;
    }

    if (!zipFile_.ExtractFileFromMMap(fileMapper->GetFileName(), fileMapper->GetDataPtr(), dataPtr, len)) {
        ABILITYBASE_LOGE("extract file from MMap failed.");
        return false;
    }
    return true;
}

bool Extractor::GetUncompressedData(std::unique_ptr<FileMapper> fileMapper,
    std::unique_ptr<uint8_t[]> &dataPtr, size_t &len, bool safeRegion) const
{
    struct sigaction oldAct;
    ZipFile::HandleSignal(oldAct);

    if (!initial_) {
        ABILITYBASE_LOGE("extractor is not initial");
        return false;
    }

    if (!fileMapper) {
        ABILITYBASE_LOGE("Input fileMapper is nullptr.");
        return false;
    }

    void *dataSrc = fileMapper->GetDataPtr();
    len = fileMapper->GetDataLen();
    if (!dataSrc || len == 0) {
        ABILITYBASE_LOGE("dataSrc is nullptr or len is 0.");
        return false;
    }

    if (safeRegion) {
        std::unique_ptr<uint8_t[]> dataDst(static_cast<uint8_t *>(dataSrc));
        dataPtr = std::move(dataDst);
        fileMapper.release();
    } else {
        dataPtr = std::make_unique<uint8_t[]>(len);
        if (!dataPtr) {
            ABILITYBASE_LOGE("Make unique ptr failed.");
            return false;
        }

        uint8_t *dataDst = static_cast<uint8_t*>(dataPtr.get());
        if (memcpy_s(dataDst, len, dataSrc, len) != EOK) {
            ABILITYBASE_LOGE("memory copy failed.");
            return false;
        }
    }
    ZipFile::RecoverSignalHandler(oldAct);
    return true;
}

bool Extractor::IsStageModel() const
{
    std::string fileName = "config.json";
    return !zipFile_.HasEntry(fileName);
}

bool Extractor::ExtractToBufByName(const std::string &fileName, std::unique_ptr<uint8_t[]> &dataPtr, size_t &len,
    bool safeRegion) const
{
    std::unique_ptr<FileMapper> fileMapper = GetData(fileName, safeRegion);
    if (!fileMapper) {
        ABILITYBASE_LOGE("Get file mapper by fileName[%{public}s] failed.", fileName.c_str());
        return false;
    }

    if (fileMapper->IsCompressed()) {
        return UnzipData(std::move(fileMapper), dataPtr, len);
    }

    return GetUncompressedData(std::move(fileMapper), dataPtr, len, safeRegion);
}

bool Extractor::GetFileInfo(const std::string &fileName, FileInfo &fileInfo) const
{
    std::string relativePath = GetRelativePath(fileName);
    ZipEntry zipEntry;
    if (!zipFile_.GetEntry(relativePath, zipEntry)) {
        ABILITYBASE_LOGE("Get zip entry failed.");
        return false;
    }

    ZipPos offset = 0;
    uint32_t length = 0;
    if (!zipFile_.GetDataOffsetRelative(relativePath, offset, length)) {
        ABILITYBASE_LOGE("Get data offset relative failed.");
        return false;
    }

    fileInfo.fileName = fileName;
    fileInfo.offset = static_cast<uint32_t>(offset);
    fileInfo.length = static_cast<uint32_t>(length);
    fileInfo.lastModTime = zipEntry.modifiedTime;
    fileInfo.lastModDate = zipEntry.modifiedDate;
    return true;
}

bool IsFilePathExist(const std::string rawfilePath, const std::vector<std::string> &pathList)
{
    for (auto path : pathList) {
        if (path.find(rawfilePath) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool Extractor::GetFileList(const std::string &srcPath, std::set<std::string> &fileSet)
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

    std::string pureSrcPath = srcPath;
    if (srcPath[srcPath.length() - 1] == '/') {
        pureSrcPath.assign(srcPath.begin(), srcPath.end() - 1);
    }

    if (!IsFilePathExist(pureSrcPath, fileList)) {
        ABILITYBASE_LOGE("GetFileList::srcPath invalid pureSrcPath, %{public}s", pureSrcPath.c_str());
        return false;
    }

    for (auto value : fileList) {
        if (StringStartWith(value, pureSrcPath.c_str(), pureSrcPath.length())) {
            if (value.length() == pureSrcPath.length() || value[pureSrcPath.length()] != '/') {
                continue;
            }
            auto separatorPos = value.find('/', pureSrcPath.length() + 1);
            fileSet.insert(value.substr(pureSrcPath.length() + 1, separatorPos - pureSrcPath.length() - 1));
        }
    }

    return true;
}

std::mutex ExtractorUtil::mapMutex_;
std::map<std::string, std::shared_ptr<Extractor>> ExtractorUtil::extractorMap_;
std::string ExtractorUtil::GetLoadFilePath(const std::string &hapPath)
{
    std::string loadPath;
    if (StringStartWith(hapPath, Constants::ABS_CODE_PATH, std::string(Constants::ABS_CODE_PATH).length())) {
        loadPath = GetLoadPath(hapPath);
    } else {
        loadPath = hapPath;
    }
    return loadPath;
}

std::shared_ptr<Extractor> ExtractorUtil::GetExtractor(const std::string &hapPath, bool &newCreate)
{
    newCreate = false;
    if (hapPath.empty()) {
        ABILITYBASE_LOGE("Input hapPath is empty.");
        return nullptr;
    }

    {
        std::lock_guard<std::mutex> mapMutex(mapMutex_);
        auto mapIter = extractorMap_.find(hapPath);
        if (mapIter != extractorMap_.end()) {
            ABILITYBASE_LOGD("Extractor exists, hapPath: %{private}s.", hapPath.c_str());
            return mapIter->second;
        }
    }

    std::shared_ptr<Extractor> extractor = std::make_shared<Extractor>(hapPath);
    if (!extractor->Init()) {
        ABILITYBASE_LOGE("Extractor create failed for %{private}s", hapPath.c_str());
        return nullptr;
    }
    newCreate = true;
    return extractor;
}

bool ExtractorUtil::AddExtractor(const std::string &hapPath, std::shared_ptr<Extractor> extractor)
{
    if (hapPath.empty()) {
        ABILITYBASE_LOGE("Input hapPath is empty.");
        return false;
    }

    if (!extractor) {
        ABILITYBASE_LOGE("Input extractor is nullptr.");
        return false;
    }

    std::lock_guard<std::mutex> mapMutex(mapMutex_);
    if (extractorMap_.find(hapPath) != extractorMap_.end()) {
        ABILITYBASE_LOGD("Extractor exists, update. hapPath: %{public}s.", hapPath.c_str());
    } else {
        ABILITYBASE_LOGD("Extractor doesn't exist, add. hapPath: %{public}s.", hapPath.c_str());
    }
    extractorMap_[hapPath] = extractor;

    return true;
}

void ExtractorUtil::DeleteExtractor(const std::string &hapPath)
{
    if (hapPath.empty()) {
        ABILITYBASE_LOGE("DeleteExtractor hapPath is empty.");
        return;
    }

    std::lock_guard<std::mutex> mapMutex(mapMutex_);
    auto mapIter = extractorMap_.find(hapPath);
    if (mapIter != extractorMap_.end()) {
        ABILITYBASE_LOGI("DeleteExtractor, hapPath: %{public}s.", hapPath.c_str());
        extractorMap_.erase(mapIter);
    }
}
}  // namespace AbilityBase
}  // namespace OHOS
