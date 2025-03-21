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
2022.07.21  Joe.tu         create

=============================================================================*/
#ifndef QL_API_VSIM_ADAPT_H
#define QL_API_VSIM_ADAPT_H

#include <stdint.h>

#include "ql_api_common.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
#include "quec_atc_common.h"
#include "quec_sio_adapter.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
#define QL_VSIM_RSP_BUFFER_SIZE        (1500)
#endif


typedef enum
{
	QL_VSIM_ADAPT_SUCCESS					= 0,
	QL_VSIM_ADAPT_ERROR					    = 1 | (QL_COMPONENT_VSIM_ADAPT << 16),
	QL_VSIM_ADAPT_PARA_ERR,
}ql_vsim_adapt_errcode_e;

typedef enum
{
    QL_VSIM_ADAPT_SIM_TYPE_TSIM, /* physical sim card */
    QL_VSIM_ADAPT_SIM_TYPE_SSIM, /* local soft sim card */ 
    QL_VSIM_ADAPT_SIM_TYPE_CSIM, /* cloud sim card */
    QL_VSIM_ADAPT_SIM_TYPE_MAX,
} ql_vsim_adapt_sim_type_e;


typedef int (*ql_vsim_adapt_process_apdu)(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp,
                                     uint16_t *apdu_rsp_len, uint8_t nSim);

typedef uint16_t (*ql_vsim_adapt_reset)(uint8_t *atr_data, uint8_t *atr_size, uint8_t nSim);

typedef struct
{
    ql_vsim_adapt_process_apdu process_apdu;
    ql_vsim_adapt_reset reset;
}ql_vsim_adapt_handler_s;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
typedef struct
{
    atCmdEngine_t *engine;
    _AtciResultCode resultCode;
    uint32_t report_code;
    char* rsp_buffer; /* buffer size: QL_VSIM_RSP_BUFFER_SIZE */
    uint16_t rsp_len; /* 0:no return string other:return string length */
    uint8_t padding;
} ql_vsim_adapt_at_resp_s;

typedef struct
{
    atCommand_t *command;
    ql_vsim_adapt_at_resp_s* response;
} ql_vsim_adapt_at_message_s;
#endif

/*****************************************************************
* Function: ql_vsim_adapt_callback
*
* Description: 
*     QUEC_VSIM_ADAPT_POWERON_IND: to change sim type at start-up. Otherwise, physical sim card will be selected, ctx is null.
*     QUEC_VSIM_ADAPT_RDY_IND: only for STD AT solution, indicate that quectel core is ready, allowed to use related API, ctx is null.
* 
* Parameters:
*	ind_type  [in] indicate type, QUEC_VSIM_ADAPT_POWERON_IND or QUEC_VSIM_ADAPT_RDY_IND
*   ctx       [in] the context of specific indicate. 
*
* Return:ql_vsim_adapt_errcode_e
*****************************************************************/
typedef int (*ql_vsim_adapt_callback)(uint32_t ind_type, void *ctx);

ql_vsim_adapt_errcode_e ql_vsim_adapt_register_callback(ql_vsim_adapt_callback vsim_adapt_cb);

/*****************************************************************
* Function: ql_vsim_adapt_set_sim_type
*
* Description: 
*     change sim card type, this is a synchronous api, the thread which will be blocked when called
* 
* Parameters:
*	sim_type  [in] sim type, should be physical sim card, local soft sim card or cloud sim card
*   handler   [in] APDU and ATR function
*	nSim      [in] Index of Sim, range:0-1.
*
* Return:ql_vsim_adapt_errcode_e
*****************************************************************/
ql_vsim_adapt_errcode_e ql_vsim_adapt_set_sim_type(ql_vsim_adapt_sim_type_e sim_type,
                                                      const ql_vsim_adapt_handler_s *handler, uint8_t nSim);

/*****************************************************************
* Function: ql_vsim_adapt_get_sim_type
*
* Description: 
*     get sim card type
* 
* Parameters:
*	nSim      [in] Index of Sim, range:0-1.
*
* Return:ql_vsim_adapt_sim_type_e
*****************************************************************/
ql_vsim_adapt_sim_type_e ql_vsim_adapt_get_sim_type(uint8_t nSim);


#endif
