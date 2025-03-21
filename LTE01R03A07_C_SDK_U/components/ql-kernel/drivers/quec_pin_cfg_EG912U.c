
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

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include "quec_pin_cfg.h"

#include "hwregs.h"
#include "hal_config.h"

/*===========================================================================
 * GPIO Map
 ===========================================================================*/
#ifdef CONFIG_QL_OPEN_EXPORT_PKG /* Open */
const quec_pin_cfg_t quec_pin_cfg_map[] = /* pin initialize */
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*PCM_CLK    */{  4  ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_0_cfg_reg           },
/*LCD_SIO    */{  116,        1,              1,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sio_cfg_reg      },
/*PCM_SYNC   */{  5  ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_1_cfg_reg           },
/*LCD_SDC    */{  106,        1,              1,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sdc_cfg_reg      },
/*PCM_IN     */{  6  ,        0,              0,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_2_cfg_reg           },
/*LCD_CLK    */{  105,        1,              1,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_clk_cfg_reg      },
/*PCM_OUT    */{  7  ,        0,              0,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_3_cfg_reg           },
/*LCD_CS     */{  16 ,        1,              1,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_cs_cfg_reg       },
/*PSM_IND    */{  1  ,        0,              0,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_4_cfg_reg           },
/*LCD_SEL    */{  108,        1,              1,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_select_cfg_reg   },
/*STATUS     */{  20 ,        0,              0,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_5_cfg_reg           },
/*LCD_FMARK  */{  92 ,        1,              1,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_lcd_fmark_cfg_reg        },
/*RESERVED   */{  65 ,        0,              0,        GPIO_7,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_7_cfg_reg           },// pay attention to USB
/*GPIO5      */{  104,        0,              0,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_8_cfg_reg           },
/*RESERVED   */{  66 ,        5,              5,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tck_cfg_reg      },
/*SPI_1_CLK  */{  26 ,        0,              0,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_9_cfg_reg           },
/*SPI_1_CS   */{  25 ,        0,              0,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_10_cfg_reg          },
/*SPI_1_MOSI */{  64 ,        0,              0,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_11_cfg_reg          },
/*SPI_1_MISO */{  88 ,        0,              0,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_12_cfg_reg          },
/*NET_STATUS */{  21 ,        0,              0,        GPIO_13,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_13_cfg_reg          },
/*I2C2_SCL   */{  40 ,        0,              0,        GPIO_14,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_14_cfg_reg          },
/*I2C2_SDA   */{  41 ,        0,              0,        GPIO_15,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_15_cfg_reg          },
/*I2C_SDA    */{  114,        4,              4,        GPIO_17,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_i2c_m1_sda_cfg_reg       },
/*MAIN_RTS   */{  37 ,        0,              0,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_18_cfg_reg          },
/*CAM_RSTL   */{  111,        4,              4,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_rst_l_cfg_reg     },
/*MAIN_CTS   */{  36 ,        0,              0,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_19_cfg_reg          },
/*CAM_PWDN   */{  115,        4,              4,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_pwdn_cfg_reg      },
/*UART2_RXD  */{  28 ,        0,              0,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_20_cfg_reg          },
/*CAM_REFCLK */{  95 ,        4,              4,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_ref_clk_cfg_reg   },
/*UART2_TXD  */{  27 ,        0,              0,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_21_cfg_reg          },
/*CAM_DATA0  */{  97 ,        4,              4,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_0_cfg_reg  },
/*USIM2_DET  */{  83 ,        0,              0,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_22_cfg_reg          },
/*CAM_DATA1  */{  98 ,        4,              4,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_1_cfg_reg  },
/*USIM1_DET  */{  42 ,        0,              0,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_23_cfg_reg          },
/*CAM_SCK    */{  78 ,        4,              4,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_sck_cfg_reg   },
/*MAIN_DCD   */{  38 ,        1,              1,        GPIO_24,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_cmd_cfg_reg       },
/*MAIN_DTR   */{  30 ,        1,              1,        GPIO_25,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_0_cfg_reg    },
/*MAIN_RI    */{  39 ,        1,              1,        GPIO_26,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_1_cfg_reg    },
/*W_DISABLE  */{  18 ,        1,              1,        GPIO_27,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_2_cfg_reg    },
/*AP_READY   */{  19 ,        1,              1,        GPIO_28,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_3_cfg_reg    },
#if defined(CONFIG_QUEC_PROJECT_FEATURE_QDSIM) || defined(CONFIG_QUEC_PROJECT_FEATURE_DSSS) 
/*USIM2_CLK  */{  84 ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_clk_cfg_reg        },
/*USIM2_DATA */{  86 ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_dio_cfg_reg        },
/*USIM2_RST  */{  85 ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_rst_cfg_reg        },
#else
/*USIM2_CLK  */{  84 ,        2,              2,        GPIO_29,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_clk_cfg_reg        },
/*USIM2_DATA */{  86 ,        2,              2,        GPIO_30,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_dio_cfg_reg        },
/*USIM2_RST  */{  85 ,        2,              2,        GPIO_31,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_rst_cfg_reg        },
#endif
//pinmux(not gpio part)
/*I2C_SCL    */{  103,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_i2c_m1_scl_cfg_reg       },
/*SDIO1_CLK  */{  110,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sdmmc1_clk_cfg_reg       },
//以下引脚保留给模块使用，客户无法使用，且不能修改
//The following pins are reserved for module use, cannot be used by customers, and cannot be modified
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
/*GNSS_BLK  */{  200,         5,              5,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_trst_cfg_reg     },
/*GNSS_RESET*/{  201,         5,              5,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tdi_cfg_reg      },
/*GNSS_PWR  */{  202,         5,              5,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->pad_ap_jtag_tdo_cfg_reg      }, 
/*KEYOUT4   */{  203 ,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_4_cfg_reg         },
/*KEYOUT5   */{  204 ,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_5_cfg_reg         },
#endif
#ifdef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH
//需要与CONFIG_GPIO_X_USED_FOR_VBAT_RF宏控对应，使用为GPIO10。平台原始代码，负载开关引脚是使用默认GPIO10的引脚，但是EG912U型号，是使用其他引脚作为GPIO10功能。
/*VBAT_RF_EN*/{  205,         5,              5,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->pad_ap_jtag_tms_cfg_reg      },
//默认需要输出高电平，开机开启负载开关功能
#endif

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                      -1},
};
#endif/* CONFIG_QL_OPEN_EXPORT_PKG */

