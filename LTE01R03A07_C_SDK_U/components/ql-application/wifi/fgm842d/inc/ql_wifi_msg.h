/**
  @file
  ql_wifi_msg.h

  @brief
  Quectel wifi msg api header.

*/
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/
#ifndef QL_WIFI_MSG_H
#define QL_WIFI_MSG_H

#include "ql_osi_def.h"
#include "ql_api_osi.h"

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QUEC_PROTOCOL_HEAD_MAX_SIZE 6
#define QUEC_PROTOCOL_MSG_DATA_MAX_LEN 1600
#define QUEC_PROTOCOL_MSG_MAX_LEN (QUEC_PROTOCOL_HEAD_MAX_SIZE + QUEC_PROTOCOL_MSG_DATA_MAX_LEN)
#define QUEC_MSG_CACHE_MAX_LEN (QUEC_PROTOCOL_MSG_MAX_LEN * 4)

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_MSG_SUCCESS = 0,
    QL_WIFI_MSG_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_MSG_INVALID_PARAM_ERR,
    QL_WIFI_MSG_TASK_CREATE_ERR,
    QL_WIFI_MSG_MUTEX_CREATE_ERR,
} ql_wifi_msg_errcode_e;

typedef enum
{
    QL_WIFI_NPTO_PARSE_RESULT_SUCC = 0,
    QL_WIFI_NPTO_PARSE_RESULT_NOMAGIC = 101, // No protocol header
    QL_WIFI_NPTO_PARSE_RESULT_TOOSMALL,      // Data too small
    QL_WIFI_NPTO_PARSE_RESULT_ERRCRC,        // CRC error
    QL_WIFI_NPTO_PARSE_RESULT_UNKNOWN,
}ql_wifi_npto_parse_result_e;

typedef enum
{
    QL_WIFI_NPTO_802_3_STREAM = 0,
    QL_WIFI_NPTO_STA_IP_START_CMD = 1,
    QL_WIFI_NPTO_STA_IP_DOWN_CMD = 2,
    QL_WIFI_NPTO_UAP_IP_START_CMD = 3,
    QL_WIFI_NPTO_UAP_IP_DOWN_CMD = 4,
    QL_WIFI_NPTO_IP_ADDR_SET_CMD = 5,
    QL_WIFI_NPTO_WLAN_CREATE_CMD = 6,
    QL_WIFI_NPTO_WLAN_DESTROY_CMD = 7,
    QL_WIFI_NPTO_STATUS_UPDATE_CMD = 8,

    QL_WIFI_APP_STA_ENABLE_CMD = 20,
    QL_WIFI_APP_STA_DISABLE_CMD = 21,
    QL_WIFI_APP_AP_ENABLE_CMD = 23,
    QL_WIFI_APP_AP_DISABLE_CMD = 24,
    QL_WIFI_APP_EVENT_NOTIFY = 25,
    QL_WIFI_APP_SCAN_START_CMD = 26,
    QL_WIFI_APP_STA_STATUS_CMD = 27,
    QL_WIFI_APP_STA_SET_GOT_IP = 29, //要进入低功耗模式，必须先设置该状态
    QL_WIFI_APP_PSM_CMD = 30,        //设置进入低功耗模式

    QL_WIFI_APP_RESET_CMD = 40,

    QL_BLE_APP_INIT_CMD = 60,                  //0x3C, 用于初始化BLE,参数为ql_ble_app_init_info结构体
    QL_BLE_APP_SET_SERICE_UUID_CMD = 61,       //0x3D, 创建BLE服务,参数为ql_ble_app_service_info结构体
    QL_BLE_APP_SET_CHARACT_UUID_CMD = 62,      //0x3E, 创建BLE特征值,参数为ql_ble_app_service_info结构体
    QL_BLE_APP_ADV_DATA_CMD = 63,              //0x3F, 设置BLE广播数据,参数为ql_ble_adv_data_info结构体
    QL_BLE_APP_SCAN_RSP_DATA_CMD = 64,         //0x40, 设置BLE扫描响应数据,参数为ql_ble_adv_data_info结构体
    QL_BLE_APP_ADV_START_CMD = 65,             //0x41, 开启BLE广播,无参数
    QL_BLE_APP_ADV_STOP_CMD = 66,              //0x42, 停止BLE广播,无参数
    QL_BLE_APP_DISCONNECT_CMD = 67,            //0x43, 断开BLE连接,无参数
    QL_BLE_APP_DATA_SEND_CMD = 68,             //0x44, 发送GATT数据,参数为ql_ble_send_data_info结构体
    QL_BLE_APP_CONNECTED_RPT = 69,             //0x45, 连接成功事件上报,上报的数据为: CONNECTED
    QL_BLE_APP_DISCONNECTED_RPT = 70,          //0x46, 断开连接事件上报,上报的数据为:DISCONNECTED
    QL_BLE_APP_DATA_RECV_RPT = 71,             //0x47, 接受数据
    QL_BLE_APP_MTU_CHANGE_RPT = 72,            //0x48, MTU数值变化提示, 上报的长度为2Byte的<mtu>值, mtu值的范围64~512
    QL_BLE_APP_MTU_DEINIT_CMD = 73,

    QL_WIFI_APP_CMD_MAX
} ql_wifi_npto_cmd_t;

/*========================================================================
 *  Type Definition
 *========================================================================*/
typedef void (*ql_wifi_msg_net_notify_cb)(void *ctx);
typedef void (*ql_wifi_msg_app_notify_cb)(void *ctx);

typedef struct
{
    int msg_id;
    void *msg_info;
} ql_wifi_msg_cmd_info_s;

typedef struct
{
    unsigned int data_len;
    unsigned int pos;
    char *data;
} ql_wifi_msg_protocol_info_s;

typedef struct
{
    unsigned char magic0;   // Must be 0xAA
    unsigned char magic1;   // Must be 0x55
    unsigned short cmdlen;  // Command length
    unsigned char command;  // Command.Set bit 8 to 1 means command feedback.
    unsigned char checksum;
    unsigned char *data;
} ql_wifi_msg_protocol_head_info_s;

typedef struct
{
    uint8_t *data;
    int data_len;
    int cmd;
} ql_wifi_msg_output_data_info_s;
/*========================================================================
 *  function Definition
 *========================================================================*/

/*****************************************************************
 * Function: ql_wifi_msg_net_notify_cb_register
 *
 * Description: Register Wi-Fi NET engine notification from MSG callback.
 *
 * Parameters:
 * cb        [in]   Wi-Fi NET engine notification from MSG callback.
 *                  Once the MSG engine parses the corresponding command,the command will be passed to NET engine.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_net_notify_cb_register(ql_wifi_msg_net_notify_cb cb);

/*****************************************************************
 * Function: ql_wifi_msg_app_notify_cb_register
 *
 * Description: Register Wi-Fi APP engine notification from MSG engine callback.
 *
 * Parameters:
 * cb        [in]   Wi-Fi APP engine notification from MSG engine callback.
 *                  Once the MSG engine parses the corresponding command,the command will be passed to APP engine.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_app_notify_cb_register(ql_wifi_msg_app_notify_cb cb);

/*****************************************************************
 * Function: ql_wifi_msg_init
 *
 * Description: Initialize Wi-Fi msg engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_init(void *argv);

/*****************************************************************
 * Function: ql_wifi_msg_deinit
 *
 * Description: Deinitialize Wi-Fi msg engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_deinit(void *argv);
#ifdef __cplusplus
} /*"C" */
#endif

#endif
