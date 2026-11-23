/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "sessioninfo_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "ipc_object_stub.h"
#include "message_parcel.h"
#include "session_info.h"

using namespace OHOS::AAFwk;

namespace OHOS {
namespace {
constexpr size_t BITS_PER_BYTE = 8;
constexpr size_t MIN_FUZZ_SIZE = 8;
// Seed is placed in the high 32 bits to compose a 64-bit uiExtensionComponentId.
constexpr size_t COMPONENT_ID_HIGH_SHIFT = 32;
// Mix the low half of the seed into itself to derive the migration gen value.
} // namespace

uint32_t GetU32Data(const char* ptr)
{
    // Compose a 32-bit seed from the first four input bytes (big-endian); the uint8_t
    // narrowing first avoids sign-extending chars above 0x7F.
    uint32_t seed = 0;
    for (size_t i = 0; i < sizeof(seed); i++) {
        seed = (seed << BITS_PER_BYTE) | static_cast<uint8_t>(ptr[i]);
    }
    return seed;
}

bool DoSomethingInterestingWithMyAPI(const char *data, size_t size)
{
    if (data == nullptr || size < MIN_FUZZ_SIZE) {
        return false;
    }
    uint32_t seed = GetU32Data(data);

    // Fuzz deserialization entry: arbitrary MessageParcel data flows into
    // Unmarshalling, covering the appended ReadParcelThree tail reads (token presence
    // bool, remote object slot, migration gen and caller persistent id).
    // BASE-02 fix: use MessageParcel (not plain Parcel) so the remote-object read
    // path (static_cast<MessageParcel*> in ReadParcelThree) is exercised.
    MessageParcel inParcel;
    inParcel.WriteBuffer(data, size);
    SessionInfo *decoded = SessionInfo::Unmarshalling(inParcel);
    if (decoded != nullptr) {
        MessageParcel dumpParcel;
        decoded->Marshalling(dumpParcel);
        delete decoded;
    }

    // Fuzz serialization entry: fill the migration fields with fuzz data.
    // BASE-02 fix: alternate between null and non-null callerToken so the
    // WriteRemoteObject branch in DoMarshallingEight is also covered.
    SessionInfo src;
    src.persistentId = static_cast<int32_t>(seed);
    src.uiExtensionComponentId = static_cast<uint64_t>(seed) << COMPONENT_ID_HIGH_SHIFT | seed;
    if (seed & 1) {
        // Odd seed: non-null token — exercises the WriteRemoteObject path.
        src.originalCallerInfo.callerToken = new IPCObjectStub(u"session.info.fuzz.token");
    }
    src.originalCallerInfo.persistentId = static_cast<int32_t>(seed >> 1);
    src.callerPersistentId = static_cast<int32_t>(~seed);
    MessageParcel outParcel;
    if (src.Marshalling(outParcel)) {
        // Round trip: a well-formed parcel must walk the ReadParcelThree success path.
        SessionInfo *roundTrip = SessionInfo::Unmarshalling(outParcel);
        if (roundTrip != nullptr) {
            delete roundTrip;
        }
    }
    return true;
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(reinterpret_cast<const char*>(data), size);
    return 0;
}
