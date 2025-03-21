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
#include "ql_cnktd_sms.h"
#include "ql_api_datacall.h"

#define CNKTD_SMS_LOG(msg, ...)          QL_LOG(QL_LOG_LEVEL_INFO, "ql_cnktd_sms", msg, ##__VA_ARGS__)

ql_task_t cnktd_sms_task = NULL;

static void cnktd_sms_demo_task(void *ctx)
{
    int err = 0;
    uint16 service_state = 0;
    do{
        qapi_QT_SMS_Get_Service_Ready_Status(&service_state);
    }while (!service_state);
    CNKTD_SMS_LOG("qapi_QT_SMS_Get_Service_Ready_Status, service_state = %d", service_state);
    CNKTD_SMS_LOG("SMS READY");

    err = qapi_QT_SMS_Chset_Set(ALPHA_IRA);
    CNKTD_SMS_LOG("qapi_QT_SMS_Chset_Set err = %d", err);

    qapi_QT_SMS_Para_t sms_para = {
        .vp = 167,
        .pid = 0,
        .dcs = 0
    };
    err = qapi_QT_SMS_Para_Set(&sms_para);
    CNKTD_SMS_LOG("qapi_QT_SMS_Para_Set err = %d", err);

    qapi_QT_SMS_CPMS_Set_t sms_cpms = {
        .mem1 = QT_WMS_MEMORY_STORE_ME,
        .mem2 = QT_WMS_MEMORY_STORE_ME,
        .mem3 = QT_WMS_MEMORY_STORE_ME,
    };
    err = qapi_QT_SMS_CPMS_Set(&sms_cpms);
    CNKTD_SMS_LOG("qapi_QT_SMS_CPMS_Set err = %d", err);

    qapi_QT_SMS_Message_Store_t msg_store_num = {0};
    err = qapi_QT_SMS_Rcvd_Num(&msg_store_num);
    CNKTD_SMS_LOG("qapi_QT_SMS_Rcvd_Num err = %d, max_num=%d,used_num=%d", err, 
                  msg_store_num.max_num, 
                  msg_store_num.used_num);
    
    qapi_QT_SMS_Message_List_t sms_list = {0};
    CNKTD_SMS_LOG("sms_list = %p", &sms_list);
    err = qapi_QT_SMS_Message_List(&sms_list);
    CNKTD_SMS_LOG("qapi_QT_SMS_Rcvd_List err = %d, sum=%d", err, sms_list.sum);

    qapi_QT_SMS_Message_Rcvd_t sms_rcvd = {0};
    for(int i=0; i<sms_list.sum; i++)
    {
        CNKTD_SMS_LOG("i = %d, number = %d, type = %d",i, sms_list.indices[i].number, sms_list.indices[i].type);
        
        err = qapi_QT_SMS_Message_Read(sms_list.indices[i].number, &sms_rcvd);
        CNKTD_SMS_LOG("qapi_QT_SMS_Message_Read err = %d, sender_num = %s, data_len = %d, data = %s", err, 
                        sms_rcvd.sms_info.sender_num,
                        sms_rcvd.sms_info.data_len,
                        sms_rcvd.sms_info.data);
    }

    if(sms_list.sum)
    {
        // delete the first sms
        qapi_QT_SMS_Message_Delete(sms_list.indices[0].number);
        CNKTD_SMS_LOG("qapi_QT_SMS_Message_Delete err = %d", err);

        err = qapi_QT_SMS_Message_Read(sms_list.indices[0].number, &sms_rcvd);
        CNKTD_SMS_LOG("qapi_QT_SMS_Message_Read err = %d", err);
    }
#if 0
    // wait network ready
    err = ql_network_register_wait(0, 60);
    if(err != QL_SUCCESS){
        CNKTD_SMS_LOG("ql_network_register_wait fail ");
		goto exit;
    }

    qapi_QT_SMS_Message_Content_t send_message = {0};
    strcpy(send_message.address, "Destination Phone Number");
    strcpy(send_message.message, "hello world");
    err = qapi_QT_SMS_Message_Send(&send_message);
    CNKTD_SMS_LOG("qapi_QT_SMS_Message_Send err = %d", err);
#endif
    goto exit;
exit:
    err = ql_rtos_task_delete(NULL);
    if(err != QL_OSI_SUCCESS)
	{
		CNKTD_SMS_LOG("task deleted failed");
	}
}

QlOSStatus cnktd_sms_demo_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&cnktd_sms_task, 4*1024, APP_PRIORITY_NORMAL, "cnktd_sms_demo", cnktd_sms_demo_task, NULL, 4);
	CNKTD_SMS_LOG("cnktd_sms_demo_task err = %d", err);
    
    return err;
}