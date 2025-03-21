/**  @file
  ble_gatt_demo.h

  @brief
  This file is used to define ble gatt demo for different Quectel Project.

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


#ifndef BLE_GATT_DEMO_H
#define BLE_GATT_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define BT_BLE_DEMO_TASK_PRIO           12
#define BT_BLE_DEMO_TASK_STACK_SIZE     8*1024
#define BT_BLE_DEMO_TASK_EVENT_CNT      16


/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_ble_gatt_server_demo_init(void);
QlOSStatus ql_ble_gatt_client_demo_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* BLE_GATT_DEMO_H */


