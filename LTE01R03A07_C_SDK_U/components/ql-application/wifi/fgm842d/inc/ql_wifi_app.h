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
typedef void (*ql_ble_gatt_data_cb)(void *ctx);

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

typedef enum
{
	QUEC_WIFI_PSM_DISABLE                   = 0,
	QUEC_WIFI_PSM_ENABLE               		= 1,
}quec_wifi_psm_type;

typedef struct
{
	quec_wifi_psm_type        enable;
	int               		  dtim; //进入低功耗模式时，设置dtim值，范围1~30。一般设置为10
}quec_wifi_psm_info;


/*QL_BLE_APP_INIT_CMD 命令的参数*/
typedef struct
{	uint8_t role;  //初始化BLE的角色。目前只支持参数0,表示 WIFI&BLE 模块为外围设备进行BLE初始化。
}ql_ble_app_init_info;

/*
  QL_BLE_APP_SET_SERICE_UUID_CMD & QL_BLE_APP_SET_CHARACT_UUID_CMD 命令的参数,
  最多支持创建1个 BLE 服务, 3个BLE 特征值
  必须在使用QL_BLE_APP_INIT_CMD命令初始化BLE后使用QL_BLE_APP_SET_SERICE_UUID_CMD命令创建BLE服务，
  否则蓝牙广播、扫描及连接等功能无法正常运行
*/
typedef struct
{
	uint8_t uuid_len; //蓝牙服务/特征值UUID长度。2字节或16字节。
    uint8_t uuid[16];
}ql_ble_app_service_info;

/*QL_BLE_APP_ADV_DATA_CMD & QL_BLE_APP_SCAN_RSP_DATA_CMD命令的参数,*/
typedef struct
{
    uint8_t adv_data_len;  //长度不能超过28字节。
    uint8_t adv_data[28];
}ql_ble_adv_data_info;


/*QL_BLE_APP_DATA_SEND_CMD 命令的参数*/
typedef struct
{
    uint16_t uuid_len;  //蓝牙特征值UUID长度。2字节或16字节。
    uint8_t  uuid[16];  
    uint16_t data_len;  //发送数据长度(长度不超过MTU-3，MTU默认值为23字节)
    uint8_t  data[20];
}ql_ble_send_data_info;

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

/*****************************************************************
 * Function: ql_wifi_app_psm
 *
 * Description: Control Wi-Fi module to enter or exit PSM mode
 *
 * Parameters:
 * argv        [in]        quec_wifi_psm_info struct pointer.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_psm(void *argv);
 
/*****************************************************************
* Function: ql_ble_init
 *
 * Description: Initialize BLE and create a BLE service.
 *
 * Parameters:
 * argv        [in]        ql_ble_app_service_info struct pointer.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_init(void *argv);

/*****************************************************************
 * Function: ql_ble_set_characteristic
 *
 * Description: Set a BLE characteristic value
 *
 * Parameters:
 * argv        [in]        ql_ble_app_service_info struct pointer.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_set_characteristic(void *argv);

/*****************************************************************
 * Function: ql_ble_register_gatt_data_cb
 *
 * Description: Register ble gatt data callback
 *
 * Parameters:
 * cb        [in]    Once we get the gatt data,this callback will be called.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_register_gatt_data_cb(ql_ble_gatt_data_cb cb);

/*****************************************************************
 * Function: ql_ble_set_adv_data
 *
 * Description: Set BLE Advertising data
 *
 * Parameters:
 * argv        [in]        ql_ble_adv_data_info struct pointer.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_set_adv_data(void *argv);

/*****************************************************************
 * Function: ql_ble_adv_start
 *
 * Description: Start BLE Advertising
 *
 * Parameters:
 * argv        [in]        No used yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_adv_start(void *argv);

/*****************************************************************
 * Function: ql_ble_adv_stop
 *
 * Description: Stop BLE Advertising
 *
 * Parameters:
 * argv        [in]        No used yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_adv_stop(void *argv);

/*****************************************************************
 * Function: ql_ble_sent_gatt_data
 *
 * Description: Sent GATT data
 *
 * Parameters:
 * argv        [in]        ql_ble_send_data_info struct pointer.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_ble_sent_gatt_data(void *argv);

#ifdef __cplusplus
} /*"C" */
#endif

#endif