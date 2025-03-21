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
10/09/2023  marvin          create

=================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_common.h"
#include "ql_log.h"
#include "ql_api_l2tp.h"
#include "ql_api_osi.h"
#include "ql_api_datacall.h"

ql_task_t l2tp_task = NULL;

void ql_l2tp_event_callback(unsigned int event_type, ql_l2tp_errcode_e errcode, void *ctx)
{
    QL_L2TP_LOG("event_type:0x%x, errcode:0x%x", event_type, errcode);

    switch(event_type)
	{
        case QUEC_L2TP_START_RSP_IND:
        {
            if(QL_L2TP_SUCCESS == errcode){
                QL_L2TP_LOG("L2TP start success");
            }else{
                QL_L2TP_LOG("L2TP start fail");
            }
        }
        break;
        case QUEC_L2TP_STOP_RSP_IND:
        {
            if(QL_L2TP_SUCCESS == errcode){
                QL_L2TP_LOG("L2TP stop success");
            }else{
                QL_L2TP_LOG("L2TP stop fail");
            }
        }
        break;
        case QUEC_L2TP_DEACTIVE_IND:
        {
            QL_L2TP_LOG("L2TP disconnected");
        }
        break;
    }
}

void l2tp_demo_task(void * param)
{
    uint8_t nSim = 0;
    uint8_t profile_idx = 1;
    uint8_t auth = QL_L2TP_PPP_AUTH_CHAP;

    ip_addr_t server_ip = {0};
    char *ip_str = "120.197.216.227";
    ipaddr_aton(ip_str, &server_ip);

    /*wait register network*/
    if(ql_network_register_wait(nSim, 60) != 0)
    {
        QL_L2TP_LOG("====network register failure!!!!!====");
		goto exit;
    }

    ql_l2tp_register_cb(ql_l2tp_event_callback, NULL);

    ql_l2tp_opt_set(QL_L2TP_OPT_SIM,       &nSim, sizeof(uint8_t));
    ql_l2tp_opt_set(QL_L2TP_OPT_PDPCID,    &profile_idx, sizeof(uint8_t));
    ql_l2tp_opt_set(QL_L2TP_OPT_SERVER_IP, &server_ip, sizeof(server_ip));
    ql_l2tp_opt_set(QL_L2TP_PPP_AUTH_TYPE, &auth, sizeof(uint8_t));
    ql_l2tp_opt_set(QL_L2TP_PPP_CHAP_USER, "quectel", 7);
    ql_l2tp_opt_set(QL_L2TP_PPP_CHAP_PWD,  "quectel", 7);

    ql_l2tp_start(NULL);

    /*
    * 调用ql_l2tp_start后在ql_l2tp_event_callback中等待QUEC_L2TP_START_RSP_IND事件.
    * 在nSim & profile_idx上成功建立L2TP通道后, 所有使用nSim & profile_idx 进行的网络业务都会封装成L2TP包再发送到网络.
    */

    while(1)
    {
        ql_rtos_task_sleep_s(1);
    }

exit:
    ql_rtos_task_delete(NULL);
}


QlOSStatus ql_l2tp_app_init(void)
{
	return ql_rtos_task_create(&l2tp_task, 4096, APP_PRIORITY_NORMAL, "Ql2tpApp", l2tp_demo_task, NULL, 2);
}

