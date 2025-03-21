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

#ifndef QL_WEBSOCKET_CLIENT_API_H
#define QL_WEBSOCKET_CLIENT_API_H
#include "ql_api_common.h"

#define QL_WEBSOCKET_LOG_LEVEL	                    QL_LOG_LEVEL_INFO
#define QL_WEBSOCKET_LOG(msg, ...)			        QL_LOG_TAG(QL_WEBSOCKET_LOG_LEVEL, QL_LOG_TAG_WEBSOCKET, "ql_websocket", msg, ##__VA_ARGS__)
#define QL_WEBSOCKET_LOG_PUSH(msg, ...)	        QL_LOG_PUSH("ql_websocket", msg, ##__VA_ARGS__)

//---- config id & client id limitation ----

#define QWS_CFG_ID_NUM 					3
#define QWS_CLIENT_ID_NUM 				QWS_CFG_ID_NUM

#define QL_WS_DATA_TEXT_FMT           	0
#define QL_WS_DATA_HEX_FMT            	1

#define QWS_LWS_TX_PKT_SZ 10*1024
#define QWS_LWS_RX_PKT_SZ 2048

typedef enum
{
	QWS_CFG_TYPE_CONN,
	QWS_CFG_TYPE_PING,
	QWS_CFG_TYPE_WRITE,
	QWS_CFG_TYPE_READ,
	QWS_CFG_TYPE_CLOSE,
	QWS_CFG_CONN_PDPCID,	
	QWS_CFG_CONN_SSLCTXID,
	QWS_CFG_CONN_URL,
    QWS_CFG_CONN_URL_EX,
	QWS_CFG_CONN_SUBPROT_EN,
	QWS_CFG_CONN_SUBPROT,
	QWS_CFG_CONN_EXTENSION_EN,
	QWS_CFG_CONN_EXTENSION,
	QWS_CFG_CONN_METHOD,
	QWS_CFG_CONN_TIMEOUT,
	QWS_CFG_PING_INTERVAL,
	QWS_CFG_WRITE_BUFFERSZ,
	QWS_CFG_WRITE_TIMEOUT,
	QWS_CFG_WRITE_ECHO,
	QWS_CFG_READ_BUFFERSZ,
	QWS_CFG_READ_TIMEOUT,
	QWS_CFG_READ_MODE,
	QWS_CFG_CLOSE_WAITTIME,
	QWS_CFG_RECV_CB,
	QWS_CFG_REQHEAD_ADD,
	QWS_CFG_REQHEAD_REMOVE,
	QWS_CFG_WRITE_MAX
} qws_cfg_e_;


typedef enum
{
	QWS_CLIENT_GET_ERRCODE,
	QWS_CLIENT_GET_CONNSTATE,
	QWS_CLIENT_GET_UNSENT_SIZE,
	QWS_CLIENT_RES_MAX
} qws_client_e_;

//---- qws errno ----
typedef enum
{
	QWS_NEGATIVE_ERROR=-1,
	QWS_OK=0,
	QWS_ERR_GENERIC,
	QWS_ERR_PROT_FAIL,
	QWS_ERR_INVAL_PARM,
	QWS_ERR_NO_MEM,
	QWS_ERR_TIMEDOUT,
	QWS_ERR_BUSY,
	QWS_ERR_NOT_INIT,
	QWS_ERR_NET_FAIL,
	QWS_ERR_ID_OCCUPIED,
	QWS_ERR_INVAL_URL,
	QWS_ERR_TASK_CREATE,
	QWS_ERR_NET_CLOSE,
	QWS_ERR_NET_DOWN,
} qws_err_e;
	
typedef enum
{
	QWS_CFG_READ_MODE_BUFFER,
	QWS_CFG_READ_MODE_PUSH
} qws_cfg_read_mode_e;

//---- connect state ----
typedef enum
{
	QWS_CONN_STA_IDLE,
	QWS_CONN_STA_CONNECTING,
	QWS_CONN_STA_CONNECTED,
	QWS_CONN_STA_CLOSING,
	QWS_CONN_STA_CLOSED=QWS_CONN_STA_IDLE
} qws_conn_sta_e;

typedef enum
{
	QWS_CB_TYPE_RECV,
	QWS_CB_TYPE_OPEN,
	QWS_CB_TYPE_CLOSE,
	QWS_CB_TYPE_MAX
} qws_cb_e_;


typedef enum
{
	QWS_WRITE_PROT_TEXT = 0,
	QWS_WRITE_PROT_BIN = 1,
	QWS_WRITE_PROT_MAX
} qws_write_prot_e;

typedef struct
{
	int type;
	char *data;
	int size;
	int virtual;
} qws_parm_t;

typedef struct
{
	int client_id;
	int cfg_id;
	char *data;
	int user;
} qws_open_t;

typedef int (*ws_client_recv_cb_t)(int client_id, qws_parm_t *cb_t);


/*****************************************************************
* Function: ql_ws_cfg_set_opt
*
* Description: 通过opt_tag类型信息,配置websocket client上下文
* 
* Parameters:
*	cfg_id        [in]    websocket client 配置索引.
*   opt_tag       [in]    websocket client 上下文中配置的参数处理类型
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
qws_err_e ql_ws_cfg_set_opt(uint8 cfg_id, int opt_tag,...);

/*****************************************************************
* Function: ql_ws_cfg_get_opt
*
* Description: 通过opt_tag类型信息,读取 websocket client 配置参数
* 
* Parameters:
*	cfg_id        [in]    websocket client 配置索引.
*   opt_tag       [in]    websocket client 上下文中配置的参数处理类型
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
qws_err_e ql_ws_cfg_get_opt(uint8 cfg_id, int opt_tag,...);

/*****************************************************************
* Function: ql_ws_open_proc
*
* Description:  websocket client 启动 qws_open_t 配置信息
* 
* Parameters:
*	client_id     [in]    websocket client 索引.
*	cfg_id        [in]    websocket client 配置索引.
*	data/user     [in]    扩展保留信息.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int ql_ws_open_proc(qws_open_t* open_t);

/*****************************************************************
* Function: ql_ws_write_proc
*
* Description:  websocket client 发送数据
* 
* Parameters:
*	client_id     [in]    websocket client 索引.
*	qws_parm_t    [in]    websocket client 发送数据结构.
*	type      	  [in]    数据类型 QWS_WRITE_PROT_TEXT/QWS_WRITE_PROT_BIN 
*	data      	  [in]    要发送的数据 
*	size      	  [in]    要发送数据的长度
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int ql_ws_write_proc(int client_id, qws_parm_t *parm_t);

/*****************************************************************
* Function: ql_ws_close_proc
*
* Description:  websocket client 关闭退出
* 
* Parameters:
*	client_id     [in]    websocket client 索引.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int ql_ws_close_proc(int client_id);

/*****************************************************************
* Function: ql_ws_client_get_opt
*
* Description: 通过opt_tag类型信息,读取 websocket client 参数
* 
* Parameters:
*	client_id     [in]    websocket client 索引.
*   opt_tag       [in]    websocket client 上下文中的数据类型 qws_client_e_
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
qws_err_e ql_ws_client_get_opt(uint8 client_id, int opt_tag,...);

#endif /*QL_WEBSOCKET_CLIENT_API_H*/
///
///


