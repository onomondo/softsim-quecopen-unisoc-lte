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
#include "ql_api_nw.h"

#include "ql_log.h"

#include "ql_cnktd.h"
#include "ql_cnktd_nw.h"
#include "ql_cnktd_socket.h"

#define CNKTD_SOCKET_LOG(msg, ...)			QL_LOG(QL_LOG_LEVEL_INFO, "ql_cnktd_sock", msg, ##__VA_ARGS__)

#define QL_SOCKET_DEMO_CHANGE_DEMO 0

#if QL_SOCKET_DEMO_CHANGE_DEMO
#define QL_SOCKET_DEMO_URL "www.baidu.com"
#else
#define QL_SOCKET_DEMO_URL "220.180.239.212"
#endif

static ql_task_t cnktd_socket_task = NULL;
static char send_buf[128]={0};
static int  send_len = 0;
static char recv_buf[128]={0};
static int  recv_len = 0;

extern void cnktd_set_data_call_param(qapi_DSS_Hndl_t dss_handle);

static void cnktd_socket_demo_task(void * arg)
{
	int ret = 0;
	int i = 0;
	int profile_idx = 1;
	fd_set read_fds;
	fd_set write_fds;
	fd_set exp_fds;
	int socket_fd = -1;
	int flags = 0;
	int fd_changed = 0;
	int connected = 0;
	int closing = false;
	int send_flag = 0;
	int recv_flag = 0;
	struct sockaddr_in local4, server_ipv4;	
	struct addrinfo *pres = NULL;	
	struct addrinfo *temp = NULL;
	uint8_t nSim = 0;
	ql_nw_data_count_info_s data_count_info = {0};
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

	ql_rtos_task_sleep_s(10);
	CNKTD_SOCKET_LOG("========== socket demo start ==========");
	CNKTD_SOCKET_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		CNKTD_SOCKET_LOG("====network registered!!!!====");
	}else{
		CNKTD_SOCKET_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);
    qapi_DSS_Get_Data_Srvc_Hndl(NULL, NULL, &dss_handle);
    if (dss_handle == NULL)
    {
        CNKTD_SOCKET_LOG("dss_handle is NULL");
        goto exit;
    }
    cnktd_set_data_call_param(dss_handle);
	
	CNKTD_SOCKET_LOG("===start data call====");
    ret = qapi_DSS_Start_Data_Call(dss_handle);
    CNKTD_SOCKET_LOG("qapi_DSS_Start_Data_Call, err = %d", ret);

    ret = qapi_QT_NW_PDP_Cfg_Get(&pdp_context_number, &profile);
	if (ret < 0)
    {
        qapi_DSS_Stop_Data_Call(dss_handle);
        CNKTD_SOCKET_LOG("qapi_QT_NW_PDP_Cfg_Get failed");
        goto exit;
    }
    CNKTD_SOCKET_LOG("qapi_QT_NW_PDP_Cfg_Get err = %d, apn=%s, pdp_type=%d", ret, profile.apn, profile.pdp_type);

    if(profile.pdp_type == QT_NW_DS_PROFILE_PDP_IPV4V6){
        ip_addr_cnt = 2;
    }

    if(ip_addr_cnt)
    {
        qapi_DSS_Addr_Info_t *qapi_DSS_Addr_Info_ptr = calloc(1, sizeof(qapi_DSS_Addr_Info_t) * ip_addr_cnt);
        qapi_DSS_Addr_Info_t *info_ptr = qapi_DSS_Addr_Info_ptr;
        if(info_ptr == NULL)
        {
            CNKTD_SOCKET_LOG("calloc info_ptr failed");
            goto exit;
        }

        ret = qapi_DSS_Get_IP_Addr(dss_handle, info_ptr, ip_addr_cnt);
        CNKTD_SOCKET_LOG("qapi_DSS_Get_IP_Addr err = %d", ret);
        if(ret == QAPI_OK)
        {
            if(profile.pdp_type == QAPI_DSS_IP_VERSION_4 || profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
            {
                CNKTD_SOCKET_LOG("V4, ip=%s, subnet mask=%s, gateway=%s, Primary DNS=%s, Secondary DNS=%s",
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
                CNKTD_SOCKET_LOG("V6, ip=%s, prefix length=%u, gateway=%s, gateway length=%u, Primary DNS=%s, Secondary DNS=%s",
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
	        CNKTD_SOCKET_LOG("qapi_DSS_Get_IP_Addr failed");
	        goto exit;
	    }
        free(qapi_DSS_Addr_Info_ptr);
    }

#if QL_SOCKET_DEMO_CHANGE_DEMO
	ret = qapi_Net_DNSc_Add_Server_on_iface("114.114.114.114", 0, profile_idx);
	if (ret < 0) 
	{
		CNKTD_SOCKET_LOG("DNS set_dns_pri failed!");
	}
	ret = qapi_Net_DNSc_Add_Server_on_iface("8.8.8.8", 1, profile_idx);
	if (ret < 0) 
	{
		CNKTD_SOCKET_LOG("DNS set_dns_sec failed!");
	}
#endif

	memset(&local4, 0x00, sizeof(struct sockaddr_in));
	local4.sin_family = AF_INET;
	local4.sin_port = 0;
	inet_aton(ip_v4, &local4.sin_addr);	
loop:
	ret = qapi_Net_DNSc_Reshost_on_iface(QL_SOCKET_DEMO_URL, &pres, (uint32)profile_idx);
	if (ret < 0 || pres == NULL) 
	{
		CNKTD_SOCKET_LOG("DNS getaddrinfo failed! ret=%d; pres=%p!\n",ret,pres);
		goto exit;
	}

	qapi_fd_zero(&read_fds);
	qapi_fd_zero(&write_fds);
	qapi_fd_zero(&exp_fds);
	flags = 0;
	connected = 0;
	closing = false;
	i = 0;
	for(temp = pres; temp != NULL; temp = temp->ai_next){
		struct sockaddr_in * sin_res = (struct sockaddr_in *)temp->ai_addr;
		if(temp->ai_family == AF_INET){
			socket_fd = qapi_socket(temp->ai_family, SOCK_STREAM, 0);
			if(socket_fd < 0){
				continue;
			}

			ret = bind(socket_fd,(struct sockaddr *)&local4,sizeof(struct sockaddr));
			if(ret < 0){
				qapi_socketclose(socket_fd);
				socket_fd = -1;
				continue;
			}
			
			flags |= O_NONBLOCK;
			fcntl(socket_fd, F_SETFL,flags);

			memset(&server_ipv4, 0x00, sizeof(struct sockaddr_in));
			server_ipv4.sin_family = temp->ai_family;
			server_ipv4.sin_port = htons(8252);
			server_ipv4.sin_addr = sin_res->sin_addr;

			ret = qapi_connect(socket_fd, (struct sockaddr *)&server_ipv4, sizeof(server_ipv4));	
			
			if(ret == 0){
				connected = 1;
				break;
			}else{
				if(lwip_get_error(socket_fd) != EINPROGRESS){
					qapi_socketclose(socket_fd);
					socket_fd = -1;
					continue;
				}else{
					break;
				}
			}
		}
	}
	if(socket_fd < 0){
		goto exit;
	}
	if(connected == 1){
		qapi_fd_set(socket_fd, &read_fds);	
		CNKTD_SOCKET_LOG("=====connect to \"220.180.239.212:8252\" success=====");
		memset(send_buf, 0x00, 128);
		send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
		
		if(qapi_send(socket_fd, send_buf, send_len, send_flag) != send_len){
			qapi_fd_set(socket_fd, &write_fds);	
		}else{
			i++;
		}
	}else{
		qapi_fd_set(socket_fd, &write_fds);	
	}	
	qapi_fd_set(socket_fd, &exp_fds);

	while(1){
		fd_changed = qapi_select(socket_fd+1, &read_fds, &write_fds, &exp_fds, 5000);
		if(fd_changed > 0){
			if(qapi_fd_isset(socket_fd, &write_fds)){
				qapi_fd_clr(socket_fd, &write_fds);
				if(connected== 0){
					int value = 0;
					int len = 0;
						
					len = sizeof(value);
					getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &value, &len);
						
					CNKTD_SOCKET_LOG("errno: %d", value);
					if(value == 0 || value == EISCONN ){
						CNKTD_SOCKET_LOG("=====connect to \"220.180.239.212:8252\" success=====");
						connected = 1;						
						qapi_fd_set(socket_fd, &read_fds);	

						memset(send_buf, 0x00, 128);
						send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
						qapi_send(socket_fd, send_buf, send_len, send_flag);
						i++;
					}else{
						CNKTD_SOCKET_LOG("=====connect to \"220.180.239.212:8252\" failed=====");
						qapi_socketclose(socket_fd);
						socket_fd = -1;
						break;
					}
				}else{
					memset(send_buf, 0x00, 128);
					send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
					qapi_send(socket_fd, send_buf, send_len, send_flag);
					i++;
				}						
			}else if(qapi_fd_isset(socket_fd, &read_fds)){
				qapi_fd_clr(socket_fd, &read_fds);
			    memset(recv_buf,0x00, 128);
				recv_len = qapi_recv(socket_fd, recv_buf, 128, recv_flag);
				if(recv_len > 0){
					CNKTD_SOCKET_LOG(">>>>Recv: %s", recv_buf);
					memset(send_buf, 0x00, 128);
					send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
					qapi_send(socket_fd, send_buf, send_len, send_flag);
					i++;
					qapi_fd_set(socket_fd, &read_fds);
					if(ql_nw_get_data_count(nSim, &data_count_info) == 0)
					{
						CNKTD_SOCKET_LOG("data count: uplink %llu, downlink %llu", data_count_info.uplink_data_count, data_count_info.downlink_data_count);
					}
				}else if(recv_len == 0){
					if(closing == false){
						CNKTD_SOCKET_LOG("===passive close!!!!");
						shutdown(socket_fd, SHUT_WR);
						closing = true;
						qapi_fd_set(socket_fd, &read_fds);	
					}else{
						qapi_socketclose(socket_fd);
						socket_fd = -1;
						break;
					}
				}else{
					if(lwip_get_error(socket_fd) == EAGAIN){
						qapi_fd_set(socket_fd, &read_fds);	
					}else{
						qapi_socketclose(socket_fd);
						socket_fd = -1;
						break;
					}
				}
			}else if(qapi_fd_isset(socket_fd, &exp_fds)){
				qapi_fd_clr(socket_fd, &exp_fds);
				qapi_socketclose(socket_fd);
				socket_fd = -1;
				break;
			}
			
		}
	}
    if(pres!=NULL)
    {
        freeaddrinfo(pres);
        pres = NULL;
    }
	goto loop;
exit:  
   if(dss_handle != NULL)
   {
	   qapi_DSS_Rel_Data_Srvc_Hndl(dss_handle);
   }
   ql_rtos_task_delete(cnktd_socket_task);	
	
   return;	
}

int cnktd_socket_demo_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
    err = ql_rtos_task_create(&cnktd_socket_task, 16*1024, APP_PRIORITY_ABOVE_NORMAL, "cnktd_socket_demo", cnktd_socket_demo_task, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		CNKTD_SOCKET_LOG("cnktd_socket_demo_task failed");
	}

	return err;
}


