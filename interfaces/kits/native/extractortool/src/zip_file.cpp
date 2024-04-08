/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

#include "zip_file.h"

#include <ostream>

#include "ability_base_log_wrapper.h"
#include "constants.h"
#include "file_mapper.h"
#include "file_path_utils.h"
#include "hitrace_meter.h"
#include "securec.h"
#include "zip_file_reader.h"
#include "zlib.h"

namespace OHOS {
namespace AbilityBase {
namespace {
constexpr uint32_t MAX_FILE_NAME = 4096;
constexpr uint32_t UNZIP_BUFFER_SIZE = 1024;
constexpr uint32_t UNZIP_BUF_IN_LEN = 160 * UNZIP_BUFFER_SIZE;   // in  buffer length: 160KB
constexpr uint32_t UNZIP_BUF_OUT_LEN = 320 * UNZIP_BUFFER_SIZE;  // out buffer length: 320KB
constexpr uint32_t LOCAL_HEADER_SIGNATURE = 0x04034b50;
constexpr uint32_t CENTRAL_SIGNATURE = 0x02014b50;
constexpr uint32_t EOCD_SIGNATURE = 0x06054b50;
constexpr uint32_t DATA_DESC_SIGNATURE = 0x08074b50;
constexpr uint32_t FLAG_DATA_DESC = 0x8;
constexpr uint8_t INFLATE_ERROR_TIMES = 5;
constexpr uint8_t MAP_FILE_SUFFIX = 4;
const char FILE_SEPARATOR_CHAR = '/';

void GetTreeFileList(std::shared_ptr<DirTreeNode> root, const std::string &rootPath,
    std::vector<std::string> &assetList)
{
    if (root == nullptr) {
        return;
    }
    if (root->children.empty()) {
        assetList.push_back(rootPath);
    } else {
        for (const auto &child : root->children) {
            GetTreeFileList(child.second, rootPath + "/" + child.first, assetList);
        }
    }
}

void AddEntryToTree(const std::string &fileName, std::shared_ptr<DirTreeNode> root)
{
    if (root == nullptr) {
        return;
    }
    size_t cur = 0;
    auto parent = root;
    do {
        while (cur < fileName.size() && fileName[cur] == FILE_SEPARATOR_CHAR) {
            cur++;
        }
        if (cur >= fileName.size()) {
            break;
        }
        auto next = fileName.find_first_of(FILE_SEPARATOR_CHAR, cur);
        auto nodeName = fileName.substr(cur, next - cur);
        auto it = parent->children.find(nodeName);
        if (it != parent->children.end()) {
            parent = it->second;
        } else {
            auto node = std::make_shared<DirTreeNode>();
            parent->children.emplace(nodeName, node);
            parent = node;
        }
        cur = next;
    } while (cur != std::string::npos);
}
}  // namespace

ZipEntry::ZipEntry(const CentralDirEntry &centralEntry)
{
    compressionMethod = centralEntry.compressionMethod;
    uncompressedSize = centralEntry.uncompressedSize;
    compressedSize = centralEntry.compressedSize;
    localHeaderOffset = centralEntry.localHeaderOffset;
    crc = centralEntry.crc;
    flags = centralEntry.flags;
    modifiedTime = centralEntry.modifiedTime;
    modifiedDate = centralEntry.modifiedDate;
}

ZipFile::ZipFile(const std::string &pathName) : pathName_(pathName) {}

ZipFile::~ZipFile()
{
    Close();
}

void ZipFile::SetContentLocation(const ZipPos start, const size_t length)
{
    if (isOpen_) {
        ABILITYBASE_LOGE("file has been opened already.");
        return;
    }
    fileStartPos_ = start;
    fileLength_ = length;
}

bool ZipFile::CheckEndDir(const EndDir &endDir) const
{
    size_t lenEndDir = sizeof(EndDir);
    if ((endDir.numDisk != 0) || (endDir.signature != EOCD_SIGNATURE) || (endDir.startDiskOfCentralDir != 0) ||
        (endDir.offset >= fileLength_) || (endDir.totalEntriesInThisDisk != endDir.totalEntries) ||
        (endDir.commentLen != 0) ||
        // central dir can't overlap end of central dir
        ((endDir.offset + endDir.sizeOfCentralDir + lenEndDir) > fileLength_)) {
        ABILITYBASE_LOGE("end dir format error");
        return false;
    }
    return true;
}

bool ZipFile::ParseEndDirectory()
{
    size_t endDirLen = sizeof(EndDir);
    size_t endFilePos = fileStartPos_ + fileLength_;

    if (fileLength_ <= endDirLen) {
        ABILITYBASE_LOGE("parse EOCD file length(%{public}llu) <= end dir length(%{public}llu)",
            fileStartPos_, fileLength_);
        return false;
    }

    size_t eocdPos = endFilePos - endDirLen;
    if (!zipFileReader_->ReadBuffer(reinterpret_cast<uint8_t*>(&endDir_), eocdPos, sizeof(EndDir))) {
        ABILITYBASE_LOGE("read EOCD struct failed.");
        return false;
    }

    centralDirPos_ = endDir_.offset + fileStartPos_;

    return CheckEndDir(endDir_);
}

bool ZipFile::ParseOneEntry(uint8_t* &entryPtr)
{
    if (entryPtr == nullptr) {
        ABILITYBASE_LOGE("Input entryPtr is nullptr.");
        return false;
    }

    CentralDirEntry directoryEntry;
    if (memcpy_s(&directoryEntry, sizeof(CentralDirEntry), entryPtr, sizeof(CentralDirEntry)) != EOK) {
        ABILITYBASE_LOGE("Mem copy directory entry failed.");
        return false;
    }

    if (directoryEntry.signature != CENTRAL_SIGNATURE) {
        ABILITYBASE_LOGE("parse entry, check signature failed");
        return false;
    }

    entryPtr += sizeof(CentralDirEntry);
    size_t fileLength = (directoryEntry.nameSize >= MAX_FILE_NAME) ? (MAX_FILE_NAME - 1) : directoryEntry.nameSize;
    std::string fileName(fileLength, 0);
    if (memcpy_s(&(fileName[0]), fileLength, entryPtr, fileLength) != EOK) {
        ABILITYBASE_LOGE("Mem copy file name failed.");
        return false;
    }

    ZipEntry currentEntry(directoryEntry);
    currentEntry.fileName = fileName;
    entriesMap_[fileName] = currentEntry;
    entryPtr += directoryEntry.nameSize + directoryEntry.extraSize + directoryEntry.commentSize;
    return true;
}

std::shared_ptr<DirTreeNode> ZipFile::MakeDirTree() const
{
    ABILITYBASE_LOGI("called.");
    auto root = std::make_shared<DirTreeNode>();
    for (const auto &[fileName, entry]: entriesMap_) {
        AddEntryToTree(fileName, root);
    }
    return root;
}

std::shared_ptr<DirTreeNode> ZipFile::GetDirRoot()
{
    if (!isOpen_) {
        return nullptr;
    }
    if (dirRoot_ == nullptr) {
        HITRACE_METER_NAME(HITRACE_TAG_ABILITY_MANAGER, "make_dir_tree");
        std::lock_guard guard(dirRootMutex_);
        if (dirRoot_ == nullptr) {
            dirRoot_ = MakeDirTree();
        }
    }
    return dirRoot_;
}

bool ZipFile::ParseAllEntries()
{
    auto centralData = zipFileReader_->ReadBuffer(static_cast<size_t>(centralDirPos_),
        static_cast<size_t>(endDir_.sizeOfCentralDir));
    if (centralData.empty()) {
        ABILITYBASE_LOGE("read central data for [%{public}s] failed.", pathName_.c_str());
        return false;
    }

    bool ret = true;
    uint8_t *entryPtr = reinterpret_cast<uint8_t *>(centralData.data());
    for (uint16_t i = 0; i < endDir_.totalEntries; i++) {
        if (!ParseOneEntry(entryPtr)) {
            ABILITYBASE_LOGE("Parse entry[%{public}d] failed.", i);
            ret = false;
            break;
        }
    }

    return ret;
}

bool ZipFile::Open()
{
    if (isOpen_) {
        ABILITYBASE_LOGE("has already opened");
        return true;
    }

    if (pathName_.length() > PATH_MAX) {
        ABILITYBASE_LOGE("path length(%{public}u) longer than max path length(%{public}d)",
            static_cast<unsigned int>(pathName_.length()), PATH_MAX);
        return false;
    }
    std::string realPath;
    realPath.reserve(PATH_MAX);
    realPath.resize(PATH_MAX - 1);
    if (pathName_.substr(0, Constants::PROC_PREFIX.size()) == Constants::PROC_PREFIX) {
        realPath = pathName_;
    } else {
        if (realpath(pathName_.c_str(), &(realPath[0])) == nullptr) {
            ABILITYBASE_LOGE("transform real path error: %{public}d, pathName: %{public}s", errno, pathName_.c_str());
            return false;
        }
    }

    zipFileReader_ = ZipFileReader::CreateZipFileReader(realPath);
    if (!zipFileReader_) {
        ABILITYBASE_LOGE("open file(%{public}s) failed", pathName_.c_str());
        return false;
    }

    if (fileLength_ == 0) {
        auto fileLength = zipFileReader_->GetFileLen();
        fileLength_ = static_cast<ZipPos>(fileLength);
        if (fileStartPos_ >= fileLength_) {
            ABILITYBASE_LOGE("open start pos > length failed");
            zipFileReader_.reset();
            return false;
        }

        fileLength_ -= fileStartPos_;
    }

    bool result = ParseEndDirectory();
    if (result) {
        result = ParseAllEntries();
    }
    // it means open file success.
    isOpen_ = true;
    return result;
}

void ZipFile::Close()
{
    if (!isOpen_ || zipFileReader_ == nullptr) {
        ABILITYBASE_LOGD("file is not opened");
        return;
    }

    isOpen_ = false;
    entriesMap_.clear();
    {
        std::lock_guard guard(dirRootMutex_);
        dirRoot_.reset();
    }
    pathName_ = "";

    zipFileReader_.reset();
}

// Get all file zipEntry in this file
const ZipEntryMap &ZipFile::GetAllEntries() const
{
    return entriesMap_;
}

bool ZipFile::HasEntry(const std::string &entryName) const
{
    return entriesMap_.find(entryName) != entriesMap_.end();
}

bool ZipFile::IsDirExist(const std::string &dir)
{
    if (dir.empty()) {
        ABILITYBASE_LOGE("target dir is empty");
        return false;
    }
    auto parent = GetDirRoot();
    if (parent == nullptr) {
        ABILITYBASE_LOGE("dir root is null");
        return false;
    }
    size_t cur = 0;
    do {
        while (cur < dir.size() && dir[cur] == FILE_SEPARATOR_CHAR) {
            cur++;
        }
        if (cur >= dir.size()) {
            break;
        }
        auto next = dir.find_first_of(FILE_SEPARATOR_CHAR, cur);
        auto nodeName = dir.substr(cur, next - cur);
        auto it = parent->children.find(nodeName);
        if (it == parent->children.end()) {
            ABILITYBASE_LOGD("target dir not found, dir : %{public}s", dir.c_str());
            return false;
        }
        parent = it->second;
        cur = next;
    } while (cur != std::string::npos);

    return true;
}

void ZipFile::GetAllFileList(const std::string &srcPath, std::vector<std::string> &assetList)
{
    if (srcPath.empty()) {
        ABILITYBASE_LOGE("target dir is empty");
        return;
    }
    auto parent = GetDirRoot();
    if (parent == nullptr) {
        ABILITYBASE_LOGE("dir root is null");
        return;
    }

    auto rootName = srcPath.back() == FILE_SEPARATOR_CHAR ?
        srcPath.substr(0, srcPath.length() - 1) : srcPath;

    size_t cur = 0;
    do {
        while (cur < rootName.size() && rootName[cur] == FILE_SEPARATOR_CHAR) {
            cur++;
        }
        if (cur >= rootName.size()) {
            break;
        }
        auto next = rootName.find_first_of(FILE_SEPARATOR_CHAR, cur);
        auto nodeName = rootName.substr(cur, next - cur);
        auto it = parent->children.find(nodeName);
        if (it == parent->children.end()) {
            ABILITYBASE_LOGI("target srcPath not found: %{public}s", rootName.c_str());
            return;
        }
        parent = it->second;
        cur = next;
    } while (cur != std::string::npos);

    GetTreeFileList(parent, rootName, assetList);
}

void ZipFile::GetChildNames(const std::string &srcPath, std::set<std::string> &fileSet)
{
    if (srcPath.empty()) {
        ABILITYBASE_LOGE("target dir is empty");
        return;
    }

    size_t cur = 0;
    auto parent = GetDirRoot();
    if (parent == nullptr) {
        ABILITYBASE_LOGE("dir root is null");
        return;
    }
    do {
        while (cur < srcPath.size() && srcPath[cur] == FILE_SEPARATOR_CHAR) {
            cur++;
        }
        if (cur >= srcPath.size()) {
            break;
        }
        auto next = srcPath.find_first_of(FILE_SEPARATOR_CHAR, cur);
        auto nodeName = srcPath.substr(cur, next - cur);
        auto it = parent->children.find(nodeName);
        if (it == parent->children.end()) {
            ABILITYBASE_LOGI("target srcPath not found: %{public}s", srcPath.c_str());
            return;
        }
        parent = it->second;
        cur = next;
    } while (cur != std::string::npos);

    for (const auto &child : parent->children) {
        fileSet.insert(child.first);
    }
}

bool ZipFile::GetEntry(const std::string &entryName, ZipEntry &resultEntry) const
{
    auto iter = entriesMap_.find(entryName);
    if (iter != entriesMap_.end()) {
        resultEntry = iter->second;
        return true;
    }
    return false;
}

size_t ZipFile::GetLocalHeaderSize(const uint16_t nameSize, const uint16_t extraSize) const
{
    return sizeof(LocalHeader) + nameSize + extraSize;
}

bool ZipFile::CheckDataDesc(const ZipEntry &zipEntry, const LocalHeader &localHeader) const
{
    uint32_t crcLocal = 0;
    uint32_t compressedLocal = 0;
    uint32_t uncompressedLocal = 0;

    if (localHeader.flags & FLAG_DATA_DESC) {  // use data desc
        DataDesc dataDesc;
        auto descPos = zipEntry.localHeaderOffset + GetLocalHeaderSize(localHeader.nameSize, localHeader.extraSize);
        descPos += fileStartPos_ + zipEntry.compressedSize;

        if (!zipFileReader_->ReadBuffer(reinterpret_cast<uint8_t*>(&dataDesc), descPos, sizeof(DataDesc))) {
            ABILITYBASE_LOGE("check local header read datadesc failed");
            return false;
        }

        if (dataDesc.signature != DATA_DESC_SIGNATURE) {
            ABILITYBASE_LOGE("check local header check datadesc signature failed");
            return false;
        }

        crcLocal = dataDesc.crc;
        compressedLocal = dataDesc.compressedSize;
        uncompressedLocal = dataDesc.uncompressedSize;
    } else {
        crcLocal = localHeader.crc;
        compressedLocal = localHeader.compressedSize;
        uncompressedLocal = localHeader.uncompressedSize;
    }

    if ((zipEntry.crc != crcLocal) || (zipEntry.compressedSize != compressedLocal) ||
        (zipEntry.uncompressedSize != uncompressedLocal)) {
        ABILITYBASE_LOGE("check local header compressed size corrupted");
        return false;
    }

    return true;
}

bool ZipFile::CheckCoherencyLocalHeader(const ZipEntry &zipEntry, uint16_t &extraSize) const
{
    // current only support store and Z_DEFLATED method
    if ((zipEntry.compressionMethod != Z_DEFLATED) && (zipEntry.compressionMethod != 0)) {
        ABILITYBASE_LOGE("check local header compressionMethod(%{public}d) not support", zipEntry.compressionMethod);
        return false;
    }

    auto nameSize = zipEntry.fileName.length();
    auto startPos = fileStartPos_ + zipEntry.localHeaderOffset;
    size_t buffSize = sizeof(LocalHeader) + nameSize;
    auto buff = zipFileReader_->ReadBuffer(startPos, buffSize);
    if (buff.size() < buffSize) {
        ABILITYBASE_LOGE("read header failed");
        return false;
    }

    LocalHeader localHeader = {0};
    if (memcpy_s(&localHeader, sizeof(LocalHeader), buff.data(), sizeof(LocalHeader)) != EOK) {
        ABILITYBASE_LOGE("memcpy localheader failed");
        return false;
    }
    if ((localHeader.signature != LOCAL_HEADER_SIGNATURE) ||
        (zipEntry.compressionMethod != localHeader.compressionMethod)) {
        ABILITYBASE_LOGE("check local header signature or compressionMethod failed");
        return false;
    }

    if (localHeader.nameSize != nameSize && nameSize < MAX_FILE_NAME - 1) {
        ABILITYBASE_LOGE("check local header file name corrupted");
        return false;
    }
    std::string fileName = buff.substr(sizeof(LocalHeader));
    if (zipEntry.fileName != fileName) {
        ABILITYBASE_LOGE("check local header file name corrupted");
        return false;
    }

    if (!CheckDataDesc(zipEntry, localHeader)) {
        ABILITYBASE_LOGE("check data desc failed");
        return false;
    }

    extraSize = localHeader.extraSize;
    return true;
}

size_t ZipFile::GetEntryStart(const ZipEntry &zipEntry, const uint16_t extraSize) const
{
    ZipPos startOffset = zipEntry.localHeaderOffset;
    // get data offset, add signature+localheader+namesize+extrasize
    startOffset += GetLocalHeaderSize(zipEntry.fileName.length(), extraSize);
    startOffset += fileStartPos_;  // add file start relative to file stream

    return startOffset;
}

bool ZipFile::UnzipWithStore(const ZipEntry &zipEntry, const uint16_t extraSize, std::ostream &dest) const
{
    auto startPos = GetEntryStart(zipEntry, extraSize);
    uint32_t remainSize = zipEntry.compressedSize;
    while (remainSize > 0) {
        size_t readLen = (remainSize > UNZIP_BUF_OUT_LEN) ? UNZIP_BUF_OUT_LEN : remainSize;
        std::string readBuffer = zipFileReader_->ReadBuffer(startPos, readLen);
        if (readBuffer.empty()) {
            ABILITYBASE_LOGE("unzip store read failed, error");
            return false;
        }
        remainSize -= readLen;
        startPos += readLen;
        dest.write(readBuffer.data(), readBuffer.length());
    }

    return true;
}

bool ZipFile::InitZStream(z_stream &zstream) const
{
    // init zlib stream
    if (memset_s(&zstream, sizeof(z_stream), 0, sizeof(z_stream))) {
        ABILITYBASE_LOGE("unzip stream buffer init failed");
        return false;
    }
    int32_t zlibErr = inflateInit2(&zstream, -MAX_WBITS);
    if (zlibErr != Z_OK) {
        ABILITYBASE_LOGE("unzip inflated init failed");
        return false;
    }

    BytePtr bufOut = new (std::nothrow) Byte[UNZIP_BUF_OUT_LEN];
    if (bufOut == nullptr) {
        ABILITYBASE_LOGE("unzip inflated new out buffer failed");
        return false;
    }

    BytePtr bufIn = new (std::nothrow) Byte[UNZIP_BUF_IN_LEN];
    if (bufIn == nullptr) {
        ABILITYBASE_LOGE("unzip inflated new in buffer failed");
        delete[] bufOut;
        return false;
    }
    zstream.next_out = bufOut;
    zstream.next_in = bufIn;
    zstream.avail_out = UNZIP_BUF_OUT_LEN;
    return true;
}

bool ZipFile::ReadZStream(const BytePtr &buffer, z_stream &zstream, uint32_t &remainCompressedSize,
    size_t &startPos) const
{
    if (zstream.avail_in == 0) {
        size_t remainBytes = (remainCompressedSize > UNZIP_BUF_IN_LEN) ? UNZIP_BUF_IN_LEN : remainCompressedSize;
        if (!zipFileReader_->ReadBuffer(buffer, startPos, remainBytes)) {
            ABILITYBASE_LOGE("unzip inflated read failed, error");
            return false;
        }

        remainCompressedSize -= remainBytes;
        startPos += remainBytes;
        zstream.avail_in = remainBytes;
        zstream.next_in = buffer;
    }
    return true;
}

bool ZipFile::UnzipWithInflated(const ZipEntry &zipEntry, const uint16_t extraSize, std::ostream &dest) const
{
    z_stream zstream;
    if (!InitZStream(zstream)) {
        return false;
    }

    auto startPos = GetEntryStart(zipEntry, extraSize);

    BytePtr bufIn = zstream.next_in;
    BytePtr bufOut = zstream.next_out;

    bool ret = true;
    int32_t zlibErr = Z_OK;
    uint32_t remainCompressedSize = zipEntry.compressedSize;
    size_t inflateLen = 0;
    uint8_t errorTimes = 0;
    while ((remainCompressedSize > 0) || (zstream.avail_in > 0)) {
        if (!ReadZStream(bufIn, zstream, remainCompressedSize, startPos)) {
            ret = false;
            break;
        }

        zlibErr = inflate(&zstream, Z_SYNC_FLUSH);
        if ((zlibErr >= Z_OK) && (zstream.msg != nullptr)) {
            ABILITYBASE_LOGE("unzip inflated inflate, fail: %{public}d, err msg: %{public}s", zlibErr, zstream.msg);
            ret = false;
            break;
        }

        inflateLen = UNZIP_BUF_OUT_LEN - zstream.avail_out;
        if (inflateLen > 0) {
            dest.write((const char *)bufOut, inflateLen);
            zstream.next_out = bufOut;
            zstream.avail_out = UNZIP_BUF_OUT_LEN;
            errorTimes = 0;
        } else {
            errorTimes++;
        }
        if (errorTimes >= INFLATE_ERROR_TIMES) {
            ABILITYBASE_LOGE("unzip inflated data is wrong!");
            ret = false;
            break;
        }
    }

    // free all dynamically allocated data structures except the next_in and next_out for this stream.
    zlibErr = inflateEnd(&zstream);
    if (zlibErr != Z_OK) {
        ABILITYBASE_LOGE("unzip inflateEnd error, error: %{public}d", zlibErr);
        ret = false;
    }

    delete[] bufOut;
    delete[] bufIn;
    return ret;
}

ZipPos ZipFile::GetEntryDataOffset(const ZipEntry &zipEntry, const uint16_t extraSize) const
{
    // get entry data offset relative file
    ZipPos offset = zipEntry.localHeaderOffset;

    offset += GetLocalHeaderSize(zipEntry.fileName.length(), extraSize);
    offset += fileStartPos_;

    return offset;
}

bool ZipFile::GetDataOffsetRelative(const std::string &file, ZipPos &offset, uint32_t &length) const
{
    ZipEntry zipEntry;
    if (!GetEntry(file, zipEntry)) {
        ABILITYBASE_LOGE("extract file: not find file");
        return false;
    }

    return GetDataOffsetRelative(zipEntry, offset, length);
}

bool ZipFile::GetDataOffsetRelative(const ZipEntry &zipEntry, ZipPos &offset, uint32_t &length) const
{
    uint16_t extraSize = 0;
    if (!CheckCoherencyLocalHeader(zipEntry, extraSize)) {
        ABILITYBASE_LOGE("check coherency local header failed");
        return false;
    }

    offset = GetEntryDataOffset(zipEntry, extraSize);
    length = zipEntry.compressedSize;
    return true;
}

bool ZipFile::ExtractFile(const std::string &file, std::ostream &dest) const
{
    ZipEntry zipEntry;
    if (!GetEntry(file, zipEntry)) {
        ABILITYBASE_LOGE("extract file: not find file");
        return false;
    }

    uint16_t extraSize = 0;
    if (!CheckCoherencyLocalHeader(zipEntry, extraSize)) {
        ABILITYBASE_LOGE("check coherency local header failed");
        return false;
    }

    bool ret = true;
    if (zipEntry.compressionMethod == 0) {
        ret = UnzipWithStore(zipEntry, extraSize, dest);
    } else {
        ret = UnzipWithInflated(zipEntry, extraSize, dest);
    }

    return ret;
    ABILITYBASE_LOGD("ExtractFile end");
}

bool ZipFile::ExtractFileFromMMap(const std::string &file, void *mmapDataPtr,
    std::unique_ptr<uint8_t[]> &dataPtr, size_t &len) const
{
    ABILITYBASE_LOGI("ExtractFileFromMMap %{public}s.", file.c_str());
    ZipEntry zipEntry;
    if (!GetEntry(file, zipEntry)) {
        ABILITYBASE_LOGE("extract file: not find file");
        return false;
    }

    if (!zipEntry.compressionMethod) {
        ABILITYBASE_LOGE("file[%{public}s] is not extracted.", file.c_str());
        return false;
    }

    uint16_t extraSize = 0;
    if (!CheckCoherencyLocalHeader(zipEntry, extraSize)) {
        ABILITYBASE_LOGE("check coherency local header failed");
        return false;
    }

    bool ret = false;
    ret = UnzipWithInflatedFromMMap(zipEntry, extraSize, mmapDataPtr, dataPtr, len);

    return ret;
}

bool ZipFile::UnzipWithInflatedFromMMap(const ZipEntry &zipEntry, const uint16_t extraSize,
    void *mmapDataPtr, std::unique_ptr<uint8_t[]> &dataPtr, size_t &len) const
{
    z_stream zstream;
    if (!InitZStream(zstream)) {
        ABILITYBASE_LOGE("Init zstream failed.");
        return false;
    }

    BytePtr bufIn = zstream.next_in;
    BytePtr bufOut = zstream.next_out;

    bool ret = true;
    int32_t zlibErr = Z_OK;
    uint32_t remainCompressedSize = zipEntry.compressedSize;
    size_t inflateLen = 0;
    uint8_t errorTimes = 0;

    len = zipEntry.uncompressedSize;
    dataPtr = std::make_unique<uint8_t[]>(len);
    uint8_t *dstDataPtr = static_cast<uint8_t *>(dataPtr.get());
    void *mmapSrcDataPtr = mmapDataPtr;

    while ((remainCompressedSize > 0) || (zstream.avail_in > 0)) {
        if (!ReadZStreamFromMMap(bufIn, mmapSrcDataPtr, zstream, remainCompressedSize)) {
            ret = false;
            break;
        }

        zlibErr = inflate(&zstream, Z_SYNC_FLUSH);
        if ((zlibErr >= Z_OK) && (zstream.msg != nullptr)) {
            ABILITYBASE_LOGE("unzip inflated inflate, error: %{public}d, err msg: %{public}s.", zlibErr, zstream.msg);
            ret = false;
            break;
        }

        inflateLen = UNZIP_BUF_OUT_LEN - zstream.avail_out;
        if (!CopyInflateOut(zstream, inflateLen, &dstDataPtr, bufOut, errorTimes)) {
            break;
        }
    }

    // free all dynamically allocated data structures except the next_in and next_out for this stream.
    zlibErr = inflateEnd(&zstream);
    if (zlibErr != Z_OK) {
        ABILITYBASE_LOGE("unzip inflateEnd error, error: %{public}d", zlibErr);
        ret = false;
    }

    delete[] bufOut;
    delete[] bufIn;
    return ret;
}

bool ZipFile::CopyInflateOut(z_stream &zstream, size_t inflateLen, uint8_t** dstDataPtr,
    BytePtr bufOut, uint8_t &errorTimes) const
{
    if (inflateLen > 0) {
        if (memcpy_s(*dstDataPtr, inflateLen, bufOut, inflateLen) != EOK) {
            ABILITYBASE_LOGE("Mem copy failed!");
            return false;
        }

        *dstDataPtr += inflateLen;
        zstream.next_out = bufOut;
        zstream.avail_out = UNZIP_BUF_OUT_LEN;
        errorTimes = 0;
    } else {
        errorTimes++;
    }
    if (errorTimes >= INFLATE_ERROR_TIMES) {
        ABILITYBASE_LOGE("unzip inflated data is abnormal!");
        return false;
    }

    return true;
}

bool ZipFile::ReadZStreamFromMMap(const BytePtr &buffer, void* &dataPtr,
    z_stream &zstream, uint32_t &remainCompressedSize) const
{
    if (!dataPtr) {
        ABILITYBASE_LOGE("Input dataPtr is nullptr.");
        return false;
    }

    uint8_t *srcDataPtr = static_cast<uint8_t *>(dataPtr);
    if (zstream.avail_in == 0) {
        size_t remainBytes = (remainCompressedSize > UNZIP_BUF_IN_LEN) ? UNZIP_BUF_IN_LEN : remainCompressedSize;
        size_t readBytes = sizeof(Byte) * remainBytes;
        if (memcpy_s(buffer, readBytes, srcDataPtr, readBytes) != EOK) {
            ABILITYBASE_LOGE("Mem copy failed.");
            return false;
        }
        srcDataPtr += readBytes;
        remainCompressedSize -= remainBytes;
        zstream.avail_in = remainBytes;
        zstream.next_in = buffer;
    }
    dataPtr = srcDataPtr;
    return true;
}

std::unique_ptr<FileMapper> ZipFile::CreateFileMapper(const std::string &fileName, FileMapperType type) const
{
    ZipEntry zipEntry;
    if (!GetEntry(fileName, zipEntry)) {
        ABILITYBASE_LOGE("GetEntry failed hapPath %{public}s.", fileName.c_str());
        return nullptr;
    }

    ZipPos offset = 0;
    uint32_t length = 0;
    if (!GetDataOffsetRelative(zipEntry, offset, length)) {
        ABILITYBASE_LOGE("GetDataOffsetRelative failed hapPath %{public}s.", fileName.c_str());
        return nullptr;
    }
    bool compress = zipEntry.compressionMethod > 0;
    if (type == FileMapperType::SAFE_ABC && compress) {
        ABILITYBASE_LOGW("Entry is compressed for safe: %{public}s.", fileName.c_str());
    }
    std::unique_ptr<FileMapper> fileMapper = std::make_unique<FileMapper>();
    auto result = false;
    if (type == FileMapperType::NORMAL_MEM) {
        result = fileMapper->CreateFileMapper(zipFileReader_, fileName, offset, length, compress);
    } else {
        result = fileMapper->CreateFileMapper(fileName, compress, zipFileReader_->GetFd(), offset, length, type);
        if (result && type == FileMapperType::SAFE_ABC) {
            zipFileReader_->SetClosable(false);
        }
    }

    if (!result) {
        return nullptr;
    }
    return fileMapper;
}

bool ZipFile::ExtractToBufByName(const std::string &fileName, std::unique_ptr<uint8_t[]> &dataPtr,
    size_t &len) const
{
    ZipEntry zipEntry;
    if (!GetEntry(fileName, zipEntry)) {
        if (fileName.length() > MAP_FILE_SUFFIX && fileName.substr(fileName.length() - MAP_FILE_SUFFIX) != ".map") {
            ABILITYBASE_LOGE("GetEntry failed hapPath %{public}s.", fileName.c_str());
        }
        return false;
    }
    uint16_t extraSize = 0;
    if (!CheckCoherencyLocalHeader(zipEntry, extraSize)) {
        ABILITYBASE_LOGE("check coherency local header failed");
        return false;
    }

    ZipPos offset = GetEntryDataOffset(zipEntry, extraSize);
    uint32_t length = zipEntry.compressedSize;
    auto dataTmp = std::make_unique<uint8_t[]>(length);
    if (!zipFileReader_->ReadBuffer(dataTmp.get(), offset, length)) {
        ABILITYBASE_LOGE("read file failed, len[%{public}zu]. fileName: %{public}s, offset: %{public}zu",
            len, fileName.c_str(), (size_t)offset);
        dataTmp.reset();
        return false;
    }

    if (zipEntry.compressionMethod > 0) {
        return UnzipWithInflatedFromMMap(zipEntry, extraSize, dataTmp.get(), dataPtr, len);
    }

    len = length;
    dataPtr = std::move(dataTmp);

    return true;
}
}  // namespace AbilityBase
}  // namespace OHOS
