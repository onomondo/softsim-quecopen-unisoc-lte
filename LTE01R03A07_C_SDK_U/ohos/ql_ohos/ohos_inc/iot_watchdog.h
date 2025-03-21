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
 * @addtogroup IotHardware
 * @{
 *
 * @brief Provides APIs for operating devices,
 * including flash, GPIO, I2C, PWM, UART, and watchdog APIs.
 *
 *
 *
 * @since 2.2
 * @version 2.2
 */

/**
 * @file iot_watchdog.h
 *
 * @brief Declares functions for operating watchdogs.
 *
 * These functions are used to enable, disable, and feed a watchdog. \n
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef IOT_WATCHDOG_H
#define IOT_WATCHDOG_H

/**
 * @brief Enables a watchdog.
 *
 * @since 2.2
 * @version 2.2
 */
void IoTWatchDogEnable(void);

/**
 * @brief Feeds a watchdog.
 *
 * @since 2.2
 * @version 2.2
 */
void IoTWatchDogKick(void);

/**
 * @brief Disables a watchdog.
 *
 * @since 2.2
 * @version 2.2
 */
void IoTWatchDogDisable(void);

#endif // IOT_WATCHDOG_H
/** @} */
