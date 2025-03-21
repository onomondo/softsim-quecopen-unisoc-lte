/**  
  @file
  ql_api_nw.h

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

=============================================================================*/

#ifndef QL_API_NW_H
#define QL_API_NW_H

#include <stdint.h>
#include <stdbool.h>
#include "ql_api_common.h"
#include "ql_api_datacall.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QL_NW_MCC_MAX_LEN 3                 //maximum length of MCC
#define QL_NW_MNC_MAX_LEN 3                 //maximum length of MNC
#define QL_NW_LONG_OPER_MAX_LEN 32          //maximum length of long operator name
#define QL_NW_SHORT_OPER_MAX_LEN 32         //maximum length of short operator name
#define QL_NW_RRCRel_MIN         0          //minimum lOCRRCRel TIME
#define QL_NW_RRCRel_MAX         20         //maximum lOCRRCRel TIME

#define QL_NW_CELL_MAX_NUM 7                //maxinum number of cell infomation (include serving cell and neighbour cell)

#define QL_NW_EDRX_VALUE_MAX 15
#define QL_NW_EDRX_PTW_MAX   15
#define QL_NW_EDRX_ACTTYPE   4              //E-UTRAN (WB-S1 mode)

#define ql_nw_get_emm_gmm_cause(ctx) ql_nw_get_emm_gmm_cause_ex(0, ctx)
#define ql_nw_get_esm_sm_cause(ctx) ql_nw_get_esm_sm_cause_ex(0, ctx)

#define QL_NW_LTE_AUTO_DISABLE_LIST_MAX_NUM  16
#define QL_NW_LTE_DISABLE_TIME_MAX    (60 * 24 * 7)  /* Unit:mins Max:7 days */
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/

typedef enum
{
    QL_NW_UE_STAT_NORMAL, /* UE state is normal */
    QL_NW_UE_STAT_DISNET, /* UE cannot register due to restricted areas */
    QL_NW_UE_STAT_CFUN,   /* UE is under cfun0 or cfun4 */
} ql_nw_ue_stat_e;

/*network mode type*/
typedef enum
{
    QL_NW_MODE_AUTO = 0,		            //  AUTO mode and LTE preferred
    QL_NW_MODE_GSM,    			            //  GSM only mode
    QL_NW_MODE_LTE,				            //  LTE only mode
}ql_nw_mode_type_e;

/*network cemode type, defined same with 3GPP 27.007*/
typedef enum
{
    QL_NW_PS_DATA     = 0,		                //  EPS Only, "data centric"
    QL_NW_CS_PS_VOICE = 1,    			        //  Combined EPS, "voice centric"
    QL_NW_PS_CS_DATA  = 2,                      //  Combined EPS, "data centric"
    QL_NW_PS_VOICE    = 3,				        //  EPS Only, "voice centric"
}ql_nw_cemode_type_e;

/*network access technology type, only support 0 and 7*/
typedef enum 
{
    QL_NW_ACCESS_TECH_GSM                 = 0,
    QL_NW_ACCESS_TECH_GSM_COMPACT         = 1,
    QL_NW_ACCESS_TECH_UTRAN               = 2,
    QL_NW_ACCESS_TECH_GSM_wEGPRS          = 3,
    QL_NW_ACCESS_TECH_UTRAN_wHSDPA        = 4,
    QL_NW_ACCESS_TECH_UTRAN_wHSUPA        = 5,
    QL_NW_ACCESS_TECH_UTRAN_wHSDPA_HSUPA  = 6,
    QL_NW_ACCESS_TECH_E_UTRAN             = 7,
}ql_nw_act_type_e;

/*network register status, equal to creg*/
typedef enum
{
    QL_NW_REG_STATE_NOT_REGISTERED=0,             //not registered, MT is not currently searching an operator to register to
    QL_NW_REG_STATE_HOME_NETWORK=1,               //registered, home network
    QL_NW_REG_STATE_TRYING_ATTACH_OR_SEARCHING=2, //not registered, but MT is currently trying to attach or searching an operator to register to
    QL_NW_REG_STATE_DENIED=3,                     //registration denied
    QL_NW_REG_STATE_UNKNOWN=4,                    //unknown
    QL_NW_REG_STATE_ROAMING=5,                    //registered, roaming
}ql_nw_reg_state_e;

typedef enum {
    QL_NO_JAMMING = 0,
	QL_JAMMED,
} ql_nw_jamdet_state_e;


typedef enum
{
	QL_NW_SUCCESS                   = 0,
    QL_NW_EXECUTE_ERR               = 1 | (QL_COMPONENT_NETWORK << 16),
    QL_NW_MEM_ADDR_NULL_ERR,         
    QL_NW_INVALID_PARAM_ERR,
    QL_NW_CFW_CFUN_GET_ERR,
    QL_NW_CFUN_DISABLE_ERR          = 5 | (QL_COMPONENT_NETWORK << 16),
    QL_NW_CFW_NW_STATUS_GET_ERR,
    QL_NW_NOT_SEARCHING_ERR,
    QL_NW_NOT_REGISTERED_ERR,
    QL_NW_CFW_GPRS_STATUS_GET_ERR,
    QL_GPRS_NOT_SEARCHING_ERR     = 10 | (QL_COMPONENT_NETWORK << 16),
    QL_GPRS_NOT_REGISTERED_ERR,
    QL_NW_CFW_NW_QUAL_GET_ERR,
    QL_NW_CFW_OPER_ID_GET_ERR,
    QL_NW_CFW_OPER_NAME_GET_ERR,
    QL_NW_CFW_OPER_SET_ERR        = 15 | (QL_COMPONENT_NETWORK << 16),
    QL_NW_SIM_ERR,
    QL_NW_NO_MEM_ERR,
    QL_NW_SEMAPHORE_CREATE_ERR,
    QL_NW_SEMAPHORE_TIMEOUT_ERR,
    QL_NW_NITZ_NOT_UPDATE_ERR     = 20 | (QL_COMPONENT_NETWORK << 16),
    QL_NW_CFW_EMOD_START_ERR,
    QL_NW_OPERATOR_NOT_ALLOWED,
    QL_NW_CFW_RRCRel_SET_ERR,
    QL_NW_BLACK_CELL_FULL_ERR,
    QL_NW_PLMNFORMAT_CONVERT_UNFOUND = 25 | (QL_COMPONENT_NETWORK << 16),
    QL_NW_CFW_NW_INFO_GET_ERR,
    QL_NW_PWR_NOT_SUPPORT_ERR        = 30 | (QL_COMPONENT_NETWORK << 16),
}ql_nw_errcode_e;

typedef enum 
{
    QL_NW_TAU_TYPE_COMBINED_TA_LA_UPDATING  = 1,
    QL_NW_TAU_TYPE_PERIODIC_UPDATING        = 3,
    QL_NW_TAU_TYPE_DEFAULT                  = QL_NW_TAU_TYPE_PERIODIC_UPDATING,	
} ql_nw_tau_type_e;

typedef enum
{
    QUEC_NW_CS_INTERNAL_CAUSE 	= 0,
    QUEC_NW_CS_NETWORK_CAUSE 	= 1,
    QUEC_NW_CS_NETWORK_REJECT	= 2,    //defined in 3GPP TS 24.008 Table 10.5.147/3GPP TS 24.008: GMM cause information element
    QUEC_NW_PS_INTERNAL_CAUSE	= 3,
    QUEC_NW_PS_NETWORK_CAUSE	= 4,
    QUEC_NW_PS_LTE_CAUSE		= 5,    //defined in 3GPP TS 24.301 Table 9.9.4.4.1: ESM cause information element
    QUEC_NW_PS_LTE_LOCAL_CAUSE	= 6,    //defined in 3GPP TS 24.301 Table 9.9.3.9.1: EMM cause information element
    QUEC_NW_PS_GPRS_CAUSE       = 7,    //defined in 3GPP TS 24.008 Table 10.5.157/3GPP TS 24.008: SM cause information element
    QUEC_NW_CAUSE_MAX,
}ql_nw_cause_type_e;

//defined in 3GPP TS 24.301 Table 9.9.3.9.1: EMM cause information element
typedef enum
{
    QUEC_EMM_CAUSE_IMSI_UNKOWN_IN_HSS                                   = 0x02,//0	0	0	0	0	0	1	0
    QUEC_EMM_CAUSE_ILLEGAL_UE                                           = 0x03,//0	0	0	0	0	0	1	1
    QUEC_EMM_CAUSE_IMEI_NOT_ACCEPTED                                    = 0x05,//0	0	0	0	0	1	0	1
    QUEC_EMM_CAUSE_ILLEGAL_ME                                           = 0x06,//0	0	0	0	0	1	1	0
    QUEC_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED                             = 0x07,//0	0	0	0	0	1	1	1
    QUEC_EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED        = 0x08,//0	0	0	0	1	0	0	0
    QUEC_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK         = 0x09,//0	0	0	0	1	0	0	1
    QUEC_EMM_CAUSE_IMPLICITLY_DETACHED                                  = 0x0a,//0	0	0	0	1	0	1	0
    QUEC_EMM_CAUSE_PLMN_NOT_ALLOWED                                     = 0x0b,//0	0	0	0	1	0	1	0
    QUEC_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED                            = 0x0c,//0	0	0	0	1	1	0	0
    QUEC_EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_PLMN                     = 0x0d,//0	0	0	0	1	1	0	1
    QUEC_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN                = 0x0e,//0	0	0	0	1	1	1	0
    QUEC_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA                   = 0x0f,//0	0	0	0	1	1	1	1
    QUEC_EMM_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE                        = 0x10,//0	0	0	1	0	0	0	0
    QUEC_EMM_CAUSE_NETWORK_FAILURE                                      = 0x11,//0	0	0	1	0	0	0	1
    QUEC_EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE                              = 0x12,//0	0	0	1	0	0	1	0
    QUEC_EMM_CAUSE_ESM_FAILURE                                          = 0x13,//0	0	0	1	0	0	1	1
    QUEC_EMM_CAUSE_MAC_FAILURE                                          = 0x14,//0	0	0	1	0	1	0	0
    QUEC_EMM_CAUSE_SYNCH_FAILURE                                        = 0x15,//0	0	0	1	0	1	0	1
    QUEC_EMM_CAUSE_CONGESTION                                           = 0x16,//0	0	0	1	0	1	1	0
    QUEC_EMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH                    = 0x17,//0	0	0	1	0	1	1	1
    QUEC_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED                   = 0x18,//0	0	0	1	1	0	0	0
    QUEC_EMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG                          = 0x19,//0	0	0	1	1	0	0	1
    QUEC_EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE                  = 0x1a,//0	0	0	1	1	0	1	0
    QUEC_EMM_CAUSE_REDIRECTION_TO_5GCN_REQUIRED                         = 0x1f,//0	0	0	1	1	1	1	1
    QUEC_EMM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN = 0x23,//0	0	1	0	0	0	1	1
    QUEC_EMM_CAUSE_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE                 = 0x27,//0	0	1	0	0	1	1	1
    QUEC_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED                      = 0x28,//0	0	1	0	1	0	0	0
    QUEC_EMM_CAUSE_SEVERE_NETWORD_FAILURE                               = 0x2a,//0	0	1	0	1	0	1	0
    QUEC_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE                       = 0x5f,//0	1	0	1	1	1	1	1
    QUEC_EMM_CAUSE_INVALID_MANDATORY_INFORMATION                        = 0x60,//0	1	1	0	0	0	0	0
    QUEC_EMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED         = 0x61,//0	1	1	0	0	0	0	1
    QUEC_EMM_CAUSE_MESSAGE_TYPE_NON_COMPATIBLE_WITH_THE_PROTOCOL_STATE  = 0x62,//0	1	1	0	0	0	1	1
    QUEC_EMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED  = 0x63,//0	1	1	0	0	0	1	1
    QUEC_EMM_CAUSE_CONDITIONAL_IE_ERROR                                 = 0x64,//0	1	1	0	0	1	0	0
    QUEC_EMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE       = 0x65,//0	1	1	0	0	1	0	1
    QUEC_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED                           = 0x6f,//0	1	1	0	1	1	1	1
}quec_ps_lte_local_cause_e;

//defined in 3GPP TS 24.008 Table 10.5.147/3GPP TS 24.008: GMM cause information element
typedef enum
{
    QUEC_GMM_CAUSE_IMSI_UNKOWN_IN_HLR                                   = 0x02,//0	0	0	0	0	0	1	0
    QUEC_GMM_CAUSE_ILLEGAL_MS                                           = 0x03,//0	0	0	0	0	0	1	1
    QUEC_GMM_CAUSE_IMSI_UNKOWN_IN_VLR                                   = 0x04,//0	0	0	0	0	0	1	1  /* Adapts Agilent 8960 instrument test */
    QUEC_GMM_CAUSE_IMEI_NOT_ACCEPTED                                    = 0x05,//0	0	0	0	0	1	0	1
    QUEC_GMM_CAUSE_ILLEGAL_ME                                           = 0x06,//0	0	0	0	0	1	1	0
    QUEC_GMM_CAUSE_GPRS_SERVICES_NOT_ALLOWED                            = 0x07,//0	0	0	0	0	1	1	1
    QUEC_GMM_CAUSE_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED      = 0x08,//0	0	0	0	1	0	0	0
    QUEC_GMM_CAUSE_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK         = 0x09,//0	0	0	0	1	0	0	1
    QUEC_GMM_CAUSE_IMPLICITLY_DETACHED                                  = 0x0a,//0	0	0	0	1	0	1	0
    QUEC_GMM_CAUSE_PLMN_NOT_ALLOWED                                     = 0x0b,//0	0	0	0	1	0	1	1
    QUEC_GMM_CAUSE_LOCATION_AREA_NOT_ALLOWED                            = 0x0c,//0	0	0	0	1	1	0	0
    QUEC_GMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_PLMN                     = 0x0d,//0	0	0	0	1	1	0	1
    QUEC_GMM_CAUSE_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN               = 0X0e,//0	0	0	0	1	1	1	0
    QUEC_GMM_CAUSE_NO_SUITABLE_CELLS_IN_LOCATION_AREA                   = 0x0f,//0	0	0	0	1	1	1	1
    QUEC_GMM_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE                        = 0x10,//0	0	0	1	0	0	0	0
    QUEC_GMM_CAUSE_NETWORK_FAILURE                                      = 0x11,//0	0	0	1	0	0	0	1
    QUEC_GMM_CAUSE_MAC_FAILURE                                          = 0x14,//0	0	0	1	0	1	0	0
    QUEC_GMM_CAUSE_SYNCH_FAILURE                                        = 0x15,//0	0	0	1	0	1	0	1
    QUEC_GMM_CAUSE_CONGESTION                                           = 0x16,//0	0	0	1	0	1	1	0
    QUEC_GMM_CAUSE_GSM_AUTHENTICATION_UNACCEPTABLE                      = 0x17,//0	0	0	1	0	1	1	1
    QUEC_GMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG                          = 0x19,//0	0	0	1	1	0	0	1
    QUEC_GMM_CAUSE_SMS_PROVIDED_VIA_GPRS_IN_THIS_ROUTING_AREA           = 0x1c,//0	0	0	1	1	1	0	0
    QUEC_GMM_SERVICE_OPTION_NOT_SUPPORTED                               = 0x20,                                /* Adapts Agilent 8960 instrument test */
    QUEC_GMM_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED                    = 0x21,                                /* Adapts Agilent 8960 instrument test */
    QUEC_GMM_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER                    = 0x22,                                /* Adapts Agilent 8960 instrument test */
    QUEC_GMM_CAUSE_NO_PDP_CONTEXT_ACTIVATED                             = 0x28,//0	0	1	0	1	0	0	0
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_0                   = 0x30,//0	0	1	1	0	0	0	0
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_1                   = 0x31,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_2                   = 0x32,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_3                   = 0x33,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_4                   = 0x34,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_5                   = 0x35,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_6                   = 0x36,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_7                   = 0x37,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_8                   = 0x38,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_9                   = 0x39,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_10                  = 0x3a,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_11                  = 0x3b,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_12                  = 0x3c,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_13                  = 0x3d,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_14                  = 0x3e,//
    QUEC_GMM_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_15                  = 0x3f,//0	0	1	1	1	1	1	1
    QUEC_GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE                       = 0x5f,//0	1	0	1	1	1	1	1
    QUEC_GMM_CAUSE_INVALID_MANDATORY_INFORMATION                        = 0x60,//0	1	1	0	0	0	0	0
    QUEC_GMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED         = 0x61,//0	1	1	0	0	0	0	1
    QUEC_GMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE  = 0x62,//0	1	1	0	0	0	1	0
    QUEC_GMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED  = 0x63,//0	1	1	0	0	0	1	1
    QUEC_GMM_CAUSE_CONDITIONAL_IE_ERROR                                 = 0x64,//0	1	1	0	0	1	0	0
    QUEC_GMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE       = 0x65,//0	1	1	0	0	1	0	1
    QUEC_GMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED                           = 0x6f,//0	1	1	0	1	1	1	1
}quec_cs_network_reject_e;

//defined in 3GPP TS 24.301 Table 9.9.4.4.1: ESM cause information element
typedef enum
{
    QUEC_ESM_CAUSE_OPERATOR_DETERMINED_BARRING	                              	      	=0x08,
    QUEC_ESM_CAUSE_INSUFFICIENT_RESOURCES	                                 	       	=0x1A,
    QUEC_ESM_CAUSE_MISSING_OR_UNKNOWN_APN	                                  	      	=0x1B,
    QUEC_ESM_CAUSE_UNKNOWN_PDN_TYPE	                                            	    =0x1C,
    QUEC_ESM_CAUSE_USER_AUTHENTICATION_FAILED	                                    	=0x1D,
    QUEC_ESM_CAUSE_REQUEST_REJECTED_BY_SERVING_GW_OR_PDN_GW	                       	 	=0x1E,
    QUEC_ESM_CAUSE_REQUEST_REJECTED_UNSPECIFIED	                                    	=0x1F,
    QUEC_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED	                                    	=0x20,
    QUEC_ESM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED	            	            =0x21,
    QUEC_ESM_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER	            	            =0x22,
    QUEC_ESM_CAUSE_PTI_ALREADY_IN_USE	                                            	=0x23,
    QUEC_ESM_CAUSE_REGULAR_DEACTIVATION	                                            	=0x24,
    QUEC_ESM_CAUSE_EPS_QOS_NOT_ACCEPTED	                                  	          	=0x25,
    QUEC_ESM_CAUSE_NETWORK_FAILURE	                                                	=0x26,
    QUEC_ESM_CAUSE_REACTIVATION_REQUESTED	                                	        =0x27,
    QUEC_ESM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION	                    	        =0x29,
    QUEC_ESM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION	                 	        =0x2A,
    QUEC_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY	                                    	=0x2B,
    QUEC_ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS	                            	=0x2C,
    QUEC_ESM_CAUSE_SYNTACTICAL_ERRORS_IN_PACKET_FILTERS	                            	=0x2D,
    QUEC_ESM_CAUSE_UNUSED	                                                        	=0x2E,
    QUEC_ESM_CAUSE_PTI_MISMATCH	                                                    	=0x2F,
    QUEC_ESM_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED	                            	=0x31,
    QUEC_ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED	                                    	=0x32,
    QUEC_ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED	                                    	=0x33,
    QUEC_ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED	                            	=0x34,
    QUEC_ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED                                			=0x35,
    QUEC_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST	                                	=0x36,
    QUEC_ESM_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED	            	=0x37,
    QUEC_ESM_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST	                        	=0x38,
    QUEC_ESM_CAUSE_PDN_TYPE_IPV4V6_ONLY_ALLOWED	                                    	=0x39,
    QUEC_ESM_CAUSE_PDN_TYPE_NON_IP_ONLY_ALLOWED	                                    	=0x3A,
    QUEC_ESM_CAUSE_UNSUPPORTED_QCI_VALUE	                                        	=0x3B,
    QUEC_ESM_CAUSE_BEARER_HANDLING_NOT_SUPPORTED	                                	=0x3C,
    QUEC_ESM_CAUSE_PDN_TYPE_ETHERNET_ONLY_ALLOWED	                                	=0x3D,
    QUEC_ESM_CAUSE_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED	                        	=0x41,
    QUEC_ESM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION		=0x42,
    QUEC_ESM_CAUSE_INVALID_PTI_VALUE	                                            	=0x51,
    QUEC_ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE	                                	=0x5F,
    QUEC_ESM_CAUSE_INVALID_MANDATORY_INFORMATION	                                	=0x60,
    QUEC_ESM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED	                    	=0x61,
    QUEC_ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE	            	=0x62,
    QUEC_ESM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED	            	=0x63,
    QUEC_ESM_CAUSE_CONDITIONAL_IE_ERROR	                                            	=0x64,
    QUEC_ESM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE	                    =0x65,
    QUEC_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED	                                        =0x6F,
    QUEC_ESM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_EPS_BEARER_CONTEXT	=0x70,
    QUEC_ESM_CAUSE_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED	                =0x71,
}quec_ps_lte_cause_e;

//defined in 3GPP TS 24.008 Table 10.5.157/3GPP TS 24.008: SM cause information element
typedef enum
{
    QUEC_SM_OPERATOR_DETERMINED_BARRING                                                 =0x08,
    QUEC_SM_MBMS_BEARER_CAPABILITIES_INSUFFICIENT_FOR_THE_SERVICE                       =0x18,
    QUEC_SM_LLC_OR_SNDCP_FAILURE_A_GB_MODE_ONLY                                         =0x19,
    QUEC_SM_INSUFFICIENT_RESOURCES                                                      =0x1A,
    QUEC_SM_MISSING_OR_UNKNOWN_APN                                                      =0x1B,
    QUEC_SM_UNKNOWN_PDP_ADDRESS_OR_PDP_TYPE                                             =0x1C,
    QUEC_SM_USER_AUTHENTICATION_FAILED                                                  =0x1D,
    QUEC_SM_ACTIVATION_REJECTED_BY_GGSN_SERVING_GW_OR_PDN_GW                            =0x1E,
    QUEC_SM_ACTIVATION_REJECTED_UNSPECIFIED                                             =0x1F,
    QUEC_SM_SERVICE_OPTION_NOT_SUPPORTED                                                =0x20,
    QUEC_SM_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED                                     =0x21,
    QUEC_SM_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER                                     =0x22,
    QUEC_SM_NSAPI_ALREADY_USED_NOT_SENT                                                 =0x23,
    QUEC_SM_REGULAR_DEACTIVATION                                                        =0x24,
    QUEC_SM_QOS_NOT_ACCEPTED                                                            =0x25,
    QUEC_SM_NETWORK_FAILURE                                                             =0x26,
    QUEC_SM_REACTIVATION_REQUESTED                                                      =0x27,
    QUEC_SM_FEATURE_NOT_SUPPORTED                                                       =0x28,
    QUEC_SM_SEMANTIC_ERROR_IN_THE_TFT_OPERATION                                         =0x29,
    QUEC_SM_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION                                      =0x2A,
    QUEC_SM_UNKNOWN_PDP_CONTEXT                                                         =0x2B,
    QUEC_SM_SEMANTIC_ERRORS_IN_PACKET_FILTER_S                                          =0x2C,
    QUEC_SM_SYNTACTICAL_ERRORS_IN_PACKET_FILTER_S                                       =0x2D,
    QUEC_SM_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED                                   =0x2E,
    QUEC_SM_MULTICAST_GROUP_MEMBERSHIP_TIME_OUT                                         =0x2F,
    QUEC_SM_REQUEST_REJECTED_BCM_VIOLATION                                              =0x30,
    QUEC_SM_PDP_TYPE_IPV4_ONLY_ALLOWED                                                  =0x32,
    QUEC_SM_PDP_TYPE_IPV6_ONLY_ALLOWED                                                  =0x33,
    QUEC_SM_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED                                         =0x34,
    QUEC_SM_COLLISION_WITH_NETWORK_INITIATED_REQUEST                                    =0x38,
    QUEC_SM_PDP_TYPE_IPV4V6_ONLY_ALLOWED                                                =0x39,
    QUEC_SM_PDP_TYPE_NON_IP_ONLY_ALLOWED                                                =0x3A,
    QUEC_SM_BEARER_HANDLING_NOT_SUPPORTED                                               =0x3C,
    QUEC_SM_MAXIMUM_NUMBER_OF_PDP_CONTEXTS_REACHED                                      =0x41,
    QUEC_SM_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION             =0x42,
    QUEC_SM_INVALID_TRANSACTION_IDENTIFIER_VALUE                                        =0x51,
    QUEC_SM_SEMANTICALLY_INCORRECT_MESSAGE                                              =0x5F,
    QUEC_SM_INVALID_MANDATORY_INFORMATION                                               =0x60,
    QUEC_SM_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED                                =0x61,
    QUEC_SM_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE                         =0x62,
    QUEC_SM_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED                         =0x63,
    QUEC_SM_CONDITIONAL_IE_ERROR                                                        =0x64,
    QUEC_SM_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE                              =0x65,
    QUEC_SM_PROTOCOL_ERROR_UNSPECIFIED                                                  =0x6F,
    QUEC_SM_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_PDP_CONTEXT                  =0x70,
    QUEC_SM_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED                           =0x71,
}quec_ps_gprs_cause_e;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET
 typedef enum
 {
	 JDCFG_ENUM_NONE = -1,
	 JDENABLE		  = 0,	 
	 JDNSIM,		
	 JDURC, 				 
	 JDPERIOD,					
	 JDMINCH,		//GSM channels num,range value:0~254		 
	 JDSINR,		//GSM SINR,range value:0~63	   
	 JDRSSI_GSM,	//GSM RSSI threshold,range value:-110 ~ -50								  
	 JDRSRP,		//lte RSRP threshold,range value:-140 ~ -44			 
	 JDRSRQ,		//lte RSRQ threshold,range value:-19 ~ -3		 
	 JDRSSI,		//LTE RSSI threshold,range value:-120 ~ -20										 
	 JDCFG_ENUM_MAX 		  
 }quectel_jdcfg_e_t;
#endif

typedef enum
{
    QL_EDRX_MODE_DISABLE = 0,   //Disable the use of requested PTW and eDRX feature
    QL_EDRX_MODE_ENABLE = 1,    //Enable the use of requested PTW and eDRX feature
    QL_EDRX_MODE_ENABLE_URC = 2,//Enable the use of requested PTW and eDRX, and enable the URC
    QL_EDRX_MODE_DISABLE_RESET_PARAM = 3,//Disable the use of eDRX and discard all eDRX parameters or,reset to the default values
}ql_edrx_mode_e;

typedef enum
{
    QL_NW_NO_BAND = 0,
    QL_NW_BAND_GSM_900    = 0x01,
    QL_NW_BAND_GSM_1800   = 0x02,
    QL_NW_BAND_GSM_850    = 0x04,
    QL_NW_BAND_GSM_1900   = 0x08,
}ql_nw_band_e;

typedef enum
{
    QUEC_PWR_BACKOFF_NONE = -1,
    QUEC_PWR_BACKOFF_GSM850 = 0,
    QUEC_PWR_BACKOFF_GSM900,
    QUEC_PWR_BACKOFF_GSM1800,
    QUEC_PWR_BACKOFF_GSM1900,

    QUEC_PWR_BACKOFF_LTE_BAND1 = 0x101,
    QUEC_PWR_BACKOFF_LTE_BAND2 = 0x102,
    QUEC_PWR_BACKOFF_LTE_BAND3 = 0x103,
    QUEC_PWR_BACKOFF_LTE_BAND4 = 0x104,
    QUEC_PWR_BACKOFF_LTE_BAND5 = 0x105,
    QUEC_PWR_BACKOFF_LTE_BAND7 = 0x107,
    QUEC_PWR_BACKOFF_LTE_BAND8 = 0x108,
    QUEC_PWR_BACKOFF_LTE_BAND12 = 0x10C,
    QUEC_PWR_BACKOFF_LTE_BAND13 = 0x10D,
    QUEC_PWR_BACKOFF_LTE_BAND17 = 0x111,
    QUEC_PWR_BACKOFF_LTE_BAND18 = 0x112,
    QUEC_PWR_BACKOFF_LTE_BAND19 = 0x113,
    QUEC_PWR_BACKOFF_LTE_BAND20 = 0x114,
    QUEC_PWR_BACKOFF_LTE_BAND25 = 0x119,
    QUEC_PWR_BACKOFF_LTE_BAND26 = 0x11A,
    QUEC_PWR_BACKOFF_LTE_BAND28 = 0x11C,
    QUEC_PWR_BACKOFF_LTE_BAND34 = 0x122,
    QUEC_PWR_BACKOFF_LTE_BAND38 = 0x126,
    QUEC_PWR_BACKOFF_LTE_BAND39 = 0x127,
    QUEC_PWR_BACKOFF_LTE_BAND40 = 0x128,
    QUEC_PWR_BACKOFF_LTE_BAND41 = 0x129,
    QUEC_PWR_BACKOFF_LTE_BAND66 = 0x142,
    QUEC_PWR_BACKOFF_LTE_BANDx,
    QUEC_PWR_BACKOFF_MAX
} quec_pwr_backoff_mode; 

typedef struct
{
    quec_pwr_backoff_mode mode;
    int16_t limit_min;
    int16_t limit_max;
}ql_nw_power_t;

typedef struct
{
    int64_t gsmbandval;
    int64_t wcdmabandval;
    int64_t ltebandval;
    int64_t lteextbandval;
}ql_nw_band_info_t;
/*========================================================================
*  Type Definition
*========================================================================*/

/*network operator information*/
typedef struct
{
    char long_oper_name[QL_NW_LONG_OPER_MAX_LEN+1];
    char short_oper_name[QL_NW_SHORT_OPER_MAX_LEN+1];
    char mcc[QL_NW_MCC_MAX_LEN+1];
    char mnc[QL_NW_MNC_MAX_LEN+1];
}ql_nw_operator_info_s;

typedef struct
{
	ql_nw_reg_state_e   state;              //network register state
	int                 lac;                //location area code
	int                 cid;                //cell ID
	ql_nw_act_type_e    act;                //access technology
    ql_nw_ue_stat_e     ue_state;           //ue state
}ql_nw_common_reg_status_info_s;

typedef struct
{
    uint32_t band;
    uint32_t freq;
    union
    {
        struct
        {
            int rsrp;
            int rsrq;
            int rssi;
            int sinr;
            uint16_t pci;
            uint16_t tdd_fdd; //0:TDD 1:FDD
            uint16_t srxlev;
            int earfcn;
            int tac;
        }lte;

        struct
        {
            int rla;
            uint16_t bsic;
        }gsm;
    }info;
}ql_nw_scell_info_s;

typedef struct
{
	ql_nw_common_reg_status_info_s   voice_reg;         //voice register info
	ql_nw_common_reg_status_info_s   data_reg;          //data register info
    ql_nw_scell_info_s scell_info;
}ql_nw_reg_status_info_s;

typedef struct
{
    int flag;                               //Cell type, 0:serving, 1:neighbor
    int cid;                                //Cell ID, (0 indicates that the cellid is not received)
    int mcc;                                
    int mnc;                                
    int lac;                                //Location area code
    int arfcn;                              //Absolute RF channel number
    char bsic;                              //Base station identity code. (0 indicates information is not present)
	int rssi;		                        //Receive signal strength, Value range: rxlev-111 for dbm format
	char mnc_len;
	char RX_dBm;                            //Received power
}ql_nw_gsm_cell_info_s;

typedef struct
{
    int flag;                               //Cell type, 0:serving, 1:neighbor
    int cid;                                //Cell ID, (0 indicates that the cellid is not received)
    int mcc;                                
    int mnc;                                
    int tac;                                //Tracing area code
    int pci;                                //Physical cell ID
    int earfcn;                             //E-UTRA absolute radio frequency channel number of the cell. RANGE: 0 TO 65535
	int rssi;		                        //Receive signal strength, Value range: rsrp-140 for dbm format
	char mnc_len;
	char RX_dBm;                            //Received power
}ql_nw_lte_cell_info_s;

typedef struct 
{
    int                     gsm_info_valid;                     //0: GSM cell information is invalid   1: GSM cell information is valid
    int                     gsm_info_num;                       //GSM cell number
    ql_nw_gsm_cell_info_s   gsm_info[QL_NW_CELL_MAX_NUM];       //GSM cell information (Serving and neighbor)
    int                     lte_info_valid;                     //0: LTE cell information is invalid   1: LTE cell information is valid
    int                     lte_info_num;                       //LTE cell number
    ql_nw_lte_cell_info_s   lte_info[QL_NW_CELL_MAX_NUM];       //LTE cell information (Serving and neighbor)
}ql_nw_cell_info_s;

typedef struct 
{
    unsigned char     nw_selection_mode;                        //0 auto select operator    1 manual select operator
    char              mcc[QL_NW_MCC_MAX_LEN+1];                 //String format
    char              mnc[QL_NW_MNC_MAX_LEN+1];                 //String format    eg:China Mobile -----> mcc="460"   mnc="00"
    ql_nw_act_type_e  act;                                      //access technology
}ql_nw_seclection_info_s;

typedef struct
{
    int rssi;                                   // received signal strength level (return 99 indicates that not known or not detectable)
    int bitErrorRate;                           // channel bit error rate (return 99 indicates that not known or not detectable)
    int rsrq;                                   // reference signal received quality (return 255 indicates that not known or not detectable)
    int rsrp;				                    // reference signal received power (return 255 indicates that not known or not detectable) 
}ql_nw_signal_strength_info_s;

typedef struct 
{
	char      nitz_time[32];                      //string format: YY/MM/DD HH:MM:SS '+/-'TZ daylight,   20/09/25 07:40:18 +32 00.
	long      abs_time;                           //Numeric format of NITZ time    0 is unavailable
}ql_nw_nitz_time_info_s;

typedef struct
{
	uint64_t uplink_pdp_count[PROFILE_IDX_MAX];    //uplink_pdp_count[0-6], pdp 1-7
	uint64_t downlink_pdp_count[PROFILE_IDX_MAX];  //downlink_pdp_count[0-6], pdp 1-7
}ql_pdp_data_count_info_s;

typedef struct
{
	uint64_t uplink_data_count;
	uint64_t downlink_data_count; 
	ql_pdp_data_count_info_s pdp_data_count;
}ql_nw_data_count_info_s;

typedef struct
{
    uint8_t  enable;  //0:disable; 1:enable
    uint32_t time;    //punishment duration. 0:permanent; range:0-255
}ql_auto_black_elecfence_cfg_s;

typedef struct
{
    ql_nw_cause_type_e causeId;
    int causeDetail;
}ql_nw_cause_s;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET
typedef struct 
{
	quectel_jdcfg_e_t	qjdcfg_enum;
	int 				value;
}quec_jamdet_param_set_s;

typedef struct 
{
	uint8_t shakeperiod;
}quec_jamdetex_param_set_s;

#endif

typedef struct 
{
    ql_edrx_mode_e mode;//wr.
    uint8_t actType;    //wr. only support value 4 - E-UTRAN (WB-S1 mode)
    uint8_t reqPTW;     //wr. range 0-15
    uint8_t reqEDRX;    //wr. range 0-15
    uint8_t nwPTW;      //read only. range 0-15
    uint8_t nwEDRX;     //read only. range 0-15
    uint8_t sib1_edrx_enable;//read only.
}ql_edrx_cfg_s;

typedef struct 
{
    uint8_t disrplmn_enable; // 0:use RPLMN when searching network 1: escape RPLMAN when searching network
}ql_nw_disrplmn_cfg_s;

typedef struct 
{
    char mcc[QL_NW_MCC_MAX_LEN + 1];
    char mnc[QL_NW_MNC_MAX_LEN + 1];
}ql_sim_plmn_s;

typedef struct
{
    uint8_t enable;         /* Function enable. */
    uint32_t expire_time;   /* Function disable time.Max:QL_NW_LTE_DISABLE_TIME_MAX */
    uint8_t roaming;        /* Enable/disable in roming */
    uint8_t reject_max;     /* Disable LTE when reach the reject max. */
    uint8_t plmn_num;
    ql_sim_plmn_s plmn[QL_NW_LTE_AUTO_DISABLE_LIST_MAX_NUM];
}ql_nw_lte_auto_disable_s;
/*========================================================================
 *	function Definition
 *========================================================================*/

typedef void (*ql_nw_callback)(uint8_t nSim, unsigned int ind_type, void *ctx);

/*****************************************************************
* Function: ql_nw_get_csq
*
* Description: 获取csq信号强度
* 
* Parameters:
*	nSim        [in]    SIM卡索引，取值：0-1.
* 	csq	  		[out] 	返回csq信号强度信息，范围（0-31），返回 99 为无效值。
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_csq(uint8_t nSim, unsigned char *csq);

/*****************************************************************
* Function: ql_nw_set_mode
*
* Description: 配置网络制式
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	nw_mode	  		[in] 	需要设置的网络制式，见ql_nw_mode_type_e
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_mode(uint8_t nSim, ql_nw_mode_type_e nw_mode);

/*****************************************************************
* Function: ql_nw_get_mode
*
* Description: 获取当前设置的网络制式
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	nw_mode	  		[out] 	返回设置的网络制式
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_mode(uint8_t nSim, ql_nw_mode_type_e *nw_mode);

/*****************************************************************
* Function: ql_nw_set_cemode
*
* Description: 配置EPS操作模式
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	cemode	  		[in] 	需要设置的EPS操作模式，见ql_nw_cemode_type_e
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_cemode(uint8_t nSim, ql_nw_cemode_type_e cemode);

/*****************************************************************
* Function: ql_nw_get_cemode
*
* Description: 获取当前设置的EPS操作制式
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	cemode	  		[out] 	返回设置的EPS操作模式
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_cemode(uint8_t nSim, ql_nw_cemode_type_e *cemode);

/*****************************************************************
* Function: ql_nw_get_rrc_duration
*
* Description: 获取当前LTE网络RRC释放时间
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	rrc_duration_value_get	  		[out] 	返回LTE网络RRC释放时间,单位ms
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_rrc_duration(uint8_t nSim, uint32_t *rrc_duration_value_get);

/*****************************************************************
* Function: ql_nw_get_drxtm
*
* Description: 获取UE特定DRX/网络DRX时间
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	drx_timer_value_get	  		[out] 	当UE特定DRX设置为0时,返回网络DRX时间,不为0时,则返回UE特定DRX,单位ms
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
* 备注: 如UE特定DRX不为0，则实际的DRX周期取UE特定DRX和网络DRX两者中的较小值
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_drxtm(uint8_t nSim, uint16_t *drx_timer_value_get);

/*****************************************************************
* Function: ql_nw_set_drxtm
*
* Description: 设置UE特定DRX时间
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1.
* 	drx_timer_value_set	  		[in] 	设置UE特定的DRX时间,只支持0,320,640,1280,2560,其中0表示使用网络配置,单位ms
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_drxtm(uint8_t nSim, uint16_t drx_timer_value_set);

/*****************************************************************
* Function: ql_nw_get_operator_name
*
* Description: 获取当前注网的运营商信息，需要等待注网成功后才可获取到
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	oper_info	  		[out] 	获取运营商信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_operator_name(uint8_t nSim, ql_nw_operator_info_s *oper_info);

/*****************************************************************
* Function: ql_nw_get_reg_status
*
* Description: 获取当前网络注册信息。
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	reg_info	  		[out] 	获取当前网络注册信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_reg_status(uint8_t nSim, ql_nw_reg_status_info_s *reg_info);

/*****************************************************************
* Function: ql_nw_set_selection
*
* Description: 设置运营商的选择，同步API,最长等待120秒，取决于网络
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	select_info	  		[in] 	选择的运营商信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_selection(uint8_t nSim, ql_nw_seclection_info_s *select_info);

/*****************************************************************
* Function: ql_nw_get_selection
*
* Description: 获取选择的运营商信息，需要等待注网成功
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	select_info	  		[out] 	获取选择的运营商信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_selection(uint8_t nSim, ql_nw_seclection_info_s *select_info);

/*****************************************************************
* Function: ql_nw_get_signal_strength
*
* Description: 获取详细信号强度信息
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[out] 	获取的详细信号强度信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_signal_strength(uint8_t nSim, ql_nw_signal_strength_info_s *pt_info);

/*****************************************************************
* Function: ql_nw_get_nitz_time_info
*
* Description: 获取当前基站时间，此时间只在注网成功的那一刻更新，需要当地网络支持
* 
* Parameters:
* 	nitz_info	  		[out] 	返回 modem 从基站获取的时间信息
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_nitz_time_info(ql_nw_nitz_time_info_s *nitz_info);

/*****************************************************************
* Function: ql_nw_get_cell_info
*
* Description: 获取当前服务及邻近小区信息。同步API,最长等待20秒
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	cell_info	  		[out] 	返回当前服务及邻近小区信息。
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_cell_info(uint8_t nSim, ql_nw_cell_info_s *cell_info);

/*****************************************************************
* Function: ql_nw_get_cell_info_async
*
* Description: 获取当前服务及邻近小区信息。异步API,查询的信息通过
                QUEC_NW_CELL_INFO_IND事件返回。
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_cell_info_async(uint8_t nSim);

/*****************************************************************
* Function: ql_nw_register_cb
*
* Description: 注册事件回调函数
* 
* Parameters:
* 	nw_cb	  		[in] 	事件回调函数。
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_nw_errcode_e ql_nw_register_cb(ql_nw_callback nw_cb);

/*****************************************************************
* Function: ql_nw_set_jamdet_mode
*
* Description: enable or disable jamming detection function
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* enable	  		[in]	0: disable	1:enable
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET
ql_nw_errcode_e ql_nw_set_jamdet_mode(uint8_t nSim, uint8_t enable);

/*****************************************************************
* Function: ql_nw_get_jamdet_mode
*
* Description: whether the jamming detection function is enabled or not
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* enable	  		[out]	0: disabled	1:enabled
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_jamdet_mode(uint8_t nSim, uint8_t *enable);

/*****************************************************************
* Function: ql_nw_get_jamdet_status
*
* Description: get jamming detection status
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* enable	  		[out]	0: no jamming	1:jammed
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_jamdet_status(uint8_t nSim, ql_nw_jamdet_state_e *status);

/*****************************************************************
* Function: ql_nw_set_jamdet_config
*
* Description: set jamming detection config
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* config	  		[in]	jamming config，Points to an array,the array length is JDCFG_ENUM_MAX
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/

ql_nw_errcode_e ql_nw_set_jamdet_config(uint8_t nSim, quec_jamdet_param_set_s *config);

/*****************************************************************
* Function: ql_nw_set_jamdetex_config
*
* Description: set ext jamming detection config
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* config	  		[in]	ext jamming config
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/

ql_nw_errcode_e ql_nw_set_jamdetex_config(uint8_t nSim, quec_jamdetex_param_set_s *config);

/*****************************************************************
* Function: ql_nw_get_jamdet_config
*
* Description: get jamming detection config
* 
* Parameters:
* config	  		[out]	jamming config，Points to an array,the array length is JDCFG_ENUM_MAX
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_jamdet_config(uint8_t nSim,quec_jamdet_param_set_s *config);

/*****************************************************************
* Function: ql_nw_get_jamdetex_config
*
* Description: get ext jamming detection config
* 
* Parameters:
* config	  		[out]	ext jamming config，Points to an array,the array length is JDCFGEX_ENUM_MAX
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_jamdetex_config(uint8_t nSim,quec_jamdetex_param_set_s *config);

#endif
/*****************************************************************
* Function: ql_nw_get_data_count
*
* Description: get uplink and downlink data count
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* data_info	  		[out]	data count
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_data_count(uint8_t nSim, ql_nw_data_count_info_s * data_info);

/*****************************************************************
* Function: ql_nw_reset_data_count
*
* Description: reset uplink and downlink data count
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_reset_data_count(uint8_t nSim);

/*****************************************************************
* Function: ql_nw_set_auto_black_elec_fence
*
* Description: Enable/Disable automatic black electronic fence
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* cfg               [in]    config of automatic black electronic fence
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_auto_black_elec_fence(uint8_t nSim, ql_auto_black_elecfence_cfg_s *cfg);

/*****************************************************************
* Function: ql_nw_get_auto_black_elec_fence
*
* Description: Get the parameters of automatic black electronic fence
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* cfg               [out]   config of automatic black electronic fence
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_auto_black_elec_fence(uint8_t nSim, ql_auto_black_elecfence_cfg_s *cfg);

/*****************************************************************
* Function: ql_nw_get_cell_sinr
*
* Description: get cell sinr
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* sinr_ptr	  		[out]	sinr
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_cell_sinr(uint8_t nSim, int *sinr_ptr);

/*****************************************************************
* Function: ql_nw_set_tau_type_aft_local_rel
*
* Description: set TAU type after local RRC release
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* type	  		    [in]	TAU type
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_tau_type_aft_local_rel(uint8_t nSim,ql_nw_tau_type_e type);

/*****************************************************************
* Function: ql_nw_get_tau_type_aft_local_rel
*
* Description: get TAU type after local RRC release
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* type	  		    [out]	TAU type
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_tau_type_aft_local_rel(uint8_t nSim,ql_nw_tau_type_e *type);

/*****************************************************************
* Function: ql_nw_set_measurement_flag_aft_local_rel
*
* Description: set measurement flag after local RRC release
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* flag	  		    [in]	measurement flag
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_measurement_flag_aft_local_rel(uint8_t nSim, uint8_t flag);

/*****************************************************************
* Function: ql_nw_get_measurement_flag_aft_local_rel
*
* Description: get measurement flag after local RRC release
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* flag	  		    [out]	measurement flag
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_measurement_flag_aft_local_rel(uint8_t nSim, uint8_t *flag);

/*****************************************************************
* Function: ql_nw_get_emm_gmm_cause_ex
*
* Description: get the latest EMM/GMM reject cause by network after attach procedure
* 
* Parameters:
* nSim              [in]     sim index，range: 0-1
* ctx        		[out]    context of EMM/GMM cause
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_emm_gmm_cause_ex(uint8_t nSim, ql_nw_cause_s *ctx);

/*****************************************************************
* Function: ql_nw_get_esm_sm_cause_ex
*
* Description: get the latest ESM/SM reject cause by network after PDN connectivity procedure
* 
* Parameters:
* nSim              [in]     sim index，range: 0-1
* ctx        		[out]    context of ESM/SM cause
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_esm_sm_cause_ex(uint8_t nSim, ql_nw_cause_s *ctx);

/*****************************************************************
* Function: ql_nw_set_esmfail_times
*
* Description: Set the times on LTE when reject by network with ESM FAILURE cause.
*              For the module that supports 2G/3G, when the maximum number of times reaches, 
               the module will seek the 2G/3G network for registration.
               When the number of times is 0, the module will not go to 2G/3G to find the network.
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* times	  	        [in]	times, range:0-5
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_esmfail_times(uint8_t nSim, uint8_t times);

/*****************************************************************
* Function: ql_nw_get_esmfail_times
*
* Description: Get the  times on LTE when reject by network with ESM FAILURE cause
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* times	  	        [out]	times
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_esmfail_times(uint8_t nSim, uint8_t *times);

/*****************************************************************
* Function: ql_nw_set_edrx
*
* Description: Set the eDRX configuration.
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* edrx_cfg	  	    [in]	eDRC configuration
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_edrx(uint8_t nSim, ql_edrx_cfg_s *edrx_cfg);


/*****************************************************************
* Function: ql_nw_get_edrx
*
* Description: Get the eDRX configuration.
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* edrx_cfg	  	    [in]	eDRC configuration
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_edrx(uint8_t nSim, ql_edrx_cfg_s *edrx_cfg);

/*****************************************************************
* Function: ql_nw_set_disrplmn
*
* Description: Weather to use RPLMN when searching network.
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* cfg	  	        [in]	disrplmn configuration.
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_disrplmn(uint8_t nSim, const ql_nw_disrplmn_cfg_s *cfg);

/*****************************************************************
* Function: ql_nw_get_disrplmn
*
* Description: Get the configuration of disrplmn.
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* cfg	  	        [in]	disrplmn configuration.
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_disrplmn(uint8_t nSim, ql_nw_disrplmn_cfg_s *cfg);

/*****************************************************************
* Function: ql_nw_set_event
*
* Description: disable or enable QUEC_NW_SIGNAL_QUALITY_IND event
* 
* Parameters:
* nSim        		[in]    sim index，range: 0-1
* event             [in]    event, support QUEC_NW_SIGNAL_QUALITY_IND only.
* enable	  	    [in]	enable or disable event.
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_event(uint8_t nSim, uint32_t event, bool enable);

/*****************************************************************
* Function: ql_nw_set_lte_auto_disable
*
* Description: Set LTE auto disable settings.
*              LTE will be automatically disabled with settings is enabled when:
*              a. Attachment has been rejected by QUEC_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA(#15);
*              b. Attachment has been rejected in "reject_max" times;
*              c. The PLMN that we try to attach is in the setting lists(plmn_num = 255 means the function will effect in all PLMNs);
*
*              When the function on,it will be off when:
*              a. Reach the expire time;
*              b. Disable the settings;
* 
* Parameters:
* nSim        		            [in]   sim index，range: 0-1
* ql_nw_lte_auto_disable_s      [in]   settings, see ql_nw_lte_auto_disable_s
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_lte_auto_disable(uint8_t nSim,ql_nw_lte_auto_disable_s *ctx);

/*****************************************************************
* Function: ql_nw_get_lte_auto_disable
*
* Description: Get lte auto disable settings
* 
* Parameters:
* nSim        		            [in]    sim index，range: 0-1
* ql_nw_lte_auto_disable_s      [out]   settings, see ql_nw_lte_auto_disable_s
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_lte_auto_disable(uint8_t nSim,ql_nw_lte_auto_disable_s *ctx);

/*****************************************************************
* Function: ql_nw_get_cfg_band
*
* Description: Get band configuration.
* 
* Parameters:
* nSim        		            [in]    sim index，range: 0-1
* band_info                     [out]   band configuration.
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_get_cfg_band(uint8_t nSim,ql_nw_band_info_t *band_info);

/*****************************************************************
* Function: ql_nw_set_cfg_band
*
* Description: Set band configuration.
* 
* Parameters:
* nSim        		            [in]   sim index，range: 0-1
* band_info                     [in]   band configuration.
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_cfg_band(uint8_t nSim,ql_nw_band_info_t *band_info);

/*****************************************************************
* Function: ql_nw_set_power_range
*
* Description: Set the band power range.
* 
* Parameters:
* set_power        		            [in]   Power configuration
* .mode		                        [in]   Power mode, see quec_pwr_backoff_mode
* .limit_min                        [in]   GSM850 GSM900 RANGE:5,7,9,11,...,33,35
*                                   [in]   GSM1800 GSM1900 RANGE:0,2,4,6,...,30,32
*                                   [in]   LTEx RANGE:-43 - 26
* .limit_max                        [in]   GSM850 GSM900 RANGE:5,7,9,11,...,33,35
*                                   [in]   GSM1800 GSM1900 RANGE:0,2,4,6,...,30,32
*                                   [in]   LTEx RANGE:-43 - 26
*
* Return:
*	0			success
*	other 	    error code
*
*****************************************************************/
ql_nw_errcode_e ql_nw_set_power_range(ql_nw_power_t *set_power);
#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_NW_H*/


