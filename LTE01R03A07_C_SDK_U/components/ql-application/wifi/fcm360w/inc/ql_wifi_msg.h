/**  @file

  @brief
  This file is used to define bt api for different Quectel Project.

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
#ifndef QL_WIFI_MSG_H
#define QL_WIFI_MSG_H

#include "ql_api_common.h"
#include <stdint.h>

#include "ql_wifi_adapter.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_MSG_SUCCESS = 0,
    QL_WIFI_MSG_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_MSG_INVALID_PARAM_ERR,
    QL_WIFI_MSG_NO_MEM_ERR,
    QL_WIFI_MSG_TASK_CREATE_ERR,
    QL_WIFI_MSG_MSG_QUEUE_CREATE_ERR,
    QL_WIFI_MSG_MUTEX_CREATE_ERR,
    QL_WIFI_MSG_SEM_CREATE_ERR,
} ql_wifi_msg_errcode_e;

typedef enum
{
    QL_WIFI_MSG_TAG_SEND_RESET = 0x0FFF,
    /* AP. */
    QL_WIFI_MSG_TAG_SEND_AP_START = 0x0004,
    QL_WIFI_MSG_TAG_SEND_AP_STOP = 0x0005,
    QL_WIFI_MSG_TAG_SEND_AP_STA_CLIENT_MAX = 0x0006,
    /* AP setting/reading tag. */
    QL_WIFI_MSG_TAG_SEND_AP_CONFIG = 0x0008,
    QL_WIFI_MSG_TAG_SEND_AP_SSID = (1 << 8)|QL_WIFI_MSG_TAG_SEND_AP_CONFIG,
    QL_WIFI_MSG_TAG_SEND_AP_PASSWORD = (2 << 8)|QL_WIFI_MSG_TAG_SEND_AP_CONFIG,
    QL_WIFI_MSG_TAG_SEND_AP_CHANNEL = (3 << 8)|QL_WIFI_MSG_TAG_SEND_AP_CONFIG,
    QL_WIFI_MSG_TAG_SEND_AP_AUTH = (4 << 8)|QL_WIFI_MSG_TAG_SEND_AP_CONFIG,
    QL_WIFI_MSG_TAG_SEND_AP_IP_INFO = (5 << 8)|QL_WIFI_MSG_TAG_SEND_AP_CONFIG,
    /* AP recving tag. */
    QL_WIFI_MSG_TAG_RECV_AP_CREATED = 0x0100,
    QL_WIFI_MSG_TAG_REVC_AP_CLIENT_CONNECTED = 0x0101,
    QL_WIFI_MSG_TAG_REVC_AP_CLIENT_DISCONNECTED = 0x0102,

    /* STA. */
    QL_WIFI_MSG_TAG_SEND_STA_START = 0x0054,
    QL_WIFI_MSG_TAG_SEND_STA_STOP = 0x0055,
    /* STA setting/reading tag. */
    QL_WIFI_MSG_TAG_SEND_STA_CONFIG = 0x0058,
    QL_WIFI_MSG_TAG_SEND_STA_SSID = (1 << 8)|QL_WIFI_MSG_TAG_SEND_STA_CONFIG,
    QL_WIFI_MSG_TAG_SEND_STA_PASSWORD = (2 << 8)|QL_WIFI_MSG_TAG_SEND_STA_CONFIG,
    QL_WIFI_MSG_TAG_SEND_STA_CHANNEL = (3 << 8)|QL_WIFI_MSG_TAG_SEND_STA_CONFIG,
    QL_WIFI_MSG_TAG_SEND_STA_BSSID = (4 << 8)|QL_WIFI_MSG_TAG_SEND_STA_CONFIG,
    /* STA recving tag. */
    QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_ACTIVE = QL_WIFI_MSG_TAG_SEND_STA_START,
    QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_ACTIVE = QL_WIFI_MSG_TAG_SEND_STA_STOP,

    QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_PASSIVE = 0x0103,
    QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_PASSIVE = 0x0104,

    QL_WIFI_MSG_TAG_RECONNECT = 0x0105,
    QL_WIFI_MSG_TAG_REASON_CALL = 0x0106,
    QL_WIFI_MSG_TAG_CONNECTED_CALL = 0x0107,
} ql_wifi_msg_tag_e;

typedef enum
{
    QL_WIFI_MSG_TYPE_WRITE,
    QL_WIFI_MSG_TYPE_READ,
} ql_wifi_msg_type_e;

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
    QL_WIFI_APP_AP_STATUS_CMD = 28,

    QL_WIFI_SIO_TEST = 30, /* [test] */

    QL_WIFI_APP_CMD_MAX
} ql_wifi_msg_cmd_t;

/*========================================================================
 *  Type Definition
 *=========================================================================*/
#pragma pack(1)
typedef struct
{
    char ssid[QL_WIFI_SSID_MAX_LEN];
    char password[QL_WIFI_PASSWORD_MAX_LEN];
} ql_wifi_app_user_ap_info_s;

typedef struct
{
    uint8_t tag;
    uint8_t length;
    uint8_t value[0];
    /* Value */
} ql_wifi_msg_tlv_s;

typedef struct
{
    uint16_t magic;
    uint16_t len;
    struct
    {
        uint16_t tag;
        uint16_t length;
        uint8_t value[0];
    } field;
} ql_wifi_msg_header_s;

typedef struct
{
    ip4_addr_t ip;
    ip4_addr_t netmask;
    ip4_addr_t gw;
    ip4_addr_t dns1;
    ip4_addr_t dns2;
    uint8_t mac[6];
} ql_wifi_msg_ip_info_s;

typedef struct
{
    ql_wifi_app_user_ap_info_s user_info;
    uint32_t channel;
    uint32_t auth_mode;
    uint32_t cur_clients;
    uint32_t max_clients;
    ql_wifi_msg_ip_info_s ip_info;
    uint16_t reason_code;
} ql_wifi_msg_recv_info_s;

#pragma pack()

typedef struct
{
    ql_wifi_msg_type_e type;
    ql_wifi_adapter_msg_s msg;
} ql_wifi_msg_s;
/*========================================================================
 *	function Definition
 *========================================================================*/

/*****************************************************************
 * Function: ql_wifi_msg_init
 *
 * Description: Initialize msg engine.
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
 * Description: Denitialize msg engine.
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

/*****************************************************************
 * Function: ql_wifi_msg_recv
 *
 * Description: Send message to msg engine.
 *
 * Parameters:
 * argv        [in]        Parsed as "ql_wifi_msg_s".
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_recv(void* argv);

/*****************************************************************
 * Function: ql_wifi_msg_write
 *
 * Description: Send message to Wi-Fi device.
 *
 * Parameters:
 * tag        [in]        Send message to Wi-Fi.
 * data       [in]        Messages.
 * len        [in]        Length of messages.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_msg_errcode_e ql_wifi_msg_write(ql_wifi_msg_tag_e tag, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif