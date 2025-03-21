/**  @file
  ql_api_bt_a2dp.h

  @brief
  This file is used to define bt a2dp api for different Quectel Project.

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

#ifndef _QL_API_BT_A2DP_H
#define _QL_API_BT_A2DP_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * include files
 ===========================================================================*/
 
#include "ql_api_bt_common.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Struct
 ===========================================================================*/

typedef struct
{
    int state;                      //different from evnet type
    ql_bt_addr_s addr;              //remote address
}ql_bt_a2dp_event_info_t;

/*===========================================================================
 * Enum
 ===========================================================================*/

typedef enum
{
    QUEC_BTA2DP_CONNECTION_STATE_DISCONNECTED = 0,//模块作为耳机角色的连接状态提示，断开
    QUEC_BTA2DP_CONNECTION_STATE_CONNECTING,//正在连接
    QUEC_BTA2DP_CONNECTION_STATE_CONNECTED,//已经连接
    QUEC_BTA2DP_CONNECTION_STATE_DISCONNECTING,//正在断开
} quec_bta2dp_connection_state_e;

typedef enum
{
    QUEC_BTA2DP_CONNECTION_STATE_SIGNALING_DISCONNECTED = 0,//模块作为手机角色的连接状态提示，信号断开
    QUEC_BTA2DP_CONNECTION_STATE_SIGNALING_CONNECTED,//信号连接
    QUEC_BTA2DP_CONNECTION_STATE_MEDIA_DISCONNECTED,//媒体连接
    QUEC_BTA2DP_CONNECTION_STATE_MEDIA_CONNECTED,//媒体断开
} quec_bta2dp_src_connection_state_t;

/*===========================================================================
 * Variate
 ===========================================================================*/
 
/*===========================================================================
 * Functions
 ===========================================================================*/
/*****************************************************************
* Function: ql_bt_a2dp_avrcp_init
*
* Description:
*   初始化BT A2DP和AVRCP并注册回调函数。初始化会设置BT的mac地址
* 
* Parameters:
*   bt_cb           [in]    回调函数
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_avrcp_init(ql_bt_callback bt_cb);

/*****************************************************************
* Function: ql_bt_a2dp_avrcp_release
*
* Description:
*   BT A2DP和AVRCP释放资源
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_avrcp_release();

/*****************************************************************
* Function: ql_bt_a2dp_disconnect
*
* Description: disconnect the bt a2dp
* 
* Parameters:
*
* Return:
*
*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_disconnect(ql_bt_addr_s addr);

/*****************************************************************
* Function: ql_bt_a2dp_get_addr
*
* Description: get the addr of the client
* 
* Parameters:
* addr          [out]    the addr of the client 
*
* Return:
*   QL_BT_SUCCESS
*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_get_addr(ql_bt_addr_s *addr);

/*****************************************************************
* Function: ql_bt_a2dp_connection_state_get
*
* Description: get the connection state of the a2dp
* 
* Parameters:
*
* Return:
*   QUEC_BTA2DP_CONNECTION_STATE_DISCONNECTED
*   QUEC_BTA2DP_CONNECTION_STATE_CONNECTING
*   QUEC_BTA2DP_CONNECTION_STATE_CONNECTED
*   QUEC_BTA2DP_CONNECTION_STATE_DISCONNECTING
*****************************************************************/
quec_bta2dp_connection_state_e ql_bt_a2dp_connection_state_get(void);

/*****************************************************************
* Function: ql_bt_a2dp_connect
*
* Description: connection other device 
* 
* Parameters: other device addr
*
* Return:
*   QL_BT_SUCCESS
*****************************************************************/
 ql_errcode_bt_e ql_bt_a2dp_connect(ql_bt_addr_s addr);

 /*****************************************************************
 * Function: ql_bt_a2dp_avrcp_phone_init
 *
 * Description:
 *   初始化BT A2DP和AVRCP主机功能并注册回调函数。
 * 
 * Parameters:
 *   bt_cb           [in]    回调函数
 *
 * Return:ql_errcode_bt_e

 *****************************************************************/

ql_errcode_bt_e ql_bt_a2dp_avrcp_phone_init(ql_bt_callback bt_cb);

/*****************************************************************
* Function: ql_bt_a2dp_avrcp_phone_release
*
* Description:
*   BT A2DP和AVRCP释放资源
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*

*****************************************************************/

ql_errcode_bt_e ql_bt_a2dp_avrcp_phone_release(void);

/*****************************************************************
* Function: ql_bt_a2dp_avrcp_set_cfg
*
* Description:
*   设置BT A2DP和AVRCP配置项
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*

*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_avrcp_set_cfg(ql_bt_a2dp_audio_cfg_t cfg);

/*****************************************************************
* Function: ql_bt_a2dp_avrcp_get_cfg
*
* Description:
*   获取BT A2DP和AVRCP配置项
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*

*****************************************************************/
ql_errcode_bt_e ql_bt_a2dp_avrcp_get_cfg(ql_bt_a2dp_audio_cfg_t *cfg);
#ifdef __cplusplus
    } /*"C" */
#endif
    
#endif /* _QL_API_BT_A2DP_H */

