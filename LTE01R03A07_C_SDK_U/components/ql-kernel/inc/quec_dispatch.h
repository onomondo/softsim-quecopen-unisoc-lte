/**  @file
  quec_dispatch.c

  @brief
  This file is used to define quectel at function.

*/

/*================================================================
  Copyright (c) 2022 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     --------------------------------------

=================================================================*/
#ifndef _QUEC_SIO_ADAPTER_H_
#define _QUEC_SIO_ADAPTER_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ql_api_osi.h"
#include "ql_uart.h"

/*========================================================================
 *  General Definition
 *========================================================================*/
#define BYPSAA_TASK_STACK		10*1024
#define BYPASS_TASK_PRIO		OSI_PRIORITY_NORMAL
#define BYPASS_TASK_EVENT_CNT	60

#define QUSB_FIFO_SIZE_MIN			4*1024
#define QUSB_RX_FIFO_SIZE			24*1024
#define QUSB_TX_FIFO_SIZE			12*1024
#define QUEC_CMUX_EXIT_WAIT			100
/*========================================================================
*  enum
*========================================================================*/
typedef enum 
{
	QL_MODE_NONE				= 0,
	QL_ATC_MODE 				= 1,	//AT模式
	QL_DATA_MODE_DATA			= 2,	//数传数据模式
	QL_DATA_MODE_OPEN			= 3,	//open模式
	QL_DATA_MODE_BYPASS 		= 4,	//数据将通过回调函数传给应用层
	QL_DATA_MODE_DATA_ESCAPE 	= 5,	//+++,DTR(at&d1)退出data/ppp模式后处于escape模式,此时可以用ATO返回
}ql_uart_mode_e;

typedef enum 
{
	QL_DISPATCH_TYPE_REAL		= 0,	//真实sio通道
	QL_DISPATCH_TYPE_VIRTUAL,			//虚拟通道
}ql_dispatch_type_e;

typedef enum 
{
	QL_UART_PROTOCAL_NONE,	//UART口不走传输协议
	QL_UART_PROTOCAL_CMUX	//UART口走CMUX协议  
}ql_uart_protocal_e;

typedef enum quec_at_mode_switch_cause
{
    /**
     * @brief start data mode
     */
    QUEC_AT_MODE_SWITCH_DATA_START,
    /**
     * @brief end data mode
     */
    QUEC_AT_MODE_SWITCH_DATA_END,
    /**
     * @brief escape from data mode to command line mode
     *
     * The data engine will be kept, and push data to command line engine.
     * Typical usage is to process command temporally.
     */
    QUEC_AT_MODE_SWITCH_DATA_ESCAPE,
    /**
     * @brief resume data mode from escaped command line mode
     */
    QUEC_AT_MODE_SWITCH_DATA_RESUME,
} quec_at_mode_switch_e;

/*========================================================================
*  struct
*========================================================================*/
typedef struct
{
	bool 	baud_flag;
	uint 	baud_value;

	bool 	ifc_flag;
	uint8 	ifc_tx;
	uint8	ifc_rx;

    bool    icf_flag;
    uint8   icf_format;
    uint8   icf_parity;
}ql_sio_hw_param_t;

typedef struct ql_dispatch
{
	uint8				 		dev;		   //dispatch对应的物理口
	ql_mutex_t					lock;		   //dispatch互斥锁
	ql_dispatch_type_e			type;		   //通道为真实通道还是虚拟通道
	ql_uart_mode_e 	 			mode;		   //数据模式
	ql_uart_protocal_e 			protocal;	   //数据协议
	void						*switch_cb;	   //数据模式切换回调函数
	void						*bypass_param; //bypass模式下的参数
	void						*cmux_engine;  //cmux引擎
	int8						dlci;		   //cmux编号
	bool						deleting;	   //该通道正在被删除中
	
	void                        *ppp_session;  //底层ppp结构
	atDevice_t					*device;	   //底层sio结构	
	ql_sio_hw_param_t			hw_setting;   //用于需要先写完当前数据,再设置波特率,ifc的场景
}ql_dispatch_t;

typedef void (*ql_at_mode_switch_cb_t)(ql_dispatch_t *th, quec_at_mode_switch_e cause); 
typedef int (*ql_at_bypass_cb_t)(ql_dispatch_t *th, uint8 *data, int size, void *param);

typedef struct
{
	void			  	*user_ctx;
	ql_dispatch_t 	  	*dispatch;
	ql_at_bypass_cb_t 	cb;
	uint8 				mode;  //进bypass模式前的模式
} ql_bypass_param_t;


/*========================================================================
*  Function
*========================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CMUX
void *quec_get_cmux_engine(ql_dispatch_t *dispatch);
int quec_dispatch_exit_cmux_mode(ql_dispatch_t *th);
int quec_dispatch_set_cmux_mode(ql_dispatch_t *th, void *cfg);
int quec_dispatch_get_cmux_cfg(ql_dispatch_t *th, void *config);
#endif

bool quec_dispatch_in_data_mode(ql_dispatch_t *th);
bool quec_dispatch_in_atc_mode(ql_dispatch_t *th);
bool quec_dispatch_in_data_escape(ql_dispatch_t *th);
bool quec_dispatch_is_valid(ql_dispatch_t *th);
int8 quec_dispatch_get_mode(ql_dispatch_t *th);
int8  quec_dispatch_get_protocal(ql_dispatch_t *th);

ql_dispatch_t *quec_atcmd_get_dispatch(ql_dispatch_t *th);
ql_dispatch_t *quec_dispatch_create(atDevice_t *device, uint32 name, void *cmux_engine, uint8 dlci);
ql_dispatch_t *quec_at_find_data_channel(void);

int quec_at_dispatch_write(ql_dispatch_t *th, unsigned char *data_buff, unsigned int len);
void quec_at_set_sio_hw_needed(ql_dispatch_t *th, ql_sio_hw_param_t *param);
void quec_dispatch_set_mode(ql_dispatch_t *th, ql_uart_mode_e mode);
void quec_dispatch_set_device(atDevice_t *th, ql_dispatch_t *recv);
void quec_at_mode_switch(quec_at_mode_switch_e cause, ql_dispatch_t *d);
void quec_at_set_mode_switch_cb(ql_dispatch_t *th, ql_at_mode_switch_cb_t cb);
void quec_at_invoke_mode_switch_cb(ql_dispatch_t *th, quec_at_mode_switch_e cause);
void quec_dispatch_auto_sleep(int timeout);
void quec_dispatch_delete(ql_dispatch_t *dispatch);
void *quec_dispatch_get_ppp_session(ql_dispatch_t *th);
void quec_dispatch_set_ppp_session(ql_dispatch_t *th, void *ppp);
int quec_at_bypass_rec_data(ql_dispatch_t *th);
int quec_at_exit_bypass_mode(ql_dispatch_t *th);
int quec_at_enter_bypass_mode(ql_dispatch_t *th, ql_at_bypass_cb_t cb, void *param);
int quec_dispatch_virt_get_port(ql_dispatch_t *th, int *port);

#endif

