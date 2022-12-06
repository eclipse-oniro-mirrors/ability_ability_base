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

#include "file_mapper.h"
#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AbilityBase {
FileMapper::FileMapper()
{}

FileMapper::~FileMapper()
{
    if (basePtr_) {
        munmap(basePtr_, baseLen_);
    }
}

bool FileMapper::CreateFileMapper(const std::string &fileName, bool compress, int32_t fd, int32_t offset, size_t len)
{
    fileName_ = fileName;
    fd_ = fd;
    offset_ = offset;
    dataLen_ = len;
    isCompressed = compress;

    int32_t pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize <= 0) {
        ABILITYBASE_LOGE("CreateFileMapper. pageSize[%{public}d]", pageSize);
        return false;
    }
    int32_t adjust = offset % pageSize;
    int32_t adjOffset = offset - adjust;
    baseLen_ = dataLen_ + adjust;
    basePtr_ = mmap(nullptr, baseLen_, MMAP_PORT, MMAP_FLAG, fd, adjOffset);
    if (basePtr_ == MAP_FAILED) {
        return false;
    }
    dataPtr_ = reinterpret_cast<uint8_t *>(basePtr_) + adjust;
    return true;
}

bool FileMapper::IsCompressed()
{
    return isCompressed;
}

void* FileMapper::GetDataPtr()
{
    return dataPtr_;
}

size_t FileMapper::GetDataLen()
{
    return dataLen_;
}

std::string FileMapper::GetFileName()
{
    return fileName_;
}

int32_t FileMapper::GetFd()
{
    return fd_;
}

int32_t FileMapper::GetOffset()
{
    return offset_;
}
}  // namespace AbilityBase
}  // namespace OHOS
