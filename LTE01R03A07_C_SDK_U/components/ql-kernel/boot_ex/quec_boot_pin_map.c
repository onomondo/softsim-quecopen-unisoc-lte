/**  
  @file
  quec_boot_pin_index.c

  @brief
  quectel boot pin index interface.

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
16/06/2021        Sum         Init version
=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
//#include "quec_boot_pin_index.h"
#include "quec_cust_feature.h"
#include "hwregs.h"
#include "quec_boot_pin_cfg.h"
#include "ql_type.h"
#include "drv_adc.h"

#include "stdio.h"

const ql_boot_model_diff_ctx_s ql_boot_model_diff_ctx = 
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
    true
#else
    false
#endif
};

/**********************************************************************************************
*>>>如何修改某个pin的复用功能
* 用复用pin对应的reg，func，替换下map数组中的reg，func，根据所选的pin所在的电压域，填写是否要
* 开启电压域。
* 注：
* 1.pin脚对应的reg在quec_pin_cfg_ECXXX.c中可以找到。
* 2.pin脚的func和power domain 在EC200U&EC600U&EG700U&EG500U配置_UIS8910DM_GPIO_Spec_VX.X.xlsx
*   excel表中可以找到。
*
**********************************************************************************************/
/*==================================================================
 ******************************uart*********************************
 ==================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC
//UART1默认的 UART2/UART3需要set func
//#define QL_BOOT_UART_PORT_NUM_MAX (2) 
#ifdef CONFIG_QL_PROJECT_DEF_EC200U
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain       mv   */    
/*UART2_TX   */{  &hwp_iomux->pad_keyin_5_cfg_reg      ,       3 ,   QUEC_BOOT_V_NONE ,  POWER_LEVEL_UNUSED         },
/*UART2_RX   */{  &hwp_iomux->pad_keyin_4_cfg_reg      ,       3 ,   QUEC_BOOT_V_NONE ,  POWER_LEVEL_UNUSED         },
    },
    {  
        QL_BOOT_UART_PORT_3,
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  &hwp_iomux->pad_keyout_5_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE ,  POWER_LEVEL_UNUSED         },
/*UART3_RX   */{  &hwp_iomux->pad_keyout_4_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE ,  POWER_LEVEL_UNUSED         },
    }
};
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain  */    
/*UART2_TX   */{  &hwp_iomux->pad_gpio_21_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART2_RX   */{  &hwp_iomux->pad_gpio_20_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_UART_PORT_3,
#ifndef __QUEC_OEM_VER_LD__                
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  &hwp_iomux->pad_keyout_5_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  &hwp_iomux->pad_keyout_4_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#else          
/*UART3_TX   */{  &hwp_iomux->pad_sdmmc1_data_3_cfg_reg,       6 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  &hwp_iomux->pad_sdmmc1_data_2_cfg_reg,       6 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#endif
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG700U) || defined (CONFIG_QL_PROJECT_DEF_EG070U) \
    || (defined CONFIG_QL_PROJECT_DEF_EC020U)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain  */    
/*UART2_TX   */{  &hwp_iomux->pad_gpio_21_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART2_RX   */{  &hwp_iomux->pad_gpio_20_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_UART_PORT_3,
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  &hwp_iomux->pad_keyout_5_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE   ,  POWER_LEVEL_UNUSED       },
/*UART3_RX   */{  &hwp_iomux->pad_keyout_4_cfg_reg     ,       4 ,   QUEC_BOOT_V_NONE   ,  POWER_LEVEL_UNUSED       },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG500U)
//EG500U只支持UART2，定义UART3只是为了统一，可以编译通过
//EG500U only support UART2，definition of UART3 is just for unification, and can be compiled passed
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain  */    
/*UART2_TX   */{  &hwp_iomux->pad_gpio_21_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART2_RX   */{  &hwp_iomux->pad_gpio_20_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_UART_PORT_3,
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG915U)
//EG915U只支持UART2，定义UART3只是为了统一，可以编译通过
//EG915U only support UART2，definition of UART3 is just for unification, and can be compiled passed
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain  */    
/*UART2_TX   */{  &hwp_iomux->pad_gpio_21_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART2_RX   */{  &hwp_iomux->pad_gpio_20_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_UART_PORT_3,
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG912U)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain  */    
/*UART2_TX   */{  &hwp_iomux->pad_gpio_21_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART2_RX   */{  &hwp_iomux->pad_gpio_20_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
                QL_BOOT_UART_PORT_3,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
    //带GNSS的型号，UART3接到了GNSS，无法使用UART3
                  /*                reg                           func     power_domain  */             
/*UART3_TX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#else
              /*                reg                           func     power_domain  */             
/*UART3_TX   */{  &hwp_iomux->pad_sdmmc1_data_3_cfg_reg,       6 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*UART3_RX   */{  &hwp_iomux->pad_sdmmc1_data_2_cfg_reg,       6 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#endif
    }
};
#endif
#endif
/*==================================================================
 *******************************spi*********************************
 ==================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_BUS
//#define QL_BOOT_SPI_PORT_NUM_MAX (2)
uint8_t ql_boot_cur_spi4_port  = QL_BOOT_CUR_SPI_PORT;
#ifdef CONFIG_QL_PROJECT_DEF_EC200U
//#define QL_EC200U_WITH_GNSS        1        //1-with GNSS,0-without GNSS 
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
//#if QL_EC200U_WITH_GNSS /* with GNSS */
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS 
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_7_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_ap_jtag_tck_cfg_reg  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_ap_jtag_trst_cfg_reg ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_ap_jtag_tms_cfg_reg  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#else /* without GNSS */
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
#endif/* QL_EC200U_WITH_GNSS */
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
//使用GPIO的方式来控制SPI CS引脚
///*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG700U) || defined (CONFIG_QL_PROJECT_DEF_EG070U)
//EG700U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG700U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG500U)
//EG500U只支持SPI2，定义SPI1只是为了统一，可以编译通过
//EG500U only support SPI2，definition of SPI1 is just for unification, and can be compiled passed
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  NULL                                 ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG915U)
//EG915U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG915U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EC020U)
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */
/*SPI_2_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG912U)
//EG912U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG912U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_9_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MOSI */{  &hwp_iomux->pad_gpio_11_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_MISO */{  &hwp_iomux->pad_gpio_12_cfg_reg      ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */             
/*SPI_2_CLK  */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_CS   */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MOSI */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_2_MISO */{  NULL                                 ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    }
};
#endif
#endif
/*==================================================================
 *******************************spi6*********************************
 ==================================================================*/
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
__attribute__((weak)) uint8_t ql_spi6_clk_div = 0xb;
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND)
//#define QL_BOOT_SPI6_PORT_NUM_MAX (2) 
uint8_t ql_boot_cur_spi6_port  = QL_BOOT_CUR_SPI6_PORT;
ql_boot_spi6_pin_cfg_t quec_boot_spi6_pin_cfg_map[QL_BOOT_SPI6_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain  */    
/*SPI_1_CLK  */{  &hwp_iomux->pad_gpio_0_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_CS   */{  &hwp_iomux->pad_gpio_1_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_SIO0 */{  &hwp_iomux->pad_gpio_2_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_SIO1 */{  &hwp_iomux->pad_gpio_3_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_SIO2 */{  &hwp_iomux->pad_gpio_4_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
/*SPI_1_SIO3 */{  &hwp_iomux->pad_gpio_5_cfg_reg       ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED        },
    },
    {  
/*
  lcd power_domain: POWER_LEVEL_1700MV --POWER_LEVEL_3200MV
  
  If your module supports GPS, the LCD voltage range can only be set to 1800mV to 1900mV. 
  Do not go beyond this range, otherwise you will burn out the GPS chip, 
  because the GPS chip is also powered by the LCD voltage range.
*/
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain  */ 
/*SPI_2_CLK  */{  &hwp_iomux->pad_spi_lcd_sio_cfg_reg   ,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
/*SPI_2_CS   */{  &hwp_iomux->pad_spi_lcd_sdc_cfg_reg   ,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
/*SPI_2_SIO0 */{  &hwp_iomux->pad_spi_lcd_clk_cfg_reg   ,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
/*SPI_2_SIO1 */{  &hwp_iomux->pad_spi_lcd_cs_cfg_reg    ,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
/*SPI_2_SIO2 */{  &hwp_iomux->pad_spi_lcd_select_cfg_reg,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
/*SPI_2_SIO3 */{  &hwp_iomux->pad_lcd_fmark_cfg_reg     ,      2 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV        },
    }
};
#endif

/********************************************************************************
*                    ADC channel corresponds to the drv
*
*           EC200U             EG500U          EC600U          EG700U          EG915U             EG912U
* ADC0      ADC_CHANNEL_2      ADC_CHANNEL_2   ADC_CHANNEL_2   ADC_CHANNEL_1   ADC_CHANNEL_1      ADC_CHANNEL_1
* ADC1      ADC_CHANNEL_3      ADC_CHANNEL_3   ADC_CHANNEL_3   ADC_CHANNEL_2   ADC_CHANNEL_2      ADC_CHANNEL_2
* ADC2      ADC_CHANNEL_4      ADC_CHANNEL_1   ADC_CHANNEL_4   ADC_CHANNEL_3   ADC_CHANNEL_NONE   ADC_CHANNEL_NONE
* ADC3      ADC_CHANNEL_NONE   ADC_CHANNEL_4   ADC_CHANNEL_1   ADC_CHANNEL_4   ADC_CHANNEL_NONE   ADC_CHANNEL_NONE
*********************************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
#define ADC_CHANNEL_NONE      (-1)
const ql_boot_adc_channel_s ql_boot_adc_channel_num =
   /*      ADC0              ADC1                ADC2                ADC3                   VBAT        */
#if defined CONFIG_QL_PROJECT_DEF_EC200U || defined CONFIG_QL_PROJECT_DEF_EC020U
    {  ADC_CHANNEL_2,     ADC_CHANNEL_3,     ADC_CHANNEL_4,     ADC_CHANNEL_NONE  , ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
    {  ADC_CHANNEL_2,     ADC_CHANNEL_3,     ADC_CHANNEL_4,     ADC_CHANNEL_1     , ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EG700U || defined CONFIG_QL_PROJECT_DEF_EG070U
    {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3,     ADC_CHANNEL_4     , ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EG500U
    {  ADC_CHANNEL_2,     ADC_CHANNEL_3,     ADC_CHANNEL_1,     ADC_CHANNEL_4     , ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EG915U || defined CONFIG_QL_PROJECT_DEF_EG912U
    {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_NONE,  ADC_CHANNEL_NONE  , ADC_CHANNEL_VBATSENSE};
#endif
#endif


