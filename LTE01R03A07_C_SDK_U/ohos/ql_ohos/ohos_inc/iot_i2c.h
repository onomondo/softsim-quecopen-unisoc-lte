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
 * @file iot_i2c.h
 *
 * @brief Declares functions for operating I2C devices.
 *
 * These functions are used to initialize or deinitialize an I2C device,
 * and read data from or write data to an I2C device. \n
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef IOT_I2C_H
#define IOT_I2C_H

/**
 * @brief Initializes an I2C device with a specified baud rate.
 *
 *
 *
 * @param id Indicates the I2C device ID.
 * @param baudrate Indicates the I2C baud rate.
 * @return Returns {@link IOT_SUCCESS} if the I2C device is initialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate);

/**
 * @brief Deinitializes an I2C device.
 *
 * @param id Indicates the I2C device ID.
 * @return Returns {@link IOT_SUCCESS} if the I2C device is deinitialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTI2cDeinit(unsigned int id);

/**
 * @brief Writes data to an I2C device.
 *
 *
 *
 * @param id Indicates the I2C device ID.
 * @param deviceAddr Indicates the I2C device address.
 * @param data Indicates the pointer to the data to write.
 * @param dataLen Indicates the length of the data to write.
 * @return Returns {@link IOT_SUCCESS} if the data is written to the I2C device successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen);

/**
 * @brief Reads data from an I2C device.
 *
 * The data read will be saved to the address specified by <b>i2cData</b>.
 *
 * @param id Indicates the I2C device ID.
 * @param deviceAddr Indicates the I2C device address.
 * @param data Indicates the pointer to the data to read.
 * @param dataLen Indicates the length of the data to read.
 * @return Returns {@link IOT_SUCCESS} if the data is read from the I2C device successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen);

/**
 * @brief Sets the baud rate for an I2C device.
 *
 * @param id Indicates the I2C device ID.
 * @param baudrate Indicates the baud rate to set.
 * @return Returns {@link IOT_SUCCESS} if the baud rate is set;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate);

#endif // IOT_I2C_H
/** @} */
