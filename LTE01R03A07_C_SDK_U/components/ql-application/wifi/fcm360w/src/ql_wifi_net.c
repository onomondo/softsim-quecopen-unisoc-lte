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
#include "ql_api_wifi.h"
#include "ql_api_osi.h"

#include "lwip/prot/ethernet.h"
#include "ql_wifi_adapter.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#include "ql_wifi_app.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_NET_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_net", msg, ##__VA_ARGS__)

#define QL_WIFI_NET_TASK_EVENT_CNT 5
#define QL_WIFI_NET_MSG_QUEUE_CNT 128

#define QL_WIFI_NET_TASK_QUEUE_WAIT 2000
#define QL_WIFI_NET_TASK_DEINIT_WAIT 5000

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_NET_TASK_TX,
    QL_WIFI_NET_TASK_RX,
    QL_WIFI_NET_TASK_MAX,
} ql_wifi_net_task_type_e;
/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t task;
    ql_queue_t queue;
    ql_sem_t task_delete_sem;
} ql_wifi_net_task_s;

typedef struct
{
    ql_wifi_net_task_s task[QL_WIFI_NET_TASK_MAX];
} ql_wifi_net_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_net_manager_s ql_wifi_net_manager = {0};
ql_mutex_t ql_wifi_net_mutex = NULL;

ql_wifi_net_ip_info_s net_ip_info = {0};
/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_net_errcode_e ql_wifi_net_recv(void *argv)
{
    ql_wifi_net_task_s *task = &ql_wifi_net_manager.task[QL_WIFI_NET_TASK_RX];
    ql_wifi_net_s *msg = (ql_wifi_net_s *)argv;
    ql_wifi_net_errcode_e ret = QL_WIFI_NET_SUCCESS;
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);

    if (QL_OSI_SUCCESS != ql_rtos_queue_release(task->queue, sizeof(ql_wifi_net_s), (uint8 *)msg, QL_NO_WAIT))
    {
        QL_WIFI_NET_LOG("release msg_queue failed");
        ret = QL_WIFI_NET_EXECUTE_ERR;
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    return ret;
}

static void ql_wifi_net_state_notify(void *ctx)
{
    ql_wifi_status_notify_ctx_s *p = (ql_wifi_status_notify_ctx_s*)ctx;
    ql_wifi_state_e state = p->state;

    ql_wifi_app_s msg = {.msg.msgType = state};
    if(QL_WIFI_MODE_STA == p->interface.mode)
    {
        msg.type = QL_WIFI_APP_STA_STATUS_CMD;
    }
    else
    {
        msg.type = QL_WIFI_APP_AP_STATUS_CMD;
    }
    ql_wifi_app_recv(&msg);
}

ql_wifi_net_ip_info_s *ql_wifi_net_get_ip_info_ptr()
{
    return &net_ip_info;
}

int ql_wifi_data_output(void *ctx)
{
    struct pbuf *p = (struct pbuf *)ctx;
    uint32_t padding = 2;
    uint32_t data_len = p->tot_len;
    uint8_t *data = (uint8_t *)calloc(1, data_len + padding);
    
    ql_wifi_app_errcode_e ret =QL_WIFI_NET_SUCCESS ;
    ql_wifi_mode_e mode = {0};
    uint8_t id;
        
    // Get the current Wi-Fi mode using ql_wifi_app_get_interface
    ret = ql_wifi_app_get_interface(&mode, &id);
    QL_WIFI_NET_LOG("mode = %d", mode );
    if (ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_NET_LOG("Failed to get Wi-Fi mode, error: 0x%x", ret);
        return -1;
    }
    
    if (!data)
    {
        return -1;
    }
    uint8_t *pData = data + padding;
    memcpy(pData,  p->payload, data_len);
    if (QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_data_send(data, data_len + padding, mode))
    {
        free(data);
        return -1;
    }
    return 0;
}
bool ql_wifi_net_check_if_id_used(ql_wifi_mode_e mode, uint8_t id)
{
    ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
    for (int i = 0; i < QL_WIFI_NET_IP_INFO_MAX; i++)
    {
        if (1 == p->info[i].is_used && mode == p->info[i].interface.mode && id == p->info[i].interface.id)
        {
            return true;
        }
    }
    return false;
}

ql_wifi_net_errcode_e ql_wifi_net_get_no_use_ip_info(uint8_t *index)
{
    if (!index)
    {
        return QL_WIFI_NET_INVALID_PARAM_ERR;
    }
    ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
    for (int i = 0; i < QL_WIFI_NET_IP_INFO_MAX; i++)
    {
        /* Find no use info. */
        if (0 == p->info[i].is_used)
        {
            *index = i;
            QL_WIFI_NET_LOG("wifi net ip info get suc1 %d", i);
            return QL_WIFI_NET_SUCCESS;
        }
    }
    return QL_WIFI_NET_EXECUTE_ERR;
}

ql_wifi_net_errcode_e ql_wifi_net_get_ip_info_index_by_mode_id(ql_wifi_mode_e mode, uint8_t id, uint8_t *index)
{
    if (!index)
    {
        return QL_WIFI_NET_INVALID_PARAM_ERR;
    }
    ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
    for (int i = 0; i < QL_WIFI_NET_IP_INFO_MAX; i++)
    {
        /* Find info. */
        if (mode == p->info[i].interface.mode && id == p->info[i].interface.id && 1 == p->info[i].is_used)
        {
            *index = i;
            QL_WIFI_NET_LOG("wifi net ip info get suc2 %d", i);
            return QL_WIFI_NET_SUCCESS;
        }
    }
    return QL_WIFI_NET_EXECUTE_ERR;
}

ql_wifi_net_errcode_e ql_wifi_net_get_ip_info_index_by_mac_ip(uint8_t mac[ETH_HWADDR_LEN], ip4_addr_t ip4, uint8_t *index)
{
    if (!index)
    {
        return QL_WIFI_NET_INVALID_PARAM_ERR;
    }
    ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
    uint8_t if_mac[6] = {0};
    ip4_addr_t if_ip = {0};
    int val_len = 0;
    for (int i = 0; i < QL_WIFI_NET_IP_INFO_MAX; i++)
    {
        if (p->info[i].is_used)
        {
            ql_wifi_app_specify_interface(p->info[i].interface.mode, p->info[i].interface.id);
            val_len = ETH_HWADDR_LEN;
            if (QL_WIFI_SUCCESS != ql_wifi_opt_get(QL_WIFI_OPT_MAC, if_mac, &val_len))
            {
                continue;
            }
            val_len = sizeof(ip4_addr_t);
            if (QL_WIFI_SUCCESS != ql_wifi_opt_get(QL_WIFI_OPT_IP, &if_ip, &val_len))
            {
                continue;
            }
            if (0 == memcmp(if_mac, mac, ETH_HWADDR_LEN) && 0 == memcmp(&if_ip, &ip4, sizeof(ip4_addr_t)))
            {
                *index = i;
                QL_WIFI_NET_LOG("wifi net ip info get suc2 %d/%02x%02x%02x%02x%02x%02x/%04x", i,
                                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ip4.addr);
                return QL_WIFI_NET_SUCCESS;
            }
        }
    }
    return QL_WIFI_NET_EXECUTE_ERR;
}

ql_wifi_net_errcode_e ql_wifi_net_create(ql_wifi_mode_e mode, ql_wifi_msg_recv_info_s *info, ql_wifi_msg_tag_e tag)
{

    int ret = QL_WIFI_MSG_SUCCESS;
    int val_len = 0;
    if (!info)
    {
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    QL_WIFI_NET_LOG("wifi net create info %d",mode);
    QL_WIFI_NET_LOG("wifi net user:%s", info->user_info.ssid);
    QL_WIFI_NET_LOG("wifi net pwd:%s", info->user_info.password);
    QL_WIFI_NET_LOG("wifi net channel:%d", info->channel);
    QL_WIFI_NET_LOG("wifi net auth:%d", info->auth_mode);
    QL_WIFI_NET_LOG("wifi net cur clients:%d", info->cur_clients);
    QL_WIFI_NET_LOG("wifi net max clients:%d", info->max_clients);
    QL_WIFI_NET_LOG("wifi net ip:%s", ip4addr_ntoa(&info->ip_info.ip));
    QL_WIFI_NET_LOG("wifi net netmask:%s", ip4addr_ntoa(&info->ip_info.netmask));
    QL_WIFI_NET_LOG("wifi net gw:%s", ip4addr_ntoa(&info->ip_info.gw));
    QL_WIFI_NET_LOG("wifi net dns1:%s", ip4addr_ntoa(&info->ip_info.dns1));
    QL_WIFI_NET_LOG("wifi net dns2:%s", ip4addr_ntoa(&info->ip_info.dns2));
    QL_WIFI_NET_LOG("wifi net mac:%02x %02x %02x %02x %02x %02x", info->ip_info.mac[0], info->ip_info.mac[1], info->ip_info.mac[2], info->ip_info.mac[3], info->ip_info.mac[4], info->ip_info.mac[5]);
    QL_WIFI_NET_LOG("wifi net reason code: %d", info->reason_code);
    uint8_t id = 0;

    /* Check if net exist. */
    ql_wifi_id_s id_ctx = {0};
    id_ctx.mode = mode;
    id_ctx.type = QL_WIFI_TYPE_MAC;
    id_ctx.ctx = (void *)info->ip_info.mac;
    val_len = sizeof(ql_wifi_id_s);
    ret = ql_wifi_opt_get(QL_WIFI_OPT_ID_BY_TYPE, &id_ctx, &val_len);
    if (ret == QL_WIFI_SUCCESS)
    {
        QL_WIFI_NET_LOG("wifi net exist,id %d,do nothing", id_ctx.id);
        ret = QL_WIFI_NET_SUCCESS;
        goto exit;
    }

    uint8_t num_max = 0;
    if (QL_WIFI_MSG_TAG_RECV_AP_CREATED == tag)
    {
        id = 0;
        num_max = QL_WIFI_APP_AP_STA_CLIENTS_MAX;
    }
    else if (QL_WIFI_MSG_TAG_REVC_AP_CLIENT_CONNECTED == tag)
    {
        for (int i = 0; i < 255; i++)
        {
            if (false == ql_wifi_net_check_if_id_used(QL_WIFI_MODE_AP, i))
            {
                id = i;
                break;
            }
        }
        if(0 == id)
        {
            QL_WIFI_NET_LOG("wifi net no id");
            ret = QL_WIFI_NET_EXECUTE_ERR;
            goto exit;
        }
        num_max = QL_WIFI_APP_AP_STA_CLIENTS_MAX;
    }
    else if(QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_ACTIVE == tag || QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_PASSIVE == tag)
    {
        id = 0;
        num_max = 1;
    }
    else
    {
        QL_WIFI_NET_LOG("tag err");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    uint8_t index = 0;
    if (QL_WIFI_NET_SUCCESS != ql_wifi_net_get_no_use_ip_info(&index))
    {
        QL_WIFI_NET_LOG("wifi net get no use ip info err");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    /* Set IP parameter. The p->num is only updated when netcard is created successfully. */
    ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
    if (p->using_num >= num_max)
    {
        QL_WIFI_NET_LOG("wifi net max using num %d/%d", p->using_num, num_max);
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    p->using_num++;
    p->info[index].is_used = 1;
    p->info[index].interface.mode = mode;
    p->info[index].interface.id = id;
    memcpy(&p->info[index].ip_info, &info->ip_info, sizeof(ql_wifi_msg_ip_info_s));

    /* Specify mode and id to determine unique interface. */
    ql_wifi_app_specify_interface(mode, id);

    /* Set static ip. */
    ip4_addr_t ip = info->ip_info.ip;
    ip4_addr_t netmask = info->ip_info.netmask;
    ip4_addr_t gw = info->ip_info.gw;

    val_len = sizeof(ip4_addr_t);
    ret = ql_wifi_opt_set(QL_WIFI_OPT_IP, &ip, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_NET_LOG("set ip faild!");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Set static netmask. */
    val_len = sizeof(ip4_addr_t);
    ret = ql_wifi_opt_set(QL_WIFI_OPT_NETMASK, &netmask, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_NET_LOG("set netmask faild!");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Set static gateway. */
    val_len = sizeof(ip4_addr_t);
    ret = ql_wifi_opt_set(QL_WIFI_OPT_GW, &gw, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_NET_LOG("set gw faild!");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Set dns. */
    val_len = sizeof(ql_wifi_dns_s);
    ql_wifi_dns_s dns = {
        .dns_server[LWIP_NETIF_IPV4_ADDR][0].u_addr.ip4 = info->ip_info.dns1,
        .dns_server[LWIP_NETIF_IPV4_ADDR][1].u_addr.ip4 = info->ip_info.dns2,
    };
    ret = ql_wifi_opt_set(QL_WIFI_OPT_DNS, &dns, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_NET_LOG("set dns faild!");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Notify wifi net. */
    ql_wifi_net_s msg = {.type = QL_WIFI_NPTO_WLAN_CREATE_CMD};
    msg.msg.msgValue = sizeof(info->ip_info.mac);
    msg.msg.msgAddr = (uint8_t *)calloc(1, msg.msg.msgValue);
    if (!msg.msg.msgAddr)
    {
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    /* Copy mode and id*/
    msg.msg.msgType = (((uint32_t)mode & 0x0000FFFF) << 16) | ((uint32_t)id & 0x0000FFFF);
    /* Copy MAC. */
    memcpy(msg.msg.msgAddr, info->ip_info.mac, msg.msg.msgValue);
    ql_wifi_net_recv(&msg);
exit:
    if (QL_WIFI_NET_SUCCESS != ret)
    {
        p->info[index].is_used = 0;
    }
    return ret;
}

ql_wifi_net_errcode_e ql_wifi_net_destroy(ql_wifi_mode_e mode, ql_wifi_msg_recv_info_s *info, ql_wifi_msg_tag_e tag)
{

    int ret = QL_WIFI_MSG_SUCCESS;
    int val_len = 0;
    if (!info)
    {
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    QL_WIFI_NET_LOG("wifi net destroy info");
    QL_WIFI_NET_LOG("wifi net user:%s", info->user_info.ssid);
    QL_WIFI_NET_LOG("wifi net pwd:%s", info->user_info.password);
    QL_WIFI_NET_LOG("wifi net channel:%d", info->channel);
    QL_WIFI_NET_LOG("wifi net auth:%d", info->auth_mode);
    QL_WIFI_NET_LOG("wifi net cur clients:%d", info->cur_clients);
    QL_WIFI_NET_LOG("wifi net max clients:%d", info->max_clients);
    QL_WIFI_NET_LOG("wifi net ip:%s", ip4addr_ntoa(&info->ip_info.ip));
    QL_WIFI_NET_LOG("wifi net netmask:%s", ip4addr_ntoa(&info->ip_info.netmask));
    QL_WIFI_NET_LOG("wifi net gw:%s", ip4addr_ntoa(&info->ip_info.gw));
    QL_WIFI_NET_LOG("wifi net dns1:%s", ip4addr_ntoa(&info->ip_info.dns1));
    QL_WIFI_NET_LOG("wifi net dns2:%s", ip4addr_ntoa(&info->ip_info.dns2));
    QL_WIFI_NET_LOG("wifi net mac:%02x %02x %02x %02x %02x %02x", info->ip_info.mac[0], info->ip_info.mac[1], info->ip_info.mac[2], info->ip_info.mac[3], info->ip_info.mac[4], info->ip_info.mac[5]);
    QL_WIFI_NET_LOG("wifi net reason code: %d", info->reason_code);
    if (QL_WIFI_MSG_TAG_REVC_AP_CLIENT_DISCONNECTED != tag 
    && QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_ACTIVE != tag
    && QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_PASSIVE != tag)
    {
        QL_WIFI_NET_LOG("tag err");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Check if interface is exist. */
    val_len = sizeof(ql_wifi_id_s);
    ql_wifi_id_s id = {.type = QL_WIFI_TYPE_MAC, .ctx = info->ip_info.mac, .mode = mode};
    if (QL_WIFI_SUCCESS != (ret = ql_wifi_opt_get(QL_WIFI_OPT_ID_BY_TYPE, &id, &val_len)))
    {
        QL_WIFI_NET_LOG("id get err");
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }

    /* Notify wifi net. */
    ql_wifi_net_s msg = {.type = QL_WIFI_NPTO_WLAN_DESTROY_CMD};
    msg.msg.msgValue = sizeof(info->ip_info.mac);
    msg.msg.msgAddr = (uint8_t *)calloc(1, msg.msg.msgValue);
    if (!msg.msg.msgAddr)
    {
        ret = QL_WIFI_NET_EXECUTE_ERR;
        goto exit;
    }
    /* Copy mode and id*/
    msg.msg.msgType = (((uint32_t)mode & 0x0000FFFF) << 16) | ((uint32_t)id.id & 0x0000FFFF);
    /* Copy MAC. */
    memcpy(msg.msg.msgAddr, info->ip_info.mac, msg.msg.msgValue);
    ql_wifi_net_recv(&msg);
exit:
    return ret;
}

void ql_wifi_net_delete_task(ql_wifi_net_task_s *task)
{
    if (!task)
    {
        return;
    }
    ql_wifi_net_s msg = {0};
    int ret = 0;
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    ql_rtos_semaphore_release(task->task_delete_sem);
    ql_rtos_semaphore_delete(task->task_delete_sem);
    uint32 cnt = 0;
    ql_rtos_queue_get_cnt(task->queue, &cnt);
    QL_WIFI_NET_LOG("del msg queue cnt %d/%p", cnt, task);
    while (0 < cnt)
    {
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_net_s), QL_WAIT_FOREVER);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_NET_LOG("wait error err= %d", ret);
            break;
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        cnt--;
        QL_WIFI_NET_LOG("remain msg %d", cnt);
    }
    ql_rtos_queue_delete(task->queue);
    memset(task, 0, sizeof(ql_wifi_net_task_s));
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    ql_rtos_task_delete(NULL);
}

void ql_wifi_net_rx_thread(void *argv)
{
    ql_wifi_net_task_s *task = (ql_wifi_net_task_s *)argv;
    QlOSStatus ret = QL_OSI_SUCCESS;
    ql_wifi_net_s msg = {0};
    QL_WIFI_NET_LOG("wifi net rx task start");

    /* Register notify cb.*/
    if (QL_WIFI_SUCCESS != ql_wifi_status_notify_cb_register(ql_wifi_net_state_notify))
    {
        QL_WIFI_NET_LOG("wifi status notify cb reg err");
        goto exit;
    }

    if (QL_WIFI_SUCCESS != ql_wifi_linkoutput_cb_register(ql_wifi_data_output))
    {
        QL_WIFI_NET_LOG("wifi output cb reg err");
        goto exit;
    }

    while (1)
    {
        /* Sem take. */
        ret = ql_rtos_semaphore_wait(task->task_delete_sem, QL_NO_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_NET_LOG("rx deleting");
            /*User called ql_wifi_net_deinit and took the sem. */
            break;
        }
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_net_s), QL_WIFI_NET_TASK_QUEUE_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            if (QL_OSI_TASK_PARAM_INVALID == ret)
            {
                break;
            }
            ql_rtos_semaphore_release(task->task_delete_sem);
            continue;
        }

        /* It should specify the interface before handle the event. */
        ql_wifi_mode_e mode = (ql_wifi_mode_e)((msg.msg.msgType >> 16) & 0x0000FFFF);
        uint8_t id = (uint8_t)(msg.msg.msgType & 0x0000FFFF);
        ql_wifi_app_specify_interface(mode, id);

        //QL_WIFI_NET_LOG("wifi net event 0x%x/%d/%d", msg.type, mode, id);

        switch (msg.type)
        {
        case QL_WIFI_NPTO_802_3_STREAM: {
            ql_wifi_data_input(msg.msg.msgAddr, msg.msg.msgValue);
            break;
        }
        case QL_WIFI_NPTO_STA_IP_START_CMD: {
            ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
            uint8_t index = 0;
            if (QL_WIFI_NET_SUCCESS != ql_wifi_net_get_ip_info_index_by_mode_id(mode, id, &index))
            {
                QL_WIFI_NET_LOG("no ip info index");
                break;
            }
            if (QL_WIFI_SUCCESS == ql_wifi_sta_ip_start(NULL))
            {
                /* IP start suc.Update record. */
                p->num = (QL_WIFI_APP_AP_STA_CLIENTS_MAX < p->num) ? p->num : p->num + 1;
            }
            else
            {
                /* IP start err.Release soucres. */
                p->info[index].is_used = 0;
                p->using_num = (0 == p->using_num) ? 0 : p->using_num - 1;
            }
            break;
        }
        case QL_WIFI_NPTO_STA_IP_DOWN_CMD: {
            ql_wifi_sta_ip_down(NULL);
            break;
        }
        case QL_WIFI_NPTO_UAP_IP_START_CMD: {
            ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
            uint8_t index = 0;
            if (QL_WIFI_NET_SUCCESS != ql_wifi_net_get_ip_info_index_by_mode_id(mode, id, &index))
            {
                QL_WIFI_NET_LOG("no ip info index");
                break;
            }

            if (QL_WIFI_SUCCESS == ql_wifi_uap_ip_start(NULL))
            {
                /* IP start suc.Update record. */
                p->num = (QL_WIFI_APP_AP_STA_CLIENTS_MAX < p->num) ? p->num : p->num + 1;
            }
            else
            {
                /* IP start err.Release soucres. */
                p->info[index].is_used = 0;
                p->using_num = (0 == p->using_num) ? 0 : p->using_num - 1;
            }
            break;
        }
        case QL_WIFI_NPTO_UAP_IP_DOWN_CMD: {
            ql_wifi_uap_ip_down(NULL);
            break;
        }
        case QL_WIFI_NPTO_WLAN_CREATE_CMD: {
            /* Disable Dhcp according to this driver. */
            char dhcp = 0;
            ret = ql_wifi_opt_set(QL_WIFI_OPT_DHCP, &dhcp, sizeof(char));
            if (ret != QL_WIFI_SUCCESS)
            {
                QL_WIFI_NET_LOG("dhcp set err %x", ret);
                break;
            }

            uint8_t *mac = msg.msg.msgAddr;
            QL_WIFI_NET_LOG("wifi net %02x%02x%02x%02x%02x%02x",
                            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            int ret = ql_wifi_wlan_create((void *)mac);
            if (QL_WIFI_SUCCESS == ret || QL_WIFI_REPEAT_CREATED_ERR == ret)
            {
                ql_wifi_net_s p = {.msg.msgType = msg.msg.msgType};
                p.type = QL_WIFI_MODE_STA == mode ? QL_WIFI_NPTO_STA_IP_START_CMD : QL_WIFI_NPTO_UAP_IP_START_CMD;
                ql_wifi_net_recv(&p);
            }
            break;
        }
        case QL_WIFI_NPTO_WLAN_DESTROY_CMD: {
            uint8_t *mac = msg.msg.msgAddr;
            QL_WIFI_NET_LOG("wifi net %02x%02x%02x%02x%02x%02x",
                            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

            ql_wifi_net_ip_info_s *p = ql_wifi_net_get_ip_info_ptr();
            uint8_t index = 0;
            if (QL_WIFI_NET_SUCCESS != ql_wifi_net_get_ip_info_index_by_mode_id(mode, id, &index))
            {
                QL_WIFI_NET_LOG("no ip info index");
                break;
            }

            if (QL_WIFI_SUCCESS == ql_wifi_wlan_destroy((void *)mac))
            {
                p->info[index].is_used = 0;
                p->using_num = (0 == p->using_num) ? 0 : p->using_num - 1;
                p->num = (0 == p->num) ? 0 : p->num - 1;
            }
            break;
        }
        case QL_WIFI_NPTO_IP_ADDR_SET_CMD:
        default: {
            break;
        }
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        ret = ql_rtos_semaphore_release(task->task_delete_sem);
    }
exit:
    QL_WIFI_NET_LOG("wifi net rx task end");
    ql_wifi_net_delete_task(task);
}

ql_wifi_net_errcode_e ql_wifi_net_task_create(ql_wifi_net_task_type_e type)
{
    ql_wifi_net_errcode_e ret = QL_WIFI_NET_SUCCESS;
    if (type >= QL_WIFI_NET_TASK_MAX)
    {
        return QL_WIFI_NET_INVALID_PARAM_ERR;
    }
    ql_wifi_net_task_s *task = &ql_wifi_net_manager.task[type];
    /* Create task delete sem. */
    if (QL_OSI_SUCCESS != ql_rtos_semaphore_create(&(task->task_delete_sem), 1))
    {
        ret = QL_WIFI_NET_SEM_CREATE_ERR;
        goto exit;
    }
    /* Create msg queue. */
    if (QL_OSI_SUCCESS != ql_rtos_queue_create(&task->queue, sizeof(ql_wifi_net_s), QL_WIFI_NET_MSG_QUEUE_CNT))
    {
        ret = QL_WIFI_NET_MSG_QUEUE_CREATE_ERR;
        goto exit;
    }
    /* Create msg task. */

    char name[][20] = {"ql_wifi_net_tx", "ql_wifi_net_rx"};

    void (*thread[])(void *) = {NULL, ql_wifi_net_rx_thread};

    if (QL_OSI_SUCCESS != ql_rtos_task_create(&task->task, 4096, APP_PRIORITY_REALTIME, (char *)name[type], thread[type], task, QL_WIFI_NET_TASK_EVENT_CNT))
    {
        QL_WIFI_NET_LOG("%s create err", name[type]);
        ret = QL_WIFI_NET_TASK_CREATE_ERR;
        goto exit;
    }
exit:
    if (QL_WIFI_NET_SUCCESS != ret)
    {
        if (task->task_delete_sem)
        {
            ql_rtos_semaphore_delete(task->task_delete_sem);
        }
        if (task->queue)
        {
            ql_rtos_queue_delete(task->queue);
        }
        memset(task, 0, sizeof(ql_wifi_net_task_s));
    }
    return ret;
}

ql_wifi_net_errcode_e ql_wifi_net_init(void *argv)
{
    ql_wifi_net_task_s *rx_task = &ql_wifi_net_manager.task[QL_WIFI_NET_TASK_RX];
    ql_wifi_net_errcode_e ret = QL_WIFI_NET_SUCCESS;
    /* Create mutex.Once mutex has been created,it will never delete for safety(e.g. Deleting task while sending msg queue). */
    if (NULL == ql_wifi_net_mutex)
    {
        if (QL_OSI_SUCCESS != ql_rtos_mutex_create(&(ql_wifi_net_mutex)))
        {
            return QL_WIFI_NET_MUTEX_CREATE_ERR;
        }
    }
    if (rx_task->task)
    {
        goto exit;
    }
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    /* Create RX task. */
    if (QL_WIFI_NET_SUCCESS != (ret = ql_wifi_net_task_create(QL_WIFI_NET_TASK_RX)))
    {
        ql_rtos_mutex_unlock(ql_wifi_net_mutex);
        ret = QL_WIFI_NET_TASK_CREATE_ERR;
        goto exit;
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
exit:
    if (QL_WIFI_NET_SUCCESS != ret)
    {
        ql_wifi_net_deinit(NULL);
    }
    return ret;
}

ql_wifi_net_errcode_e ql_wifi_net_deinit(void *argv)
{
    ql_wifi_net_task_s *rx_task = &ql_wifi_net_manager.task[QL_WIFI_NET_TASK_RX];
    int ret = 0;
    /* All tasks have been deleted if both tasks are NULL. */
    if (!rx_task->task)
    {
        return QL_WIFI_NET_SUCCESS;
    }
    ql_rtos_mutex_try_lock(ql_wifi_net_mutex);
    /* Take sem. */
    if (QL_WIFI_SUCCESS != (ret = ql_rtos_semaphore_wait(rx_task->task_delete_sem, QL_WIFI_NET_TASK_DEINIT_WAIT)))
    {
        QL_WIFI_NET_LOG("wait rx sem %x", ret);
    }
    ql_rtos_mutex_unlock(ql_wifi_net_mutex);
    return QL_WIFI_NET_SUCCESS;
}
