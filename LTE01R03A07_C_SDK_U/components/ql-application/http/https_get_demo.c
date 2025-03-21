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

/*
*  brief: get method
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_http_client.h"
#include "ql_fs.h"

														
#define ROOT_CRT_PATH   "UFS:root.crt"							
#define CLIENT_KEY_PATH "UFS:client.key"							
#define CLIENT_CRT_PATH "UFS:client.crt"
							
#define QL_HTTP_LOG_LEVEL           	QL_LOG_LEVEL_INFO
#define QL_HTTP_LOG(msg, ...)			QL_LOG(QL_HTTP_LOG_LEVEL, "ql_HTTP", msg, ##__VA_ARGS__)
#define QL_HTTP_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_HTTP", msg, ##__VA_ARGS__)
													
struct cert_array_t {
	char *path;
	char *data;
};

#ifndef USE_CUST_KEY
#define USE_CUST_KEY 0    // 自定义http证书、秘钥等, 默认关闭0
#endif

#if USE_CUST_KEY
#define root_crt "-----BEGIN CERTIFICATE-----\n\
MIIDYTCCAkkCFFr50PfFXMCG+Lg5XoZ1mPeMJ3P9MA0GCSqGSIb3DQEBCwUAMG0x\n\
CzAJBgNVBAYTAmNuMQswCQYDVQQIDAJoZjELMAkGA1UEBwwCaGYxDTALBgNVBAoM\n\
BHRlc3QxDTALBgNVBAsMBHRlc3QxDTALBgNVBAMMBHJvb3QxFzAVBgkqhkiG9w0B\n\
CQEWCHJvb3QuY29tMB4XDTIxMDQyMDAxMjU0M1oXDTMxMDQxODAxMjU0M1owbTEL\n\
MAkGA1UEBhMCY24xCzAJBgNVBAgMAmhmMQswCQYDVQQHDAJoZjENMAsGA1UECgwE\n\
dGVzdDENMAsGA1UECwwEdGVzdDENMAsGA1UEAwwEcm9vdDEXMBUGCSqGSIb3DQEJ\n\
ARYIcm9vdC5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDG3HTp\n\
zAarO7JJIfiDQTi8nsoAIYjAIDDY6PTdylLs/F5HJtiqWr1ZpyfC11hwRofFIE0u\n\
i1V8BUEGaIYb3UgN8/y8w483Fi7+kZVm3NRK1yo2oMqvTQyMZzhqOv7hdsv17qdD\n\
AN21iS7jna29Qzjcrt4jwZ1eRZtUXtZiVMZ/3UT3PKaJTtllVxVaDXhPMPuNh4w3\n\
jA2aNlLqPc21zSIvHhFHQNHsNWKlxBvchomWKRpdDpW7D32RnjlCzYW4/RiHeCQa\n\
7+fdOpaUDuR8RIG8pDK5x3G4t8hcisGl88T0pqKmHNaG0WO6VqKHmOaSWq2+Gzmo\n\
WZTLO0NPxcJGTGEXAgMBAAEwDQYJKoZIhvcNAQELBQADggEBABcdf529CGJ+QwgS\n\
UBXtVubvZIsUKznFpFnVV/jvqjRTkQgHbohtkba6mCgGYYBUnc6uEVNXx1fVlXX/\n\
3mT+iC5zZh2dkNM8covcOCmI6yih9uwgdPURu56/clsY/8IqdH8DTcrDylqH1yVS\n\
zsxRKTNvfWltO2avorufUg1swKCF5SsmK+SYpVpZYosCCfGe2PAdhllVpxtbrv2z\n\
VDy2DqbWhYU0GOB6fd9trxeshHVAsyk5VczGRnPoy1yZ/h8hwnhUk5WuvEY0n3ce\n\
ZxASiQTSVs4sC7H5navGt0dZH7W9VPvszyYdC7c1Dh+pbdn59d/YHYF+SmLwLX9/\n\
c+kx7+E=\n\
-----END CERTIFICATE-----"
							
#define client_key  "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAx+raeEvE+/teiTFcAxYYJAMaePmtuUGfe5lcysRQTALSTmT1\n\
VccV4HXai73Q3KS12lxeoc/ib/z+8cyoEAPATyYAUV8tDh76hOcgUX5g0hxvzA7H\n\
65xrgytGCL+CoU2eM6mDvLNo4tCNKxFRtYvgoq3zVaS57cK7WykagnsIawYl2ivz\n\
BrbDchOv5fi+g/uJfRYFVTl54zbSSFUFar91q6QLEWENjKA8r4aTPsiIFARYIQly\n\
OI7GHpH8QrNSOspdBVByUqpVXDJFBiX748uijp72cVNcye4pDVf27Iip6S0FCLc0\n\
tIWwKnl5/d3IZIUG9yCXFEB933u7useVjoniYQIDAQABAoIBAQCla3silu+D4Vc0\n\
BhewphOLmNXKzC4nYFlqmFfjn9UhUCRcocUvGPDNLjeL8aBlMhUXfd7/3Vfselcq\n\
FMHrnm7+Dt+sdSh+yrtYRTbEt+G9ebI4EvLxzhhYDPKCQpUAi40gCl/rQDjFlPN6\n\
c3Y81mUltvHmJ8uBJOSKZdR9qvgoNRLBJqk+ZAQzUK6iNbdEOZRkFyO7tJ0aI+QH\n\
L3bAkqaimxku1QSCzE5dQ2JC9scFBhUNyBB+N7Gb03oYobozBPJu5LKCY0BpjRAM\n\
crIEa3k9RPA0BLOQP51byBgKvwy8YK/P3ddVkb+W55fIm/ukjKuxUF1QiD44Kiql\n\
U6hDlWcBAoGBAPLb/oO0kXPk71HQrTpSVQJZ21Xh+hAPRLntEJSsFrDIjYNheNX5\n\
bcKJqR3JyA+NTJb5OZedAPYv5xtdWBS9HrMspMMMxZ1+0NbQQL/5rjEVoX8yqA5r\n\
H1QWfq1RABfpOYjKVDal7H76VJF/Zi+Gbg4dPzUt5HFU1lFApObTEjR5AoGBANK8\n\
Cq2rGUUX2qg0gd/OZW85Ov0CUwd9awNc2x6VaWG+gMSiEv2fiv8Ka2MvlQjjtmds\n\
S8/G2jIbxkAHddopXGdXIsF2Pr2Bzw8bgKtPliMVte3dYOOoHdTt6P/MaFbNtwoJ\n\
CKPRHAdSI0pYkdMjgMYQkOPBb3xY5+jO7eCk7pMpAoGATIHtrkQmRmJq2lVpHSRu\n\
z2zLYkTsaD7HIeODA7Mq+spTen4seQw4/b4sXswAkJWs+SrOb+rwMJ5qqf5AD1h8\n\
4WlBBolxf+pBlErK+y+HJsNvuwiUVOZ+Whk0Exmfc+jvTR6gZQ7kcCQgnC2MXZB1\n\
/1BLZ09r1iAp3DMHQAhihdECgYAo1xKGt7sM/eN9Z7enxRs5Y/D/qZynvfGoVC2P\n\
Dsu+iqnv8a0Wg/3vEXjgXw9WCrOF9aH3oYUV4vYX4JpBbnSe/m8Y6S6dkXOsbNsR\n\
2DmzkPiSIa4uUJ1/b6AROu8Zq4KGqluHS/ZPoOZ/+QxBzAaf3AQIb6uIgL5pCvlk\n\
6PHniQKBgBPGQVFg7iGbbjnWGXL6nbvPWN4bfmy4ESIK3TN0p3UxZBnfAsArOl7D\n\
HXMRUx2qW+WkUHpfKgqC17Q1srm6w/jJuZAYBkCgLhp3iy/ENtqdE2yYPSGSJnlh\n\
sR94u9+nkJXw5d6u5xew/y5YelM+gH9X3fv87ulTV2fpoHRk8KH2\n\
-----END RSA PRIVATE KEY-----"
							
#define client_crt  "-----BEGIN CERTIFICATE-----\n\
MIIDbDCCAlQCFHu5iQ8zrC5zzju2bI/yWWnCYhW/MA0GCSqGSIb3DQEBCwUAMG0x\n\
CzAJBgNVBAYTAmNuMQswCQYDVQQIDAJoZjELMAkGA1UEBwwCaGYxDTALBgNVBAoM\n\
BHRlc3QxDTALBgNVBAsMBHRlc3QxDTALBgNVBAMMBHJvb3QxFzAVBgkqhkiG9w0B\n\
CQEWCHJvb3QuY29tMB4XDTIxMDQyMDA3MDU1OFoXDTMxMDQxODA3MDU1OFoweDEL\n\
MAkGA1UEBhMCY24xCzAJBgNVBAgMAmhmMQswCQYDVQQHDAJoZjENMAsGA1UECgwE\n\
dGVzdDENMAsGA1UECwwEdGVzdDEYMBYGA1UEAwwPMjIwLjE4MC4yMzkuMjEyMRcw\n\
FQYJKoZIhvcNAQkBFghyb290LmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCC\n\
AQoCggEBAMfq2nhLxPv7XokxXAMWGCQDGnj5rblBn3uZXMrEUEwC0k5k9VXHFeB1\n\
2ou90NyktdpcXqHP4m/8/vHMqBADwE8mAFFfLQ4e+oTnIFF+YNIcb8wOx+uca4Mr\n\
Rgi/gqFNnjOpg7yzaOLQjSsRUbWL4KKt81Wkue3Cu1spGoJ7CGsGJdor8wa2w3IT\n\
r+X4voP7iX0WBVU5eeM20khVBWq/daukCxFhDYygPK+Gkz7IiBQEWCEJcjiOxh6R\n\
/EKzUjrKXQVQclKqVVwyRQYl++PLoo6e9nFTXMnuKQ1X9uyIqektBQi3NLSFsCp5\n\
ef3dyGSFBvcglxRAfd97u7rHlY6J4mECAwEAATANBgkqhkiG9w0BAQsFAAOCAQEA\n\
H5qIexLGos836/GIoOHyF1vG6n7VZhu2G8GF41XQs9KfvuJY3FQ4a7drHEujbdpI\n\
9J9mR8hUaafTjxQVytfekCYkTytsUzJO+qGnxGeS+xWJO/jc1NnPre8ldG1X13Rp\n\
IbqwRbo0z6J/XrzJzbTVd/joxNBWuIXqCSb8SmKYLUeUOZVMG7autr4VryJgAFma\n\
hOVJPl3sAEnKJ1dvJC+VXyeWwjfDdXK4RD0UgoDfux7UezAThMC/YKrLz+/kVkps\n\
dG52zsbbIKZ/omgFgThUFuR36mc0DAL5+Exk5Fb0biB+mOzYTejzmZ/kWQS1EGxI\n\
0VZFtnOt9nIZ0+uH2sbHcQ==\n\
-----END CERTIFICATE-----"

#define cert_array_len 3
struct cert_array_t cert_array[cert_array_len] = {
							{ ROOT_CRT_PATH, root_crt },
							{ CLIENT_KEY_PATH, client_key },
							{ CLIENT_CRT_PATH, client_crt }
											     };
#endif


enum CUST_HTTPS_CHECK {CUST_NONE=0, CUST_SINGLE, CUST_DOUBLE};

ql_task_t http_get_task = NULL;
static http_client_t  http_cli = 0;
static ql_sem_t  http_semp = NULL;

static void http_event_cb(http_client_t *client, int evt, int evt_code, void *arg)
{
	QL_HTTP_LOG("*client:%d, http_cli:%d", *client, http_cli);
	if(*client != http_cli)
		return;
	QL_HTTP_LOG("evt:%d, evt_code:%d", evt, evt_code);
	switch(evt){
	case HTTP_EVENT_SESSION_ESTABLISH:{
			if(evt_code != HTTP_SUCCESS){
				QL_HTTP_LOG("HTTP session create failed!!!!!");
				ql_rtos_semaphore_release(http_semp);
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
				QL_HTTP_LOG("response code:%d", resp_code);
				ql_httpc_getinfo(client, HTTP_INFO_CHUNK_ENCODE, &chunk_encode);
				if(chunk_encode == 0){
					ql_httpc_getinfo(client, HTTP_INFO_CONTENT_LEN, &content_length);
					QL_HTTP_LOG("content_length:%d",content_length);
				}else{
					QL_HTTP_LOG("http chunk encode!!!");
				}

				if(resp_code >= 300 && resp_code < 400){
					ql_httpc_getinfo(client, HTTP_INFO_LOCATION, &location);
					QL_HTTP_LOG("redirect location:%s", location);
					free(location);
				}
			}
		}
		break;
	case HTTP_EVENT_SESSION_DISCONNECT:{
			if(evt_code == HTTP_SUCCESS){
				QL_HTTP_LOG("===http transfer end!!!!");
			}else{
				QL_HTTP_LOG("===http transfer occur exception!!!!!");
			}			
			ql_rtos_semaphore_release(http_semp);
		}
		break;
	}
}

static int http_write_response_data(http_client_t *client, void *arg, char *data, int size, unsigned char end)
{
    unsigned char *recbuff=NULL;
    QL_HTTP_LOG("recv size:%d", size);
    recbuff=malloc(size+1);
    if(NULL!=recbuff)
	{	
        memset(recbuff, 0, size+1);
        memcpy(recbuff, data, size);
    	QL_HTTP_LOG("recv data: %s", recbuff);
        free(recbuff);
        recbuff=NULL;
    }
	return size;
}

static int sim_net_register(int nSim, int profile_idx)
{

	int ret = 0, i = 0;
	ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};

	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
		i++;
		ql_rtos_task_sleep_ms(1000);
	}
	if(ret == 0) {
		i = 0;
		QL_HTTP_LOG("====network registered!!!!====");
	} else {
		QL_HTTP_LOG("====network register failure!!!!!====");
		return ret;
	}
	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
	if(ret != 0) {
		QL_HTTP_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0) {
		QL_HTTP_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		return ret;
	}
		
	QL_HTTP_LOG("info->profile_idx: %d", info.profile_idx);
	QL_HTTP_LOG("info->ip_version: %d", info.ip_version);
					
	QL_HTTP_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_HTTP_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);
		
	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_HTTP_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);
		
	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_HTTP_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);
	return ret;
}

#if USE_CUST_KEY
/*
* 客户端证书秘钥初始化函数，将自定义证书秘钥写入自定义文件中
*/
static int client_certfication_init()
{
	int ret = 0;
	QFILE fp = -1;
	int i = 0;

	for(i = 0; i < cert_array_len; i++) {
		fp = ql_fopen(cert_array[i].path, "w+");
		if(fp < 0) {
			QL_HTTP_LOG("open file:%s failed!!!!", cert_array[i].path);
			return -1;
		}
		ret = ql_fwrite(cert_array[i].data, strlen(cert_array[i].data), 1, fp);
		if( ret != strlen(cert_array[i].data) ) {
			QL_HTTP_LOG("http write %d bytes data failure", ret);
			ql_fclose(fp);
			return -1;
		}
		ql_fclose(fp);
	}

	return 0;
}
#endif

static void https_get_app_thread(void * arg)
{
    int ret = 0;
    int profile_idx = 1;
    
	int run_num = 0;
	uint8_t nSim = 0;
	char *url[3] = {"https://220.180.239.212:9634/get_request",  // https 无校验
					"https://220.180.239.212:9634/get_request",  // https 单向认证
					"https://220.180.239.212:9633/get_request"   // https 双向认证
				   };
	int case_id = 0;
	
	ql_rtos_task_sleep_s(5);
	QL_HTTP_LOG("========== http new demo start ==========");
	QL_HTTP_LOG("wait for network register done");
	ret = sim_net_register(nSim, profile_idx);
	if( ret != 0 ) {
		QL_HTTP_LOG("sim register net failure , ret= %d", ret);
		goto exit;
	}
#if USE_CUST_KEY
	ret = client_certfication_init();
	if( ret != 0 ) goto exit;
#endif
	do {
		QL_HTTP_LOG("==============http_client_get_test[%d]================\n",run_num+1);
		
		ql_rtos_semaphore_create(&http_semp, 0);
	
		if(ql_httpc_new(&http_cli, http_event_cb, NULL) != HTTP_SUCCESS){
			QL_HTTP_LOG("http client create failed!!!!");
			break;
		}
		
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SIM_ID, nSim);
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_PDPCID, profile_idx);
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_WRITE_FUNC, http_write_response_data);
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_METHOD, HTTP_METHOD_GET);
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSLCTXID, 1);
		
		case_id = run_num%3;
		switch( case_id ) {
			case CUST_NONE:				
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_VERIFY_LEVEL, HTTPS_VERIFY_NONE);
				break;
			case CUST_SINGLE:				
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_VERIFY_LEVEL, HTTPS_VERIFY_SERVER);
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_CACERT_PATH, ROOT_CRT_PATH);
				break;
			case CUST_DOUBLE:
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_VERIFY_LEVEL, HTTPS_VERIFY_SERVER_CLIENT);
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_CACERT_PATH, ROOT_CRT_PATH);
				ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_SSL_OWNCERT_PATH, CLIENT_CRT_PATH, CLIENT_KEY_PATH, NULL);
				break;
			default:
				break;
		}
		ql_httpc_setopt(&http_cli, HTTP_CLIENT_OPT_URL, url[case_id]);
		
		if(ql_httpc_perform(&http_cli) == HTTP_SUCCESS) {
			QL_HTTP_LOG("wait http perform end!!!!!!");
			ql_rtos_semaphore_wait(http_semp, QL_WAIT_FOREVER);
		} else {
			QL_HTTP_LOG("http perform failed!!!!!!!!!!");
		}

		ql_httpc_release(&http_cli);
		http_cli = 0;
		QL_HTTP_LOG("==============http_client_test_end[%d]================\n",run_num+1);
		run_num++;

		if(http_semp != NULL)
		{
			ql_rtos_semaphore_delete(http_semp);
			http_semp = NULL;
		}
		
		ql_rtos_task_sleep_s(5);
	 } while(1);
exit:
    if(http_semp != NULL)
    {
		ql_rtos_semaphore_delete(http_semp);
		http_semp = NULL;
    }
	
  	ql_rtos_task_delete(http_get_task);
    return;		
}

void ql_https_get_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    
    err = ql_rtos_task_create(&http_get_task, 4096, APP_PRIORITY_NORMAL, "QhttpGet", https_get_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
	{
		QL_HTTP_LOG("created task failed");
	}
}

