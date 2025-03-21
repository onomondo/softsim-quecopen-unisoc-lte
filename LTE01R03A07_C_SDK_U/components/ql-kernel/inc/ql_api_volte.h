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

#ifndef QL_API_VOLTE_H
#define QL_API_VOLTE_H


#include <stdint.h>
#include <stdbool.h>
#include "ql_api_common.h"
#include "ql_api_osi.h"

#ifdef __cplusplus
 extern "C" {
#endif


/*========================================================================
*  Marco Definition
*========================================================================*/
#define QL_VOLTE_LOG(msg, ...)   QL_LOG_TAG(QL_LOG_LEVEL_INFO, QL_LOG_TAG_QVOLTE,"ql_volte", msg, ##__VA_ARGS__)


 /*========================================================================
  *  Enumeration Definition
  *========================================================================*/
typedef enum
{
	QL_VOLTE_SUCCESS					= 0,
	QL_VOLTE_ERROR					    = 1 | (QL_COMPONENT_IMS << 16),
	QL_VOLTE_PARA_ERR,
    QL_VOLTE_FILE_ERR,
    QL_VOLTE_NO_MEMORY_ERR
}ql_volte_errcode_e;

typedef struct
{
    uint8_t volte_en;    /* disable or enable VOLTE */
	uint8_t reg_info;    /* register state, no set */
    uint8_t ext_info;    /* IMS capability, no set */
    uint8_t vdp;         /* voice domain preference */
    uint8_t usage;       /* usage of the UE */
} ql_volte_ctx_t;

/*****************************************************************
* Function: ql_volte_set
*
* Description:
*	Disable or enable VOLTE.
* 
* Parameters:
*   nSim	  [in] Index of Sim, range:0-1.
*	mode	  [in] 0:Disable VOLTE; 1:Enable VOLTE
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_set(uint8_t nSim, uint8_t mode);

/*****************************************************************
* Function: ql_volte_get
*
* Description:
*	get VoLTE status
* 
* Parameters:
*   nSim	  [in] 	Index of Sim, range:0-1.
*	mode	  [out] 0:Disable VOLTE; 1:Enable VOLTE
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_get(uint8_t nSim, uint8_t *mode);


/*****************************************************************
* Function: ql_volte_vdp_set
*
* Description:
*	set the specified voice domain preference
* 
* Parameters:
*   nSim	[in] 	Index of Sim, range:0-1.
*	setting [in] :
*   	1 CS Voice only 
*       2 CS Voice preferred, IMS PS Voice as secondary
*       3 IMS PS Voice preferred, CS Voice as secondary
*       4 IMS PS Voice only
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_vdp_set(uint8_t nSim, uint8_t setting);


/*****************************************************************
* Function: ql_volte_vdp_get
*
* Description:
*	get the specified voice domain preference
* 
* Parameters:
* Parameters:
*   nSim	[in]   Index of Sim, range:0-1.
*	setting [out] :
*   	1 CS Voice only 
*       2 CS Voice preferred, IMS PS Voice as secondary
*       3 IMS PS Voice preferred, CS Voice as secondary
*       4 IMS PS Voice only
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_vdp_get(uint8_t nSim, uint8_t *setting);


/*****************************************************************
* Function: ql_volte_usage_set
*
* Description:
*	set the usage of the UE
* 
* Parameters:
*   nSim	  [in] Index of Sim, range:0-1.
*	setting   [in] 0:voice centric   1:data centric
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_usage_set(uint8_t nSim, uint8_t setting);


/*****************************************************************
* Function: ql_volte_usage_get
*
* Description:
*	get the usage of the UE
* 
* Parameters:
*   nSim	  [in] 	Index of Sim, range:0-1.
*	setting   [out] 0:voice centric   1:data centric
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_usage_get(uint8_t nSim, uint8_t *setting);


/*****************************************************************
* Function: ql_volte_ctx_set
*
* Description:
*	set VOLTE context info.
* 
* Parameters:
*   nSim	      [in] 	Index of Sim, range:0-1.
*   volte_ctx	  [in]
*                   volte_en: disable or enable VOLTE
*                   vdp: voice domain preference
*                   usage: usage of the UE
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_ctx_set(uint8_t nSim, ql_volte_ctx_t *volte_ctx);


/*****************************************************************
* Function: ql_volte_ctx_get
*
* Description:
*	get VOLTE context info.
* 
* Parameters:
*   nSim	      [in] 	Index of Sim, range:0-1.
*	volte_ctx	  [out]
*                   volte_en: disable or enable VOLTE
*                   reg_info: 
*                       0 not registered
*                       1 registered
*                   ext_info:
*                       1 RTP-based transfer of voice according to MMTEL
*                       2 RTP-based transfer of text according to MMTEL
*                       4 SMS using IMS functionality
*                       8 RTP-based transfer of video according to MMTEL
*                   vdp: voice domain preference
*                   usage: usage of the UE
*
* Return:ql_volte_errcode_e
*****************************************************************/
ql_volte_errcode_e ql_volte_ctx_get(uint8_t nSim, ql_volte_ctx_t *volte_ctx);


#ifdef __cplusplus
 }/*"C" */
#endif


#endif

