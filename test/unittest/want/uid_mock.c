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

#include <dlfcn.h>
#include <unistd.h>
#include "uid_mock.h"

static int g_mockUid = -1;

void SetMockUid(int uid)
{
    g_mockUid = uid;
}

typedef uid_t (*GetUidFunc)(void);

static GetUidFunc GetRealGetUid(void)
{
    static GetUidFunc realGetUid = NULL;
    if (realGetUid == NULL) {
        realGetUid = (GetUidFunc)dlsym(RTLD_NEXT, "getuid");
    }
    return realGetUid;
}

uid_t MockGetUid(void) __asm__("getuid");

uid_t MockGetUid(void)
{
    if (g_mockUid >= 0) {
        return (uid_t)g_mockUid;
    }
    GetUidFunc realGetUid = GetRealGetUid();
    if (realGetUid != NULL) {
        return realGetUid();
    }
    return 0;
}
