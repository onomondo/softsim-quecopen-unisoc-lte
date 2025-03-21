/**  @file
  quec_usbnet.h

  @brief
  This file is used to define version information for different Quectel Project.

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



#ifndef QUEC_USBNET_H
#define QUEC_USBNET_H

#include "quec_modem_interface.h"
#include "quec_common.h"
#include "ql_api_usbnet.h"
#include "quec_at_engine.h"
#include "ql_api_osi.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USBNET

#define QUEC_USBNET_DEBUG 0


/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QUEC_USBNET_TIMER_PERIOD_MIN 2
#define QUEC_USBNET_TIMER_PERIOD_MAX 300


/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum 
{
    QUEC_USB_NET_CONNECT_DISABLE = 0,
    QUEC_USB_NET_CONNECT_MANAUAL = 1,
    QUEC_USB_NET_CONNECT_AUTO = 3,
    QUEC_USB_NET_CONNECT_MAX,
}quec_usb_net_connect_e;
    
    
typedef enum 
{
    QUEC_USB_NET_MSG_NONE = 0,
    QUEC_USB_NET_MSG_AUTO_TIMER_START,
    QUEC_USB_NET_MSG_AUTO_TIMER_STOP,
    QUEC_USB_NET_MSG_ENABLE,//enable usbnet by at cmd
    QUEC_USB_NET_MSG_DISABLE,//disable usbnet by at cmd
    QUEC_USB_NET_MSG_UP,
    QUEC_USB_NET_MSG_DOWN,
    QUEC_USB_NET_MSG_PORT_CONNECT,
    QUEC_USB_NET_MSG_PORT_DISCONNECT,
    QUEC_USB_NET_MSG_NETIF_SET,
    QUEC_USB_NET_MSG_PS_ATTACH,
    QUEC_USB_NET_MSG_MAX,
}quec_usb_net_msg_e;


/*========================================================================
 *	Type Definition
 *========================================================================*/
typedef struct
{
    quec_usb_net_connect_e connect_type;
    uint8_t connect_sim_cid;
    quec_enable_e connect_urc;
}quec_usb_net_dev_cfg_s;

typedef enum 
{
	QUEC_USB_STATE_NONE = 0,
	QUEC_USB_STATE_DISCONNECT,
	QUEC_USB_STATE_CONNECT,
	QUEC_USB_STATE_MAX,
}quec_usb_state_e;


typedef struct
{
    quec_usb_net_dev_cfg_s usbnetdevcfg;
    ql_usbnet_state_e connect_state;
	quec_usb_state_e usb_state;
    quec_netif *connect_netif;
    quec_netif *connect_wan_netif;
    uint16_t timer_period;//unit: s
    uint32_t retry_cnt;
}quec_usb_net_connect_ctx_s;

typedef struct
{
    uint32_t id;
    uint32_t nMsg;
    uint32_t nParam1;
    uint8_t nParam2;
    uint8_t nParam3;
    uint16_t nParam4;
}quec_usbnet_event_s;

typedef struct
{
	uint32_t ind_type;
	int err_code;
}quec_usb_net_event_ctx_s;

typedef struct
{
	ql_usbnet_callback event_cb;
	void *ctx;
}quec_usb_net_cb_context_s;


/*========================================================================
*  Variable Definition
*========================================================================*/
extern quec_usb_net_connect_ctx_s q_usbnet_connect_ctx;

/*========================================================================
 *	Utilities Definition
 *========================================================================*/


/*========================================================================
 *	function Definition
 *========================================================================*/
void quectel_exec_qcfg_usbnet_cmd(atCommand_t *cmd);
void quectel_exec_qdbgcfg_usbnet_cmd(atCommand_t *cmd);


void quec_usbnet_try_lock(void);
void quec_usbnet_unlock(void);
void ql_usbnet_set_status(ql_usbnet_state_e status);

void quec_netdev_init(void);
quec_netif *quec_usbnet_connect_netif_get(void);
quec_netif *quec_usbnet_wan_netif_get(void);
bool quec_usbnet_sendevent(uint32_t nMsg, uint32_t nParam1, uint8_t nParam2, uint8_t nParam3, uint16_t nParam4);
bool quec_usbnet_send_osi_event_set_if(quec_netif *net_if, quec_netif *wan_if);
void quec_usbnet_connect_ctx_init(void);
void quec_usbnet_retry_or_reinit(void *ctx);
int8_t quec_usbnet_connect_enable_with_cid(const uint8_t nSim, const uint8_t cid);
ql_task_t quec_get_usbnet_threadID(void);
int8_t quec_netdevctl_cfg_save(uint8_t usbnet_op, uint8_t usbnet_cid, uint8_t usbnet_urc);
ql_usbnet_errcode_e ql_usbnet_active_up(void);


#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_USBNET_H */






