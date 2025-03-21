
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

#ifndef _QL_OHOS_HAL_GPIO_H_
#define _QL_OHOS_HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iot_errno.h"
#include "iot_gpio.h"

#define QL_OHOS_GPIO_INDEX_MAX 32

typedef struct ql_ohos_gpio_s
{
    unsigned char gpio_index_max;
    // struct list_head list; // 接口层数据

    unsigned int (*gpio_init)(unsigned int id);
    unsigned int (*gpio_set_dir)(unsigned int id, IotGpioDir dir);
    unsigned int (*gpio_get_dir)(unsigned int id, IotGpioDir *dir);
    unsigned int (*gpio_set_output_val)(unsigned int id, IotGpioValue val);
    unsigned int (*gpio_get_output_val)(unsigned int id, IotGpioValue *val);
    unsigned int (*gpio_get_input_val)(unsigned int id, IotGpioValue *val);
    unsigned int (*gpio_register_isr_func)(unsigned int id, IotGpioIntType int_type, IotGpioIntPolarity int_polarity, GpioIsrCallbackFunc func, char *arg);
    unsigned int (*gpio_unregister_isr_func)(unsigned int id);
    unsigned int (*gpio_set_isr_mask)(unsigned int id, unsigned char mask);
    unsigned int (*gpio_set_isr_mode)(unsigned int id, IotGpioIntType int_type, IotGpioIntPolarity int_polarity);
    unsigned int (*gpio_deinit)(unsigned int id);
}ql_ohos_gpio_t;

int ql_ohos_gpio_api_register(ql_ohos_gpio_t *gpio);

int ql_ohos_gpio_api_unregister(void);

int ql_ohos_gpio_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_GPIO_H_ */