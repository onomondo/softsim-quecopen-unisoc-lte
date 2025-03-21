
/**  @file
  ql_mms_client.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
    
#ifndef QL_MMS_CLIENT_H
#define QL_MMS_CLIENT_H

#define mms_print(msg, ...)     custom_log_tag("MMS_CLI", QUEC_LOG_TAG_MMS, msg, ##__VA_ARGS__)

#define QL_MMS_BCC_ADDR_MAX_CNT             6
#define QL_MMS_CC_ADDR_MAX_CNT              6
#define QL_MMS_TO_ADDR_MAX_CNT              6
#define QL_MMS_ATTACHMENT_MAX_CNT           12

typedef void*  ql_mms_client; 

typedef enum{
	QL_MMS_OK                        = 0,
	QL_MMS_INVLIAD_PARAM             = -1,
	QL_MMS_WOULDBLOCK                = -2,
	QL_MMS_OUT_OF_MEM                = -3,
	QL_MMS_HTTP_REQ_FAIL             = -4,
	QL_MMS_OPT_BUSY                  = -5,
	QL_MMS_HTTP_RESP_FAIL            = -6,
	QL_MMS_SEND_FAIL                 = -7,
	QL_MMS_HTTP_TIMEOUT              = -8,
	QL_MMS_DNS_FAIL                  = -9,
	QL_MMS_FS_FAIL                   = -10,
}mms_result_code_e;

typedef enum{
	MMS_CHARSET_ASCII_VALUE      = 0x03,
	MMS_CHARSET_UTF8_VALUE       = 0x6A,
	MMS_CHARSET_UCS2_VALUE       = 0x03E8,
	MMS_CHARSET_BIG5_VALUE       = 0x07EA,
	MMS_CHARSET_GBK_VALUE        = 0x71,
}mms_charset_type;


typedef enum 
{
    MMS_PARAM_VALID_1HOUR=0,
    MMS_PARAM_VALID_12HOURS,
    MMS_PARAM_VALID_24HOURS,
    MMS_PARAM_VALID_2DAYS,
    MMS_PARAM_VALID_1WEEK,
    MMS_PARAM_VALID_MAXIMUM,
    MMS_PARAM_VALID_DEFAULT
}mms_valid_type;

typedef enum 
{
    MMS_PARAM_PRIORITY_LOWEST=0,
    MMS_PARAM_PRIORITY_NORMAL,
    MMS_PARAM_PRIORITY_HIGHEST,
    MMS_PARAM_PRIORITY_DEFAULT
}mms_priority_type;

typedef enum 
{
    MMS_PARAM_SENDERADDR_HIDE=0,
    MMS_PARAM_SENDERADDR_SHOW,
    MMS_PARAM_SENDERADDR_DEFAULT
}mms_visible_type;

typedef enum
{
    MMS_PARAM_CLASS_PERSONAL=0,
    MMS_PARAM_CLASS_ADVERTISEMENT,
    MMS_PARAM_CLASS_INFORMATIONAL,
    MMS_PARAM_CLASS_AUTO,
    MMS_PARAM_CLASS_DEFAULT
}mms_class_type;

struct mms_send_param_t{
	int            validity_period;
	int            priority;
	int            delivery_report;
	int            read_report;
	int            visible;
	int            class;
};

struct mms_addr_t{
	struct mms_addr_t  *next;
	int                 addr_len;
	char               *address;
};

struct mms_attachment_t{
	struct mms_attachment_t  *next;
	char                     *file_path;
};

struct mms_subject_t{
	char                   *subject;
	int                     length;
};

struct mms_option_t{
	int             	          context_id;
	int              	          sim_id;
	char                         *mmsc_url;
	char           	             *proxy_addr;
	uint16_t                      proxy_port;
	int                           charset;	
	uint8_t                       supportfield;
	int                           wait_timeout;
	struct mms_send_param_t       send_param;
};

struct mms_info_t{
	struct mms_subject_t        *subject;
	struct mms_addr_t           *to_addr;
	struct mms_addr_t           *cc_addr;
	struct mms_addr_t           *bcc_addr;
	struct mms_attachment_t     *attachments;
};

typedef void(*mms_send_result_cb)(ql_mms_client        client_hndl, int mms_result, int http_code, char *mms_err_msg, void *arg);

/*****************************************************************
* Function: ql_mms_client_new
*
* Description: 创建并初始化mms client端的句柄
* 
* Parameters:
*   client_hndl   [out]   mms client 端句柄,用于记录mms消息头和消息体,以及mms的状态信息.
*   result_cb     [in]    结果的回调处理函数,mms消息响应后,调用此函数,获取mms响应的状态信息.
*   arg           [in]    arg用户自定义配置参数,可用于回调函数result_cb中.
*
* Return:
*   0             成功.
*   other         错误码.
*
*****************************************************************/
int  ql_mms_client_new(ql_mms_client *client_hndl, mms_send_result_cb       result_cb, void *arg);


/*****************************************************************
* Function: ql_mms_client_send_msg
*
* Description: 发送mms消息
* 
* Parameters:
*   client_hndl   [in]    mms client 端句柄,将已记录mms消息作为发送内容的一部分.
*   send_option   [in]    用户配置mms的参数信息,决定mms消息的具体属性.
*   mms_info_t    [in]    用户通过命令行设置的mms信息,做为mms将要发送的内容的一部分.
*
* Return:
*   0             成功.
*   other         错误码.
*
*****************************************************************/
int  ql_mms_client_send_msg(ql_mms_client *client_hndl, struct mms_option_t *send_option, struct mms_info_t  *mms_info);


/*****************************************************************
* Function: ql_mms_client_release
*
* Description: 释放mms client端句柄所持有的全部资源
* 
* Parameters:
*   client_hndl   [in]   mms client 端句柄.
*
* Return:
*   0             成功.
*   other         错误码.
*
*****************************************************************/
int  ql_mms_client_release(ql_mms_client *client_hndl);

#endif
