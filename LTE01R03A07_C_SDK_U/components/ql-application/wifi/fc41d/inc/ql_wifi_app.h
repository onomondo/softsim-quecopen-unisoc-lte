/**
  @file
  ql_wifi_app.h

  @brief
  Quectel wifi app api header.

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
#ifndef QL_WIFI_APP_H
#define Ql_WIFI_APP_H

#include "ql_api_common.h"
#include "ql_api_wifi.h"

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
    // ql_wifi_app_errcode_e extends from ql_wifi_errcode_e in file ql_api_wifi.h
    QL_WIFI_APP_SUCCESS = 0,
    QL_WIFI_APP_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_INVALID_PARAM_ERR,
    QL_WIFI_APP_INVALID_PARAM_LEN_ERR,
    QL_WIFI_APP_NO_MEM_ERR,
    QL_WIFI_APP_MEM_ADDR_NULL_ERR = 5 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_INVALID_IP_ERR,
    QL_WIFI_APP_INVALID_GW_ERR,
    QL_WIFI_APP_INVALID_NETMASK_ERR,
    QL_WIFI_APP_INVALID_DNS_ERR,
    QL_WIFI_APP_INVALID_DHCP_ERR = 10 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_INVALID_MAC_ERR,
    QL_WIFI_APP_INVALID_CID_ERR,
    QL_WIFI_APP_STATE_ERR,
    // ql_wifi_app.h add
    QL_WIFI_APP_INIT_UART_ERR,
    QL_WIFI_APP_INIT_SPI_ERR = 15 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_INIT_NET_ERR,
    QL_WIFI_APP_INIT_MSG_ERR,
    QL_WIFI_APP_INIT_TASK_ERR,
    QL_WIFI_APP_INVALID_AP_USER_INFO_ERR,
    QL_WIFI_APP_CB_NULL_ERR = 20 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_SCAN_WORKING_ERR,
    QL_WIFI_APP_REPEAT_INIT_ERR,
    QL_WIFI_APP_MUTEX_CREATE_ERR,
} ql_wifi_app_errcode_e;

typedef enum 
{
    QL_WIFI_APP_OPT_SSID = 0,     // val_len: ssid len,STA/AP ssid
    QL_WIFI_APP_OPT_PASSWORD, // val_len: password len,STA/AP password
} ql_wifi_app_opt_e;

/*========================================================================
*  Type Definition
*=========================================================================*/
typedef void (*ql_wifi_app_sta_status_cb)(void *ctx);
typedef void (*ql_wifi_app_scan_result_cb)(void *ctx);
typedef int (*ql_wifi_app_output_data_msg_cb)(void *ctx);

typedef struct
{
    char ssid[QL_WIFI_SSID_MAX_LEN];
    char password[QL_WIFI_PASSWORD_MAX_LEN];
} ql_wifi_app_user_ap_info_s;

typedef struct
{
    char ssid[QL_WIFI_SSID_MAX_LEN + 1]; // The SSID of an access point
    char ap_power;                       // Signal strength, min:0, max:100
    uint8_t bssid[6];                    // The BSSID of an access point
    char channel;                        // The RF frequency, 1-13
    uint8_t security;                    // Security type, @ref wlan_sec_type_t
} ql_wifi_app_ap_info_s;

typedef struct
{
    char ApNum; // The number of access points found in scanning
    ql_wifi_app_ap_info_s *ApList;
} ql_wifi_app_scan_result_s;

/*========================================================================
 *	function Definition
 *========================================================================*/

/*****************************************************************
 * Function: ql_wifi_app_get_user_ap_info
 *
 * Description: Get Wi-Fi user ap information.
 *
 * Parameters:
 * info        [out]    Wi-Fi user ap information.See ql_wifi_app_user_ap_info_s.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_get_user_ap_info(ql_wifi_app_user_ap_info_s *info);

/*****************************************************************
 * Function: ql_wifi_app_set_user_ap_info
 *
 * Description: Set Wi-Fi user ap information.
 * Parameters:
 * info        [in]    Wi-Fi user ap information.See ql_wifi_app_user_ap_info_s.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_set_user_ap_info(ql_wifi_app_user_ap_info_s *info);

/*****************************************************************
 * Function: ql_wifi_app_scan
 *
 * Description: Register Wi-Fi scan callback which get the scan result and start Wi-Fi scan.
 *
 * Parameters:
 * cb        [in]    Wi-Fi scan callback.Once we get the result of Wi-Fi scan,this callback will be called.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_scan(ql_wifi_app_scan_result_cb cb);

/*****************************************************************
 * Function: ql_wifi_app_sta_enable
 *
 * Description: Register Wi-Fi STA callback and enable the function of Wi-Fi STA(Station).
 *
 * Parameters:
 * sta_status_cb        [in]    Wi-Fi STA callback.Once we get the status of Wi-Fi access procedure,this callback will be called.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_sta_enable(ql_wifi_app_sta_status_cb sta_status_cb);

/*****************************************************************
 * Function: ql_wifi_app_sta_disable
 *
 * Description: Register Wi-Fi STA callback and enable the function of Wi-Fi STA(Station).
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_sta_disable(void *argv);

/*****************************************************************
 * Function: ql_wifi_app_reset
 *
 * Description: Software reset of Wi-Fi device.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_reset(void *argv);

/*****************************************************************
 * Function: ql_wifi_app_init
 *
 * Description: Wi-Fi application initialization.All needed tasks and communication bus are initialized in this function.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_init(void *argv);

/*****************************************************************
 * Function: ql_wifi_app_deinit
 *
 * Description: Wi-Fi application deinitialization.Release all Wi-Fi application resource.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_deinit(void *argv);

#ifdef __cplusplus
} /*"C" */
#endif

#endif