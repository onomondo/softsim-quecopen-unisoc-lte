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
 * @file iot_uart.h
 *
 * @brief Declares UART functions.
 *
 * These functions are used for UART initialization,
 * data input/output, and data flow control. \n
 *
 * @since 2.2
 * @version 2.2
 */

#ifndef IOT_UART_H
#define IOT_UART_H

/**
 * @brief Enumerates the number of UART data bits.
 *
 * @since 2.2
 * @version 2.2
 */
typedef enum {
    /** 5 data bits */
    IOT_UART_DATA_BIT_5 = 5,
    /** 6 data bits */
    IOT_UART_DATA_BIT_6,
    /** 7 data bits */
    IOT_UART_DATA_BIT_7,
    /** 8 data bits */
    IOT_UART_DATA_BIT_8,
} IotUartIdxDataBit;

/**
 * @brief Enumerates the number of UART stop bits.
 *
 * @since 2.2
 * @version 2.2
 */
typedef enum {
    /** 1 stop bit */
    IOT_UART_STOP_BIT_1 = 1,
    /** 2 stop bits */
    IOT_UART_STOP_BIT_2 = 2,
} IotUartStopBit;

/**
 * @brief Enumerates UART parity bits.
 *
 * @since 2.2
 * @version 2.2
 */
typedef enum {
    /** No parity */
    IOT_UART_PARITY_NONE = 0,
    /** Odd parity */
    IOT_UART_PARITY_ODD = 1,
    /** Even parity */
    IOT_UART_PARITY_EVEN = 2,
} IotUartParity;

/**
 * @brief Enumerates UART block states.
 *
 * @since 2.2
 * @version 2.2
 */
typedef enum {
    /** Block disabled */
    IOT_UART_BLOCK_STATE_NONE_BLOCK = 0,
    /** Block enabled */
    IOT_UART_BLOCK_STATE_BLOCK,
} IotUartBlockState;

/**
 * @brief Enumerates hardware flow control modes.
 *
 * @since 2.2
 * @version 2.2
 */
typedef enum {
    /** Hardware flow control disabled */
    IOT_FLOW_CTRL_NONE,
    /** RTS and CTS hardware flow control enabled */
    IOT_FLOW_CTRL_RTS_CTS,
    /** RTS hardware flow control enabled */
    IOT_FLOW_CTRL_RTS_ONLY,
     /** CTS hardware flow control enabled */
    IOT_FLOW_CTRL_CTS_ONLY,
} IotFlowCtrl;

/**
 * @brief Defines basic attributes of a UART port.
 *
 * @since 2.2
 * @version 2.2
 */
typedef struct {
    /** Baud rate */
    unsigned int baudRate;
    /** Data bits */
    IotUartIdxDataBit dataBits;
    /** Stop bit */
    IotUartStopBit stopBits;
    /** Parity */
    IotUartParity parity;
    /** Rx block state */
    IotUartBlockState rxBlock;
    /** Tx block state */
    IotUartBlockState txBlock;
    /** Padding bit */
    unsigned char pad;
} IotUartAttribute;

/**
 * @brief Configures a UART device with the port number specified by <b>id</b>
 * based on the basic and extended attributes.
 *
 *
 *
 * @param id Indicates the port number of the UART device.
 * @param param Indicates the pointer to the UART attributes.
 * @return Returns {@link IOT_SUCCESS} if the UART device is configured successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param);

/**
 * @brief Reads a specified length of data from a UART device with the port number specified by <b>id</b>.
 *
 *
 *
 * @param id Indicates the port number of the UART device.
 * @param data Indicates the pointer to the start address of the data to read.
 * @param dataLen Indicates the number of bytes to read.
 * @return Returns the number of bytes read if the operation is successful; returns <b>-1</b> otherwise.
 * @since 2.2
 * @version 2.2
 */
int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen);

/**
 * @brief Writes a specified length of data to a UART device with the port number specified by <b>id</b>.
 *
 *
 *
 * @param id Indicates the port number of the UART device.
 * @param data Indicates the pointer to the start address of the data to write.
 * @param dataLen Indicates the number of bytes to write.
 * @return Returns the number of bytes written if the operation is successful; returns <b>-1</b> otherwise.
 * @since 2.2
 * @version 2.2
 */
int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen);

/**
 * @brief Deinitializes a UART device.
 *
 * @param id Indicates the port number of the UART device.
 * @return Returns {@link IOT_SUCCESS} if the UART device is deinitialized;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTUartDeinit(unsigned int id);

/**
 * @brief Sets flow control for a UART device with the port number specified by <b>id</b>.
 *
 *
 *
 * @param id Indicates the port number of the UART device.
 * @param flowCtrl Indicates the flow control parameters, as enumerated in {@link IotFlowCtrl}.
 * @return Returns {@link IOT_SUCCESS} if flow control is set successfully;
 * returns {@link IOT_FAILURE} otherwise. For details about other return values, see the chip description.
 * @since 2.2
 * @version 2.2
 */
unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl);

#endif // IOT_UART_H
/** @} */
