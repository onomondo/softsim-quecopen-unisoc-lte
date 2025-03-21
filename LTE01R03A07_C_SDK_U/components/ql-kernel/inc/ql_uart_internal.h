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

#ifndef QL_UART_INTERNAL_H
#define QL_UART_INTERNAL_H

#include <stdint.h>
#include <stdbool.h>
#include "quec_common.h"
#include "quec_sio_adapter.h"
#include "ql_uart.h"
#include "ql_api_osi.h"
#include "quec_dispatch.h"
#include "atr_config.h"
#include "ql_api_virt_at.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QUEC_TX_MAX_CONTENT_SIZE 20*1024
#define QUEC_TX_MAX_HEADER_SIZE	1024
#define QL_UART_TX_FIFO_SIZE   (QUEC_TX_MAX_CONTENT_SIZE+QUEC_TX_MAX_HEADER_SIZE)

#ifdef CONFIG_AT_USB_REDUCE_MEMORY
#define USERIAL_RX_BUF_SIZE (128 * 1024)
#define USERIAL_TX_BUF_SIZE (64 * 1024)
#define USERIAL_RX_DMA_SIZE (2048)
#define USERIAL_TX_DMA_SIZE (2048)
#else
#define USERIAL_RX_BUF_SIZE (256 * 1024)
#define USERIAL_TX_BUF_SIZE (128 * 1024)
#define USERIAL_RX_DMA_SIZE (2048)
#define USERIAL_TX_DMA_SIZE (2048)
#endif

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
	DEV_NONE = -1,
	DEV_UART1 = 	QL_UART_PORT_1,
	DEV_UART2 = 	QL_UART_PORT_2,
	DEV_UART3 = 	QL_UART_PORT_3,
	DEV_USB_AT = 	QL_USB_PORT_AT,
	DEV_USB_MODEM = QL_USB_PORT_MODEM,
	DEV_USB_NMEA  = QL_USB_PORT_NMEA,
	DEV_USB_ACM0  = QL_USB_PORT_ACM0,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CMUX
	DEV_CMUX1,
	DEV_CMUX2,
	DEV_CMUX3,
	DEV_CMUX4,
#endif
    DEV_USB_DIAG,
	DEV_MAX,
} quec_dev_e;

typedef enum 
{
	QL_UART_CLOSE,
	QL_UART_AT_OPEN,
	QL_UART_APP_OPEN,
}ql_uart_open_staus_e;

typedef enum
{
    DCD_OFF,
    DCD_ON,
}quec_dcd_e;

typedef struct
{
	atDevice_t				*device;
	ql_dispatch_t			*dispatch;
	ql_uart_open_staus_e	status;
	ql_mutex_t				lock;
    ql_uart_callback		uart_cb;
    uint8_t                 nSim;       //for DS-DS
} ql_dev_info_t;

typedef struct
{
	char * 			port_name;
	uint32_t		drv_name;
} ql_dev_name_info_s;

//UART1
extern const uint32_t baud_list[];
extern const uint32_t cbaud_list[];
extern const uint32_t autobaud_list[];
extern unsigned baud_list_size;
extern unsigned cbaud_list_size;
extern unsigned autobaud_list_size;
//UART2/3
extern const uint32_t uart23_baud_list[];
extern unsigned uart23_baud_list_size;

/*========================================================================
*	function Definition
*========================================================================*/
ql_dispatch_t *quec_get_dispatch_by_dev(uint32_t dev);

quec_dev_e quec_get_dev_by_dispatch(ql_dispatch_t *dispatch);

bool quec_uart_at_device_create(ql_uart_port_number_e port);

uint8_t quec_get_uart_status(uint32_t dev);

bool quec_uart_is_allow_create(uint32_t drv_name);

void quec_at_device_create_success(uint32_t drv_name, atDevice_t *device, ql_dispatch_t *dispatch);

bool quec_uart_pin_init(ql_uart_port_number_e port);

void quec_write_to_at_dev(quec_dev_e dev,unsigned char * data_buff,unsigned int len);

int quec_at_dispatch_reset(ql_dispatch_t *th);

int quec_at_dispatch_read(ql_dispatch_t *th, uint8 *data, uint len);

ql_dispatch_t *quec_dispatch_create(atDevice_t *device, uint32 name, void *cmux_engine, uint8 dlci);

int quec_at_dispatch_read_avail(ql_dispatch_t *th);

int quec_at_dispatch_write_avail(ql_dispatch_t *th);

atDevice_t *quec_dispatch_get_device(ql_dispatch_t *th);

bool quec_dispatch_bind_sim(ql_dispatch_t* dispatch, uint8_t nSim);

uint8_t quec_get_sim_by_dispatch(ql_dispatch_t *dispatch);

bool quec_at_userial_create(uint32 name, uint rx_size, uint tx_size);

void quec_set_dcd_level(uint8_t dev, quec_dcd_e state);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VIRT_AT
int ql_virt_at_set_simid(ql_virt_at_port_number_e port, uint8 sim_id);

int ql_virt_at_get_simid(ql_virt_at_port_number_e port);
#endif

#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_UART_INTERNAL_H*/






