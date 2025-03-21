/**  
  @file
  ql_sms.h

  @brief
  This file provides the definitions for sms API functions.

*/
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------
            marvin          create

=============================================================================*/

#ifndef QL_API_SMS_H
#define QL_API_SMS_H


#include <stdint.h>
#include <stdbool.h>
#include "sockets.h"
#include "ql_api_common.h"
#include "ql_api_osi.h"


#ifdef __cplusplus
 extern "C" {
#endif



 /*========================================================================
  *  Marco Definition
  *========================================================================*/

#define QL_SMS_LOG_LEVEL		QL_LOG_LEVEL_INFO
#define QL_SMS_LOG(msg, ...)	QL_LOG_TAG(QL_LOG_LEVEL_INFO, QL_LOG_TAG_SMS, "ql_sms", msg, ##__VA_ARGS__)       

#define QL_SMS_MIN_LEN                1       
#define MAX_LONGSMS_SEGMENT           8
#define MAX_SMS_LENGTH                (160*4)
#define SMS_BUF_LEN                   (MAX_SMS_LENGTH + 32)
#define MAX_LONGSMS_LENGTH            (160*4*MAX_LONGSMS_SEGMENT)
#define QL_TEL_MAX_LEN                21         
#define QL_SMS_PDU_MAX_LEN            176
 


 /*========================================================================
  *  Enumeration Definition
  *========================================================================*/

 typedef enum
 {
	 QL_SMS_SUCCESS					 = 0,
	 QL_SMS_ERROR		             = 1 | (QL_COMPONENT_SMS << 16),
	 QL_SMS_NOT_INIT_ERR,
	 QL_SMS_PARA_ERR,
	 QL_SMS_NO_MEMORY_ERR,
	 QL_SMS_SEM_CREATE_ERR,
	 QL_SMS_SEM_TIMEOUT_ERR,    
	 QL_SMS_NO_MSG_ERR,
 }ql_sms_errcode_e;


typedef enum
{
	QL_SMS_INIT_OK_IND = 1| (QL_COMPONENT_SMS << 16),
	QL_SMS_NEW_MSG_IND,
	QL_SMS_LIST_IND,
	QL_SMS_LIST_END_IND,
	QL_SMS_LIST_EX_IND,
	QL_SMS_MEM_FULL_IND,
	QL_SMS_REPORT_IND,
}ql_sms_event_id_e;


typedef enum
{
	ME = 1, //Mobile Equipment message storage
	SM = 2,//SIM message storage
}ql_sms_stor_e;

typedef enum
{
	GSM = 0,
	UCS2 = 1,
    DATA_8BIT,
}ql_sms_code_e;

typedef enum
{
	PDU = 0,
	TEXT = 1,
}ql_sms_format_e;

typedef enum
{
	QL_SMS_UNREAD = 0,
	QL_SMS_READ = 1,
	QL_SMS_UNSENT = 2,
	QL_SMS_SENT = 3,
	QL_SMS_ALL,
}ql_sms_status_e;


typedef enum {
    QL_SMS_DEL_INDEX = 0,         /* Delete the message specified in <index> */
    QL_SMS_DEL_RECV_READ,         /* Delete all read messages from <mem1> storage */
    QL_SMS_DEL_RECV_READ_SENT,    /* Delete all read messages from <mem1> storage and sent mobile originated messages */
    QL_SMS_DEL_RECV_READ_MO,      /* Delete all read messages from <mem1> storage, sent and unsent mobile originated messages */
    QL_SMS_DEL_ALL,               /* Delete all messages from <mem1> storage */

    QL_SMS_DEL_MODES_NUMS
}QL_SMS_DELETE_MODE;

typedef enum
{
    QL_CS_GSM,
    QL_CS_IRA,
    QL_CS_UCS2,
    QL_CS_ALL,
} ql_sms_cscs_e;

 /*========================================================================
 *	Struct Definition
 *========================================================================*/
typedef struct{
    uint8_t status;
    uint8_t index;
    char* buf;
    uint16_t buf_len;
}ql_sms_msg_s;
 
typedef struct{
    uint8_t index;
    uint8_t mem;
}ql_sms_new_s;


typedef struct
{
    uint16_t usedSlotSM;
    uint16_t totalSlotSM;
    uint16_t unReadRecordsSM;
    uint16_t usedSlotME;
    uint16_t totalSlotME;
    uint16_t unReadRecordsME;
    ql_sms_stor_e  newSmsStorId;
}ql_sms_stor_info_s;

typedef struct
{
    ql_sms_stor_e mem1; //messages to be read and deleted from this memory storage.
    ql_sms_stor_e mem2; //messages will be written and sent to this memory storage.
    ql_sms_stor_e mem3; //received messages will be placed in this memory storage if routing to PC is not set.
}ql_sms_mem_info_t;

typedef struct
{
    uint16_t uYear;    //Only the last two digits
    uint8_t  uMonth;
    uint8_t  uDay;
    uint8_t  uHour;
    uint8_t  uMinute;
    uint8_t  uSecond;
    int8_t   iZone;
}ql_sms_time_stamp_s;
 

typedef struct
{
    uint8_t  index;                 //Index in storage
    char     oa[QL_TEL_MAX_LEN*4+1];//Originating Address
    uint8_t  tooa;                  //Type of originating Address
    ql_sms_time_stamp_s scts;       //Service Centre Time Stamp
    ql_sms_status_e     status;
    uint8_t  fo;                    //First octet of PDU 
    uint8_t  dcs;                   //Data Coding Scheme
    uint16_t uid;                   //Message identification of a concatenated message
    uint8_t  msg_seg;               //Sequence number of a concatenated message
    uint8_t  msg_total;             //The total number of the segments of one concatenated message.
    uint16_t dataLen;               //number of bytes
    uint8_t  data[QL_SMS_PDU_MAX_LEN*2+1];
}ql_sms_recv_s;


typedef struct
{
    bool srr;             //Status Report Request
    uint8_t vp;           //Validiy Period
    ql_sms_code_e code;
    uint8_t mr;           //[out] Message Reference
}ql_sms_cfg_s;
 

typedef struct
{
    uint8_t ra[TEL_NUMBER_MAX_LEN];
    uint8_t ra_size;
    uint8_t tora;
    uint8_t fo;
    uint8_t mr;
    uint8_t st;
    ql_sms_time_stamp_s scts;
    ql_sms_time_stamp_s dt;
} ql_sms_report_s;


 /*========================================================================
  *  Callback Definition
  *========================================================================*/

typedef void (*ql_sms_event_handler_t)(uint8_t sim_id, int event_id, void *ctx);
extern ql_sms_event_handler_t ql_sms_callback;


 /*========================================================================
  *  function Definition
  *========================================================================*/


/*****************************************************************
* Function: ql_sms_send_msg
*
* Description:
* 	发送文本格式的短消息
* 
* Parameters:
*   nSim		  [in] 	SIM卡索引，取值0-1。
* 	phone_num	  [in] 	目的手机号码。 
*	data		  [in] 	短消息数据。 
*   code          [in]  短消息发送时采用的格式,可取值GSM或者UCS2
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_send_msg(uint8_t nSim, char *phone_num, char *data, ql_sms_code_e code);

/*****************************************************************
* Function: ql_sms_send_msg_ex
*
* Description:
* 	发送文本格式的短消息，可配置短信报告请求
* 
* Parameters:
*   nSim		  [in] 	SIM卡索引，取值0-1。
*   phone_num	  [in] 	目的手机号码。 
*   data		  [in] 	短消息数据。 
*   cfg           [in/out]  短消息发送时的配置，其中mr为[out]参数
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_send_msg_ex(uint8_t nSim, char *phone_num, char *data, ql_sms_cfg_s *cfg);

/*****************************************************************
* Function: ql_sms_send_pdu
*
* Description:
* 	发送PDU格式的短消息
* 
* Parameters:
*   nSim		 [in] 	SIM卡索引，取值0-1。
*	pdu			 [in] 	短消息数据。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_send_pdu(uint8_t nSim, char *pdu);

/*****************************************************************
* Function: ql_sms_get_msg_list
*
* Description:
* 	获取短消息列表
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*   format      [in]    PDU或者TEXT格式
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_read_msg_list(uint8_t nSim, ql_sms_format_e format);

/*****************************************************************
* Function: ql_sms_read_msg
*
* Description:
* 	文本方式读取单条短消息
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	index		[in] 	短信索引号。
*   buf         [out]   用于接收短信内容，其基本组成形式为"<oa>,<scts>,<data>"。
*   buf_len     [out]   数组长度。
*   format      [in]    短信格式。文本或者PDU格式。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_read_msg(uint8_t nSim, uint8_t index, char *buf, uint16_t buf_len, ql_sms_format_e format);

/*****************************************************************
* Function: ql_sms_read_msg_ex
*
* Description:
* 	读取单条短消息
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	index		[in] 	短信索引号。
*   format      [in]    短信格式。文本或者PDU格式。
*   msg         [out]   参考ql_sms_recv_s结构体。
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_read_msg_ex(uint8_t nSim, uint8_t index, ql_sms_format_e format, ql_sms_recv_s *msg);

/*****************************************************************
* Function: ql_sms_delete_msg
*
* Description:
* 	删除单条消息
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	index		[in] 	短消息序列号。
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_delete_msg(uint8_t nSim, uint8_t index);

/*****************************************************************
* Function: ql_sms_delete_msg_ex
*
* Description:
* 	删除单条消息
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	index		[in] 	短消息序列号。
*   del_mode    [in]    QL_SMS_DELETE_MODE类型数据.
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_delete_msg_ex(uint8_t nSim, uint8_t index, QL_SMS_DELETE_MODE del_mode);


/*****************************************************************
* Function: ql_sms_get_center_address
*
* Description:
* 	获取短消息中心号码
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	address		[out] 	短消息中心号码。
*   len         [in]    接收短消息中心号码的数组长度.
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_center_address(uint8_t nSim, char* address, uint8_t len);

/*****************************************************************
* Function: ql_sms_set_center_address
*
* Description:
* 	设置短消息中心号码
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	address		[in] 	短消息中心号码。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_set_center_address(uint8_t nSim, char* address);

/*****************************************************************
* Function: ql_sms_get_storage_info
*
* Description:
* 	获取SM与ME的存储信息
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	stor_info	[out] 	用于保存短信存储信息的结构体。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_storage_info(uint8_t nSim, ql_sms_stor_info_s *stor_info);

/*****************************************************************
* Function: ql_sms_set_storage
*
* Description:
* 	设置短信存储位置
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	mem1		[in] 	读取和删除消息所在内存空间。
*	mem2		[in] 	写入和发送消息的内存空间。
*	mem3		[in] 	接收消息的存储位置。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_set_storage(uint8_t nSim, ql_sms_stor_e mem1,ql_sms_stor_e mem2, ql_sms_stor_e mem3);


/*****************************************************************
* Function: ql_sms_get_storage
*
* Description:
* 	获取短信存储位置
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	mem_info    [in] 	见ql_sms_mem_info_t结构体。
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_storage(uint8_t nSim, ql_sms_mem_info_t *mem_info);


/*****************************************************************
* Function: ql_sms_set_code_mode
*
* Description:
* 	设置字符编码
* 
* Parameters:
*   cscs		 [in] 	字符编码,参考ql_sms_cscs_e联合体。
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_set_code_mode(ql_sms_cscs_e cscs);


/*****************************************************************
* Function: ql_sms_get_code_mode
*
* Description:
* 	获取字符编码
* 
* Parameters:
*   cscs		[out] 	返回当前字符编码。
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_code_mode(ql_sms_cscs_e* cscs);

/*****************************************************************
* Function: ql_sms_set_retry_count
*
* Description:
* 	用于设置smc层与smr层短消息发送失败后的重试次数
* 
* Parameters:
*   nSim		  [in] 	SIM卡索引，取值0-1。
* 	smc_count	  [in] 	smc层重试次数,取值0-2。 
*	smr_count	  [in] 	smr层重试次数,取值0-2。 
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_set_retry_count(uint8_t nSim, uint8_t smc_count, uint8_t smr_count);

/*****************************************************************
* Function: ql_sms_get_ignore_cscs
*
* Description:
* 	用于获取在读取短信时, 当短信的编码为8bit或者16bit时,是否忽略CSCS值。
* 
* Parameters:
*   nSim		  [in] 	SIM卡索引，取值0-1。
* 	enable	      [out] 是否忽略cscs值。 
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_ignore_cscs(uint8_t nSim, bool *enable);

/*****************************************************************
* Function: ql_sms_set_ignore_cscs
*
* Description:
* 	用于设置在读取短信时, 当短信的编码为8bit或者16bit时,是否忽略CSCS值。
* 
* Parameters:
*   nSim		  [in] 	SIM卡索引，取值0-1。
* 	enable	      [in] 	是否忽略cscs值。 
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_set_ignore_cscs(uint8_t nSim, bool enable);

/*****************************************************************
* Function: ql_sms_get_init_status
*
* Description:
* 	获取短信模块初始化状态
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	status		[out] 	短信初始化状态
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
ql_sms_errcode_e ql_sms_get_init_status(uint8_t nSim, uint8_t *status);

/*****************************************************************
* Function: ql_sms_callback_register
*
* Description:
* 	注册短信回调函数
* 
* Parameters:
*	cb		   [in] 	回调函数
*
* Return:ql_sms_errcode_e
*
*****************************************************************/
void ql_sms_callback_register(ql_sms_event_handler_t cb);




#ifdef __cplusplus
 }/*"C" */
#endif

#endif /*QL_API_SMS_H*/



