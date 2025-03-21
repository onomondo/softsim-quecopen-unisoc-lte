/**  @file
  ql_cnktd_sms.h

  @brief
  This file is used to define cnktd sms for different Quectel Project.

*/

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


#ifndef QL_CNKTD_SMS_H
#define QL_CNKTD_SMS_H

#include "ql_cnktd.h"


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
typedef struct {
//uint8 fo; //no need to set this parameter
uint8 vp;
uint8 pid;
uint8 dcs;
} qapi_QT_SMS_Para_t;

typedef enum {
  ALPHA_IRA = 0,        /* International Reference Alphabet T.50 */
  ALPHA_GSM,            /* GSM 7 bit alphabet, not packed to 7 bits
                          (will not contain @ (0x00); might have got mapped to 
                           0xe6 at the ATCOP parser ) */
  ALPHA_UCS2,           /* UCS2 Unicode, rep'd by "4 hex character"-tuplets */
  ALPHA_8BIT,           /* Octets, of 0-255 value */
  ALPHA_MAX
} qapi_QT_Chset_Type_e;

typedef enum {
QT_WMS_MEMORY_STORE_ME=0,
QT_WMS_MEMORY_STORE_MT=1,
QT_WMS_MEMORY_STORE_SM=2,
QT_WMS_MEMORY_STORE_SR=3,
QT_WMS_MEMORY_STORE_MAX
} qapi_QT_SMS_Mem_e;

typedef struct {
qapi_QT_SMS_Mem_e mem1;
qapi_QT_SMS_Mem_e mem2;
qapi_QT_SMS_Mem_e mem3;
} qapi_QT_SMS_CPMS_Set_t;


typedef struct {
char        address[20];
char        message[256];
} qapi_QT_SMS_Message_Content_t;

typedef struct {
uint8        used_num;
uint8        max_num;
} qapi_QT_SMS_Message_Store_t;

typedef struct {
  uint8_t number;
  uint8_t type;    /* 0x00-WMS_TAG_NONE; 
                      0x01-WMS_TAG_MT_READ;    0x03-WMS_TAG_MT_NOT_READ;     0x05-WMS_TAG_MO_SENT;     0x07-WMS_TAG_MO_NOT_SENT;
                      0x0D-WMS_TAG_MO_SENT_ST_NOT_RECEIVED;     0x15-WMS_TAG_MO_SENT_ST_NOT_STORED;     0x1D-WMS_TAG_MO_SENT_ST_STORED*/
}qapi_QT_SMS_Message_Index_t;

typedef struct {
  uint8_t sum;
  qapi_QT_SMS_Message_Index_t indices[100];
}qapi_QT_SMS_Message_List_t;

/** @addtogroup wms_qmi_enums
    @{
  */
typedef enum {
  WMS_MESSAGE_TAG_TYPE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  WMS_TAG_TYPE_MT_READ_V01 = 0x00, 
  WMS_TAG_TYPE_MT_NOT_READ_V01 = 0x01, 
  WMS_TAG_TYPE_MO_SENT_V01 = 0x02, 
  WMS_TAG_TYPE_MO_NOT_SENT_V01 = 0x03, 
  WMS_MESSAGE_TAG_TYPE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}wms_message_tag_type_enum_v01;

typedef wms_message_tag_type_enum_v01 qapi_QT_SMS_Status_e;

#define QUECTEL_SMS_TEXT_LENGTH_MAX_V01 281
#define WMS_ADDRESS_DIGIT_MAX_V01 21

typedef struct{
  uint32_t data_len;
  uint8_t data[QUECTEL_SMS_TEXT_LENGTH_MAX_V01];
  uint8_t sender_num[WMS_ADDRESS_DIGIT_MAX_V01];  
}qapi_QT_SMS_Message_Info_t;

typedef struct{
//time_t time;
qapi_QT_SMS_Status_e status;
qapi_QT_SMS_Message_Info_t sms_info;
}qapi_QT_SMS_Message_Rcvd_t;

/*========================================================================
 *  function Definition
 *========================================================================*/

/*
@func
  qapi_QT_SMS_Para_Set
@brief
  To set the vp/pid/dcs of SMS
@param [in]
  sms_para - The vp/pid/dcs of SMS paramater
*/
qapi_Status_t qapi_QT_SMS_Para_Set(qapi_QT_SMS_Para_t* sms_para);

/*
@func
  qapi_QT_SMS_Chset_Set
@brief
  To set the charset of SMS
@param [in]
  cscs_val - The charset of SMS
*/
qapi_Status_t qapi_QT_SMS_Chset_Set(qapi_QT_Chset_Type_e cscs_val);

/*
@func
  qapi_QT_SMS_CPMS_Set
@brief
  To set the storage of SMS
@param [in]
  sms_cpms - the SMS storage to be set
*/
qapi_Status_t qapi_QT_SMS_CPMS_Set(qapi_QT_SMS_CPMS_Set_t* sms_cpms);

/*
@func
qapi_QT_SMS_Message_Send
@brief
To send a short message
@param [in]
send_message - The address and content of SMS to be sent
*/
qapi_Status_t qapi_QT_SMS_Message_Send(qapi_QT_SMS_Message_Content_t* send_message);

/*
@func
  qapi_QT_SMS_Rcvd_Num
@brief
  To get number of received and total SMS
@param [out]
  msg_store_num - The received and totalSMS number
*/
qapi_Status_t qapi_QT_SMS_Rcvd_Num(qapi_QT_SMS_Message_Store_t* msg_store_num);

/*
@func
 qapi_QT_SMS_Message_List
@brief
 To list all the SMS messages
@param [out]
 para - All the SMS messages
*/
qapi_Status_t qapi_QT_SMS_Message_List(qapi_QT_SMS_Message_List_t* para);

/*
@func
  qapi_QT_SMS_Message_Read
@brief
  To read the content of specific SMS
@param [in]
  index - index of the SMS to be read
@param [out]
  msg_info - The content of the specific SMS tobe read
*/
qapi_Status_t qapi_QT_SMS_Message_Read(uint16 index, qapi_QT_SMS_Message_Rcvd_t* msg_info);

/*
@func
  qapi_QT_SMS_Message_Delete
@brief
  To delete the apecific SMS
@param [in]
  index - The index of SMS to be delete
*/
qapi_Status_t qapi_QT_SMS_Message_Delete(uint16 index);

/*
@func
  qapi_QT_SMS_Get_Service_Ready_Status
@brief
  To get the state of SMS service, the state must be ready before operate the SMS
@param [out]
  service_state - The service state of SMS
*/
qapi_Status_t qapi_QT_SMS_Get_Service_Ready_Status(uint16* service_state);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_SMS_H */


