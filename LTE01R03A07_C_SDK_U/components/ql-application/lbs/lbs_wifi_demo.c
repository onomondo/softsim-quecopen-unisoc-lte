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

=================================================================*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "ql_api_osi.h"
#include "ql_api_nw.h"

#include "ql_log.h"
#include "ql_wifi_scan.h"
#include "ql_api_datacall.h"
#include "ql_lbs_client.h"

#define QL_LBS_WIFI_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_LBS_WIFI_LOG(msg, ...)			QL_LOG(QL_LBS_WIFI_LOG_LEVEL, "ql_LBS_WIFI_DEMO", msg, ##__VA_ARGS__)
#define QL_LBS_WIFI_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_LBS_WIFI_DEMO", msg, ##__VA_ARGS__)

typedef enum{
	QLBS_WIFI_EVENT_TIMER_START	= 1001,
	QLBS_WIFI_EVENT_TIMER_STOP,
	QLBS_WIFI_EVENT_WIFISCAN_START,
	QLBS_WIFI_EVENT_WIFISCAN_STOP,
	QLBS_WIFI_EVENT_LBS_CFG,
	QLBS_WIFI_EVENT_LBS_START,
	QLBS_WIFI_EVENT_END,
}qlbs_wifi_event_code_e;

typedef enum{
	QLBS_WIFI_STATE_INIT	     	= 0,
}qlbs_wifi_state_code_e;

typedef struct
{
	lbs_client_hndl 		lbs_client;

	ql_task_t				task;
	ql_mutex_t				lock;
	ql_queue_t				queue;
	ql_timer_t 				timer;
	int                     timeout;

	int 				 	pdp_cid;
	int 				 	req_timeout;
	char                 	host[256];
	lbs_basic_info_t     	basic_info;
	lbs_auth_info_t      	auth_info;
	
	int                  	wifi_num;
	lbs_wifi_mac_info_t 	*wifi_info;
	lbs_cell_info_t 		*cell_info;
	int 					loc_method;
}qlbs_wifi_ctx_t;

#define QLBS_WIFI_MAX_MSG_CNT	10
static ql_task_t lbs_wifi_task = NULL;
qlbs_wifi_ctx_t *lbs_wifi_client = NULL;

/* wifi scan parameter */
#define QLBS_WIFI_SCAN_AP_CNT 		QL_WIFI_SCAN_DEFAULT_AP_CNT
#define QLBS_WIFI_SCAN_ROUND  		QL_WIFI_SCAN_DEFAULT_ROUND+1
#define QLBS_WIFI_SCAN_CHANNEL    	QL_WIFISCAN_CHANNEL_ALL
#define QLBS_WIFI_SCAN_TIME 		QL_WIFI_SCAN_DEFAULT_TIME

#define QLBS_WIFI_SCAN_SERVER		"www.queclocator.com"
/*token application*/
#define QLBS_WIFI_SCAN_TOKEN		"1111111122222222" /*Need to apply for a valid token*/
#define QLBS_WIFI_SCAN_USER_NAME	"quectel"
#define QLBS_WIFI_SCAN_USER_PWD		"123456"
#define QLBS_WIFI_SCAN_IMEI			"861687000001091"

static void ql_lbs_wifi_timeout_cb(void *param)
{
	qlbs_wifi_ctx_t *client_ptr = (qlbs_wifi_ctx_t *)param;
	ql_event_t send_event = {0};
	QL_LBS_WIFI_LOG("enter");
	if(client_ptr == NULL || client_ptr->queue == NULL)
		return;
	
	send_event.id = QLBS_WIFI_EVENT_END;
	send_event.param1 = 0;
	ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, QL_WAIT_FOREVER);
}

void ql_lbs_wifi_timer_start(void *param)
{
	qlbs_wifi_ctx_t *client_ptr = NULL;
	client_ptr = (qlbs_wifi_ctx_t *)param;
	QL_LBS_WIFI_LOG("enter");

	if(NULL == client_ptr)
		return;

	if(client_ptr->timer == NULL || ql_rtos_timer_is_running(client_ptr->timer) == false)
	{
		if (client_ptr->timer == NULL)
		{
			ql_rtos_timer_create(&client_ptr->timer, client_ptr->task, ql_lbs_wifi_timeout_cb, client_ptr);
		}
        ql_rtos_timer_start(client_ptr->timer, client_ptr->timeout * 1000, 0);
	}
}

void ql_lbs_wifi_timer_stop(void *param)
{
	qlbs_wifi_ctx_t *client_ptr = NULL;
	client_ptr = (qlbs_wifi_ctx_t *)param;

	QL_LBS_WIFI_LOG("enter");

	if(NULL == client_ptr)
		return;
	
	if(client_ptr->timer != NULL)
	{
		if( ql_rtos_timer_is_running(client_ptr->timer) == true )
			ql_rtos_timer_stop(client_ptr->timer);
		ql_rtos_timer_delete(client_ptr->timer);
		client_ptr->timer = NULL;
		
	}
}

//all memory of msg_buf and it's sub item will be released by core after the call of callback.
void ql_lbs_wifi_scan_app_callback(ql_wifiscan_ind_msg_s *msg_buf)
{
	int i = 0;
    ql_event_t send_event = {0};
	qlbs_wifi_ctx_t *client_ptr = lbs_wifi_client;
	
	send_event.param1 = (uint32)client_ptr;
	
    if ((NULL == msg_buf)||(NULL == client_ptr))
    {
		ql_wifiscan_close();
        return;
    }
    
    QL_LBS_WIFI_LOG("id=0x%X, err=%d, buf=%p", msg_buf->msg_id, msg_buf->msg_err_code, msg_buf->msg_data);
    if (QUEC_WIFISCAN_EVENT_DO == msg_buf->msg_id)
    {
        if ((QL_WIFISCAN_SUCCESS == msg_buf->msg_err_code) && (NULL != msg_buf->msg_data))
        {
            ql_wifiscan_result_s *scan_result = msg_buf->msg_data;
            QL_LBS_WIFI_LOG("ap_cnt=%d", scan_result->ap_cnt);
			
			ql_rtos_mutex_lock(client_ptr->lock, 1000);
			client_ptr->wifi_num = scan_result->ap_cnt;

			if(client_ptr->wifi_info != NULL)
				free(client_ptr->wifi_info);
			
			client_ptr->wifi_info = (lbs_wifi_mac_info_t *)malloc(scan_result->ap_cnt * sizeof(lbs_wifi_mac_info_t));
			if(client_ptr->wifi_info == NULL)
			{
				send_event.id = QLBS_WIFI_EVENT_END;
				ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, QL_WAIT_FOREVER);
				QL_LBS_WIFI_LOG("malloc faild");
				return;
			}
			
			for(i = 0; i < scan_result->ap_cnt; i++)
			{
				sprintf(client_ptr->wifi_info[i].wifi_mac, "%02X:%02X:%02lX:%02lX:%02lX:%02lX", 	
															((scan_result->ap_infos[i].bssid_high >> 8) & 0xFF),
															(scan_result->ap_infos[i].bssid_high & 0xFF),
															((scan_result->ap_infos[i].bssid_low >> 24) & 0xFF),
															((scan_result->ap_infos[i].bssid_low >> 16) & 0xFF),
															((scan_result->ap_infos[i].bssid_low >> 8) & 0xFF),
															(scan_result->ap_infos[i].bssid_low & 0xFF));
				
				client_ptr->wifi_info[i].wifi_rssi = (int)scan_result->ap_infos[i].rssival;
				strncpy(client_ptr->wifi_info[i].wifi_ssid, "test", strlen("test")+1);
			}
			ql_rtos_mutex_unlock(client_ptr->lock);
			send_event.id = QLBS_WIFI_EVENT_LBS_START;
        }
		else
		{
			send_event.id = QLBS_WIFI_EVENT_END;
		}
		
        ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, QL_WAIT_FOREVER);
    }
}

void ql_lbs_wifi_scan_async_start(void *param)
{
	qlbs_wifi_ctx_t *client_ptr = NULL;
    ql_event_t send_event = {0};
	ql_event_t *recv_event_ptr = (ql_event_t *)param;

	if(recv_event_ptr->param1 == 0)
		return;

	client_ptr = (qlbs_wifi_ctx_t *)recv_event_ptr->param1;

	send_event.param1 = (uint32)client_ptr;
	
	if (QL_WIFISCAN_SUCCESS != ql_wifiscan_option_set(QLBS_WIFI_SCAN_AP_CNT,
                                                      QLBS_WIFI_SCAN_ROUND,
                                                      QLBS_WIFI_SCAN_CHANNEL,
                                                      QLBS_WIFI_SCAN_TIME)
       )
    {
        QL_LBS_WIFI_LOG("option set err");
        goto exit;
    }
	   
	if (QL_WIFISCAN_SUCCESS != ql_wifiscan_register_cb(ql_lbs_wifi_scan_app_callback))
    {
        QL_LBS_WIFI_LOG("register cb err");
        goto exit;
    }
	
    if (QL_WIFISCAN_SUCCESS != ql_wifiscan_open())
    {
        QL_LBS_WIFI_LOG("device open err");
        goto exit;
    }
	
    if (QL_WIFISCAN_SUCCESS != ql_wifiscan_async())
    {
        QL_LBS_WIFI_LOG("to do a async scan err");
        goto exit;
    }

	return;
	
exit:
	send_event.id = QLBS_WIFI_EVENT_END;
	ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, QL_WAIT_FOREVER);
}

int ql_lbs_cell_info_get(void *client)
{
	int ret = -1;
	uint8_t nSim = 0;
    ql_nw_cell_info_s *cell_info = NULL;
    ql_nw_seclection_info_s select_info;
	qlbs_wifi_ctx_t *client_ptr = (qlbs_wifi_ctx_t *)client;
	if(NULL == client_ptr){
		goto exit;
	}
	cell_info = (ql_nw_cell_info_s *)malloc(sizeof(ql_nw_cell_info_s));
	if(cell_info == NULL){
		goto exit;
	}
	if(client_ptr->cell_info == NULL){
	client_ptr->cell_info = (lbs_cell_info_t *)malloc(sizeof(lbs_cell_info_t));
	if(client_ptr->cell_info == NULL){
		goto exit;
	}
		}
	memset( cell_info, 0, sizeof(ql_nw_cell_info_s));
	ret = ql_nw_get_cell_info(nSim, cell_info);
	if(ret!= QL_NW_SUCCESS)
	{
		QL_LBS_WIFI_LOG("===============lbs get cell info fail===============\n");
		goto exit;
	}
	
	ret = ql_nw_get_selection(nSim, &select_info);	
	if(ret != 0){
		QL_LBS_WIFI_LOG("ql_nw_get_selection ret: %d", ret);
		goto exit;
	}
	
	QL_LBS_WIFI_LOG("nw_act_type=%d",select_info.act);
	if(select_info.act==QL_NW_ACCESS_TECH_GSM)
	{
		client_ptr->cell_info->radio=1;
		client_ptr->cell_info->mcc = cell_info->gsm_info[0].mcc;
		client_ptr->cell_info->mnc = cell_info->gsm_info[0].mnc;
		client_ptr->cell_info->cell_id = cell_info->gsm_info[0].cid;
		client_ptr->cell_info->lac_id = cell_info->gsm_info[0].lac;
		client_ptr->cell_info->bsic = cell_info->gsm_info[0].bsic;
		client_ptr->cell_info->uarfcndl = cell_info->gsm_info[0].arfcn;
		client_ptr->cell_info->bcch = cell_info->gsm_info[0].arfcn;
		client_ptr->cell_info->signal=cell_info->gsm_info[0].rssi;
	}
	else if(select_info.act==QL_NW_ACCESS_TECH_E_UTRAN)
	{
		client_ptr->cell_info->radio=3;
		client_ptr->cell_info->mcc = cell_info->lte_info[0].mcc;
		client_ptr->cell_info->mnc = cell_info->lte_info[0].mnc;
		client_ptr->cell_info->cell_id = cell_info->lte_info[0].cid;
		client_ptr->cell_info->lac_id = cell_info->lte_info[0].tac;
		client_ptr->cell_info->tac = cell_info->lte_info[0].tac;
		client_ptr->cell_info->pci = cell_info->lte_info[0].pci;
		client_ptr->cell_info->earfcn = cell_info->lte_info[0].earfcn;
		client_ptr->cell_info->bcch = cell_info->lte_info[0].earfcn;
		client_ptr->cell_info->signal=cell_info->lte_info[0].rssi;
	}
	else
	{
		ret =-1;
		goto exit;
	}
	QL_LBS_WIFI_LOG("cell infoinfo: radio=%d,mcc=%d,mnc=%d,lac_id=%x,cell_id=%x,signal=%d,tac =%x,bcch=%d,bsic=%d,uarfcndl=%d,psc=%d,rsrq=%d,pci=%d,earfcn=%d",\
	client_ptr->cell_info->radio,client_ptr->cell_info->mcc,client_ptr->cell_info->mnc,client_ptr->cell_info->lac_id,client_ptr->cell_info->cell_id,client_ptr->cell_info->signal,client_ptr->cell_info->tac,\
	client_ptr->cell_info->bcch,client_ptr->cell_info->bsic,client_ptr->cell_info->uarfcndl,client_ptr->cell_info->psc,client_ptr->cell_info->rsrq,client_ptr->cell_info->pci,client_ptr->cell_info->earfcn);

exit:
	if(NULL != cell_info) free(cell_info);
	return ret;
}

void ql_lbs_wifi_lbs_cfg_func(void *client, int method)
{
	qlbs_wifi_ctx_t *client_ptr = NULL;

	client_ptr = (qlbs_wifi_ctx_t *)client;
	if(NULL == client_ptr) return;
	
	client_ptr->req_timeout = 60;

	snprintf(client_ptr->host, 256, "%s", QLBS_WIFI_SCAN_SERVER);

	client_ptr->basic_info.type = 0x01;
	client_ptr->basic_info.encrypt = 0x01;
	client_ptr->basic_info.key_index = 0x01;
	client_ptr->basic_info.pos_format = 0x01;
	client_ptr->basic_info.loc_method = method;

	snprintf(client_ptr->auth_info.user_name, 64, "%s", QLBS_WIFI_SCAN_USER_NAME);
	snprintf(client_ptr->auth_info.user_pwd, 64, "%s", QLBS_WIFI_SCAN_USER_PWD);
	snprintf(client_ptr->auth_info.token, 128, "%s", QLBS_WIFI_SCAN_TOKEN);
	snprintf(client_ptr->auth_info.imei, 64, "%s", QLBS_WIFI_SCAN_IMEI);
	client_ptr->auth_info.rand = 2346;	
}

static void ql_lbs_wifi_result_cb(lbs_response_data_t *response_data)
{
	int i = 0;
	ql_event_t send_event = {0};
	qlbs_wifi_ctx_t *client_ptr = NULL;

    QL_LBS_WIFI_LOG("enter");
    if (NULL == response_data || response_data->arg == NULL)
    {
        return;
    }

	client_ptr = (qlbs_wifi_ctx_t *)response_data->arg;

	if(client_ptr->lbs_client != response_data->hndl)
	{
        return;
    }
	
    QL_LBS_WIFI_LOG("lbs result: %08X", response_data->result);
	if(response_data->result == QL_LBS_OK && response_data->pos_num > 0)
	{
		for(i = 0; i < response_data->pos_num; i++)
		{
			QL_LBS_WIFI_LOG("Location[%d]: %f, %f, %d\n", i, response_data->pos_info[i].longitude, 
						response_data->pos_info[i].latitude, response_data->pos_info[i].accuracy);
		}
	}
	else
	{
		QL_LBS_WIFI_LOG("response faild");
	}

	send_event.id = QLBS_WIFI_EVENT_END;
    send_event.param1 = (uint32)client_ptr;
    ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, 0);
}

void ql_lbs_wifi_start_func(void *param)
{
	int ret = 0;
	int loc_method = LBS_CELL_LOC_METHOD;// LBS_CELL_WIFI_LOC_METHOD  LBS_WIFI_LOC_METHOD
	qlbs_wifi_ctx_t *client_ptr = NULL;
    ql_event_t send_event = {0};
	ql_event_t *recv_event_ptr = (ql_event_t *)param;
    lbs_option_t user_option = {0};

	if((NULL == recv_event_ptr)||(recv_event_ptr->param1 == 0))
		return;

	client_ptr = (qlbs_wifi_ctx_t *)recv_event_ptr->param1;
	
	loc_method = client_ptr->loc_method;
	if(NULL == client_ptr) return;
	if(client_ptr->wifi_num < 2){
		client_ptr->wifi_num = 0;
		ql_lbs_wifi_lbs_cfg_func(client_ptr, LBS_CELL_LOC_METHOD);/*wifi is invalid, use CELL*/
	}else if(loc_method == LBS_WIFI_LOC_METHOD){
		ql_lbs_wifi_lbs_cfg_func(client_ptr, LBS_WIFI_LOC_METHOD);
	}else if(loc_method == LBS_CELL_LOC_METHOD){
		client_ptr->wifi_num = 0;
		ql_lbs_wifi_lbs_cfg_func(client_ptr, LBS_CELL_LOC_METHOD);
	}else{
		ql_lbs_wifi_lbs_cfg_func(client_ptr, LBS_CELL_WIFI_LOC_METHOD);
	}
	QL_LBS_WIFI_LOG("wifi_num:%d, loc_method=%d", client_ptr->wifi_num, loc_method);
	user_option.pdp_cid = client_ptr->pdp_cid;
	user_option.sim_id = 0;
	user_option.req_timeout = client_ptr->req_timeout;
	user_option.basic_info = &client_ptr->basic_info;
	user_option.auth_info = &client_ptr->auth_info;
	user_option.wifi_num = client_ptr->wifi_num;
	user_option.wifi_info = &client_ptr->wifi_info[0];	
	user_option.cell_num = 1;
	user_option.cell_info = client_ptr->cell_info;
	QL_LBS_WIFI_LOG("cell infoinfo: radio=%d,mcc=%d,mnc=%d,lac_id=%x,cell_id=%x,signal=%d,tac =%x,bcch=%d,bsic=%d,uarfcndl=%d,psc=%d,rsrq=%d,pci=%d,earfcn=%d",\
	client_ptr->cell_info->radio,client_ptr->cell_info->mcc,client_ptr->cell_info->mnc,client_ptr->cell_info->lac_id,client_ptr->cell_info->cell_id,client_ptr->cell_info->signal,client_ptr->cell_info->tac,\
	client_ptr->cell_info->bcch,client_ptr->cell_info->bsic,client_ptr->cell_info->uarfcndl,client_ptr->cell_info->psc,client_ptr->cell_info->rsrq,client_ptr->cell_info->pci,client_ptr->cell_info->earfcn);
	ret = ql_lbs_get_position(&client_ptr->lbs_client, client_ptr->host, &user_option, ql_lbs_wifi_result_cb, (void *)client_ptr);	
    QL_LBS_WIFI_LOG("ret:%08X", ret);

	if (QL_LBS_OK != ret)
    {
        send_event.id = QLBS_WIFI_EVENT_END;
	    send_event.param1 = (uint32)client_ptr;
	    ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&send_event, 10);
    }
}

static void ql_lbs_app_run(void *param)
{
	int ret = 0;
	int flags_break = 0;
	int profile_idx = 1;
	int run_cnt = 0;
	ql_event_t qlbs_wifi_event_msg = {0};
	qlbs_wifi_ctx_t *lbs_client = NULL;
	ql_event_t send_event = {0};
	
	lbs_client = (qlbs_wifi_ctx_t *)param;
	if (NULL == lbs_client) 
	{
		QL_LBS_WIFI_LOG("ql_rtos_mutex_create failed!!!!");
		return;
	}

	/*
	Three positioning methods
	(1) WIFI positioning, LBS_WIFI_LOC_METHOD
	(2) CELL base station positioning, LBS_CELL_LOC_METHOD
	(3) WIFI+CELL hybrid positioning, LBS_CELL_WIFI_LOC_METHOD
	
	Note: For hybrid positioning, you need to obtain cell information first, then turn on QIFISCAN, scan WIFI, and finally turn off WIFISCAN.
	*/	
	lbs_client->pdp_cid = profile_idx;
	lbs_client->timeout = 120;
	lbs_client->task = lbs_wifi_task;
	lbs_client->loc_method = LBS_CELL_WIFI_LOC_METHOD;
	
	ret = ql_rtos_timer_create(&lbs_client->timer, lbs_client->task, ql_lbs_wifi_timeout_cb, lbs_client);
	if (ret != QL_OSI_SUCCESS) 
	{
		QL_LBS_WIFI_LOG("ql_rtos_mutex_create failed!!!!");
		return;
	}
	
	ret = ql_rtos_mutex_create(&lbs_client->lock);
	if (ret != QL_OSI_SUCCESS) 
	{
		QL_LBS_WIFI_LOG("ql_rtos_mutex_create failed!!!!");
		return;
	}
	
	ret = ql_rtos_queue_create(&lbs_client->queue, sizeof(ql_event_t), QLBS_WIFI_MAX_MSG_CNT);
	if (ret != QL_OSI_SUCCESS) 
	{
		QL_LBS_WIFI_LOG("ql_rtos_queue_create failed!!!!");
		return;
	}
	
	QL_LBS_WIFI_LOG("==========lbs running==========");
	ret = ql_lbs_cell_info_get(lbs_client);
	if (ret != QL_OSI_SUCCESS) 
	{
		QL_LBS_WIFI_LOG("ql_lbs_cell_info_get failed!!!!");
		return;
	}
	
	send_event.param1 = (uint32)lbs_client;
	send_event.id = QLBS_WIFI_EVENT_WIFISCAN_START;
	ql_rtos_queue_release(lbs_client->queue, sizeof(ql_event_t), (uint8 *)&send_event, 10);
	
	flags_break = 0;
	for(;;)
	{
		memset(&qlbs_wifi_event_msg, 0x00, sizeof(ql_event_t));
			
		ql_rtos_queue_wait(lbs_client->queue, (uint8 *)&qlbs_wifi_event_msg, sizeof(ql_event_t), QL_WAIT_FOREVER);
		switch(qlbs_wifi_event_msg.id)
		{
			case QLBS_WIFI_EVENT_WIFISCAN_START:
			{
				ql_lbs_wifi_timer_start(lbs_client);
				ql_lbs_wifi_scan_async_start(&qlbs_wifi_event_msg);
			}
			break;

			case QLBS_WIFI_EVENT_LBS_START:
			{
				ql_wifiscan_close();
				ql_lbs_wifi_start_func(&qlbs_wifi_event_msg);
			}
			break;
			
			case QLBS_WIFI_EVENT_END:
			{
				ql_lbs_wifi_timer_stop(lbs_client);
				ql_wifiscan_close();
				if(run_cnt++ > 9){
					flags_break = 1;
				}else{
					/*Test: next cycle*/
					ql_rtos_task_sleep_s(5);
					send_event.id = QLBS_WIFI_EVENT_WIFISCAN_START;
					ql_rtos_queue_release(lbs_client->queue, sizeof(ql_event_t), (uint8 *)&send_event, 10);
				}
			}
			break;
			
			default:
				break;
		}

		if(flags_break)
			break;
	}

}

static void ql_lbs_app_destroy(void *param)
{
	qlbs_wifi_ctx_t *lbs_client = NULL;
	
	lbs_client = (qlbs_wifi_ctx_t *)param;
	if (NULL == lbs_client) 
	{
		QL_LBS_WIFI_LOG("qlbs_wifi_ctx_t NULL!");
		return;
	}

	if(lbs_client->timer != NULL)
	{
		if( ql_rtos_timer_is_running(lbs_client->timer) == true )
			ql_rtos_timer_stop(lbs_client->timer);
		ql_rtos_timer_delete(lbs_client->timer);
		lbs_client->timer = NULL;
	}
	
	if(lbs_client->wifi_info != NULL)
	{
		free(lbs_client->wifi_info);
		lbs_client->wifi_info = NULL;
	}
	
	if(lbs_client->cell_info != NULL)
	{
		free(lbs_client->cell_info);
		lbs_client->cell_info = NULL;
	}
	
	if(lbs_client->queue != NULL)
	{
		ql_rtos_queue_delete(lbs_client->queue);
	}

	if(lbs_client->lock != NULL)
	{
		ql_rtos_mutex_delete(lbs_client->lock);
	}

	free(lbs_client);
	lbs_client = NULL;
}

static void lbs_wifi_app_thread(void *arg)
{
	int i = 0;
	int ret = 0;
	uint8_t nSim = 0;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};

	ql_rtos_task_sleep_s(10);
	QL_LBS_WIFI_LOG("==========lbs wifi demo start ==========");
	QL_LBS_WIFI_LOG("wait for network register done");

	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_LBS_WIFI_LOG("====network registered!!!!====");
	}else{
		QL_LBS_WIFI_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	QL_LBS_WIFI_LOG("===start data call====");
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
	QL_LBS_WIFI_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_LBS_WIFI_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0){
		QL_LBS_WIFI_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    QL_LBS_WIFI_LOG("info->profile_idx: %d", info.profile_idx);
	QL_LBS_WIFI_LOG("info->ip_version: %d", info.ip_version);
            
	QL_LBS_WIFI_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_LBS_WIFI_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_LBS_WIFI_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_LBS_WIFI_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);
	

	lbs_wifi_client = malloc(sizeof(qlbs_wifi_ctx_t));
	if (NULL == lbs_wifi_client) 
	{
		QL_LBS_WIFI_LOG("malloc failed!!!!");
		goto exit;
	}
	
	memset(lbs_wifi_client, 0x00, sizeof(qlbs_wifi_ctx_t));
	ql_lbs_app_run(lbs_wifi_client);
exit:
	QL_LBS_WIFI_LOG("==========lbs wifi demo exit ==========");
	ql_lbs_app_destroy(lbs_wifi_client);
	ql_rtos_task_delete(NULL);	

	return;
}

int ql_lbs_wifi_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
    err = ql_rtos_task_create(&lbs_wifi_task, 16*1024, APP_PRIORITY_ABOVE_NORMAL, "QlbsWifiApp", lbs_wifi_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_LBS_WIFI_LOG("QlbsWifiApp init failed");
	}

	return err;
}

