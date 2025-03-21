/**  @file
  ql_power.h

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


#ifndef QL_POWER_H
#define QL_POWER_H


#include "ql_api_common.h"
#include "ql_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_POWER_ERRCODE_BASE (QL_COMPONENT_PM<<16)

#define QL_QSCLKEX_NO_DATA_MIN	       1    //unit: second
#define QL_QSCLKEX_NO_DATA_DEFAULT	   3
#define QL_QSCLKEX_NO_DATA_MAX	       50

#define QL_QSCLKEX_PUNISH_TIME_MIN		1   //unit: minute
#define QL_QSCLKEX_PUNISH_TIME_DEFAULT	30
#define QL_QSCLKEX_PUNISH_TIME_MAX		600   

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWK

#define QL_PWRKEY_SHUTDOWN_TIME         2500   //unit: ms, default 2500ms
#define QL_PWRKEY_LONGPRESS_TIME        5000   //unit: ms, default 5000ms

/*===========================================================================
 * Typedef
 ===========================================================================*/
typedef void (*ql_pwrkey_callback)(void);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
typedef void (*ql_psm_enter_callback)(void *ctx);
#endif
/*===========================================================================
 * Extern
 ===========================================================================*/
extern ql_pwrkey_callback ql_pwrkey_shutdown_cb;
extern ql_pwrkey_callback ql_pwrkey_longpress_cb;
extern ql_pwrkey_callback ql_pwrkey_press_cb;
extern ql_pwrkey_callback ql_pwrkey_release_cb;
extern uint8_t ql_pwrkey_status;
extern uint32_t ql_shutdown_time;
extern uint32_t ql_longpress_time;

#endif

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * POWER errcode
 */
typedef enum
{
    QL_POWER_POWD_SUCCESS  = QL_SUCCESS,
    QL_POWER_RESET_SUCCESS = QL_POWER_POWD_SUCCESS,

    QL_POWER_CFW_CTRL_ERR                   =  1|QL_POWER_ERRCODE_BASE,
    QL_POWER_CFW_CTRL_RSP_ERR,
    QL_POWER_CFW_RESET_BUSY,

    QL_POWER_SEMAPHORE_CREATE_ERR           =  5|QL_POWER_ERRCODE_BASE,
    QL_POWER_SEMAPHORE_TIMEOUT_ERR,

    QL_POWER_POWD_EXECUTE_ERR               = 11|QL_POWER_ERRCODE_BASE,
    QL_POWER_POWD_INVALID_PARAM_ERR,

    QL_POWER_RESET_EXECUTE_ERR              = 21|QL_POWER_ERRCODE_BASE,
    QL_POWER_RESET_INVALID_PARAM_ERR,

    QL_POWER_UP_REASON_GET_ERR              = 31|QL_POWER_ERRCODE_BASE,
    QL_POWER_UP_REASON_MEM_NULL_ERR,

    QL_POWER_KEY_CB_NULL_ERR                = 41|QL_POWER_ERRCODE_BASE,
    QL_POWER_KEY_STATUS_GET_ERR,
    QL_POWER_KEY_MEM_NULL_ERR,
    QL_POWER_KEY_SHUTDOWN_TIME_SET_ERR,
    QL_POWER_KEY_LONGPRESS_TIME_SET_ERR,

    QL_POWER_USB_DETECT_INVALID_PARAM       = 51|QL_POWER_ERRCODE_BASE,
    QL_POWER_USB_DETECT_SAVE_NV_ERR,

    QL_POWER_DOMAIN_ID_INVALID_PARAM        = 61|QL_POWER_ERRCODE_BASE,
}ql_errcode_power;

typedef enum
{
	QL_SLEEP_SUCCESS      					= QL_SUCCESS,
	QL_SLEEP_INVALID_PARAM					= (QL_COMPONENT_PM_SLEEP << 16) | 1000,   /*  invalid input param  	   		 */
	QL_SLEEP_LOCK_CREATE_FAIL				= (QL_COMPONENT_PM_SLEEP << 16) | 1001,   /*  failed to create wake lock  		 */
	QL_SLEEP_LOCK_DELETE_FAIL				= (QL_COMPONENT_PM_SLEEP << 16) | 1002,   /*  failed to delete wake lock		 */
	QL_SLEEP_LOCK_LOCK_FAIL					= (QL_COMPONENT_PM_SLEEP << 16) | 1003,   /*  failed to lock the wake lock 	 */
	QL_SLEEP_LOCK_UNLOCK_FAIL               = (QL_COMPONENT_PM_SLEEP << 16) | 1004,   /*  failed to unlock the wake lock 	 */
	QL_SLEEP_LOCK_AUTOSLEEP_FAIL            = (QL_COMPONENT_PM_SLEEP << 16) | 1005,   /*  failed to enter auto sleep	 */
	QL_SLEEP_PARAM_SAVE_FAIL				= (QL_COMPONENT_PM_SLEEP << 16) | 1006,   /*  failed to save sleep param	 */
	QL_SLEEP_EXECUTE_FAIL                   = (QL_COMPONENT_PM_SLEEP << 16) | 1007,   /*  failed to execute	 */

    QL_SLEEP_SEMAPHORE_CREATE_ERR           = (QL_COMPONENT_PM_SLEEP << 16) | 1008,
    QL_SLEEP_SEMAPHORE_TIMEOUT_ERR          = (QL_COMPONENT_PM_SLEEP << 16) | 1009,
    QL_SLEEP_NO_MEM_ERR                     = (QL_COMPONENT_PM_SLEEP << 16) | 1010,
}ql_errcode_sleep;


#define QL_CHARGE_ERRCODE_BASE (QL_COMPONENT_BSP_CHARGE<<16)
/**
 * usb charge errcode
 */
typedef enum
{
    QL_CHARGE_SUCCESS  = QL_SUCCESS,

    QL_CHARGE_INVALID_PARAM_ERR                   =  1|QL_CHARGE_ERRCODE_BASE,
    QL_CHARGE_CHARGE_STATUS_ERROR,
    QL_CHARGE_BATTERY_DETECT_ERROR,
    QL_CHARGE_SAVE_NV_ERROR,
    QL_CHARGE_MEM_NULL_ERROR,
}ql_errcode_charge;

    
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_CHARGE
/**
 * usb charge status
 */
typedef enum
{
    //charge idle
    QL_CHG_IDLE = 0,
    //charging
    QL_CHG_CHARGING
} ql_charge_status_e;

typedef enum
{
	QL_CHARGER_CURRENT_300MA  = 300,
	QL_CHARGER_CURRENT_350MA  = 350,
	QL_CHARGER_CURRENT_400MA  = 400,
	QL_CHARGER_CURRENT_450MA  = 450,
	QL_CHARGER_CURRENT_500MA  = 500,
	QL_CHARGER_CURRENT_550MA  = 550,
	QL_CHARGER_CURRENT_600MA  = 600,
	QL_CHARGER_CURRENT_650MA  = 650,
	QL_CHARGER_CURRENT_700MA  = 700,
	QL_CHARGER_CURRENT_750MA  = 750,
	QL_CHARGER_CURRENT_800MA  = 800,
	QL_CHARGER_CURRENT_850MA  = 850,
	QL_CHARGER_CURRENT_900MA  = 900,
	QL_CHARGER_CURRENT_950MA  = 950,
	QL_CHARGER_CURRENT_1000MA = 1000,
	QL_CHARGER_CURRENT_1050MA = 1050,
	QL_CHARGER_CURRENT_1100MA = 1100,
	QL_CHARGER_CURRENT_1200MA = 1200,
	QL_CHARGER_CURRENT_MAX    = 1300,
}ql_charge_current_e;

typedef struct
{
	uint16_t rechg_vol;              //recharge vol,default:4150mV
    uint16_t chg_end_vol;			 //charge end vol,default:4210mV
    uint16_t standard_chg_current;   //charging current,default:700mA
    uint16_t chg_timeout;			 //total charging time,default:18000s
	uint32_t chg_ontime_ms;			 //battery charging on  time，unit: ms, range<1000,100000>,default:9000ms
	uint32_t chg_offtime_ms;		 //battery charging off time，unit: ms, range<1000,100000>,default:1000ms
}ql_charge_config_t;
#endif

/**
 * Power down mode select
 */
typedef enum
{
    POWD_IMMDLY,
    POWD_NORMAL,            //actually psm shutdown
    POWD_ORDINARY,          //ordinary shutdown.Ps: when pwk pin pull down, mode will restart after shutdown
}ql_PowdMode;

/**
 * Reset mode select
 */
typedef enum
{
    RESET_QUICK,
    RESET_NORMAL
}ql_ResetMode;

/**
 * Enable auto sleep
 */
typedef enum 
{
	QL_NOT_ALLOW_SLEEP = 0,
	QL_ALLOW_SLEEP,
}QL_SLEEP_FLAG_E;

/**
 * Power up reason
 */
typedef enum
{
    QL_PWRUP_UNKNOWN,         // unknown reason
    QL_PWRUP_PWRKEY,          // power up by power key
    QL_PWRUP_PIN_RESET,       // power up by pin reset
    QL_PWRUP_ALARM,           // power up by alarm
    QL_PWRUP_CHARGE,          // power up by charge in
    QL_PWRUP_WDG,             // power up by watchdog
    QL_PWRUP_PSM_WAKEUP,      // power up from PSM wakeup
    QL_PWRUP_PANIC            // power up by panic reset
}ql_pwrup_reason;

/**
 * Boot cause
 */
typedef enum
{
    QL_BOOTCAUSE_UNKNOWN = 0,           // placeholder for unknown reason
    QL_BOOTCAUSE_PWRKEY = (1 << 0),     // boot by power key
    QL_BOOTCAUSE_PIN_RESET = (1 << 1),  // boot by pin reset
    QL_BOOTCAUSE_ALARM = (1 << 2),      // boot by alarm
    QL_BOOTCAUSE_CHARGE = (1 << 3),     // boot by charge in
    QL_BOOTCAUSE_WDG = (1 << 4),        // boot by watchdog
    QL_BOOTCAUSE_PIN_WAKEUP = (1 << 5), // boot by wakeup
    QL_BOOTCAUSE_PSM_WAKEUP = (1 << 6), // boot from PSM wakeup
    QL_BOOTCAUSE_PANIC = (1 << 9),      // boot by panic reset
    QL_BOOTCAUSE_SW_RST = (1 << 15),    // boot by sw reset
}ql_boot_cause;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWK
/**
 * Pwrkey status
 */
typedef enum 
{
	PWRKEY_RELEASE = 0,
	PWRKEY_PRESSED
}QL_PWRKEY_STATUS_E;
#endif

typedef enum
{
    QL_HAL_POWER_LCD = 0,
    QL_HAL_POWER_SD,
    QL_HAL_POWER_SIM1,
    QL_HAL_POWER_VIBR,
    QL_HAL_POWER_VIO18,
}ql_power_domain_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint8_t  enable;
    uint8_t  no_data_time;
    uint16_t punish_time;
    uint16_t punish_remain_time;
}qsclkex_cfg_s;


#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
typedef struct
{
    bool is_valid;
    uint32_t periodic_TAU_time;
    uint32_t periodic_TAU_time_remain;
    uint32_t active_time;
    uint32_t active_time_remain;
}ql_psm_timer_s;

#endif
/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_power_down
*
* Description: power off for shutdown
* 
* Parameters:
*   powd_mode  [in] IMMDLY/NORMAL/ORDINARY power off
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_power_down(ql_PowdMode powd_mode);

/*****************************************************************
* Function: ql_power_reset
*
* Description: reset for shutdown
* 
* Parameters:
*   powd_mode  [in] QUICK/NORMAL reset
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_power_reset(ql_ResetMode reset_mode);

/*****************************************************************
* Function: ql_get_powerup_reason
*
* Description: get power on reason
* 
* Parameters:
*   pwrup_reason  [out] getted power on reason
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_get_powerup_reason(uint8_t *pwrup_reason);

/*****************************************************************
* Function: ql_get_boot_cause
*
* Description: get boot cause
* 
* Parameters:
*   pwrup_reason  [out] getted boot cause
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_get_boot_cause(uint32_t *boot_cause);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWK
/*****************************************************************
* Function: ql_get_pwrkey_status
*
* Description: get powerkey's status
*
* Parameters:
*   pwrkey_status  [out] getted powerkey's status
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_get_pwrkey_status(uint8_t *pwrkey_status);

/*****************************************************************
* Function: ql_pwrkey_callback_register
*
* Description: register powerkey's shutdown callback(trigger after pwrkey released)
*
* Parameters:
*   pwrkey_cb  [in] be registered callback
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_pwrkey_callback_register(ql_pwrkey_callback pwrkey_cb);

/*****************************************************************
* Function: ql_pwrkey_longpress_cb_register
*
* Description: register powerkey's callback when long press(but not release trigger)
*
* Parameters:
*   pwrkey_cb       [in] be registered callback
*   longpress_time  [in] needed powerkey pressed time
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_pwrkey_longpress_cb_register(ql_pwrkey_callback pwrkey_cb, uint32_t longpress_time);

/*****************************************************************
* Function: ql_pwrkey_press_cb_register
*
* Description: register powerkey's callback when press
*
* Parameters:
*   pwrkey_cb  [in] be registered callback
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_pwrkey_press_cb_register(ql_pwrkey_callback pwrkey_cb);

/*****************************************************************
* Function: ql_pwrkey_release_cb_register
*
* Description: register powerkey's callback when release from short press
*
* Parameters:
*   pwrkey_cb  [in] be registered callback
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_pwrkey_release_cb_register(ql_pwrkey_callback pwrkey_cb);

/*****************************************************************
* Function: ql_pwrkey_shutdown_time_set
*
* Description: set the time of shutdown by powerkey
*
* Parameters:
*   shutdown_time  [in] needed powerkey pressed time
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_errcode_power ql_pwrkey_shutdown_time_set(uint32_t shutdown_time);

#endif

/*****************************************************************
* Function: ql_lpm_wakelock_create
*
* Description: 用于创建休眠锁
* 
* Parameters:
*   lock_name  [in] 休眠锁名字 
	name_size  [in] 休眠锁名字长度
*
* Return:
*   >0          休眠锁id   
*	other 	    错误码。
*
*****************************************************************/
int ql_lpm_wakelock_create(char *lock_name, int name_size);

/*****************************************************************
* Function: ql_autosleep_enable
*
* Description: 用于使能普通休眠
* 
* Parameters:
*   sleep_flag  [in] 休眠选择
*
* Return:
*   0           成功  
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_sleep ql_autosleep_enable(QL_SLEEP_FLAG_E sleep_flag);

/*****************************************************************
* Function: ql_autosleepex_enable
*
* Description: 用于使能增强型休眠
* 
* Parameters:
*   sleep_flag  	[in] 休眠选择
*	no_data_time	[in] 无数据交互后释放RRC时间
*	punish_time		[in] 异常注网后不可使用增强休眠时间
*
* Return:
*   0           成功  
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_sleep ql_autosleepex_enable(QL_SLEEP_FLAG_E sleep_flag, uint8_t no_data_time, uint16_t punish_time);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
/*****************************************************************
* Function: ql_psm_sleep_enable
*
* Description: This api controls the setting of the UE's power saving mode (PSM) parameters.
* 
* Parameters:
*	nSim				[in] sim index，range: 0-1
*   psm_enable  		[in] Indication to disable or enable the use of PSM in the UE
*	periodic_TAU_time	[in] string type; one byte in an 8 bit format. Requested extended periodic TAU value (T3412) to be allocated to the UE in E-UTRAN. 
*							 The requested extended periodic TAU value is coded as one byte (octet 3) of the GPRS Timer 3 information element coded as bit 
*							 format (e.g. "01000111" equals 70 hours). For the coding and the value range, see the GPRS Timer 3 IE in 3GPP TS 24.008 [8] 
*							 Table 10.5.163a/3GPP TS 24.008. See also 3GPP TS 23.682 [149] and 3GPP TS 23.401 [82]. The default value, if available, is manufacturer specific
*						
*		GPRS Timer 3 value
*		Bits 5 to 1 represent the binary coded timer value
*		Bits 6 to 8 defines the timer value uint for the GPRS timer as follows
*		Bits
*		8	7	6
*		-----------------------------------------------------------
*		0	0	0	value is incremented in multiples of 10 minutes
*		0	0	1	value is incremented in multiples of 1 hour
*		0	1	0	value is incremented in multiples of 10 hours
*		0	1	1	value is incremented in multiples of 2 seconds
*		1	0	0	value is incremented in multiples of 30 seconds
*		1	0	1	value is incremented in multiples of 1 minute
*		1	1	0	value is incremented in multiples of 320 hours
*		1	1	1	value indicates that the timer is deactivated
*
*
*	active_time			[in] string type; one byte in an 8 bit format. Requested Active Time value (T3324) to be allocated to the UE. The requested Active Time 
*							 value is coded as one byte (octet 3) of the GPRS Timer 2 information element coded as bit format (e.g. "00100100" equals 4 minutes). 
*							 For the coding and the value range, see the GPRS Timer 2 IE in 3GPP TS 24.008 [8] Table 10.5.163/3GPP TS 24.008. See also 3GPP TS 23.682 [149], 
*							 3GPP TS 23.060 [47] and 3GPP TS 23.401 [82]. The default value, if available, is manufacturer specific.
*
*		GPRS Timer 2 value
*		Bits 5 to 1 represent the binary coded timer value
*		Bits 6 to 8 defines the timer value uint for the GPRS timer as follows
*		Bits
*		8	7	6
*		-----------------------------------------------------------
*		0	0	0	value is incremented in multiples of 2 seconds
*		0	0	1	value is incremented in multiples of 1 minute
*		0	1	0	value is incremented in multiples of decihours
*		1	1	1	value indicates that the timer is deactivated
*
* Return:
*   0           success  
*	other 	    error code
*
*****************************************************************/
ql_errcode_sleep ql_psm_sleep_enable(uint8_t nSim, bool psm_enable, const char *periodic_TAU_time, const char *active_time);
/*****************************************************************
* Function: ql_psm_register_enter_cb
*
* Description: Used to register the callback function before entering the PSM.
*              用于注册进入PSM模式前的回调函数。
*              
* Parameters:
*   ql_psm_enter_callback    [in]   callback function
*                                   回调函数
*                          
*   ctx                      [in]   parameter of the callback function
*                                   回调函数的传入参数
* Return:
*   0            success
*                注册状态成功。
*	other 	     error code
*                错误码。
*
*****************************************************************/
ql_errcode_sleep ql_psm_register_enter_cb(ql_psm_enter_callback cb, void *ctx);

/*****************************************************************
* Function: ql_psm_get_timer
*
* Description: Used to get periodic TAU time and active time which are from network and get the ramain time.
*              获取网络下发的TAU周期定时器值和活动定时器值和其剩余到期时间。
*              
* Parameters:
*   nSim        [in]    SIM index,range: 0-1.
                        SIM卡索引，取值：0-1。
*
*   timer       [out]   parameter of timer.See ql_psm_timer_s.
*                       timer结构体.见ql_psm_timer_s。
* Return:
*   0            success
*                获取成功。
*	other 	     error code
*                错误码。
*
*****************************************************************/
ql_errcode_sleep ql_psm_get_timer(uint8_t nSim,ql_psm_timer_s *timer);

#endif

/*****************************************************************
* Function: ql_lpm_wakelock_delete
*
* Description: 用于删除休眠锁
* 
* Parameters:
*   wakelock_fd  [in] 休眠锁id
*
* Return:
*   0           成功  
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_sleep ql_lpm_wakelock_delete(int wakelock_fd);

/*****************************************************************
* Function: ql_lpm_wakelock_lock
*
* Description: 锁定休眠锁，不允许系统进入休眠
* 
* Parameters:
*   wakelock_fd  [in] 休眠锁id
*
* Return:
*   0           成功  
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_sleep ql_lpm_wakelock_lock(int wakelock_fd);

/*****************************************************************
* Function: ql_lpm_wakelock_unlock
*
* Description: 释放休眠锁，所有休眠锁均释放后，系统可进入休眠
* 
* Parameters:
*   wakelock_fd  [in] 休眠锁id
*
* Return:
*   0           成功  
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_sleep ql_lpm_wakelock_unlock(int wakelock_fd);

#ifdef CONFIG_QL_OPEN_EXPORT_PKG
/*****************************************************************
* Function: ql_enter_sleep_callback
*
* Description: callback function before enter sleep
* 
* Parameters:
*   ctx 	    [out] 	reserved parameters 
*
* Return:
*   0           success。
*	other 	    error code。
*
*****************************************************************/
typedef void (*ql_enter_sleep_callback)(void *ctx);

/*****************************************************************
* Function: ql_exit_sleep_callback
*
* Description: callback function after exit sleep
* 
* Parameters:
*   ctx 	    [out] 	reserved parameters  
*
* Return:
*   0           success。
*	other 	    error code。
*
*****************************************************************/
typedef void (*ql_exit_sleep_callback)(void *ctx);

/*****************************************************************
* Function: ql_sleep_register_cb
*
* Description: registering sleep callback function
* 
* Parameters:
*   cb 	        [in] 	sleep callback function 
*
* Return:
*   0           success。
*	other 	    error code。
*
*****************************************************************/
ql_errcode_sleep ql_sleep_register_cb(ql_enter_sleep_callback cb);

/*****************************************************************
* Function: ql_wakeup_register_cb
*
* Description: registering wakeup callback function
* 
* Parameters:
*   cb 	        [in] 	wakeup callback function 
*
* Return:
*   0           success。
*	other 	    error code。
*
*****************************************************************/
ql_errcode_sleep ql_wakeup_register_cb(ql_exit_sleep_callback cb);

/*****************************************************************
* Function: ql_get_wakeup_source
*
* Description: get the wakeup source function
*
* Return:
*   0           do not enter sleep
*	other 	    
*               Wakeup source corresponding to each bit
*               QUEC_RESUME_SRC_PMIC 			(1 << 0)
*               QUEC_RESUME_SRC_VAD 			(1 << 1)
*               QUEC_RESUME_SRC_KEY 			(1 << 2)
*               QUEC_RESUME_SRC_GPIO1 			(1 << 3)
*               QUEC_RESUME_SRC_UART1 			(1 << 4 | 1<<5)  //UART1 and UART1_RXD
*               QUEC_RESUME_SRC_WCN2SYS 		(1 << 6)
*               QUEC_RESUME_SRC_WCN_OSC			(1 << 7)
*               QUEC_RESUME_SRC_IDLE_TIMER1		(1 << 8)
*               QUEC_RESUME_SRC_IDLE_TIMER2 	(1 << 9)
*               QUEC_RESUME_SRC_SELF		    (1 << 10)
*               QUEC_RESUME_SRC_USB_MON 		(1 << 11)
*
*****************************************************************/
uint ql_get_wakeup_source();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_CHARGE
/*****************************************************************
* Function: ql_get_vchg_vol
*
* Description: 用于获取USB端电压
* 
* Parameters:
*   vol 	    [out] 	USB端电压 
*
* Return:
*   0           获取电压成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_vchg_vol(uint32_t* vol);

/*****************************************************************
* Function: ql_get_vbus_state
*
* Description: 用于获取USB在位状态
* 
* Parameters:
*   state 	    [out] 	USB在位状态,0表示未连接USB,1表示已连接USB 
*
* Return:
*   0           获取状态成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_vbus_state(uint32_t* state);

/*****************************************************************
* Function: ql_get_battery_vol
*
* Description: 用于获取电池端电压
* 
* Parameters:
*   vol 	    [out] 	电池端电压 
*
* Return:
*   0           获取电压成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_battery_vol(uint32_t* vol);

/*****************************************************************
* Function: ql_get_battery_level
*
* Description: 用于获取电池电量百分比
* 
* Parameters:
*   level 	    [out] 	电池电量百分比 
*
* Return:
*   0           获取电量成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_battery_level(uint32_t* level);

/*****************************************************************
* Function: ql_get_charge_cur
*
* Description: 用于获取充电电流
* 
* Parameters:
*   cur 	    [out] 	充电电流 
*
* Return:
*   0           获取电流成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_charge_cur(uint32_t* cur);

/*****************************************************************
* Function: ql_get_charge_status
*
* Description: 用于获取当前充电状态
* 
* Parameters:
*   status 	    [out] 	充电状态,参考ql_charge_status_e枚举 
*
* Return:
*   0           获取状态成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_get_charge_status(ql_charge_status_e* status);


/*****************************************************************
* Function: ql_charge_set_config
*
* Description: 用于设置充电参数
* 
* Parameters:
*   charge_config [in] 	充电参数,参考ql_charge_config_t结构体
*
* Return:
*   0           设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_charge_set_config(ql_charge_config_t *charge_config);

/*****************************************************************
* Function: ql_charge_get_config
*
* Description: 用于获取充电参数
* 
* Parameters:
*   charge_config [out]  充电参数,参考ql_charge_config_t结构体
*
* Return:
*   0           设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_charge_get_config(ql_charge_config_t *charge_config);

/*****************************************************************
* Function: ql_charge_set_over_temp
*
* Description: 用于温度过高/低时设置保护
* 
* Parameters:
*   over_temp [in]  true-温度过高，停止充电， false-温度正常，恢复充电
*
* Return:
*   0           设置成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_charge ql_charge_set_over_temp(bool over_temp);

#endif
/*****************************************************************
* Function: ql_switch_power_domain
*
* Description: Voltage domain used to turn on/off when running/sleeping.
*              用于打开/关闭 运行/休眠时的电压域。
*              
* Parameters:
*   id        [in] 	电压域id，取值参考ql_power_domain_e 
*                          
*   enabled   [in] 	开关，正常运行时电压域是否打开。取值<0,1>
*
*   enabled   [in] 	开关，睡眠时电压域是否打开。取值<0,1>
* Return:
*   0            success
*                设置成功。
*	other 	     error code
*                错误码。
*
*****************************************************************/
ql_errcode_charge ql_switch_power_domain(uint32_t id, bool enabled, bool lp_enabled);
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_POWER_H */

