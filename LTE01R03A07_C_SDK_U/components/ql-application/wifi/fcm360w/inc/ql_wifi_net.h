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

#include "ql_wifi_adapter.h"
#include "ql_wifi_msg.h"

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_NET_IP_INFO_MAX 32
#define QL_WIFI_NET_DEVICE_NAME_MAX 20
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_NET_SUCCESS = 0,
    QL_WIFI_NET_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_NET_INVALID_PARAM_ERR,
    QL_WIFI_NET_NO_MEM_ERR,
    QL_WIFI_NET_TASK_CREATE_ERR,
    QL_WIFI_NET_MSG_QUEUE_CREATE_ERR,
    QL_WIFI_NET_MUTEX_CREATE_ERR,
    QL_WIFI_NET_SEM_CREATE_ERR,
} ql_wifi_net_errcode_e;

/*========================================================================
 *  Type Definition
 *========================================================================*/
typedef void (*ql_wifi_net_app_notify_cb)(void *ctx);

typedef struct
{
    ql_wifi_msg_cmd_t type;
    ql_wifi_adapter_msg_s msg;
} ql_wifi_net_s;

typedef struct
{
    uint8_t num;
    uint8_t using_num;
    struct
    {
        struct
        {
            ql_wifi_mode_e mode;
            uint8_t id;
        } interface;
        uint8_t is_used;
        char name[QL_WIFI_NET_DEVICE_NAME_MAX];
        ql_wifi_msg_ip_info_s ip_info;
    } info[QL_WIFI_NET_IP_INFO_MAX];
} ql_wifi_net_ip_info_s;
/*========================================================================
 *  function Definition
 *========================================================================*/

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
 * Function: ql_wifi_net_deinit
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
ql_wifi_net_errcode_e ql_wifi_net_deinit(void *argv);

/*****************************************************************
 * Function: ql_wifi_net_recv
 *
 * Description: Send message to net engine.
 *
 * Parameters:
 * argv        [in]        Parsed as "ql_wifi_app_s".
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_recv(void *argv);

/*****************************************************************
 * Function: ql_wifi_net_create
 *
 * Description: Create netcard.
 *
 * Parameters:
 * mode        [in]        See ql_wifi_mode_e.
 * info        [in]        See ql_wifi_msg_recv_info_s.
 * tag        [in]         See ql_wifi_msg_tag_e.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_create(ql_wifi_mode_e mode,ql_wifi_msg_recv_info_s *info,ql_wifi_msg_tag_e tag);

/*****************************************************************
 * Function: ql_wifi_net_destroy
 *
 * Description: Destroy netcard.
 *
 * Parameters:
 * mode        [in]        See ql_wifi_mode_e.
 * info        [in]        See ql_wifi_msg_recv_info_s.
 * tag         [in]         See ql_wifi_msg_tag_e.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_destroy(ql_wifi_mode_e mode,ql_wifi_msg_recv_info_s *info,ql_wifi_msg_tag_e tag);

/*****************************************************************
 * Function: ql_wifi_net_get_ip_info_ptr
 *
 * Description: Get net ip info with mac and ip.
 *
 * Parameters:
 * mac        [in]          MAC address.
 * ip4        [in]          IP address.
 * index      [out]         Net ip info index.
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_net_errcode_e ql_wifi_net_get_ip_info_index_by_mac_ip(uint8_t mac[6], ip4_addr_t ip4, uint8_t *index);

/*****************************************************************
 * Function: ql_wifi_net_get_ip_info_ptr
 *
 * Description: Get net ip info pointer.
 *
 * Parameters:
 *
 * Return: Pointer of net ip info.
 *
 *****************************************************************/
ql_wifi_net_ip_info_s* ql_wifi_net_get_ip_info_ptr();

#ifdef __cplusplus
} /*"C" */
#endif

#endif