/**  @file
  quec_boot_scatter.h

  @brief
  This file is used to define boot scatter api for different Quectel Project.

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
06/08/2021        Neo         Init version
=================================================================*/

#ifndef QUEC_BOOT_SCATTER_H
#define QUEC_BOOT_SCATTER_H



#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/



/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/**
* @brief get vbus state
*
* @param 
*      NULL
*
* @return
*      true---connect
*      false--not connect
*/
bool quec_boot_vbus_state(void);




#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_SCATTER_H */


