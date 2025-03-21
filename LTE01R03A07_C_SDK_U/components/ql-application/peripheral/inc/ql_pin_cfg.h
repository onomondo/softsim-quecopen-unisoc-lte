/**  @file
  ql_pin_cfg.h

  @brief
  This file provides the definitions for ql pin cfg, and declares the API functions.

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
15/03/2021        Neo         Init version
=================================================================*/



#ifndef QL_PIN_CFG_H
#define QL_PIN_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "ql_type.h"
#include "ql_gpio.h"
#include "ql_app_feature_config.h"
#include "quec_pin_index.h"

/*===========================================================================
 * Macro
 ===========================================================================*/
#define QUEC_PIN_NONE    0xff

//different projects are changed into different corresponding relationships
//for power demo
#define QL_FUN_NUM_UART_2_CTS      3
#define QL_FUN_NUM_UART_3_TXD      4

//for ledcfg demo
#ifdef CONFIG_QL_PROJECT_DEF_EG912U
//EG912型号QUEC_PIN_CFG_MAX值统计为kernel使用pin脚，根据GNSS和负载开关宏控，会增加6个虚拟的pin脚，在APP侧不能使用
#define QL_GPIO_PIN_MAX            45
#else
#define QL_GPIO_PIN_MAX            QUEC_PIN_CFG_MAX
#endif

#define QL_PIN_NUM_KEYOUT_5        QUEC_PIN_DNAME_KEYOUT_5

#define QL_PIN_GPIO1                        126
#define QL_PIN_GPIO1_FUNC_ZSP_CTS           3
#define QL_PIN_FLASH_DATA2                  79
#define QL_PIN_FLASH_DATA2_FUNC_FLASH_SIO2  1


#ifdef QL_APP_PROJECT_DEF_EC200U
#define QL_PIN_NET_MODE            6
#define QL_PIN_NET_MODE_FUNC_GPIO  4
#define QL_GPIO_NET_MODE           22

#define QL_PIN_SIM0_HOTPLUG              13
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_4

#elif defined QL_APP_PROJECT_DEF_EC600U
#define QL_PIN_NET_MODE            52
#define QL_PIN_NET_MODE_FUNC_GPIO  2
#define QL_GPIO_NET_MODE           30

#define QL_PIN_SIM0_HOTPLUG              9
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_8


#elif defined QL_APP_PROJECT_DEF_EG700U || defined QL_APP_PROJECT_DEF_EG070U
#define QL_PIN_NET_MODE            81
#define QL_PIN_NET_MODE_FUNC_GPIO  1
#define QL_GPIO_NET_MODE           4

#define QL_PIN_SIM0_HOTPLUG              22
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_23

#elif defined QL_APP_PROJECT_DEF_EG500U
#define QL_PIN_NET_MODE            72
#define QL_PIN_NET_MODE_FUNC_GPIO  2
#define QL_GPIO_NET_MODE           30

#define QL_PIN_SIM0_HOTPLUG              17
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_5

#elif defined QL_APP_PROJECT_DEF_EG915U
#define QL_PIN_NET_MODE            QUEC_PIN_NONE
#define QL_PIN_NET_MODE_FUNC_GPIO  QUEC_PIN_NONE
#define QL_GPIO_NET_MODE           QUEC_PIN_NONE

#define QL_PIN_SIM0_HOTPLUG              42
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_23

#elif defined QL_APP_PROJECT_DEF_EC020U
#define QL_PIN_NET_MODE            6
#define QL_PIN_NET_MODE_FUNC_GPIO  4
#define QL_GPIO_NET_MODE           19

#define QL_PIN_SIM0_HOTPLUG              13
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_14

#elif defined QL_APP_PROJECT_DEF_EG912U
#define QL_PIN_NET_MODE            QUEC_PIN_NONE
#define QL_PIN_NET_MODE_FUNC_GPIO  QUEC_PIN_NONE
#define QL_GPIO_NET_MODE           QUEC_PIN_NONE

#define QL_PIN_SIM0_HOTPLUG              42
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_23

#endif

//for sdmmc demo
#define QL_SDMMC_PIN_DET            QUEC_PIN_DNAME_GPIO_15
#define QL_PIN_SDMMC_CMD            QUEC_PIN_DNAME_SDMMC1_CMD
#define QL_PIN_SDMMC_DATA_0         QUEC_PIN_DNAME_SDMMC1_DATA_0
#define QL_PIN_SDMMC_DATA_1         QUEC_PIN_DNAME_SDMMC1_DATA_1
#define QL_PIN_SDMMC_DATA_2         QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QL_PIN_SDMMC_DATA_3         QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QL_PIN_SDMMC_CLK            QUEC_PIN_DNAME_SDMMC1_CLK

#define QL_PIN_SDMMC_MODE_FUNC      0
#define QL_PIN_SDMMC_MODE_FUNC_GPIO 0

//for gpio/gpio_int demo
#define QL_TEST1_PIN_GPIO0                  QUEC_PIN_DNAME_GPIO_0
#define QL_TEST1_PIN_GPIO0_FUNC_GPIO        0
#define QL_TEST1_PIN_GPIO0_FUNC_FLASH1_CLK  1
#define QL_TEST1_PIN_GPIO1                  QUEC_PIN_DNAME_GPIO_1
#define QL_TEST1_PIN_GPIO1_FUNC_GPIO        0
#define QL_TEST1_PIN_GPIO1_FUNC_FLASH1_CS   1

#define QL_TEST2_PIN_GPIO0                  QUEC_PIN_DNAME_SPI_LCD_SIO
#define QL_TEST2_PIN_GPIO0_FUNC_GPIO        1
#define QL_TEST2_PIN_GPIO0_FUNC_LCD_SIO     0
#define QL_TEST2_PIN_GPIO1                  QUEC_PIN_DNAME_SPI_LCD_SDC
#define QL_TEST2_PIN_GPIO1_FUNC_GPIO        1
#define QL_TEST2_PIN_GPIO1_FUNC_LCD_SDC     0

//for rs485 demo
#define QL_RS485_PIN_GPIO                   QUEC_PIN_DNAME_SDMMC1_DATA_1
#define QL_RS485_PIN_GPIO_FUNC_GPIO         1
#define QL_RS485_GPIO_NUM                   GPIO_26

//for uart demo
#define QL_UART2_TX_PIN                     QUEC_PIN_UART2_TXD
#define QL_UART2_RX_PIN                     QUEC_PIN_UART2_RXD

#ifdef QL_APP_PROJECT_DEF_EC200U
#define QL_UART2_TX_FUNC                    0x03
#define QL_UART2_RX_FUNC                    0x03
#else
#define QL_UART2_TX_FUNC                    0x01
#define QL_UART2_RX_FUNC                    0x01
#endif

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint8_t             pin_num;
    uint8_t             default_func;
    uint8_t             gpio_func;
    ql_GpioNum          gpio_num;
    ql_GpioDir          gpio_dir;
    ql_PullMode         gpio_pull;
    ql_LvlMode          gpio_lvl;
} ql_pin_cfg_t;

/*===========================================================================
 * Extern
 ===========================================================================*/
extern const ql_pin_cfg_t ql_pin_cfg_map[];


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_PIN_CFG_H */


