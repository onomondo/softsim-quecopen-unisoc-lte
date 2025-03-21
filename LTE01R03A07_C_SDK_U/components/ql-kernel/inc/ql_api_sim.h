/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#ifndef QL_API_SIM_H
#define QL_API_SIM_H

#include <stddef.h>

#include "ql_api_common.h"
#include "ql_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QL_SIM_ERRCODE_BASE (QL_COMPONENT_SIM<<16)

#define QL_SIM_PIN_LEN_MAX  8          // Maximum length of PIN data. 

#define QL_PBK_ITEM_COUNT_MAX 20
#define QL_PBK_USERNAME_LEN_MAX 32
#define QL_PBK_PHONENUM_LEN_MAX 24

#define QL_SIM_APDU_DATA_MAX 512
#define QL_SIM_APDU_CHANNEL_DEF 0

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
//SIM componant detail errcode, 4 bytes
typedef enum
{
    QL_SIM_SUCCESS                          = 0,
    QL_SIM_EXECUTE_ERR                      = 1|QL_SIM_ERRCODE_BASE,
    QL_SIM_MEM_ADDR_NULL_ERR,
    QL_SIM_INVALID_PARAM_ERR,
    QL_SIM_NO_MEMORY_ERR,
    QL_SIM_SEMAPHORE_CREATE_ERR             = 5|QL_SIM_ERRCODE_BASE,
    QL_SIM_SEMAPHORE_TIMEOUT_ERR,
    QL_SIM_NOT_INSERTED_ERR,
    QL_SIM_CFW_IMSI_GET_REQUEST_ERR,
    QL_SIM_CFW_IMSI_GET_RSP_ERR,
    QL_SIM_CFW_ICCID_GET_REQUEST_ERR        = 10|QL_SIM_ERRCODE_BASE,
    QL_SIM_CFW_PHONE_NUM_GET_REQUEST_ERR,
    QL_SIM_CFW_PHONE_NUM_GET_RSP_NULL_ERR,
    QL_SIM_CFW_STATUS_GET_REQUEST_ERR,
    QL_SIM_CFW_PIN_ENABLE_REQUEST_ERR,
    QL_SIM_CFW_PIN_DISABLE_REQUEST_ERR      = 15|QL_SIM_ERRCODE_BASE,
    QL_SIM_CFW_PIN_VERIFY_REQUEST_ERR,
    QL_SIM_CFW_PIN_CHANGE_REQUEST_ERR,
    QL_SIM_CFW_PIN_UNBLOCK_REQUEST_ERR,
    QL_SIM_OPERATION_NOT_ALLOWED_ERR,
    QL_SIM_OPTION_NOT_SUPPORT_ERR           = 20|QL_SIM_ERRCODE_BASE,

    QL_PBK_NOT_EXIT_ERR                     = 100|QL_SIM_ERRCODE_BASE,
    QL_PBK_NOT_INIT_ERR,
    QL_PBK_ITEM_NOT_FOUND_ERR,
} ql_sim_errcode_e;


typedef enum 
{
    QL_SIM_STATUS_READY                 = 0,
    QL_SIM_STATUS_PIN1_READY,
    QL_SIM_STATUS_SIMPIN,
    QL_SIM_STATUS_SIMPUK,
    QL_SIM_STATUS_PHONE_TO_SIMPIN,
    QL_SIM_STATUS_PHONE_TO_FIRST_SIMPIN = 5,
    QL_SIM_STATUS_PHONE_TO_FIRST_SIMPUK,
    QL_SIM_STATUS_SIMPIN2,
    QL_SIM_STATUS_SIMPUK2,
    QL_SIM_STATUS_NETWORKPIN,
    QL_SIM_STATUS_NETWORKPUK            = 10,
    QL_SIM_STATUS_NETWORK_SUBSETPIN,
    QL_SIM_STATUS_NETWORK_SUBSETPUK,
    QL_SIM_STATUS_PROVIDERPIN,
    QL_SIM_STATUS_PROVIDERPUK,
    QL_SIM_STATUS_CORPORATEPIN          = 15,
    QL_SIM_STATUS_CORPORATEPUK,
    QL_SIM_STATUS_NOSIM,
    QL_SIM_STATUS_PIN1BLOCK,
    QL_SIM_STATUS_PIN2BLOCK,
    QL_SIM_STATUS_PIN1_DISABLE          = 20,
    QL_SIM_STATUS_SIM_PRESENT,
    QL_SIM_STATUS_UNKNOW,
}ql_sim_status_e;

typedef enum
{
    QL_SIM_PBK_STORAGE_FD, // (U)SIM fix dialing-phone book
    QL_SIM_PBK_STORAGE_LD, // (U)SIM last-dialing-phone book
    QL_SIM_PBK_STORAGE_ME, // Mobile equipment phonebook
    QL_SIM_PBK_STORAGE_ON, // SIM (or MT) own numbers (MSISDNs) list
    QL_SIM_PBK_STORAGE_SM, // SIM/UICC phonebook
    QL_SIM_PBK_STORAGE_MAX,
} ql_sim_pbk_storage_e;

typedef enum
{
    QL_SIM_PBK_GSM,
    QL_SIM_PBK_HEX,
    QL_SIM_PBK_UCS2,
    QL_SIM_PBK_GBK,
    QL_SIM_PBK_IRA,
    QL_SIM_PBK_ENCODE_MAX,
} ql_sim_pbk_encoding_e;

typedef enum
{
    PBK_INIT_NONE  = 0x00,
    PBK_INIT_DOING = 0x01,
    PBK_INIT_DONE  = 0x02,
    PBK_INIT_ERR   = 0x04,
} ql_sim_pbk_init_e;

typedef struct
{
    uint8_t state;     /* init state */
    uint16_t used;     /* used num */
    uint16_t total;    /* total num */
    uint16_t max_idx;  /* max index, 0:disable */
} ql_sim_pbk_ctx_t;

typedef enum{
    QL_SIM_HOTPLUG_DISABLE          = 0,
    QL_SIM_HOTPLUG_ENABLE           = 1,
}ql_sim_hotplug_e;

typedef enum{
    QL_SIM_HOTPLUG_STATUS_OUT       = 0,
    QL_SIM_HOTPLUG_STATUS_IN        = 1,
}ql_sim_hotplug_status_e;

/*========================================================================
*  Type Definition
*========================================================================*/
typedef struct 
{
    uint8_t                     pin_value[QL_SIM_PIN_LEN_MAX+1];      /*  Value of the PIN */
}ql_sim_verify_pin_info_s;  

typedef struct 
{
    uint8_t                     old_pin_value[QL_SIM_PIN_LEN_MAX+1];  /**< Value of the old PIN as a sequence of ASCII characters. */
    uint8_t                     new_pin_value[QL_SIM_PIN_LEN_MAX+1];  /**< Value of the new PIN as a sequence of ASCII characters. */
}ql_sim_change_pin_info_s; 

typedef struct 
{
    uint8_t                     puk_value[QL_SIM_PIN_LEN_MAX+1];      /**< Value of the PUK as a sequence of ASCII characters. */
    uint8_t                     new_pin_value[QL_SIM_PIN_LEN_MAX+1];  /**< Value of the new PIN as a sequence of ASCII characters. */
}ql_sim_unblock_pin_info_s;

typedef struct
{
    int index;                                 /**< the item index in phone book */
    uint8_t username[QL_PBK_USERNAME_LEN_MAX]; /**< user name */
    uint8_t phonenum[QL_PBK_PHONENUM_LEN_MAX]; /**< phone number, it can include '+'. when write, if phonenum is empty, it means to delete this item specified by index*/
    uint8_t username_len;                      /**< user name len*/
} ql_sim_pbk_item_info_s;

typedef struct
{
    int item_count;                                     /**< the count of items */
    ql_sim_pbk_item_info_s item[QL_PBK_ITEM_COUNT_MAX]; /**< the list of items */
} ql_sim_pbk_itemset_info_s;

typedef struct
{
    ql_GpioNum gpio_num;
    ql_LvlMode gpio_level;
}ql_sim_hotplug_gpio_s;

typedef struct
{
    uint16_t attemptCount;
    uint16_t realCount;
}ql_sim_write_count_s;

typedef struct
{
    uint32_t req_apdu_len;                      /**< Request APDU data length. */
    uint8_t req_apdu[QL_SIM_APDU_DATA_MAX];     /**< Request APDU data. */

    uint32_t resp_apdu_len;                     /**< Response APDU data length. */
    uint8_t resp_apdu[QL_SIM_APDU_DATA_MAX];    /**< Response APDU data. */
} ql_sim_apdu_t;

/*========================================================================
  *  Callback Definition
  *========================================================================*/
typedef void (*ql_pbk_event_handler_t)(uint8_t nSim, int event_id, void *ctx);
extern ql_pbk_event_handler_t ql_pbk_call_callback;

typedef void (*ql_sim_hotplug_cb_t)(uint8_t nSim, ql_sim_hotplug_status_e status);

/*========================================================================
 *	function Definition
 *========================================================================*/
 
/*****************************************************************
* Function: ql_sim_get_imsi
*
* Description:
* 	获取 SIM 卡的 IMSI。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	imsi	  		[out] 		SIM 卡的 IMSI，字符串。  
* 	imsi_len	  	[in] 		imsi 数组的长度。
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_imsi(uint8_t nSim, char *imsi, size_t imsi_len);

/*****************************************************************
* Function: ql_sim_get_iccid
*
* Description:
* 	获取 SIM 卡的 ICCID。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	iccid	  		[out] 		SIM 卡的 ICCID，字符串。 
* 	iccid_len	  	[in] 		iccid 数组的长度。  
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_iccid(uint8_t nSim, char *iccid, size_t iccid_len);

/*****************************************************************
* Function: ql_sim_get_phonenumber
*
* Description:
* 	获取 SIM 卡本机号码。 
* 
* Parameters:
*	nSim        		[in]    SIM卡索引，取值：0-1
* 	phonenumber	  		[out] 	SIM 卡本机号码，字符串。  
* 	phonenumber_len	  	[in] 	phonenumber 数组的长度。  
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_phonenumber(uint8_t nSim, char *phonenumber,size_t phonenumber_len);
/*****************************************************************
* Function: ql_sim_get_card_status_phy
*
* Description:
* 	获取 SIM 卡实时状态信息。
* 
* Parameters:
*	nSim        	    [in]    SIM卡索引，取值：0-1
* 	card_status	  		[out] 	返回 SIM 卡类型信息，见 ql_sim_status_e 
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_card_status_phy(uint8_t nSim, ql_sim_status_e *card_status);

/*****************************************************************
* Function: ql_sim_get_card_status
*
* Description:
* 	获取 SIM 卡缓存状态信息。
* 
* Parameters:
*	nSim        	    [in]    SIM卡索引，取值：0-1
* 	card_status	  		[out] 	返回 SIM 卡类型信息，见 ql_sim_status_e 
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_card_status(uint8_t nSim, ql_sim_status_e *card_status);

/*****************************************************************
* Function: ql_sim_enable_pin
*
* Description:
* 	启用 SIM 卡 PIN 码验证，重启生效。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[in] 	输入 PIN 码，见 ql_sim_verify_pin_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_enable_pin(uint8_t nSim, ql_sim_verify_pin_info_s *pt_info);

/*****************************************************************
* Function: ql_sim_disable_pin
*
* Description:
* 	关闭 SIM 卡 PIN 码验证。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[in] 	输入 PIN 码，见 ql_sim_verify_pin_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_disable_pin(uint8_t nSim, ql_sim_verify_pin_info_s *pt_info);

/*****************************************************************
* Function: ql_sim_verify_pin
*
* Description:
* 	当 SIM 状态为请求 PIN/PIN2，输入 PIN/PIN2 码，进行验证。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[in] 	输入 PIN 码，见 ql_sim_verify_pin_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_verify_pin(uint8_t nSim, ql_sim_verify_pin_info_s *pt_info);

/*****************************************************************
* Function: ql_sim_change_pin
*
* Description:
* 	启用 SIM 卡 PIN 码验证后，更改 SIM 卡的 PIN 码。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[in] 	输入旧的 PIN 和新的 PIN，见 ql_sim_change_pin_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_change_pin(uint8_t nSim, ql_sim_change_pin_info_s *pt_info);

/*****************************************************************
* Function: ql_sim_unblock_pin
*
* Description:
* 	当多次错误输入 PIN/PIN2 码后，SIM 卡状态为请求 PUK/PUK2 时，输入 PUK/PUK2 码和新的PIN/PIN2 码进行解锁,
*   解PUK锁成功后，SIM卡会自动启用PIN 码验证，用户需再次调用ql_sim_disable_pin()关闭 SIM 卡 PIN 码验证。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	pt_info	  		[in] 	输入 PUK/PUK2 码和新的 PIN/PIN2 码，见 ql_sim_unblock_pin_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_unblock_pin(uint8_t nSim, ql_sim_unblock_pin_info_s *pt_info);

/*****************************************************************
* Function: ql_sim_set_operate_id
*
* Description:
* 	设置需要操作的SIM卡卡槽ID，默认值为0；仅适用于双卡单待应用，以切换操作的物理卡槽。
* 
* Parameters:
*	nSim        	[in]    SIM卡卡槽ID，取值：0-1
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_set_operate_id(uint8_t nSim);

/*****************************************************************
* Function: ql_sim_get_operate_id
*
* Description:
* 	获取当前操作的SIM卡卡槽ID；仅适用于双卡单待应用。
* 
* Parameters:
*	NULL
*
* Return:
* 	SIM ID。
*
*****************************************************************/
uint8_t ql_sim_get_operate_id(void);

/*****************************************************************
* Function: ql_sim_set_hotplug
*
* Description:
* 	SIM卡热插拔使能函数
* 
* Parameters:
*   nSim        [in]    SIM卡卡槽ID，取值：0-1
*   status      [in]    SIM卡热插拔是否使能,取值: 1 ENABLE,0 DISABLE
*   gpio_cfg    [in]    SIM卡热插拔GPIO配置结构体,ENABLE时必须传入有效值,DISABLE时必须传入NULL
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_set_hotplug(uint8_t nSim, ql_sim_hotplug_e status, ql_sim_hotplug_gpio_s* gpio_cfg);


/*****************************************************************
* Function: ql_sim_hotplug_register_cb
*
* Description:
* 	注册SIM卡热插拔回调函数
* 
* Parameters:
*   cb          [in]    SIM卡热插拔回调函数
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。
*

*****************************************************************/
ql_sim_errcode_e ql_sim_hotplug_register_cb(ql_sim_hotplug_cb_t cb);


/*****************************************************************
* Function: ql_sim_get_write_count
*
* Description:
* 	获取SIM卡中文件的写次数
* 
* Parameters:
*   nSim        [in]    SIM卡卡槽ID，取值：0-1
*   count       [Out]   返回sim卡的写次数
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。	
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_write_count(uint8_t nSim, ql_sim_write_count_s *count);

/*****************************************************************
* Function: ql_sim_send_apdu
*
* Description:
* 	SIM卡 APDU 发送和接收
* 
* Parameters:
*   nSim        [in]      SIM卡卡槽ID，取值：0-1
*   channel_id  [in]      为后续扩展用，当前请填充QL_SIM_APDU_CHANNEL_DEF
*   p_apdu      [in/Out]  输入和输出APDU
*
* Return:
* 	QL_SIM_SUCCESS			成功。
*	other           		错误码。	
*
*****************************************************************/
ql_sim_errcode_e ql_sim_send_apdu(uint8_t nSim, uint8_t channel_id, ql_sim_apdu_t *p_apdu);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PBK
/*****************************************************************
* Function: ql_sim_read_pbk_item
*
* Description:
* 	获取SIM卡上指定电话本中的一条或多电话号码记录。
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	storage	  		[in] 	需要读取电话号码记录的电话本存储位置，见 ql_sim_pbk_storage_e
* 	start_index	  	[in] 	需要读取电话号码记录的起始编号，start_index 为 0 表示不使用编号获取电话号码记录 
* 	end_index	  	[in] 	需要读取电话号码记录的结束编号
* 	username	  	[in] 	当 start_index 为 0 时有效，电话号码中的用户名
* 	username_len	[in] 	当 start_index 为 0 时有效，电话号码中的用户名的长度
* 	itemset	  		[out] 	返回电话号码记录列表，见 ql_sim_pbk_itemset_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_read_pbk_item(uint8_t nSim,
                                      ql_sim_pbk_storage_e storage,
                                      int start_index,
                                      int end_index,
                                      uint8_t *username,
                                      uint8_t username_len,
                                      ql_sim_pbk_itemset_info_s *itemset);

/*****************************************************************
* Function: ql_sim_write_pbk_item
*
* Description:
* 	写入SIM卡上指定电话本中的一条电话号码记录。 
* 
* Parameters:
*	nSim        	[in]    SIM卡索引，取值：0-1
* 	storage	  		[in] 	需要读取电话号码记录的电话本存储位置   
* 	item	  		[in] 	需要写入的电话号码记录，见 ql_sim_pbk_item_info_s
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_write_pbk_item(uint8_t nSim,
                                       ql_sim_pbk_storage_e storage,
                                       ql_sim_pbk_item_info_s *item);

/*****************************************************************
* Function: ql_sim_set_pbk_ctx
*
* Description:
*   设置SIM卡上指定电话本中的context信息
* 
* Parameters:
*   nSim               [in]    SIM卡索引, 取值: 0-1
*   storage            [in]    需要设置的电话本存储位置 
*   ctx:               [in]    
*                              state, PBK初始化状态, 设置时可忽略
*                              used, 已用的存储单元, 设置时可忽略
*                              total, 总计的存储单元, 设置时可忽略
*                              max_idx, 用户配置的最大索引, 为0表示禁用(不限制), set接口实际只配置该参数
*
* Return:
*   QL_SIM_SUCCESS              成功。
*   other                       错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_set_pbk_ctx(uint8_t nSim, ql_sim_pbk_storage_e storage, ql_sim_pbk_ctx_t *ctx);

/*****************************************************************
* Function: ql_sim_get_pbk_ctx
*
* Description:
*   获取SIM卡上指定电话本中的context信息
* 
* Parameters:
*   nSim               [in]    SIM卡索引, 取值: 0-1
*   storage            [in]    需要获取的电话本存储位置 
*   ctx:               [out]    
*                              state, PBK初始化状态
*                              used, 已用的存储单元
*                              total, 总计的存储单元
*                              max_idx, 用户配置的最大索引, 实际最大索引为total和max_idx的较小值
*
* Return:
*   QL_SIM_SUCCESS              成功。
*   other                       错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_pbk_ctx(uint8_t nSim, ql_sim_pbk_storage_e storage, ql_sim_pbk_ctx_t *ctx);

/*****************************************************************
* Function: ql_sim_set_pbk_encoding
*
* Description:
* 	设置电话簿字符集,重启后恢复为QL_SIM_PBK_GSM
* 
* Parameters:
*	encoding		 [in]    字符集, 见ql_sim_pbk_encoding_e
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_set_pbk_encoding(ql_sim_pbk_encoding_e encoding);

/*****************************************************************
* Function: ql_sim_get_pbk_encoding
*
* Description:
* 	获取电话簿字符集,默认为QL_SIM_PBK_GSM
* 
* Parameters:
*	encoding		 [out]    字符集, 见ql_sim_pbk_encoding_e
*
* Return:
* 	QL_SIM_SUCCESS				成功。
*	other           		    错误码。
*
*****************************************************************/
ql_sim_errcode_e ql_sim_get_pbk_encoding(ql_sim_pbk_encoding_e *encoding);

/*****************************************************************
* Function: ql_pbk_callback_register
*
* Description:
* 	注册电话簿回调函数
* 
* Parameters:
*	cb		        [in]    回调函数
*
*****************************************************************/
void ql_pbk_callback_register(ql_pbk_event_handler_t cb);

#endif
#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_SIM_H*/

