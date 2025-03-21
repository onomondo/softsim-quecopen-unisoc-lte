/**  @file
  quec_atcmd_def.h

  @brief
  This file is used to define at command for different Quectel Project.

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



=================================================================*/
#ifndef QUEC_ATCMD_GENERAL_H
#define QUEC_ATCMD_GENERAL_H

#include "at_engine.h"
#include "ql_api_bt_common.h"
#include "quec_at_engine.h"
/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QUEC_BT_BLE_DEFALT_NAME     "quec_bt_def"
#define QUEC_INIT_DONE	0xFFFF

/* 3gpp Release veriosn */
#define RELEASE_VER_10                 (10)
#define RELEASE_VER_13                 (13)
#define EMOD_NV_RELEASE_VER_DATATYPE   (0)
#define EMOD_NV_RELEASE_VER_OFFSET     (524)

/*default IMEI*/
#define IMEI0_DEF "864430010001091"
#define IMEI1_DEF "352751019523267"

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define QUEC_USB_ENUM_USBNET_SERIAL		0
#define QEUC_USB_ENUM_CDC_ACM			1
#endif

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum egmr_type_e
{
    DSAT_EGMR_BASEBAND_CHIPSET = 0,
	DSAT_EGMR_DSP_CODE = 1,
	DSAT_EGMR_DSP_PATCH = 2,
	DSAT_EGMR_MCU_SW = 3,
	DSAT_EGMR_MD_BOARD = 4,
    DSAT_EGMR_SN = 5,
    DSAT_EGMR_IMEI = 7,
    DSAT_EGMR_MEID_GSN = 8,
    DSAT_EGMR_IMEISV_SVN = 9,
    DSAT_EGMR_IMEI2 = 10,
    DSAT_EGMR_MAX
}dsat_egmr_type_e;

typedef enum
{
    QUECTEL_URC_ALL_IND = 0,    //use to contol all urc report set true as default
    QUECTEL_URC_CSQ_IND,        // use to contol CSQ urc report
    QUECTEL_URC_ACTCHGRPT_IND,  //use to contol network mode change urc report
    QUECTEL_URC_SMSFULL_IND,    //use to control smsfull urc report
    QUECTEL_URC_SMSINCOMING_IND,//use to control sms incoming urc report
    QUECTEL_URC_RING_IND,
    QUECTEL_URC_PSM_IND,        //use to psm sleep/wakeup urc report
    QUECTEL_URC_MAX_IND,
}quectel_urc_ind_cfg_e_type;

typedef enum {
    AT_AND_C = 0,
    AT_AND_D,
    AT_AND_F,
    AT_AND_W,
    AT_E,
    AT_Q,
    AT_V,
    AT_X,
    AT_Z,
    AT_S_0,
    AT_S_3,
    AT_S_4,
    AT_S_5,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CALL_AT
    AT_S_7,
#endif
    AT_CONFIG_MAX_NUM,
}quec_at_config_type_e;

typedef enum
{
	QUEC_INIT_PROTOCOL_STACK,
	QUEC_INIT_BT,
	QUEC_INIT_AUDIO,
	QUEC_INIT_WIFI,
	QUEC_INIT_SLEEP,

	QUEC_INIT_MAX = 16,
	//0xFFFF indicates that all function modules have been initialized
}quec_init_bitmap_e;


/*========================================================================
*  Type Definition
*========================================================================*/
typedef void (*QCFG_ProcessFunc)(atCommand_t *);

typedef struct
{
    char *pCmdName;
    QCFG_ProcessFunc pCmdHandler;
    char *test_info;//set test info as "", it will ouput at the cmd handler; set it as null, it will be ignored.
} QCFG_SCmdListStruct;

typedef QCFG_SCmdListStruct quec_cfg_cmdlist_s;

typedef struct
{
    uint8_t sim_stat_urc;
    uint8_t slot_stat_urc;
} quec_at_setting_s;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_UART
typedef struct
{
    uint32_t baudrate;
	uint32_t icf_format;
	uint32_t icf_parity;
	uint32_t ifc_rx; 
    uint32_t ifc_tx;
} quec_uart_setting_s;

typedef struct
{
    uint32_t u1_rxmode;
    uint32_t u2_rxmode;
    uint32_t u3_rxmode;
	uint8_t atport1_switch;//记录辅助串口1是否用作ATPORT
	uint8_t atport2_switch;//记录辅助串口2是否用作ATPORT
} quec_uart_custom_nv_s;
#endif

#ifdef __QUEC_OEM_VER_HW__
typedef struct
{
    uint32_t quec_dorind_enable;          //使能hw定制功能，输出相应电平
    int quec_lvl_lock;                    //配合hw定制功能，上休眠锁，防止瞬间又进入休眠
    uint32_t quec_wakeuptime_flag;        //hw定制功能，定时器唤醒标志位
    uint32_t quec_wakeuptime_value;       //hw定制功能，定时器设定时间，单位s
    ql_timer_t quec_wakeuptime_timer;     //hw定制功能，唤醒定时器
}custom_hw;
#endif

#ifdef __QUEC_OEM_VER_QDMHYT__
typedef struct
{
    bool mask;
    bool gpio_lvl_low;
    uint8_t pin;
	uint8_t gpio;
    ql_timer_t quec_pwm_virt_timer;
	uint32_t high_level_time;
	uint32_t low_level_time; 
} quec_hytgpio_setting_s;
#endif

typedef struct {
    quec_at_config_type_e config_type;
    char  *config_name;
}quec_at_config_context_s;

typedef enum
{
    QUEC_BT_BLE_NO_TRANSTER = 0,                    //bt/ble no need transfer
    QUEC_BT_BLE_NEDD_TRANSTER,                      //bt/ble need transfer
} quec_bt_ble_name_transfer_type_e;

typedef enum
{
    QUEC_CODE_CP936_UTF8 = 0,               //from cp936(gbk) to utf8
    QUEC_CODE_UTF8_CP936,                   //from utf8 to cp936(gbk)
}quec_code_transfer_e;

/*========================================================================
 *	Variable Definition
 *========================================================================*/
extern quec_at_setting_s quec_at_current_setting;

/*========================================================================
 *	function Definition
 *========================================================================*/
void quec_cfg_common_handler(atCommand_t *cmd, quec_cfg_cmdlist_s *cmdlist);
void quec_at_setting_reset_default(void);
void quec_at_setting_read(void);
bool quec_wdt_cfg_read(void);
bool quec_at_setting_save(void);
bool quec_uart_setting_save(void);
void quec_creg_emergency_read(void);


uint8_t quectel_read_urc_ind_flag(quectel_urc_ind_cfg_e_type flag);
bool quec_get_3gpp_release_version(uint8_t *version, uint8_t nSim);
bool quec_set_3gpp_release_version(uint8_t version, uint8_t nSim);
void quectel_exec_qcfg_urc_ri_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_urc_delay_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_urc_interval_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_urc_cache_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_urc_signaltype_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_fota_cid_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_fota_http_timeout_cmd(atCommand_t *cmd);
void quectel_exec_qcfg_pdp_kalive_ip_cmd(atCommand_t *cmd);
void quec_at_ecc_read();
bool quec_convert_string_to_hex(unsigned char *str_ptr, unsigned short length, unsigned char *hex);

bool quec_bt_ble_set_localname(ql_bt_ble_local_name_s local_name);
bool quec_bt_ble_get_localname(ql_bt_ble_local_name_s *local_name, quec_bt_ble_name_transfer_type_e type);
bool quec_ble_write_ibeacon_cfg(ql_ble_ibeacon_cfg_s cfg);
bool quec_ble_read_ibeacon_cfg(ql_ble_ibeacon_cfg_s *cfg);
bool quec_code_transfer(char *from, int from_size, char *to, int *to_size, quec_code_transfer_e type);

void quec_init_set_status_bit(quec_init_bitmap_e bit);
bool quec_init_get_status_bit(quec_init_bitmap_e bit);

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
void quec_get_qgsmcert_from_nv(void);
uint8_t quec_get_stackcfg_gea(void);
#endif

#endif /* QUEC_ATCMD_GENERAL_H */

