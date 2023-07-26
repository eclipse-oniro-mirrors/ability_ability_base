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

#ifndef OHOS_ABILITY_BASE_EXTRACTOR_H
#define OHOS_ABILITY_BASE_EXTRACTOR_H

#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "file_mapper.h"
#include "zip_file.h"

namespace OHOS {
namespace AbilityBase {
struct FileInfo {
    std::string fileName;
    uint32_t offset = 0;
    uint32_t length = 0;
    uint16_t lastModTime = 0;
    uint16_t lastModDate = 0;
};

class Extractor {
public:
    explicit Extractor(const std::string &source);
    virtual ~Extractor();

    /**
     * @brief Open compressed file.
     * @return Returns true if the file is successfully opened; returns false otherwise.
     */
    virtual bool Init();

    /**
     * @brief Extract to dest stream by file name.
     * @param fileName Indicates the file name.
     * @param dest Indicates the obtained std::ostream object.
     * @return Returns true if the file extracted successfully; returns false otherwise.
     */
    bool ExtractByName(const std::string &fileName, std::ostream &dest) const;
    /**
     * @brief Extract to dest path on filesystem.
     * @param fileName Indicates the file name.
     * @param targetPath Indicates the target Path.
     * @return Returns true if the file extracted to filesystem successfully; returns false otherwise.
     */
    bool ExtractFile(const std::string &fileName, const std::string &targetPath) const;
    /**
     * @brief Get specified type names in a zip file.
     * @param fileNames Indicates the obtained file names in zip.
     * @param suffix Indicates the suffix of file.
     */
    void GetSpecifiedTypeFiles(std::vector<std::string> &fileNames, const std::string &suffix);
    /**
     * @brief Has entry by name.
     * @param entryName Indicates the entry name.
     * @return Returns true if the ZipEntry is successfully finded; returns false otherwise.
     */
    bool HasEntry(const std::string &fileName) const;
    bool IsDirExist(const std::string &dir) const;
    bool IsStageBasedModel(std::string abilityName);
    bool GetFileBuffer(const std::string& srcPath, std::ostringstream& dest);
    bool GetFileList(const std::string& srcPath, std::vector<std::string>& assetList);
    bool GetFileList(const std::string &srcPath, std::set<std::string> &fileSet);
    bool IsSameHap(const std::string& hapPath) const;

    std::unique_ptr<FileMapper> GetData(const std::string &fileName, bool safeRegion = false) const;

    bool UnzipData(std::unique_ptr<FileMapper> fileMapper, std::unique_ptr<uint8_t[]> &dataPtr, size_t &len) const;
    bool IsStageModel();

    bool GetFileInfo(const std::string &fileName, FileInfo &fileInfo) const;

    bool IsHapCompress(const std::string &fileName) const;

    bool ExtractToBufByName(const std::string &fileName, std::unique_ptr<uint8_t[]> &dataPtr, size_t &len);
    /**
     * For abc file only, to mmap to safe region.
     */
    std::shared_ptr<FileMapper> GetSafeData(const std::string &fileName);
    /**
     * Cached mmap memory has been transfered to the outside and could be in use for life-long time.
     * So the zip file is not able to be cloesd.
     */
    bool IsRemoveable() const
    {
        return mapperCache_.empty();
    }
private:
    const std::string sourceFile_;
    ZipFile zipFile_;
    bool initial_ = false;
    std::string hapPath_;
    std::optional<bool> isStageModel_;

    std::mutex cacheMutex_;
    std::unordered_map<std::string, std::shared_ptr<FileMapper>> mapperCache_;
};

class ExtractorUtil {
public:
    static std::string GetLoadFilePath(const std::string &hapPath);
    static std::shared_ptr<Extractor> GetExtractor(const std::string &hapPath, bool &newCreate);
    static void DeleteExtractor(const std::string &hapPath);

private:
    static std::mutex mapMutex_;
    static std::unordered_map<std::string, std::shared_ptr<Extractor>> extractorMap_;

    static std::vector<std::shared_ptr<Extractor>> removedExtractors_;
};
}  // namespace AbilityBase
}  // namespace OHOS
#endif  // OHOS_ABILITY_BASE_EXTRACTOR_H
