
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

/*===========================================================================
 * GPIO Map
 ===========================================================================*/
#ifdef CONFIG_QL_OPEN_EXPORT_PKG /* Open */
const quec_pin_cfg_t quec_pin_cfg_map[] = /* pin initialize */
{            /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  */
/*PCM_CLK    */{  61 ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_0_cfg_reg           },
/*LCD_SIO    */{  66 ,        1,              1,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sio_cfg_reg      },
/*PCM_SYNC   */{  58 ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_1_cfg_reg           },
/*LCD_SDC    */{  63 ,        1,              1,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sdc_cfg_reg      },
/*PCM_IN     */{  59 ,        0,              0,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_2_cfg_reg           },
/*LCD_CLK    */{  67 ,        1,              1,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_clk_cfg_reg      },
/*PCM_OUT    */{  60 ,        0,              0,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_3_cfg_reg           },
/*LCD_CS     */{  65 ,        1,              1,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_cs_cfg_reg       },
/*GPIO1      */{  69 ,        0,              0,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_4_cfg_reg           },
/*LCD_SEL    */{  137,        1,              1,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_select_cfg_reg   },
/*GPIO2      */{  70 ,        0,              0,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_5_cfg_reg           },
/*LCD_FMARK  */{  62 ,        1,              1,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_lcd_fmark_cfg_reg        },
/*SDIO2_CLK  */{  118,        0,              0,        GPIO_7,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_7_cfg_reg           },// pay attention to USB
/*USIM_DET   */{  9  ,        0,              0,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_8_cfg_reg           },
/*SDIO2_CMD  */{  99 ,        5,              5,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tck_cfg_reg      },
/*SPI_1_CLK  */{  1  ,        0,              0,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_9_cfg_reg           },
/*SDIO2_DATA0*/{  98 ,        5,              5,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_trst_cfg_reg     },
/*SPI_1_CS   */{  4  ,        0,              0,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_10_cfg_reg          },
/*SDIO2_DATA1*/{  95 ,        1,              5,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_ap_jtag_tms_cfg_reg      },
/*SPI_1_MOSI */{  3  ,        0,              0,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_11_cfg_reg          },
/*SDIO2_DATA2*/{  119,        5,              5,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tdi_cfg_reg      },
/*SPI_1_MISO */{  2  ,        0,              0,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_12_cfg_reg          },
/*SDIO2_DATA3*/{  100,        5,              5,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tdo_cfg_reg      },
/*NET_STATUS */{  54 ,        0,              0,        GPIO_13,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_13_cfg_reg          },
/*I2C2_SCL   */{  57 ,        0,              0,        GPIO_14,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_14_cfg_reg          },
/*I2C2_SDA   */{  56 ,        0,              0,        GPIO_15,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_15_cfg_reg          },
/*I2C_SCL    */{  11 ,        4,              4,        GPIO_16,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_i2c_m1_scl_cfg_reg       },// output only
/*I2C_SDA    */{  12 ,        4,              4,        GPIO_17,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_i2c_m1_sda_cfg_reg       },
/*MAIN_RTS   */{  34 ,        0,              0,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_18_cfg_reg          },
/*CAM_RSTL   */{  120,        4,              4,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_rst_l_cfg_reg     },
/*MAIN_CTS   */{  33 ,        0,              0,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_19_cfg_reg          },
/*CAM_PWDN   */{  16 ,        4,              4,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_pwdn_cfg_reg      },
/*UART2_RXD  */{  123,        0,              0,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_20_cfg_reg          },
/*CAM_REFCLK */{  10 ,        4,              4,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_ref_clk_cfg_reg   },
/*UART2_TXD  */{  124,        0,              0,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_21_cfg_reg          },
/*CAM_DATA0  */{  14 ,        4,              4,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_0_cfg_reg  },
/*UART2_RTS  */{  122,        0,              0,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_22_cfg_reg          },
/*CAM_DATA1  */{  15 ,        4,              4,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_1_cfg_reg  },
/*UART2_CTS  */{  121,        0,              0,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_23_cfg_reg          },
/*CAM_SCK    */{  13 ,        4,              4,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_sck_cfg_reg   },
/*MAIN_DCD   */{  48 ,        1,              1,        GPIO_24,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_cmd_cfg_reg       },
/*MAIN_DTR   */{  39 ,        1,              1,        GPIO_25,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_0_cfg_reg    },
/*MAIN_RI    */{  40 ,        1,              1,        GPIO_26,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_1_cfg_reg    },
/*WAKEUP_IN  */{  49 ,        1,              1,        GPIO_27,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_2_cfg_reg    },
/*AP_READY   */{  50 ,        1,              1,        GPIO_28,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_3_cfg_reg    },
#if defined(CONFIG_QUEC_PROJECT_FEATURE_QDSIM) || defined(CONFIG_QUEC_PROJECT_FEATURE_DSSS) 
/*SLEEP_IND  */{  53 ,        0,              2,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_clk_cfg_reg        },
/*NET_MODE   */{  52 ,        0,              2,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_dio_cfg_reg        },
/*W_DISABLE  */{  51 ,        0,              2,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sim_2_rst_cfg_reg        },
#else /* public */
/*SLEEP_IND  */{  53 ,        2,              2,        GPIO_29,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_clk_cfg_reg        },
/*NET_MODE   */{  52 ,        2,              2,        GPIO_30,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_dio_cfg_reg        },
/*W_DISABLE  */{  51 ,        2,              2,        GPIO_31,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_rst_cfg_reg        },
#endif
//pinmux(not gpio part)
/*SDIO1_CLK  */{  132,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sdmmc1_clk_cfg_reg       },
/*KEYOUT4    */{  104,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_4_cfg_reg         },
/*KEYOUT5    */{  103,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_5_cfg_reg         },
/*KEYIN4     */{  126,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyin_4_cfg_reg          },
/*KEYIN5     */{  125,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyin_5_cfg_reg          },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                      -1},
};
#endif/* CONFIG_QL_OPEN_EXPORT_PKG */

