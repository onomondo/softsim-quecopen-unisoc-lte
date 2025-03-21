/**
  @file
  ql_wifi_net.h

  @brief
  Quectel wifi net api header.

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
#ifndef QL_WIFI_NET_H
#define QL_WIFI_NET_H

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_NET_SUCCESS = 0,
    QL_WIFI_NET_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_NET_INVALID_PARAM_ERR,
    QL_WIFI_NET_TASK_CREATE_ERR,
    QL_WIFI_NET_MUTEX_CREATE_ERR,
} ql_wifi_net_errcode_e;

/*========================================================================
 *  Type Definition
 *========================================================================*/
typedef void (*ql_wifi_net_app_notify_cb)(void *ctx);

/*========================================================================
 *  function Definition
 *========================================================================*/

/*****************************************************************
 * Function: ql_wifi_net_app_notify_cb_register
 *
 * Description: Register Wi-Fi APP engine notification from NET engine callback.
 *
 * Parameters:
 * cb        [in]   Wi-Fi APP engine notification from NET engine callback.
 *                  Once the NET engine parses the corresponding command,the command will be passed to APP engine.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_app_notify_cb_register(ql_wifi_net_app_notify_cb cb);

/*****************************************************************
 * Function: ql_wifi_net_init
 *
 * Description: Initialize Wi-Fi net engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_init(void *argv);

/*****************************************************************
 * Function: ql_wifi_net_deint
 *
 * Description: Deinitialize Wi-Fi net engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_deint(void *argv);
#ifdef __cplusplus
} /*"C" */
#endif

#endif