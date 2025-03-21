/**  @file
  bt_a2dp_demo.c

  @brief
  This file is demo of bt a2dp.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
Note: Don't delete BT firmware from the file system
=================================================================*/

/*===========================================================================
 * include files
 ===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_api_bt.h"
#include "ql_api_bt_a2dp.h"
#include "ql_api_bt_avrcp.h"
#include "ql_log.h"
#include "bt_a2dp_avrcp_demo.h"
#include "ql_audio.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/
 
#define QL_A2DP_AVRCP_DEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_A2DP_AVRCP_DEMO_LOG(msg, ...)         QL_LOG(QL_A2DP_AVRCP_DEMO_LOG_LEVEL, "QL_A2DP_AVRCP_DEMO", msg, ##__VA_ARGS__)
#define QL_A2DP_AVRCP_DEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("QL_A2DP_AVRCP_DEMO", msg, ##__VA_ARGS__)

#define QL_A2DP_AVRCP_TASK_STACK_SIZE     		(4*1024)
#define QL_A2DP_AVRCP_TASK_PRIO          	 	APP_PRIORITY_NORMAL
#define QL_A2DP_AVRCP_TASK_EVENT_CNT      		5


/*===========================================================================
 * Struct
 ===========================================================================*/

/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QUEC_BT_A2DP_CONNECT = 0,
    QUEC_BT_A2DP_DISCONNECT,
    QUEC_BT_A2DP_AUDIO_CONFIG,
    QUEC_BT_A2DP_AUDIO_STATE_START,
    QUEC_BT_A2DP_AUDIO_STATE_STOPPED,
    QUEC_BT_AVRCP_CONNECT,
    QUEC_BT_AVRCP_DISCONNECT,
    QUEC_BT_AVRCP_VOLUME_CHANGE,
    QUEC_BT_A2DP_AVRCP_MAX,
}ql_bt_a2dp_avrcp_info_e;

/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t bt_a2dp_avrcp_task = NULL;

static const char * ql_bt_a2dp_avrcp_text[] = 
{
    "+QBTA2DPSCON: ",
    "+QBTA2DPSDCON: ",
    "+QBTA2DPAUDIOCFG: ",
    "+QBTA2DPAUDIOSTART: ",
    "+QBTA2DPAUDIOSTOP: ",
    "+QBTAVRCPSCON: ",
    "+QBTAVRCPSDCON: ",
    "+QBTAVRCPVOL: ",
};
/*===========================================================================
 * Functions
 ===========================================================================*/

static ql_errcode_bt_e ql_a2dp_avrcp_demo_set_scanmode(void)
{
    ql_errcode_bt_e ret;
    
    ql_bt_visible_mode_e scan_mode = QL_BT_PAGE_AND_INQUIRY_ENABLE;
    ret = ql_classic_bt_set_scanmode(scan_mode);
    if (ret == QL_BT_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("sucess");
    }
    else 
    {
        QL_A2DP_AVRCP_DEMO_LOG("error=%x", ret);
    }

    return ret;
}

static ql_errcode_bt_e ql_a2dp_avrcp_demo_set_name(void)
{
    ql_errcode_bt_e ret;
    
    ql_bt_ble_local_name_s bt_name = 
    {
        .name = "QL_A2DP_AVRCP_DEMO",
        .code_type = QL_BT_BLE_NAME_CODE_UTF8,
    };

    ret = ql_bt_ble_set_localname(bt_name);
    if (ret == QL_BT_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("sucess");
    }
    else 
    {
        QL_A2DP_AVRCP_DEMO_LOG("error=%x", ret);
    }

    return ret;
}

static void ql_demo_bt_a2dp_avrcp_handle(ql_bt_a2dp_avrcp_handle_e protocol_handle, ql_bt_a2dp_avrcp_info_e index, ql_event_t *event, char state_flag)
{
    char string[256] = {0};
    if (index >= QUEC_BT_A2DP_AVRCP_MAX)
    {
        QL_A2DP_AVRCP_DEMO_LOG("index error=%d", index);
        return ;
    }

    const char *at_text = ql_bt_a2dp_avrcp_text[index];
    
    if(protocol_handle == QL_A2DP_HANDLE)
    {
        //avrcp_info point to a malloc address,must be freed
        ql_bt_a2dp_event_info_t *a2dp_info = (ql_bt_a2dp_event_info_t *)event->param2;
        if (a2dp_info)
        {
            QL_A2DP_AVRCP_DEMO_LOG("a2dp_info->state=%d", a2dp_info->state);
            if (state_flag == 1)
            {
                sprintf((char *)string, "%s%d,\"%02x%02x%02x%02x%02x%02x\"", at_text, a2dp_info->state, a2dp_info->addr.addr[0], a2dp_info->addr.addr[1], \
                a2dp_info->addr.addr[2], a2dp_info->addr.addr[3], a2dp_info->addr.addr[4], a2dp_info->addr.addr[5]);
            }
            else 
            {
                sprintf((char *)string, "%s\"%02x%02x%02x%02x%02x%02x\"", at_text, a2dp_info->addr.addr[0], a2dp_info->addr.addr[1], \
                a2dp_info->addr.addr[2], a2dp_info->addr.addr[3], a2dp_info->addr.addr[4], a2dp_info->addr.addr[5]);
            }
            QL_A2DP_AVRCP_DEMO_LOG("%s", string);
            free(a2dp_info);
        }
    }
    else
    {
        //avrcp_info point to a malloc address,must be freed
        ql_bt_avrcp_event_info_t *avrcp_info = (ql_bt_avrcp_event_info_t *)event->param2;
        if (avrcp_info)
        {
            QL_A2DP_AVRCP_DEMO_LOG("avrcp_info->state=%d", avrcp_info->state);
            if (state_flag == 1)
            {
                sprintf((char *)string, "%s%d,\"%02x%02x%02x%02x%02x%02x\"", at_text, avrcp_info->state, avrcp_info->addr.addr[0], avrcp_info->addr.addr[1], \
                avrcp_info->addr.addr[2], avrcp_info->addr.addr[3], avrcp_info->addr.addr[4], avrcp_info->addr.addr[5]);
            }
            else 
            {
                sprintf((char *)string, "%s\"%02x%02x%02x%02x%02x%02x\"", at_text, avrcp_info->addr.addr[0], avrcp_info->addr.addr[1], \
                avrcp_info->addr.addr[2], avrcp_info->addr.addr[3], avrcp_info->addr.addr[4], avrcp_info->addr.addr[5]);
            }
            QL_A2DP_AVRCP_DEMO_LOG("%s", string);
            free(avrcp_info);
        }
    }
}

void ql_bt_a2dp_avrcp_notify_cb(void *ind_msg_buf, void *ctx)
{
    ql_event_t *event_temp = NULL;
    ql_event_t test_event = {0};
    
    if (ind_msg_buf == NULL)
    {
        return ;
    }
    event_temp = (ql_event_t *)ind_msg_buf; 

    switch (event_temp->id)
    {
        case QUEC_BT_A2DP_AUDIO_CONFIG_IND:
        case QUEC_BT_A2DP_AUDIO_STATE_START_IND:
        case QUEC_BT_A2DP_AUDIO_STATE_STOPPED_IND:
        case QUEC_BT_A2DP_CONNECTION_STATE_CONNECTED_IND:
        case QUEC_BT_A2DP_CONNECTION_STATE_DISCONNECTED_IND:
        {
            ql_bt_a2dp_event_info_t *temp = (ql_bt_a2dp_event_info_t *)event_temp->param2;
            ql_bt_a2dp_event_info_t *a2dp_info = (ql_bt_a2dp_event_info_t *)malloc(sizeof(ql_bt_a2dp_event_info_t));
            if(a2dp_info)
            {
                memset(a2dp_info, 0x00, sizeof(ql_bt_a2dp_event_info_t));
                a2dp_info->state = temp->state;
                memcpy(a2dp_info->addr.addr, temp->addr.addr, QL_BT_MAC_ADDRESS_SIZE);
            }

            test_event.id = event_temp->id;
            test_event.param1 = event_temp->param1;
            test_event.param2 = (uint32)a2dp_info;
        }
        break;
        case QUEC_BT_AVRCP_CONNECTION_STATE_CONNECTED_IND:
        case QUEC_BT_AVRCP_CONNECTION_STATE_DISCONNECTED_IND:
        case QUEC_BT_AVRCP_VOLUME_CHANGE_IND:
        {
            ql_bt_avrcp_event_info_t *temp = (ql_bt_avrcp_event_info_t *)event_temp->param2;
            ql_bt_avrcp_event_info_t *avrcp_info = (ql_bt_avrcp_event_info_t *)malloc(sizeof(ql_bt_avrcp_event_info_t));
            if(avrcp_info)
            {
                memset(avrcp_info, 0x00, sizeof(ql_bt_avrcp_event_info_t));
                avrcp_info->state = temp->state;
                memcpy(avrcp_info->addr.addr, temp->addr.addr, QL_BT_MAC_ADDRESS_SIZE);
            }

            test_event.id = event_temp->id;
            test_event.param1 = event_temp->param1;
            test_event.param2 = (uint32)avrcp_info;
        }
        break;
        default:
        {
            test_event.id = event_temp->id;
            test_event.param1 = event_temp->param1;
            test_event.param2 = event_temp->param2;
            test_event.param3 = event_temp->param3;
        }
        break;
    }
    if (test_event.id != 0)
    {
        ql_rtos_event_send(bt_a2dp_avrcp_task,&test_event);
    }
    
}

static ql_errcode_bt_e ql_bt_a2dp_avrcp_event()
{
    ql_event_t test_event = {0};
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    QL_BT_STATUS status;
    if (ql_event_try_wait(&test_event) == 0)
	{
        if (test_event.id == 0)
        {
            return ret;
        }
        status = (QL_BT_STATUS)(test_event.param1);
        switch (test_event.id)
        {
            case QUEC_BT_START_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_A2DP_AVRCP_DEMO_LOG("start sucess");
                    ret = ql_a2dp_avrcp_demo_set_scanmode();
                    if (ret != QL_BT_SUCCESS)
                    {
                        goto QL_BT_A2DP_AVRCP_STOP;
                    }

                    ret = ql_a2dp_avrcp_demo_set_name();
                    if (ret != QL_BT_SUCCESS)
                    {
                        goto QL_BT_A2DP_AVRCP_STOP;
                    }  
                }
                else
                {
                    QL_A2DP_AVRCP_DEMO_LOG("start failed");
                }
            }
            break;
            case QUEC_BT_STOP_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_A2DP_AVRCP_DEMO_LOG("stop sucess");
                }
                else
                {
                    QL_A2DP_AVRCP_DEMO_LOG("stop failed");
                }
            }
            break;
            case QUEC_BT_BLE_RESET_IND:
            {
                //蓝牙已经被复位，需要释放资源
                QL_A2DP_AVRCP_DEMO_LOG("bt reset");
                ret = QL_BT_ALREADY_STOPED_ERR; 
            }
            break;
            case QUEC_BT_A2DP_AUDIO_CONFIG_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_A2DP_HANDLE, QUEC_BT_A2DP_AUDIO_CONFIG, &test_event, 0);
            }
            break;
            case QUEC_BT_A2DP_AUDIO_STATE_START_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_A2DP_HANDLE, QUEC_BT_A2DP_AUDIO_STATE_START, &test_event, 1);
            }
            break;
            case QUEC_BT_A2DP_AUDIO_STATE_STOPPED_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_A2DP_HANDLE, QUEC_BT_A2DP_AUDIO_STATE_STOPPED, &test_event, 1);
            }
            break;
            case QUEC_BT_A2DP_CONNECTION_STATE_CONNECTED_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_A2DP_HANDLE, QUEC_BT_A2DP_CONNECT, &test_event, 1);
            }
            break;
            case QUEC_BT_A2DP_CONNECTION_STATE_DISCONNECTED_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_A2DP_HANDLE, QUEC_BT_A2DP_DISCONNECT, &test_event, 1);
            }
            break;
            case QUEC_BT_AVRCP_CONNECTION_STATE_CONNECTED_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_AVRCP_HANDLE, QUEC_BT_AVRCP_CONNECT, &test_event, 1);
                // ql_bt_avrcp_start();
            }
            break;
            case QUEC_BT_AVRCP_CONNECTION_STATE_DISCONNECTED_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_AVRCP_HANDLE, QUEC_BT_AVRCP_DISCONNECT, &test_event, 1);
            }
            break;
            case QUEC_BT_AVRCP_VOLUME_CHANGE_IND:
            {
                ql_demo_bt_a2dp_avrcp_handle(QL_AVRCP_HANDLE, QUEC_BT_AVRCP_VOLUME_CHANGE, &test_event, 1);
            }
            break;
            default:
            break;
        }
        return ret;
QL_BT_A2DP_AVRCP_STOP:
        ql_bt_stop();
	}
    return ret;
}

static void ql_a2dp_avrcp_demo_thread(void *argv)
{
	QlOSStatus err = 0;
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    ql_bt_a2dp_audio_cfg_t cfg = {0};

    //If you want to play local audio while playing Bluetooth audio, you can set the mode to QL_BTA2DP_MODE_ALLOW_INT
    cfg.mode = QL_BTA2DP_MODE_NORMAL;
    ret = ql_bt_a2dp_avrcp_set_cfg(cfg);
    if (ret != QL_BT_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("set cfg error");
        goto QL_BT_A2DP_AVRCP_NOT_INIT_EXIT;
    }

    ret = ql_bt_a2dp_avrcp_init(ql_bt_a2dp_avrcp_notify_cb);
    if (ret != QL_BT_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("init error");
        goto QL_BT_A2DP_AVRCP_NOT_INIT_EXIT;
    }

    ret = ql_bt_start();
    if (ret != QL_BT_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("start error");
        goto QL_BT_A2DP_AVRCP_INIT_EXIT;
    }
	while(1)
	{
        ret = ql_bt_a2dp_avrcp_event(); 
        if (ret != QL_BT_SUCCESS)
        {
            break;
        }
	}
QL_BT_A2DP_AVRCP_INIT_EXIT:
    ql_bt_a2dp_avrcp_release();
    ql_bt_stop();
QL_BT_A2DP_AVRCP_NOT_INIT_EXIT:	
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_A2DP_AVRCP_DEMO_LOG("deleted failed");
	}
}

void ql_bt_a2dp_avrcp_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    
    err = ql_rtos_task_create(&bt_a2dp_avrcp_task, QL_A2DP_AVRCP_TASK_STACK_SIZE, QL_A2DP_AVRCP_TASK_PRIO, "bt a2dp avrcp", ql_a2dp_avrcp_demo_thread, NULL, QL_A2DP_AVRCP_TASK_EVENT_CNT);
    if (err != QL_OSI_SUCCESS)
    {
        QL_A2DP_AVRCP_DEMO_LOG("Create bt audio task Fail");
    }
}