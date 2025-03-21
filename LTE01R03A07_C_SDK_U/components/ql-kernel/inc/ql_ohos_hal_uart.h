
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------
20230328         Jamie.li    Init version
=================================================================*/

#ifndef _QL_OHOS_HAL_UART_H_
#define _QL_OHOS_HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iot_errno.h"
#include "iot_uart.h"

typedef struct ql_ohos_uart_s
{
    unsigned int (*uart_init)(unsigned int id, const IotUartAttribute *param);
    int (*uart_read)(unsigned int id, unsigned char *data, unsigned int data_len);
    int (*uart_write)(unsigned int id, const unsigned char *data, unsigned int data_len);
    unsigned int (*uart_deinit)(unsigned int id);
    unsigned int (*uart_set_flow_ctrl)(unsigned int id, IotFlowCtrl flow_ctrl);
}ql_ohos_uart_t;

int ql_ohos_uart_api_register(ql_ohos_uart_t *uart);

int ql_ohos_uart_api_unregister(void);

int ql_ohos_uart_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_UART_H_ */