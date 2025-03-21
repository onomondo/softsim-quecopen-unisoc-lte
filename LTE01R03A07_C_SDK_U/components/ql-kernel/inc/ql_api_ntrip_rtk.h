/**  @file
  ql_api_ntrip_rtk.h

  @brief
  This file is used to define ntrip rtk api for different Quectel Project.

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


#ifndef QL_API_NTRIP_RTK_H
#define QL_API_NTRIP_RTK_H
#include "ql_api_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/****************************  error code about ql ntrip rtk    ***************************/
typedef enum
{
	QL_NTRIP_RTK_SUCCESS                  =   0,
    
    QL_NTRIP_RTK_ERROR                    =   1 | (QL_COMPONNET_LWIP_NTRIP_RTK << 16),   //ntrip rtk一般错误
    QL_NTRIP_RTK_PARAM_ERROR,                             //参数错误
    QL_NTRIP_RTK_STATE_ERROR,                             //状态错误，如没有挂载到差分服务器挂载点上，就直接发送数据
    QL_NTRIP_RTK_SEND_ERROR,                              //发送数据错误
    QL_NTRIP_RTK_MALLOC_MEM_ERROR,                        //申请内存错误
    QL_NTRIP_RTK_MUTEX_LOCK_ERROR,                        //互斥锁上锁超时报错
    QL_NTRIP_RTK_SOCKET_ERROR,                            //使用socket出错
}ql_errcode_ntrip_rtk_e;

typedef enum
{
	QL_NTRIP_RTK_IDLE = 0,                  //空闲状态，没有连接差分服务器。过挂载差分服务器出错，也会返回idle状态，并断开socket连接
	QL_NTRIP_RTK_CONNECTING,                //正在连接差服务器
	QL_NTRIP_RTK_CONNECTED,                 //连接上差服务器
	QL_NTRIP_RTK_POINTING,                  //正在挂载到差分服务器的挂载点上
	QL_NTRIP_RTK_POINTED,                   //挂载到差分服务器的挂载点上
	QL_NTRIP_RTK_RECV_DATA,                 //收到差分服务器下发的数据，需要处理数据
}ql_ntrip_rtk_state_e;

typedef void(*ql_ntrip_rtk_event_cb)(ql_ntrip_rtk_state_e state, char *data, int len);


#define QUEC_NTTIP_RTK_ID_MAX           12
typedef struct
{
    uint8_t pdp_cid;                                    //pdp ID
	uint8_t sim_id;                                     //SIM ID
    char *ip;                                           //差分服务器ip
    unsigned short port;                                //差分服务器端口
    char *user;                                         //用户名
    char *pwd;                                          //password密码
    char *point;                                        //差分服务器挂载点   
    char *gnss_type;                                    //上传到差分服务器的GNSS数据类型

    ql_ntrip_rtk_event_cb func;                         //回调处理
} ql_ntrip_rtk_config_s;


/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_ntrip_rtk_init
*
* Description:
*   初始化ntrip，发起和差分服务器建立socket连接
* 
* Parameters:
*   id          [in]    ntrip rtk id，最大不超过12个
*   config      [in]    ntrip的配置信息，包括差分服务器的ip，端口，挂载点，用户名，密码等信息
*
* Return:ql_errcode_ntrip_rtk_e
*
*****************************************************************/
ql_errcode_ntrip_rtk_e ql_ntrip_rtk_init(int id, ql_ntrip_rtk_config_s *config);

/*****************************************************************
* Function: ql_ntrip_rtk_send
*
* Description:
*   发送数据到差分服务器，一般为GNSS的定位数据
* 
* Parameters:
*   id          [in]    ntrip rtk id，最大不超过12个
*   data        [in]    发往差分服务器的定位数据
*   len         [in]    发往差分服务器的定位数据长度
*
* Return:ql_errcode_ntrip_rtk_e
*
*****************************************************************/
ql_errcode_ntrip_rtk_e ql_ntrip_rtk_send(int id, char *data, int len);

/*****************************************************************
* Function: ql_ntrip_rtk_close
*
* Description:
*   断开和差分服务器的连接，并释放资源
* 
* Parameters:
*   id          [in]    ntrip rtk id，最大不超过12个
*
* Return:ql_errcode_ntrip_rtk_e
*
*****************************************************************/

ql_errcode_ntrip_rtk_e ql_ntrip_rtk_close(int id);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_NTRIP_RTK_H */



