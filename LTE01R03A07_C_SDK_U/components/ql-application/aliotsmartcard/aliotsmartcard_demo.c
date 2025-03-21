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
//#include "ql_ping_app.h"
#include "ql_api_aliotsmartcard.h"
#define QL_ALIOTSMARTCARD_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_ALIOTSMARTCARD_LOG(msg, ...)			QL_LOG(QL_ALIOTSMARTCARD_LOG_LEVEL, "ql_smartcard_DEMO", msg, ##__VA_ARGS__)
#define QL_ALIOTSMARTCARD_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_smartcard_DEMO", msg, ##__VA_ARGS__)

static ql_task_t smartcard_task = NULL;
struct ql_smartcard_mnenable
{
    bool     func_hac;
	bool     func_sc;
    bool     func_sw;
    bool     func_mse;
}asc_mnenable={1,1,0,1};
#define QL_SMARTCARD_PBK_WAIT_TIMEOUT		30000 //unit:ms
#define QL_SMARTCARD_CONFIG_FILE_PATH       "UFS:ql_smartcard.json"
#define QL_SMARTCARD_SLOT_SIGN			    3 /*3:SIM1 ~ SIM3*/
#define QL_SMARTCARD_PDP_CID				1


static int datacall_satrt(void)
{
   	int ret = 0;
	int i = 0;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	uint8_t nSim = 0;
	
	ql_rtos_task_sleep_s(10);
	QL_ALIOTSMARTCARD_LOG("========== smartcard demo start ==========");
	QL_ALIOTSMARTCARD_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_ALIOTSMARTCARD_LOG("====network registered!!!!====");
	}else{
		QL_ALIOTSMARTCARD_LOG("====network register failure!!!!!====");
		goto exit;
	}
	QL_ALIOTSMARTCARD_LOG("========== smartcard demo start ==========");
	aliyun_iot_smartcard_hal_pbk_wait(QL_SMARTCARD_PBK_WAIT_TIMEOUT);
	ql_rtos_task_sleep_s(3);
	QL_ALIOTSMARTCARD_LOG("========== smartcard demo start ==========");

	ql_set_data_call_asyn_mode(nSim, QL_SMARTCARD_PDP_CID, 0);

	QL_ALIOTSMARTCARD_LOG("===start data call====");
	ret=ql_start_data_call(nSim, QL_SMARTCARD_PDP_CID, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
	QL_ALIOTSMARTCARD_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_ALIOTSMARTCARD_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, QL_SMARTCARD_PDP_CID, &info);
	if(ret != 0){
		QL_ALIOTSMARTCARD_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, QL_SMARTCARD_PDP_CID);
		goto exit;
	}
    QL_ALIOTSMARTCARD_LOG("info->QL_SMARTCARD_PDP_CID: %d", info.profile_idx);
	QL_ALIOTSMARTCARD_LOG("info->ip_version: %d", info.ip_version);
            
	QL_ALIOTSMARTCARD_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_ALIOTSMARTCARD_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_ALIOTSMARTCARD_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_ALIOTSMARTCARD_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

	return 0;
exit:
    return -1;
}

static void aliotsmartcard_app_thread(void * arg)
{
	int ret = 0;
	int i = 0;
	uint8_t func_save = TRUE;/*Parameter update, default save*/
	uint8_t update_mnenable_para = FALSE;
	uint8_t update_mnpolicy_para = FALSE;
	uint8_t update_scan_carrier = FALSE;
	uint8_t update_manual_switch = FALSE;
	bool old_func_hac = TRUE;
	bool old_func_sw = FALSE;
	const char *ping_dest_ip = "114.114.114.114";
	uint16_t delay_threshold = 60;
	uint8_t slot_cnt = QL_SMARTCARD_SLOT_SIGN;
	char viccid_buf[32] = {0};
	aliyun_iot_smartcard_sim_t sim_info[QL_SMARTCARD_SLOT_SIGN] = {0};
	bool mode = FALSE;
			
	/*step1: wait card init done and active*/
	ret = datacall_satrt();
	if (ret < 0) {
		QL_ALIOTSMARTCARD_LOG("smartcard active ret=%d", ret);
		goto exit;
	}

	QL_ALIOTSMARTCARD_LOG("smartcard runn...");
	/*step2: init config path*/
	ret = aliyun_iot_smartcard_parse_configs(QL_SMARTCARD_CONFIG_FILE_PATH);
	if (ret < 0) {
		QL_ALIOTSMARTCARD_LOG("smartcard parse_configs failed ret=%d", ret);
		goto exit;
	}


	/*step3: start card thread*/
    for (int i = 0; i < 3; i++) {
        ret = aliyun_iot_smartcard_init();
        if (ret == 0) {
            break;
        }
		aliyun_iot_smartcard_deinit();
        QL_ALIOTSMARTCARD_LOG("aliyun_iot_smartcard_init fail %d times, ret = %d!",i+1, ret);
        ql_rtos_task_sleep_ms(2000);
        QL_ALIOTSMARTCARD_LOG("wait for 2 second and retry");
    }
	if (3 == i) {
		QL_ALIOTSMARTCARD_LOG("aliyun_iot_smartcard_init failed ret=%d", ret);
		goto exit;
	}
	

	/*step4: API get parameters, exp.*/
	ret = aliyun_iot_smartcard_get_enable((bool*)&old_func_hac);
	if(0 != ret){
		QL_ALIOTSMARTCARD_LOG("smartcard get func_hac ret=%d", ret);
		goto exit;
	}
	ret = aliyun_iot_smartcard_get_switch_flag((bool*)&old_func_sw);
	if(0 != ret){
		QL_ALIOTSMARTCARD_LOG("smartcard get func_sw ret=%d", ret);
		goto exit;
	}
	

	/*step5: API set parameters*/
	if(TRUE == update_mnenable_para){
		ret = aliyun_iot_smartcard_set_enable(asc_mnenable.func_hac);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard func_hac ret=%d", ret);
			goto exit;
		}
		aliyun_iot_smartcard_set_scan_flag(asc_mnenable.func_sc);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard func_sc ret=%d", ret);
			goto exit;
		}
		aliyun_iot_smartcard_set_switch_flag(asc_mnenable.func_sw);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard func_sw ret=%d", ret);
			goto exit;
		}
		aliyun_iot_smartcard_set_manu_switch_flag(asc_mnenable.func_mse);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard func_mse ret=%d", ret);
			goto exit;
		}
		
		/*modify "func_hac" parameters require reinitialization*/
		if(asc_mnenable.func_hac != old_func_hac){
			aliyun_iot_smartcard_deinit();
			ret = aliyun_iot_smartcard_init();
			if (ret != 0) {
				QL_ALIOTSMARTCARD_LOG("smartcard update param, init fail ret=%d", ret);
				goto exit;
			}
		}

		/*modify "func_sw" parameters, Need to modify monitor task at the same time*/
		if((TRUE == asc_mnenable.func_sw)&&(FALSE == old_func_sw)){
			ret = aliyun_iot_smartcard_start_monitor();/*start monitor task*/
			if (ret != 0) {
				QL_ALIOTSMARTCARD_LOG("smartcard start monitor fail ret=%d", ret);
				goto exit;
			}
		}
		if((TRUE == old_func_sw)&&(FALSE == asc_mnenable.func_sw)){
			ret = aliyun_iot_smartcard_stop_monitor();/*stop monitor task*/
			if (ret != 0) {
				QL_ALIOTSMARTCARD_LOG("smartcard stop monitor fail ret=%d", ret);
				goto exit;
			}
		}


		/*Parameter modification, update configuration file*/
		if(TRUE == func_save){
			aliyun_iot_smartcard_update_configs();
		}
	}


	/*step6: API set mnpolicy, exp.*/
	if(TRUE == update_mnpolicy_para){
		ret = aliyun_iot_smartcard_set_delay_threshold(delay_threshold);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard delay_threshold ret=%d", ret);
			goto exit;
		}
		
		ret = aliyun_iot_smartcard_set_ping_addr(ping_dest_ip);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard ping_dest_ip ret=%d", ret);
			goto exit;
		}
	}


	/*step7: API manual switch.*/
	if(TRUE == update_manual_switch){
		ret = aliyun_iot_smartcard_manual_switch(SWITCH_TO_SIM1);
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard manual switch ret=%d", ret);
			goto exit;
		}
	}


	/*step8: API auto switch.*/
	if(TRUE == update_scan_carrier){
		ret = aliyun_iot_smartcard_scan_carrier();
		if(0 != ret){
			QL_ALIOTSMARTCARD_LOG("smartcard scan carrier ret=%d", ret);
			goto exit;
		}
	}
	

	/*step9: API get cardlist info.*/
	ret = aliyun_iot_is_smartcard(&mode);
	if(0 != ret){
		QL_ALIOTSMARTCARD_LOG("not smartcard!");
		goto exit;
	}

	ret = aliyun_iot_smartcard_get_aliiccid(viccid_buf, sizeof(viccid_buf));
	if(0 != ret){
		QL_ALIOTSMARTCARD_LOG("get aliiccid ret=%d", ret);
		goto exit;
	}

	memset(&sim_info[0], 0, sizeof(aliyun_iot_smartcard_sim_t)*QL_SMARTCARD_SLOT_SIGN);
	ret = aliyun_iot_smartcard_get_all_sim_info(&slot_cnt, &sim_info[0]);
	if(0 != ret){
		QL_ALIOTSMARTCARD_LOG("get sim info ret=%d", ret);
		goto exit;
	}

	slot_cnt = 0;
	QL_ALIOTSMARTCARD_LOG("smartcard %d iccid=%s, act=%d, active=%d, car_name=%s, car_type=%d, rssi=%d", slot_cnt,
		sim_info[slot_cnt].iccid, sim_info[slot_cnt].act, sim_info[slot_cnt].active, 
		sim_info[slot_cnt].car_name, sim_info[slot_cnt].car_type, sim_info[slot_cnt].rssi);
	slot_cnt = 1;
	QL_ALIOTSMARTCARD_LOG("smartcard %d iccid=%s, act=%d, active=%d, car_name=%s, car_type=%d, rssi=%d", slot_cnt,
		sim_info[slot_cnt].iccid, sim_info[slot_cnt].act, sim_info[slot_cnt].active, 
		sim_info[slot_cnt].car_name, sim_info[slot_cnt].car_type, sim_info[slot_cnt].rssi);
	slot_cnt = 2;
	QL_ALIOTSMARTCARD_LOG("smartcard %d iccid=%s, act=%d, active=%d, car_name=%s, car_type=%d, rssi=%d", slot_cnt,
		sim_info[slot_cnt].iccid, sim_info[slot_cnt].act, sim_info[slot_cnt].active, 
		sim_info[slot_cnt].car_name, sim_info[slot_cnt].car_type, sim_info[slot_cnt].rssi);

	while(1){
		QL_ALIOTSMARTCARD_LOG("smartcard running");
		ql_rtos_task_sleep_s(1);
	}
	QL_ALIOTSMARTCARD_LOG("smartcard task exit!");
exit:
	while(1){
		QL_ALIOTSMARTCARD_LOG("smartcard running");
		ql_rtos_task_sleep_s(1);
	}
	QL_ALIOTSMARTCARD_LOG("smartcard task exit!");
   smartcard_task = NULL;	
   ql_rtos_task_delete(NULL);/*self-destruction*/

   return;	
}


int ql_aliotsmartcard_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&smartcard_task, 4*1024, APP_PRIORITY_NORMAL, "ascApp", aliotsmartcard_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_ALIOTSMARTCARD_LOG("ping_app init failed");
	}

	return err;
}

