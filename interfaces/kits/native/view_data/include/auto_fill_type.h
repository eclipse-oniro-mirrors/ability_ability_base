/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_ABILITY_BASE_AUTO_FILL_TYPE_H
#define OHOS_ABILITY_BASE_AUTO_FILL_TYPE_H

namespace OHOS {
namespace AbilityBase {
enum class AutoFillType {
    UNSPECIFIED = 0,
    PASSWORD,
    USER_NAME,
    NEW_PASSWORD,
    FULL_STREET_ADDRESS,
    HOUSE_NUMBER,
    DISTRICT_ADDRESS,
    CITY_ADDRESS,
    PROVINCE_ADDRESS,
    COUNTRY_ADDRESS,
    PERSON_FULL_NAME,
    PERSON_LAST_NAME,
    PERSON_FIRST_NAME,
    PHONE_NUMBER,
    PHONE_COUNTRY_CODE,
    FULL_PHONE_NUMBER,
    EMAIL_ADDRESS,
    BANK_CARD_NUMBER,
    ID_CARD_NUMBER,
    PRECISE_TIME,
    HOUR_AND_MINUTE,
    DATE,
    MONTH,
    YEAR,
    NICKNAME,
    DETAIL_INFO_WITHOUT_STREET,
    FORMAT_ADDRESS,
};

enum class PopupPlacement {
    LEFT = 0,
    RIGHT,
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    LEFT_TOP,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_BOTTOM,
    NONE,
};
}  // namespace AbilityBase
}  // namespace OHOS
#endif // OHOS_ABILITY_BASE_AUTO_FILL_TYPE_H