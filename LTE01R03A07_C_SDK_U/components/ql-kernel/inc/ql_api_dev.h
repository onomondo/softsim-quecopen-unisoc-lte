/**  
  @file
  ql_api_dev.h

  @brief
  quectel dev info interface definition.

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


#ifndef QL_API_DEV_H
#define QL_API_DEV_H


#include "ql_api_common.h"


#ifdef __cplusplus
    extern "C" {
#endif

/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QL_DEV_ERRCODE_BASE (QL_COMPONENT_STATE_INFO<<16)

#define CONFIG_AT_CFUN_SIM_DETECT_SUPPORT

#define QL_NOT_LIMIT_WRITE		    0			//Special meaning, which means that the number of dump writes is not limited
#define QL_LIMIT_WRITE_MAX_CNT 		1000		//The maximum cnt of dump writes allowed without clean dump cnt


#ifndef bool
typedef char bool;
#endif

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint32_t total_size;     ///< memory heap total size
    uint32_t avail_size;     ///< available size. The actual allocatable size may be less than this
    uint32_t max_block_size; ///< maximum allocatable block size
} ql_memory_heap_state_t;


typedef struct
{
	bool	 save_dump_info;    //true--save dumo info to file system  false--not save
	bool     del_dump_file;		//true--delete dump file, false--not delete
	bool     clean_dump_cnt;	//true--clean dump cnt, false--not clean
	uint16_t max_dump_cnt;		//the maximum number of times allowed to write dump. clean_dump_cnt can clear the count, range[QL_NOT_LIMIT_WRITE-QL_LIMIT_WRITE_MAX_COUNT]
}ql_dump_config_t;

typedef struct
{
	uint16_t current_dump_cnt;
	char    *dump_buffer;
	int      length;
}ql_dump_info_t;

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
//dev component detail errcode, 4 bytes
typedef enum
{
    QL_DEV_SUCCESS = QL_SUCCESS,
    
    QL_DEV_EXECUTE_ERR                      = 1|QL_DEV_ERRCODE_BASE,
    QL_DEV_MEM_ADDR_NULL_ERR,
    QL_DEV_INVALID_PARAM_ERR,
    QL_DEV_BUSY_ERR,
    QL_DEV_SEMAPHORE_CREATE_ERR,
    QL_DEV_SEMAPHORE_TIMEOUT_ERR,
	QL_DEV_HANDLE_INVALID_ERR,
    
    QL_DEV_CFW_CFUN_GET_ERR                 = 15|QL_DEV_ERRCODE_BASE,
    QL_DEV_CFW_CFUN_SET_CURR_COMM_FLAG_ERR  = 18|QL_DEV_ERRCODE_BASE,
    QL_DEV_CFW_CFUN_SET_COMM_ERR,
    QL_DEV_CFW_CFUN_SET_COMM_RSP_ERR,
    QL_DEV_CFW_CFUN_RESET_BUSY              = 25|QL_DEV_ERRCODE_BASE,
    QL_DEV_CFW_CFUN_RESET_CFW_CTRL_ERR,
    QL_DEV_CFW_CFUN_RESET_CFW_CTRL_RSP_ERR,
    
    QL_DEV_IMEI_GET_ERR                     = 33|QL_DEV_ERRCODE_BASE,
    
    QL_DEV_SN_GET_ERR                       = 36|QL_DEV_ERRCODE_BASE,
    
    QL_DEV_UID_READ_ERR                     = 39|QL_DEV_ERRCODE_BASE,
    
    QL_DEV_TEMP_GET_ERR                     = 50|QL_DEV_ERRCODE_BASE,
    
    QL_DEV_WDT_CFG_ERR                      = 53|QL_DEV_ERRCODE_BASE,
    
	QL_DEV_HEAP_QUERY_ERR 					= 56|QL_DEV_ERRCODE_BASE,

    QL_DEV_AUTHCODE_READ_ERR                = 90|QL_DEV_ERRCODE_BASE,
    QL_DEV_AUTHCODE_ADDR_NULL_ERR,  

    QL_DEV_READ_WIFI_MAC_ERR                = 100|QL_DEV_ERRCODE_BASE,              //read wifi mac address nv error
} ql_errcode_dev_e;

typedef enum
{
    QL_DEV_CFUN_MIN  = 0,
    QL_DEV_CFUN_FULL = 1,
    QL_DEV_CFUN_AIR  = 4,
} ql_dev_cfun_e;

/*========================================================================
 *  Variable Definition
 *========================================================================*/


/*========================================================================
 *  function Definition
 *========================================================================*/
/*
* to get the IMEI label
*/
ql_errcode_dev_e ql_dev_get_imei(char *p_imei, size_t imei_len, uint8_t nSim);

/*
* to get the SN label
*/
ql_errcode_dev_e ql_dev_get_sn(char *p_sn, size_t sn_len, uint8_t nSim);

/*
* to get the product ID
*/
ql_errcode_dev_e ql_dev_get_product_id(char *p_product_id, size_t product_id_len);

/*
* to get the model type
*/
ql_errcode_dev_e ql_dev_get_model(char *p_model, size_t model_len);

/*
* to get the firmware version of model
*/
ql_errcode_dev_e ql_dev_get_firmware_version(char *p_version, size_t version_len);

/*
* to get the firmware subversion of model
*/
ql_errcode_dev_e ql_dev_get_firmware_subversion(char *p_subversion, size_t subversion_len);

/*
* to get the cp version, maximum support 64 bytes.
*/
ql_errcode_dev_e ql_dev_get_cp_version(char* cp_version, uint8_t *len);
/*
* to get the unique chip id of model
*/
ql_errcode_dev_e ql_dev_get_cpu_uid(unsigned long long *p_chip_id);


/*
* to get the current functionality of model
*/
ql_errcode_dev_e ql_dev_get_modem_fun(uint8_t *p_function, uint8_t nSim);

/*
* to set the functionality of model or to reset the model
*/
ql_errcode_dev_e ql_dev_set_modem_fun(uint8_t at_dst_fun, uint8_t rst, uint8_t nSim);

/*
* to get temperature value of chip
*/
ql_errcode_dev_e ql_dev_get_temp_value(int32_t *ql_temp);

/*
* to config watchdog
*/
ql_errcode_dev_e ql_dev_cfg_wdt(uint8_t opt);

/*
* to feed wdt
*/
ql_errcode_dev_e ql_dev_feed_wdt(void);

/*
* to get wdt cfg
*/
ql_errcode_dev_e ql_dev_get_wdt_cfg(uint8_t *opt);

/*
* to query memory heap state information
*/
ql_errcode_dev_e ql_dev_memory_size_query(ql_memory_heap_state_t *ql_heap_state);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SECURE_BOOT
/*
* get flag of secure boot enable
*/
ql_errcode_dev_e ql_dev_get_secure_boot_enable_state(char *state);

/*
* to enable secure boot
*/
ql_errcode_dev_e ql_dev_enable_secure_boot();


/*
* to enable secure boot with flag of whether reset
*/
ql_errcode_dev_e ql_dev_enable_secure_boot_ex(bool reset);
#endif

/*
* to read mac address from wifi mac nv
*/
ql_errcode_dev_e ql_dev_get_mac_address(char *mac_address, size_t mac_len);

#ifdef CONFIG_QL_OPEN_EXPORT_PKG
/*
* to config dump parameter
*/
ql_errcode_dev_e ql_dev_config_dump(ql_dump_config_t *dump_config);

/*
* to get dump info
* if dump_info->dump_buffer is NULL, you can get current_dump_cnt and really data len, 
* else you can also get data with specify len.
*/
ql_errcode_dev_e ql_dev_get_dump_info(ql_dump_info_t *dump_info);
#endif

/*
* to get systerm random number
*/
uint32_t ql_dev_get_sys_srand();

#ifdef __cplusplus
    } /*"C" */
#endif

#endif /* QL_API_DEV_H */

