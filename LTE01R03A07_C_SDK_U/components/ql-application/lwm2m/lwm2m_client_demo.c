/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *    Ricky Liu - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>
 Bosch Software Innovations GmbH - Please refer to git log

*/

#include "sockets.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "ql_api_osi.h"
#include "ql_api_nw.h"

#include "ql_api_datacall.h"
#include "ql_api_datacall.h"
#include "ql_lwm2m_api.h"
#include "ql_log.h"


#define QL_LWM2M_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_LWM2M_LOG(msg, ...)			QL_LOG(QL_LWM2M_LOG_LEVEL, "ql_LWM2M_DEMO", msg, ##__VA_ARGS__)
#define QL_LWM2M_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_LWM2M_DEMO", msg, ##__VA_ARGS__)

#define 		  M2M_MAX_MSG_CNT							8
#define 		  M2M_SERVER_LIFE_TIME					    60
#define 		  M2M_SERVER_PMIN_PERIOD					50
#define 		  M2M_SERVER_PMAX_PERIOD					180
#define 		  M2M_SERVER_DISABLE_TIMEOUT_CONNECT		40

static ql_task_t  lwm2m_task = NULL;
static ql_queue_t lwm2m_queue = NULL;
static ql_timer_t lwm2m_timer = NULL;

typedef enum{
	QM2M_EVENT_REG     		= 0,
	QM2M_EVENT_UPDATE     	= 1,
	QM2M_EVENT_DEREG,
}qm2m_event_code_e;


void ql_m2m_client_event_cb(int event_id, void *arg)
{
	ql_event_t qm2m_event_msg = {0};
	static int re_start = 0;
	m2m_state_event_info *event_info =  (m2m_state_event_info*)arg;
	
	QL_LWM2M_LOG("event_id=%d, state:%d, result:%d",event_id, event_info->state, event_info->result);	
	switch(event_id)
	{
		case M2M_CLIENT_STATE_CHG_IND:
		{
				switch(event_info->state)
				{
					case M2M_CLIENT_BOOTSTRAP:
					case M2M_CLIENT_READY:
						{
							QL_LWM2M_LOG("result=%d", event_info->result);
						}
						break;
					
					case M2M_CLIENT_BOOTSTRAPING:
					case M2M_CLIENT_REGISTERING:
						{
							QL_LWM2M_LOG("result=%d", event_info->result);
						}
						break;
					
					case M2M_CLIENT_DEREG:
						{
							QL_LWM2M_LOG("result=%d", event_info->result);
						}
						break;
					default:break;
				}
				
		}
		break;

		case M2M_CLIENT_UPDATE_IND:
		{	
			QL_LWM2M_LOG("result=%d", event_info->result);
			if(0 == event_info->result)
				QL_LWM2M_LOG("update success");
			else
				QL_LWM2M_LOG("update abnormal");
		}
		break;

		case M2M_CLIENT_FW_STAT_IND:
		{	
			QL_LWM2M_LOG("result=%d", event_info->result);
			if(1 == event_info->result)
				QL_LWM2M_LOG("download start");
			else if(2 == event_info->result)
				QL_LWM2M_LOG("download end");
			else
				QL_LWM2M_LOG("download abnormal");
		}
		break;
	}

	QL_LWM2M_LOG("state:%d, result:%d", event_info->state, event_info->result);	
	if(event_info->result < 0)
	{
		if(re_start++ > 1){
			qm2m_event_msg.id = QM2M_EVENT_DEREG;
			ql_rtos_queue_release(lwm2m_queue, sizeof(ql_event_t), (uint8 *)&qm2m_event_msg, 0);
		}
	}else{
		re_start = 0;
	}

}

void m2m_timer_callback(void *ctx)
{
	ql_event_t qm2m_event_msg = {0};

	qm2m_event_msg.id = QM2M_EVENT_REG;
	ql_rtos_queue_release(lwm2m_queue, sizeof(ql_event_t), (uint8 *)&qm2m_event_msg, 0);

	return;
}

int ql_m2m_app_run(void)
{
	int err = QL_OSI_NO_MEMORY;
	ql_event_t qm2m_event_msg = {0};
	int security_mode = M2M_CLIENT_SECURITY_MODE_NULL;
	QL_LWM2M_LOG("==========lwm2m enter start ==========");
	int ssid = 123;
	const char *manufacturer = "quectel";
	const char *model_number = "quec-module";
	/*step1: init*/
	ql_m2m_client_reset(NULL);/*after mod config, execute*/

	//ql_lwm2m_client_setopt(LWM2M_CLIENT_OPT_DEL_SERVER, Bootstrap_Server_ID);
	//ql_lwm2m_client_setopt(LWM2M_CLIENT_OPT_DEL_SERVER, DM_Server_ID);/*delete previous configuration*/

	lwm2m_server_info_t 	*server_info = NULL;
	server_info = malloc(sizeof(lwm2m_server_info_t));
	if(NULL == server_info) goto exit;
	

	memset(server_info ,0, sizeof(lwm2m_server_info_t));
	server_info->valid = TRUE;
	server_info->server_id = ssid;
	memset(server_info->server_url,0, sizeof(server_info->server_url));
	server_info->bootstrap = FALSE;
	if(security_mode == M2M_CLIENT_SECURITY_MODE_PKS){
		server_info->security_mode = M2M_CLIENT_SECURITY_MODE_PKS;
		ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_EPNAME, M2M_ENDPOINT_NAME_URN_IMEI);
		strcpy(server_info->server_url, "coaps://leshan.eclipseprojects.io:5684");
		strcpy(server_info->psk_id, "urn:imei:864430010001091");
		strcpy(server_info->psk_key, "313233343536"); 	
	}
	else if(security_mode == M2M_CLIENT_SECURITY_MODE_CERTIFICATE)
	{
		server_info->security_mode = M2M_CLIENT_SECURITY_MODE_CERTIFICATE;
		ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_EPNAME, M2M_ENDPOINT_NAME_URN_IMEI);
		strcpy(server_info->server_url, "coaps://leshan.eclipseprojects.io:5684");
		strcpy(server_info->ca_cert_path, "UFS:leshan_server_cert.der");
		strcpy(server_info->client_cert_path, "UFS:leshan_client_cert.der");
		strcpy(server_info->client_key_path, "UFS:leshan_client_key.pem");
		QL_LWM2M_LOG("server_info->ca_cert_path:%s", server_info->ca_cert_path);
		QL_LWM2M_LOG("server_info->client_cert_path:%s", server_info->client_cert_path);
		QL_LWM2M_LOG("server_info->client_key_path:%s", server_info->client_key_path);
	}
	else{
		server_info->security_mode = M2M_CLIENT_SECURITY_MODE_NULL;
		ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_EPNAME, M2M_ENDPOINT_NAME_URN_IMEI_IMSI);
		strcpy(server_info->server_url, "coap://leshan.eclipseprojects.io:5683");
	}

	server_info->life_time = M2M_SERVER_LIFE_TIME;
	server_info->period_min = M2M_SERVER_PMIN_PERIOD;
	server_info->period_max = M2M_SERVER_PMAX_PERIOD;
	server_info->disable_timeout = M2M_SERVER_DISABLE_TIMEOUT_CONNECT;
	server_info->storing = FALSE;
	memset(server_info->binding, 0, 4);
	strcpy(server_info->binding, QLW_BINDING_MODE_U);
	QL_LWM2M_LOG("===lwm2m pass====");

	if(0 != ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_SERVER, DM_Server_ID, (void*)server_info)){
		goto exit;
	}
	
	if(0 != ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_DEVICE, manufacturer, model_number)){
		goto exit;
	}

	if(0 != ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_VERSION, "002.001.001")){
		goto exit;
	}

	//ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_FOTA_TRY_TIMES, 3);//Default value 2
	//int fota_auto_dload = 1;//0:not download; 1: Download
	//int fota_auto_upgrade = 1;//0: Update and upgrade after restart; 1: Update and upgrade immediately after downloading
	//ql_m2m_client_setopt(LWM2M_CLIENT_OPT_SET_FOTA, fota_dload, fota_upgrade);//Receive Package URI from server, perform download, and auto-update; default 1,1

	qm2m_event_msg.id = QM2M_EVENT_REG;
	if(QL_OSI_SUCCESS != ql_rtos_queue_release(lwm2m_queue, sizeof(ql_event_t), (uint8 *)&qm2m_event_msg, 0))
	{
		QL_LWM2M_LOG("ql_rtos_queue_release faild");
		goto exit;
	}

	while(1)
	{
		memset(&qm2m_event_msg, 0x00, sizeof(ql_event_t));
		
		if(QL_OSI_SUCCESS != ql_rtos_queue_wait(lwm2m_queue, (uint8 *)&qm2m_event_msg, sizeof(ql_event_t), QL_WAIT_FOREVER))
			break;
	
		QL_LWM2M_LOG("===lwm2m do====%d", qm2m_event_msg.id);
		switch(qm2m_event_msg.id)
		{
			case QM2M_EVENT_REG:
			{
				/*register*/
				if(ql_m2m_client_reg(NULL) != QL_OSI_SUCCESS)
				{
					qm2m_event_msg.id = QM2M_EVENT_DEREG;
					ql_rtos_queue_release(lwm2m_queue, sizeof(ql_event_t), (uint8 *)&qm2m_event_msg, 0);
				}
			}
			break;
			
			case QM2M_EVENT_UPDATE:
			{
				/*Active update*/
				if(ql_m2m_client_update(ssid, NULL) != QL_OSI_SUCCESS)
				{
					qm2m_event_msg.id = QM2M_EVENT_DEREG;
					ql_rtos_queue_release(lwm2m_queue, sizeof(ql_event_t), (uint8 *)&qm2m_event_msg, 0);
				}
			}
			break;

			case QM2M_EVENT_DEREG:
			{
				/*destory */
				if(ql_m2m_client_dereg(NULL) == QL_OSI_SUCCESS)
				{
					QL_LWM2M_LOG("dereg pass");
				}
				
				err = ql_rtos_timer_start(lwm2m_timer, M2M_SERVER_DISABLE_TIMEOUT_CONNECT*2*1000, 0);
				if(err != QL_OSI_SUCCESS)
				{
					QL_LWM2M_LOG("timer start failed");
					goto exit;
				}	
			}
			break;
			
			default:
				break;
		}
	}


exit:
	ql_rtos_queue_delete(lwm2m_queue);
	lwm2m_queue = NULL;
	ql_rtos_timer_delete(lwm2m_timer);
	lwm2m_timer = NULL;
	/*task exit*/
	if(server_info != NULL) free(server_info);
	QL_LWM2M_LOG("===lwm2m exit====");
	return err;
}

static void ql_m2m_app_thread(void * arg)
{
	int ret = QL_OSI_SUCCESS;
	int i = 0;
	int use_ipv6 = 0;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip_addr_str[64] = {0};
	uint8_t nSim = 0;
	
	ql_m2m_client_init(0);
	ql_m2m_client_setopt(LWM2M_CLIENT_OPT_EVENT_CB_FUNC, ql_m2m_client_event_cb);

	ql_rtos_task_sleep_s(10);
	QL_LWM2M_LOG("==========M2M demo start ==========");
	QL_LWM2M_LOG("wait for network register done");

	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_LWM2M_LOG("====network registered!!!!====");
	}else{
		QL_LWM2M_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	QL_LWM2M_LOG("===start data call====");

	ret=ql_start_data_call(nSim, profile_idx, (1 != use_ipv6) ? QL_PDP_TYPE_IP:QL_PDP_TYPE_IPV6, "uninet", NULL, NULL, 0);
	QL_LWM2M_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_LWM2M_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0){
		QL_LWM2M_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    QL_LWM2M_LOG("info->profile_idx: %d", info.profile_idx);
	QL_LWM2M_LOG("info->ip_version: %d", info.ip_version);
	if(1 == info.v6.state) {
		QL_LWM2M_LOG("info->v6.state: %d", info.v6.state); 
		snprintf(ip_addr_str,sizeof(ip_addr_str),"%s",ip6addr_ntoa(&info.v6.addr.ip));
		QL_LWM2M_LOG("info.v6.addr.ip: %s\r\n", ip_addr_str);
		
		memset(ip_addr_str, 0, sizeof(ip_addr_str));
		snprintf(ip_addr_str,sizeof(ip_addr_str),"%s",ip6addr_ntoa(&info.v6.addr.pri_dns));
		QL_LWM2M_LOG("info.v6.addr.pri_dns: %s\r\n", ip_addr_str);

		memset(ip_addr_str, 0, sizeof(ip_addr_str));
		snprintf(ip_addr_str,sizeof(ip_addr_str),"%s",ip6addr_ntoa(&info.v6.addr.sec_dns));
		QL_LWM2M_LOG("info.v6.addr.sec_dns: %s\r\n", ip_addr_str);
	}
	if(1 == info.v4.state){
		QL_LWM2M_LOG("info->v4.state: %d", info.v4.state); 
		inet_ntop(AF_INET, &info.v4.addr.ip, ip_addr_str, sizeof(ip_addr_str));
		QL_LWM2M_LOG("info.v4.addr.ip: %s\r\n", ip_addr_str);

		inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip_addr_str, sizeof(ip_addr_str));
		QL_LWM2M_LOG("info.v4.addr.pri_dns: %s\r\n", ip_addr_str);

		inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip_addr_str, sizeof(ip_addr_str));
		QL_LWM2M_LOG("info.v4.addr.sec_dns: %s\r\n", ip_addr_str);
	}
	/*task run loop*/
	ql_m2m_app_run();
	
exit:
    lwm2m_task = NULL;
	/*destroy*/
    ql_rtos_task_delete(lwm2m_task);	

    return;	
}

int ql_m2m_app_init(void)
{
	QlOSStatus ret = QL_OSI_SUCCESS;
	ret = ql_rtos_queue_create(&lwm2m_queue, sizeof(ql_event_t), M2M_MAX_MSG_CNT);
	if (ret) 
	{
		QL_LWM2M_LOG("create queue failed!!!!");
		goto exit;
	}

	ret = ql_rtos_timer_create(&lwm2m_timer, lwm2m_task, m2m_timer_callback, NULL);
	if(ret != QL_OSI_SUCCESS)
	{
		QL_LWM2M_LOG("lwm2m_timer created failed");
		goto exit;
	}
	
    ret = ql_rtos_task_create(&lwm2m_task, 8*1024, APP_PRIORITY_ABOVE_NORMAL, "Qlwm2mApp", ql_m2m_app_thread, NULL, 10);
	if(ret != QL_OSI_SUCCESS)
    {
		QL_LWM2M_LOG("m2m_app init failed");
		goto exit;
	}

	return ret;

exit:
	ql_rtos_queue_delete(lwm2m_queue);
	lwm2m_queue = NULL;
	ql_rtos_timer_delete(lwm2m_timer);
	lwm2m_timer = NULL;

	return ret;
}


