/**  @file
  quec_boot_uart.h

  @brief
  This file is used to define boot uart api for different Quectel Project.

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
24/01/2021        Neo         Init version
=================================================================*/

#ifndef QUEC_BOOT_UART_H
#define QUEC_BOOT_UART_H


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "boot_fdl_channel.h"
#include "drv_names.h"
#include "quec_boot_pin_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
/*===========================================================================
 * Enum
 ===========================================================================*/ 
typedef struct
{
    ql_boot_uart_port_e port;
    uint8_t tx_pin;
    uint8_t tx_func;
    uint8_t rx_pin;
    uint8_t rx_func;
} quec_boot_uart_func_s;

typedef struct
{
	uint32_t		            dev_name;
	ql_boot_uart_port_e	        uart_port;
    fdlChannel_t*               channel;
} quec_boot_uart_info_t;

typedef struct
{
    uint32_t baudrate;
	uint32_t icf_format;
	uint32_t icf_parity;
	uint32_t ifc_rx; 
    uint32_t ifc_tx;
} quec_boot_uart_setting_s;


/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/*****************************************************************
* Function: quec_boot_fota_urc_output
*
* Description:
* output fota urc
* 
* Parameters:
*	block_count     [in]        upgrated block 
*   block        	[in]        total block 
*
* Return:
* 	NULL
*
*****************************************************************/
 void quec_boot_fota_urc_output(int block_count, int block);

/*****************************************************************
* Function: quec_boot_urc_init
*
* Description:
*  boot urc init.
* 
* Parameters:
*
* Return:
* 	0			    success
*	-1           	error	
*
*****************************************************************/
int quec_boot_urc_init();


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_UART_H */


