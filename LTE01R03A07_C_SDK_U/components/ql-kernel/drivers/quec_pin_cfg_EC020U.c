
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
/*I2S_CLK    */{  27 ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_0_cfg_reg           },
/*LCD_SIO    */{  125,        1,              1,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sio_cfg_reg      },
/*I2S_SYNC   */{  26 ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_1_cfg_reg           },
/*LCD_SDC    */{  124,        1,              1,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_sdc_cfg_reg      },
/*I2S_DIN    */{  24 ,        0,              0,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_2_cfg_reg           },
/*LCD_CLK    */{  123,        1,              1,        GPIO_2,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_clk_cfg_reg      },
/*I2S_DO     */{  25 ,        0,              0,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_3_cfg_reg           },
/*LCD_CS     */{  122,        1,              1,        GPIO_3,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_cs_cfg_reg       },
/*I2C3_SCL   */{  141,        0,              0,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_4_cfg_reg           },
/*LCD_SEL    */{  121,        1,              1,        GPIO_4,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_lcd_select_cfg_reg   },
/*I2C3_SDA   */{  142,        0,              0,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_5_cfg_reg           },
/*LCD_FMARK  */{  119,        1,              1,        GPIO_5,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_lcd_fmark_cfg_reg        },
/*SD1_CLK    */{  133,        0,              0,        GPIO_7,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_7_cfg_reg           },// pay attention to USB
/*STATUS     */{  61 ,        0,              0,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_8_cfg_reg           },
/*SD1_CMD    */{  134,        5,              5,        GPIO_8,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tck_cfg_reg      },
/*SPI_1_CLK  */{  40 ,        0,              0,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_9_cfg_reg           },
/*SD1_DATA0  */{  132,        5,              5,        GPIO_9,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_trst_cfg_reg     },
/*SPI_1_CS   */{  37 ,        0,              0,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_10_cfg_reg          },
/*SD1_DATA1  */{  131,        5,              5,        GPIO_10,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tms_cfg_reg      },
/*SPI_1_MOSI */{  38 ,        0,              0,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_11_cfg_reg          },
/*SD1_DATA2  */{  130,        5,              5,        GPIO_11,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tdi_cfg_reg      },
/*SPI_1_MISO */{  39 ,        0,              0,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_12_cfg_reg          },
/*SD1_DATA3  */{  129,        5,              5,        GPIO_12,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_ap_jtag_tdo_cfg_reg      },
/*NET_STATUS */{  5  ,        0,              0,        GPIO_13,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_13_cfg_reg          },// hardware pin name is NET_MODE
/*USIM_DET   */{  13 ,        0,              0,        GPIO_14,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_14_cfg_reg          },
/*SD_DET     */{  23 ,        0,              0,        GPIO_15,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_15_cfg_reg          },
/*I2C_SCL    */{  41 ,        4,              4,        GPIO_16,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_i2c_m1_scl_cfg_reg       },// output only
/*I2C_SDA    */{  42 ,        4,              4,        GPIO_17,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_i2c_m1_sda_cfg_reg       },
/*MAIN_RTS   */{  64 ,        0,              0,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_18_cfg_reg          },
/*GPIO18     */{  18 ,        4,              4,        GPIO_18,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_rst_l_cfg_reg     },
/*MAIN_CTS   */{  65 ,        0,              0,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_19_cfg_reg          },
/*NET_MODE   */{  6  ,        4,              4,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_pwdn_cfg_reg      },// hardware pin name is NET_STATUS
/*UART2_RXD  */{  66 ,        0,              0,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_20_cfg_reg          },
/*CAM_REFCLK */{  139,        4,              4,        GPIO_20,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_camera_ref_clk_cfg_reg   },
/*UART2_TXD  */{  63 ,        0,              0,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_21_cfg_reg          },
/*CAM_DATA0  */{  136,        4,              4,        GPIO_21,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_0_cfg_reg  },
/*ZSP_UART_TXD*/{  116,        0,              0,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_22_cfg_reg          },
/*MAIN_RI    */{  62 ,        4,              4,        GPIO_22,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_si_1_cfg_reg  },
/*UART2_CTS  */{  126,        0,              0,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_gpio_23_cfg_reg          },
/*SLEEP_IND  */{  3  ,        4,              4,        GPIO_23,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_spi_camera_sck_cfg_reg   },
/*SDC2_CMD   */{  33 ,        1,              1,        GPIO_24,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_cmd_cfg_reg       },
/*SDC2_DATA0 */{  31 ,        1,              1,        GPIO_25,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_0_cfg_reg    },
/*SDC2_DATA1 */{  30 ,        1,              1,        GPIO_26,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_1_cfg_reg    },
/*SDC2_DATA2 */{  29 ,        1,              1,        GPIO_27,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_2_cfg_reg    },
/*SDC2_DATA3 */{  28 ,        1,              1,        GPIO_28,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sdmmc1_data_3_cfg_reg    },
/*WAKEUP_IN  */{  1  ,        2,              2,        GPIO_29,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_clk_cfg_reg        },
/*AP_READY   */{  2  ,        2,              2,        GPIO_30,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_dio_cfg_reg        },
/*W_DISABLE  */{  4  ,        2,              2,        GPIO_31,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->pad_sim_2_rst_cfg_reg        },

//pinmux(not gpio part)
/*SDC2_CLK   */{  32 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_sdmmc1_clk_cfg_reg       },
/*UART3_RXD  */{  137,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_4_cfg_reg         },
/*UART3_TXD  */{  138,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyout_5_cfg_reg         },
/*KEYIN4     */{  81 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyin_4_cfg_reg          },
/*KEYIN5     */{  82 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->pad_keyin_5_cfg_reg          },

//             should before here!
               {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                      -1},
};
#endif/* CONFIG_QL_OPEN_EXPORT_PKG */

