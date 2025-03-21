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
 * @file iot_pwm.h
 *
 * @brief Declares functions for operating PWM devices,
 * including initializing and deinitializing a PWM device and starting and stopping PWM signal output.
 *
 *
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef IOT_PWM_H
#define IOT_PWM_H

/**
 * @brief Initializes a PWM device.
 *
 * @param port Indicates the port number of the PWM device.
 * @return Returns {@link IOT_SUCCESS} if the PWM device is initialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTPwmInit(unsigned int port);

/**
 * @brief Deinitializes a PWM device.
 *
 * @param port Indicates the port number of the PWM device.
 * @return Returns {@link IOT_SUCCESS} if the PWM device is deinitialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTPwmDeinit(unsigned int port);

/**
 * @brief Starts PWM signal output from a specified port based on the given output frequency and duty cycle.
 *
 *
 *
 * @param port Indicates the port number of the PWM device.
 * @param duty Indicates the duty cycle for PWM signal output. The value ranges from 1 to 99.
 * @param freq Indicates the frequency for PWM signal output.
 * @return Returns {@link IOT_SUCCESS} if the PWM signal output is started;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq);

/**
 * @brief Stops PWM signal output from a specified port.
 *
 * @param port Indicates the port number of the PWM device.
 * @return Returns {@link IOT_SUCCESS} if the PWM signal output is stopped;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTPwmStop(unsigned int port);

#endif // IOT_PWM_H
/** @} */
