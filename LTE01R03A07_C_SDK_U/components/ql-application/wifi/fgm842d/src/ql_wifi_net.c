/**
  @file
  ql_wifi_net.c

  @brief
  Quectel wifi net api.

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
#include "ql_osi_def.h"
#include "ql_log.h"
#include "ql_api_wifi.h"

#include "quec_common.h"

#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_NET_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_net", msg, ##__VA_ARGS__)

#define QL_WIFI_NET_TASK_EVENT_CNT 5
#define QL_WIFI_NET_MSG_QUEUE_CNT 100

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_queue_t msg_queue;
    ql_task_t task;
    ql_wifi_net_app_notify_cb app_notify_cb;
} ql_wifi_net_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_net_manager_s ql_wifi_net_manager = {0};
ql_mutex_t ql_wifi_net_mutex = NULL;

/*========================================================================
 *	function Definition
 *========================================================================*/
void ql_wifi_net_recv(void *ctx)
{
    // Message from MSG engine.It needs to be registered to MSG engine.
    // Once we get data from MSG engine,it will be passed to NET engine.
    ql_wifi_msg_cmd_info_s *info = (ql_wifi_msg_cmd_info_s *)ctx;
    QuecOSStatus err;
    ql_wifi_msg_cmd_info_s msg_cmd_info = {0};
    ql_wifi_net_manager_s *manager = &ql_wifi_net_manager;

    msg_cmd_info.msg_id = info->msg_id;
    msg_cmd_info.msg_info = info->msg_info;

    QL_WIFI_NET_LOG("net msg %x/%p", info->msg_id, info->msg_info);
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    err = ql_rtos_queue_release(manager->msg_queue, sizeof(ql_wifi_msg_cmd_info_s), (uint8 *)&msg_cmd_info, QL_NO_WAIT);
    if (err)
    {
        QL_WIFI_NET_LOG("net msg rel failed");
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    return;
}

static void ql_wifi_net_state_notify(void *ctx)
{
    ql_wifi_state_e state = *((ql_wifi_state_e*)ctx);
    char *p = NULL;
    ql_wifi_net_manager_s *manager = &ql_wifi_net_manager;

    p = malloc(2);
    if (p == NULL)
    {
        return;
    }
    p[0] = (char)state;
    p[1] = '\0';

    // Nofity Wi-Fi connect status to APP engine.
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    if (manager->app_notify_cb)
    {
        ql_wifi_msg_cmd_info_s info = {.msg_id = QL_WIFI_APP_STA_STATUS_CMD,.msg_info = p};
        manager->app_notify_cb(&info);
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
}

ql_wifi_net_errcode_e ql_wifi_net_app_notify_cb_register(ql_wifi_net_app_notify_cb cb)
{
    // APP engine notification cb for notifying app status.
    // Use ql_wifi_net_state_notify to calling this callback.
    ql_wifi_net_manager_s *manager = &ql_wifi_net_manager;
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    manager->app_notify_cb = cb;
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    return QL_WIFI_NET_SUCCESS;
}

void ql_wifi_net_thread(void *argv)
{
    QuecOSStatus err;
    ql_wifi_msg_cmd_info_s cmd_info = {0};
    ql_wifi_net_manager_s *manager = &ql_wifi_net_manager;

    QL_WIFI_NET_LOG("wifi net init");

    err = ql_rtos_queue_create(&(manager->msg_queue), sizeof(ql_wifi_msg_cmd_info_s), QL_WIFI_NET_MSG_QUEUE_CNT);
    if (err)
    {
        QL_WIFI_NET_LOG("create queue faild!");
        goto exit;
    }
    if (QL_WIFI_MSG_SUCCESS != ql_wifi_msg_net_notify_cb_register(ql_wifi_net_recv))
    {
        QL_WIFI_NET_LOG("msg net cb reg err");
        goto exit;
    }
    // from net to app
    if (QL_WIFI_SUCCESS != ql_wifi_status_notify_cb_register(ql_wifi_net_state_notify))
    {
        QL_WIFI_NET_LOG("wifi status notify cb reg err");
        goto exit;
    }
    while (1)
    {
        memset(&cmd_info, 0, sizeof(ql_wifi_msg_cmd_info_s));
        QL_WIFI_NET_LOG("");
        err = ql_rtos_queue_wait(manager->msg_queue, (uint8 *)&cmd_info, sizeof(ql_wifi_msg_cmd_info_s), QL_WAIT_FOREVER);
        if (err != kNoErr)
        {
            QL_WIFI_NET_LOG("wait error err= %d", err);
            goto exit;
        }

        QL_WIFI_NET_LOG("cmd_info.msg_id=%d", cmd_info.msg_id);
        switch (cmd_info.msg_id)
        {
        case QL_WIFI_NPTO_802_3_STREAM: {
            // Undefined net command
            // ql_wifi_msg_info_s *msg_ptr = (ql_wifi_msg_info_s *)(cmd_info.msg_info);
            // ql_wifi_data_input(msg_ptr->data,msg_ptr->data_len);
            break;
        }
        case QL_WIFI_NPTO_STA_IP_START_CMD: {
            ql_wifi_sta_ip_start(cmd_info.msg_info);
            break;
        }
        case QL_WIFI_NPTO_STA_IP_DOWN_CMD: {
            ql_wifi_sta_ip_down(cmd_info.msg_info);
            break;
        }
        case QL_WIFI_NPTO_WLAN_CREATE_CMD: {
            ql_wifi_wlan_create(cmd_info.msg_info);
            break;
        }
        case QL_WIFI_NPTO_WLAN_DESTROY_CMD: {
            ql_wifi_wlan_destroy(cmd_info.msg_info);
            break;
        }
        case QL_WIFI_NPTO_UAP_IP_START_CMD:
        case QL_WIFI_NPTO_UAP_IP_DOWN_CMD:
        case QL_WIFI_NPTO_IP_ADDR_SET_CMD:
        default: {
            QL_WIFI_NET_LOG("cmd_info.msg_id=%d  ERROR", cmd_info.msg_id);
            break;
        }
        }

        QL_WIFI_NET_LOG("cmd_info.msg_info=%p", cmd_info.msg_info);
        if (cmd_info.msg_info != NULL)
        {
            free(cmd_info.msg_info);
            cmd_info.msg_info = NULL;
        }
    }

exit:
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    memset(manager, 0, sizeof(ql_wifi_net_manager_s));
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    ql_rtos_task_delete(NULL);
    return;
}

ql_wifi_net_errcode_e ql_wifi_net_init(void *argv)
{
    ql_wifi_net_errcode_e ret = QL_WIFI_NET_SUCCESS;
    ql_wifi_net_manager_s *manager = &ql_wifi_net_manager;
    QL_WIFI_NET_LOG("ql_wifi_net_init");

    // Create mutex.Never delete.
    if (NULL == ql_wifi_net_mutex)
    {
        if (QL_OSI_SUCCESS != ql_rtos_mutex_create(&(ql_wifi_net_mutex)))
        {
            ret = QL_WIFI_NET_MUTEX_CREATE_ERR;
            return ret;
        }
    }
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    if(manager->task)
    {
        return QL_WIFI_NET_SUCCESS;
    }
    if (QL_OSI_SUCCESS != ql_rtos_task_create(&(manager->task), 4096, APP_PRIORITY_REALTIME, "ql_wifi_net", ql_wifi_net_thread, NULL, QL_WIFI_NET_TASK_EVENT_CNT))
    {
        QL_WIFI_NET_LOG("ql_wifi_net_init err");
        ret = QL_WIFI_NET_TASK_CREATE_ERR;
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    return ret;
}

ql_wifi_net_errcode_e ql_wifi_net_deint(void *argv)
{
    return QL_WIFI_NET_SUCCESS;
}