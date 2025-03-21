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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_nw.h"
#include "ql_api_datacall.h"
#include "ql_api_usbnet.h"
#include "ql_power.h"



#define QL_USBNET_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_USBNET_DEMO_LOG(msg, ...)			QL_LOG(QL_USBNET_LOG_LEVEL, "usbnet_demo", msg, ##__VA_ARGS__)

#define QUEC_USBNET_STOP_REQ	100|(QL_COMPONENT_NETWORK_USBNET<<16)


ql_sem_t ql_nw_reg_sem = NULL;
ql_task_t usbnet_task = NULL;
ql_timer_t usbnet_stop_timer = NULL;


static void usbnet_timer_callback(void *ctx)
{
	ql_event_t event={0};
	event.id = QUEC_USBNET_STOP_REQ;
	ql_rtos_event_send(usbnet_task, &event);	//send event to reconnect

	return;
}


static void ql_nw_event_callback(uint8_t sim_id, unsigned int event_type, void *ind_msg_buf)
{
	if(QUEC_NW_DATA_REG_STATUS_IND == event_type)
	{
		ql_nw_common_reg_status_info_s  *data_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
		QL_USBNET_DEMO_LOG("Sim%d data reg status changed, current status is %d", sim_id, data_reg_status->state);
		if((QL_NW_REG_STATE_HOME_NETWORK == data_reg_status->state) || (QL_NW_REG_STATE_ROAMING == data_reg_status->state))
		{
			ql_rtos_semaphore_release(ql_nw_reg_sem);
		}
	}
}

static void ql_usbnet_event_callback(unsigned int event_type, ql_usbnet_errcode_e errcode, void *ctx)
{
	QL_USBNET_DEMO_LOG("event_type: 0x%x, errcode: 0x%x", event_type, errcode);

	switch(event_type)
	{
		case QUEC_USBNET_START_RSP_IND:
		{
			if(QL_USBNET_SUCCESS == errcode)
			{
				QL_USBNET_DEMO_LOG("usbnet connect success");
				//run the following blocked code, the RNDIS/ECM dial will be disconnected after two minutes
#if 0
				int ret = ql_rtos_timer_start(usbnet_stop_timer, 1000 * 120, 0);
				if(ret != 0)
				{
					QL_USBNET_DEMO_LOG("usbnet_stop_timer start failed");
				}
#endif
			}
			else
			{
				QL_USBNET_DEMO_LOG("usbnet connect fail, err: 0x%x", errcode);
				ql_rtos_timer_stop(usbnet_stop_timer);
			}
			break;
		}
		case QUEC_USBNET_DEACTIVE_IND:
		{
			QL_USBNET_DEMO_LOG("PDP deactived");
			ql_rtos_timer_stop(usbnet_stop_timer);
			
			ql_event_t event={0};
			event.id = QUEC_USBNET_DEACTIVE_IND;
			ql_rtos_event_send(usbnet_task, &event);	//send event to reconnect
			
			break;
		}
		case QUEC_USBNET_PORT_CONNECT_IND:
		{
			QL_USBNET_DEMO_LOG("usb port plug in");
			
			ql_event_t event={0};
			event.id = QUEC_USBNET_PORT_CONNECT_IND;
			ql_rtos_event_send(usbnet_task, &event);	//send event to reconnect
			
			break;
		}
		case QUEC_USBNET_PORT_DISCONNECT_IND:
		{
			QL_USBNET_DEMO_LOG("usb port pull out");
			ql_rtos_timer_stop(usbnet_stop_timer);
			break;
		}
	}
}

static void usbent_app_thread(void * arg)
{
    ql_usbnet_errcode_e ret = 0;
	uint8_t sim_id = 0;
	int profile_idx = 1;	//range 1 to 7
	ql_nw_reg_status_info_s nw_info = {0};
	ql_event_t event;
	ql_usbnet_type_e saved_type = 0;
	ql_usbnet_type_e target_type = QL_USBNET_ECM;
	ql_usbnet_state_e status = QL_USBNET_STATE_NONE;
	uint8_t reset_flag = false;

	ret = ql_rtos_timer_create(&usbnet_stop_timer, usbnet_task, usbnet_timer_callback, NULL);
	QL_USBNET_DEMO_LOG("create timer ret: 0x%x", ret);
	if(ret != 0)
	{
		goto exit;
	}

	ret = ql_usbnet_get_type(&saved_type);
	QL_USBNET_DEMO_LOG("get usbnet saved type ret: 0x%x, %d", ret, saved_type);
	if(0 != ret)
	{
		goto exit;
	}

	//run the following blocked code, RNDIS/ECM and Socket can use profile 1 simultaneously 
#if 0
	uint32_t saved_list = 0;
	uint32_t target_list = 0x1;	//enable sim0 profile 1 nat function
	ret = ql_datacall_get_nat(&saved_list);
	QL_USBNET_DEMO_LOG("get datacall saved nat ret: 0x%x, %d", ret, saved_list);
	if(0 != ret)
	{
		goto exit;
	}
	if(saved_list != target_list)
	{
		ret = ql_datacall_set_nat(target_list);
		QL_USBNET_DEMO_LOG("ql_datacall_set_nat err, ret=0x%x", ret);
		if(0 != ret)
		{
			goto exit;
		}
		reset_flag = true;		//NAT function reboot to take effect
	}

	/*ql_nat_subnet_config_s config = {"192.168.1.0","255.255.255.0"};
	ret = ql_datacall_set_subnet(sim_id, profile_idx, &config);
	QL_USBNET_DEMO_LOG("ql_datacall_set_subnet ret=0x%x", ret);
	if(0 != ret)
	{
		goto exit;
	}*/
#endif

	if(saved_type != target_type)
	{
		ret = ql_usbnet_set_type(target_type);
		QL_USBNET_DEMO_LOG("ql_usbnet_set_type err, ret=0x%x", ret);
		if(0 != ret)
		{
			goto exit;
		}
		reset_flag = true;		//usbnet type reboot to take effect
	}
	
	if(reset_flag)
	{
		ql_power_reset(RESET_NORMAL);
	}
	
	ql_rtos_semaphore_create(&ql_nw_reg_sem, 0);

	ret = ql_nw_register_cb(ql_nw_event_callback);
	QL_USBNET_DEMO_LOG("register network callback ret=0x%x", ret);
	if(0 != ret)
	{
		goto exit;
	}

	ret = ql_nw_get_reg_status(sim_id, &nw_info);
	QL_USBNET_DEMO_LOG("ret: 0x%x, current data reg status=%d", ret, nw_info.data_reg.state);
	if((QL_NW_REG_STATE_HOME_NETWORK != nw_info.data_reg.state) && (QL_NW_REG_STATE_ROAMING != nw_info.data_reg.state))   
	{
		ql_rtos_semaphore_wait(ql_nw_reg_sem, QL_WAIT_FOREVER);  //wait network registered
	}

	ret = ql_usbnet_register_cb(ql_usbnet_event_callback, NULL);
	QL_USBNET_DEMO_LOG("register usbnet callback ret=0x%x", ret);
	if(ret != QL_USBNET_SUCCESS)
	{
		goto exit;
	}
	
	ret = ql_usbnet_get_status(&status);
	QL_USBNET_DEMO_LOG("ret: 0x%x, usbnet status: %d", ret, status);
	if((QL_USBNET_SUCCESS == ret) && (QL_USBNET_STATE_NONE == status))
	{
		ret = ql_usbnet_start(sim_id, profile_idx, NULL);
		QL_USBNET_DEMO_LOG("ql_usbnet_start ret=0x%x", ret);
	}

	while(1)
	{
		if(ql_event_try_wait(&event) != 0)
		{
			continue;
		}

		switch (event.id)
		{
			case QUEC_USBNET_DEACTIVE_IND:
			{
				ql_rtos_task_sleep_ms(5000);	//wait network status update
				ret = ql_nw_get_reg_status(sim_id, &nw_info);
				if((QL_NW_REG_STATE_HOME_NETWORK != nw_info.data_reg.state) && (QL_NW_REG_STATE_ROAMING != nw_info.data_reg.state))   
				{
					ql_rtos_semaphore_wait(ql_nw_reg_sem, QL_WAIT_FOREVER);
				}
				
				ret = ql_usbnet_start(sim_id, profile_idx, NULL);
				QL_USBNET_DEMO_LOG("ql_usbnet_start ret=0x%x", ret);
				break;
			}
			case QUEC_USBNET_PORT_CONNECT_IND:
			{
				ret = ql_usbnet_start(sim_id, profile_idx, NULL);
				QL_USBNET_DEMO_LOG("ql_usbnet_start ret=0x%x", ret);
				break;
			}
			case QUEC_USBNET_STOP_REQ:
			{
				ret = ql_usbnet_get_status(&status);
				QL_USBNET_DEMO_LOG("ret: 0x%x, usbnet status: %d", ret, status);
				if((QL_USBNET_SUCCESS == ret) && (QL_USBNET_STATE_CONNECT == status))
				{
					ret = ql_usbnet_stop();
					QL_USBNET_DEMO_LOG("ql_usbnet_stop ret=0x%x", ret);
				}
				break;
			}
		}
	}

exit:
	if(ql_nw_reg_sem != NULL)
	{
		ql_rtos_semaphore_delete(ql_nw_reg_sem);
		ql_nw_reg_sem = NULL;
	}
	if(usbnet_stop_timer != NULL)
	{
		ql_rtos_timer_delete(usbnet_stop_timer);
		usbnet_stop_timer = NULL;
	}
	if(usbnet_task != NULL)
	{
		ql_rtos_task_delete(usbnet_task);
		usbnet_task = NULL;
	}
}


void ql_usbnet_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&usbnet_task, 4*1024, APP_PRIORITY_NORMAL, "QUSBNETDEMO", usbent_app_thread, NULL, 10);
    if(err != QL_OSI_SUCCESS)
	{
		QL_USBNET_DEMO_LOG("task created failed");
        return;
	}
    
}

