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

#ifndef OHOS_ABILITY_BASE_FILE_MAPPER_H
#define OHOS_ABILITY_BASE_FILE_MAPPER_H

#include <string>
#include <sys/mman.h>
#include <unistd.h>

namespace OHOS {
namespace AbilityBase {
class FileMapper {
public:
    FileMapper();

    ~FileMapper();

    bool CreateFileMapper(const std::string &fileName, bool compress, int32_t fd, int32_t offset, size_t len);

    bool IsCompressed();

    void* GetDataPtr();

    size_t GetDataLen();

    std::string GetFileName();

    int32_t GetFd();

private:
    static const int32_t MMAP_PORT = PROT_READ;
    static const int32_t MMAP_FLAG = MAP_SHARED;

    std::string fileName_;
    bool isCompressed = false;

    void* dataPtr_ = nullptr;
    size_t dataLen_ = 0;
    void* basePtr_ = nullptr;
    size_t baseLen_ = 0;
    int32_t fd_ = 0;
    int32_t offset_ = 0;
};
}  // namespace AbilityBase
}  // namespace OHOS
#endif  // OHOS_ABILITY_BASE_FILE_MAPPER_H