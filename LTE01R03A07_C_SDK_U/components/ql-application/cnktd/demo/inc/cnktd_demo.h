/**  @file
  cnktd_demo.h

  @brief
  This file is used to define cnktd demo for different Quectel Project.

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


#ifndef CNKTD_DEMO_H
#define CNKTD_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define CNKTD_DEMO_TASK_PRIO           12
#define CNKTD_DEMO_TASK_STACK_SIZE     8*1024
#define CNKTD_DEMO_TASK_EVENT_CNT      4

/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus cnktd_demo_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* CNKTD_DEMO_H */


