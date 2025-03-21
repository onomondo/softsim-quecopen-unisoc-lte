/**  
  @file
  ql_api_xlat.h

  @brief
  This file provides the definitions for xlat, and declares the 
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

#ifndef QL_API_XLAT_H
#define QL_API_XLAT_H

#include <stdint.h>
#include <stdbool.h>
#include "sockets.h"
#include "ql_api_common.h"
#include "ql_api_osi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QL_DISABLE_FEATURE 	0
#define QL_ENABLE_FEATURE 	1
#define QL_ZERO_VALUE 		0
#define QL_NONZERO_VALUE 	1

#define QL_XLAT_DEFAULT_VALUE 	0 /*0: Off by default; 1: Turn on XLAT function*/

/*========================================================================
*  Type Definition
*========================================================================*/
typedef int ql_xlat_client_t;

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum{
	QL_XLAT_OPT_CLAT_STATUS, 						 
	QL_XLAT_OPT_DNS64_MODE,
	QL_XLAT_OPT_PREFIX_LEN, 						 
							 
	QL_XLAT_OPT_UL_IGNORE_V4_TOS,
	QL_XLAT_OPT_DL_V4_TOS, 						 
	QL_XLAT_OPT_NO_DF,
	QL_XLAT_OPT_UDP_ZERO_CKSUM, 

	QL_XLAT_OPT_ICMPV4_ERR_MODE,
	QL_XLAT_OPT_ICMPV6_ERR_MODE, 

	QL_XLAT_OPT_FQDN_DNS64,
	QL_XLAT_OPT_DEST_PREFIX,
}ql_xlat_option_e;								 

typedef enum{
	QL_XLAT_OK = 0,
	QL_XLAT_ERROR_INVALID_PARAM = (QL_COMPONENT_LWIP_XLAT << 16) | 1,
	QL_XLAT_ERROR_MEMORY,
	QL_XLAT_ERROR_UNKNOWN,
	
}ql_xlat_errcode_e;

/*****************************************************************
* Function: ql_xlat_get_enable
*
* Description:
*  Query the XLAT status
* 
* Parameters: 
*   code                [in]    reserved, unused, default NULL.
*
* Return:
*   0      fail, XLAT is not running;
*   1      success, XLAT has been run, CLAT enabled, PLAT supported.
*
*****************************************************************/
int ql_xlat_get_enable(int *code);

/*****************************************************************
* Function: ql_xlat_setopt
*
* Description: Set XLAT option.See ql_xlat_option_e.
*
* Parameters:
* id		       	   [in]    reserved, unused, default 1.
* opt_tag       	   [in]    Option type.See ql_xlat_option_e.
* val                  [in]    The address of input parameter.
* val_len              [in]    The length of input parameter.See ql_xlat_option_e.
*
* Return:
*  0           success
*  other       error code
*
*****************************************************************/
int ql_xlat_setopt(int id, int opt_tag, void *value, int len);

/*****************************************************************
* Function: ql_xlat_getopt
*
* Description: Get XLAT option.See ql_xlat_option_e.
*
* Parameters:
* id		       	   [in]    reserved, unused, default 1.
* opt_tag       	   [in]    Option type.See ql_xlat_option_e.
* val                  [in]    The address of output parameter.
* val_len              [in]    The length of output parameter.See ql_xlat_option_e.
*
* Return:
*  0           success
*  other       error code
*
*****************************************************************/
int ql_xlat_getopt(int id, int opt_tag, void *value, int len);

#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_XLAT_H*/


