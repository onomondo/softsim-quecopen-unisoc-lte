/**
  @file
  ql_wifi_app.c

  @brief
  Quectel wifi app api.

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

#include <lwip/inet.h>
#include "netif/etharp.h"

#include "ql_osi_def.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#include "quec_common.h"

#include "ql_api_wifi.h"

#include "ql_wifi_app.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#include "ql_wifi_uart.h"
#include "ql_wifi_spi.h"

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_APP_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_app", msg, ##__VA_ARGS__)

#define QL_WIFI_APP_TASK_EVENT_CNT 5
#define QL_WIFI_APP_MSG_QUEUE_CNT 10

#define QL_WIFI_APP_AP_DISABLE 0x01
#define QL_WIFI_APP_STA_DISABLE 0x02

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_APP_SCAN_CMD_TYPE_START = 1,
    QL_WIFI_APP_SCAN_CMD_TYPE_AP_DATA,
    QL_WIFI_APP_SCAN_CMD_TYPE_END
} ql_wifi_app_scan_cmd_type_e;

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_wifi_app_scan_cmd_type_e cmd_type;
    char num;
    ql_wifi_app_ap_info_s ap_info;
} ql_wifi_app_scan_info_s;

typedef struct
{
    ql_task_t task;
    ql_queue_t msg_queue;
    ql_wifi_app_sta_status_cb sta_status_cb;
    ql_wifi_app_scan_result_cb scan_start_cb;
    ql_wifi_app_output_data_msg_cb output_data_msg_cb;
    ql_wifi_app_user_ap_info_s ap_info;
} ql_wifi_app_manager_s;

#pragma pack(1)
typedef struct
{
    char dhcp;
    char ip_str[QL_WIFI_IPV4_MAX_LEN];
    char netmask_str[QL_WIFI_IPV4_MAX_LEN];
    char gw_str[QL_WIFI_IPV4_MAX_LEN];
    char ssid[QL_WIFI_SSID_MAX_LEN];
    char password[QL_WIFI_PASSWORD_MAX_LEN];
} ql_wifi_app_start_info_s;
#pragma pack()
/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_app_manager_s ql_wifi_app_manager = {0};
ql_mutex_t ql_wifi_app_mutex = NULL;

ql_wifi_app_scan_result_s ql_wifi_app_scan_result = {0};
static int ql_wifi_app_scan_num = 0;

/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_app_errcode_e ql_wifi_app_get_user_ap_info(ql_wifi_app_user_ap_info_s *info)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    if(!info)
    {
        return QL_WIFI_APP_INVALID_PARAM_ERR;
    }
    memcpy(info,&(manager->ap_info),sizeof(ql_wifi_app_user_ap_info_s));
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_set_user_ap_info(ql_wifi_app_user_ap_info_s *info)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    if(!info)
    {
        return QL_WIFI_APP_INVALID_PARAM_ERR;
    }
    memcpy(&(manager->ap_info),info,sizeof(ql_wifi_app_user_ap_info_s));
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_scan(ql_wifi_app_scan_result_cb cb)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;

    // Set wifi app scan result callback
    if (cb)
    {
        ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
        if (ql_wifi_app_scan_result.ApList != NULL)
        {
            return QL_WIFI_APP_SCAN_WORKING_ERR;
        }
        manager->scan_start_cb = cb;
        // Send wifi control command
        if (manager->output_data_msg_cb)
        {
            ql_wifi_msg_output_data_info_s info = {.cmd = QL_WIFI_APP_SCAN_START_CMD, .data = NULL, .data_len = 0};
            manager->output_data_msg_cb(&info);
        }
        ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    }
    else
    {
        return QL_WIFI_APP_CB_NULL_ERR;
    }
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_sta_enable(ql_wifi_app_sta_status_cb sta_status_cb)
{
    int ret = 0;
    ql_wifi_app_errcode_e err = QL_WIFI_APP_SUCCESS;
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    ql_wifi_app_start_info_s start = {0};
    int val_len = 0;
    char dhcp = 0;
    ip4_addr_t ip = {0};
    ip4_addr_t netmask = {0};
    ip4_addr_t gw = {0};

    memset(&start, 0, sizeof(ql_wifi_app_start_info_s));

    // Get ssid & password
    ql_wifi_app_user_ap_info_s *ap_user_info = (ql_wifi_app_user_ap_info_s *)calloc(1,sizeof(ql_wifi_app_user_ap_info_s));
    if(!ap_user_info)
    {
        QL_WIFI_APP_LOG("calloc faild!");
        err = QL_WIFI_APP_NO_MEM_ERR;
        goto exit;
    }
    ret = ql_wifi_app_get_user_ap_info(ap_user_info);
    if(ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_APP_LOG("get ssid/password faild!");
        err = QL_WIFI_APP_INVALID_AP_USER_INFO_ERR;
        free(ap_user_info);
        goto exit;
    }

    memcpy(&(start.ssid),ap_user_info->ssid,QL_WIFI_SSID_MAX_LEN);
    memcpy(&(start.password),ap_user_info->password,QL_WIFI_PASSWORD_MAX_LEN);
    free(ap_user_info);

    // Get dhcp flag
    val_len = sizeof(char);
    ret = ql_wifi_opt_get(QL_WIFI_OPT_DHCP, &dhcp, &val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_APP_LOG("get dhcp faild!");
        err = QL_WIFI_APP_INVALID_DHCP_ERR;
        goto exit;
    }
    start.dhcp = dhcp;

    // Get static ip information when dhcp is not enable.
    if (!dhcp)
    {
        // Get static ip
        val_len = sizeof(ip4_addr_t);
        ret = ql_wifi_opt_get(QL_WIFI_OPT_IP, &ip, &val_len);
        if (ret != QL_WIFI_SUCCESS)
        {
            QL_WIFI_APP_LOG("get ip faild!");
            err = QL_WIFI_APP_INVALID_IP_ERR;
            goto exit;
        }

        // Get static netmask
        val_len = sizeof(ip4_addr_t);
        ret = ql_wifi_opt_get(QL_WIFI_OPT_NETMASK, &netmask, &val_len);
        if (ret != QL_WIFI_SUCCESS)
        {
            QL_WIFI_APP_LOG("get netmask faild!");
            err = QL_WIFI_APP_INVALID_NETMASK_ERR;
            goto exit;
        }

        // Get static gateway
        val_len = sizeof(ip4_addr_t);
        ret = ql_wifi_opt_get(QL_WIFI_OPT_GW, &gw, &val_len);
        if (ret != QL_WIFI_SUCCESS)
        {
            QL_WIFI_APP_LOG("get gw faild!");
            err = QL_WIFI_APP_INVALID_GW_ERR;
            goto exit;
        }

        // Copy ip information to data field.
        memcpy(start.ip_str, inet_ntoa(ip), QL_WIFI_IPV4_MAX_LEN);
        memcpy(start.netmask_str, inet_ntoa(netmask), QL_WIFI_IPV4_MAX_LEN);
        memcpy(start.gw_str, inet_ntoa(gw), QL_WIFI_IPV4_MAX_LEN);

        QL_WIFI_APP_LOG("ip:%s", start.ip_str);
        QL_WIFI_APP_LOG("netmask:%s", start.netmask_str);
        QL_WIFI_APP_LOG("gw:%s", start.gw_str);
    }

    // Send wifi control command
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    if (manager->output_data_msg_cb)
    {
        ql_wifi_msg_output_data_info_s info = {.cmd = QL_WIFI_APP_STA_ENABLE_CMD, .data = (unsigned char *)&start, .data_len = sizeof(ql_wifi_app_start_info_s)};
        manager->output_data_msg_cb(&info);
    }
    // Set wifi sta status notify callback
    manager->sta_status_cb = sta_status_cb;
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
exit:
    QL_WIFI_APP_LOG("exit");
    return err;
}

ql_wifi_app_errcode_e ql_wifi_app_sta_disable(void *argv)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    unsigned char data = QL_WIFI_APP_STA_DISABLE;

    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    // Send wifi control command
    if (manager->output_data_msg_cb)
    {
        ql_wifi_msg_output_data_info_s info = {.cmd = QL_WIFI_APP_STA_DISABLE_CMD, .data = &data, .data_len = 1};
        manager->output_data_msg_cb(&info);
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_reset(void *argv)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    unsigned char data = QUEC_WIFI_APP_RESET_CMD;

    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    // Send wifi control command
    if (manager->output_data_msg_cb)
    {
        ql_wifi_msg_output_data_info_s info = {.cmd = QUEC_WIFI_APP_RESET_CMD, .data = &data, .data_len = 1};
        manager->output_data_msg_cb(&info);
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    return QL_WIFI_APP_SUCCESS;
}

static void ql_wifi_app_notify_recv(void *ctx)
{
    ql_wifi_msg_cmd_info_s *info = (ql_wifi_msg_cmd_info_s *)ctx;
    // Receive wifi app msg command.
    QuecOSStatus err;
    ql_wifi_msg_cmd_info_s cmd_info = {0};
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;

    cmd_info.msg_id = info->msg_id;
    cmd_info.msg_info = info->msg_info;

    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    err = ql_rtos_queue_release(manager->msg_queue, sizeof(ql_wifi_msg_cmd_info_s), (uint8 *)&cmd_info, QL_NO_WAIT);
    if (err)
    {
        QL_WIFI_APP_LOG("release failed");
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
}

static void ql_wifi_app_wifiscan_result_handler(void *argv)
{
    ql_wifi_app_scan_info_s *scan_ptr = (ql_wifi_app_scan_info_s *)argv;
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;

    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    // Initialize the parameter when receiving start command
    if (scan_ptr->cmd_type == QL_WIFI_APP_SCAN_CMD_TYPE_START)
    {
        if (ql_wifi_app_scan_result.ApList != NULL)
        {
            free(ql_wifi_app_scan_result.ApList);
            ql_wifi_app_scan_result.ApList = NULL;
            ql_wifi_app_scan_num = 0;
        }

        ql_wifi_app_scan_result.ApList = malloc(sizeof(ql_wifi_app_ap_info_s) * scan_ptr->num);
        if (ql_wifi_app_scan_result.ApList == NULL)
        {
            QL_WIFI_APP_LOG("malloc failed");
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            return;
        }
        ql_wifi_app_scan_result.ApNum = 0;
        ql_wifi_app_scan_num = 0;
        QL_WIFI_APP_LOG("scan start");
    }
    // Get AP lists
    else if (scan_ptr->cmd_type == QL_WIFI_APP_SCAN_CMD_TYPE_AP_DATA)
    {
        if (ql_wifi_app_scan_result.ApList == NULL)
        {
            QL_WIFI_APP_LOG("ApList is NULL");
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            return;
        }

        memcpy(&ql_wifi_app_scan_result.ApList[ql_wifi_app_scan_num], &scan_ptr->ap_info, sizeof(ql_wifi_app_ap_info_s));
        ql_wifi_app_scan_num++;
    }
    // Command end
    else if (scan_ptr->cmd_type == QL_WIFI_APP_SCAN_CMD_TYPE_END)
    {
        if (ql_wifi_app_scan_result.ApList == NULL)
        {
            QL_WIFI_APP_LOG("ApList is NULL");
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            return;
        }

        QL_WIFI_APP_LOG("scan end");

        ql_wifi_app_scan_result.ApNum = ql_wifi_app_scan_num;
        // Result notification
        if (manager->scan_start_cb)
        {
            manager->scan_start_cb((void *)&ql_wifi_app_scan_result);
        }
        free(ql_wifi_app_scan_result.ApList);
        ql_wifi_app_scan_result.ApList = NULL;
        ql_wifi_app_scan_result.ApNum = 0;
        ql_wifi_app_scan_num = 0;
    }
    else
    {
        if (ql_wifi_app_scan_result.ApList)
        {
            free(ql_wifi_app_scan_result.ApList);
        }
        ql_wifi_app_scan_result.ApList = NULL;
        ql_wifi_app_scan_result.ApNum = 0;
        ql_wifi_app_scan_num = 0;
        QL_WIFI_APP_LOG("NULL");
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
}

static void ql_wifi_app_thread(void *argv)
{
    QuecOSStatus err;
    ql_wifi_msg_cmd_info_s cmd_info = {0};
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;

    // Create message queue
    err = ql_rtos_queue_create(&manager->msg_queue, sizeof(ql_wifi_msg_cmd_info_s), QL_WIFI_APP_MSG_QUEUE_CNT);
    if (err)
    {
        goto exit;
    }

    // Handle QL_WIFI_APP_SCAN_START_CMD && QL_WIFI_APP_EVENT_NOTIFY && QL_WIFI_APP_STA_STATUS_CMD from ql_wifi_msg.c
    if (QL_WIFI_MSG_SUCCESS != ql_wifi_msg_app_notify_cb_register(ql_wifi_app_notify_recv))
    {
        QL_WIFI_APP_LOG("msg app cb reg err");
        goto exit;
    }

    // Handle QL_WIFI_APP_STA_STATUS_CMD from ql_wifi_net.c && ql_api_wifi.c
    if (QL_WIFI_NET_SUCCESS != ql_wifi_net_app_notify_cb_register(ql_wifi_app_notify_recv))
    {
        QL_WIFI_APP_LOG("net app cb reg err");
        goto exit;
    }
    while (1)
    {
        memset(&cmd_info, 0, sizeof(ql_wifi_msg_cmd_info_s));
        err = ql_rtos_queue_wait(manager->msg_queue, (uint8 *)&cmd_info, sizeof(ql_wifi_msg_cmd_info_s), QL_WAIT_FOREVER);
        if (err != kNoErr)
        {
            QL_WIFI_APP_LOG("msg_queue wait error err= %d", err);
            goto exit;
        }

        QL_WIFI_APP_LOG("msg_id: %d", cmd_info.msg_id);

        switch (cmd_info.msg_id)
        {
        case QL_WIFI_APP_SCAN_START_CMD: {
            ql_wifi_app_wifiscan_result_handler(cmd_info.msg_info);
            break;
        }
        case QL_WIFI_APP_EVENT_NOTIFY: {
            break;
        }
        case QL_WIFI_APP_STA_STATUS_CMD: {
            // Wifi connect status notification
            char *p = (char *)cmd_info.msg_info;
            int status = p[0];
            QL_WIFI_APP_LOG("status=%d", status);
            ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
            if (manager->sta_status_cb != NULL)
            {
                manager->sta_status_cb(&status);
            }
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            break;
        }
        default: {
            QL_WIFI_APP_LOG("cmd_info.msg_id=%d error", cmd_info.msg_id);
            break;
        }
        }

        if (cmd_info.msg_info != NULL)
        {
            free(cmd_info.msg_info);
            cmd_info.msg_info = NULL;
        }
    }

exit:
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    memset(manager, 0, sizeof(ql_wifi_app_manager_s));
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    ql_rtos_task_delete(NULL);
    return;
}

ql_wifi_app_errcode_e ql_wifi_app_init(void *argv)
{
    int ret = 0;
    ql_wifi_app_errcode_e err = QL_WIFI_APP_SUCCESS;
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    // Create mutex.Never delete.
    if (NULL == ql_wifi_app_mutex)
    {
        ret = ql_rtos_mutex_create(&(ql_wifi_app_mutex));
        if (ret != 0)
        {
            err = QL_WIFI_APP_MUTEX_CREATE_ERR;
            goto exit;
        }
    }
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    if (manager->task)
    {
        return QL_WIFI_APP_SUCCESS;
    }
    memset(manager, 0, sizeof(ql_wifi_app_manager_s));
    // Uart is used for sending wifi control command
    ret = ql_wifi_uart_init(NULL);
    if (ret != 0)
    {
        err = QL_WIFI_APP_INIT_UART_ERR;
        goto exit;
    }

    // Spi is used for sending or receiving 802.3 data streams
    ret = ql_wifi_spi_init(NULL);
    if (ret != 0)
    {
        err = QL_WIFI_APP_INIT_SPI_ERR;
        goto exit;
    }

    // Msg is used for parsing user protocol
    ret = ql_wifi_msg_init(NULL);
    if (ret != 0)
    {
        err = QL_WIFI_APP_INIT_MSG_ERR;
        goto exit;
    }

    // Net is used for handling netcard
    ret = ql_wifi_net_init(NULL);
    if (ret != 0)
    {
        err = QL_WIFI_APP_INIT_NET_ERR;
        goto exit;
    }

    // Register wifi command output handler
    manager->output_data_msg_cb = ql_wifi_uart_output_data_msg;
    // Register 802.3 data streams output handler
    ql_wifi_linkoutput_cb_register(ql_wifi_spi_output_data_msg);

    QL_WIFI_APP_LOG("ql_wifi_app_init");
    ret = ql_rtos_task_create(&manager->task, 4096, APP_PRIORITY_REALTIME, "ql_wifi_app",
                              ql_wifi_app_thread, NULL, QL_WIFI_APP_TASK_EVENT_CNT);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_APP_LOG("wifi app init err");
        err = QL_WIFI_APP_INIT_TASK_ERR;
        goto exit;
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    return err;
exit:
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    ql_wifi_uart_deinit(NULL);
    ql_wifi_spi_deinit(NULL);
    ql_wifi_net_deint(NULL);
    ql_wifi_msg_deinit(NULL);
    return err;
}

ql_wifi_app_errcode_e ql_wifi_app_deinit(void *argv)
{
    return QL_WIFI_APP_SUCCESS;
}
