/**  @file
  alipay_api_demo.h

  @brief
  This file is used to test alipay api for different Quectel Project.

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


#ifndef ALIPAY_API_DEMO_H
#define ALIPAY_API_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define ALIPAY_API_DEMO_TASK_PRIO           12
#define ALIPAY_API_DEMO_TASK_STACK_SIZE     8*1024
#define ALIPAY_API_DEMO_TASK_EVENT_CNT      16


/*========================================================================
 *  function Definition
 *========================================================================*/
int ql_alipay_api_init(void);
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* ALIPAY_API_DEMO_H */


