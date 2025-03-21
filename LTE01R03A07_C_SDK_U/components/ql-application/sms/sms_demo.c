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
10/10/2020  marvin          create

=================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_common.h"
#include "ql_api_osi.h"
#include "ql_api_sms.h"
#include "ql_log.h"

ql_task_t sms_task = NULL;
ql_sem_t  sms_init_sem = NULL;
ql_sem_t  sms_list_sem = NULL;

void user_sms_event_callback(uint8_t nSim, int event_id, void *ctx)
{
	switch(event_id)
	{
		case QL_SMS_INIT_OK_IND:
		{
			QL_SMS_LOG("QL_SMS_INIT_OK_IND");
			ql_rtos_semaphore_release(sms_init_sem);
			break;
		}
		case QL_SMS_NEW_MSG_IND:
		{
			ql_sms_new_s *msg = (ql_sms_new_s *)ctx;
			QL_SMS_LOG("sim=%d, index=%d, storage memory=%d", nSim, msg->index, msg->mem);
			break;
		}
		case QL_SMS_LIST_IND:
		{
#if 0
			ql_sms_msg_s *msg = (ql_sms_msg_s *)ctx;
			QL_SMS_LOG("sim=%d,index=%d, msg = %s",nSim, msg->index, msg->buf);
#endif
			break;
		}
        case QL_SMS_LIST_EX_IND:
        {
            ql_sms_recv_s *msg = (ql_sms_recv_s *)ctx;
            QL_SMS_LOG("index=%d,os=%s,tooa=%u,status=%d,fo=0x%x,dcs=0x%x,scst=%d/%d/%d %d:%d:%d±%d,uid=%u,total=%u,seg=%u,dataLen=%d,data=%s",
                msg->index,msg->oa,msg->tooa,msg->status,msg->fo,msg->dcs,
                msg->scts.uYear,msg->scts.uMonth,msg->scts.uDay,msg->scts.uHour,msg->scts.uMinute,msg->scts.uSecond,msg->scts.iZone,
                msg->uid,msg->msg_total,msg->msg_seg,msg->dataLen,msg->data);
            break;
        }
		case QL_SMS_LIST_END_IND:
		{
			QL_SMS_LOG("QL_SMS_LIST_END_IND");
			ql_rtos_semaphore_release(sms_list_sem);
			break;
		}
		case QL_SMS_MEM_FULL_IND:
		{
			ql_sms_new_s *msg = (ql_sms_new_s *)ctx;
			QL_SMS_LOG("QL_SMS_MEM_FULL_IND sim=%d, memory=%d",nSim,msg->mem);
			break;
		}
		case QL_SMS_REPORT_IND:
		{
            ql_sms_report_s *msg = (ql_sms_report_s *)ctx;
            QL_SMS_LOG("QL_SMS_REPORT_IND sim=%d, fo=%02x, mr=%02x, st=%02x, ra_size=%d, tora=%02x, ra=%s, scst=%d/%d/%d %d:%d:%d,%d, dt=%d/%d/%d %d:%d:%d,%d", \
                        nSim, msg->fo, msg->mr, msg->st, msg->ra_size, msg->tora, (char *)&msg->ra, \
                        msg->scts.uYear,msg->scts.uMonth,msg->scts.uDay,msg->scts.uHour,msg->scts.uMinute,msg->scts.uSecond,msg->scts.iZone, \
                        msg->dt.uYear,msg->dt.uMonth,msg->dt.uDay,msg->dt.uHour,msg->dt.uMinute,msg->dt.uSecond,msg->dt.iZone);
			break;
		}
		default :
			break;
	}
}


void sms_demo_task(void * param)
{
	char addr[20] = {0};
	uint8_t nSim = 0;
    uint8_t sms_init_done = false;
	QL_SMS_LOG("enter");

	if(ql_rtos_semaphore_create(&sms_init_sem, 0) != QL_OSI_SUCCESS)
	{
		QL_SMS_LOG("sms_init_sem created failed");
        goto exit;
	}

	if(ql_rtos_semaphore_create(&sms_list_sem, 0) != QL_OSI_SUCCESS)
	{
		QL_SMS_LOG("sms_init_sem created failed");
        goto exit;
	}

	ql_sms_callback_register(user_sms_event_callback);
	
	//wait sms ok
    ql_sms_get_init_status(nSim, &sms_init_done);
	if(!sms_init_done)
    {
		if(ql_rtos_semaphore_wait(sms_init_sem, QL_WAIT_FOREVER)){
			QL_SMS_LOG("Waiting for SMS init timeout");
		}
	}
	
	if(QL_SMS_SUCCESS == ql_sms_get_center_address(nSim, addr, sizeof(addr))){
		QL_SMS_LOG("ql_sms_get_center_address OK, addr=%s",addr);
	}else{
		QL_SMS_LOG("ql_sms_get_center_address FAIL");
	}
    
	ql_sms_set_code_mode(QL_CS_GSM);
    
#if 0

    //Send English text message
	if(QL_SMS_SUCCESS == ql_sms_send_msg(nSim,"+8610086","~!@#$%^&*()_+<>?:{}|", GSM)){
		QL_SMS_LOG("ql_sms_send_msg OK");
	}else{
		QL_SMS_LOG("ql_sms_send_msg FAIL");
	}
    


	//Send messages in Chinese and English. (Need use UTF8 encoding to open sms_demo.c for chinese.)
	if(QL_SMS_SUCCESS == ql_sms_send_msg(nSim,"+8610086","hello,你好", UCS2)){
		QL_SMS_LOG("ql_sms_send_msg OK");
	}else{
		QL_SMS_LOG("ql_sms_send_msg FAIL");
	}
#endif
	
	//Get how many SMS messages can be stored in the SIM card in total and how much storage is used
#if 1
	ql_sms_stor_info_s stor_info;
	if(QL_SMS_SUCCESS == ql_sms_get_storage_info(nSim,&stor_info)){
		QL_SMS_LOG("ql_sms_get_storage_info OK");
		QL_SMS_LOG("SM used=%u,SM total=%u,SM unread=%u,ME used=%u,ME total=%u,ME unread=%u, newSmsStorId=%u",
			stor_info.usedSlotSM,stor_info.totalSlotSM,stor_info.unReadRecordsSM,
			stor_info.usedSlotME,stor_info.totalSlotME,stor_info.unReadRecordsME,
			stor_info.newSmsStorId);
	}else{
		QL_SMS_LOG("ql_sms_get_storage_info FAIL");
	}
#endif

    //The first parameter specifies that SMS messages are read from SM
    ql_sms_set_storage(nSim,SM,SM,SM);

	//Read one messages in SIM
#if 1
	uint16_t msg_len = 512;
	ql_sms_mem_info_t sms_mem = {0};
	ql_sms_recv_s *sms_recv = NULL;
	
	char *msg = malloc(msg_len);
	if(msg == NULL){
		QL_SMS_LOG("malloc ql_sms_msg_s fail");
		goto exit;
	}
	memset(msg ,0 ,msg_len);

	ql_sms_get_storage(nSim, &sms_mem);
	QL_SMS_LOG("mem1=%d, mem2=%d, mem3=%d", sms_mem.mem1, sms_mem.mem2, sms_mem.mem3);
    
	//Read SMS messages as text
	if(QL_SMS_SUCCESS == ql_sms_read_msg(nSim,2, msg, msg_len, TEXT)){
//		QL_SMS_LOG("read msg OK, msg=%s", msg);
	}else{
		QL_SMS_LOG("read sms FAIL");
	}
	
	//Read SMS messages as pdu
	memset(msg ,0 ,msg_len);
	if(QL_SMS_SUCCESS == ql_sms_read_msg(nSim,2, msg, msg_len, PDU)){
//		QL_SMS_LOG("read msg OK, msg=%s", msg);
	}else{
		QL_SMS_LOG("read sms FAIL");
	}
    if(msg)free(msg);

    //Read SMS messages as text
    sms_recv = (ql_sms_recv_s *)calloc(1,sizeof(ql_sms_recv_s));
    if(sms_recv == NULL)
    {
        QL_SMS_LOG("calloc FAIL");
        goto exit;
    }
	if(QL_SMS_SUCCESS == ql_sms_read_msg_ex(nSim,2, TEXT,sms_recv)){
        QL_SMS_LOG("index=%d,os=%s,tooa=%u,status=%d,fo=0x%x,dcs=0x%x,scst=%d/%d/%d %d:%d:%d±%d,uid=%u,total=%u,seg=%u,dataLen=%d,data=%s",
            sms_recv->index,sms_recv->oa,sms_recv->tooa,sms_recv->status,sms_recv->fo,sms_recv->dcs,
            sms_recv->scts.uYear,sms_recv->scts.uMonth,sms_recv->scts.uDay,sms_recv->scts.uHour,sms_recv->scts.uMinute,sms_recv->scts.uSecond,sms_recv->scts.iZone,
            sms_recv->uid,sms_recv->msg_total,sms_recv->msg_seg,sms_recv->dataLen,sms_recv->data);

	}else{
		QL_SMS_LOG("read sms FAIL");
	}
	if(sms_recv)free(sms_recv);
#endif

	//Read all message in SIM
#if 1
	ql_sms_set_storage(nSim,SM,SM,SM);//set sms storage as SIM.
	if(QL_SMS_SUCCESS == ql_sms_read_msg_list(nSim, TEXT)){
		if(ql_rtos_semaphore_wait(sms_list_sem, QL_WAIT_FOREVER)){
			QL_SMS_LOG("sms_list_sem time out");
		}
	}else{
		QL_SMS_LOG("get msg list FAIL");
	}
#endif
	
	//Delete message.
#if 0
	if(QL_SMS_SUCCESS == ql_sms_delete_msg_ex(nSim, 0, QL_SMS_DEL_ALL)){
		QL_SMS_LOG("delete msg OK");
	}else{
		QL_SMS_LOG("delete sms FAIL");
	}
#endif
	goto exit;
exit:
    if(sms_init_sem){
        ql_rtos_semaphore_delete(sms_init_sem);
    }
    if(sms_list_sem){
        ql_rtos_semaphore_delete(sms_list_sem);
    }
	ql_rtos_task_delete(NULL);
}


QlOSStatus ql_sms_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&sms_task, 4096, APP_PRIORITY_NORMAL, "QsmsApp", sms_demo_task, NULL, 2);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SMS_LOG("sms_task created failed, ret = 0x%x", err);
	}

	return err;
}

