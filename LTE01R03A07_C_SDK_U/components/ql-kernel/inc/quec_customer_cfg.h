
/**  @file
  quec_customer_cfg.h

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

#ifndef QUEC_CUSTOMER_CFG_H
#define QUEC_CUSTOMER_CFG_H

#include "quec_common.h"
#include "ql_uart.h"
#include "ql_api_osi.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro
 ===========================================================================*/
#if defined CONFIG_QL_PROJECT_DEF_EG500U || defined CONFIG_QL_PROJECT_DEF_EG915U
#define UART_PIN_MAX  (1)  //EG500U/EG915U only UART2 //different uart num, so don't used in core code directly
#elif defined CONFIG_QL_PROJECT_DEF_EC600U
 #ifndef __QUEC_OEM_VER_LD__
#define UART_PIN_MAX  (1)  //EC600U only UART2 //different uart num, so don't used in core code directly
 #else
#define UART_PIN_MAX  (2)  //UART2 UART3 //different uart num, so don't used in core code directly
 #endif
#else
#define UART_PIN_MAX  (2)  //UART2 UART3 //different uart num, so don't used in core code directly
#endif
#define CAM_PIN_MAX   (8)  //fixed pin num
#define LCD_PIN_MAX   (6)  //fixed pin num
#define I2S_PIN_MAX   (4)  //fixed pin num
#define CAM_IIC_CH_AUTO   (0xff)  //All iic channels are tried automatically during camera initialization

#define HAL_PMU_POWER_MAX_COUNT 	0xFF

/* GNSS SYS CFG */
#define GPS_SYS                     0x01
#define BDS_SYS                     0x10
#define GPS_BDS_SYS                 0x11
#define GPS_GLONASS_SYS             0x101
#define GPS_GALILEO_SYS             0x1001
#define GPS_GLONASS_GALILEO_SYS     0x1101
#define GPS_BDS_GALILEO_SYS         0x1011
#define INVALID_SYS                 0xffff

#define AT_ENGINE_STACK_SIZE_APP	(32 * 1024)

#ifdef CONFIG_QL_PROJECT_DEF_EC020U
#define QL_AUDIO_IIC_CHANNEL     	0	//1 for iic2, 0 for iic 1, used for control external codec
#else
#define QL_AUDIO_IIC_CHANNEL     	1
#endif

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef enum
{
   POWER_INIT_NONE   = -1,
   POWER_INIT_PHASE1 =  0,	   //unisoc init phase
   POWER_INIT_PHASE2,		   //quec init phase
   POWER_INIT_PHASE3,		   //individual function init phase
}ql_power_init_phase_e;

typedef enum
{
    DSIM_SIGNEL,
    DSIM_DSSS,
    DSIM_DSDS,
}ql_dsim_feature_e;

typedef struct
{
    uint8_t        adc_num;
    uint8_t        sdmmc_enabled;
    uint8_t        ex_uart_num;
    uint8_t        pin_num; //=QUEC_PIN_CFG_MAX
    uint8_t 	   hp_input_type; //see it in AudInputType_e
}ql_model_diff_ctx_s;

typedef struct
{
	bool enable;
	uint reserve;
}usbsusp_mg_t;

typedef struct
{
    char *manufacturer;        //Custom manufacturer string descriptor
    char *product;             //Custom product string descriptor
	char *serialstr;           //Custom serialstr string descriptor
}usbdesc_mg_t;
	
typedef struct
{
    unsigned int        ap_addr; 
    unsigned int        ap_size; 
    unsigned int        appimg_addr; 
    unsigned int        appimg_size; 
    unsigned int        fs_addr; 
    unsigned int        fs_size;
    unsigned int        fs_modem_addr;
    unsigned int        fs_modem_size;
    unsigned int        ext_flash_addr;
    unsigned int        ext_flash_size;
}ql_model_partinfo_ctx_s;

typedef struct
{
    unsigned int        bt_addr; 
    unsigned int        bt_size; 
    unsigned int        mipi_lcd_addr; 
    unsigned int        mipi_lcd_size; 
    unsigned int        undache_enable;     //Select undache or not
}ql_model_mem_ctx_s;

typedef struct
{
    bool volte_enabled;
    uint8_t sim_count;
    ql_dsim_feature_e dsim_feature;
    uint32_t app_ram_offset;
    uint32_t app_total_ram_size;
    uint8_t ap_ifc_dma_count;           //ap can use ifc dma channel number
    usbsusp_mg_t usbsusp;
    usbdesc_mg_t usbdesc;               //Custom usb descriptor 
    uint8 gnss_device_type;            //0-不支持GNSS功能的型号，1-EC200U且支持gnss功能的型号，2-EG912U且支持gnss功能的型号
    bool auto_headsetdet_enabled;
    bool vbat_rf_switch;                //负载开关使能。vbat rf switch.
    int vbat_rf_gpio;                   //负载开关GPIO引脚，当vbat_rf_switch为true时使用。vbat rf switch GPIO pin, used when vbat_rf_switch is true
    bool charge_enable;                 //启动充电管理标志。为true时，启动充电管理，为false，关闭充电管理。默认为true。open charge management sign. When it is true, the charging management is started, and when it is false, the charging management is turned off. Defaults to true.
    uint8_t vibr_domain_flg;            // be used for close VIBR domain when LCD enter sleep.
    int run_ext_flash;                  //代码是否放到外置6线nor flash运行。为0时，不放到外置flash。为1时，APP代码放到外置flash。为2时，APP部分代码放到外置flash。为3时，kernel部分代码放到外置flash
	uint8 uart_prio;					//uart口回调函数的优先级
	uint8 usb_prio;						//usb口回调函数的优先级
    uint8 app_event_id_min;				//用户调用ql_event_send/ql_event_send_ex函数时,event.id的最小值(当前0,1,2,3,4,8这几个event id禁止使用,内核使用这几个id作为其他功能,用户使用后可能会导致dump)          
	uint at_engine_stack;			    //模组内部AT引擎的栈大小
	uint8 mipi_continuous_display;      //MIPI图像从boot显示到APP。0-不需要从boot里显示到APP，1-需要从boot里显示到APP
	uint8_t fatfs_safe_enable[2]; 		//fatfs文件系统是否启用掉电保护功能
    uint32_t logfs_size[2];				//fatfs文件系统用掉电保护的logfs空间大小,单位:簇
} ql_model_feature_ctx_s;

typedef struct
{
    ql_uart_port_number_e port;
    uint8_t tx_pin;
    uint8_t tx_func;
    uint8_t rx_pin;
    uint8_t rx_func;
} ql_uart_func_s;

typedef struct
{
    uint8_t pin;
    uint8_t func;
} ql_cam_func_s;

typedef ql_cam_func_s ql_lcd_func_s;
typedef ql_cam_func_s ql_i2s_func_s;

/**
 * Power domain set
 */
typedef struct
{
    uint32_t id;          //configurable power domain
    bool     enabled;     //work mode
    bool     lp_enabled;  //sleep mode
    uint32_t mv;          //power domain value(units:mV)
    int8_t   init_phase;  //power domain initialization stage
} ql_power_domain_s;

/*===========================================================================
 * Extern
 ===========================================================================*/
extern volatile ql_model_diff_ctx_s ql_model_diff_ctx;
extern const ql_model_partinfo_ctx_s ql_model_partinfo_ctx;
extern volatile ql_model_feature_ctx_s ql_model_feature_ctx;
extern const ql_uart_func_s ql_uart_pin_func[];
extern const ql_cam_func_s ql_cam_pin_func[];
extern const ql_lcd_func_s ql_lcd_pin_func[];
extern const ql_i2s_func_s ql_i2s_pin_func[];
extern const ql_power_domain_s ql_power_domain_table[];
extern const unsigned char ql_power_domain_table_len;
extern const ql_model_mem_ctx_s ql_model_mem_ctx;


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_CUSTOMER_CFG_H */

