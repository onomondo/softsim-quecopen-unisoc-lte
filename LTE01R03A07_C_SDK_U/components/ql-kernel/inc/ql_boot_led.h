
/**  @file
  ql_boot_led.h

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
                
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_BOOT_LED_H
#define QL_BOOT_LED_H


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_BOOT_LED_ERRCODE_BASE (0x820F<<16)

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * boot_led errcode
 */
typedef enum
{
    QL_BOOT_LED_SUCCESS = 0,

    QL_BOOT_LED_EXECUTE_ERR                  = 1|QL_BOOT_LED_ERRCODE_BASE,
    QL_BOOT_LED_INVALID_PARAM_ERR,
}ql_errcode_boot_led;

/**
 * boot_led LPG Period and high-level time
 */
typedef enum
{
    BOOT_LPG_PER_125MS,
    BOOT_LPG_PER_250MS,
    BOOT_LPG_PER_500MS,
    BOOT_LPG_PER_1000MS,
    BOOT_LPG_PER_1500MS,
    BOOT_LPG_PER_2000MS,
    BOOT_LPG_PER_2500MS,
    BOOT_LPG_PER_3000MS
}ql_boot_LpgPer;

typedef enum
{
    BOOT_LPG_ONTIME_UNDEFINE,
    BOOT_LPG_ONTIME_15_6MS,
    BOOT_LPG_ONTIME_31_2MS,
    BOOT_LPG_ONTIME_46_8MS,
    BOOT_LPG_ONTIME_62MS,
    BOOT_LPG_ONTIME_78MS,
    BOOT_LPG_ONTIME_94MS,
    BOOT_LPG_ONTIME_110MS,
    BOOT_LPG_ONTIME_125MS,
    BOOT_LPG_ONTIME_140MS,
    BOOT_LPG_ONTIME_156MS,
    BOOT_LPG_ONTIME_172MS,
    BOOT_LPG_ONTIME_188MS,
    BOOT_LPG_ONTIME_200MS,
    BOOT_LPG_ONTIME_218MS,
    BOOT_LPG_ONTIME_234MS
}ql_boot_LpgOntime;


/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief boot_led function enable
 * @param lpg_period     : LPG period
 * @param highlvl_time   : LPG high-level time
 * @return
 *       QL_BOOT_LED_INVALID_PARAM_ERR
 *       QL_BOOT_LED_SUCCESS
 */
ql_errcode_boot_led ql_boot_led_enable(ql_boot_LpgPer lpg_period, ql_boot_LpgOntime highlvl_time);

/**
 * @brief boot_led function disable
 * @param none
 * @return
 *       QL_BOOT_LED_SUCCESS
 */
ql_errcode_boot_led ql_boot_led_disable(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_BOOT_LED_H */


