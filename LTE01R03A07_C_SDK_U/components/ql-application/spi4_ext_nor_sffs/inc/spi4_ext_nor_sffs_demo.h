/**  @file
  spi4_ext_nor_sffs_demo.h

  @brief
  This file is used to define spi4 ext nor sffs demo for different Quectel Project.

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


#ifndef SPI4_EXT_NOR_SFFS_DEMO_H
#define SPI4_EXT_NOR_SFFS_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define SPI4_EXT_NOR_SFFS_DEMO_TASK_PRIO           12
#define SPI4_EXT_NOR_SFFS_DEMO_TASK_STACK_SIZE     4*1024
#define SPI4_EXT_NOR_SFFS_DEMO_TASK_EVENT_CNT      4

/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_spi4_ext_nor_sffs_demo_init(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* SPI4_EXT_NOR_SFFS_DEMO_H */


