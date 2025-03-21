/**  @file
  tp_demo.h

  @brief
  This file is used to define touch penal for different Quectel Project.

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


#ifndef TP_DEMO_H
#define TP_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define TP_DEMO_TASK_PRIO           12
#define TP_DEMO_TASK_STACK_SIZE     8*1024
#define TP_DEMO_TASK_EVENT_CNT      4

/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_tp_demo_init(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* TP_DEMO_H */


