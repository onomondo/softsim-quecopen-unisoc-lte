/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#ifndef _QUEC_AT_RESPONSE_H_
#define _QUEC_AT_RESPONSE_H_

#include "ql_api_common.h"
#include "ql_uart_internal.h"
#include "quec_at_param.h"

/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QUEC_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define QUEC_DO_WHILE0(expr) \
    do                      \
    {                       \
        expr                \
    } while (0)

#define quec_at_resp_error(engine)				quec_atCmdResp(engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR)
#define quec_at_resp_ok(engine)					quec_atCmdResp(engine, ATCI_RESULT_CODE_OK, CMD_RC_OK)
#define quec_at_resp_cms_error(engine,errcode)	quec_atCmdResp(engine, ATCI_RESULT_CODE_CMS_ERROR, errcode)
#define quec_at_resp_cme_error(engine,errcode)	quec_atCmdResp(engine, ATCI_RESULT_CODE_CME_ERROR, errcode)
	
#define QUEC_RETURN_CME_ERR(engine, err)		{quec_at_resp_cme_error(engine, err); return;}						
#define QUEC_RETURN_CMS_ERR(engine, err) 		{quec_at_resp_cms_error(engine, err); return;}	
#define QUEC_RETURN_OK(engine)					{quec_at_resp_ok(engine); return;}	
#define QUEC_RETURN_ERROR(engine)				{quec_at_resp_error(engine); return;}	
#define QUEC_RETURN_FOR_ASYNC() 				{return;}

#define QUEC_ATRESP_TEXT(engine,text)			quec_atCmdRespInfoNText(engine, text, strlen(text), 1)
#define QUEC_ATRESP_NTEXT(engine,text,len)		quec_atCmdRespInfoNText(engine, text, len, 1)

/*========================================================================
 *  Enum Definition
 *========================================================================*/
typedef enum
{
	QL_AT_CB_TYPE_TIMEOUT,
	QL_AT_CB_TYPE_MAX
}QL_AT_CB_TYPE_E;

/*========================================================================
 *  Struct Definition
 *========================================================================*/
typedef int (*resp_cb)(ql_dispatch_t *engine, _AtciResultCode resultCode, int code);
typedef int (*finish_cb)(ql_dispatch_t *engine);
typedef struct
{
    ql_dispatch_t *engine;
    int32_t resultCode;
	int32_t code;
    resp_cb cb;
    void *param;
} quec_at_result_resp_t;

typedef struct
{
    ql_dispatch_t *engine;
	finish_cb cb;
} quec_at_finish_resp_t;

typedef struct
{
	quec_dev_e				dev;
	ql_dispatch_t			*dispatch;
	ql_timer_t 				timer;
	ql_at_cmd_t				*cmd;
	quec_at_timeout_cb_t	cb;
	uint					value;
} quec_at_timeout_t;

typedef struct
{
	QL_AT_CB_TYPE_E		type;
	
	quec_at_timeout_t	*timeout;//超时触发类回调函数
} quec_at_callback_t;

bool quec_atc_is_parsing(ql_dispatch_t *engine);

ql_task_t quec_at_get_engine_id(void);

ql_at_cmd_t *quec_at_get_cmd_table(ql_dispatch_t *dispatch);

void quec_at_output_promt(ql_dispatch_t *engine, int formatcfg);

uint quec_at_get_count(void);

const ql_at_desc_t *quec_at_get_table(size_t n);

void quec_at_set_first_info(ql_dispatch_t *engine, bool is_first);

bool quec_at_get_first_info(ql_dispatch_t *engine);

void quec_at_resp_interm_code(ql_dispatch_t *engine, int code);

void quec_at_set_error_flag(ql_dispatch_t *engine);

void quec_atCmdResp(ql_dispatch_t *engine, _AtciResultCode resultCode, uint report_code);

int quec_atCmdRespInfoNText(ql_dispatch_t *engine, const char *text, size_t length, unsigned char padding);

int quec_at_finish(ql_dispatch_t *engine);

int quec_at_get_attach_cnt(ql_dispatch_t *engine);

bool quec_at_set_timeout_cb(ql_dispatch_t *dispatch, uint timeout, quec_at_timeout_cb_t handler);

void quec_atCmdRespOutputPrompt(ql_dispatch_t *engine, int formatcfg);

void quec_at_async_param_free(ql_at_cmd_t *cmd);

void quec_at_worker_call(ql_dispatch_t *th, quec_at_cb_t cb, void *ctx);

const char *quec_at_get_code_txt(int code);

int quec_at_list_deinit(ql_dispatch_t *dispatch);
	
const char *quec_at_get_code_txt_ds(int code);
/**********************************************************************************************************************
    resultCode:   ATCI_RESULT_CODE_OK  ATCI_RESULT_CODE_ERROR  ATCI_RESULT_CODE_CME_ERROR  ATCI_RESULT_CODE_CMS_ERROR
    report_code:  CMD_RC_OK,  CMD_RC_ERROR, CMD_RC_CONNECT, CMD_RC_BUSY ......
	rsp_buffer:   the buffer to be send out , if no buffer to send , input NULL
	padding   :   if need add \r\n at the begin and/or end of the string, the begin padding is effected by atv
				  0: no add s3,s4, 1: auto detect s3,s4 and add to begain and end, 2: force add s3,s4 at the begin and end, 
				  3: force add s3,s4 at the begin, 4: force add s3,s4 at the end, and 1 is default
***********************************************************************************************************************/
void quec_atResp(ql_dispatch_t *engine, _AtciResultCode resultCode, uint report_code, char *rsp_buffer , unsigned char padding);

void quec_atCmdRespOutputPrompt(ql_dispatch_t *engine, int formatcfg);

#endif


