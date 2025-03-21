/**
  @file
  ql_wifi_uart.h

  @brief
  Quectel wifi uart api header.

*/
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/
#ifndef QL_WIFI_UART_H
#define QL_WIFI_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/

#define QL_WIFI_UART_MAGIC0 0xAA
#define QL_WIFI_UART_MAGIC1 0x55
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_UART_SUCCESS = 0,
    QL_WIFI_UART_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_UART_INVALID_PARAM_ERR,
    QL_WIFI_UART_TASK_CREATE_ERR,
    QL_WIFI_UART_MUTEX_CREATE_ERR,
    QL_WIFI_UART_PORT_SET_ERR,
    QL_WIFI_UART_PIN_SET_ERR,
    QL_WIFI_UART_OPEN_ERR,
    QL_WIFI_UART_CB_REG_ERR,
} ql_wifi_uart_errcode_e;

/*========================================================================
*  Type Definition
*========================================================================*/
typedef int (*ql_wifi_uart_notify_cb)(void *ctx);

/*========================================================================
 *  function Definition
 *========================================================================*/

unsigned char npto_CRC(void *src, int ilen);
/*****************************************************************
 * Function: ql_wifi_uart_output_data_msg
 *
 * Description: Wi-Fi command output function.Use this function to send command.
 *
 * Parameters:
 * ctx        [in]       See ql_wifi_msg_output_data_info_s in ql_wifi_msg.h
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
int ql_wifi_uart_output_data_msg(void *ctx);

/*****************************************************************
 * Function: ql_wifi_uart_notify_cb_register
 *
 * Description: Register UART notificatiion callback
 *
 * Parameters:
 * cb        [in]       UART notificatiion callback
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_uart_errcode_e ql_wifi_uart_notify_cb_register(ql_wifi_uart_notify_cb cb);

/*****************************************************************
 * Function: ql_wifi_uart_init
 *
 * Description: Initialize Wi-Fi uart engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_uart_errcode_e ql_wifi_uart_init(void *argv);

/*****************************************************************
 * Function: ql_wifi_uart_deinit
 *
 * Description: Deinitialize Wi-Fi uart engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_uart_errcode_e ql_wifi_uart_deinit(void* argv);

int ql_wifi_uart_write(char *data, int data_len);

#ifdef __cplusplus
}/*"C" */
#endif

#endif
