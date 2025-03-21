/**  
  @file
  ql_pin_cfg.c

  @brief
  quectel pin cfg.

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



/*===========================================================================
 * include files
 ===========================================================================*/
#include "ql_pin_cfg.h"

/*===========================================================================
 * GPIO Map
 ===========================================================================*/
const ql_pin_cfg_t ql_pin_cfg_map[] = /* pin initialize */
#ifdef QL_APP_PROJECT_DEF_EC200U /* open */
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl  */
/*PCM_CLK    */{  27 ,        1,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SIO    */{  125,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_SYNC   */{  26 ,        1,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SDC    */{  124,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_IN     */{  24 ,        1,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CLK    */{  123,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_OUT    */{  25 ,        1,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CS     */{  122,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM_DET   */{  13 ,        1,              0,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SEL    */{  121,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WLAN_WAKE  */{  135,        1,              0,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_FMARK  */{  119,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_CLK  */{  133,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*SLEEP_IND  */{  3  ,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_CMD  */{  134,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  40 ,        0,              0,        GPIO_9,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_DATA0*/{  132,        1,              5,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  37 ,        0,              0,        GPIO_10,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_DATA1*/{  131,        1,              5,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  38 ,        0,              0,        GPIO_11,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_DATA2*/{  130,        1,              5,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  39 ,        0,              0,        GPIO_12,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_DATA3*/{  129,        1,              5,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  5  ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },// hardware pin name is NET_MODE
/*I2C2_SCL   */{  141,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C2_SDA   */{  142,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SCL    */{  41 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  42 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  65 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  61 ,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  64 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RI    */{  62 ,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WLAN_EN    */{  136,        0,              0,        GPIO_20,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DCD   */{  63 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*BT_EN      */{  139,        0,              0,        GPIO_21,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DTR   */{  66 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*GPIO1      */{  126,        0,              0,        GPIO_22,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*NET_MODE   */{  6  ,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// hardware pin name is NET_STATUS
/*WLAN_PWR_EN*/{  127,        0,              0,        GPIO_23,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SD_DET     */{  23 ,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_CMD  */{  33 ,        0,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA0*/{  31 ,        0,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA1*/{  30 ,        0,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA2*/{  29 ,        0,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA3*/{  28 ,        0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#if defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS) //resolved for CLK, DATA and RST pins of SIM2 in double sim scenario
/*WAKEUP_IN  */{  1  ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*AP_READY   */{  2  ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*W_DISABLE  */{  4  ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#else
/*WAKEUP_IN  */{  1  ,        2,              2,        GPIO_29,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*AP_READY   */{  2  ,        2,              2,        GPIO_30,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*W_DISABLE  */{  4  ,        2,              2,        GPIO_31,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
#endif // defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS)
//pinmux(not gpio part)
/*SDIO1_CLK  */{  32 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT4    */{  81 ,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT5    */{  82 ,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*AUX_RXD    */{  137,        3,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*AUX_TXD    */{  138,        3,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EC600U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*PCM_CLK    */{  61 ,        3,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SIO    */{  66 ,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_SYNC   */{  58 ,        3,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SDC    */{  63 ,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_IN     */{  59 ,        3,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CLK    */{  67 ,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_OUT    */{  60 ,        3,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CS     */{  65 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  69 ,        0,              0,        GPIO_4,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SEL    */{  137,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  70 ,        0,              0,        GPIO_5,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_FMARK  */{  62 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_CLK  */{  118,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*USIM_DET   */{  9  ,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_CMD  */{  99 ,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  1  ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA0*/{  98 ,        1,              5,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  4  ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA1*/{  95 ,        5,              5,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH      },
/*SPI_1_MOSI */{  3  ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA2*/{  119,        1,              5,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  2  ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA3*/{  100,        1,              5,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  54 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*I2C2_SCL   */{  57 ,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C2_SDA   */{  56 ,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SCL    */{  11 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  12 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  34 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  120,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  33 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  16 ,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  123,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  10 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  124,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  14 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RTS  */{  122,        1,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA1  */{  15 ,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_CTS  */{  121,        1,              0,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_SCK    */{  13 ,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_DCD   */{  48 ,        1,              1,        GPIO_24,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DTR   */{  39 ,        1,              1,        GPIO_25,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_RI    */{  40 ,        1,              1,        GPIO_26,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*WAKEUP_IN  */{  49 ,        1,              1,        GPIO_27,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*AP_READY   */{  50 ,        1,              1,        GPIO_28,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
#if defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS) //resolved for CLK, DATA and RST pins of SIM2 in double sim scenario
/*SLEEP_IND  */{  53 ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_MODE   */{  52 ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*W_DISABLE  */{  51 ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#else
/*SLEEP_IND  */{  53 ,        2,              2,        GPIO_29,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*NET_MODE   */{  52 ,        2,              2,        GPIO_30,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*W_DISABLE  */{  51 ,        2,              2,        GPIO_31,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
#endif // defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS)
//pinmux(not gpio part)
/*SDIO1_CLK  */{  132,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT4    */{  104,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT5    */{  103,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN4     */{  126,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN5     */{  125,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EG700U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*FLASH_CLK  */{  34 ,        1,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*W_DISABLE  */{  84 ,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_CS   */{  35 ,        1,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  85 ,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA0*/{  77 ,        1,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WAKEUP_IN  */{  43 ,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA1*/{  78 ,        1,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*AP_READY   */{  42 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA2*/{  79 ,        1,              0,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_MODE   */{  81 ,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA3*/{  80 ,        1,              0,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SLEEP_IND  */{  82 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  46 ,        0,              0,        GPIO_7,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },// pay attention to USB
/*MAIN_DTR   */{  55 ,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SPI_1_CLK  */{  17 ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  16 ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  19 ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  18 ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  45 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DCD   */{  86 ,        0,              0,        GPIO_14,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_RI    */{  87 ,        0,              0,        GPIO_15,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*I2C_SCL    */{  28 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  29 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  1  ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  75 ,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  90 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  98 ,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  64 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  99 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  63 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  76 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CP_TXD     */{  20 ,        5,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA1  */{  100,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM_DET   */{  22 ,        0,              0,        GPIO_23,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_SCK    */{  101,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_CMD  */{  21 ,        0,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA0*/{  72 ,        0,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA1*/{  73 ,        0,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA2*/{  69 ,        0,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA3*/{  68 ,        0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
//pinmux(not gpio part)
/*SDIO1_CLK  */{  66 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT4    */{  24 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT5    */{  23 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EG500U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*MAIN_DTR   */{  46 ,        0,              0,        GPIO_0,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SIO    */{  19 ,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RI    */{  13 ,        0,              0,        GPIO_1,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SDC    */{  24 ,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_DCD   */{  45 ,        0,              0,        GPIO_2,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_CLK    */{  20 ,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_DSR   */{  44 ,        0,              0,        GPIO_3,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_CS     */{  25 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  54 ,        0,              0,        GPIO_4,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SEL    */{  26 ,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM_DET   */{  17 ,        0,              0,        GPIO_5,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_FMARK  */{  27 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  55 ,        0,              0,        GPIO_7,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },// pay attention to USB
/*HEADSET_DET*/{  56 ,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*NET_STATUS */{  53 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*I2C2_SCL   */{  35 ,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C2_SDA   */{  36 ,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SCL    */{  29 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  28 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  14 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  85 ,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  11 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  84 ,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  30 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  83 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  31 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  86 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RTS  */{  32 ,        1,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA1  */{  87 ,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_CTS  */{  33 ,        1,              0,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_SCK    */{  88 ,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_CMD  */{  81 ,        0,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA0*/{  90 ,        0,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA1*/{  97 ,        0,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA2*/{  101,        0,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA3*/{  111,        0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WAKEUP_IN  */{  120,        2,              2,        GPIO_29,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*NET_MODE   */{  72 ,        2,              2,        GPIO_30,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*W_DISABLE  */{  43 ,        2,              2,        GPIO_31,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
//pinmux(not gpio part)
/*SDIO1_CLK  */{  80 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT4    */{  75 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT5    */{  76 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN4     */{  78 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN5     */{  79 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EG915U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl    */
/*PCM_CLK    */{  4  ,        3,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SIO    */{  116,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_SYNC   */{  5  ,        3,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SDC    */{  106,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_IN     */{  6  ,        3,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CLK    */{  105,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_OUT    */{  7  ,        3,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CS     */{  16 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PSM_IND    */{  1  ,        0,              0,        GPIO_4,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SEL    */{  108,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  20 ,        0,              0,        GPIO_5,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_FMARK  */{  92 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  65 ,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*GPIO5      */{  104,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*RESERVED   */{  66 ,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  26 ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  25 ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  64 ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  88 ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  21 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*I2C2_SCL   */{  40 ,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C2_SDA   */{  41 ,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SCL    */{  103,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  114,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  37 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  111,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  36 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  115,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  28 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  95 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  27 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  97 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DET  */{  83 ,        0,              0,        GPIO_22,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_DATA1  */{  98 ,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM1_DET  */{  42 ,        0,              0,        GPIO_23,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_SCK    */{  78 ,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_DCD   */{  38 ,        1,              1,        GPIO_24,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DTR   */{  30 ,        1,              1,        GPIO_25,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_RI    */{  39 ,        1,              1,        GPIO_26,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*W_DISABLE  */{  18 ,        1,              1,        GPIO_27,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*AP_READY   */{  19 ,        1,              1,        GPIO_28,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
#if defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS)
/*USIM2_CLK  */{  84 ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  86 ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  85 ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#else
/*USIM2_CLK  */{  84 ,        2,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  86 ,        2,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  85 ,        2,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#endif
//pinmux(not gpio part)
/*SDIO1_CLK  */{  110,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EG070U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*FLASH_CLK  */{  34 ,        1,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*W_DISABLE  */{  84 ,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_CS   */{  35 ,        1,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  85 ,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA0*/{  77 ,        1,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WAKEUP_IN  */{  43 ,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA1*/{  78 ,        1,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*AP_READY   */{  42 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA2*/{  79 ,        1,              0,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_MODE   */{  81 ,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*FLASH_DATA3*/{  80 ,        1,              0,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SLEEP_IND  */{  82 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_CLK  */{  123,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*MAIN_DTR   */{  55 ,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*SDIO2_CMD  */{  124,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  17 ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA0*/{  125,        1,              5,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  16 ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA1*/{  126,        1,              5,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  19 ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA2*/{  127,        1,              5,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  18 ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO2_DATA3*/{  128,        1,              5,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  45 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DCD   */{  86 ,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RI    */{  87 ,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SCL    */{  28 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  29 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  1  ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  75 ,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  90 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  98 ,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  64 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  99 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  63 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  76 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CP_TXD     */{  20 ,        5,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA1  */{  100,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM_DET   */{  22 ,        0,              0,        GPIO_23,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_SCK    */{  101,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_CMD  */{  21 ,        0,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA0*/{  72 ,        0,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA1*/{  73 ,        0,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA2*/{  69 ,        0,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_DATA3*/{  68 ,        0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#if defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS)
/*USIM2_CLK  */{  129,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  130,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  132,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#else
/*USIM2_CLK  */{  129,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  130,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  132,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#endif
//pinmux(not gpio part)
/*SDIO1_CLK  */{  66 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT4    */{  24 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYOUT5    */{  23 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EC020U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl  */
/*I2S_CLK    */{  27 ,        3,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SIO    */{  125,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2S_SYNC   */{  26 ,        3,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SDC    */{  124,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2S_DIN    */{  24 ,        3,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CLK    */{  123,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2S_DO     */{  25 ,        3,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CS     */{  122,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C3_SCL   */{  141,        6,              0,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SEL    */{  121,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C3_SDA   */{  142,        6,              0,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_FMARK  */{  119,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SD1_CLK    */{  133,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*STATUS     */{  61 ,        0,              0,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW       },
/*SD1_CMD    */{  134,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  40 ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SD1_DATA0  */{  132,        1,              5,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  37 ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SD1_DATA1  */{  131,        1,              5,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  38 ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SD1_DATA2  */{  130,        1,              5,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  39 ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SD1_DATA3  */{  129,        1,              5,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  5  ,        1,              0,        GPIO_13,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// hardware pin name is NET_MODE
/*USIM_DET   */{  13 ,        0,              0,        GPIO_14,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE },
/*SD_DET     */{  23 ,        0,              0,        GPIO_15,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE },
/*I2C_SCL    */{  41 ,        0,              4,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// output only
/*I2C_SDA    */{  42 ,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  64 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*GPIO18     */{  18 ,        4,              4,        GPIO_18,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_CTS   */{  65 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_MODE   */{  6  ,        4,              4,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW       },// hardware pin name is NET_STATUS
/*UART2_RXD  */{  66 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  139,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  63 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  136,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*ZSP_UART_TXD*/{  116,        3,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RI    */{  62 ,        4,              4,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH      },
/*UART2_CTS  */{  126,        1,              0,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SLEEP_IND  */{  3  ,        4,              4,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW       },
/*SDC2_CMD   */{  33 ,        0,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDC2_DATA0 */{  31 ,        0,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDC2_DATA1 */{  30 ,        0,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDC2_DATA2 */{  29 ,        0,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDC2_DATA3 */{  28 ,        0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*WAKEUP_IN  */{  1  ,        2,              2,        GPIO_29,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE },
/*AP_READY   */{  2  ,        2,              2,        GPIO_30,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE },
/*W_DISABLE  */{  4  ,        2,              2,        GPIO_31,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE },
//pinmux(not gpio part)
/*SDC2_CLK   */{  32 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART3_RXD  */{  137,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART3_TXD  */{  138,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN4     */{  81 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*KEYIN5     */{  82 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                -1},
};
#elif defined QL_APP_PROJECT_DEF_EG912U
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl    */
/*PCM_CLK    */{  4  ,        3,              0,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SIO    */{  116,        0,              1,        GPIO_0,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_SYNC   */{  5  ,        3,              0,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_SDC    */{  106,        0,              1,        GPIO_1,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_IN     */{  6  ,        3,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CLK    */{  105,        0,              1,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PCM_OUT    */{  7  ,        3,              0,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*LCD_CS     */{  16 ,        0,              1,        GPIO_3,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*PSM_IND    */{  1  ,        0,              0,        GPIO_4,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_SEL    */{  108,        0,              1,        GPIO_4,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*STATUS     */{  20 ,        0,              0,        GPIO_5,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*LCD_FMARK  */{  92 ,        0,              1,        GPIO_5,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*RESERVED   */{  65 ,        1,              0,        GPIO_7,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },// pay attention to USB
/*GPIO5      */{  104,        0,              0,        GPIO_8,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*RESERVED   */{  66 ,        1,              5,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CLK  */{  26 ,        1,              0,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_CS   */{  25 ,        1,              0,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MOSI */{  64 ,        1,              0,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SPI_1_MISO */{  88 ,        1,              0,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*NET_STATUS */{  21 ,        0,              0,        GPIO_13,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*I2C2_SCL   */{  40 ,        1,              0,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C2_SDA   */{  41 ,        1,              0,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*I2C_SDA    */{  114,        0,              4,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_RTS   */{  37 ,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_RSTL   */{  111,        0,              4,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_CTS   */{  36 ,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_PWDN   */{  115,        0,              4,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_RXD  */{  28 ,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_REFCLK */{  95 ,        0,              4,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*UART2_TXD  */{  27 ,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*CAM_DATA0  */{  97 ,        0,              4,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DET  */{  83 ,        0,              0,        GPIO_22,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_DATA1  */{  98 ,        0,              4,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM1_DET  */{  42 ,        0,              0,        GPIO_23,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*CAM_SCK    */{  78 ,        0,              4,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*MAIN_DCD   */{  38 ,        1,              1,        GPIO_24,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_DTR   */{  30 ,        1,              1,        GPIO_25,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*MAIN_RI    */{  39 ,        1,              1,        GPIO_26,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*W_DISABLE  */{  18 ,        1,              1,        GPIO_27,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
/*AP_READY   */{  19 ,        1,              1,        GPIO_28,   GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE },
#if defined(QL_APP_FEATURE_DSIM) || defined(QL_APP_FEATURE_DSSS)
/*USIM2_CLK  */{  84 ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  86 ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  85 ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#else
/*USIM2_CLK  */{  84 ,        2,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_DATA */{  86 ,        2,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*USIM2_RST  */{  85 ,        2,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
#endif
//pinmux(not gpio part)
/*I2C_SCL    */{  103,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },
/*SDIO1_CLK  */{  110,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,    -1},
};

#endif/* open */


