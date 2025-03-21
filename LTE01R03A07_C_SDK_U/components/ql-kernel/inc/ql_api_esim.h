/**  
  @file
  ql_api_nw.h

  @brief
  This file provides the definitions for nw, and declares the 
  API functions.

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

=============================================================================*/

#ifndef QL_API_NW_H
#define QL_API_NW_H

#include <stdint.h>
#include <stdbool.h>
#include "ql_api_common.h"
#include "ql_api_datacall.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QL_ERRCODE_ESIM_BASE (QL_COMPONENT_ESIM_RESULT << 16)
#define QL_EVENT_ESIM_RSP_BASE (QL_COMPONENT_ESIM_RSP << 16)

#define  ESIM_LPA_PROFILE_LIST_MAX_NUM          10
#define  ESIM_LPA_DATA_MAX                      128
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
 typedef enum
{
    QL_ESIM_MODE_LPA = 0,
    QL_ESIM_MODE_IPA,
}ql_esim_mode_e;

 typedef enum
{
    QL_ESIM_NOTIFICATION_TYPE_ALL = 0,
    QL_ESIM_NOTIFICATION_TYPE_ENABLE,
    QL_ESIM_NOTIFICATION_TYPE_DISABLE,
    QL_ESIM_NOTIFICATION_TYPE_DELETE,
    QL_ESIM_NOTIFICATION_TYPE_INSTALL
}ql_esim_notify_type;
    
typedef enum
{
    QL_ESIM_LPA_INVALID_RSP           = 0,         
    QL_ESIM_LPA_PROFILE_RSP           =1 | QL_EVENT_ESIM_RSP_BASE,      
    QL_ESIM_LPA_PROFILE_LIST_RSP      ,
    QL_ESIM_LPA_NICKNAME_RSP          ,
    QL_ESIM_LPA_GET_EID_RSP           , 
    QL_ESIM_LPA_APN_OPT_RSP           ,
    QL_ESIM_LPA_NOTIFICATION_LIST_RSP ,
    QL_ESIM_LPA_NOTIFICATION_OPT_RSP  ,    
    QL_ESIM_LPA_DOWN_OPT_RSP          ,
    QL_ESIM_LPA_PROFILE_TRANSMIT_RSP  ,          
    QL_ESIM_LPA_PROFILE_INSTALL_RESULT_RSP,        
    QL_ESIM_LPA_VER_RSP               ,                
    QL_ESIM_SDK_COMPILE_TIME_RSP      ,
    QL_ESIM_IPA_VER_RSP               ,
    QL_ESIM_IPA_POLL_TIME_SET_RSP     ,
    QL_ESIM_IPA_EIM_LIST_INFO_RSP     ,               
    QL_ESIM_IPA_EIM_ADD_RESULT_RSP    , 
    QL_ESIM_IPA_EIM_OPT_RESULT_RSP    ,        
    QL_ESIM_IPA_TIMER_CONFIG_GET_RSP  ,
    QL_ESIM_IPA_FALLBACK_RESULT_RSP   ,
    QL_ESIM_IPA_FALLBACK_RETURN_RESULT_RSP,
} ql_esim_command_rsp_e;

typedef enum {
    QL_ESIM_GET_PROFILE_INFO = 0, 
    QL_ESIM_LIST_PROFILE_INFO    
}ql_esim_profile_list;

typedef enum {
    QL_ESIM_ENABLE_PROFILE = 0,
    QL_ESIM_DISABLE_PROFILE,
    QL_ESIM_DELETE_PROFILE,

    QL_ESIM_PROFILE_TAG_MAX_NUM
}ql_esim_profile_operate_e;
    
typedef enum
{
    QL_ESIM_LPA_OTA_DOWNLOAD_PROFILE = 0,
    QL_ESIM_LPA_UART_DOWNLOAD_PROFILE
}ql_esim_profile_down_type;



typedef enum
{
    QL_ESIM_OK = 0,
    QL_ESIM_MEMROY_ERR = 1 | QL_ERRCODE_ESIM_BASE, /*!< ESIM  SDK 内存错误 */
    QL_ESIM_INVALID_PARAM_ERR,                      /*!< ESIM 参数错误 */
    QL_ESIM_AC_CODE_ERR,                            /*!< ESIM AC code 错误 */
    QL_ESIM_OPERATION_TIMEOUT,                      /*!< 操作超时错误 */
    QL_ESIM_ERROR_GENERAL,                          /*!< ESIM SDK内部通用错误 */
    QL_ESIM_BUFF_OVERFLOW_ERR,                      /*!< 内存溢出错误 */
    QL_ESIM_OPERATION_ERR,                          /*!< 操作错误 */
    QL_ESIM_MSG_SENT_ERR,                           /*!< 消息队列发送错误 */
    QL_ESIM_APDU_SENT_ERR,                          /*!< APDU发送错误 */
    QL_ESIM_APDU_STATUS_ERR,                        /*!< APDU状态码错误 */
    QL_ESIM_APDU_PARSE_ERR,                         /*!< APDU解析错误 */
    QL_ESIM_TLV_PARSE_ERR,                          /*!< TLV解析错误 */
    QL_ESIM_JSON_PARSE_ERR,                         /*!< JSON解析错误 */
    QL_ESIM_FS_OPERATION_ERR,                       /*!< 文件系统操作错误 */
    QL_ESIM_HTTPS_OPERATION_ERR,                    /*!< https操作错误 */
    QL_ESIM_HTTPS_BUSY_ERR,                         /*!< https忙碌错误 */
    QL_ESIM_PROFILE_DOWNLOAD_BUSY_ERR,              /*!< profile 下载,任务忙碌错误 */
    QL_ESIM_PROFILE_DOWNLOAD_GET_EUICCINFO1_ERR,    /*!< profile 下载,获取euiccinfo1错误 */
    QL_ESIM_PROFILE_DOWNLOAD_SEND_HTTPS_ERR,        /*!< profile 下载,发送https消息错误 */
    QL_ESIM_PROFILE_DOWNLOAD_RESPONSE_HTTPS_ERR,    /*!< profile 下载,接收https响应消息错误 */
    QL_ESIM_PROFILE_DOWNLOAD_HTTPS_HEAD_ERR,        /*!< profile 下载  ,https head 错误 */
    QL_ESIM_PROFILE_DOWNLOAD_SMDP_RETURN_ERR,       /*!< profile 下载,SMDP返回错误 */
    QL_ESIM_PROFILE_DOWNLOAD_WAIT_DATA_TIMEOUT_ERR, /*!< profile 下载,等待数据超时错误 */

    QL_ESIM_ENABLE_PROFILE_ERROR_START                 = 0X100 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_ENABLE_ICCID_OR_AID_NOTFOUND               = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x01,
    QL_ESIM_ENABLE_PROFILE_NOT_IN_DISABLE_STATUS       = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x02,
    QL_ESIM_ENABLE_DISALLOWED_BY_POLICY                = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x03,
    QL_ESIM_ENABLE_WRONG_PROFILE_REENABLING            = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x04,
    QL_ESIM_ENABLE_CAT_BUSY                            = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x05,
    QL_ESIM_ENABLE_UNDEFINED_ERROR                     = QL_ESIM_ENABLE_PROFILE_ERROR_START | 0x7F,

    QL_ESIM_DISABLE_PROFILE_ERROR_START                = 0X200 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_DISABLE_ICCID_OR_AID_NOTFOUND              = QL_ESIM_DISABLE_PROFILE_ERROR_START | 0x01,
    QL_ESIM_DISABLE_PROFILE_NOT_IN_ENABLE_STATUS       = QL_ESIM_DISABLE_PROFILE_ERROR_START | 0x02,
    QL_ESIM_DISABLE_DISALLOWED_BY_POLICY               = QL_ESIM_DISABLE_PROFILE_ERROR_START | 0x03,
    QL_ESIM_DISABLE_CAT_BUSY                           = QL_ESIM_DISABLE_PROFILE_ERROR_START | 0x05,
    QL_ESIM_DISABLE_UNDEFINED_ERROR                    = QL_ESIM_DISABLE_PROFILE_ERROR_START | 0x7F,

    QL_ESIM_DELETE_PROFILE_ERROR_START                 = 0X300 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_DELETE_ICCID_OR_AID_NOTFOUND               = QL_ESIM_DELETE_PROFILE_ERROR_START | 0x01,
    QL_ESIM_DELETE_PROFILE_NOT_IN_ENABLE_STATUS        = QL_ESIM_DELETE_PROFILE_ERROR_START | 0x02,
    QL_ESIM_DELETE_DISALLOWED_BY_POLICY                = QL_ESIM_DELETE_PROFILE_ERROR_START | 0x03,
    QL_ESIM_DELETE_UNDEFINED_ERROR                     = QL_ESIM_DELETE_PROFILE_ERROR_START | 0x7F,

    QL_ESIM_LIST_PROFILE_ERROR_START                   = 0X400 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_LIST_INCORRECT_INPUT_VALUES                = QL_ESIM_LIST_PROFILE_ERROR_START | 0x01,
    QL_ESIM_LIST_UNDEFINED_ERROR                       = QL_ESIM_LIST_PROFILE_ERROR_START | 0x7F,

    QL_ESIM_NICKNAME_PROFILE_ERROR_START               = 0X500 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_NICKNAME_ICCID_NOT_FOUND                   = QL_ESIM_NICKNAME_PROFILE_ERROR_START | 0x01,
    QL_ESIM_NICKNAME_UNDEFINED_ERROR                   = QL_ESIM_NICKNAME_PROFILE_ERROR_START | 0x7F,

    QL_ESIM_DELETE_NOTIFICATION_ERROR_START            = 0X600 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_NOTIFICATION_NOTHING_TO_DELETE             = QL_ESIM_DELETE_NOTIFICATION_ERROR_START | 0x01,
    QL_ESIM_NOTIFICATION_UNDEFINED_ERROR               = QL_ESIM_DELETE_NOTIFICATION_ERROR_START | 0x7F,

    QL_ESIM_AUTH_SERVER_RESP_ERROR_START                           = 0X700 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_AUTH_SERVER_INVALID_CERTIFICATE                        = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x01,
    QL_ESIM_AUTH_SERVER_INVALID_SIGNATURE                          = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x02,
    QL_ESIM_AUTH_SERVER_UNSUPPORTED_CURVE                          = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x03,
    QL_ESIM_AUTH_SERVER_NO_SESSION_CONTEXT                         = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x04,
    QL_ESIM_AUTH_SERVER_INVALID_OID                                = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x05,
    QL_ESIM_AUTH_SERVER_EUICC_CHALLENGE_MISMATCH                   = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x06,
    QL_ESIM_AUTH_SERVER_CIPK_UNKNOWN                               = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x07,
    QL_ESIM_AUTH_SERVER_UNDEFINED_ERROR                            = QL_ESIM_AUTH_SERVER_RESP_ERROR_START | 0x7F,

    QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START                      = 0X800 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_PREPARE_DOWNLOAD_INVALID_CERTIFICATE                   = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x01,
    QL_ESIM_PREPARE_DOWNLOAD_INVALID_SIGNATURE                     = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x02,
    QL_ESIM_PREPARE_DOWNLOAD_UNSUPPORTED_CURVE                     = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x03,
    QL_ESIM_PREPARE_DOWNLOAD_NO_SESSION_CONTEXT                    = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x04,
    QL_ESIM_PREPARE_DOWNLOAD_INVALID_TRANSACTIONID                 = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x05,
    QL_ESIM_PREPARE_DOWNLOAD_UNDEFINED_ERROR                       = QL_ESIM_PREPARE_DOWNLOAD_RESP_ERROR_START | 0x7F,

    QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START                     = 0X900 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_INCORRECT_INPUT_VALUES                                 = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x01,
    QL_ESIM_INVALID_SIGNATURE                                      = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x02,
    QL_ESIM_INVALID_TRANSACTIONID                                  = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x03,
    QL_ESIM_UNSUPPOTRED_CRT_VALUES                                 = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x04,
    QL_ESIM_UNSUPPORTED_REMOTE_OPERATION_TYPE                      = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x05,
    QL_ESIM_UNSUPPORTED_PROFILE_CLASS                              = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x06,
    QL_ESIM_SECP03T_STRUCTURE_ERROR                                = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x07,
    QL_ESIM_SECP03T_SECURITY_ERROR                                 = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x08,
    QL_ESIM_INSTALL_FAIL_DUE_TO_ICCID_ALREADY_EXISTS_ON_EUICC      = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x09,
    QL_ESIM_INSTALL_FAIL_DUE_TO_INSUFFICIENT_MEMORY_FOR_PROFILE    = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0A,
    QL_ESIM_INSTALL_FAIL_DUE_TOINTERRUOTION                        = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0B,
    QL_ESIM_INSTALL_FAIL_DUE_TOPE_PROCESSING_ERROR                 = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0C,
    QL_ESIM_INSTALL_FAIL_DUE_TO_DATA_MISMATCH                      = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0D,
    QL_ESIM_TEST_PROFILE_INSTALL_FAIL_DUE_TO_INVALID_NAAKEY        = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0E,
    QL_ESIM_PPR_NOT_ALLOWED                                        = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x0F,
    QL_ESIM_INSTALL_FAIL_DUE_TO_UNDEFINED_ERROR                    = QL_ESIM_PROFILE_INSTALL_RESULT_ERROR_START | 0x7F,

    QL_ESIM_FALLBACK_RESP_ERROR_START                              = 0XA00 | QL_ERRCODE_ESIM_BASE,
    QL_ESIM_FALLBACK_PROFILE_NOT_IN_DISABLE_STATUS                 = QL_ESIM_FALLBACK_RESP_ERROR_START | 0x02,
    QL_ESIM_FALLBACK_CAT_BUSY                                      = QL_ESIM_FALLBACK_RESP_ERROR_START | 0x05,
    QL_ESIM_FALLBACK_NOT_AVAILABLE                                 = QL_ESIM_FALLBACK_RESP_ERROR_START | 0x06,
    QL_ESIM_FALLBACK_COMMAND_ERROR                                 = QL_ESIM_FALLBACK_RESP_ERROR_START | 0x07,
    QL_ESIM_FALLBACK_UNDEFINED_ERROR                               = QL_ESIM_FALLBACK_RESP_ERROR_START | 0x7F,
}ql_esim_result_errno_e;

typedef enum
{
    QL_ESIM_NOTIFICATION_GET_OPT = 0,
    QL_ESIM_NOTIFICATION_DELETE_OPT,
    QL_ESIM_NOTIFICATION_REPORT_OPT
}ql_esim_notify_opt_e;

typedef enum
{
    QL_ESIM_EIM_INFO_UPDATE = 0,
    QL_ESIM_EIM_INFO_DELETE,
    QL_ESIM_EIM_INFO_ADD
} ql_esim_eim_opt;

typedef enum {
    QL_EIMIDTYPEOID = 1,
    QL_EIMIDTYPEFQDN,
    QL_EIMIDTYPEPROPRIETARY
}ql_eim_id_type_e;

typedef enum {
    HELIOS_ESIM_MODE_LPA = 0X00,
    HELIOS_ESIM_MODE_IPA = 0X01,
    HELIOS_ESIM_MODE_INVALID = 0XFF
}ql_esim_mode;
/*========================================================================
*  Type Definition
*========================================================================*/
typedef struct {
    char iccid[ESIM_LPA_DATA_MAX+1];
    uint8_t status;
    char nickname[ESIM_LPA_DATA_MAX+1];
    char provider[ESIM_LPA_DATA_MAX+1];
}ql_esim_profile_inforation_t;

typedef struct {
    int32_t profile_num;
    ql_esim_profile_inforation_t profile_list[ESIM_LPA_PROFILE_LIST_MAX_NUM];
} ql_esim_profile_list_t;



typedef struct {
    uint32_t type;
    uint32_t seq_num;
    char iccid[ESIM_LPA_DATA_MAX+1];
}ql_esim_lpa_nofify_info_t;

typedef struct{
    uint8_t list_num;
    ql_esim_lpa_nofify_info_t notyfication_list[ESIM_LPA_PROFILE_LIST_MAX_NUM];
}ql_esim_lpa_nofify_list_t;

typedef struct {
    ql_esim_profile_down_type down_type;
    uint32_t   playload_len;
    uint8_t    *playload_info;
    uint32_t    data_len;
    uint8_t     data[ESIM_LPA_DATA_MAX+1];
}ql_esim_uart_download_resp_t;

typedef struct {
    bool        status;
    ql_eim_id_type_e eim_type;
    char eim_id_info[ESIM_LPA_DATA_MAX+1];
    char eim_fqdn_info[ESIM_LPA_DATA_MAX+1];
}ql_esim_ipa_eim_info_t;

typedef struct{
    int32_t element_num;
    ql_esim_ipa_eim_info_t notyfication_list[ESIM_LPA_PROFILE_LIST_MAX_NUM];
}ql_esim_ipa_eim_list_t;

typedef struct {
    uint32_t    ipa_poll_interval;
    uint32_t    rollback_interval;
#if 0
    bool        auto_sync_flag;
    bool        regular_sync_flag;
    uint32_t    regular_interval;
#endif
}ql_esim_ipa_cfg_rsp_t;

typedef struct {
    uint32_t data_len;
    char *data;
}ql_esim_data_array;

typedef struct {
    ql_esim_notify_opt_e    opt;              /*!< notification 操作类型*/
    uint32_t                apdu_result_code; /*!< 执行APDU应答结果返回 */
    uint32_t                result;           /*!< 执行命令应答结果返回 */
    ql_esim_data_array      rsp_info;           /*!< 仅用于get notification，存放notification信杯*/
}ql_esim_notify_resp_t;
/*========================================================================
 *	function Definition
 *========================================================================*/
typedef void (*ql_esim_cb_t)(uint8_t sim_id, ql_esim_command_rsp_e ind_flag,ql_esim_result_errno_e result , void *contextPtr);

/*****************************************************************
* Function: ql_esim_lpa_get_eid
*
* Description: 获取esim卡eid信息
* 
* Parameters:NULL
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_get_eid(void);

/*int Helios_Esim_init(Helios_Esim_Mode mode);
int Helios_Esim_deinit();*/
/*****************************************************************
* Function: ql_esim_lpa_get_profiles_info
*
* Description: 获取profiles信息
*   
* 
* Parameters:
*   listmode     [in] 查询模式
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_esim_lpa_get_profiles_info(ql_esim_profile_list listmode);

/*****************************************************************
* Function: ql_esim_lpa_profile_handle
*
* Description:对esim中的profile进行操作
*   
* 
* Parameters:
*   opt       [in] 操作类型
*   iccid     [in] profile的iccid
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_profile_handle(ql_esim_profile_operate_e opt, uint8_t *iccid);

/*****************************************************************
* Function: ql_esim_lpa_set_nickname
*
* Description:设置profile的nickname
*   
* 
* Parameters:
*   iccid       [in] profile的iccid
*   nickname    [in] nickname指针
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_set_nickname(uint8_t *iccid,uint8_t *nickname);

/*****************************************************************
* Function: ql_esim_lpa_profile_ota
*
* Description:下载profile到esim
*   
* 
* Parameters:
*   download_mode       [in] 下载模式
*   activationCode      [in] profile激活码
*   confirmationCode    [in] profile确认码
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_profile_ota(ql_esim_profile_down_type download_mode,uint8_t *activationCode, uint8_t *confirmationCode);

/*****************************************************************
* Function: ql_esim_lpa_profile_transmit
*
* Description:使用串口下载profile，传输本地写号的数据
*   
* 
* Parameters:
*   total_len       [in] 表示需要发送数据的总长度，最大是65535
*   state           [in] 表示是否是最后一包, 1不是， 0是
*   seq             [in] 表示数据包的编号，从0开始，按顺序累加，不支持乱序
*   data_len        [in] 表示当前数据包的长度
*   data            [in] p当前数据包的内容
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_profile_transmit(uint32_t total_len,bool state,uint8_t seq,uint32_t data_len,uint8_t *data);

/*****************************************************************
* Function: ql_esim_lpa_get_notification_list
*
* Description:查询esim中的notification
*   
* 
* Parameters:
*   type                [in] 查询的类型
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_get_notification_list(ql_esim_notify_type type);

/*****************************************************************
* Function: ql_esim_lpa_notification_handle
*
* Description:操作esim中的notification
*   
* 
* Parameters:
*   opt_mod             [in] 操作类型
*   seq_num             [in] notification序号
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_lpa_notification_handle(ql_esim_notify_opt_e opt_mode,uint32_t seq_num);

/*****************************************************************
* Function: ql_esim_ipa_poll_timer_interval_set
*
* Description:设置IPA从eim请求命令的时间间隔
*   
* 
* Parameters:
*   sec          [in] 时间，单位s
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_poll_timer_interval_set(int sec);

/*****************************************************************
* Function: ql_esim_ipa_eim_info_list
*
* Description:遍历eim信息
*   
* 
* Parameters:
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_eim_info_list(void);

/*****************************************************************
* Function: ql_esim_ipa_eim_info_operate
*
* Description:管理eim信息
*   
* 
* Parameters:
*   eim_id          [in] eid id
*   eim_domain      [in] eim域名
*   mode            [in] 操作域名
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_eim_info_operate(uint8_t *eim_id,uint8_t *eim_domain,ql_esim_eim_opt mode);

/*****************************************************************
* Function: ql_esim_ipa_timer_config_get
*
* Description:查询ipa相关的定时器配置信息
*   
* 
* Parameters:
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_timer_config_get(void);

/*****************************************************************
* Function: ql_esim_ipa_profile_fallback
*
* Description:IPA 执行fallback 操作，启用被标记fallback的profile。。
*   
* 
* Parameters:
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_profile_fallback(void);

/*****************************************************************
* Function: ql_esim_ipa_profile_fallback_return
*
* Description:IPA 执行fallback return 操作，回退到启用fallbcak之前的profile。
*   
* 
* Parameters:
*
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_ipa_profile_fallback_return(void);

/*****************************************************************
* Function: ql_esim_service_init
*
* Description:esim task初始化。
*   
* 
* Parameters:
*   mode        [in] esim功能类型
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/

int ql_esim_service_init(ql_esim_mode_e mode);

/*****************************************************************
* Function: ql_esim_register_cb
*
* Description:
*   注册回调函数
* 
* Parameters:
*   callback     [in] 回调函数
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
int ql_esim_register_cb(ql_esim_cb_t callback);

#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_NW_H*/


