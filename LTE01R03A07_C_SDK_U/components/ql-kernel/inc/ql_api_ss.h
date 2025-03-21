/**  
  @file
  ql_api_ss.h

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
02/11/2022  Felix          create
=============================================================================*/

#ifndef QL_API_SS_H
#define QL_API_SS_H

#include <stdint.h>
#include <stdbool.h>
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QUEC_SS_USSD_STRING_SIZE_MAX 160
#define QUEC_SS_USSD_UCS2_SIZE_MAX   ((2 * QUEC_SS_USSD_STRING_SIZE_MAX) + 2)
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
	QL_SS_SUCCESS                   = 0,
	QL_SS_EXECUTE_ERR               = 1 | (QL_COMPONENT_SS << 16),
	QL_SS_MEM_ADDR_NULL_ERR,         
	QL_SS_INVALID_PARAM_ERR,
	QL_SS_NO_MEM_ERR,
}ql_ss_errcode_e;

typedef enum
{
	QL_SS_USSD_SESSION_INITIATE     = 1,
    QL_SS_USSD_SESSION_CANCEL       = 2,
}ql_ss_ussd_option_e;

typedef enum
{
	QL_SS_USSD_RESP_SUCCESS         = 0,
    QL_SS_USSD_CANCEL_SUCCESS       = 1,
    QL_SS_USSD_SAT_SUCCESS          = 2,
    QL_SS_USSD_NO_STR_ERR           = 3,
    QL_SS_USSD_OTHER_ERR            = 4,
}ql_ss_ussd_resp_e;

typedef enum
{
    QL_SS_SERVICE_NOT_IMPLEMENTED       = (0xFA << 16)|10,
    QL_SS_INVALID_OPERATION             = (0xFA << 16)|11,
    QL_SS_INVALID_SERVICE_CODE          = (0xFA << 16)|12,
    QL_SS_MEMORY_FAILURE                = (0xFA << 16)|14,
    QL_SS_MM_ESTABLISHMENT              = (0xFA << 16)|20,
    QL_SS_TRANSACTION_TIMEOUT           = (0xFA << 16)|21,
    QL_SS_MM_CONNECTION_RELEASED        = (0xFA << 16)|22,
    QL_SS_UNKNOWN_COMPONENT_RECEIVED    = (0xFA << 16)|23,
    QL_SS_OPERATION_NOT_SUPPORTED       = 4,
    QL_SS_UNKNOWN                       = 50,
}ql_ss_exec_err_e;
/*========================================================================
*  Type Definition
*========================================================================*/
typedef struct
{
    uint8_t                 dcs;
    uint8_t                 *str;
    uint32_t                len;
    ql_ss_ussd_resp_e       resp_type;
    ql_ss_exec_err_e         err_code;
}ql_ss_ussd_str_s;

typedef struct 
{
    ql_ss_ussd_option_e option;
    char* str;                      //USSD code
}ql_ss_ussd_send_s;


typedef void (*ql_ss_callback_t)(uint8_t nSim, unsigned int ind_type,void *ctx);
/*========================================================================
 *	function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_ss_ussd_set_dcs
* 
* Description: Set user DCS
* 
* Parameters: 
*   nSim                [in]    SIM index,range: 0-1
*   dcs                 [in]    CBS Data Coding Scheme,see 3GPP TS 23.038
*                               The DCS decides in which format we send the USSD code.
* 
* Return:
*   0           success
*   other       error code
* 
*****************************************************************/
ql_ss_errcode_e ql_ss_ussd_set_dcs(uint8_t nSim,uint8_t dcs);

/*****************************************************************
* Function: ql_ss_ussd_get_dcs
*
* Description: Get user setting DCS
* 
* Parameters: 
*   nSim                [in]    SIM index,range: 0-1
*   dcs                 [out]   CBS Data Coding Scheme,see 3GPP TS 23.038
*                               The DCS decides in which format we send the USSD code.
* Return:
*   0           success
*   other       error code
* 
*****************************************************************/
ql_ss_errcode_e ql_ss_ussd_get_dcs(uint8_t nSim,uint8_t *dcs);

/*****************************************************************
* Function: ql_ss_ussd_get_session_state
*
* Description: Get current USSD session state.
* 
* Parameters: 
*   nSim                [in]    SIM index,range: 0-1
*   state               [out]   The state of USSD session
*                               True:Session is ongoing
*                               False:Session is end
* 
* Return:
*   0           success
*   other       error code
* 
*****************************************************************/
ql_ss_errcode_e ql_ss_ussd_get_session_state(uint8_t nSim,bool *state);

/*****************************************************************
* Function: ql_ss_register_cb
*
* Description: Register ss callback
* 
* Parameters: 
*   cb                  [in]   ss callback.Notice that it should keep in reentrancy
* 
* Return:
*   0           success
*   other       error code
* 
*****************************************************************/
ql_ss_errcode_e ql_ss_register_cb(ql_ss_callback_t cb);

/*****************************************************************
* Function: ql_ss_ussd_send
*
* Description: Initiate ussd session.
* 
* Parameters: 
*   nSim                [in]    SIM index,range: 0-1
*   option              [in]    see ql_ss_ussd_send_s
* 
* Return:
*   0           success
*   other       error code
* 
*****************************************************************/
ql_ss_errcode_e ql_ss_ussd_send(uint8_t nSim,ql_ss_ussd_send_s *ctx);


#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_SS_H*/