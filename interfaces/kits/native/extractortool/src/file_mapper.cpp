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

#include <cerrno>

#include "file_mapper.h"
#include "ability_base_log_wrapper.h"

namespace OHOS {
namespace AbilityBase {
int32_t FileMapper::pageSize_ = 0;
FileMapper::FileMapper()
{
    if (pageSize_ <= 0) {
        pageSize_ = sysconf(_SC_PAGESIZE);
    }
}

FileMapper::~FileMapper()
{
    if (basePtr_) {
        munmap(basePtr_, baseLen_);
    }
}

bool FileMapper::CreateFileMapper(const std::string &fileName, bool compress,
    int32_t fd, int32_t offset, size_t len, bool safeRegion)
{
    if (pageSize_ <= 0) {
        ABILITYBASE_LOGE("CreateFileMapper. pageSize[%{public}d]", pageSize_);
        return false;
    }

    fileName_ = fileName;
    offset_ = offset;
    dataLen_ = len;
    isCompressed = compress;
    int32_t adjust = offset % pageSize_;
    int32_t adjOffset = offset - adjust;
    baseLen_ = dataLen_ + adjust;
    int32_t mmapFlag = MMAP_FLAG;
    int32_t prot = MMAP_PROT;
    if (safeRegion) {
        mmapFlag = MAP_PRIVATE|MAP_XPM;
        prot |= PROT_WRITE;
    }
    basePtr_ = mmap(nullptr, baseLen_, prot, mmapFlag, fd, adjOffset);
    if (basePtr_ == MAP_FAILED) {
        ABILITYBASE_LOGE("CreateFileMapper, mmap failed, errno[%{public}d].", errno);
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

int32_t FileMapper::GetOffset()
{
    return offset_;
}
}  // namespace AbilityBase
}  // namespace OHOS
