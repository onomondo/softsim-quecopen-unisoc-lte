/**  @file
  quec_boot_pin_index.h

  @brief
  This file is used to define boot pin index api for different Quectel Project.

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


#ifndef QUEC_BOOT_PIN_CFG_H
#define QUEC_BOOT_PIN_CFG_H


#include "quec_cust_feature.h"
#include "ql_api_common.h"
#include "hal_chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_BOOT_ERRCODE_BASE (QL_COMPONENT_BSP_GPIO<<16)

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * GPIO errcode
 */
typedef enum
{
    QL_BOOT_PIN_SUCCESS = 0,

    QL_BOOT_PIN_INVALID_PARAM_ERR           = 1|QL_BOOT_ERRCODE_BASE,
    QL_BOOT_PIN_MAP_ERR,
    QL_BOOT_PIN_SET_PWR_DOMIAN_ERR,
    QL_BOOT_PIN_SET_PWR_INVALID_PARAM_ERRR,
    
}ql_boot_errcode_pin_e;

//==================================================================

typedef enum
{
    QUEC_BOOT_V_NONE    = 0,                     //V_PAD_1V8和V_PAD_1V8_RTC 芯片初始值默认打开 V_MMC芯片上电默认打开3V2
    QUEC_BOOT_V_LCD,                             //V_LCD 电压域芯片初始值默认关闭 要自行打开
    QUEC_BOOT_V_SIM1                             // hardware is SIM2, = HAL_POWER_SIM1
}quec_boot_pd_type_e;

typedef struct
{
    //uint8_t             pin_num;
    volatile uint32_t   *reg;
    uint8_t             function;
    quec_boot_pd_type_e power_domain;
    uint32_t            power_mv;
} quec_boot_pin_cfg_t;
//==================================================================
typedef enum 
{
    QL_BOOT_UART_PORT_1 = 0,
    QL_BOOT_UART_PORT_2,
    QL_BOOT_UART_PORT_3,
    QL_BOOT_UART_PORT_MAX
}ql_boot_uart_port_e;

typedef struct
{
    ql_boot_uart_port_e port;
    quec_boot_pin_cfg_t pin_tx;
    quec_boot_pin_cfg_t pin_rx;
} ql_boot_uart_pin_cfg_t;
//==================================================================

typedef enum
{
    QL_BOOT_SPI_PORT_1 = 0,                               //SPI1总线
    QL_BOOT_SPI_PORT_2,                                   //SPI2总线
    QL_BOOT_SPI_PORT_MAX
}ql_boot_spi_port_e;
    
typedef struct
{
    ql_boot_spi_port_e  port;
    quec_boot_pin_cfg_t pin_clk;
    quec_boot_pin_cfg_t pin_cs;
    quec_boot_pin_cfg_t pin_mosi;
    quec_boot_pin_cfg_t pin_miso;
} ql_boot_spi_pin_cfg_t;

   
typedef struct
{
    ql_boot_spi_port_e  port;
    quec_boot_pin_cfg_t pin_clk;
    quec_boot_pin_cfg_t pin_cs;
    quec_boot_pin_cfg_t pin_sio0;
    quec_boot_pin_cfg_t pin_sio1;
    quec_boot_pin_cfg_t pin_sio2;
    quec_boot_pin_cfg_t pin_sio3;    
} ql_boot_spi6_pin_cfg_t;

typedef struct
{
    uint8_t channel_adc0;
    uint8_t channel_adc1;
    uint8_t channel_adc2;
    uint8_t channel_adc3;
    uint8_t channel_vbat;
} ql_boot_adc_channel_s;

typedef struct
{
    bool gnss_enabled;
}ql_boot_model_diff_ctx_s;

/*==================================================================
 ******************************uart*********************************
 ==================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC
#define QL_BOOT_UART_PORT_NUM_MAX (2) //UART1默认的 UART2/UART3需要set func 
extern ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX];
#endif
/*==================================================================
 *******************************spi*********************************
 ==================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_BUS
#define QL_BOOT_SPI_PORT_NUM_MAX (2) //最多支持两组4线spi
extern ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX];
#define QL_BOOT_CUR_SPI_PORT     QL_BOOT_SPI_PORT_1   //当前boot中使用的4线spi口
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND)
/*======================================================================================
**注：SPI6 PORT其实是一个6线SPI控制器复用到不同的pin脚上，所以不能同时复用。
**    例如EC200 默认QL_BOOT_SPI_PORT_1所对应pin脚复用成6线SPI功能，如果想切换
**    成QL_BOOT_SPI_PORT_2所对应pin脚复用成6线SPI功能，要先把QL_BOOT_SPI_PORT_1所对应
**    pin脚复用成其他功能(比如gpio等),再把QL_BOOT_SPI_PORT_2所对应pin脚复用成6线SPI功能。
**    PS:pin默认功能表:ql_pin_cfg.c
=======================================================================================*/
#define QL_BOOT_SPI6_PORT_NUM_MAX (2) 
extern ql_boot_spi6_pin_cfg_t quec_boot_spi6_pin_cfg_map[QL_BOOT_SPI6_PORT_NUM_MAX];
#define QL_BOOT_CUR_SPI6_PORT    QL_BOOT_SPI_PORT_1   //当前boot中使用的6线spi口
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
extern const ql_boot_adc_channel_s ql_boot_adc_channel_num;
#endif

/*========================================================================
 *  function Definition
 *========================================================================*/
 
/*****************************************************************
* Function: ql_boot_set_power_domain
*
* Description:
*   设置电压域
* 
* Parameters:
*   type        [in]    电压域类型选择
*
* Return:ql_boot_errcode
*
*****************************************************************/
ql_boot_errcode_pin_e ql_boot_set_power_domain(quec_boot_pd_type_e type,powerLevel_t mv);

ql_boot_errcode_pin_e quec_boot_pin_init(quec_boot_pin_cfg_t *pin, uint32_t *reg_bak, uint32_t *pad_reg_bak);
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_PIN_CFG_H */


