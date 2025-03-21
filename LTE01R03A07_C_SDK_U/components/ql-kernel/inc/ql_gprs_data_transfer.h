/**  
  @file
  ql_gprs_data_transfer.h

  @brief
  This file provides the definitions for data transfer interface, and declares the 
  API functions.

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
2021/08/27   larson.li     init
=============================================================================*/

#ifndef __QL_DATA_TRANSFER_H__
#define __QL_DATA_TRANSFER_H__
#include "ql_api_common.h"

#define QL_GPRS_DATA_ERRCODE_BASE        (QL_COMPONNET_LWIP_GPRS_DATA_TRANSFER<<16)


typedef enum
{
	QL_GPRS_DATA_SUCCESS                        = 0,
	QL_GPRS_DATA_PARAM_ERROR                    = 1 | QL_GPRS_DATA_ERRCODE_BASE,
	QL_GPRS_DATA_SIZE_LARGE_ERROR,              //if \p size is too large for one packet
	QL_GPRS_DATA_UPLINK_UNAVAILABLE_ERROR,      //is uplink buffer is unavailable
	QL_GPRS_DATA_SIM_CID_ERROR,                 //sim_cid param error
	QL_GPRS_DATA_GET_NETIF_ERROR,               //sim/cid is not active
	QL_GRPS_DATA_OUTPUT_DATA_SIZE_ERROR,        //data transfer output data size error,data_len size 1-1600
}ql_gprs_data_errcode_e;

/*****************************************************************
* Function: ql_gprs_data_transfer_input_cb
*
* Description:
* 	gprs数据接收函数指针类型，
* 
* Parameters:
*   payload         	[in] 	对应pdp激活的GPRS数据会送入此函数，数据内容为void*类型,
*                               此函数对应lwip tcpip_input函数接口截取数据
*   tot_len             [in]    对应payload数据总长度
*
* Return:
*
*****************************************************************/
typedef void (*ql_gprs_data_transfer_input_cb)(void *payload, unsigned short tot_len);


/*****************************************************************
* Function: ql_gprs_data_transfer_output
*
* Description:
* 	gprs数据发送接口 
* 
* Parameters:
*   data					[in] 	无符号类型字符数据
* 	data_len	  			[in] 	数据长度，范围1-1600 
*
* Return:
* 	如果成功，该函数需要返回已送入PS域的数据长度
*	如果失败，返回ql_gprs_data_errcode_e错误码
*
*****************************************************************/
int ql_gprs_data_transfer_output(unsigned char *data,int data_len);


/*****************************************************************
* Function: ql_gprs_data_transfer_input_cb_reg
*
* Description:
* 	gprs数据接收函数注册
* 
* Parameters:
*   ql_data_transfer_input_cb	[in] 	数据输出回调函数注册,类型：函数指针
*
* Return:
* 	TRUE		执行成功
*	FALSE 	    执行失败
*
*****************************************************************/
ql_gprs_data_errcode_e ql_gprs_data_transfer_input_cb_reg(ql_gprs_data_transfer_input_cb cb);


/*****************************************************************
* Function: ql_gprs_data_transfer_start
*
* Description:
* 	GPRS 对应的PDP激活之后，才能执行此函数，否则会数据收发会执行失败
* 
* Parameters:
*   sim          	[in] 	SIM卡,范围:0-1
*   cid             [in]    pdp cid,范围:1-7
* Return:
* 	TRUE		执行成功
*	FALSE 	    执行失败
*
*****************************************************************/
ql_gprs_data_errcode_e ql_gprs_data_transfer_start(unsigned char sim,unsigned char cid );


/*****************************************************************
* Function: ql_gprs_data_transfer_stop
*
* Description:
* 	停止GPRS数据透传，恢复数据送入lwip tcp/ip协议栈中进行处理
* 
* Parameters: void
* Return:     void
*****************************************************************/
void ql_gprs_data_transfer_stop(void);

#endif /* __QL_DATA_TRANSFER_H__ */
