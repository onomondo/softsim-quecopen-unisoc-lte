/**
  @file
  ql_wifi_uart.c

  @brief
  Quectel wifi uart api.

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

/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_osi_def.h"

#include "quec_pin_index.h"
#include "ql_gpio.h"
#include "ql_uart.h"

#include "ql_wifi_net.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_uart.h"
#include "ql_api_wifi.h"

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_UART_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_uart", msg, ##__VA_ARGS__)

#define QL_WIFI_UART_RX_BUFF_SIZE 2048
#define QL_WIFI_UART_TX_BUFF_SIZE 2048
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// uart2
#define QL_WIFI_UART2_TX_PIN QUEC_PIN_UART2_TXD
#define QL_WIFI_UART2_TX_FUNC 0x01
#define QL_WIFI_UART2_RX_PIN QUEC_PIN_UART2_RXD
#define QL_WIFI_UART2_RX_FUNC 0x01

#define QL_WIFI_UART_PORT QL_UART_PORT_2
#define QL_WIFI_UART_TX_PIN QL_WIFI_UART2_TX_PIN
#define QL_WIFI_UART_TX_FUNC QL_WIFI_UART2_TX_FUNC
#define QL_WIFI_UART_RX_PIN QL_WIFI_UART2_RX_PIN
#define QL_WIFI_UART_RX_FUNC QL_WIFI_UART2_RX_FUNC

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_wifi_uart_notify_cb notify_cb;
    ql_mutex_t write_mutex;
} ql_wifi_uart_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_uart_manager_s ql_wifi_uart_manager = {0};

/*========================================================================
 *	function Definition
 *========================================================================*/
unsigned char npto_CRC(void *src, int ilen)
{
    int i;
    unsigned char crc = 0, *ptr = (unsigned char *)src;

    for (i = 0; i < ilen; i++)
    {
        crc += ptr[i];
    }

    return crc;
}

void ql_wifi_uart_cb(unsigned int ind_type, ql_uart_port_number_e port, unsigned int size)
{
    unsigned char *recv_buff = 0;
    unsigned int real_size = 0;
    int read_len = 0;
    ql_wifi_uart_manager_s *manager = &ql_wifi_uart_manager;

    QL_WIFI_UART_LOG("UART port %d receive ind type:0x%x, receive data size:%d", port, ind_type, size);
    switch (ind_type)
    {
    case QUEC_UART_RX_OVERFLOW_IND: // rx buffer overflow
    case QUEC_UART_RX_RECV_DATA_IND: {
        while (size > 0)
        {
            real_size = MIN(size, QL_WIFI_UART_RX_BUFF_SIZE);
            recv_buff = malloc(real_size + 1);
            if (recv_buff == NULL)
            {
                QL_WIFI_UART_LOG("malloc failed");
                return;
            }

            read_len = ql_uart_read(port, recv_buff, real_size);
            QL_WIFI_UART_LOG("read_len=%d, recv_data=%p", read_len, recv_buff);

            if (manager->notify_cb != NULL)
            {
                ql_wifi_msg_info_s info = {.data = recv_buff,.data_len = read_len};
                manager->notify_cb(&info);
            }
            else
            {
                free(recv_buff);
                recv_buff = NULL;
            }

            if ((read_len > 0) && (size >= read_len))
            {
                size -= read_len;
            }
            else
            {
                break;
            }
        }
        break;
    }
    case QUEC_UART_TX_FIFO_COMPLETE_IND: {
        QL_WIFI_UART_LOG("tx fifo complete");
        break;
    }
    }
}

int ql_wifi_uart_write(char *data, int data_len)
{
    int write_len = 0;
    ql_wifi_uart_manager_s *manager = &ql_wifi_uart_manager;
    ql_uart_tx_status_e tx_status;

    if (manager->write_mutex == NULL)
        return 0;

    ql_rtos_mutex_lock(manager->write_mutex, QL_WAIT_FOREVER);

    write_len = ql_uart_write(QL_WIFI_UART_PORT, (unsigned char *)data, data_len);

    QL_WIFI_UART_LOG("write_len:%d", write_len);
    ql_uart_get_tx_fifo_status(QL_WIFI_UART_PORT, &tx_status);
    QL_WIFI_UART_LOG("tx_status:%d", tx_status);

    ql_rtos_mutex_unlock(manager->write_mutex);

    return data_len;
}

ql_wifi_uart_errcode_e ql_wifi_uart_notify_cb_register(ql_wifi_uart_notify_cb cb)
{
    ql_wifi_uart_manager_s *manager = &ql_wifi_uart_manager;
    manager->notify_cb = cb;
    return QL_WIFI_UART_SUCCESS;
}

ql_wifi_uart_errcode_e ql_wifi_uart_init(void *argv)
{
    QL_WIFI_UART_LOG("ql_wifi_uart_init");
    ql_wifi_uart_errcode_e ret = QL_WIFI_UART_SUCCESS;
    int err = 0;
    ql_uart_config_s uart_cfg = {0};
    ql_wifi_uart_manager_s *manager = &ql_wifi_uart_manager;
    uart_cfg.baudrate = QL_UART_BAUD_921600;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;

    memset(manager, 0, sizeof(ql_wifi_uart_manager_s));
    err = ql_rtos_mutex_create(&(manager->write_mutex));
    if (QL_OSI_SUCCESS != err)
    {
        ret = QL_WIFI_UART_MUTEX_CREATE_ERR;
        goto exit;
    }

    err = ql_uart_set_dcbconfig(QL_WIFI_UART_PORT, &uart_cfg);
    QL_WIFI_UART_LOG("err: 0x%x", err);
    if (QL_UART_SUCCESS != err)
    {
        ret = QL_WIFI_UART_PORT_SET_ERR;
        goto exit;
    }

    err = ql_pin_set_func(QL_WIFI_UART_TX_PIN, QL_WIFI_UART_TX_FUNC);
    if (QL_GPIO_SUCCESS != err)
    {
        ret = QL_WIFI_UART_PIN_SET_ERR;
        goto exit;
    }
    err = ql_pin_set_func(QL_WIFI_UART_RX_PIN, QL_WIFI_UART_RX_FUNC);
    if (QL_GPIO_SUCCESS != err)
    {
        ret = QL_WIFI_UART_PIN_SET_ERR;
        goto exit;
    }
    /*Note end*/
    err = ql_uart_open(QL_WIFI_UART_PORT);
    QL_WIFI_UART_LOG("err: 0x%x", err);

    if (QL_UART_SUCCESS == err)
    {
        err = ql_uart_register_cb(QL_WIFI_UART_PORT, ql_wifi_uart_cb);
        QL_WIFI_UART_LOG("err: 0x%x", err);
        if (QL_UART_SUCCESS != err)
        {
            ret = QL_WIFI_UART_CB_REG_ERR;
            goto exit;
        }
        memset(&uart_cfg, 0, sizeof(ql_uart_config_s));
        err = ql_uart_get_dcbconfig(QL_WIFI_UART_PORT, &uart_cfg);
        QL_WIFI_UART_LOG("err: 0x%x, baudrate=%d, flow_ctrl=%d, data_bit=%d, stop_bit=%d, parity_bit=%d",
                         err, uart_cfg.baudrate, uart_cfg.flow_ctrl, uart_cfg.data_bit, uart_cfg.stop_bit, uart_cfg.parity_bit);
    }
    else
    {
        ret = QL_WIFI_UART_OPEN_ERR;
        goto exit;
    }
exit:
    if (ret != QL_WIFI_UART_SUCCESS)
    {
        ql_wifi_uart_deinit(NULL);
        return ret;
    }
    return QL_WIFI_UART_SUCCESS;
}

ql_wifi_uart_errcode_e ql_wifi_uart_deinit(void *argv)
{
    ql_wifi_uart_manager_s *manager = &ql_wifi_uart_manager;
    ql_wifi_uart_manager_s cmp = {0};
    if (0 != memcmp(manager, &cmp, sizeof(ql_wifi_uart_manager_s)))
    {
        ql_uart_close(QL_WIFI_UART_PORT);
        ql_rtos_mutex_delete(manager->write_mutex);
        memset(manager, 0, sizeof(ql_wifi_uart_manager_s));
    }
    else
    {
        QL_WIFI_UART_LOG("wifi uart repeat deinit");
    }
    return QL_WIFI_UART_SUCCESS;
}

// SIO output data msg
int ql_wifi_uart_output_data_msg(void *ctx)
{
    ql_wifi_msg_output_data_info_s *info = (ql_wifi_msg_output_data_info_s*)ctx;
    uint8_t *data = info->data;
    int data_len = info->data_len;
    int cmd = info->cmd;

    ql_wifi_msg_protocol_head_info_s head_info = {0};
    char *output_data = NULL;
    unsigned char checksum;

    memset(&head_info, 0, sizeof(ql_wifi_msg_protocol_head_info_s));

    output_data = malloc(data_len + QUEC_PROTOCOL_HEAD_MAX_SIZE + 1);
    if (output_data == NULL)
        return 0;

    head_info.magic0 = QL_WIFI_UART_MAGIC0;
    head_info.magic1 = QL_WIFI_UART_MAGIC1;
    head_info.command = cmd;
    head_info.cmdlen = data_len + QUEC_PROTOCOL_HEAD_MAX_SIZE;

    memcpy(output_data, &head_info, QUEC_PROTOCOL_HEAD_MAX_SIZE);
    memcpy(output_data + QUEC_PROTOCOL_HEAD_MAX_SIZE, data, data_len);

    checksum = npto_CRC(output_data, QUEC_PROTOCOL_HEAD_MAX_SIZE + data_len);
    output_data[QUEC_PROTOCOL_HEAD_MAX_SIZE - 1] = checksum;

    ql_wifi_uart_write(output_data, QUEC_PROTOCOL_HEAD_MAX_SIZE + data_len);
    OSI_LOGXI(OSI_LOGPAR_M, 0, "uart tx: %*s", QUEC_PROTOCOL_HEAD_MAX_SIZE + data_len, output_data);

    if (output_data != NULL)
    {
        free(output_data);
        output_data = NULL;
    }

    return 0;
}