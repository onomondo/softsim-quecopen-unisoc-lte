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
#include "ql_api_sms.h"
#include "ql_cnktd_sms.h"

#include "ql_log.h"

#define QL_CNKTD_SMS_LOG(msg, ...)	QL_LOG_TAG(QL_LOG_LEVEL_INFO, QL_LOG_TAG_OPEN, "ql_cnktd_sms", msg, ##__VA_ARGS__)       

#define QL_SIM_ID_0 0

static qapi_QT_SMS_Message_List_t *msg_list_ptr = NULL;
static qapi_QT_SMS_Para_t qapi_sms_para = {
    .vp = 167,
    .pid = 0,
    .dcs = 0
};

qapi_Status_t qapi_QT_SMS_Para_Set(qapi_QT_SMS_Para_t* sms_para)
{
    qapi_Status_t status = QAPI_OK;
	
	if(sms_para== NULL)
	{
		return QAPI_ERR_INVALID_PARAM;
	}

    if(sms_para->pid != 0){
        return QAPI_ERR_INVALID_PARAM;
    }
    //support class0 GSM_7BIT, 8BIT_DATA, UCS2
    if(sms_para->dcs != 0 && sms_para->dcs != 4 && sms_para->dcs != 8){
        return QAPI_ERR_INVALID_PARAM;
    }

    qapi_sms_para.vp = sms_para->vp;
    qapi_sms_para.pid = sms_para->pid;
    qapi_sms_para.dcs = sms_para->dcs;

	return status;
}

qapi_Status_t qapi_QT_SMS_Chset_Set(qapi_QT_Chset_Type_e cscs_val)
{
    qapi_Status_t status = QAPI_OK;
	
    if(cscs_val >= ALPHA_MAX){
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_sms_cscs_e cscs;
    if(cscs_val == ALPHA_IRA){
        cscs = QL_CS_IRA;
    }else if(cscs_val == ALPHA_UCS2){
        cscs = QL_CS_UCS2;
    }else if(cscs_val){
        cscs = QL_CS_GSM;
    }

    if(ql_sms_set_code_mode(cscs) != 0){
        return QAPI_ERR_INVALID_PARAM;
    }

	return status;
}

qapi_Status_t qapi_QT_SMS_CPMS_Set(qapi_QT_SMS_CPMS_Set_t* sms_cpms)
{
    qapi_Status_t status = QAPI_OK;
    
    if(sms_cpms == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    ql_sms_stor_e mem1,mem2,mem3;

    /*just support ME & SM*/
    if(sms_cpms->mem1 == QT_WMS_MEMORY_STORE_ME){
        mem1 = ME;
    }else if(sms_cpms->mem1 == QT_WMS_MEMORY_STORE_SM){
        mem1 = SM;
    }else{
        return QAPI_ERR_INVALID_PARAM;
    }

    if(sms_cpms->mem2 == QT_WMS_MEMORY_STORE_ME){
        mem2 = ME;
    }else if(sms_cpms->mem2 == QT_WMS_MEMORY_STORE_SM){
        mem2 = SM;
    }else{
        return QAPI_ERR_INVALID_PARAM;
    }

    if(sms_cpms->mem3 == QT_WMS_MEMORY_STORE_ME){
        mem3 = ME;
    }else if(sms_cpms->mem3 == QT_WMS_MEMORY_STORE_SM){
        mem3 = SM;
    }else{
        return QAPI_ERR_INVALID_PARAM;
    }
    
    if(ql_sms_set_storage(QL_SIM_ID_0, mem1, mem2, mem3) != 0)
    {
        status = QAPI_ERR_INVALID_PARAM;
    }
    return status;
}

 qapi_Status_t qapi_QT_SMS_Message_Send(qapi_QT_SMS_Message_Content_t* send_message)
{
    qapi_Status_t status = QAPI_OK;

    if(send_message == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    ql_sms_cfg_s ql_sms_cfg = {0};
    ql_sms_cfg.vp  = qapi_sms_para.vp;
    ql_sms_cfg.code = qapi_sms_para.dcs;

    if(ql_sms_send_msg_ex(QL_SIM_ID_0, send_message->address, send_message->message, &ql_sms_cfg) != 0)
    {
        status = QAPI_ERROR;
    }
    return status;
}

qapi_Status_t qapi_QT_SMS_Rcvd_Num(qapi_QT_SMS_Message_Store_t* msg_store_num)
{
    if(msg_store_num == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_sms_mem_info_t  mem_info = {0};
    if(ql_sms_get_storage(QL_SIM_ID_0, &mem_info) != 0)
    {
        return QAPI_ERROR;
    }

    ql_sms_stor_info_s stor_info = {0};
    if(ql_sms_get_storage_info(QL_SIM_ID_0, &stor_info) != 0)
    {
        return QAPI_ERROR;
    }
    if(mem_info.mem3 == ME)
    {
        msg_store_num->max_num  = stor_info.totalSlotME;
        msg_store_num->used_num = stor_info.usedSlotME;
    }else//SM
    {
        msg_store_num->max_num = stor_info.totalSlotSM;
        msg_store_num->used_num = stor_info.usedSlotSM;
    }

    return QAPI_OK;  
}

void qapi_sms_event_callback(uint8_t nSim, int event_id, void *ctx)
{
    if(msg_list_ptr == NULL){
        return;
    }

    if(event_id == QL_SMS_LIST_EX_IND)
    {
        ql_sms_recv_s *sms_list = (ql_sms_recv_s *)ctx;

        msg_list_ptr->indices[msg_list_ptr->sum].number = sms_list->index;

        //status is a member of ql_sms_status_e
        QL_CNKTD_SMS_LOG("number=%d,status=%d", sms_list->index, sms_list->status);
        switch (sms_list->status)
        {
        case QL_SMS_UNREAD:
            msg_list_ptr->indices[msg_list_ptr->sum].type = 0x03;//WMS_TAG_MT_NOT_READ
            break;
        case QL_SMS_READ:
            msg_list_ptr->indices[msg_list_ptr->sum].type = 0x01;//WMS_TAG_MT_READ
            break;
        case QL_SMS_UNSENT:
            msg_list_ptr->indices[msg_list_ptr->sum].type = 0x07;//WMS_TAG_MO_NOT_SENT
            break;
        case QL_SMS_SENT:
            msg_list_ptr->indices[msg_list_ptr->sum].type = 0x05;//WMS_TAG_MO_SENT
            break;
        default:
            break;
        }
        msg_list_ptr->sum++;
    }
}

qapi_Status_t qapi_QT_SMS_Message_List(qapi_QT_SMS_Message_List_t* para)
{
	qapi_Status_t q_status = QAPI_OK;
    
	if(para == NULL)
	{
		return QAPI_ERR_INVALID_PARAM;
	}
    msg_list_ptr = para;

    ql_sms_callback_register(qapi_sms_event_callback);
    
    if(ql_sms_read_msg_list(QL_SIM_ID_0, TEXT) != 0)
    {
        q_status = QAPI_ERROR;
    }

    return q_status;
}

/**************************************************************************************************
            SMS-DELIVER PDU 
8bit    1-12 1  2-12  1   1   7    1  0-140
element  SCA FO  OA  PID DCS SCTS UDL  UD

OA: Address length TOA OA/DA/RA

(info_temp[1]+1): the length of OA 
(info_temp[1]+1)+1+1: length of OA bit
                      Address (1) + length (1) +  (info_temp[1]+1)
info_temp[12 + (info_temp[1]+1)/2]:  UDL                    
***************************************************************************************************/
qapi_Status_t qapi_QT_SMS_Message_Read(uint16 index, qapi_QT_SMS_Message_Rcvd_t* msg_info)
{
    qapi_Status_t status = QAPI_OK;

    if(msg_info== NULL)
    {
    	return QAPI_ERR_INVALID_PARAM;
    }

    ql_sms_recv_s sms_msg = {0};

    if(ql_sms_read_msg_ex(QL_SIM_ID_0, index, TEXT, &sms_msg) != 0){
        return QAPI_ERROR;
    }

    msg_info->sms_info.data_len = sms_msg.dataLen;
    memcpy(msg_info->sms_info.data ,sms_msg.data, sms_msg.dataLen);
    strcpy((char *)msg_info->sms_info.sender_num, sms_msg.oa);
    switch (sms_msg.status)
    {
    case QL_SMS_UNREAD:
        msg_info->status = WMS_TAG_TYPE_MT_NOT_READ_V01;
        break;
    case QL_SMS_READ:
        msg_info->status = WMS_TAG_TYPE_MT_READ_V01;
        break;
    case QL_SMS_UNSENT:
        msg_info->status = WMS_TAG_TYPE_MO_NOT_SENT_V01;
        break;
    case QL_SMS_SENT:
        msg_info->status = WMS_TAG_TYPE_MO_SENT_V01;
        break;
    default:
        break;
    }
    return status;
}

qapi_Status_t qapi_QT_SMS_Message_Delete(uint16 index)
{
    /*index start with 0*/
    if(ql_sms_delete_msg(QL_SIM_ID_0, index) != 0){
        return QAPI_ERROR;
    }
	return QAPI_OK;
}

qapi_Status_t qapi_QT_SMS_Get_Service_Ready_Status(uint16* service_state)
{
    uint8_t status = 0;
    ql_sms_get_init_status(QL_SIM_ID_0, &status);

    *service_state = status;

    return QAPI_OK;
}





