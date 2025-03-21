/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

/**
 * @addtogroup power
 * @{
 *
 * @brief Provides device power management functions.
 *
 * This module is used to reboot the device and set low power consumption for the device. \n
 *
 * @since 2.2
 * @version 2.2 */

/**
 * @file lowpower.h
 *
 * @brief Sets low power consumption for the device.
 *
 *
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef LOWPOWER_H
#define LOWPOWER_H

/**
 * @brief Enumerates low power consumption modes.
 */
typedef enum {
    /** No sleep */
    NO_SLEEP,
    /** Light sleep */
    LIGHT_SLEEP,
    /** Deep sleep */
    DEEP_SLEEP,
}LpcType;

/**
 * @brief Initializes low power consumption.
 *
 * @return Returns {@link IOT_SUCCESS} if low power consumption is initialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int LpcInit(void);

/**
 * @brief Sets low power consumption for the device.
 *
 * @param type Indicates the low power consumption mode, as enumerated in {@link LpcType}.
 * @return Returns {@link IOT_SUCCESS} if low power consumption is set;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int LpcSetType(LpcType type);

#endif // LOWPOWER_H
/** @} */
