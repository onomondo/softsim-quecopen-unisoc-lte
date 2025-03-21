/**  @file
  ble_hid_demo.h

  @brief
  This file is used to define ble hid demo for different Quectel Project.

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


#ifndef BLE_HID_DEMO_H
#define BLE_HID_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define BT_BLE_HID_DEMO_TASK_PRIO           12
#define BT_BLE_HID_DEMO_TASK_STACK_SIZE     8*1024
#define BT_BLE_HID_DEMO_TASK_EVENT_CNT      16

#define ATT_UUID_HIDS               0x1812      //HID Service

#define ATT_UUID_HID_PROTOCOL_MODE  0x2A4E      //HID char
#define ATT_UUID_HID_REPORT         0x2A4D
#define ATT_UUID_HID_REPORT_MAP     0x2A4B
#define ATT_UUID_HID_KEY_INPUT      0x2A22
#define ATT_UUID_HID_KEY_OUTPUT     0x2A32
#define ATT_UUID_HID_MOUSE_INPUT    0x2A33
#define ATT_UUID_HID_INFORMATION    0x2A4A
#define ATT_UUID_HID_CONTROL        0x2A4C

#define ATT_UUID_CHAR_EXT           0x2900
#define ATT_UUID_CHAR_USER          0x2901
#define ATT_UUID_CLIENT             0x2902
#define ATT_UUID_SERVER             0x2903
#define ATT_UUID_CHAR_FORMAT        0x2904
#define ATT_UUID_CHAR_AGGREGATE     0x2905
#define ATT_UUID_EXTERNAL_REF       0x2907
#define ATT_UUID_REPORT_RE          0x2908

#define HID_PROTOCOL_MODE_BOOT      0x00
#define HID_PROTOCOL_MODE_REPORT    0x01
#define HID_REPORT_TYPE_INPUT       0x01
#define HID_REPORT_TYPE_OUTPUT      0x02
#define HID_REPORT_TYPE_FEATURE     0x03

#define HID_DEVICE_KEYBOARD_ID      0x03C1
#define HID_DEVICE_MOUSE_ID         0x03C2

#define QL_BLE_HID_DEMO_ADDR_MAX_SIZE           17
#define QL_BLE_HID_ADV_DATA_ORIGAL_LEN          11
/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_ble_hid_demo_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* BLE_HID_DEMO_H */


