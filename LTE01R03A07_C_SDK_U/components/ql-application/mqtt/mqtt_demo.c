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
#include "ql_api_datacall.h"
#include "ql_mqttclient.h"

#include "ql_ssl.h"

#define QL_MQTT_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_MQTT_LOG(msg, ...)			QL_LOG(QL_MQTT_LOG_LEVEL, "ql_MQTT", msg, ##__VA_ARGS__)
#define QL_MQTT_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_MQTT", msg, ##__VA_ARGS__)
static ql_task_t mqtt_task = NULL;

#define MQTT_CLIENT_IDENTITY        "quectel_01"
#define MQTT_CLIENT_USER            ""
#define MQTT_CLIENT_PASS            ""


#define MQTT_CLIENT_ONENET_PRODUCTID             "417661"
#define MQTT_CLIENT_ONENET_ACCESSKEY             "aggblpDBWoKg2CGdbxp/CVKt9xXWcV162hJ1+Bhzvw4="
#define MQTT_CLIENT_ONENET_DEVICENAME            "test_led"
#define MQTT_CLIENT_ONENET_VERSION               "2018-10-31"

#define USE_CRT_BUFFER 0

#define MQTT_CLIENT_QUECTEL_URL                  "mqtt://220.180.239.212:8306"
#define MQTT_CLIENT_ONENET_URL                   "mqtt://mqtts.heclouds.com:1883" //onenet 的ip地址

#if USE_CRT_BUFFER
#define MQTT_CLIENT_QUECTEL_SSL_URL              "mqtts://112.31.84.164:8308"
#else
#define MQTT_CLIENT_QUECTEL_SSL_URL              "mqtts://220.180.239.212:8307"
#endif
#define MQTT_CLIENT_ONENET_SSL_URL               "mqtts://mqttstls.heclouds.com:8883"//onenet 的ip地址

// publist 的内容
#define MQTT_PUB_MSG0 "{\"id\": 000000,\"dp\": {\"temperatrue\": [{\"v\": 0.001,}],\"power\": [{\"v\": 0.001,}]}}"
#define MQTT_PUB_MSG1 "{\"id\": 111111,\"dp\": {\"temperatrue\": [{\"v\": 1.000,}],\"power\": [{\"v\": 1.001,}]}}"
#define MQTT_PUB_MSG2 "{\"id\": 222222,\"dp\": {\"temperatrue\": [{\"v\": 2.000,}],\"power\": [{\"v\": 2.002,}]}}"

static ql_sem_t  mqtt_semp;
static int  mqtt_connected = 0;

#if USE_CRT_BUFFER
char *root_ca_crt_buffer= "-----BEGIN CERTIFICATE-----\r\n\
MIIEhDCCAuwCCQDuE1BpeAeMwzANBgkqhkiG9w0BAQsFADCBgjELMAkGA1UEBhMC\r\n\
Q04xCzAJBgNVBAgMAkFIMQswCQYDVQQHDAJIRjEQMA4GA1UECgwHUVVFQ1RFTDEL\r\n\
MAkGA1UECwwCU1QxFjAUBgNVBAMMDTExMi4zMS44NC4xNjQxIjAgBgkqhkiG9w0B\r\n\
CQEWE2VkZGllLnpoYW5nQHF1ZWN0ZWwwIBcNMjIwMTI1MDcyMzI3WhgPMjEyMjAx\r\n\
MDEwNzIzMjdaMIGCMQswCQYDVQQGEwJDTjELMAkGA1UECAwCQUgxCzAJBgNVBAcM\r\n\
AkhGMRAwDgYDVQQKDAdRVUVDVEVMMQswCQYDVQQLDAJTVDEWMBQGA1UEAwwNMTEy\r\n\
LjMxLjg0LjE2NDEiMCAGCSqGSIb3DQEJARYTZWRkaWUuemhhbmdAcXVlY3RlbDCC\r\n\
AaIwDQYJKoZIhvcNAQEBBQADggGPADCCAYoCggGBAMt3cjY0eLEDqv8Y7FomA+7N\r\n\
G5ztAbR7+P/WxjPlodqRDZ5HQORkfAr44gAZcWsKoo4DHTInwr9JBbBnETBMnL8+\r\n\
13h1PRp5CfwXKFvjppWYvBZfeTwhWQYbSMKINoS+d1Zl11jg/+ZbSd7Fi0bYq8ip\r\n\
Hbt30H+NANQZP1XQdsCf5/kvn+vXiP4EgJc56JQ9L6ALIF2Q6F3G/PTaYItg463N\r\n\
lv/S+eRi1VMDSs8Qc+DTlVwlgZZJdSlC8Yjr5pVqoyXm8ENKfSTrdhrLiKSWJTz9\r\n\
JUr04E7SJ+CoBAnLYNPHR2y0CFS/15aCa1JbK27ZJ/0cvBvzpWdkcgrDtKIcxNYM\r\n\
9QFPpehb1N4pgqi0NPhCkc/BasfmXUaTwM4ghhi4tQRptKMdTN/kdyC+V5a8Hyhb\r\n\
Nvw5qeJlLJKpgZ9X3HQzuKstKMkxLNuDIzK9TvO7zLowr+0BetUdllq+fDjXQM0M\r\n\
+9P3Xv2VmDwGRkmZ0IjYpDjm+qqGTFVLzzVwEqVD6wIDAQABMA0GCSqGSIb3DQEB\r\n\
CwUAA4IBgQAuNVwkBhd5nyWMmV/ESNxy59Sz+5FcesGclKjs4YocgcKbLD2bS+LN\r\n\
lKk6zenES7Cq6+l3NMAxxh/QhgHUCThAfREzfPXbmiicrUfaudN4YFivpoFwKIAs\r\n\
NczsL9S3FPbzAB4nLDATacc2BK0//aKMOU2t3KLNNomKbzlR+EW3wd0F1GoZ9SY6\r\n\
sCQeLa8Wp1KarOmbvgoFL/DAiTSqjjsU/Lq24dOCCctmG+qXRZxQa4npHD4xJwQJ\r\n\
qzA0JLu4n+DgoJftm1KpvB0wuzTn6M9+wnk5rv/fGc2t4Zra8B4prEReZZVfy65d\r\n\
cb8pBdb20Yrmznj+6DR50X/o/8Qzoyj9XpxtjwF23ql0XPYCI7kB03Ms9euP0btc\r\n\
HFacHapm0qBKx+vWy0V2Qf482OWSbewqaRbud44sErNoKqpqm02yN8PpsCywpFUj\r\n\
UC5G5DzxzYspMzQv/yidti0scMSKFObseZmNGlRYymCWhXnxmoCFjLpw5RnJSB2+\r\n\
cZ/1KFFHHZI=\r\n\
-----END CERTIFICATE-----";

char *client_crt_buffer= "-----BEGIN CERTIFICATE-----\r\n\
MIIEhDCCAuwCCQDuE1BpeAeMwzANBgkqhkiG9w0BAQsFADCBgjELMAkGA1UEBhMC\r\n\
Q04xCzAJBgNVBAgMAkFIMQswCQYDVQQHDAJIRjEQMA4GA1UECgwHUVVFQ1RFTDEL\r\n\
MAkGA1UECwwCU1QxFjAUBgNVBAMMDTExMi4zMS44NC4xNjQxIjAgBgkqhkiG9w0B\r\n\
CQEWE2VkZGllLnpoYW5nQHF1ZWN0ZWwwIBcNMjIwMTI1MDcyMzI3WhgPMjEyMjAx\r\n\
MDEwNzIzMjdaMIGCMQswCQYDVQQGEwJDTjELMAkGA1UECAwCQUgxCzAJBgNVBAcM\r\n\
AkhGMRAwDgYDVQQKDAdRVUVDVEVMMQswCQYDVQQLDAJTVDEWMBQGA1UEAwwNMTEy\r\n\
LjMxLjg0LjE2NDEiMCAGCSqGSIb3DQEJARYTZWRkaWUuemhhbmdAcXVlY3RlbDCC\r\n\
AaIwDQYJKoZIhvcNAQEBBQADggGPADCCAYoCggGBAMt3cjY0eLEDqv8Y7FomA+7N\r\n\
G5ztAbR7+P/WxjPlodqRDZ5HQORkfAr44gAZcWsKoo4DHTInwr9JBbBnETBMnL8+\r\n\
13h1PRp5CfwXKFvjppWYvBZfeTwhWQYbSMKINoS+d1Zl11jg/+ZbSd7Fi0bYq8ip\r\n\
Hbt30H+NANQZP1XQdsCf5/kvn+vXiP4EgJc56JQ9L6ALIF2Q6F3G/PTaYItg463N\r\n\
lv/S+eRi1VMDSs8Qc+DTlVwlgZZJdSlC8Yjr5pVqoyXm8ENKfSTrdhrLiKSWJTz9\r\n\
JUr04E7SJ+CoBAnLYNPHR2y0CFS/15aCa1JbK27ZJ/0cvBvzpWdkcgrDtKIcxNYM\r\n\
9QFPpehb1N4pgqi0NPhCkc/BasfmXUaTwM4ghhi4tQRptKMdTN/kdyC+V5a8Hyhb\r\n\
Nvw5qeJlLJKpgZ9X3HQzuKstKMkxLNuDIzK9TvO7zLowr+0BetUdllq+fDjXQM0M\r\n\
+9P3Xv2VmDwGRkmZ0IjYpDjm+qqGTFVLzzVwEqVD6wIDAQABMA0GCSqGSIb3DQEB\r\n\
CwUAA4IBgQAuNVwkBhd5nyWMmV/ESNxy59Sz+5FcesGclKjs4YocgcKbLD2bS+LN\r\n\
lKk6zenES7Cq6+l3NMAxxh/QhgHUCThAfREzfPXbmiicrUfaudN4YFivpoFwKIAs\r\n\
NczsL9S3FPbzAB4nLDATacc2BK0//aKMOU2t3KLNNomKbzlR+EW3wd0F1GoZ9SY6\r\n\
sCQeLa8Wp1KarOmbvgoFL/DAiTSqjjsU/Lq24dOCCctmG+qXRZxQa4npHD4xJwQJ\r\n\
qzA0JLu4n+DgoJftm1KpvB0wuzTn6M9+wnk5rv/fGc2t4Zra8B4prEReZZVfy65d\r\n\
cb8pBdb20Yrmznj+6DR50X/o/8Qzoyj9XpxtjwF23ql0XPYCI7kB03Ms9euP0btc\r\n\
HFacHapm0qBKx+vWy0V2Qf482OWSbewqaRbud44sErNoKqpqm02yN8PpsCywpFUj\r\n\
UC5G5DzxzYspMzQv/yidti0scMSKFObseZmNGlRYymCWhXnxmoCFjLpw5RnJSB2+\r\n\
cZ/1KFFHHZI=\r\n\
-----END CERTIFICATE-----";

char *client_key_buffer= "-----BEGIN RSA PRIVATE KEY-----\r\n\
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\r\n\
-----END RSA PRIVATE KEY-----";
#endif

static void mqtt_state_exception_cb(mqtt_client_t *client)
{
	QL_MQTT_LOG("mqtt session abnormal disconnect");
	mqtt_connected = 0;
}

static void mqtt_connect_result_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_e status)
{
	QL_MQTT_LOG("status: %d", status);
	if(status == 0){
		mqtt_connected = 1;
	}
	ql_rtos_semaphore_release(mqtt_semp);
}

static void mqtt_requst_result_cb(mqtt_client_t *client, void *arg,int err)
{
	QL_MQTT_LOG("err: %d", err);
	
	ql_rtos_semaphore_release(mqtt_semp);
}

static void mqtt_inpub_data_cb(mqtt_client_t *client, void *arg, int pkt_id, const char *topic, const unsigned char *payload, unsigned short payload_len)
{
	QL_MQTT_LOG("topic: %s", topic);
	QL_MQTT_LOG("payload: %s", payload);
}

static void mqtt_disconnect_result_cb(mqtt_client_t *client, void *arg,int err){
	QL_MQTT_LOG("err: %d", err);
	
	ql_rtos_semaphore_release(mqtt_semp);
}
static void mqtt_app_thread(void * arg)
{
	int ret = 0;
	int i = 0, run_num = 1;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	mqtt_client_t  mqtt_cli;
	uint8_t nSim = 0;
	uint16_t sim_cid;
    struct mqtt_connect_client_info_t  client_info = {0};
    char *token = NULL;
    int is_user_onenet = 1;
	ql_rtos_semaphore_create(&mqtt_semp, 0);
	ql_rtos_task_sleep_s(10);

    char* client_id = (char*)malloc(256);
    char* client_user = (char*)malloc(256);
    char* client_pass = (char*)malloc(256);
    
	QL_MQTT_LOG("========== mqtt demo start ==========");
	QL_MQTT_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_MQTT_LOG("====network registered!!!!====");
	}else{
		QL_MQTT_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	QL_MQTT_LOG("===start data call====");
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
	QL_MQTT_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_MQTT_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0){
		QL_MQTT_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    QL_MQTT_LOG("info->profile_idx: %d", info.profile_idx);
	QL_MQTT_LOG("info->ip_version: %d", info.ip_version);
            
	QL_MQTT_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_MQTT_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_MQTT_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_MQTT_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

	
	while(run_num <= 100)
	{	
        int test_num = 0;
        int case_id = 0;
        if(is_user_onenet != 1)
        {   
            case_id = run_num%2;
        }

		int ret = MQTTCLIENT_SUCCESS;
		
		QL_MQTT_LOG("==============mqtt_client_test[%d]================\n",run_num);

		if(QL_DATACALL_SUCCESS != ql_bind_sim_and_profile(nSim, profile_idx, &sim_cid))
		{
			QL_MQTT_LOG("nSim or profile_idx is invalid!!!!");
			break;
		}
		
		if(ql_mqtt_client_init(&mqtt_cli, sim_cid) != MQTTCLIENT_SUCCESS){
			QL_MQTT_LOG("mqtt client init failed!!!!");
			break;
		}

		QL_MQTT_LOG("mqtt_cli:%d", mqtt_cli);

        if(is_user_onenet == 1)
        {
    		client_info.keep_alive = 60;
    		client_info.pkt_timeout = 5;
    		client_info.retry_times = 3;
    		client_info.clean_session = 1;
    		client_info.will_qos = 0;
    		client_info.will_retain = 0;
    		client_info.will_topic = NULL;
    		client_info.will_msg = NULL;
            
    		client_info.client_id = client_id;
    		client_info.client_user = client_user;
    		client_info.client_pass = client_pass;
            
            memset((void*)client_info.client_user,0,256);
            memset((void*)client_info.client_id,0,256);
            memset((void*)client_info.client_pass,0,256);
             /*token过期时间(单位为s),请按照实际具体需求计算token过期时间,本例中为当前时刻的一年后过期*/
            signed long long expire_time = 24 * 60 * 60 * 365;
            token = ql_mqtt_onenet_generate_auth_token(expire_time,MQTT_CLIENT_ONENET_PRODUCTID,MQTT_CLIENT_ONENET_DEVICENAME,MQTT_CLIENT_ONENET_VERSION,MQTT_CLIENT_ONENET_ACCESSKEY);
            
		    if(NULL != token) {
		    	memcpy((void*)client_info.client_pass, token, strlen(token));
                memcpy((void*)client_info.client_user,MQTT_CLIENT_ONENET_PRODUCTID,strlen(MQTT_CLIENT_ONENET_PRODUCTID));
                memcpy((void*)client_info.client_id,MQTT_CLIENT_ONENET_DEVICENAME,strlen(MQTT_CLIENT_ONENET_DEVICENAME));
                free(token);
                token = NULL;
		    }
            else 
            {
                break;
            }
		    QL_MQTT_LOG("clientid_str=%s", client_info.client_id);
            QL_MQTT_LOG("username_str=%s", client_info.client_user);
            QL_MQTT_LOG("password_str=%s", client_info.client_pass);
        }
        else 
        {
            client_info.keep_alive = 60;
    		client_info.pkt_timeout = 5;
    		client_info.retry_times = 3;
            client_info.clean_session = 1;
            client_info.will_qos = 0;
            client_info.will_retain = 0;
            client_info.will_topic = NULL;
            client_info.will_msg = NULL;
            client_info.client_id = MQTT_CLIENT_IDENTITY;
            client_info.client_user = MQTT_CLIENT_USER;
            client_info.client_pass = MQTT_CLIENT_PASS;
        }
        QL_MQTT_LOG("connect ssl %d onenet mode %d",case_id,is_user_onenet);
		if(case_id == 0){
			client_info.ssl_cfg = NULL;
		    if(is_user_onenet == 1)
		    {
                ret = ql_mqtt_connect(&mqtt_cli, MQTT_CLIENT_ONENET_URL, mqtt_connect_result_cb, NULL, (const struct mqtt_connect_client_info_t *)&client_info, mqtt_state_exception_cb);
		    }
            else 
            {
                ret = ql_mqtt_connect(&mqtt_cli, MQTT_CLIENT_QUECTEL_URL , mqtt_connect_result_cb, NULL, (const struct mqtt_connect_client_info_t *)&client_info, mqtt_state_exception_cb);
            }
			
		}else{
			struct mqtt_ssl_config_t quectel_ssl_cfg = {
				.ssl_ctx_id = 1,
#if USE_CRT_BUFFER
				.verify_level = MQTT_SSL_VERIFY_SERVER_CLIENT,
                .client_cert_type= QL_SSL_CLIENT_CERT_BUFFER,
                .cacert_buffer = root_ca_crt_buffer,
				.client_cert_path = client_crt_buffer,
				.client_key_path = client_key_buffer,
#else
				.verify_level = MQTT_SSL_VERIFY_NONE,
				.cacert_path = NULL,
				.client_cert_path = NULL,
				.client_key_path = NULL,
#endif			
				.client_key_pwd = NULL,
				.ssl_version = QL_SSL_VERSION_ALL,
				.sni_enable = 0,
				.ssl_negotiate_timeout = QL_SSL_NEGOTIATE_TIME_DEF,
				.ignore_invalid_certsign = 0,
				.ignore_multi_certchain_verify = 0,
				.ignore_certitem = MBEDTLS_X509_BADCERT_NOT_TRUSTED|MBEDTLS_X509_BADCERT_EXPIRED|MBEDTLS_X509_BADCERT_FUTURE,
			};
            struct mqtt_ssl_config_t ontnet_ssl_cfg = {
                .ssl_ctx_id = 1,
                .verify_level = MQTT_SSL_VERIFY_NONE,
                .cacert_path = "UFS:MQTTS-certificate.pem",
                .client_cert_path = NULL,
                .client_key_path = NULL,
                .client_key_pwd = NULL,
				.ssl_version = QL_SSL_VERSION_ALL,
				.sni_enable = 0,
				.ssl_negotiate_timeout = QL_SSL_NEGOTIATE_TIME_DEF,
				.ignore_invalid_certsign = 0,
				.ignore_multi_certchain_verify = 0,
				.ignore_certitem = MBEDTLS_X509_BADCERT_EXPIRED|MBEDTLS_X509_BADCERT_FUTURE,
            };
		    if(is_user_onenet == 1)
		    {
                client_info.ssl_cfg = &ontnet_ssl_cfg;
                ret = ql_mqtt_connect(&mqtt_cli, MQTT_CLIENT_ONENET_SSL_URL, mqtt_connect_result_cb, NULL, (const struct mqtt_connect_client_info_t *)&client_info, mqtt_state_exception_cb);
		    }
            else 
            {
                client_info.ssl_cfg = &quectel_ssl_cfg;
                ret = ql_mqtt_connect(&mqtt_cli, MQTT_CLIENT_QUECTEL_SSL_URL, mqtt_connect_result_cb, NULL, (const struct mqtt_connect_client_info_t *)&client_info, mqtt_state_exception_cb);
            }
            
		}
		if(ret  == MQTTCLIENT_WOUNDBLOCK){
			QL_MQTT_LOG("====wait connect result");
			ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
			if(mqtt_connected == 0){
				ql_mqtt_client_deinit(&mqtt_cli);
				break;
			}
		}else{
			QL_MQTT_LOG("===mqtt connect failed ,ret = %d",ret);
			break;
		}

		ql_mqtt_set_inpub_callback(&mqtt_cli, mqtt_inpub_data_cb, NULL);

        if(is_user_onenet == 1)
        {
            if(is_user_onenet == 1)
            {
                if(ql_mqtt_sub_unsub(&mqtt_cli, "$sys/417661/test_led/dp/post/json/+", 1, mqtt_requst_result_cb,NULL, 1) == MQTTCLIENT_WOUNDBLOCK){
                	QL_MQTT_LOG("======wait subscrible result");
                	ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
                }
                if(ql_mqtt_publish(&mqtt_cli, "$sys/417661/test_led/dp/post/json",MQTT_PUB_MSG0, strlen(MQTT_PUB_MSG0), 0, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
                	QL_MQTT_LOG("======wait publish result");
                	ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
                }
                if(ql_mqtt_publish(&mqtt_cli, "$sys/417661/test_led/dp/post/json", MQTT_PUB_MSG1, strlen(MQTT_PUB_MSG1), 1, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
                	QL_MQTT_LOG("======wait publish result");
                	ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
                }
                //  onenet 平台不支持qos2
                if(ql_mqtt_sub_unsub(&mqtt_cli,"$sys/417661/test_led/dp/post/json/+", 1, mqtt_requst_result_cb,NULL, 0) == MQTTCLIENT_WOUNDBLOCK){
                	QL_MQTT_LOG("=====wait unsubscrible result");
                	ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
                }
            }
        }
        else{
    		while(test_num < 10 && mqtt_connected == 1){
    			if(ql_mqtt_sub_unsub(&mqtt_cli, "test", 1, mqtt_requst_result_cb,NULL, 1) == MQTTCLIENT_WOUNDBLOCK){
    				QL_MQTT_LOG("======wait subscrible result");
    				ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
    			}

    			if(ql_mqtt_publish(&mqtt_cli, "test", "hi, mqtt qos 0", strlen("hi, mqtt qos 0"), 0, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
    				QL_MQTT_LOG("======wait publish result");
    				ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
    			}

    			if(ql_mqtt_publish(&mqtt_cli, "test", "hi, mqtt qos 1", strlen("hi, mqtt qos 1"), 1, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
    				QL_MQTT_LOG("======wait publish result");
    				ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
    			}

    			if(ql_mqtt_publish(&mqtt_cli, "test", "hi, mqtt qos 2", strlen("hi, mqtt qos 2"), 2, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
    				QL_MQTT_LOG("======wait publish result");
    				ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
    			}
    			
    			if(ql_mqtt_sub_unsub(&mqtt_cli, "test", 1, mqtt_requst_result_cb,NULL, 0) == MQTTCLIENT_WOUNDBLOCK){
    				QL_MQTT_LOG("=====wait unsubscrible result");
    				ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
    			}
    			test_num++;
    			ql_rtos_task_sleep_ms(500);
    		}
        }
        if(mqtt_connected == 1 && ql_mqtt_disconnect(&mqtt_cli, mqtt_disconnect_result_cb, NULL) == MQTTCLIENT_WOUNDBLOCK){
            QL_MQTT_LOG("=====wait disconnect result");
            ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
        }
		QL_MQTT_LOG("==============mqtt_client_test[%d] end=======%x=========\n",run_num,&mqtt_cli);
		ql_mqtt_client_deinit(&mqtt_cli);
		mqtt_connected = 0;
		run_num++;
		ql_rtos_task_sleep_s(1);
        if(is_user_onenet == 1)
        {
            break;
        }
	}
	
exit:
    if(is_user_onenet == 1)
    {
        free((void*)client_id);
        free((void*)client_user);
        free((void*)client_pass);
        client_id = NULL;
        client_user = NULL;
        client_pass = NULL;
    }

    ql_rtos_semaphore_delete(mqtt_semp);
    ql_rtos_task_delete(mqtt_task);	

   return;	
}


int ql_mqtt_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
    err = ql_rtos_task_create(&mqtt_task, 16*1024, APP_PRIORITY_ABOVE_NORMAL, "QmqttApp", mqtt_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_MQTT_LOG("mqtt_app init failed");
	}

	return err;
}

