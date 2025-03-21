/**  @file
  quec_aliot_smartcard.h

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
#ifndef QUEC_ALIIOT_SMARTCARD__H
#define QUEC_ALIIOT_SMARTCARD__H
#include "at_engine.h"

#define QUEC_SMARTCARD_EVN_VALUE_MAX 5
#define QUEC_SMARTCARD_URC_BUF_MAX_LEN 128

#define QUEC_SMARTCARD_RESULT_OK 			0
#define QUEC_SMARTCARD_RESULT_FAIL 			-1
#define QUEC_SMARTCARD_INIT_FAIL 			-2
#define QUEC_SMARTCARD_PARAM_ERR 			-3
#define QUEC_SMARTCARD_UNKNOW_SIM 			-4

#define QUEC_SMARTCARD_UNCHANGE_ICCID 		-5
#define QUEC_SMARTCARD_EXEC_TIMEOUT 		-6
#define QUEC_SMARTCARD_UNRECOGNIZED			-7

#define QUEC_SMARTCARD_PARSE_DATA_ERR		-9
#define QUEC_SMARTCARD_DIAL_FAIL			-10
#define QUEC_SMARTCARD_REGISTER_FAIL		-11

#define QUEC_SMARTCARD_SDK_UNENABLE			-12
#define QUEC_SMARTCARD_SCAN_FAIL			-13
#define QUEC_SMARTCARD_SWITCH_RUN			-14

#define QUEC_SMARTCARD_OPERATOR_DISABLE		-15
#define QUEC_SMARTCARD_PARSE_FILE_ERR		-16

#define QUEC_SMARTCARD_SCAN_UNENABLE		-17
#define QUEC_SMARTCARD_SWITCH_UNENABLE		-18
#define QUEC_SMARTCARD_MANUAL_UNENABLE		-19

#define QUEC_FREE_PTR(p)    {if((p) != NULL) {free(p);(p) = NULL;}}

typedef enum{
	QUEC_SMARTCARD_SDK_IDLE = 0,
    QUEC_SMARTCARD_SDK_RUN,
    QUEC_SMARTCARD_SDK_SHUT,

}quec_smartcard_sdk_state;

typedef enum{
	QUEC_ALIPAY_APP_OPT_IDLE_STAT = 0,
    QUEC_ALIPAY_APP_OPT_RUN_STAT,
	QUEC_ALIPAY_APP_OPT_RUN_INIT_FAIL,
}quec_smartcard_opt_state;

typedef enum{
	QUEC_SMARTCARD_INIT = 1,
	QUEC_SMARTCARD_SETENABLE = 2,
	QUEC_SMARTCARD_GETENABLE=3,
	QUEC_SMARTCARD_LIST=4,
	QUEC_SMARTCARD_SEL=5,
	QUEC_SMARTCARD_SETPOLICY=6,
	QUEC_SMARTCARD_GETPOLICY=7,
	QUEC_SMARTCARD_SCAN=8,
	QUEC_SMARTCARD_SAVE=9,
	QUEC_SMARTCARD_DEINIT = 10,
}tcpip_msg_id;


/*save parameter, 需要掉电保存的参数*/
typedef struct
{
    int     							rsv1;
}quec_smartcard_save;

typedef struct
{
	int 	startup_init;
}quec_smartcard_qcfg_info;

typedef struct
{
    int     					running;/*sdk run flag*/
	int							state;/*业务执行流程状态, app state*/
	ql_queue_t 					queue;
    ql_at_cmd_t *				cmd;
    ql_dispatch_t* 				dispatch;
    int     				    func_save;
    uint32_t     				interval_time;
    int     				    try_times;
    quec_smartcard_save     	smartcard_save;
	quec_smartcard_qcfg_info	qcfg;
}quec_aliot_smartcard_ctx;

typedef struct
{
    uint32_t     func_hac;
	uint32_t     func_sc;
    uint32_t     func_sw;
    uint32_t     func_mse;
}quec_aliot_smartcard_mnenable;

typedef struct
{
    uint16_t     policy;
    uint16_t     check_retry_cnt;
    uint16_t     delay_threshold;
    uint16_t     ping_cycle;
	uint16_t     ping_cnt_once;
    uint16_t     silent_time;
    char     	 ping_dest_ip[256];
	int     	 rssi_threshold;
}quec_aliot_smartcard_mnpolicy;

int quec_aliot_smartcard_int(void);
int quec_aliot_smartcard_sdk_sta(void);
int quec_aliot_smartcard_get_all_sim_info(void);
int quec_aliot_smartcard_manual_switch(void *arg);
int quec_aliot_smartcard_get_mnenable(void *arg);
int quec_aliot_smartcard_get_mnsave(void);
int quec_aliot_smartcard_set_mnenable(void *arg);
int quec_aliot_smartcard_scan_carrier(void);
int quec_aliot_smartcard_update_configs(void *arg);
int quec_aliot_smartcard_get_mnpolicy(void *arg);
int quec_aliot_smartcard_set_mnpolicy(void *arg);
void quec_aliot_smartcard_set_at_param(ql_at_cmd_t* atengine);
int quec_aliot_smartcard_event_send(uint32 id, void *arg);
int ql_aliot_smartcard_write_qcfg_nv( const void *data, int size);
int ql_aliot_smartcard_read_qcfg_nv( void *data, int size);
void quec_aliot_smartcard_startup_init(void);
int quec_aliot_smartcard_get_init_state(void);
int quec_aliot_smartcard_app_startup_cfg_fxn(int opt,int* startup);
#endif/*QUEC_ALIIOT_SMARTCARD__H*/
