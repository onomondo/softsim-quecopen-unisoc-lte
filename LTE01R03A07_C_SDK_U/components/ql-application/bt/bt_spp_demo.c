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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_api_bt.h"
#include "ql_api_bt_spp.h"

#include "ql_log.h"
#include "bt_spp_demo.h"

#define QL_BT_SPP_LOG_LEVEL         	QL_LOG_LEVEL_INFO
#define QL_BT_SPP_LOG(msg, ...)			QL_LOG(QL_BT_SPP_LOG_LEVEL, "ql_bt_spp_DEMO", msg, ##__VA_ARGS__)
#define QL_BT_SPP_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_bt_spp_DEMO", msg, ##__VA_ARGS__)

#define QL_BT_DEMO_ADDR_MAX_SIZE                    17
ql_task_t bt_demo_task = NULL;

ql_bt_inquiry_type_e bt_inquiry_type = QL_BT_SERVICE_ALL;
ql_bt_addr_s bt_connection_addr = {{0x66, 0xCA, 0xC9, 0xA2, 0x3E, 0x38}};

extern ql_errcode_bt_e ql_bt_demo_start();
extern ql_errcode_bt_e ql_bt_demo_stop();
extern ql_errcode_bt_e ql_bt_demo_get_state();
extern ql_errcode_bt_e ql_bt_demo_get_local_name();
extern ql_errcode_bt_e ql_bt_demo_set_local_name();
extern ql_errcode_bt_e ql_bt_demo_get_scanmde();
extern ql_errcode_bt_e ql_be_demo_set_scanmode();

ql_errcode_bt_e ql_bt_demo_start_inquiry()
{
    ql_errcode_bt_e ret;
    
    ret = ql_classic_bt_start_inquiry(bt_inquiry_type);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else 
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_cancel_inquiry()
{
    ql_errcode_bt_e ret;
    
    ret = ql_classic_bt_cancel_inquiry();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else 
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

static ql_errcode_bt_e ql_bt_demo_connect()
{
    ql_errcode_bt_e ret;
    ql_bt_addr_s connect_info;

    connect_info.addr[0] = 0x66;
    connect_info.addr[1] = 0xCA;
    connect_info.addr[2] = 0xC9;
    connect_info.addr[3] = 0xA2;
    connect_info.addr[4] = 0x3E;
    connect_info.addr[5] = 0x38;
    
    ret = ql_bt_spp_connect(connect_info);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

static ql_errcode_bt_e ql_bt_demo_disconnect()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_spp_disconnect();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}


static ql_errcode_bt_e ql_bt_demo_spp_send_data()
{
    ql_errcode_bt_e ret;
    uint8 spp_info[10]="123456789";
    
    ret = ql_bt_spp_data_send(spp_info,9);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

void ql_bt_notify_cb(void *ind_msg_buf, void *ctx)
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
        case QUEC_BT_SPP_CONNECT_IND:
        case QUEC_BT_SPP_DISCONNECT_IND:
        {
            ql_bt_spp_event_info_t *temp = (ql_bt_spp_event_info_t *)event_temp->param2;
            
            ql_bt_spp_event_info_t *spp_info = (ql_bt_spp_event_info_t *)malloc(sizeof(ql_bt_spp_event_info_t));
            
            if (spp_info)
            {
                memset(spp_info, 0x00, sizeof(ql_bt_spp_event_info_t));
                spp_info->state = temp->state;
                memcpy(spp_info->addr.addr, temp->addr.addr, QL_BT_MAC_ADDRESS_SIZE);

                test_event.id = event_temp->id;
                test_event.param2 = (uint32)spp_info;

            }

        }  
        break; 
        case QUEC_BT_SPP_REVDATA_IND:
        {
            ql_bt_spp_data_info_t *temp = (ql_bt_spp_data_info_t *)event_temp->param2;
            
            ql_bt_spp_data_info_t *sppdata_info = (ql_bt_spp_data_info_t *)calloc(1,sizeof(ql_bt_spp_data_info_t));
            
            if (sppdata_info)
            {
                sppdata_info->buf = (uint8*)calloc(1,temp->dataLen+1);
                if(sppdata_info->buf == NULL)
                {
                    free(sppdata_info);
                    QL_BT_SPP_LOG("buf malloc error");
                    return;
                }

                sppdata_info->dataLen = temp->dataLen;
                memcpy(sppdata_info->buf, temp->buf, temp->dataLen);
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)sppdata_info;

            }

        }  
        break;
        case QUEC_BT_INQUIRY_IND:
        case QUEC_BT_INQUIRY_END_IND:
        {
            ql_bt_inquiry_info_s *temp = (ql_bt_inquiry_info_s *)event_temp->param2;
            
            ql_bt_inquiry_info_s *bt_inquiry_info = (ql_bt_inquiry_info_s *)malloc(sizeof(ql_bt_inquiry_info_s));
            
            if (bt_inquiry_info)
            {
                memset(bt_inquiry_info, 0x00, sizeof(ql_bt_inquiry_info_s));
                bt_inquiry_info->rssi=temp->rssi;
                memcpy(bt_inquiry_info->addr.addr, temp->addr.addr, QL_BT_MAC_ADDRESS_SIZE);
                memcpy(bt_inquiry_info->device_name, temp->device_name, strlen((const char *)temp->device_name));
                bt_inquiry_info->device_class=temp->device_class;

                test_event.id = event_temp->id;
                test_event.param2 = (uint32)bt_inquiry_info;

            }

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
        ql_rtos_event_send(bt_demo_task,&test_event);
    }

}

static ql_errcode_bt_e ql_bt_spp_event()
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
                    QL_BT_SPP_LOG("start sucess");
                    ret = ql_bt_demo_get_state();
                    if (ret == QL_BT_SUCCESS)
                    {  
                        ql_be_demo_set_scanmode();

                        ret = ql_bt_demo_set_local_name();
                        if (ret != QL_BT_SUCCESS)
                        {
                            goto QL_BT_SPP_CLIENT_STOP;
                        }
                        else 
                        {
                            ql_bt_demo_start_inquiry();
                        }    
                    }
                    else 
                    {
                        goto QL_BT_SPP_CLIENT_STOP;
                    }    
                }
                else
                {
                    QL_BT_SPP_LOG("start failed");
                }
            }
            break;
            case QUEC_BT_STOP_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("stop sucess");
                }
                else
                {
                    QL_BT_SPP_LOG("stop failed");
                }
            }
            break;
            case QUEC_BT_BLE_ERROR_IND:
            {
                ql_bt_ble_state_e state = test_event.param2;
                QL_BT_SPP_LOG("state=%d", state);
                //只需要处理QL_BT_SPP_ERR
                if (state == QL_BT_SPP_ERR)
                {
                    //出现未知错误，需要先关闭蓝牙，在开启蓝牙后跑SPP流程
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            case QUEC_BT_BLE_RESET_IND:
            {
                //蓝牙已经被复位，需要释放资源
                QL_BT_SPP_LOG("bt reset");
                ret = QL_BT_ALREADY_STOPED_ERR; 
            }
            break;
            case QUEC_BT_INQUIRY_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    
                    ql_bt_inquiry_info_s *bt_inquiry_info = (ql_bt_inquiry_info_s *)test_event.param2;
                    
                    if (bt_inquiry_info)
                    {
                        QL_BT_SPP_LOG("addr=%02x%02x%02x%02x%02x%02x", bt_inquiry_info->addr.addr[0], bt_inquiry_info->addr.addr[1], \
                            bt_inquiry_info->addr.addr[2], bt_inquiry_info->addr.addr[3], bt_inquiry_info->addr.addr[4], bt_inquiry_info->addr.addr[5]); 
                        QL_BT_SPP_LOG("rssi=%d,name=%s,class=%d",bt_inquiry_info->rssi,bt_inquiry_info->device_name,bt_inquiry_info->device_class);
                        free(bt_inquiry_info);
                    }
                }
                else
                {
                    QL_BT_SPP_LOG("inqury failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            case QUEC_BT_INQUIRY_END_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    ql_bt_inquiry_info_s *bt_inquiry_info = (ql_bt_inquiry_info_s *)test_event.param2;
                    
                    if (bt_inquiry_info)
                    {
                        QL_BT_SPP_LOG("status=%d",bt_inquiry_info->status);
                        free(bt_inquiry_info);
                    }

                    ql_bt_demo_cancel_inquiry();
                    ql_bt_demo_connect();
                }
                else
                {
                    QL_BT_SPP_LOG("inqury end failed");
                    goto QL_BT_SPP_CLIENT_STOP; 
                }
            }
            break;
            case QUEC_BT_SPP_CONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    ql_bt_spp_event_info_t *spp_info = (ql_bt_spp_event_info_t *)test_event.param2;
                    if (spp_info)
                    {   

                        QL_BT_SPP_LOG("addr=%02x%02x%02x%02x%02x%02x", spp_info->addr.addr[0], spp_info->addr.addr[1], \
                            spp_info->addr.addr[2], spp_info->addr.addr[3], spp_info->addr.addr[4], spp_info->addr.addr[5]); 

                        free(spp_info);
                    }                    
                }
                else
                {
                    QL_BT_SPP_LOG("connect failed");
                    goto QL_BT_SPP_CLIENT_STOP; 
                }
            }
            break;
            case QUEC_BT_SPP_REVDATA_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    ql_bt_spp_data_info_t *sppdata_info = (ql_bt_spp_data_info_t *)test_event.param2;
                    
                    if (sppdata_info)
                    {
                        if(sppdata_info->buf)
                        {
                            QL_BT_SPP_LOG("data=%s;datalen=%d",sppdata_info->buf,sppdata_info->dataLen);
                            free(sppdata_info->buf);
                        }  
                        free(sppdata_info);
                    }

                    ret = ql_bt_demo_spp_send_data();
                    if (ret != QL_BT_SUCCESS)
                    {
                        goto QL_BT_SPP_CLIENT_STOP;
                    }
                    ql_bt_demo_disconnect();
                }
                else
                {
                    QL_BT_SPP_LOG("spp send data failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            case QUEC_BT_SPP_DISCONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    ql_bt_spp_event_info_t *spp_info = (ql_bt_spp_event_info_t *)test_event.param2;
                    if (spp_info)
                    {   

                        QL_BT_SPP_LOG("addr=%02x%02x%02x%02x%02x%02x", spp_info->addr.addr[0], spp_info->addr.addr[1], \
                            spp_info->addr.addr[2], spp_info->addr.addr[3], spp_info->addr.addr[4], spp_info->addr.addr[5]); 

                        free(spp_info);
                    } 

                    QL_BT_SPP_LOG("disconnect sucess");
                    goto QL_BT_SPP_CLIENT_STOP;   
                }
                else
                {
                    QL_BT_SPP_LOG("disconnect failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            default:
            break;
        }
        return ret;
QL_BT_SPP_CLIENT_STOP:
        ql_bt_demo_stop();
	}
    return ret;
}

void ql_bt_spp_demo_task_pthread(void *ctx)
{
	QlOSStatus err = 0;
    ql_errcode_bt_e ret = QL_BT_SUCCESS;

 QL_BT_SPP_RE_START:
    ret = ql_bt_spp_init(ql_bt_notify_cb);
    if (ret != QL_BT_SUCCESS)
    {
        goto QL_BT_SPP_SERVER_NOT_INIT_EIXT;
    }

    ret = ql_bt_demo_start();
    if (ret != QL_BT_SUCCESS)
    {
        goto QL_BT_SPP_SERVER_INIT_EIXT;
    }
    
	while(1)
	{
        ret = ql_bt_spp_event(); 
        if (ret != QL_BT_SUCCESS)
        {
            break;
        }
	}
QL_BT_SPP_SERVER_INIT_EIXT:
	ql_bt_spp_release();

    goto QL_BT_SPP_RE_START;
QL_BT_SPP_SERVER_NOT_INIT_EIXT:
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_BT_SPP_LOG("task deleted failed");
	}
}



QlOSStatus ql_bt_spp_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    QL_BT_SPP_LOG("enter ql_bt_spp_demo_init");
	err = ql_rtos_task_create(&bt_demo_task, BT_BLE_DEMO_TASK_STACK_SIZE, BT_BLE_DEMO_TASK_PRIO, "ql_spp_demo", ql_bt_spp_demo_task_pthread, NULL, BT_BLE_DEMO_TASK_EVENT_CNT);
	
    return err;
}


