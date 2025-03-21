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

#ifndef QL_API_VOICE_CALL_H
#define QL_API_VOICE_CALL_H


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

#define QL_VC_LOG(msg, ...)	 QL_LOG(QL_LOG_LEVEL_INFO, "ql_vc", msg, ##__VA_ARGS__)

#define  QL_VC_MAX_NUM            7
 /*========================================================================
  *  Type Definition
  *========================================================================*/
typedef enum
{
    QL_VC_REASON_NOANSWER = 1,
    QL_VC_REASON_NOCARRIER,
    QL_VC_REASON_BUSY,
    QL_VC_REASON_PWROFF,
    QL_VC_REASON_ERROR,
    QL_VC_REASON_USER_END
}ql_vc_release_reason_e;

typedef enum
{
	QL_VC_SUCCESS					= 0,
	QL_VC_ERROR					    = 1 | (QL_COMPONENT_VOICE_CALL << 16),
	QL_VC_NOT_INIT_ERR,
	QL_VC_PARA_ERR,
	QL_VC_NO_MEMORY_ERR,
	QL_VC_NO_CALL_ERR,
	QL_VC_SEM_CREATE_ERR,
	QL_VC_SEM_TIMEOUT_ERR,    
}ql_vc_errcode_e;

typedef enum
{
	QL_VC_INIT_OK_IND =    1| (QL_COMPONENT_VOICE_CALL << 16),
	QL_VC_RING_IND,        //MT call,       ctx is ql_vc_info_s
	QL_VC_CONNECT_IND,     //MO & MT call,  ctx is ql_vc_info_s
	QL_VC_NOCARRIER_IND,   //MO & MT call,  ctx is ql_vc_release_reason_e
	QL_VC_ERROR_IND,       //MT call,       ctx is null
	QL_VC_CCWA_IND,        //MT call,       ctx is ql_vc_info_s
    QL_VC_DIAL_IND,        //MO call,       ctx is ql_vc_info_s
    QL_VC_ALERT_IND,       //MO call,       ctx is ql_vc_info_s
    QL_VC_HOLD_IND,        //MO & MT call,  ctx is ql_vc_info_s
    QL_VC_DISCONNECT_IND,  //MO & MT call,  ctx is ql_vc_info_s
    QL_VC_AUDIO_IND        //MO & MT call,  ctx is ql_vc_info_s
}ql_vc_event_id_e;



 /*****************************************************************
 * Parameters:
 *	 idx: call identification number 
 *	 direction: direction of the call
 *			0-mobile originated (MO) call 
 *			1-mobile terminated (MT) call
 *   status:(status of the call)
 *			0 active
 *			1 held
 *			2 dialing (MO call)
 *			3 alerting (MO call)
 *			4 incoming (MT call)
 *			5 waiting (MT call)
 *			7 release (network release this call)
 *			8 handshake
 *   multiparty: 
 *			0 call is not one of multiparty (conference) call parties
 *			1 call is one of multiparty (conference) call parties
 *	 number:
 *
 *****************************************************************/
 typedef struct
 {
    char number[23];
 	uint8_t idx;
 	uint8_t direction;
	uint8_t status;
	uint8_t multiparty;
 }ql_vc_info_s;

 /*========================================================================
  *  Callback Definition
  *========================================================================*/
 typedef void (*ql_vc_event_handler_t)(uint8_t sim, ql_vc_event_id_e event_id, void *ctx);
 extern ql_vc_event_handler_t ql_voice_call_callback;


 /*========================================================================
  *  function Definition
  *========================================================================*/
 /*****************************************************************
 * Function: ql_voice_auto_answer
 *
 * Description:
 *	 设置自动接听
 * 
 * Parameters:
 *   nSim		[in] 	SIM卡索引，取值0-1。
 *	 times	   [in]  设置响铃多少声后自动接听电话,0表示关闭自动接听功能。 
 *
 * Return:ql_vc_errcode_e
 *
 *****************************************************************/
ql_vc_errcode_e ql_voice_auto_answer(uint8_t nSim, uint8_t times);

/*****************************************************************
* Function: ql_voice_call_start
*
* Description:
*	拨打电话
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*	times	  [dial_num] 字符串格式手机号码。 
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_start(uint8_t nSim, char* dial_num);

/*****************************************************************
* Function: ql_voice_call_answer
*
* Description:
*	接听电话
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_answer(uint8_t nSim);

/*****************************************************************
* Function: ql_voice_call_end
*
* Description:
*	挂断电话
* 
* Parameters:
*   nSim		[in] 	SIM卡索引，取值0-1。
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_end(uint8_t nSim);

/*****************************************************************
* Function: ql_voice_call_start_dtmf
*
* Description:
*	发送DTMF
* 
* Parameters:
*   nSim	  [in] 	SIM卡索引，取值0-1。
*	dtmf	  [in] 包含"0-9,#,*,A-D。"字符的字符串
*   duration  [in] 持续时间,取值0-10, 0表示使用默认值,1表示100ms
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_start_dtmf(uint8_t nSim, char *dtmf, uint16_t duration);

/*****************************************************************
* Function: ql_voice_call_wait_set
*
* Description:
*	设置呼叫等待
* 
* Parameters:
*   nSim	  [in] 	SIM卡索引，取值0-1。
*	mode	  [in] 0表示关闭呼叫等待,1表示打开呼叫等待
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_wait_set(uint8_t nSim, uint8_t mode);

/*****************************************************************
* Function: ql_voice_call_wait_get
*
* Description:
*	获取呼叫等待状态
* 
* Parameters:
*   nSim	  [in] 	SIM卡索引，取值0-1。
*	mode	  [out] 0表示呼叫等待已关闭,1表示呼叫等待已打开
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_wait_get(uint8_t nSim, uint8_t *mode);

/*****************************************************************
* Function: ql_voice_call_fw
*
* Description:
*	设置呼叫转移
* 
* Parameters:
*   nSim	  [in] 	SIM卡索引，取值0-1。
*	reason	  [in] 对应AT+CCFC指令的reason
*  fwmode     [in] 对应AT+CCFC指令的mode
*  phone_num  [in] 对应AT+CCFC指令的number
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_fw(uint8_t nSim, int reason, int fwmode, char* phone_num);


/*****************************************************************
* Function: ql_voice_call_hold
*
* Description:
*	呼叫保持
* 
* Parameters:
*   nSim	  [in] 	SIM卡索引，取值0-1。
*	n	  [in] 对应AT+HOLD指令的n
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_hold(uint8_t nSim, uint8_t n);


/*****************************************************************
* Function: ql_voice_call_clcc
*
* Description:
*	获取当前电话列表
* 
* Parameters:
*   nSim	      [in] 	SIM卡索引，取值0-1。
*	total	      [out] 当前电话总数
*   ql_vc_info_s  [out] 结构体数组指针，每个电话的序号、状态、号码等信息.
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
ql_vc_errcode_e ql_voice_call_clcc(uint8_t nSim, uint8_t *total, ql_vc_info_s vc_info[QL_VC_MAX_NUM]);


/*****************************************************************
* Function: ql_voice_call_callback_register
*
* Description:
*	注册回调函数
* 
* Parameters:
*	ql_vc_event_handler_t	  [in] 回调函数
*
* Return:ql_vc_errcode_e
*
*****************************************************************/
void ql_voice_call_callback_register(ql_vc_event_handler_t cb);


#ifdef __cplusplus
 }/*"C" */
#endif


#endif

