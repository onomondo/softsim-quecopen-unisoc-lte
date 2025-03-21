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
 * @file iot_flash.h
 *
 * @brief Declares flash functions.
 *
 * These functions are used to initialize or deinitialize a flash device,
 * and read data from or write data to a flash memory. \n
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef IOT_FLASH_H
#define IOT_FLASH_H

/**
 * @brief Reads data from a flash memory address.
 *
 * This function reads a specified length of data from a specified flash memory address.
 *
 * @param flashOffset Indicates the address of the flash memory from which data is to read.
 * @param size Indicates the length of the data to read.
 * @param ramData Indicates the pointer to the RAM for storing the read data.
 * @return Returns {@link IOT_SUCCESS} if the data is read successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData);

/**
 * @brief Writes data to a flash memory address.
 *
 * This function writes a specified length of data to a specified flash memory address.
 *
 * @param flashOffset Indicates the address of the flash memory to which data is to be written.
 * @param size Indicates the length of the data to write.
 * @param ramData Indicates the pointer to the RAM for storing the data to write.
 * @param doErase Specifies whether to automatically erase existing data.
 * @return Returns {@link IOT_SUCCESS} if the data is written successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase);

/**
 * @brief Erases data in a specified flash memory address.
 *
 * @param flashOffset Indicates the flash memory address.
 * @param size Indicates the data length in bytes.
 * @return Returns {@link IOT_SUCCESS} if the data is erased successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size);

/**
 * @brief Initializes a flash device.
 *
 * @return Returns {@link IOT_SUCCESS} if the flash device is initialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTFlashInit(void);

/**
 * @brief Deinitializes a flash device.
 *
 * @return Returns {@link IOT_SUCCESS} if the flash device is deinitialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTFlashDeinit(void);

#endif  // IOT_FLASH_H
/** @} */
