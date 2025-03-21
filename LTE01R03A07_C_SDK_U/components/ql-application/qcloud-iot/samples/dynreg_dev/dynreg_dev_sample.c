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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_api_osi.h"
#include "ql_api_datacall.h"

#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"
#include "utils_getopt.h"

#ifdef AUTH_MODE_CERT
/* NULL cert file */
#define QCLOUD_IOT_NULL_CERT_FILENAME "./certs/Smart_light_01_cert.crt"
/* NULL key file */
#define QCLOUD_IOT_NULL_KEY_FILENAME "./certs/Smart_light_01_private.key"
#else
/* NULL device secret */
#define QCLOUD_IOT_NULL_DEVICE_SECRET "HdpsormITQtQvaQiKff5UA=="
#endif

#define PROFILE_IDX 1

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

void dynreg_dev_sample(void *arg)
{
//    int c;
//    while ((c = utils_getopt(argc, argv, "c:")) != EOF) switch (c) {
//            case 'c':
//                if (HAL_SetDevInfoFile(utils_optarg))
//                    return -1;
//                break;
//
//            default:
//                HAL_Printf(
//                    "usage: %s [options]\n"
//                    "  [-c <config file for DeviceInfo>] \n",
//                    argv[0]);
//
//                return -1;
//        }

    // init log level
    IOT_Log_Set_Level(eLOG_DEBUG);

	/*执行拨号操作，设备联网*/
	if(datacall_satrt() != 0)
	{
        Log_e("Cloud Device datacall Failed");
		goto exit;
	}

    int        ret;
    DeviceInfo sDevInfo;
    bool       infoNullFlag = false;

    memset((char *)&sDevInfo, 0, sizeof(DeviceInfo));

    ret = HAL_GetDevInfo(&sDevInfo);

#ifndef GATEWAY_ENABLED
    sDevInfo.dev_type = eCOMMON_DEV;
#else
    sDevInfo.dev_type = eGW_SUB_DEV;
#endif

#ifdef AUTH_MODE_CERT
    /* just demo the cert/key files are empty */
    if (!strcmp(sDevInfo.dev_cert_file_name, QCLOUD_IOT_NULL_CERT_FILENAME) ||
        !strcmp(sDevInfo.dev_key_file_name, QCLOUD_IOT_NULL_KEY_FILENAME)) {
        Log_d("dev Cert not exist!");
        infoNullFlag = true;
    } else {
        Log_d("dev Cert exist");
    }
#else
    /* just demo the PSK is empty */
    if (!strcmp(sDevInfo.device_secret, QCLOUD_IOT_NULL_DEVICE_SECRET)) {
        Log_d("dev psk not exist!");
        infoNullFlag = true;
    } else {
        Log_d("dev psk exist");
    }
#endif
    /* device cert/key files or PSK is empty, do dynamic register to fetch */
    if (infoNullFlag) 
    {
        if (QCLOUD_RET_SUCCESS == IOT_DynReg_Device(PROFILE_IDX, &sDevInfo)) 
        {
            ret = HAL_SetDevInfo(&sDevInfo);
            if (QCLOUD_RET_SUCCESS != ret) 
            {
                Log_e("devices info save fail");
            } 
            else 
            {
#ifdef AUTH_MODE_CERT
                Log_d(
                    "dynamic register success, productID: %s, devName: %s, CertFile: "
                    "%s, KeyFile: %s",
                    sDevInfo.product_id, sDevInfo.device_name, sDevInfo.dev_cert_file_name, sDevInfo.dev_key_file_name);
#else
                Log_d(
                    "dynamic register success,productID: %s, devName: %s, "
                    "device_secret: %s",
                    sDevInfo.product_id, sDevInfo.device_name, sDevInfo.device_secret);
#endif
            }
        } 
        else 
        {
            Log_e("%s dynamic register fail", sDevInfo.device_name);
        }
   }
exit:
    ql_rtos_task_delete(NULL);
}

int dynreg_dev_sample_start(void)
{
    int err = 0;
    int count = 0;
   	ql_task_t dynreg_dev_sample_task = NULL;
    /* 主循环进入休眠 */
    while (count < 10)
    {
        count++;
        ql_rtos_task_sleep_s(1);
        Log_e("ql_qcloud_sdk_dynreg_dev_sample_start_init  -0x%04X\n", err);
    }
	err = ql_rtos_task_create(&dynreg_dev_sample_task, 10*1024, APP_PRIORITY_NORMAL, "dynreg_dev", dynreg_dev_sample, NULL, 5);
	
	if (err != QL_OSI_SUCCESS)
	{
        return -1;
	}
    return 0;
}



