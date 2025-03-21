/*
 * Tencent is pleased to support the open source community by making IoT Hub
 available.
 * Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

 * Licensed under the MIT License (the "License"); you may not use this file
 except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND,
 * either express or implied. See the License for the specific language
 governing permissions and
 * limitations under the License.
 *
 */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_api_osi.h"
#include "ql_api_datacall.h"

#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"
#include "utils_getopt.h"

#define PROFILE_IDX 1

#ifdef DEBUG_DEV_INFO_USED
#define PRODUCT_ID "KSS57PUDXP"
#define DEVICE_NAME "Smart_light_02"
#define DEVICE_SECRET "mndRH7Y75zyFbXIvb5H2Yw=="
#define DEVICE_CERT "./certs/Smart_light_01_cert.crt"
#define DEVICE_PRIVATE "./certs/Smart_light_01_private.key"
#define REGION "china"
#else
#define DEVICE_INFO_JSON_DOC "UFS:device_info.json"
#endif

#ifdef AUTH_MODE_CERT
static char sg_cert_file[PATH_MAX + 1];  // full path of device cert file
static char sg_key_file[PATH_MAX + 1];   // full path of device key file
#endif

static DeviceInfo sg_devInfo;
static int        sg_count         = 0;
static int        sg_sub_packet_id = -1;
ql_task_t mqtt_sample_task = NULL;

#ifdef DEBUG_DEV_INFO_USED
extern int device_info_copy(void *pdst, void *psrc, uint8_t max_len);
#endif


// user's log print callback
static bool log_handler(const char *message)
{
    // return true if print success
    return false;
}

// MQTT event callback
static void _mqtt_event_handler(void *pclient, void *handle_context, MQTTEventMsg *msg)
{
    MQTTMessage *mqtt_messge = (MQTTMessage *)msg->msg;
    uintptr_t    packet_id   = (uintptr_t)msg->msg;

    switch (msg->event_type) {
        case MQTT_EVENT_UNDEF:
            Log_i("undefined event occur.");
            break;

        case MQTT_EVENT_DISCONNECT:
            Log_i("MQTT disconnect.");
            break;

        case MQTT_EVENT_RECONNECT:
            Log_i("MQTT reconnect.");
            break;

        case MQTT_EVENT_PUBLISH_RECVEIVED:
            Log_i(
                "topic message arrived but without any related handle: topic=%.*s, "
                "topic_msg=%.*s",
                mqtt_messge->topic_len, mqtt_messge->ptopic, mqtt_messge->payload_len, mqtt_messge->payload);
            break;
        case MQTT_EVENT_SUBCRIBE_SUCCESS:
            Log_i("subscribe success, packet-id=%u", (unsigned int)packet_id);
            sg_sub_packet_id = packet_id;
            break;

        case MQTT_EVENT_SUBCRIBE_TIMEOUT:
            Log_i("subscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
            sg_sub_packet_id = packet_id;
            break;

        case MQTT_EVENT_SUBCRIBE_NACK:
            Log_i("subscribe nack, packet-id=%u", (unsigned int)packet_id);
            sg_sub_packet_id = packet_id;
            break;

        case MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            Log_i("unsubscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            Log_i("unsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case MQTT_EVENT_UNSUBCRIBE_NACK:
            Log_i("unsubscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case MQTT_EVENT_PUBLISH_SUCCESS:
            Log_i("publish success, packet-id=%u", (unsigned int)packet_id);
            break;

        case MQTT_EVENT_PUBLISH_TIMEOUT:
            Log_i("publish timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case MQTT_EVENT_PUBLISH_NACK:
            Log_i("publish nack, packet-id=%u", (unsigned int)packet_id);
            break;
        default:
            Log_i("Should NOT arrive here.");
            break;
    }
}

// Setup MQTT construct parameters
static int _setup_connect_init_params(MQTTInitParams *initParams)
{
    int ret;

#ifdef DEBUG_DEV_INFO_USED
	ret = HAL_GetDevInfo((void *)&sg_devInfo);
#else
	ret = HAL_GetDevInfoFromFile(DEVICE_INFO_JSON_DOC, (void *)&sg_devInfo);
#endif
    if (QCLOUD_RET_SUCCESS != ret) {
        return ret;
    }

    initParams->region      = sg_devInfo.region;
    initParams->device_name = sg_devInfo.device_name;
    initParams->product_id  = sg_devInfo.product_id;

#ifdef AUTH_MODE_CERT
//    char  certs_dir[PATH_MAX + 1] = "certs";
//    char  current_path[PATH_MAX + 1];
//    char *cwd = getcwd(current_path, sizeof(current_path));
//
//    if (cwd == NULL) {
//        Log_e("getcwd return NULL");
//        return QCLOUD_ERR_FAILURE;
//    }

//#ifdef WIN32
//    sprintf(sg_cert_file, "%s\\%s\\%s", current_path, certs_dir, sg_devInfo.dev_cert_file_name);
//    sprintf(sg_key_file, "%s\\%s\\%s", current_path, certs_dir, sg_devInfo.dev_key_file_name);
//#else
//    sprintf(sg_cert_file, "%s/%s/%s", current_path, certs_dir, sg_devInfo.dev_cert_file_name);
//    sprintf(sg_key_file, "%s/%s/%s", current_path, certs_dir, sg_devInfo.dev_key_file_name);
//#endif
    sprintf(sg_cert_file, "%s",  sg_devInfo.dev_cert_file_name);
    sprintf(sg_key_file, "%s",  sg_devInfo.dev_key_file_name);

    initParams->cert_file = sg_cert_file;
    initParams->key_file  = sg_key_file;
#else
    initParams->device_secret = sg_devInfo.device_secret;
#endif

    initParams->command_timeout        = QCLOUD_IOT_MQTT_COMMAND_TIMEOUT;
    initParams->keep_alive_interval_ms = QCLOUD_IOT_MQTT_KEEP_ALIVE_INTERNAL;

    initParams->auto_connect_enable  = 1;
    initParams->event_handle.h_fp    = _mqtt_event_handler;
    initParams->event_handle.context = NULL;

    return QCLOUD_RET_SUCCESS;
}

#define MAX_SIZE_OF_TOPIC_CONTENT 100
// publish MQTT msg
static int _publish_test_msg(void *client, char *topic_keyword, QoS qos)
{
    char topicName[128] = {0};
    sprintf(topicName, "%s/%s/%s", sg_devInfo.product_id, sg_devInfo.device_name, topic_keyword);

    PublishParams pub_params = DEFAULT_PUB_PARAMS;
    pub_params.qos           = qos;

    char topic_content[MAX_SIZE_OF_TOPIC_CONTENT + 1] = {0};

    int size = HAL_Snprintf(topic_content, sizeof(topic_content), "{\"action\": \"publish_test\", \"count\": \"%d\"}",
                            sg_count++);
    if (size < 0 || size > sizeof(topic_content) - 1) {
        Log_e("payload content length not enough! content size:%d  buf size:%d", size, (int)sizeof(topic_content));
        return -3;
    }

    pub_params.payload     = topic_content;
    pub_params.payload_len = strlen(topic_content);

    return IOT_MQTT_Publish(client, topicName, &pub_params);
}

// callback when MQTT msg arrives
static void on_message_callback(void *pClient, MQTTMessage *message, void *userData)
{
    if (message == NULL) {
        return;
    }

    Log_i("Receive Message With topicName:%.*s, payload:%.*s", (int)message->topic_len, message->ptopic,
          (int)message->payload_len, (char *)message->payload);
}

// subscribe MQTT topic and wait for sub result
static int subscribe_topic_wait_result(void *client, char *topic_keyword, QoS qos)
{
    char topic_name[128] = {0};

    int size = HAL_Snprintf(topic_name, sizeof(topic_name), "%s/%s/%s", sg_devInfo.product_id, \
                            sg_devInfo.device_name,  topic_keyword);
    if (size < 0 || size > sizeof(topic_name) - 1) {
        Log_e("topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(topic_name));
        return QCLOUD_ERR_FAILURE;
    }

    SubscribeParams sub_params = DEFAULT_SUB_PARAMS;
    sub_params.qos = qos;
    sub_params.on_message_handler = on_message_callback;

    int rc = IOT_MQTT_Subscribe(client, topic_name, &sub_params);
    if (rc < 0) {
        Log_e("MQTT subscribe FAILED: %d", rc);
        return rc;
    }

    int wait_cnt = 10;
    while (!IOT_MQTT_IsSubReady(client, topic_name) && (wait_cnt > 0)) {
        // wait for subscription result
        rc = IOT_MQTT_Yield(client, 1000);
        if (rc) {
            Log_e("MQTT error: %d", rc);
            return rc;
        }
        wait_cnt--;
    }

    if (wait_cnt > 0) {
        return QCLOUD_RET_SUCCESS;
    } else {
        Log_e("wait for subscribe result timeout!");
        return QCLOUD_ERR_FAILURE;
    }
}

#ifdef LOG_UPLOAD
// init log upload module
static int _init_log_upload(MQTTInitParams *init_params)
{
    LogUploadInitParams log_init_params;
    memset(&log_init_params, 0, sizeof(LogUploadInitParams));

    log_init_params.product_id = init_params->product_id;
    log_init_params.device_name = init_params->device_name;
#ifdef AUTH_MODE_CERT
    log_init_params.sign_key = init_params->cert_file;
#else
    log_init_params.sign_key = init_params->device_secret;
#endif

#if defined(__linux__) || defined(WIN32)
    log_init_params.read_func = HAL_Log_Read;
    log_init_params.save_func = HAL_Log_Save;
    log_init_params.del_func = HAL_Log_Del;
    log_init_params.get_size_func = HAL_Log_Get_Size;
#endif

	log_init_params.profile_idx = init_params->profile_idx;

    return IOT_Log_Init_Uploader(&log_init_params);
}

#endif

//static bool sg_loop_test = false;
static bool sg_loop_test = true;
//static int  parse_arguments(int argc, char **argv)
//{
//    int c;
//    while ((c = utils_getopt(argc, argv, "c:l")) != EOF) switch (c) {
//            case 'c':
//                if (HAL_SetDevInfoFile(utils_optarg))
//                    return -1;
//                break;
//
//            case 'l':
//                sg_loop_test = true;
//                break;
//
//            default:
//                HAL_Printf(
//                    "usage: %s [options]\n"
//                    "  [-c <config file for DeviceInfo>] \n"
//                    "  [-l ] loop test or not\n",
//                    argv[0]);
//                return -1;
//        }
//    return 0;
//}
static int datacall_satrt(void)
{
    int ret = 0;
    int i = 0;
    ql_data_call_info_s info;
    char ip4_addr_str[16] = {0};
    uint8_t nSim = 0;

    ql_rtos_task_sleep_s(10);

    Log_e("wait for network register done");

    while ((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10)
    {
        i++;
        ql_rtos_task_sleep_s(1);
    }
    if (ret == 0)
    {
        i = 0;
        Log_e("====network registered!!!!====");
    }
    else
    {
        Log_e("====network register failure!!!!!====");
        goto exit;
    }

    ql_set_data_call_asyn_mode(nSim, PROFILE_IDX, 0);

    Log_e("===start data call====");
    ret = ql_start_data_call(nSim, PROFILE_IDX, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
    Log_e("===data call result:%d", ret);
    if (ret != 0)
    {
        Log_e("====data call failure!!!!=====");
    }
    memset(&info, 0x00, sizeof(ql_data_call_info_s));

    ret = ql_get_data_call_info(nSim, PROFILE_IDX, &info);
    if (ret != 0)
    {
        Log_e("ql_get_data_call_info ret: %d", ret);
        ql_stop_data_call(nSim, PROFILE_IDX);
        goto exit;
    }
    
    Log_i("info.profile_idx: %d", info.profile_idx);
    Log_i("info.ip_version: %d", info.ip_version);
    Log_i("info.v4.state: %d", info.v4.state);
    //Log_i("info.v4.reconnect: %d", info.v4.reconnect);
    
    inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
    Log_i("info.v4.addr.ip: %s", ip4_addr_str);
    
    inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
    Log_i("info.v4.addr.pri_dns: %s", ip4_addr_str);
    
    inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
    Log_i("info.v4.addr.sec_dns: %s", ip4_addr_str);

    return 0;
exit:
    return -1;
}

static void mqtt_sample(void *argv)
{
    int rc;
    void *client =NULL;
    // init log level
    IOT_Log_Set_Level(eLOG_DEBUG);
    IOT_Log_Set_MessageHandler(log_handler);
    
	/*执行拨号操作，设备联网*/
	if(datacall_satrt() != 0)
	{
		goto exit;
	}

#ifdef DEBUG_DEV_INFO_USED
	DeviceInfo devInfo = 
    {
		.product_id = PRODUCT_ID,
		.device_name = DEVICE_NAME,
		.client_id = ""PRODUCT_ID"_"DEVICE_NAME"",
		.region = REGION,
		.dev_type = eCOMMON_DEV,
	};
#ifndef AUTH_MODE_CERT
     device_info_copy(devInfo.device_secret, DEVICE_SECRET,MAX_SIZE_OF_DEVICE_SECRET);  // set dev cert file name,
#else
     device_info_copy(devInfo.dev_cert_file_name, DEVICE_CERT,MAX_SIZE_OF_DEVICE_CERT_FILE_NAME);  // set dev cert file name
     device_info_copy(devInfo.dev_key_file_name, DEVICE_PRIVATE,MAX_SIZE_OF_DEVICE_SECRET_FILE_NAME);  // set dev cert file name
#endif
	HAL_SetDevInfo(&devInfo);
#endif

    // init connection
    MQTTInitParams init_params = DEFAULT_MQTTINIT_PARAMS;
    rc                         = _setup_connect_init_params(&init_params);
    if (rc != QCLOUD_RET_SUCCESS) {
        Log_e("init params error, rc = %d", rc);
        goto exit;
    }

#ifdef LOG_UPLOAD
    // _init_log_upload should be done after _setup_connect_init_params and before
    // IOT_MQTT_Construct
    rc = _init_log_upload(&init_params);
    if (rc != QCLOUD_RET_SUCCESS)
        Log_e("init log upload error, rc = %d", rc);
#endif

    // create MQTT client and connect with server
    client = IOT_MQTT_Construct(&init_params);
    if (client != NULL) {
        Log_i("Cloud Device Construct Success");
    } else {
        rc = IOT_MQTT_GetErrCode();
        Log_e("MQTT Construct failed, rc = %d", rc);
        goto exit;
    }

#ifdef SYSTEM_COMM
    long time = 0;
    // get system timestamp from server
    rc = IOT_Get_Sys_Resource(client, eRESOURCE_TIME, &sg_devInfo, &time);	
    if (QCLOUD_RET_SUCCESS == rc) {
        Log_i("system time is %ld", time);
    } else {
        Log_e("get system time failed!");
    }
#endif

    // subscribe normal topics here
    rc = subscribe_topic_wait_result(client, "data", QOS0);
    if (rc < 0) {
        Log_e("Client Subscribe Topic Failed: %d", rc);
        goto exit;
    }

    // wait for subscription result
    IOT_MQTT_Yield(client, 500);

    do {
        if (sg_sub_packet_id > 0) {
            rc = _publish_test_msg(client, "data", QOS1);
            if (rc < 0) {
                Log_e("client publish topic failed :%d.", rc);
            }
        }

        rc = IOT_MQTT_Yield(client, 500);
        if (rc == QCLOUD_ERR_MQTT_ATTEMPTING_RECONNECT) {
            HAL_SleepMs(1000);
            continue;
        } else if (rc != QCLOUD_RET_SUCCESS && rc != QCLOUD_RET_MQTT_RECONNECTED) {
            Log_e("exit with error: %d", rc);
            break;
        }

        if (sg_loop_test)
            HAL_SleepMs(1000);

    } while (sg_loop_test);

    rc = IOT_MQTT_Destroy(&client);
	IOT_Log_Upload(true);
	
#ifdef LOG_UPLOAD	
	IOT_Log_Fini_Uploader();
#endif
exit:
    Log_e("task exit!");
    if(client!=NULL)
    {
        rc = IOT_MQTT_Destroy(&client);
    }
    ql_rtos_task_delete(NULL);
}

//static void ql_nw_status_callback(int profile_idx, int nw_status)
//{
//	Log_i("profile(%d) status: %d", profile_idx, nw_status);
//}

//static int datacall_satrt(void)
//{
//	ql_data_call_info_s info ;
//    memset(&info, 0x00, sizeof(ql_data_call_info_s));
//	char ip4_addr_str[16] = {0};
//	uint8_t nSim = 0;
//    
//	Log_d("wait for network register done");
//
//	if(ql_network_register_wait(nSim,120) != 0)
//	{
//		Log_e("*** network register fail ***");
//		return -1;
//	}
//	else
//	{
//		Log_d("doing network activating ...");
//		
//		ql_nw_register_cb(ql_nw_status_callback);
//        ql_set_data_call_asyn_mode(nSim, PROFILE_IDX, 1); 
//		if(ql_start_data_call(PROFILE_IDX, 0, "3gnet.mnc001.mcc460.gprs", NULL, NULL, 0) == 0)
//		{
//			ql_get_data_call_info(PROFILE_IDX, 0, &info);
//			
//			Log_i("info.profile_idx: %d", info.profile_idx);
//			Log_i("info.ip_version: %d", info.ip_version);
//			Log_i("info.v4.state: %d", info.v4.state);
//			//Log_i("info.v4.reconnect: %d", info.v4.reconnect);
//			
//			inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
//			Log_i("info.v4.addr.ip: %s", ip4_addr_str);
//			
//			inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
//			Log_i("info.v4.addr.pri_dns: %s", ip4_addr_str);
//			
//			inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
//			Log_i("info.v4.addr.sec_dns: %s", ip4_addr_str);
//
//			return 0;
//		}
//
//		Log_e("*** network activated fail ***");
//		return -1;
//	}
//}

//int mqtt_sample_start(void)
//{
//	ql_task_t mqtt_sample_task = NULL;
//
//	if(datacall_satrt() == 0)
//		return ql_rtos_task_create(&mqtt_sample_task, 8*1024, 100, "mqtt_sample", main_entry, NULL);
//
//	return -1;
//}
int mqtt_sample_start(void)
{
    int err = 0;
    int count = 0;
    /* 主循环进入休眠 */
    while (count < 10)
    {
        count++;
        ql_rtos_task_sleep_s(1);
        Log_e("ql_qcloud_sdk_mqtt_sample_start_init  -0x%04X\n", err);
    }
	err = ql_rtos_task_create(&mqtt_sample_task, 10*1024, APP_PRIORITY_NORMAL, "mqtt_sample", mqtt_sample, NULL, 5);
	if (err != QL_OSI_SUCCESS)
	{
        return -1;
	}
    return 0;
}


