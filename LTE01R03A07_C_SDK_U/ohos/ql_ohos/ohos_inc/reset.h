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
 * @version 2.2
 */

/**
 * @file reset.h
 *
 * @brief Reboots the device.
 *
 *
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef RESET_H
#define RESET_H

/**
 * @brief Reboots the device using different causes.
 *
 *
 *
 * @param cause Indicates the reboot cause.
 * @since 2.2
 * @version 2.2
 */
void RebootDevice(unsigned int cause);

#endif // RESET_H
/** @} */
