/**  @file
  quec_boot_adc.h

  @brief
  This file is used to define boot adc api for different Quectel Project.

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
04/08/2021        Neo         Init version
=================================================================*/

#ifndef QUEC_BOOT_ADC_H
#define QUEC_BOOT_ADC_H



#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_BOOT_ADC0_CHANNEL       0
#define QL_BOOT_ADC1_CHANNEL       1
#define QL_BOOT_ADC2_CHANNEL       2
#define QL_BOOT_ADC3_CHANNEL       3
#define QL_BOOT_VBAT_CHANNEL       4


typedef enum
{
    QL_BOOT_ADC_SCALE_1V250 = 0,
    QL_BOOT_ADC_SCALE_2V444 = 1,
    QL_BOOT_ADC_SCALE_3V233 = 2,
    QL_BOOT_ADC_SCALE_5V000 = 3,
    QL_BOOT_ADC_SCALE_MAX   = 3,
}ql_boot_adc_aux_scale;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/**
* @brief init adc
*/
void quec_boot_adc_init(void);

/**
* @brief get adc channel volt
* you need to initialize ADC first, and then call this function to get the voltage
*
* @param 
*      channel id
*
* @return
*      channel volt
*/
int quec_boot_adc_get_volt(int ql_adc_channel_id);

/**
* @brief get adc channel raw volt
* you need to initialize ADC first, and then call this function to get the raw voltage
* this interface obtain adc raw data in order to improve the speed, customers need to 
* optimize the accuracy themselves
*
* @param 
*      adc_id	  		[in] 	input adcid
* 	   aux_scale	  	[in] 	input adc scale
*      adc_value        [out]   raw adc value
*
* @return
* 	   channel volt
*/
int quec_boot_adc_get_volt_raw(int ql_adc_channel_id, ql_boot_adc_aux_scale aux_scale);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_ADC_H */


