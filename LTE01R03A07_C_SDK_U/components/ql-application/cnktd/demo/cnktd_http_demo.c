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

#include "ql_api_osi.h"

#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_http_client.h"
#include "ql_fs.h"
#include "ql_cnktd_http.h"
#include "ql_cnktd_nw.h"
#include "ql_api_nw.h"

#define CNKTD_HTTP_LOG(msg, ...)			QL_LOG(QL_LOG_LEVEL_INFO, "ql_cnktd_http", msg, ##__VA_ARGS__)

typedef enum{
	QHTTPC_EVENT_RESPONSE     	= 1001,
	QHTTPC_EVENT_END,
}qhttpc_event_code_e;

typedef struct
{
	qapi_Net_HTTPc_handle_t     http_client;
	ql_queue_t 					queue;

	ql_mutex_t                	simple_lock;
	bool						dl_block;
	int							dl_high_line;
	int							dl_total_len;

	QFILE						upload_fd;
	QFILE						dload_fd;
}qhttpc_ctx_t;


#define 		HTTP_MAX_MSG_CNT		8
#define 		HTTP_DLOAD_HIGH_LINE	40960
ql_task_t 		cnktd_http_task = NULL;
qhttpc_ctx_t 	http_demo_client = {0};

extern void cnktd_set_data_call_param(qapi_DSS_Hndl_t dss_handle);

static void http_event_cb(http_client_t *client, int evt, int evt_code, void *arg)
{
	qhttpc_ctx_t *client_ptr = (qhttpc_ctx_t *)arg;
	ql_event_t qhttpc_event_send = {0};

	CNKTD_HTTP_LOG("enter");
	
	if(client_ptr == NULL)
		return;
	
	CNKTD_HTTP_LOG("*client:%d, http_cli:%d", *client, client_ptr->http_client);

	if((qapi_Net_HTTPc_handle_t)*client != client_ptr->http_client)
		return;
	CNKTD_HTTP_LOG("evt:%d, evt_code:%d", evt, evt_code);
	switch(evt){
	case HTTP_EVENT_SESSION_ESTABLISH:{
			if(evt_code != HTTP_SUCCESS){
				CNKTD_HTTP_LOG("HTTP session create failed!!!!!");
				qhttpc_event_send.id = QHTTPC_EVENT_END;
				qhttpc_event_send.param1 = (uint32)client_ptr;
				ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&qhttpc_event_send, QL_WAIT_FOREVER);
			}
		}
		break;
	case HTTP_EVENT_RESPONE_STATE_LINE:{
			if(evt_code == HTTP_SUCCESS){
				int resp_code = 0;
				int content_length = 0;
				int chunk_encode = 0;
				char *location = NULL;
				ql_httpc_getinfo(client, HTTP_INFO_RESPONSE_CODE, &resp_code);
				CNKTD_HTTP_LOG("response code:%d", resp_code);
				ql_httpc_getinfo(client, HTTP_INFO_CHUNK_ENCODE, &chunk_encode);
				if(chunk_encode == 0){
					ql_httpc_getinfo(client, HTTP_INFO_CONTENT_LEN, &content_length);
					CNKTD_HTTP_LOG("content_length:%d",content_length);
				}else{
					CNKTD_HTTP_LOG("http chunk encode!!!");
				}

				if(resp_code >= 300 && resp_code < 400){
					ql_httpc_getinfo(client, HTTP_INFO_LOCATION, &location);
					CNKTD_HTTP_LOG("redirect location:%s", location);
					free(location);
				}
			}
		}
		break;
	case HTTP_EVENT_SESSION_DISCONNECT:{
			if(evt_code == HTTP_SUCCESS){
				CNKTD_HTTP_LOG("===http transfer end!!!!");
			}else{
				CNKTD_HTTP_LOG("===http transfer occur exception!!!!!");
			}			
			qhttpc_event_send.id = QHTTPC_EVENT_END;
			qhttpc_event_send.param1 = (uint32)client_ptr;
			ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&qhttpc_event_send, QL_WAIT_FOREVER);
		}
		break;
	}
}

static int http_write_response_data(http_client_t *client, void *arg, char *data, int size, unsigned char end)
{
	int ret = size;
	uint32 msg_cnt = 0;
	char *read_buff = NULL;
	qhttpc_ctx_t *client_ptr = (qhttpc_ctx_t *)arg;
	ql_event_t qhttpc_event_send = {0};

	CNKTD_HTTP_LOG("enter");	
	
	if(client_ptr == NULL)
		return 0;
	
	CNKTD_HTTP_LOG("*client:%d, http_cli:%d", *client, client_ptr->http_client);

	if((qapi_Net_HTTPc_handle_t)*client != client_ptr->http_client)
		return 0;

	read_buff = (char *)malloc(size+1);
	if(read_buff == NULL)
	{
		CNKTD_HTTP_LOG("mem faild");
		return 0;
	}

	memcpy(read_buff, data, size);

	if(QL_OSI_SUCCESS != ql_rtos_queue_get_cnt(client_ptr->queue, &msg_cnt))
	{
		free(read_buff);
		CNKTD_HTTP_LOG("ql_rtos_queue_get_cnt faild");
		return 0;
	}
	
	ql_rtos_mutex_lock(client_ptr->simple_lock, 100);
	if(msg_cnt >= (HTTP_MAX_MSG_CNT-1) || (client_ptr->dl_total_len + size) >= client_ptr->dl_high_line)
	{
		client_ptr->dl_block = true;
		ret = HTTP_ERR_WOUNDBLOCK;
	}
	ql_rtos_mutex_unlock(client_ptr->simple_lock);

	CNKTD_HTTP_LOG("msg_cnt %d, total_len+size %d", msg_cnt, (client_ptr->dl_total_len + size));

	qhttpc_event_send.id = QHTTPC_EVENT_RESPONSE;
	qhttpc_event_send.param1 = (uint32)client_ptr;
	qhttpc_event_send.param2 = (uint32)read_buff;
	qhttpc_event_send.param3 = (uint32)size;
	if(QL_OSI_SUCCESS != ql_rtos_queue_release(client_ptr->queue, sizeof(ql_event_t), (uint8 *)&qhttpc_event_send, 0))
	{
		free(read_buff);
		CNKTD_HTTP_LOG("ql_rtos_queue_release faild");
		return 0;
	}
	
	ql_rtos_mutex_lock(client_ptr->simple_lock, 100);
	client_ptr->dl_total_len += size;
	ql_rtos_mutex_unlock(client_ptr->simple_lock);
	
	CNKTD_HTTP_LOG("http response :%d bytes data", size);
	
	return ret;
}

static int http_read_request_data(http_client_t *client, void *arg, char *data, int size)
{
	int ret = 0;
	QFILE fd = 0;
	qhttpc_ctx_t *client_ptr = (qhttpc_ctx_t *)arg;
	
	CNKTD_HTTP_LOG("enter");	
	
	if(client_ptr == NULL)
		return 0;
	
	CNKTD_HTTP_LOG("*client:%d, http_cli:%d", *client, client_ptr->http_client);

	if((qapi_Net_HTTPc_handle_t)*client != client_ptr->http_client)
		return 0;

	ql_rtos_mutex_lock(client_ptr->simple_lock, 100);
	fd = client_ptr->upload_fd;
	ql_rtos_mutex_unlock(client_ptr->simple_lock);

	CNKTD_HTTP_LOG("fd:%d", fd);	
	
	if(fd < 0)
		return 0;
	CNKTD_HTTP_LOG("read size:%d", size);
	ret = ql_fread(data, size, 1, fd);
	CNKTD_HTTP_LOG("http read :%d bytes data", ret);
	if(ret > 0)
		return ret;

	return 0;
}

static void http_write_response_data_func(void *param)
{
	int ret = 0;
	int size = 0;
	QFILE fd = 0;
	bool dload_block = false;
	char *read_buff = NULL;
	qhttpc_ctx_t *client_ptr = NULL;
	ql_event_t *qhttpc_event_recv = (ql_event_t *)param;

	if(qhttpc_event_recv == NULL || qhttpc_event_recv->param1 == 0 || qhttpc_event_recv->param2 == 0 || qhttpc_event_recv->param3 == 0)
		return;

	client_ptr = (qhttpc_ctx_t *)qhttpc_event_recv->param1;
	read_buff = (char *)qhttpc_event_recv->param2;
	size = (int)qhttpc_event_recv->param3;
	fd = (QFILE)client_ptr->dload_fd;

	ret = ql_fwrite(read_buff, size, 1, fd);

	free(read_buff);

	ql_rtos_mutex_lock(client_ptr->simple_lock, 100);
	client_ptr->dl_total_len -= size;
	if(client_ptr->dl_total_len < 0)
		client_ptr->dl_total_len = 0;
	if(client_ptr->dl_block == true && client_ptr->dl_total_len < client_ptr->dl_high_line)
	{
		dload_block = client_ptr->dl_block;
		client_ptr->dl_block = false;
	}
	ql_rtos_mutex_unlock(client_ptr->simple_lock);

	if(dload_block == true)
		ql_httpc_continue_dload(client_ptr->http_client);

	CNKTD_HTTP_LOG("http write :%d bytes data", ret);
}

static void cnktd_http_demo_task(void * arg)
{
    int ret = 0, i = 0;
    int profile_idx = 1;
	int run_num = 0;
	struct stat dload_stat;
	uint8_t nSim = 0;
	int flags_break = 0;
	ql_event_t qhttpc_event_msg = {0};
	qapi_Net_HTTPc_Method_e HTTPc_Method = 0;
	char upload_str[] = "Message=1111&Appleqty=2222&Orangeqty=3333&find=1";
	char dload_file[] = "UFS:http_dload.txt";
	char upload_file[] = "UFS:test_device.txt";
	qapi_DSS_Hndl_t dss_handle = NULL;
	qapi_QT_NW_DS_Profile_PDP_Context_t profile = {0};
    uint8_t pdp_context_number = 1;
	uint8_t ip_addr_cnt = 1;
    char ip_v4[16] = {0};
    char ip_v6[130] = {0};
    char ip_v4_sub[16] = {0};
    char gt_v4[16] = {0};
    char gt_v6[130] = {0};
    char dp_v4[16] = {0};
    char ds_v4[16] = {0};
    char dp_v6[130] = {0};
    char ds_v6[130] = {0};
	
	ql_rtos_task_sleep_s(5);
	CNKTD_HTTP_LOG("========== socket demo start ==========");
	CNKTD_HTTP_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		CNKTD_HTTP_LOG("====network registered!!!!====");
	}else{
		CNKTD_HTTP_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);
    qapi_DSS_Get_Data_Srvc_Hndl(NULL, NULL, &dss_handle);
    if (dss_handle == NULL)
    {
        CNKTD_HTTP_LOG("dss_handle is NULL");
        goto exit;
    }
    cnktd_set_data_call_param(dss_handle);
	
	CNKTD_HTTP_LOG("===start data call====");
    ret = qapi_DSS_Start_Data_Call(dss_handle);
    CNKTD_HTTP_LOG("qapi_DSS_Start_Data_Call, err = %d", ret);

    ret = qapi_QT_NW_PDP_Cfg_Get(&pdp_context_number, &profile);
	if (ret < 0)
    {
        qapi_DSS_Stop_Data_Call(dss_handle);
        CNKTD_HTTP_LOG("qapi_QT_NW_PDP_Cfg_Get failed");
        goto exit;
    }
    CNKTD_HTTP_LOG("qapi_QT_NW_PDP_Cfg_Get err = %d, apn=%s, pdp_type=%d", ret, profile.apn, profile.pdp_type);

    if(profile.pdp_type == QT_NW_DS_PROFILE_PDP_IPV4V6){
        ip_addr_cnt = 2;
    }

    if(ip_addr_cnt)
    {
        qapi_DSS_Addr_Info_t *qapi_DSS_Addr_Info_ptr = calloc(1, sizeof(qapi_DSS_Addr_Info_t) * ip_addr_cnt);
        qapi_DSS_Addr_Info_t *info_ptr = qapi_DSS_Addr_Info_ptr;
        if(info_ptr == NULL)
        {
            CNKTD_HTTP_LOG("calloc info_ptr failed");
			qapi_DSS_Stop_Data_Call(dss_handle);
            goto exit;
        }

        ret = qapi_DSS_Get_IP_Addr(dss_handle, info_ptr, ip_addr_cnt);
        CNKTD_HTTP_LOG("qapi_DSS_Get_IP_Addr err = %d", ret);
        if(ret == QAPI_OK)
        {
            if(profile.pdp_type == QAPI_DSS_IP_VERSION_4 || profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
            {
                CNKTD_HTTP_LOG("V4, ip=%s, subnet mask=%s, gateway=%s, Primary DNS=%s, Secondary DNS=%s",
                                inet_ntop(AF_INET, &info_ptr->iface_addr_s.addr.v4, ip_v4, sizeof(ip_v4)),
                                inet_ntop(AF_INET, &info_ptr->iface_mask, ip_v4_sub, sizeof(ip_v4_sub)),
                                inet_ntop(AF_INET, &info_ptr->gtwy_addr_s.addr.v4, gt_v4, sizeof(gt_v4)),
                                inet_ntop(AF_INET, &info_ptr->dnsp_addr_s.addr.v4, dp_v4, sizeof(dp_v4)),
                                inet_ntop(AF_INET, &info_ptr->dnss_addr_s.addr.v4, ds_v4, sizeof(ds_v4)));
                if(profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
                {
                    info_ptr++;
                }
            }

            if(profile.pdp_type == QAPI_DSS_IP_VERSION_6 || profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
            {
                CNKTD_HTTP_LOG("V6, ip=%s, prefix length=%u, gateway=%s, gateway length=%u, Primary DNS=%s, Secondary DNS=%s",
                            inet_ntop(AF_INET6, &info_ptr->iface_addr_s.addr.v6_addr64, ip_v6, sizeof(ip_v6)),
                            info_ptr->iface_mask,
                            inet_ntop(AF_INET6, &info_ptr->gtwy_addr_s.addr.v6_addr64, gt_v6, sizeof(gt_v6)),
                            info_ptr->gtwy_mask,
                            inet_ntop(AF_INET6, &info_ptr->dnsp_addr_s.addr.v6_addr64, dp_v6, sizeof(dp_v6)),
                            inet_ntop(AF_INET6, &info_ptr->dnss_addr_s.addr.v6_addr64, ds_v6, sizeof(ds_v6)));
            }
        }
		else
	    {
	        free(qapi_DSS_Addr_Info_ptr);
	        qapi_DSS_Stop_Data_Call(dss_handle);
	        CNKTD_HTTP_LOG("qapi_DSS_Get_IP_Addr failed");
	        goto exit;
	    }
        free(qapi_DSS_Addr_Info_ptr);
    }


	http_demo_client.upload_fd = ql_fopen(upload_file, "w+");
	if(http_demo_client.upload_fd < 0){
		ql_fclose(http_demo_client.upload_fd);
		goto exit;
	}
	else
	{
		if(ql_fwrite(upload_str, strlen(upload_str) + 1, 1, http_demo_client.upload_fd) < 0)
		{
			ql_fclose(http_demo_client.upload_fd);
			goto exit;
		}
	}
	ql_fclose(http_demo_client.upload_fd);

	while(run_num < 100){
		int http_method = HTTP_METHOD_NONE;
		int case_id = run_num%4;
	    char url[256] = {0};

		CNKTD_HTTP_LOG("==============http_client_test[%d]================\n",run_num+1);

		memset(&http_demo_client, 0x00, sizeof(qhttpc_ctx_t));

		http_demo_client.dl_block = false;
		http_demo_client.dl_high_line = HTTP_DLOAD_HIGH_LINE;

		ret = ql_rtos_mutex_create(&http_demo_client.simple_lock);
		if (ret) 
		{
			CNKTD_HTTP_LOG("ql_rtos_mutex_create failed!!!!");
			break;
		}

		ret = ql_rtos_queue_create(&http_demo_client.queue, sizeof(ql_event_t), HTTP_MAX_MSG_CNT);
		if (ret) 
		{
			CNKTD_HTTP_LOG("ql_rtos_queue_create failed!!!!");
			break;
		}

		http_demo_client.http_client = qapi_Net_HTTPc_New_sess(60000, 1, http_event_cb, (void *)&http_demo_client, 1024, 1024);
		if(http_demo_client.http_client == NULL){
			CNKTD_HTTP_LOG("http client create failed!!!!");
			break;
		}
		
		http_demo_client.dload_fd = ql_fopen(dload_file, "w+");
		if(http_demo_client.dload_fd < 0){
			CNKTD_HTTP_LOG("open file failed!!!!");
			qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
			goto exit;
		}	
		
		ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_SIM_ID, nSim);
		ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_PDPCID, profile_idx);
		ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_WRITE_FUNC, http_write_response_data);
		ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_WRITE_DATA, (void *)&http_demo_client);
		
		switch(case_id){		
		case 0:{//test http get 
				strncpy(url,"http://112.31.84.164:8300/10K.txt",strlen("http://112.31.84.164:8300/10K.txt"));
				http_method = HTTP_METHOD_GET;
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_METHOD, http_method);
			}
			break;
		case 1:{//test https get
		        strncpy(url,"https://112.31.84.164:8301/10K.txt",strlen("https://112.31.84.164:8301/10K.txt"));
				http_method = HTTP_METHOD_GET;
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_METHOD, http_method);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_SSL_VERIFY_LEVEL, HTTPS_VERIFY_NONE);
			}
			break;
		case 2:{//test http post			
				strncpy(url,"http://112.31.84.164:8300/processorder.php",strlen("http://112.31.84.164:8300/processorder.php"));
		    	struct stat stat_buf;
				http_method = HTTP_METHOD_POST;
				http_demo_client.upload_fd = ql_fopen(upload_file, "r");
				if(http_demo_client.upload_fd < 0){
					ql_fclose(http_demo_client.dload_fd);
					qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
					goto exit;
				}
				memset(&stat_buf, 0x00, sizeof(struct stat));

				ql_fstat(http_demo_client.upload_fd, &stat_buf);
				CNKTD_HTTP_LOG("file size:%d", stat_buf.st_size);
				if(stat_buf.st_size == 0){
					ql_fclose(http_demo_client.upload_fd);
					ql_fclose(http_demo_client.dload_fd);
					qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
				}	
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_METHOD, http_method);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_READ_FUNC, http_read_request_data);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_READ_DATA, (void *)&http_demo_client);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_UPLOAD_LEN, stat_buf.st_size);
			}
			break;
		case 3:{//test http put
				strncpy(url,"http://112.31.84.164:8300/processorder.php",strlen("http://112.31.84.164:8300/processorder.php"));
				struct stat stat_buf;
				http_method = HTTP_METHOD_PUT;
				http_demo_client.upload_fd = ql_fopen(upload_file, "r");
				if(http_demo_client.upload_fd < 0){
					ql_fclose(http_demo_client.dload_fd);
					qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
					goto exit;
				}
				memset(&stat_buf, 0x00, sizeof(struct stat));
				
				ql_fstat(http_demo_client.upload_fd, &stat_buf);
				CNKTD_HTTP_LOG("file size:%d", stat_buf.st_size);
				if(stat_buf.st_size == 0){
					ql_fclose(http_demo_client.upload_fd);
					ql_fclose(http_demo_client.dload_fd);
					qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
				}
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_METHOD, http_method);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_READ_FUNC, http_read_request_data);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_READ_DATA, (void *)&http_demo_client);
				ql_httpc_setopt((http_client_t *)&http_demo_client.http_client, HTTP_CLIENT_OPT_UPLOAD_LEN, stat_buf.st_size);
			}
			break;
		}
		
		if(qapi_Net_HTTPc_Request(http_demo_client.http_client, HTTPc_Method, url) == QAPI_OK)
		{
			CNKTD_HTTP_LOG("wait http perform end!!!!!!");

			flags_break = 0;
			for (;;)
			{
				memset(&qhttpc_event_msg, 0x00, sizeof(ql_event_t));
				
				ql_rtos_queue_wait(http_demo_client.queue, (uint8 *)&qhttpc_event_msg, sizeof(ql_event_t), QL_WAIT_FOREVER);

				switch(qhttpc_event_msg.id)
				{
					case QHTTPC_EVENT_RESPONSE:
					{
						http_write_response_data_func((void *)&qhttpc_event_msg);
					}
						break;
					case QHTTPC_EVENT_END:
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
		}else{
			CNKTD_HTTP_LOG("http perform failed!!!!!!!!!!");
		}
		memset(&dload_stat, 0x00, sizeof(struct stat));
		ql_fstat(http_demo_client.dload_fd, &dload_stat);
		CNKTD_HTTP_LOG("=========dload_file_size:%d", dload_stat.st_size);
		if(http_demo_client.dload_fd >= 0){
			ql_fclose(http_demo_client.dload_fd);
			http_demo_client.dload_fd = -1;
		}
		ql_rtos_mutex_lock(http_demo_client.simple_lock, 100);
		if(http_demo_client.upload_fd >= 0){
			ql_fclose(http_demo_client.upload_fd);
			http_demo_client.upload_fd = -1;
		}
		ql_rtos_mutex_unlock(http_demo_client.simple_lock);
		
		qapi_Net_HTTPc_Free_sess(http_demo_client.http_client);
		http_demo_client.http_client = NULL;
		CNKTD_HTTP_LOG("==============http_client_test_end[%d]================\n",run_num+1);
		run_num++;

		if(http_demo_client.queue != NULL)
		{
			while(1)
			{
				memset(&qhttpc_event_msg, 0x00, sizeof(ql_event_t));
				
				if(QL_OSI_SUCCESS != ql_rtos_queue_wait(http_demo_client.queue, (uint8 *)&qhttpc_event_msg, sizeof(ql_event_t), 0))
					break;

				switch(qhttpc_event_msg.id)
				{
					case QHTTPC_EVENT_RESPONSE:
					{
						free((void *)(qhttpc_event_msg.param2));
					}
						break;
					default:
						break;
				}
			}
			ql_rtos_queue_delete(http_demo_client.queue);
			http_demo_client.queue = NULL;
		}

		ql_rtos_mutex_delete(http_demo_client.simple_lock);
		http_demo_client.simple_lock = NULL;
		
		ql_rtos_task_sleep_s(3);
	}
exit:
	if(http_demo_client.queue != NULL)
	{
		while(1)
		{
			memset(&qhttpc_event_msg, 0x00, sizeof(ql_event_t));
			
			if(QL_OSI_SUCCESS != ql_rtos_queue_wait(http_demo_client.queue, (uint8 *)&qhttpc_event_msg, sizeof(ql_event_t), 0))
				break;

			switch(qhttpc_event_msg.id)
			{
				case QHTTPC_EVENT_RESPONSE:
				{
					free((void *)(qhttpc_event_msg.param2));
				}
					break;
				default:
					break;
			}
		}
		ql_rtos_queue_delete(http_demo_client.queue);
	}

	if(http_demo_client.simple_lock != NULL)
		ql_rtos_mutex_delete(http_demo_client.simple_lock);

   if(dss_handle != NULL)
   {
	   qapi_DSS_Rel_Data_Srvc_Hndl(dss_handle);
   }
  	ql_rtos_task_delete(cnktd_http_task);	
    return;		
}

void cnktd_http_demo_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    
    err = ql_rtos_task_create(&cnktd_http_task, 4096, APP_PRIORITY_NORMAL, "cnktd_http_demo", cnktd_http_demo_task, NULL, 5);
	if(err != QL_OSI_SUCCESS)
	{
		CNKTD_HTTP_LOG("created task failed");
	}
}

