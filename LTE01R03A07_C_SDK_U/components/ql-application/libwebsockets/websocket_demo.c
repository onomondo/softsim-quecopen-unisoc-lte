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

#include "ql_api_osi.h"
#include "ql_api_nw.h"
#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_ssl.h"
#include "ql_api_websocket.h"

static ql_task_t websocket_task = NULL;

#define QL_WEBSOCKET_PDP_CID				1
#define QL_WEBSOCKET_SSL_CTX_ID				1
#define WS_MAX_TEST_DATA_LEN				1360
#define WS_MAX_MSG_CNT						128

typedef enum{
	QWS_EVENT_OPEN     	= 1001,
	QWS_EVENT_RECV,	
	QWS_EVENT_CLOSE,
}qws_event_code_e;
	

typedef struct
{
	int  			  			client_id;
	int  			  			cfg_id;
	ql_queue_t 					queue;
	ql_mutex_t                	ws_lock;

}qws_ctx_t;
qws_ctx_t 	ws_demo_client = {0};

int ql_ws_service_cb(int client_id, qws_parm_t *cb_t)
{
   	int ret = QWS_OK;
	ql_event_t ws_event_send = {0};
	if(NULL == cb_t) goto exit;

    QL_WEBSOCKET_LOG("qws service_cb %d, type=%d",client_id, cb_t->type);
	switch (cb_t->type)
	{
		case QWS_CB_TYPE_OPEN: {
			//open event
			QL_WEBSOCKET_LOG("open code %d",cb_t->virtual);
			
			ws_event_send.id = QWS_EVENT_OPEN;
			ws_event_send.param1 = (uint32)client_id;
			ws_event_send.param2 = (uint32)cb_t->virtual;
			ret = ql_rtos_queue_release(ws_demo_client.queue, sizeof(ql_event_t), (uint8 *)&ws_event_send, 0);
			if(QL_OSI_SUCCESS != ret)
			{
				QL_WEBSOCKET_LOG("release faild");
			}
		}
		break;

		/* recv event*/
		case QWS_CB_TYPE_RECV: {
			//ToDo: 
			//Cache and message notification processing data
			QL_WEBSOCKET_LOG("recv %d, %s",cb_t->size, cb_t->data);
		}
		break;
		
		case QWS_CB_TYPE_CLOSE: {
			//disconnect event
			//ToDo:
			//Abnormal reconnection is performed later

			QL_WEBSOCKET_LOG("disconn errcode %d",cb_t->virtual);
		}
		break;

		default:
			break;
	}
	
exit:	
	return ret;
}

static int datacall_satrt(void)
{
   	int ret = 0;
	int i = 0;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	uint8_t nSim = 0;
	
	ql_rtos_task_sleep_s(10);
	QL_WEBSOCKET_LOG("========== websocket demo start ==========");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_WEBSOCKET_LOG("====network registered!!!!====");
	}else{
		QL_WEBSOCKET_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, QL_WEBSOCKET_PDP_CID, 0);

	QL_WEBSOCKET_LOG("===start data call====");
	ret=ql_start_data_call(nSim, QL_WEBSOCKET_PDP_CID, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
	QL_WEBSOCKET_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_WEBSOCKET_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, QL_WEBSOCKET_PDP_CID, &info);
	if(ret != 0){
		QL_WEBSOCKET_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, QL_WEBSOCKET_PDP_CID);
		goto exit;
	}
    QL_WEBSOCKET_LOG("info->QL_WEBSOCKET_PDP_CID: %d", info.profile_idx);
	QL_WEBSOCKET_LOG("info->ip_version: %d", info.ip_version);
            
	QL_WEBSOCKET_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_WEBSOCKET_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_WEBSOCKET_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_WEBSOCKET_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

	return 0;
exit:
    return -1;
}

static void websocket_app_thread(void * arg)
{
	int ret = 0;
	int flags_break = 0;
	uint32_t client_id = 0; /*max value QWS_CLIENT_ID_NUM*/
	uint32_t cfg_id = 0; /*max value QWS_CFG_ID_NUM*/
	char* test_data = NULL;	
	qws_open_t open_t;
	ql_event_t qws_event_msg = {0};
	char url[] = "ws://220.180.239.212:8189/v1";

	/*step1: wait card init done and active*/
	ret = datacall_satrt();
	if (ret < 0) {
		QL_WEBSOCKET_LOG("websocket active ret=%d", ret);
		goto exit;
	}
	test_data = malloc(WS_MAX_TEST_DATA_LEN);
	if (NULL == test_data){
		goto exit;
	}
	ret = ql_rtos_queue_create(&ws_demo_client.queue, sizeof(ql_event_t), WS_MAX_MSG_CNT);
	if (ret != QL_OSI_SUCCESS) 
	{
		QL_WEBSOCKET_LOG("queue_create failed!!!!");
		goto exit;
	}

	/*step2: init config */
	ql_ws_cfg_set_opt(cfg_id, QWS_CFG_CONN_PDPCID, QL_WEBSOCKET_PDP_CID);
	ql_ws_cfg_set_opt(cfg_id, QWS_CFG_CONN_SSLCTXID, QL_WEBSOCKET_SSL_CTX_ID);
	ql_ws_cfg_set_opt(cfg_id, QWS_CFG_CONN_URL, (char *)url);
	ql_ws_cfg_set_opt(cfg_id, QWS_CFG_RECV_CB, ql_ws_service_cb);

	/*step3: connect open*/
	memset(&open_t , 0, sizeof(qws_open_t));
	open_t.cfg_id = cfg_id;
	open_t.client_id = client_id; 
	ret = ql_ws_open_proc(&open_t);
	if (ret != QWS_OK){
		QL_WEBSOCKET_LOG("websocket open err=%d", ret);
		goto exit;
	}
	//ToDo 

	/*step5: recv data*/
	QL_WEBSOCKET_LOG("websocket running");
	//ToDo:

	flags_break = 0;
	for (;;)
	{
	
		memset(&qws_event_msg, 0x00, sizeof(ql_event_t));
		
		ql_rtos_queue_wait(ws_demo_client.queue, (uint8 *)&qws_event_msg, sizeof(ql_event_t), QL_WAIT_FOREVER);
	
		switch(qws_event_msg.id)
		{
			case QWS_EVENT_OPEN:
			{
				if(QL_OSI_SUCCESS == qws_event_msg.param2)//open success
				{
					memset(test_data , 0, WS_MAX_TEST_DATA_LEN);
					snprintf(test_data, WS_MAX_TEST_DATA_LEN, "%s", "Welcome to Websocket!");
				
					/*step4: send data*/
					qws_parm_t parm_t;
					parm_t.type = QWS_WRITE_PROT_TEXT;
					parm_t.data = test_data;
					parm_t.size = strlen(test_data);
					ret = ql_ws_write_proc(client_id, &parm_t);
					if (ret != QWS_OK){
						QL_WEBSOCKET_LOG("websocket write err=%d", ret);
					}
				}else{
					flags_break = 1;
				}
			}
				break;
			case QWS_EVENT_CLOSE:
			{
				flags_break = 1;
			}
				break;
			default:
				break;
		}

		if(flags_break)
			break;
	}
		

	/*step6: close exit*/
	ql_ws_close_proc(client_id);
		
exit:

	QL_WEBSOCKET_LOG("websocket task exit!");
	if(test_data != NULL) free(test_data);
	
	websocket_task = NULL;	
   	ql_rtos_task_delete(NULL);/*self-destruction*/

   	return;	
}


int ql_websocket_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&websocket_task, 4*1024, APP_PRIORITY_NORMAL, "wssApp", websocket_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_WEBSOCKET_LOG("websocket init failed");
	}

	return err;
}

