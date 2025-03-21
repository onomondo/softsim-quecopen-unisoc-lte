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
 * @file iot_gpio.h
 *
 * @brief Declares functions for operating GPIO devices.
 *
 * These functions are used for GPIO initialization, input/output settings, and level settings. \n
 *
 * @since 2.2
 * @version 2.2
 */
#ifndef IOT_GPIO_H
#define IOT_GPIO_H

/**
 * @brief Enumerates GPIO level values.
 */
typedef enum {
    /** Low GPIO level */
    IOT_GPIO_VALUE0 = 0,
    /** High GPIO level */
    IOT_GPIO_VALUE1
} IotGpioValue;

/**
 * @brief Enumerates GPIO directions.
 */
typedef enum {
    /** Input */
    IOT_GPIO_DIR_IN = 0,
    /** Output */
    IOT_GPIO_DIR_OUT
} IotGpioDir;

/**
 * @brief Enumerates GPIO interrupt trigger modes.
 */
typedef enum {
    /** Level-sensitive interrupt */
    IOT_INT_TYPE_LEVEL = 0,
    /** Edge-sensitive interrupt */
    IOT_INT_TYPE_EDGE
} IotGpioIntType;

/**
 * @brief Enumerates I/O interrupt polarities.
 */
typedef enum {
    /** Interrupt at a low level or falling edge */
    IOT_GPIO_EDGE_FALL_LEVEL_LOW = 0,
    /** Interrupt at a high level or rising edge */
    IOT_GPIO_EDGE_RISE_LEVEL_HIGH
} IotGpioIntPolarity;

/**
 * @brief Indicates the GPIO interrupt callback.
 *
 */
typedef void (*GpioIsrCallbackFunc) (char *arg);

/**
 * @brief Initializes a GPIO device.
 *
 * @param id Indicates the GPIO pin number.
 * @return Returns {@link IOT_SUCCESS} if the GPIO device is initialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioInit(unsigned int id);

/**
 * @brief Deinitializes a GPIO device.
 *
 * @param id Indicates the GPIO pin number.
 * @return Returns {@link IOT_SUCCESS} if the GPIO device is deinitialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioDeinit(unsigned int id);

/**
 * @brief Sets the direction for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param dir Indicates the GPIO input/output direction.
 * @return Returns {@link IOT_SUCCESS} if the direction is set;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir);

/**
 * @brief Obtains the direction for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param dir Indicates the pointer to the GPIO input/output direction.
 * @return Returns {@link IOT_SUCCESS} if the direction is obtained;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir);

/**
 * @brief Sets the output level value for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param val Indicates the output level value.
 * @return Returns {@link IOT_SUCCESS} if the output level value is set;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val);

/**
 * @brief Obtains the output level value of a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param val Indicates the pointer to the output level value.
 * @return Returns {@link IOT_SUCCESS} if the output level value is obtained;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val);

/**
 * @brief Obtains the input level value of a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param val Indicates the pointer to the input level value.
 * @return Returns {@link IOT_SUCCESS} if the input level value is obtained;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val);

/**
 * @brief Enables the interrupt feature for a GPIO pin.
 *
 * This function can be used to set the interrupt type, interrupt polarity, and interrupt callback for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param intType Indicates the interrupt type.
 * @param intPolarity Indicates the interrupt polarity.
 * @param func Indicates the interrupt callback function.
 * @param arg Indicates the pointer to the argument used in the interrupt callback function.
 * @return Returns {@link IOT_SUCCESS} if the interrupt feature is enabled;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity,
                                    GpioIsrCallbackFunc func, char *arg);

/**
 * @brief Disables the interrupt feature for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @return Returns {@link IOT_SUCCESS} if the interrupt feature is disabled;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioUnregisterIsrFunc(unsigned int id);

/**
 * @brief Masks the interrupt feature for a GPIO pin.
 *
 * @param id Indicates the GPIO pin number.
 * @param mask Indicates whether the interrupt function is masked.
 * The value <b>1</b> means to mask the interrupt function, and <b>0</b> means not to mask the interrupt function.
 * @return Returns {@link IOT_SUCCESS} if the interrupt feature is masked;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask);

/**
 * @brief Sets the interrupt trigger mode of a GPIO pin.
 *
 * This function configures a GPIO pin based on the interrupt type and interrupt polarity.
 *
 * @param id Indicates the GPIO pin number.
 * @param intType Indicates the interrupt type.
 * @param intPolarity Indicates the interrupt polarity.
 * @return Returns {@link IOT_SUCCESS} if the interrupt trigger mode is set;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTGpioSetIsrMode(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity);

#endif // IOT_GPIO_H
/** @} */
