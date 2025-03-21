
/**  @file
  quec_customer_cfg.c

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

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
#include "quec_customer_cfg.h"
#include "ql_uart.h"
#include "quec_pin_index.h"
#include "hal_chip.h"
#include "quec_cust_feature.h"
#include "ql_audio.h"
#include "hal_config.h"

/*===========================================================================
 * Customer set Map
 ===========================================================================*/
volatile ql_model_diff_ctx_s ql_model_diff_ctx = 
{
#if defined CONFIG_QL_PROJECT_DEF_EC200U || defined CONFIG_QL_PROJECT_DEF_EC020U
    3, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L,
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
    4, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L,
#else
    4, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L,
#endif
#elif defined CONFIG_QL_PROJECT_DEF_EG700U || defined CONFIG_QL_PROJECT_DEF_EG070U
    4, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L,
#elif defined CONFIG_QL_PROJECT_DEF_EG500U
    4, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_MAINMIC,
#elif defined CONFIG_QL_PROJECT_DEF_EG915U || defined CONFIG_QL_PROJECT_DEF_EG912U
    2, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_MAINMIC,
#endif
};

const ql_model_partinfo_ctx_s ql_model_partinfo_ctx = 
{
    CONFIG_APP_FLASH_ADDRESS,
    CONFIG_APP_FLASH_SIZE,
    CONFIG_APPIMG_FLASH_ADDRESS,
    CONFIG_APPIMG_FLASH_SIZE,
    CONFIG_FS_SYS_FLASH_ADDRESS,
    CONFIG_FS_SYS_FLASH_SIZE,
    CONFIG_FS_MODEM_FLASH_ADDRESS,
    CONFIG_FS_MODEM_FLASH_SIZE,
#ifdef CONFIG_APPLIB_FLASH_EXT_ENABLED
    CONFIG_APPLIB_FLASH_ADDRESS,
    CONFIG_APPLIB_FLASH_SIZE,
#elif defined CONFIG_APP_FLASH2_ENABLED
    CONFIG_APP_FLASH2_ADDRESS,
    CONFIG_APP_FLASH2_SIZE,
#endif
};

//Customers can choose to specify the iic channel or choose to automatically identify the iic channel via CAM_IIC_CH_AUTO.
//Note:If the default pin function is not an i2c function, reuse it as an i2c function first.
uint8_t  ql_cam_iic_ch = CAM_IIC_CH_AUTO; //i2c_1/i2c_2/i2c_3/CAM_IIC_CH_AUTO
  
volatile ql_model_feature_ctx_s ql_model_feature_ctx = {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_VOLTE
    .volte_enabled = true,
#else
    .volte_enabled = false,
#endif
    .app_ram_offset     = CONFIG_APP_RAM_OFFSET,
    .app_total_ram_size = CONFIG_APP_TOTAL_RAM_SIZE,

#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
    .sim_count        = 2,
#else
    .sim_count        = 1,
#endif
    .ap_ifc_dma_count = 7,                  //R02单卡版本，cp侧会固定用ch5、ch6这2个通道，ap只能用ch0~ch4这5个通道。R03、R05分支，AP和CP会共用全部7个IFC DMA通道
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
    .dsim_feature = DSIM_DSDS,
#elif defined(CONFIG_QUEC_PROJECT_FEATURE_DSSS)
    .dsim_feature = DSIM_DSSS,
#else
    .dsim_feature = DSIM_SIGNEL,
#endif
	.usbsusp = 
	{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_SUSPEND
		.enable = TRUE,
#else
		.enable = FALSE,
#endif
		.reserve = FALSE,
	},
	.usbdesc =  // usb device string descriptor
    {
        .manufacturer = "Android",          //Custom manufacturer string descriptor 
        .product = "Android",               //Custom product string descriptor 
        .serialstr = "",                    //Custom serialstr string descriptor 
    },
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
#ifdef CONFIG_QL_PROJECT_DEF_EC200U
    .gnss_device_type = 1,            //EC200U且支持gnss功能的型号
#elif defined CONFIG_QL_PROJECT_DEF_EG912U
    .gnss_device_type = 2,            //EG912U且支持gnss功能的型号
#else
    .gnss_device_type = 1,
#endif
#else
    .gnss_device_type = 0,            //不支持GNSS功能的型号
#endif
#ifdef CONFIG_QL_PROJECT_DEF_EC600U
	.auto_headsetdet_enabled= true,
#else
	.auto_headsetdet_enabled = false,
#endif
#ifdef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH
    .vbat_rf_switch = true,
    .vbat_rf_gpio = CONFIG_GPIO_X_USED_FOR_VBAT_RF,
#else
    .vbat_rf_switch = false,
    .vbat_rf_gpio = 0,
#endif
    .charge_enable = true,
#if (defined CONFIG_QL_PROJECT_DEF_EC600U) || (defined CONFIG_QL_PROJECT_DEF_EG700U)  || (defined CONFIG_QL_PROJECT_DEF_EG070U)
    .vibr_domain_flg = 1,
#else
    .vibr_domain_flg = 0,
#endif

#ifdef CONFIG_APPIMG_FLASH_EXT_ENABLED
    .run_ext_flash = 1,
#elif defined CONFIG_APPLIB_FLASH_EXT_ENABLED
    .run_ext_flash = 2,
#elif defined CONFIG_APP_FLASH2_ENABLED
    .run_ext_flash = 3,  
#else 
    .run_ext_flash = 0, 
#endif
	.uart_prio = (APP_PRIORITY_REALTIME - 1),	//uart口回调函数的优先级
	.usb_prio = (APP_PRIORITY_REALTIME - 1),	//usb口回调函数的优先级
	.app_event_id_min = 10,	//用户调用ql_event_send/ql_event_send_ex函数时,event.id的最小值(当前0,1,2,3,4,8这几个event id禁止使用,内核使用这几个id作为其他功能,用户使用后可能会导致dump)
	.at_engine_stack = AT_ENGINE_STACK_SIZE_APP, //at引擎占用的栈空间大小
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    .mipi_continuous_display = 0,   //若需要从boot到APP一直显示MIPI图像，需要改成1
#else
    .mipi_continuous_display = 0,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FATFS_SAFE
	//fatfs文件系统是否开启掉电保护功能。分别对应的是分区1和分区2
	.fatfs_safe_enable[0] = 1,
	.fatfs_safe_enable[1] = 1,
#else
	.fatfs_safe_enable[0] = 0,
	.fatfs_safe_enable[1] = 0,
#endif
	.logfs_size[0] = 0,  //小于12时,使用默认值:从fatfs中分配32个簇用于存放log信息,至少12个簇。
	.logfs_size[1] = 0,
};

/*
i2s1引脚配置:
如果只用到了音频播报功能,不用录音功能,可以将{QUEC_PIN_PCM_DIN   ,  0}屏蔽,PCM_DIN引脚可以配置成通用GPIO使用。
*/
const ql_i2s_func_s ql_i2s_pin_func[] =
{
    {QUEC_PIN_PCM_CLK   ,  3},
    {QUEC_PIN_PCM_SYNC  ,  3},
    {QUEC_PIN_PCM_DIN   ,  3},
    {QUEC_PIN_PCM_DOUT  ,  3},
    {QUEC_PIN_NONE      , -1},
};

const ql_uart_func_s ql_uart_pin_func[] = {
#ifdef CONFIG_QL_PROJECT_DEF_EC200U
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 3, QUEC_PIN_UART2_RXD, 3},
    {QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4},
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
 #ifndef __QUEC_OEM_VER_LD__
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
    //{QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4},
 #else
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
    {QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 6, QUEC_PIN_UART3_RXD, 6},
 #endif
#elif defined CONFIG_QL_PROJECT_DEF_EG700U || defined CONFIG_QL_PROJECT_DEF_EG070U \
    || defined CONFIG_QL_PROJECT_DEF_EC020U 
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
    {QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4},
#elif defined CONFIG_QL_PROJECT_DEF_EG500U
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
    //{QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4},
#elif defined CONFIG_QL_PROJECT_DEF_EG915U
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
#elif defined CONFIG_QL_PROJECT_DEF_EG912U
    {QL_UART_PORT_2, QUEC_PIN_UART2_TXD, 1, QUEC_PIN_UART2_RXD, 1},
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
    //GNSS型号
    {QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4},
#else
    //非GNSS型号
    {QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 6, QUEC_PIN_UART3_RXD, 6},
#endif
#endif
    {QL_PORT_NONE, -1, -1, -1, -1},
};

const ql_cam_func_s ql_cam_pin_func[] =
{
    {QUEC_PIN_CAM_I2C_SCL   , 0},
    {QUEC_PIN_CAM_I2C_SDA   , 0},
    {QUEC_PIN_CAM_RSTL      , 0},
    {QUEC_PIN_CAM_PWDN      , 0},
    {QUEC_PIN_CAM_REFCLK    , 0},
    {QUEC_PIN_CAM_SPI_DATA0 , 0},
    {QUEC_PIN_CAM_SPI_DATA1 , 0},
    {QUEC_PIN_CAM_SPI_SCK   , 0},
    {QUEC_PIN_NONE, -1},
};

const ql_lcd_func_s ql_lcd_pin_func[] =
{
    {QUEC_PIN_LCD_SPI_SIO , 0},
    {QUEC_PIN_LCD_SPI_SDC , 0},
    {QUEC_PIN_LCD_SPI_CLK , 0},
    {QUEC_PIN_LCD_SPI_CS  , 0},
    {QUEC_PIN_LCD_SPI_SEL , 0},
    {QUEC_PIN_LCD_FMARK   , 0},
    {QUEC_PIN_NONE, -1},
};
    
#if defined (CONFIG_QL_PROJECT_DEF_EC200U_EU_AA) || defined (CONFIG_QL_PROJECT_DEF_EC200U_AU_AA) || defined (CONFIG_QL_PROJECT_DEF_EG912U_GL_AA)
    uint16 quec_gnss_cfg_table[]={GPS_SYS,INVALID_SYS,INVALID_SYS,GPS_GLONASS_GALILEO_SYS,GPS_GLONASS_SYS,GPS_BDS_GALILEO_SYS,GPS_GALILEO_SYS,BDS_SYS};
#else
    uint16 quec_gnss_cfg_table[]={GPS_SYS,INVALID_SYS,INVALID_SYS,GPS_GLONASS_GALILEO_SYS,GPS_GLONASS_SYS,GPS_BDS_SYS,        GPS_GALILEO_SYS,BDS_SYS};
#endif

/*==================================================================================
 * Power Domain Set
 * description:
 *           The voltage domain configuration is divided into three stages: unisoc initialization stage,
 *			 quectel kernel initialization stage and app initialization stage. The customer can change 
 *			 the voltage value of the voltage domain or whether it is enabled or disabled and
 *           initialization stage
 * parameters:
 *           id			: optional power domain(don't modify!!!)
 *           enabled   	: work mode enable power
 *           lp_enabled : sleep mode enable power
 *           mv			: power domain value's range(units:mV)
 *                        POWER_LEVEL_1700MV ~ POWER_LEVEL_3200MV
 *                        step: 100MV
 *                        min_V: HAL_POWER_VIBR is: 2.8V
 *                        		 others         is: 1.6125V
 *           init_phase : Initialization phase
 *						  POWER_INIT_PHASE1---unisoc  kernel initialization stage
 *						  POWER_INIT_PHASE2---quectel kernel initialization stage
 *						  POWER_INIT_PHASE3---app     kernel initialization stage						  
 * matters need attention:
 *           if LCD analog voltage(use LCD func)
 *           comes from the VIBR power domain of module what like EC600U/EG700U,
 *           VIBR power domain must be great than or equal to LCD power domain.
 * example:
 *			 1. if you want LCD display from boot to app, you can enable HAL_POWER_BACK_LIGHT in phase1 
 *           2. if you want advance the initialization stage of SD card voltage domain, you can directly
 *              modify HAL_POWER_SD init pahse to POWER_INIT_PHASE1
 ===================================================================================*/
const ql_power_domain_s ql_power_domain_table[] =
{  
	 /* id                  enabled   lp_enabled   mv          				init_phase */
//power init phase1(unisoc kernel init)
#ifdef CONFIG_FLASH_LP_POWER_DOWN
	{  HAL_POWER_SPIMEM,	true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* SPIMEM */ /* not available */
#else
	{  HAL_POWER_SPIMEM,	true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* SPIMEM */ /* not available */
#endif
	{  HAL_POWER_MEM,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* MEM */    /* not available */
	{  HAL_POWER_VIO18,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* VIO18 */
#ifdef CONFIG_HEADSET_DETECT_SUPPORT
	{  HAL_POWER_VDD28,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* VDD28 */  /* not available */
#else
	{  HAL_POWER_VDD28,	    true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* VDD28 */  /* not available */
#endif
	{  HAL_POWER_DCXO,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* DCXO */   /* not available */
#ifdef CONFIG_CAMA_POWER_ON_INIT
	{  HAL_POWER_CAMA,	    true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* CAMA */   /* not available */
#else
	{  HAL_POWER_CAMA,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* CAMA */   /* not available */
#endif
	{  HAL_POWER_BACK_LIGHT,false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* BACK_LIGHT */ /* not available */
	{  HAL_POWER_CAMD,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* CAMD */   /* not available */
    {  HAL_POWER_WCN,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* WCN */    /* not available */
    {  HAL_POWER_USB,       false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  }, 	/* USB */    /* not available */
    {  HAL_POWER_BUCK_PA,   false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  }, 	/* BUCK_PA *//* not available */
#ifdef CONFIG_BOARD_KPLED_USED_FOR_RF_SWITCH
    {  HAL_POWER_KEYLED,    true,     false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* KEYLED */ /* not available */
#else
    {  HAL_POWER_KEYLED,    false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },   /* KEYLED */ /* not available */
#endif

//power init phase2(quectel kernel init)
#ifdef __QUEC_OEM_VER_AC__
	{  HAL_POWER_LCD ,		true ,	  true ,       POWER_LEVEL_3200MV,		POWER_INIT_PHASE2  },	/* LCD */
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_GNSS)
	{  HAL_POWER_LCD ,		true ,	  true,	       POWER_LEVEL_1900MV,		POWER_INIT_PHASE2  },   /* can't alter this power domain value(mV), when GNSS ver.*/
#else
	{  HAL_POWER_LCD ,		true ,	  true,	  	   POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  },   /* for open series(csdk/ccsdk) customer,can alter power value when customers need */
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_LD__
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_3200MV,		POWER_INIT_PHASE2  },
#else
#if (!defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC) && (!defined CONFIG_QUEC_PROJECT_FEATURE_EMMC)
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  },
#endif
#endif
#else
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  }, 	/* for open series(csdk/ccsdk) customer ,can alter power value when customers need */ 
#endif
#if (!defined CONFIG_QUEC_PROJECT_FEATURE_QDSIM) && (!defined CONFIG_QUEC_PROJECT_FEATURE_DSSS)
    {  HAL_POWER_SIM1,  	true ,    true ,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  }, 	/* for open series(csdk/ccsdk) customer,can alter power value when customers need */
#endif

//power init phase3(quectel app init)
#if (defined CONFIG_QL_PROJECT_DEF_EC600U) || (defined CONFIG_QL_PROJECT_DEF_EG700U)  || (defined CONFIG_QL_PROJECT_DEF_EG070U)
	{  HAL_POWER_VIBR,		true ,	  false,	   POWER_LEVEL_2800MV,		POWER_INIT_PHASE3  },	/* for open series(csdk/ccsdk) customer, can alter power value when customers need */
#endif

//should before here!
	{  HAL_POWER_NONE,  	false ,   false ,      POWER_LEVEL_UNUSED,		POWER_INIT_NONE    }, 	/* for open series(csdk/ccsdk) customer,can alter power value when customers need */
};
const unsigned char ql_power_domain_table_len = sizeof(ql_power_domain_table)/sizeof(ql_power_domain_table[0]);


#ifdef CONFIG_QL_PROJECT_DEF_EC020U
uint16_t module_def = 20;
#elif  defined CONFIG_QL_PROJECT_DEF_EC200U
uint16_t module_def = 200;
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
uint16_t module_def = 600;
#elif defined CONFIG_QL_PROJECT_DEF_EG070U
uint16_t module_def = 70;
#elif defined CONFIG_QL_PROJECT_DEF_EG700U
uint16_t module_def = 700;
#elif defined CONFIG_QL_PROJECT_DEF_EG500U
uint16_t module_def = 500;
#elif defined CONFIG_QL_PROJECT_DEF_EG915U
uint16_t module_def = 915;
#elif defined CONFIG_QL_PROJECT_DEF_EG912U
uint16_t module_def = 912;
#endif

const ql_model_mem_ctx_s ql_model_mem_ctx = 
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
    .bt_addr = CONFIG_RAM_PHY_ADDRESS + CONFIG_APP_BTFW_RAM_OFFSET,
    .bt_size = CONFIG_APP_BTFW_RAM_SIZE,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    .mipi_lcd_addr = CONFIG_RAM_PHY_ADDRESS + CONFIG_QUEC_APP_MIPI_LCD_RAM_OFFSET,
    .mipi_lcd_size = CONFIG_QUEC_APP_MIPI_LCD_RAM_SIZE,
    .undache_enable = false,
#endif
};
