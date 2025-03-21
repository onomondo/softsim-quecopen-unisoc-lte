
/**  @file
  ql_ntp_client.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
#ifndef QL_NTP_CLIENT_H
#define QL_NTP_CLIENT_H

#include "ql_api_common.h"

typedef int  ntp_client_id;

typedef struct{
	int    pdp_cid;
	int    sim_id;
	int    retry_cnt;//表示ntp请求的最大重传次数
	int    retry_interval_tm;//表示ntp请求超时重传的时间间隔，单位为S
}ql_ntp_sync_option;

typedef enum{
    QL_NTP_SUCCESS                    = 0,
    QL_NTP_ERROR_UNKNOWN              = 550 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_WODBLOCK             = 551 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_INVALID_PARAM        = 552 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_OUT_OF_MEM           = 553 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_SOCKET_ALLOC_FAIL    = 554 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_SOCKET_SEND_FAIL     = 558 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_SOCKET_RECV_FAIL     = 559 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_OPEN_PDP_FAIL        = 561 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_DNS_FAIL             = 565 | (QL_COMPONENT_LWIP_QNTP << 16),
    QL_NTP_ERROR_TIMEOUT              = 569 | (QL_COMPONENT_LWIP_QNTP << 16),
}ql_ntp_error_code_e;

typedef void(*ql_ntp_sync_result_cb)(ntp_client_id        cli_id, int result, struct tm *sync_time, void *arg);

/*****************************************************************
* Function: ql_ntp_sync
*
* Description: ntp时间同步函数,启用ntp服务功能
* 
* Parameters:
*	host          [in]    ntp服务器IP地址或域名地址,以及端口号.
*   user_option   [in]    记录pdp相关信息,以及是否同步ntp时间的配置信息
*   cb            [in]    ntp事件回调函数,设置事件类型为ntp,以及对回调结果的判断
*   arg           [in]    自定义传参
*   error_code    [out]   返回ntp服务过程中错误代码
*
* Return:
* 	记录ntp服务信息的上下文id号
*
*****************************************************************/
ntp_client_id ql_ntp_sync(const char* host, ql_ntp_sync_option *user_option,ql_ntp_sync_result_cb cb, void *arg, int *error_code);

#endif
