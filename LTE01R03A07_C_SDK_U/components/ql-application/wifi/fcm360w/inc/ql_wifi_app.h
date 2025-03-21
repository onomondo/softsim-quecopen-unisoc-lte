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

#include "ql_wifi_adapter.h"
#include "ql_wifi_msg.h"
#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_APP_AP_STA_CLIENTS_MAX 8

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_APP_SUCCESS = 0,
    QL_WIFI_APP_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_APP_INVALID_PARAM_ERR,
    QL_WIFI_APP_INVALID_PARAM_LEN_ERR,
    QL_WIFI_APP_NO_MEM_ERR,

    QL_WIFI_APP_INIT_SPI_ERR,
    QL_WIFI_APP_INIT_NET_ERR,
    QL_WIFI_APP_INIT_MSG_ERR,
    QL_WIFI_APP_INVALID_AP_USER_INFO_ERR,
    QL_WIFI_APP_INVALID_STA_CLIENTS_ERR,
    QL_WIFI_APP_CB_NULL_ERR,
    QL_WIFI_APP_SCAN_WORKING_ERR,
    QL_WIFI_APP_REPEAT_INIT_ERR,
    QL_WIFI_APP_MUTEX_CREATE_ERR,

    QL_WIFI_APP_TASK_CREATE_ERR,
    QL_WIFI_APP_MSG_QUEUE_CREATE_ERR,
    QL_WIFI_APP_SEM_CREATE_ERR,
} ql_wifi_app_errcode_e;

/*========================================================================
*  Type Definition
*=========================================================================*/
typedef void (*ql_wifi_app_sta_status_cb)(void *ctx);
typedef ql_wifi_app_sta_status_cb ql_wifi_app_ap_status_cb;

typedef struct
{ 
    ql_wifi_msg_cmd_t type;
    ql_wifi_adapter_msg_s msg;
}ql_wifi_app_s;
/*========================================================================
 *	function Definition
 *========================================================================*/
typedef void (*ql_wifi_app_scan_result_cb)(void *ctx);


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
 * Description: Disable Wi-Fi station.
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
 * Function: ql_wifi_app_ap_enable
 *
 * Description: Register Wi-Fi AP callback and enable the function of Wi-Fi AP.
 *
 * Parameters:
 * ap_status_cb        [in]    Wi-Fi AP callback.Once we get the status of Wi-Fi access procedure,this callback will be called.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_ap_enable(ql_wifi_app_ap_status_cb ap_status_cb);

/*****************************************************************
 * Function: ql_wifi_app_ap_disable
 *
 * Description: Disable Wi-Fi AP.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_ap_disable(void *argv);
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
 * Function: ql_wifi_app_recv
 *
 * Description: Send message to app engine.
 *
 * Parameters:
 * argv        [in]        Parsed as "ql_wifi_app_s".
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_recv(void *argv);

/*****************************************************************
 * Function: ql_wifi_app_specify_interface
 *
 * Description: Specify the Wi-Fi interface. The "mode" and "id" specify the unique interface.
 *
 * Parameters:
 * mode        [in]        STA or AP
 * id          [in]        Defined by user.From 0 to 255.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
void ql_wifi_app_specify_interface(ql_wifi_mode_e mode,uint8_t id);

/*****************************************************************
 * Function: ql_wifi_app_get_interface
 *
 * Description: Get current handle interface.
 *
 * Parameters:
 * mode        [out]        Current mode.STA or AP
 * id          [out]        Current ID.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_app_errcode_e ql_wifi_app_get_interface(ql_wifi_mode_e *mode,uint8_t *id);

/*****************************************************************
 * Function: ql_wifi_app_spi_set_cs
 *
 * Description: SPI CS handler. This function is called by SPI driver.
 *
 * Parameters:
 * ctx        [in]        Parsed as "ql_LvlMode"
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
void ql_wifi_app_spi_set_cs(void *ctx);

#ifdef __cplusplus
} /*"C" */
#endif

#endif