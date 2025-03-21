
/**  @file
  ql_uart.h

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
                
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_UART_H
#define QL_UART_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_UART_ERRCODE_BASE    (QL_COMPONENT_BSP_UART<<16)

#define QL_UART_MAX_NUM         3

#define QL_UART_MAX_TX_DELAY_BITS       5
#define QL_UART_MIN_TX_DELAY_BITS       0

#define QL_UART_RX_BUF_SIZE             (6 * 1024)
#define QL_UART_TX_BUF_SIZE             (4 * 1024)
/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QL_UART_SUCCESS             = 0,
    QL_UART_EXECUTE_ERR         = 1|QL_UART_ERRCODE_BASE,
    QL_UART_MEM_ADDR_NULL_ERR,
    QL_UART_INVALID_PARAM_ERR,
    QL_UART_OPEN_REPEAT_ERR,
    QL_UART_NOT_OPEN_ERR        = 5|QL_UART_ERRCODE_BASE,
} ql_uart_errcode_e;

typedef enum 
{
    QL_PORT_NONE = -1,
	QL_UART_PORT_1,
	QL_UART_PORT_2,
	QL_UART_PORT_3,
	QL_USB_PORT_AT,
	QL_USB_PORT_MODEM,
	QL_USB_PORT_NMEA,
    QL_USB_PORT_ACM0,
	QL_PORT_MAX,
}ql_uart_port_number_e;

typedef enum
{
	QL_FC_NONE = 0,
	QL_FC_HW,
}ql_uart_flowctrl_e;

typedef enum
{
    QL_UART_BAUD_AUTO       = 0,
    QL_UART_BAUD_1200       = 1200,
    QL_UART_BAUD_2400       = 2400,
    QL_UART_BAUD_4800       = 4800,
    QL_UART_BAUD_9600       = 9600,
    QL_UART_BAUD_10400      = 10400,
    QL_UART_BAUD_14400      = 14400,
    QL_UART_BAUD_19200      = 19200,
    QL_UART_BAUD_28800      = 28800,
    QL_UART_BAUD_33600      = 33600,
    QL_UART_BAUD_38400      = 38400,
    QL_UART_BAUD_57600      = 57600,
    QL_UART_BAUD_115200     = 115200,
    QL_UART_BAUD_187500     = 187500,
    QL_UART_BAUD_230400     = 230400,
    QL_UART_BAUD_460800     = 460800,
    QL_UART_BAUD_921600     = 921600,
	QL_UART_BAUD_1000000	= 1000000,   
    QL_UART_BAUD_1843200    = 1843200,
    QL_UART_BAUD_2000000    = 2000000,      //only support uart2/3
    QL_UART_BAUD_2100000    = 2100000,
    QL_UART_BAUD_3686400    = 3686400,      //only support uart2/3
    QL_UART_BAUD_4000000    = 4000000,      //only support uart2/3
	QL_UART_BAUD_4468750    = 4468750       //only support uart2/3
}ql_uart_baud_e;

typedef enum
{
	QL_UART_DATABIT_7 = 7,
	QL_UART_DATABIT_8 = 8,      //8910 ARM UART hardware only support 8bit Data
}ql_uart_databit_e;

typedef enum
{
	QL_UART_STOP_1  =1,         
	QL_UART_STOP_2  =2,
}ql_uart_stopbit_e;

typedef enum
{
	QL_UART_PARITY_NONE,
    QL_UART_PARITY_ODD,
	QL_UART_PARITY_EVEN,
}ql_uart_paritybit_e;

typedef enum
{
    QL_UART_TX_SENDING,
	QL_UART_TX_COMPLETE,
}ql_uart_tx_status_e;

typedef enum
{
    QL_UART_EVENT_RX_ARRIVED = (1 << 0),  ///< Received new data
    QL_UART_EVENT_RX_OVERFLOW = (1 << 1), ///< Rx fifo overflowed
    QL_UART_EVENT_TX_COMPLETE = (1 << 2)  ///< All data had been sent
}ql_uart_event_e;

typedef enum
{
    QL_SERIAL_EVENT_RX_ARRIVED = (1 << 0),  ///< received new data
    QL_SERIAL_EVENT_RX_OVERFLOW = (1 << 1), ///< rx overflow
    QL_SERIAL_EVENT_TX_COMPLETE = (1 << 2), ///< all data had been sent
    QL_SERIAL_EVENT_BROKEN = (1 << 3),      ///< serial broken
    QL_SERIAL_EVENT_READY = (1 << 4),       ///< serial ready
    QL_SERIAL_EVENT_OPEN = (1 << 5),        ///< open by host
    QL_SERIAL_EVENT_CLOSE = (1 << 6),       ///< close by host
} ql_serial_event_e;

typedef enum
{
    QL_UART_RX_DMA_MODE,                    ///< uart rx use DMA mode
    QL_UART_RX_FIFO_MODE,                   ///< uart rx use FIFO mode
}ql_uart_rx_mode_e;

/*===========================================================================
 * Struct
 ===========================================================================*/

typedef struct
{
	ql_uart_baud_e baudrate;
	ql_uart_databit_e data_bit;
	ql_uart_stopbit_e stop_bit;
	ql_uart_paritybit_e parity_bit;
	ql_uart_flowctrl_e flow_ctrl;
    uint rx_fifo_size; //default:
    uint tx_fifo_size; //default:
}ql_uart_config_s;


/*****************************************************************
* Description: UART callback
* 
* Parameters:
*   ind_type        [in]    事件类型，UART RX接收数据、RX buffer溢出、TX fifo发送完成。 
* 	port	  		[in] 	串口号
* 	size	  		[in] 	数据大小
*
*****************************************************************/
typedef void (*ql_uart_callback)(uint32 ind_type, ql_uart_port_number_e port, uint32 size);

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_uart_set_event_mask
*
* Description: 设置事件掩码，设置的事件会回调通知
* 
* Parameters:
*   port        [in]    串口选择。 
* 	event_mask	[in] 	事件掩码。 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_set_event_mask(ql_uart_port_number_e port, uint32_t event_mask);


/*****************************************************************
* Function: ql_uart_set_dcbconfig
*
* Description: 设置串口属性，设置完重新打开uart口生效
* 
* Parameters:
*   port        [in]    串口选择。 
* 	dcb	  		[in] 	串口属性配置结构体 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_set_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb);

/*****************************************************************
* Function: ql_uart_get_dcbconfig
*
* Description: 获取串口属性
* 
* Parameters:
*   port        [in]    串口选择。 
* 	dcb	  		[out] 	串口属性配置结构体 
*
* Return:
* 	0			获取成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_get_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb);

/*****************************************************************
* Function: ql_uart_open
*
* Description: 打开 UART 设备
* 
* Parameters:
*   port        [in]    串口选择。 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_open(ql_uart_port_number_e port);

/*****************************************************************
* Function: ql_uart_close
*
* Description: 关闭 UART 设备
* 
* Parameters:
*   port        [in]    串口选择。 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_close(ql_uart_port_number_e port);

/*****************************************************************
* Function: ql_uart_write
*
* Description: UART写
* 
* Parameters:
*   port        [in]    串口选择。 
* 	data	    [in] 	要写入的数据 
*   data_len    [in]    要写入的数据长度
*
* Return:
*   < 0         错误码
*	other 	    写入的实际字节长度。
*
*****************************************************************/
int ql_uart_write(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len);

/*****************************************************************
* Function: ql_uart_read
*
* Description: UART读
* 
* Parameters:
*   port        [in]    串口选择。 
* 	data	    [out] 	读取的数据 
*   data_len    [in]    要读取的数据长度
*
* Return:
*   < 0         错误码
*	other 	    实际读取的数据长度
*
*****************************************************************/
int ql_uart_read(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len);

/*****************************************************************
* Function: ql_uart_write_aviable
*
* Description: 获取写缓存剩余
* 
* Parameters:
*   port        [in]    串口选择。 
*
* Return:
*   < 0         错误码
*	other 	    写缓存空间剩余大小
*
*****************************************************************/
int ql_uart_write_aviable(ql_uart_port_number_e port);

/*****************************************************************
* Function: ql_uart_read_aviable
*
* Description: 获取读缓存剩余
* 
* Parameters:
*   port        [in]    串口选择。 
*
* Return:
*   < 0         错误码
*	other 	    读缓存空间数据大小
*
*****************************************************************/
int ql_uart_read_aviable(ql_uart_port_number_e port);

/*****************************************************************
* Function: ql_uart_register_cb
*
* Description: 注册UART 事件回调函数
* 
* Parameters:
*   port        [in]    串口选择。 
*   uart_cb     [in]    需要注册的回调函数，若为NULL,则取消注册callback，可使用轮询读取数据
*
* Return:
* 	0			注册成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_register_cb(ql_uart_port_number_e port, ql_uart_callback uart_cb);

/*****************************************************************
* Function: ql_uart_get_tx_fifo_status
*
* Description: 获取UART TX FIFO数据发送状态
* 
* Parameters:
*   port        [in]    串口选择。 
*   tx_status   [in]    TX FIFO状态
*
* Return:
* 	0			获取成功
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_get_tx_fifo_status(ql_uart_port_number_e port, ql_uart_tx_status_e *tx_status);

/*****************************************************************
* Function: ql_uart_set_simid
*
* Description: 设置串口绑定的SIM卡ID
* 
* Parameters:
*   port        [in]    串口选择
*   nSim        [in]    需要绑定的SIM卡ID，取值：0 - 1
* Return:
* 	0			设置成功
*	other 	    错误码
*
*****************************************************************/
ql_uart_errcode_e ql_uart_set_simid(ql_uart_port_number_e port,uint8_t nSim);

/*****************************************************************
* Function: ql_uart_get_simid
*
* Description: 获取当前串口绑定的SIM卡ID
* 
* Parameters:
*   port        [in]    串口选择
*   nSim        [out]   当前串口绑定的SIM卡ID
* Return:
* 	0			获取成功
*	other 	    错误码
*
*****************************************************************/
ql_uart_errcode_e ql_uart_get_simid(ql_uart_port_number_e port,uint8_t *nSim);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SLEEP
/*****************************************************************
* Function: ql_uart_set_sleep_delay_time
*
* Description: 设置UART无数据进入休眠的时间
* 
* Parameters:
*   times        [in]    设置的时间,单位:s。设置的最小值1s，最大值255s。 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_set_sleep_delay_time(uint8_t times);

/*****************************************************************
* Function: ql_uart_get_sleep_delay_time
*
* Description: 获取当前设置的UART无数据进入休眠的时间
* 
* Parameters:
*   times        [out]    获取到的时间,单位:s。 
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_get_sleep_delay_time(uint8_t *times);

/*****************************************************************
* Function: ql_uart_tx_delay_config
*
* Description: 修改发送数据间的间隔位
*              注意，该函数必须在串口打开之后调用
* 
* Parameters:
*   port        [in]    串口选择。 
*   delay_bits  [in]    间隔位位数,范围：（QL_UART_MIN_TX_DELAY_BITS-QL_UART_MAX_TX_DELAY_BITS）。 QL_UART_MIN_TX_DELAY_BITS:0              QL_UART_MAX_TX_DELAY_BITS:5
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_tx_delay_config(ql_uart_port_number_e port,uint8_t delay_bits);

/*****************************************************************
* Function: ql_uart_set_rx_mode
*
* Description: 修改串口RX的接收模式，默认是DMA模式
*              注意，该函数必须在串口打开之前调用，会有写NV的操作将接收模式配置保存，一般设置一次即可
* 
* Parameters:
*   port        [in]    串口选择。 
*   mode        [in]    RX接收模式。
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_set_rx_mode(ql_uart_port_number_e port, ql_uart_rx_mode_e mode);

/*****************************************************************
* Function: ql_uart_get_rx_mode
*
* Description: 获取串口RX的接收模式
* 
* Parameters:
*   port        [in]    串口选择。 
*   mode        [out]   RX接收模式。
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_get_rx_mode(ql_uart_port_number_e port, ql_uart_rx_mode_e *mode);

/*****************************************************************
* Function: ql_uart_rx_trig_config
*
* Description: 修改接收中断的触发值
*              注意，该函数必须在串口RX是FIFO模式下并且在串口打开之后调用，不同波特率允许的trig范围不同。
* 
* Parameters:
*   port        [in]    串口选择。 
*   rx_trig     [in]    接收中断阈值。baud = 0,不支持。
*                                   1200  <= baud < 28800, trig范围：1~100
*                                   28800 <= baud < 57600, trig范围：16~100
*                                   57600 <= baud < 115200, trig范围：32~100
*                                   115200 <= baud, trig范围：64~100
*
* Return:
* 	0			设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_uart_errcode_e ql_uart_rx_trig_config(ql_uart_port_number_e port,uint8_t rx_trig);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_UART_H */


