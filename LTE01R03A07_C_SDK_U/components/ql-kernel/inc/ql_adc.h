
/**  @file
  ql_adc.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------


=================================================================*/


#ifndef QL_ADC_H
#define QL_ADC_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_ADC0_CHANNEL       0
#define QL_ADC1_CHANNEL       1
#define QL_ADC2_CHANNEL       2
#define QL_ADC3_CHANNEL       3

/*========================================================================
*  Enumeration Definition
*========================================================================*/

/****************************  error code about ADC    ***************************/
typedef enum
{
	QL_ADC_SUCCESS  			= 0,
    QL_ADC_INVALID_PARAM_ERR    = 10  | (QL_COMPONENT_BSP_ADC << 16),
	QL_ADC_GET_VALUE_ERROR 		= 50  | (QL_COMPONENT_BSP_ADC << 16),
	QL_ADC_MEM_ADDR_NULL_ERROR	= 60  | (QL_COMPONENT_BSP_ADC << 16),
} ql_errcode_adc_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef enum
{
    QL_ADC_SCALE_AUTO = 0xFF, //自适应
	QL_ADC_SCALE_1V250 = 0,
    QL_ADC_SCALE_2V444 = 1,
    QL_ADC_SCALE_3V233 = 2,
    QL_ADC_SCALE_5V000 = 3,
    QL_ADC_SCALE_MAX = 3,
}ql_adc_aux_scale;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_adc_get_volt
*
* Description:  get adc value of voltage by ADC0/ADC1/ADC2/ADC3
* 
* Parameters:
* 	adc_id	  		[in] 	input adcid
*   adc_value       [out]   adc value
*
* Return:
* 	volt_value	succeed
*	-1 	        failure
*
*****************************************************************/
ql_errcode_adc_e ql_adc_get_volt(int ql_adc_channel_id, int *adc_value);

/*****************************************************************
* Function: ql_adc_get_volt_raw
*
* Description:  get adc raw value of voltage by ADC0/ADC1/ADC2/ADC3
*               this interface obtain adc raw data in order to improve the speed
*               customers need to optimize the accuracy themselves
* 
* Parameters:
* 	adc_id	  		[in] 	input adcid
* 	aux_scale	  	[in] 	input adc scale
*   adc_value       [out]   raw adc value
*
* Return:
* 	volt_value	succeed
*	-1 	        failure
*
*****************************************************************/
ql_errcode_adc_e ql_adc_get_volt_raw(int ql_adc_channel_id, ql_adc_aux_scale aux_scale, int *adc_value);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_ADC_H */


