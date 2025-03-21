/**  
  @file
  ql_api_l2tp.h

  @brief
  This file provides the definitions for l2tp API functions.

*/
/*============================================================================
  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
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
#ifndef QL_API_L2TP_H
#define QL_API_L2TP_H

#include "ql_api_common.h"
#include "netif/ppp/ppp.h"
#include "netif/ppp/ppp_opts.h"
#include "netif/ppp/pppol2tp.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#define QL_L2TP_LOG(msg, ...)  QL_LOG_TAG(QL_LOG_LEVEL_INFO, QL_LOG_TAG_L2TP, "L2TP", msg, ##__VA_ARGS__)

#define QUEC_L2TP_SECRET_MAX 32

#if PPP_SUPPORT && PPPOL2TP_SUPPORT /* don't build if not configured for use in lwipopts.h */

typedef enum
{
    QL_L2TP_SUCCESS = 0,
    QL_L2TP_EXECUTE_ERR = 1 | (QL_COMPONENT_L2TP << 16),
    QL_L2TP_INVALID_PARAM_ERR,
    QL_L2TP_NO_MEM_ERR,
    QL_L2TP_INVALID_IP_ERR,
    QL_L2TP_INVALID_CID_ERR,
    QL_L2TP_PDP_ACTIVE_ERR,
    QL_L2TP_PPP_CONNECT_ERR,
} ql_l2tp_errcode_e;

typedef enum
{
    QL_L2TP_PPP_AUTH_NONE = 0,
    QL_L2TP_PPP_AUTH_PAP = 1,
    QL_L2TP_PPP_AUTH_CHAP = 2,
} ql_l2tp_ppp_auth_e;

typedef enum
{
    QL_L2TP_OPT_SIM = 0,
    QL_L2TP_OPT_PDPCID,
    QL_L2TP_OPT_SERVER_IP,
    QL_L2TP_PPP_AUTH_TYPE,
    QL_L2TP_PPP_CHAP_USER,
    QL_L2TP_PPP_CHAP_PWD,
    QL_L2TP_OPT_MAX,
} ql_l2tp_opt_e;

typedef enum
{
	QL_L2TP_STATE_NONE = 0,
	QL_L2TP_STATE_START,
	QL_L2TP_STATE_CONNECTED,    //成功创建L2TP通道.
    QL_L2TP_STATE_DISCONNECTING,//模组主动断开L2TP通道
    QL_L2TP_STATE_DEACTIVE,     //服务器主动断开L2TP或者掉网或等原因使用该状态
	QL_L2TP_STATE_MAX,
}ql_l2tp_state_e;

typedef void (*ql_l2tp_callback)(unsigned int ind_type, ql_l2tp_errcode_e errcode, void *ctx);

typedef struct
{
    uint8_t sim;
    uint8_t cid;
    ip_addr_t ipaddr;
    uint8_t secret[QUEC_L2TP_SECRET_MAX];
    uint8_t secret_len;
    char ppp_auth;
    char chap_user[QUEC_L2TP_SECRET_MAX];
    char chap_pwd[QUEC_L2TP_SECRET_MAX];
    uint8_t status;
} quec_l2tp_opt_s;

typedef struct
{
    ppp_pcb *ppp_pcb;
    pppol2tp_pcb *l2tp;
    void (*pppol2tp_input)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
    quec_l2tp_opt_s opt;

    ql_l2tp_callback event_cb;
	void *event_cb_ctx;

    struct netif *pppif; //该netif的ip地址为l2tp_ppp协商时, 服务器分配给模块ip地址
}quec_l2tp_session_s;


extern quec_l2tp_session_s quec_l2tp_session;

/*****************************************************************
 * Description: Register L2TP status notification callback.
 *
 * Parameters:
 * l2tp_cb        [in]        L2TP status notification callback.
 * ctx            [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_l2tp_errcode_e ql_l2tp_register_cb(ql_l2tp_callback l2tp_cb, void *ctx);

/*****************************************************************
 * Description: Set L2TP option.
 *
 * Parameters:
 * opt                  [in]    Option type.See ql_l2tp_opt_e.
 * val                  [in]    The address of input parameter.
 * val_len              [in]    The length of input parameter.See ql_l2tp_opt_e.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_l2tp_errcode_e ql_l2tp_opt_set(ql_l2tp_opt_e opt, void *val, int val_len);

/*****************************************************************
 * Description: Get L2TP option.
 *
 * Parameters:
 * opt                  [in]        Option type.See ql_l2tp_opt_e.
 * val                  [Out]       The address of output parameter.
 * val_len              [in]        The length of output parameter.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_l2tp_errcode_e ql_l2tp_opt_get(ql_l2tp_opt_e opt, void *val, int val_len);

/*****************************************************************
* Description: start L2TP connect
* 
* Parameters:
* argv        [in]        No defined yet.
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_l2tp_errcode_e ql_l2tp_start(void *argv);

/*****************************************************************
* Description: stop L2TP connect
* 
* Parameters:
* argv        [in]        No defined yet.
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_l2tp_errcode_e ql_l2tp_stop(void *argv);

#endif
#endif
