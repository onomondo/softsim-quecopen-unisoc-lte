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
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_api_nw.h"
#include "ql_power.h"

#include "wifi_demo_macro.h"

#if defined QL_WIFI_FCM360W
#define QL_WIFI_DEVICE
#else
#undef QL_WIFI_DEVICE
#endif

#ifdef QL_WIFI_DEVICE
#include "ql_wifi_app.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#endif

/*========================================================================
 *	Macro Definition
 *========================================================================*/

#define QL_WIFI_DEMO_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_DEMO_LOG(msg, ...) QL_LOG(QL_WIFI_DEMO_LOG_LEVEL, "wifi_demo", msg, ##__VA_ARGS__)

#define QL_WIFI_DEMO_MSG_QUEUE_CNT 10

#define QL_WIFI_DEMO_DISCONNECT_RETRY_TIME (20 * 1000)
#define QL_WIFI_DEMO_RETRY_CNT_MAX 3

#define QL_WIFI_DEMO_SIM_ID 0
#define QL_WIFI_DEMO_PROFILE_IDX 5

#define QL_ETHERNET_SIM_NUM 2

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    WIFI_DEMO_RETRY_CONNECT_START = 0,
    WIFI_DEMO_RETRY_CONNECT_END,
} wifi_demo_retry_state_e;

typedef enum
{
    WIFI_DEMO_EVENT_INIT = 0,
    WIFI_DEMO_EVENT_TRY_CONNECT,
    WIFI_DEMO_EVENT_CONNECTED,
    WIFI_DEMO_EVENT_DISCONNECT,
    WIFI_DEMO_EVENT_GET_STATUS,
} wifi_demo_event_e;

/*========================================================================
 *  Type Definition
 *=========================================================================*/
#ifdef QL_WIFI_DEVICE
typedef struct
{
    uint32_t id;
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
} wifi_demo_event_s;
#endif

typedef struct
{
    ql_task_t task;
#ifdef QL_WIFI_DEVICE
    uint8_t sim_id;
    int profile_idx;
    ql_wifi_state_e state;
    ql_queue_t msg_queue;
    ql_timer_t retry_timer;
    wifi_demo_retry_state_e retry_state;
    int retry_cnt;
#endif
} wifi_demo_manager_s;
/*========================================================================
 *   Global Variable
 *========================================================================*/
wifi_demo_manager_s wifi_demo_manager = {0};

#ifdef QL_WIFI_DEVICE

ql_sem_t ql_data_reg_sem[QL_ETHERNET_SIM_NUM] = {0};

#endif

/*========================================================================
 *	function Definition
 *========================================================================*/
#ifdef QL_WIFI_DEVICE
void wifi_demo_send_event(ql_task_t task, uint32_t id, uint32_t param1, uint32_t param2, uint32_t param3)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    ql_queue_t *msg_queue = NULL;
    if (task == manager->task)
    {
        msg_queue = manager->msg_queue;
    }
    else
    {
        return;
    }

    QlOSStatus err = QL_OSI_SUCCESS;
    wifi_demo_event_s event = {.id = id, .param1 = param1, .param2 = param2, .param3 = param3};
    err = ql_rtos_queue_release(msg_queue, sizeof(wifi_demo_event_s), (uint8 *)&event, QL_NO_WAIT);
    if (err)
    {
        QL_WIFI_DEMO_LOG("release failed");
    }
    return;
}

static void wifi_demo_retry_timer_callback(void *ctx)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("timer retry");
    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
    return;
}

static void wifi_demo_connect_status(void* ctx)
{
    uint32_t value = *((uint32_t*)ctx);
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("wifi connect %d", value);
    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_GET_STATUS, value, 0, 0);
    return;
}

void ql_nw_ind_callback(uint8_t sim_id, unsigned int ind_type, void *ind_msg_buf)
{
    if(QUEC_NW_DATA_REG_STATUS_IND == ind_type)
    {
        ql_nw_common_reg_status_info_s  *data_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
        QL_WIFI_DEMO_LOG("Sim%d data reg status changed, current status is %d", sim_id, data_reg_status->state);
        if((QL_NW_REG_STATE_HOME_NETWORK == data_reg_status->state) || (QL_NW_REG_STATE_ROAMING == data_reg_status->state))
        {
            ql_rtos_semaphore_release(ql_data_reg_sem[sim_id]);
        }
    }
}

bool wifi_demo_start_data_call(uint8_t sim_id,int profile_idx)
{
    int ret = 0;
    ql_nw_reg_status_info_s nw_info = {0};

    ql_rtos_semaphore_create(&ql_data_reg_sem[sim_id], 0);

    ret = ql_nw_register_cb(ql_nw_ind_callback);
    if(0 != ret)
    {
        QL_WIFI_DEMO_LOG("ql_nw_register_cb err, ret=0x%x", ret);
        goto exit;
    }

    ret = ql_nw_get_reg_status(sim_id, &nw_info);
    QL_WIFI_DEMO_LOG("ret: 0x%x, current data reg status=%d", ret, nw_info.data_reg.state);
    if((QL_NW_REG_STATE_HOME_NETWORK != nw_info.data_reg.state) && (QL_NW_REG_STATE_ROAMING != nw_info.data_reg.state))   
    {
        ql_rtos_semaphore_wait(ql_data_reg_sem[sim_id], QL_WAIT_FOREVER);  //wait network registered
    }

    /* Start datacall. */
    ret = ql_set_data_call_asyn_mode(sim_id, profile_idx, 0);
    QL_WIFI_DEMO_LOG("mode 0x%x", ret);
    ret = ql_start_data_call(sim_id, profile_idx, QL_PDP_TYPE_IP, NULL, NULL, NULL, 0);
    if(0 != ret)
    {
        QL_WIFI_DEMO_LOG("ql_start_data_call err, ret=0x%x", ret);
        goto exit;
    }
#if 1
    /*Get datacall info. */
    ql_data_call_info_s info ={0};
    ret = ql_get_data_call_info(sim_id, profile_idx, &info);
    QL_WIFI_DEMO_LOG("ql_get_data_call_info ret: 0x%x", ret);
    QL_WIFI_DEMO_LOG("info.profile_idx: %d, info.ip_version: %d", info.profile_idx, info.ip_version);
    QL_WIFI_DEMO_LOG("info.v4.state: %d, info.v6.state: %d", info.v4.state, info.v6.state);
    if(info.v4.state)
    {
        QL_WIFI_DEMO_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info.v4.addr.ip)));
        QL_WIFI_DEMO_LOG("info.v4.addr.pri_dns: %s", ip4addr_ntoa(&(info.v4.addr.pri_dns)));
        QL_WIFI_DEMO_LOG("info.v4.addr.sec_dns: %s", ip4addr_ntoa(&(info.v4.addr.sec_dns)));
    }
#endif
    ret = 0;
exit:
    if(0 != ret)
    {
        return false;
    }
    return true;
}

int wifi_demo_set_ap_param(uint8_t sim_id, int profile_idx)
{
    int ret = 0;
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    char ssid_str[] = {"enp0s11u"};
    char password_str[] = {"l123654789"};

    /* Set AP mode. */
    ql_wifi_mode_e mode = QL_WIFI_MODE_AP;
    ret = ql_wifi_opt_set(QL_WIFI_OPT_MODE, &mode, sizeof(ql_wifi_mode_e));
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("mode set err %x", ret);
        goto exit;
    }
    /* Specify wifi cid. */
    ql_wifi_cid_s ctx =
    {
        .sim_id = sim_id,
        .cid = profile_idx,
        .enable = 0,    /* Muse be 0 in AP mode. */
    };
    ret = ql_wifi_opt_set(QL_WIFI_OPT_CID, &ctx, sizeof(ql_wifi_cid_s));
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("cid set err %x", ret);
        goto exit;
    }
    manager->sim_id = sim_id;
    manager->profile_idx = profile_idx;

    /* Set ssid & password */
    ql_wifi_app_user_ap_info_s ap_user_info = {0};
    int ssid_len = strlen(ssid_str);
    int password_len = strlen(password_str);
    if(ssid_len >= QL_WIFI_SSID_MAX_LEN || password_len >= QL_WIFI_PASSWORD_MAX_LEN)
    {
        goto exit;
    }
    memcpy(&ap_user_info.ssid,ssid_str,ssid_len);
    memcpy(&ap_user_info.password,password_str,password_len);
    ret = ql_wifi_app_set_user_ap_info(&ap_user_info);
    if(ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("get ssid/password faild!");
        goto exit;
    }

    /* Start datacall. */
    if(!wifi_demo_start_data_call(sim_id,profile_idx))
    {
        QL_WIFI_DEMO_LOG("start data call err");
        ret = -1;
        goto exit;
    }
    ret = 0;
exit:
    return ret;
}

#endif

static void wifi_demo_thread(void *arg)
{
#ifdef QL_WIFI_DEVICE
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("wifi demo task start");
    int ret = 0;
    uint8_t sim_id = QL_WIFI_DEMO_SIM_ID;
    int profile_idx = QL_WIFI_DEMO_PROFILE_IDX;

    /* Nat on for AP mode.*/
    uint8_t reset_flag = false;
    uint32_t saved_list = 0;
    uint32_t target_list = (sim_id << 8)|(1 << ((uint8_t)profile_idx - 1));
    ret = ql_datacall_get_nat(&saved_list);
    QL_WIFI_DEMO_LOG("get datacall saved nat ret: 0x%x, %d", ret, saved_list);
    if(0 != ret)
    {
        goto exit;
    }
    if(saved_list != target_list)
    {
        ret = ql_datacall_set_nat(target_list);
        QL_WIFI_DEMO_LOG("ql_datacall_set_nat err, ret=0x%x", ret);
        if(0 != ret)
        {
            goto exit;
        }
        /* NAT function reboot to take effect. */
        reset_flag = true;
    }
    if(reset_flag)
    {
        ql_power_reset(RESET_NORMAL);
    }

    /* Create timer. */
    ret = ql_rtos_timer_create(&(manager->retry_timer), NULL, wifi_demo_retry_timer_callback, NULL);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("create timer ret: %x", ret);
        goto exit;
    }

    /* Create message queue */
    ret = ql_rtos_queue_create(&(manager->msg_queue), sizeof(wifi_demo_event_s), QL_WIFI_DEMO_MSG_QUEUE_CNT);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("msg queue ret: %x", ret);
        goto exit;
    }

    /* Wifi init. */
    ret = ql_wifi_app_init(NULL);
    if (ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("ql_wifi_app_init err %x", ret);
        goto exit;
    }

    if(0 != wifi_demo_set_ap_param(sim_id,profile_idx))
    {
        QL_WIFI_DEMO_LOG("set ap param err");
        goto exit;
    }

    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_INIT, 0, 0, 0);

    while (1)
    {
        wifi_demo_event_s event = {0};
        if (QL_OSI_SUCCESS != ql_rtos_queue_wait(manager->msg_queue, (uint8 *)&event, sizeof(wifi_demo_event_s), QL_WAIT_FOREVER))
        {
            QL_WIFI_DEMO_LOG("queue wait error");
            goto exit;
        }
        QL_WIFI_DEMO_LOG("wifi_demo_thread %x", event.id);
        switch (event.id)
        {
        case WIFI_DEMO_EVENT_INIT: {
            /* Wifi software reset,better add hardware reset. */
            ret = ql_wifi_app_reset(NULL);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("reset err %x", ret);
                goto exit;
            }
            ql_rtos_task_sleep_ms(5000);
            QL_WIFI_DEMO_LOG("wifi init");
            wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
            break;
        }
        case WIFI_DEMO_EVENT_TRY_CONNECT: {
            manager->retry_cnt++;
            if (manager->retry_cnt >= QL_WIFI_DEMO_RETRY_CNT_MAX)
            {
                manager->retry_cnt = 0;
                wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_INIT, 0, 0, 0);
                break;
            }
            /* Start timer. */
            ret = ql_rtos_timer_start(manager->retry_timer, QL_WIFI_DEMO_DISCONNECT_RETRY_TIME, 0);
            QL_WIFI_DEMO_LOG("timer start %x", ret);
            if (ret != QL_OSI_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("manager->retry_timer start failed");
                wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
            }

            QL_WIFI_DEMO_LOG("wifi connect start");
            /* start to connect wifi. */
            ret = ql_wifi_app_ap_disable(NULL);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("ap disable err %x", ret);
                goto exit;
            }
            ql_rtos_task_sleep_ms(10);
            ret = ql_wifi_app_ap_enable(wifi_demo_connect_status);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("ap enable err %x", ret);
                goto exit;
            }
            QL_WIFI_DEMO_LOG("ql_wifi_app_ap_enable done");
            /* 
                Set retry_state end.When we get disconnect status,
                we check this state for ignoring to send WIFI_DEMO_EVENT_TRY_CONNECT again.
            */
            manager->retry_state = WIFI_DEMO_RETRY_CONNECT_END;
            break;
        }
        case WIFI_DEMO_EVENT_CONNECTED: {
            ql_data_call_info_s info = {0};
            ret = ql_get_data_call_info(sim_id, profile_idx, &info);
            QL_WIFI_DEMO_LOG("ql_get_data_call_info ret: 0x%x", ret);
            QL_WIFI_DEMO_LOG("info.profile_idx: %d, info.ip_version: %d", info.profile_idx, info.ip_version);
            QL_WIFI_DEMO_LOG("info->v4.state: %d, info.v6.state: %d", info.v4.state, info.v6.state);
            if (info.v4.state)
            {
                QL_WIFI_DEMO_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info.v4.addr.ip)));
                QL_WIFI_DEMO_LOG("info.v4.addr.pri_dns: %s", ip4addr_ntoa(&(info.v4.addr.pri_dns)));
                QL_WIFI_DEMO_LOG("info.v4.addr.sec_dns: %s", ip4addr_ntoa(&(info.v4.addr.sec_dns)));
            }
            break;
        }
        case WIFI_DEMO_EVENT_GET_STATUS: {
            uint32_t value = event.param1;
            if (value == QL_WIFI_STATE_CONNECTED)
            {
                manager->retry_state = WIFI_DEMO_RETRY_CONNECT_END;
                ql_rtos_timer_stop(manager->retry_timer);
                //From others to QL_WIFI_STATE_CONNECTED
                if(manager->state != QL_WIFI_STATE_CONNECTED)
                {
                    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_CONNECTED, 0, 0, 0);
                }
            }
            else if (value == QL_WIFI_STATE_DISCONNECT)
            {
                if (manager->retry_state == WIFI_DEMO_RETRY_CONNECT_END)
                {
                    manager->retry_state = WIFI_DEMO_RETRY_CONNECT_START;
                    ql_rtos_timer_stop(manager->retry_timer);
                    int ret = 0;
                    ret = ql_rtos_timer_start(manager->retry_timer, QL_WIFI_DEMO_DISCONNECT_RETRY_TIME, 0);
                    QL_WIFI_DEMO_LOG("timer start %x", ret);
                    if (ret != QL_OSI_SUCCESS)
                    {
                        QL_WIFI_DEMO_LOG("manager->retry_timer start failed");
                        wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
                    }
                }
            }
            manager->state = value;
            manager->retry_cnt = 0;
            break;
        }
        default: {
            break;
        }
        }
    }
exit:
#endif
    QL_WIFI_DEMO_LOG("wifi demo task end");
    ql_rtos_task_delete(NULL);
}

void ql_wifi_demo_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    err = ql_rtos_task_create(&(manager->task), 4 * 1024, APP_PRIORITY_NORMAL, "wifi_demo", wifi_demo_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("task created failed");
        return;
    }
}
