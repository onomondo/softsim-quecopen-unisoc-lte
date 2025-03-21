/**
  @file
  ql_wifi_msg.c

  @brief
  Quectel wifi msg api.

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

#include "ql_osi_def.h"
#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_wifi.h"

#include "quec_common.h"

#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#include "ql_wifi_uart.h"

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_MSG_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_msg", msg, ##__VA_ARGS__)

#define QL_WIFI_MSG_TASK_EVENT_CNT 5
#define QL_WIFI_MSG_MSG_QUEUE_CNT 100

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t task;
    ql_wifi_msg_net_notify_cb net_notify_cb;
    ql_wifi_msg_app_notify_cb app_notify_cb;
    char *malloc_data_field;
    ql_queue_t msg_queue;
} ql_wifi_msg_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_msg_manager_s ql_wifi_msg_manager = {0};
ql_mutex_t ql_wifi_msg_mutex = NULL;
/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_msg_errcode_e ql_wifi_msg_net_notify_cb_register(ql_wifi_msg_net_notify_cb cb)
{
    // Net engine notification cb for handling net command
    ql_wifi_msg_manager_s *manager = &ql_wifi_msg_manager;
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    manager->net_notify_cb = cb;
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return QL_WIFI_MSG_SUCCESS;
}

ql_wifi_msg_errcode_e ql_wifi_msg_app_notify_cb_register(ql_wifi_msg_app_notify_cb cb)
{
    // APP engine notification cb for handling app command
    ql_wifi_msg_manager_s *manager = &ql_wifi_msg_manager;
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    manager->app_notify_cb = cb;
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return QL_WIFI_MSG_SUCCESS;
}

int ql_wifi_msg_npto_parse(char *src, int src_len, ql_wifi_msg_protocol_head_info_s *cmdPkt, int *dst_len, int *result)
{
    int i = 0;
    int cmdlen = 0;
    char *ptr = src;
    unsigned char checksum;

#if 0 // debug
	for (i = 0; i < src_len; i++)
	{
        QL_WIFI_MSG_LOG("src[%d]=%x", i, ptr[i]);
	}
#endif

    // Check source input length
    if (src_len < QUEC_PROTOCOL_HEAD_MAX_SIZE)
    {
        QL_WIFI_MSG_LOG("less than protocol header size!!\r\n");
        *dst_len = 0; // free no magic buff
        *result = QL_WIFI_NPTO_PARSE_RESULT_TOOSMALL;
        return -1;
    }

    // Find header identification "QL_WIFI_UART_MAGIC0 QL_WIFI_UART_MAGIC1"
    for (i = 0; i < (src_len - 1); i++)
    {
        if (ptr[i] == QL_WIFI_UART_MAGIC0 && ptr[i + 1] == QL_WIFI_UART_MAGIC1)
        {
            break;
        }
    }

    QL_WIFI_MSG_LOG("i=%d", i);

    // Return error when no header
    if (i >= (src_len - 1))
    {
        QL_WIFI_MSG_LOG("no search magic!!");
        /*
            Parse error:
            No valid magic header,returns i as parsing length.
            e.g. "0x12 0x34 0x56..."=>*dst_len = i
        */
        *dst_len = i;
        *result = QL_WIFI_NPTO_PARSE_RESULT_NOMAGIC;
        return -1;
    }

    // Check command length
    cmdlen = src_len - i;
    if (cmdlen < QUEC_PROTOCOL_HEAD_MAX_SIZE)
    {
        QL_WIFI_MSG_LOG("to small!! cmdlen = %d", cmdlen);
        /*
            Parse error:
            No valid parsing buffer length,returns i as parsing length.
            e.g. "uint8_t uint8_t"=>*dst_len = i
        */
        *dst_len = i;
        *result = QL_WIFI_NPTO_PARSE_RESULT_TOOSMALL;
        return 0;
    }

    // Get command header
    memcpy(cmdPkt, &ptr[i], QUEC_PROTOCOL_HEAD_MAX_SIZE);
    if (cmdPkt->cmdlen > QUEC_PROTOCOL_MSG_MAX_LEN)
    {
        /*
            Parse error:
            Command length from header is too large,but is valid.Return i + 2 as parsing length.
            e.g. "uint8_t uint8_t uint16_t"=>*dst_len = i + 2
        */
        *dst_len = i + 2;
        *result = QL_WIFI_NPTO_PARSE_RESULT_UNKNOWN;
        return -1;
    }

    // Check if command length is valid
    if (cmdPkt->cmdlen > cmdlen)
    {
        QL_WIFI_MSG_LOG("to small!!cmdPkt->cmdlen = %d cmdlen=%d", cmdPkt->cmdlen, cmdlen);
        /*
            Parse error:
            Command length from header is larger than parsing buffer length.Drop command length.Return i as parsing length.
            e.g. "uint8_t uint8_t"=>*dst_len = i
        */
        *dst_len = i;
        *result = QL_WIFI_NPTO_PARSE_RESULT_TOOSMALL;
        return 0;
    }

    // Get data field
    if ((cmdPkt->cmdlen - QUEC_PROTOCOL_HEAD_MAX_SIZE) > 0)
    {
        cmdPkt->data = malloc(cmdPkt->cmdlen - QUEC_PROTOCOL_HEAD_MAX_SIZE);
        if (cmdPkt->data == NULL)
        {
            QL_WIFI_MSG_LOG("malloc failed cmdPkt->cmdlen=%d", cmdPkt->cmdlen);
            /*
                Malloc error:
                No enough memory.Return i + 2 as parsing length.
                e.g. "uint8_t uint8_t uint16_t"=>*dst_len = i + 2
            */
            *dst_len = i + 2;
            *result = QL_WIFI_NPTO_PARSE_RESULT_UNKNOWN;
            return -1;
        }
        // Copy data field
        memcpy(cmdPkt->data, &ptr[i + QUEC_PROTOCOL_HEAD_MAX_SIZE], cmdPkt->cmdlen - QUEC_PROTOCOL_HEAD_MAX_SIZE);
    }

    // Set checksum 0 for re-calculation
    ptr[i + 5] = 0;

    checksum = npto_CRC(&ptr[i], cmdPkt->cmdlen);

    // Compare checksum from header to calculated checksum
    if (checksum != cmdPkt->checksum)
    {
        QL_WIFI_MSG_LOG("crc check error checksum=%x cmdPkt->checksum=%x", checksum, cmdPkt->checksum);
        /*
            Checksum error:
            Checksum mismatched.Return i + 2 as parsing length.
            e.g. "uint8_t uint8_t uint16_t"=>*dst_len = i + 2
        */
        *dst_len = i + 2;
        *result = QL_WIFI_NPTO_PARSE_RESULT_ERRCRC;
        return -1;
    }
    /*
        Parse successfully.Return i + command length as parsing length.
        e.g. "uint8_t uint8_t ......"=>*dst_len = i + cmdPkt->cmdlen
    */
    *dst_len = i + cmdPkt->cmdlen;
    *result = QL_WIFI_NPTO_PARSE_RESULT_SUCC;
    return QL_WIFI_NPTO_PARSE_RESULT_SUCC;
}

int ql_wifi_msg_recv(void *ctx)
{
    ql_wifi_msg_info_s *info = (ql_wifi_msg_info_s *)ctx;
    uint8_t *data = info->data;
    uint32_t data_len = info->data_len;
    // Data streams from UART/SPI and so on.It needs to be registered as physical device callback.
    // Once we get data,it will be passed to MSG engine.
    QuecOSStatus err;
    ql_wifi_msg_info_s msg_info = {0};
    ql_wifi_msg_manager_s *manager = &ql_wifi_msg_manager;

    memset(&msg_info, 0, sizeof(ql_wifi_msg_info_s));

    msg_info.data = data;
    msg_info.data_len = data_len;
    QL_WIFI_MSG_LOG("msg_info.data=%p", msg_info.data);

    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    err = ql_rtos_queue_release(manager->msg_queue, sizeof(ql_wifi_msg_info_s),
                                (uint8 *)&msg_info, QL_NO_WAIT);
    if (err != kNoErr)
    {
        QL_WIFI_MSG_LOG("release msg_queue failed");
    }
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return err;
}

static void ql_wifi_msg_thread(void *argv)
{
    QuecOSStatus err;
    ql_wifi_msg_info_s msg_info = {0};
    ql_wifi_msg_protocol_info_s protocol_info = {0};
    int max_cache_len = QUEC_MSG_CACHE_MAX_LEN; // max cache 4 data
    ql_wifi_msg_protocol_head_info_s protocol_head_info = {0};
    int dst_buff_len = 0;
    int result = 0;
    ql_wifi_msg_manager_s *manager = &ql_wifi_msg_manager;

    memset(manager, 0, sizeof(ql_wifi_msg_manager_s));

    // Create message queue
    err = ql_rtos_queue_create(&manager->msg_queue, sizeof(ql_wifi_msg_info_s), QL_WIFI_MSG_MSG_QUEUE_CNT);
    if (err)
    {
        goto exit;
    }
    // Register data physical device.Use UART for command data streams.
    if(QL_WIFI_UART_SUCCESS != ql_wifi_uart_notify_cb_register(ql_wifi_msg_recv))
    {
        goto exit;
    }

    // Malloc command data streams receive cache
    protocol_info.data = malloc(max_cache_len);
    if (protocol_info.data == NULL)
    {
        err = -1;
        goto exit;
    }
    // Pointer is used for deinit.
    manager->malloc_data_field = protocol_info.data;

    memset(&protocol_head_info, 0, sizeof(ql_wifi_msg_protocol_head_info_s));

    while (1)
    {
        memset(&msg_info, 0, sizeof(ql_wifi_msg_info_s));
        protocol_head_info.data = NULL;
        dst_buff_len = 0;
        result = 0;

        err = ql_rtos_queue_wait(manager->msg_queue, (uint8 *)&msg_info,
                                 sizeof(ql_wifi_msg_info_s), QL_WAIT_FOREVER);
        if (err != kNoErr)
        {
            QL_WIFI_MSG_LOG("wait error err= %d", err);
            goto exit;
        }
        // Protocol processing
        if (msg_info.data_len + protocol_info.data_len > max_cache_len)
        {
            QL_WIFI_MSG_LOG("msg data too long!!!!!");
            goto exit;
        }

        memcpy(protocol_info.data + protocol_info.data_len, msg_info.data, msg_info.data_len);
        protocol_info.data_len = protocol_info.data_len + msg_info.data_len;
        while (protocol_info.data_len > 0)
        {
            ql_wifi_msg_npto_parse(protocol_info.data, protocol_info.data_len,
                                   &protocol_head_info, &dst_buff_len, &result);
            QL_WIFI_MSG_LOG("protocol_info.data_len=%d dst_buff_len=%d result=%d",
                            protocol_info.data_len, dst_buff_len, result);

            if (dst_buff_len > 0 && protocol_info.data_len - dst_buff_len >= 0)
            {
                memmove(protocol_info.data, protocol_info.data + dst_buff_len,
                        protocol_info.data_len - dst_buff_len);
                protocol_info.data_len -= dst_buff_len;
            }

            // protocol success
            if (result == QL_WIFI_NPTO_PARSE_RESULT_SUCC)
            {
                QL_WIFI_MSG_LOG("command=%d", protocol_head_info.command);
                switch (protocol_head_info.command)
                {
                case QL_WIFI_NPTO_802_3_STREAM: {
                    // Pass data streams to NET engine
                    ql_wifi_msg_info_s *msg_ptr = NULL;
                    msg_ptr = malloc(sizeof(ql_wifi_msg_info_s));
                    if (msg_ptr == NULL)
                        goto exit;
                    msg_ptr->data_len = protocol_head_info.cmdlen - QUEC_PROTOCOL_HEAD_MAX_SIZE;
                    msg_ptr->data = protocol_head_info.data;
                    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
                    // QL_WIFI_MSG_LOG("data_len=%d dst_buff_len=%d result=%d", protocol_info.data_len, dst_buff_len, result);
                    if (manager->net_notify_cb != NULL)
                    {
                        ql_wifi_msg_cmd_info_s info = {.msg_id = QL_WIFI_NPTO_802_3_STREAM,.msg_info = msg_ptr};
                        manager->net_notify_cb(&info);
                    }
                    else
                    {
                        free(msg_ptr);
                        msg_ptr = NULL;
                    }
                    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
                    break;
                }
                case QL_WIFI_NPTO_STA_IP_START_CMD:
                case QL_WIFI_NPTO_STA_IP_DOWN_CMD:
                case QL_WIFI_NPTO_UAP_IP_START_CMD:
                case QL_WIFI_NPTO_UAP_IP_DOWN_CMD:
                case QL_WIFI_NPTO_IP_ADDR_SET_CMD:
                case QL_WIFI_NPTO_WLAN_CREATE_CMD:
                case QL_WIFI_NPTO_WLAN_DESTROY_CMD: {
                    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
                    if (manager->net_notify_cb != NULL)
                    {
                        ql_wifi_msg_cmd_info_s info = {.msg_id = protocol_head_info.command,.msg_info = protocol_head_info.data};
                        manager->net_notify_cb(&info);
                    }
                    else
                    {
                        free(protocol_head_info.data);
                        protocol_head_info.data = NULL;
                    }
                    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
                    break;
                }
                case QL_WIFI_APP_SCAN_START_CMD:
                case QL_WIFI_APP_STA_STATUS_CMD:
                case QL_WIFI_APP_EVENT_NOTIFY: {
                    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
                    if (manager->app_notify_cb != NULL)
                    {
                        ql_wifi_msg_cmd_info_s info = {.msg_id = protocol_head_info.command,.msg_info = protocol_head_info.data};
                        manager->app_notify_cb(&info);
                    }
                    else
                    {

                        free(protocol_head_info.data);
                        protocol_head_info.data = NULL;
                    }
                    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
                    break;
                }
                default: {
                    QL_WIFI_MSG_LOG("command=%d error", protocol_head_info.command);
                    break;
                }
                }
            }
            else
            {
                memset(&protocol_head_info, 0, sizeof(ql_wifi_msg_protocol_head_info_s));
                break;
            }
            memset(&protocol_head_info, 0, sizeof(ql_wifi_msg_protocol_head_info_s));
        }

        QL_WIFI_MSG_LOG("msg_info.data=%p", msg_info.data);
        if (msg_info.data != NULL)
        {
            free(msg_info.data);
            msg_info.data = NULL;
        }
    }
exit:
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    memset(manager, 0, sizeof(ql_wifi_msg_manager_s));
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    ql_rtos_task_delete(NULL);
    return;
}

ql_wifi_msg_errcode_e ql_wifi_msg_init(void *argv)
{
    ql_wifi_msg_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    ql_wifi_msg_manager_s *manager = &ql_wifi_msg_manager;
    QL_WIFI_MSG_LOG("ql_wifi_msg_init");

    // Create mutex.Never delete.
    if (NULL == ql_wifi_msg_mutex)
    {
        if (QL_OSI_SUCCESS != ql_rtos_mutex_create(&(ql_wifi_msg_mutex)))
        {
            ret = QL_WIFI_MSG_MUTEX_CREATE_ERR;
            return ret;
        }
    }
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    if(manager->task)
    {
        return QL_WIFI_MSG_SUCCESS;
    }
    memset(manager, 0, sizeof(ql_wifi_msg_manager_s));
    if (QL_OSI_SUCCESS != ql_rtos_task_create(&manager->task, 4096, APP_PRIORITY_REALTIME, "ql_wifi_msg", ql_wifi_msg_thread, NULL, QL_WIFI_MSG_TASK_EVENT_CNT))
    {
        QL_WIFI_MSG_LOG("ql_wifi_msg_init err");
        ret = QL_WIFI_MSG_TASK_CREATE_ERR;
    }
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return ret;
}

ql_wifi_msg_errcode_e ql_wifi_msg_deinit(void *argv)
{
    return QL_WIFI_MSG_SUCCESS;
}
