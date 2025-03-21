/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/

/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_API_USBNET_H
#define QL_API_USBNET_H


#include "ql_api_common.h"
#include "ql_api_datacall.h"


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_USBNET_ERRCODE_BASE        (QL_COMPONENT_NETWORK_USBNET<<16)


/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
	QL_USBNET_SUCCESS 					= 0,
	QL_USBNET_EXECUTE_ERR 				= 1 | QL_USBNET_ERRCODE_BASE,
	QL_USBNET_MEM_ADDR_NULL_ERR,
	QL_USBNET_INVALID_PARAM_ERR,
	QL_USBNET_USB_NOT_CONNECT_ERR,
	QL_USBNET_PDP_ACTIVE_ERR			= 5 | QL_USBNET_ERRCODE_BASE,
	QL_USBNET_REPEAT_CONNECT_ERR,
	QL_USBNET_REPEAT_DISCONNECT_ERR,
}ql_usbnet_errcode_e;

//support ECM and RNDIS
typedef enum 
{
	QL_USBNET_NONE = 0,
	QL_USBNET_ECM,
	QL_USBNET_MBIM,
	QL_USBNET_RNDIS,
	QL_USBNET_MAX
}ql_usbnet_type_e;


typedef enum 
{
	QL_USBNET_STATE_NONE = 0,			//usbnet initialization state
	QL_USBNET_STATE_START,				//usbnet connecting
	QL_USBNET_STATE_CONNECT,			//usbnet connected
	QL_USBNET_STATE_PORT_DISCONNECT,	//usb port disconnect
	QL_USBNET_STATE_MAX,
}ql_usbnet_state_e;

/*===========================================================================
 * STRUCT
 ===========================================================================*/
typedef struct
{
    uint8_t *data;
    int      data_len;
}ql_usbnet_uplink_s;


/*===========================================================================
 * function
 ===========================================================================*/

/*****************************************************************
* Description: usbnet callback
* 
* Parameters:
*   ind_type	[in]	Event Notification Type
* 	errcode		[in]	error code, 0 indicates that the execution result was correct
*	ctx			[in]	reserved for callback
*
* Retuen:
*	None
*****************************************************************/
typedef void (*ql_usbnet_callback)(unsigned int ind_type, ql_usbnet_errcode_e errcode, void *ctx);

/*****************************************************************
* Description: usbnet datalink callback
* 
* Parameters:
*   uplink_data	    [in]	usbnet data
*
* Retuen:
*	None
*****************************************************************/
typedef bool (*ql_usbnet_datalink_callback)(ql_usbnet_uplink_s *uplink_data);

/*****************************************************************
* Description: set usbnet type and reboot to take effect
* 
* Parameters:
*   usbnet_type		[in]	usbnet type, only support QL_USBNET_ECM and QL_USBNET_RNDIS
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_set_type(ql_usbnet_type_e usbnet_type);


/*****************************************************************
* Description: get usbnet type that has been set
* 
* Parameters:
*   usbnet_type		[out]	usbnet type that has been saved
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_get_type(ql_usbnet_type_e *usbnet_type);


/*****************************************************************
* Description: start usbnet connect
* 
* Parameters:
*	nSim			[in]	sim index，range: 0-1
*	profile_idx		[in]	pdp index, range: 1-7
*	config			[in]	pdp context configuration info, If set to NULL, the default parameters are used 
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_start(uint8_t nSim, int profile_idx, ql_data_call_conf_s *config);


/*****************************************************************
* Description: stop usbnet connect
* 
* Parameters:
*	None 
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_stop(void);


/*****************************************************************
* Description: get usbnet status
* 
* Parameters:
*	status			[out]	usbnet status, Refer to the enumeration of ql_usbnet_state_e
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_get_status(ql_usbnet_state_e *status);


/*****************************************************************
* Description: register usbnet callback
* 
* Parameters:
*	usbnet_cb		[in]	usbnet callback
*	ctx				[in]	reserved for callback
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e ql_usbnet_register_cb(ql_usbnet_callback usbnet_cb, void *ctx);


ql_usbnet_errcode_e ql_usbnet_register_uplink_cb(ql_usbnet_datalink_callback uplink_cb);

/*****************************************************************
* Description: send data to usbnet
* 
* Parameters:
*	data		    [in]	data address, must be 4-byte aligned
*	size			[in]	size of data send
*
* Return:
*	0: 		success
*	other: 	error code
*****************************************************************/
ql_usbnet_errcode_e  ql_usbnet_send_data_to_usb(uint8_t *data, uint16_t size);
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_USBNET_H */


