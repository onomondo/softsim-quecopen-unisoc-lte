/**  @file
  usb_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef USB_DEMO_H
#define USB_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_usb.h"

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_USB_TASK_STACK    8*1024

/*========================================================================
 *  function Definition
 *========================================================================*/

void ql_usb_app_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* USB_DEMO_H */


