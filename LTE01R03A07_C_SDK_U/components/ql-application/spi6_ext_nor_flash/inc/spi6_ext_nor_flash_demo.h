/**  @file
  spi6_ext_nor_flash_demo.h

  @brief
  This file is used to define bt demo for different Quectel Project.

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

=================================================================*/


#ifndef SPI6_EXT_NOR_FLASH_DEMO_H
#define SPI6_EXT_NOR_FLASH_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define SPI6_EXT_NOR_FLASH_DEMO_TASK_PRIO           12
#define SPI6_EXT_NOR_FLASH_DEMO_TASK_STACK_SIZE     4*1024
#define SPI6_EXT_NOR_FLASH_DEMO_TASK_EVENT_CNT      4

/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_spi6_ext_nor_flash_demo_init(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* SPI6_EXT_NOR_FLASH_DEMO_H */


