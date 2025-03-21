/**  
  @file
  ql_api_datacall.h

  @brief
  This file provides the definitions for datacall, and declares the 
  API functions.

*/
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

#ifndef QL_API_DATACALL_H
#define QL_API_DATACALL_H

#include <stdint.h>
#include <stdbool.h>
#include "sockets.h"
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Marco Definition
 *========================================================================*/
#define QL_PDP_TYPE_IP_NONE             0 
#define QL_PDP_TYPE_IP                  1           
#define QL_PDP_TYPE_IPV6                2           
#define QL_PDP_TYPE_IPV4V6              3           

#define PROFILE_IDX_MIN                 1
#define PROFILE_IDX_VOLTE_MAX           5//支持volte时候最大允许的数传pdp
#define PROFILE_IDX_MAX                 7//不支持volte时最大允许的数传pdp

#define APN_LEN_MAX                     99
#define USERNAME_LEN_MAX                64
#define PASSWORD_LEN_MAX                64

#define PROFILE_IDX_NUM                 (PROFILE_IDX_MAX - PROFILE_IDX_MIN + 1 )

#define QL_PDP_DEACTIVED                0 
#define QL_PDP_ACTIVED                  1

#define QL_NAT_ASSIGN_NONE              0 
#define QL_NAT_ASSIGN_IP                1
#define QL_NAT_ASSIGN_IP_PORT           2

#define QL_NET_ADDRESS_LEN_MAX          16


#define QL_DATACALL_ERRCODE_BASE        (QL_COMPONENT_NETWORK_MANAGE<<16)
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/

typedef enum
{
    QL_PDP_AUTH_TYPE_NONE = 0,		            //no authentication protocol is used for this PDP context. Username and password are removed if previously specified
    QL_PDP_AUTH_TYPE_PAP,    			        
    QL_PDP_AUTH_TYPE_CHAP,				            
    QL_PDP_AUTH_TYPE_PAP_OR_CHAP,
}ql_pdp_auth_type_e;

typedef enum
{
	QL_DATACALL_SUCCESS                     = 0,
    QL_DATACALL_EXECUTE_ERR                 = 1 | QL_DATACALL_ERRCODE_BASE,  
    QL_DATACALL_MEM_ADDR_NULL_ERR,         
    QL_DATACALL_INVALID_PARAM_ERR,
    QL_DATACALL_NW_REGISTER_TIMEOUT_ERR,
    QL_DATACALL_CFW_ACT_STATE_GET_ERR       = 5 | QL_DATACALL_ERRCODE_BASE,
    QL_DATACALL_REPEAT_ACTIVE_ERR,
    QL_DATACALL_REPEAT_DEACTIVE_ERR,
    QL_DATACALL_CFW_PDP_CTX_SET_ERR,
    QL_DATACALL_CFW_PDP_CTX_GET_ERR,
    QL_DATACALL_CS_CALL_ERR                 = 10 | QL_DATACALL_ERRCODE_BASE,
    QL_DATACALL_CFW_CFUN_GET_ERR,
    QL_DATACALL_CFUN_DISABLE_ERR,
    QL_DATACALL_NW_STATUS_GET_ERR,
    QL_DATACALL_NOT_REGISTERED_ERR,
    QL_DATACALL_NO_MEM_ERR                  = 15 | QL_DATACALL_ERRCODE_BASE,
    QL_DATACALL_CFW_ATTACH_STATUS_GET_ERR,
    QL_DATACALL_SEMAPHORE_CREATE_ERR,
    QL_DATACALL_SEMAPHORE_TIMEOUT_ERR,
    QL_DATACALL_CFW_ATTACH_REQUEST_ERR,
    QL_DATACALL_CFW_ACTIVE_REQUEST_ERR      = 20 | QL_DATACALL_ERRCODE_BASE,
    QL_DATACALL_ACTIVE_FAIL_ERR,
    QL_DATACALL_CFW_DEACTIVE_REQUEST_ERR,
    QL_DATACALL_NO_DFTPDN_CFG_CONTEXT,
    QL_DATACALL_NO_DFTPDN_INFO_CONTEXT,
}ql_datacall_errcode_e;

/*========================================================================
*  Type Definition
*========================================================================*/

struct v4_address_status
{
	ip4_addr_t ip;
	ip4_addr_t pri_dns;
	ip4_addr_t sec_dns;
};

struct v4_info
{
	int state;                          //dial status
	struct v4_address_status addr;      //IPv4 address information
};

struct v6_address_status
{
	ip6_addr_t  ip;
	ip6_addr_t  pri_dns;
	ip6_addr_t  sec_dns;
};

struct v6_info
{
	int state;                          //dial status
	struct v6_address_status addr;      //IPv6 address information
};

typedef struct
{
	int profile_idx;
	int ip_version;
	struct v4_info v4;
	struct v6_info v6;
    uint8_t auth_type;
    uint8_t apn_user[USERNAME_LEN_MAX+1];
    uint8_t apn_pwd[PASSWORD_LEN_MAX+1];
    uint8_t apn[APN_LEN_MAX+1];
}ql_data_call_info_s;

typedef struct
{
	int ip_version;
	char apn_name[APN_LEN_MAX+1];
	char username[USERNAME_LEN_MAX+1];
	char password[PASSWORD_LEN_MAX+1];
	int auth_type;
}ql_data_call_conf_s;

typedef struct
{
	uint8_t			ip_version;
	ip4_addr_t      ipv4;
	ip6_addr_t      ipv6;
	char 			apn_name[APN_LEN_MAX];
}ql_data_call_default_pdn_info_s;

typedef ql_data_call_conf_s ql_data_call_default_pdn_cfg_s;

typedef struct
{
	char subnet_addr[QL_NET_ADDRESS_LEN_MAX];
	char subnet_mask[QL_NET_ADDRESS_LEN_MAX];
}ql_nat_subnet_config_s;

typedef struct 
{
  	u8_t type;
	ip4_addr_t ip4;
	ip6_addr_t ip6;
}ql_datacall_dns_info_s;

typedef struct
{
    uint8_t status;
    uint8_t ip_ver;
	ip4_addr_t ip_addr;
	ip4_addr_t dns_server;
	ip4_addr_t virt_dns_server;
}ql_datacall_qdmz_cfg_s;

typedef enum
{
    QL_NAT_ASSIGN_TYPE_USBNET = 0,
    QL_NAT_ASSIGN_TYPE_PPP,
    QL_NAT_ASSIGN_TYPE_MAX
}ql_nat_assign_type_e;

typedef struct
{
    uint8_t mode;            /* 0:off 1:ip 2:ip/port */
	ip4_addr_t ip_addr;
	ip4_addr_t gw_addr;
	ip4_addr_t dns_addr;
}ql_nat_assign_cfg_s;


extern ql_datacall_qdmz_cfg_s ql_qdmz_cfg;

typedef struct
{
    uint8_t status;
	ip4_addr_t mcu_ip_addr;
	ip4_addr_t mcu_netmask;
	ip4_addr_t mcu_gw;
	ip4_addr_t lwip_ip_addr;
	ip4_addr_t lwip_netmask;
	ip4_addr_t lwip_gw;    
}ql_datacall_usbnet_customer_cfg_s;
/*========================================================================
 *	function Definition
 *========================================================================*/


/*****************************************************************
* Description: datacall callback
* 
* Parameters:
*   ind_type        [in]    事件类型
* 	profile_idx	    [in] 	PDP索引 
* 	result	  		[in] 	执行结果
*
*****************************************************************/
typedef void (*ql_datacall_callback)(uint8_t nSim, unsigned int ind_type, int profile_idx, bool result, void *ctx);

/*****************************************************************
* Function: ql_network_register_wait
*
* Description:
* 	等待网络注册 
* 
* Parameters:
*   nSim					[in] 	SIM卡索引，取值0-1。
* 	timeout_s	  			[in] 	超时时间，单位秒。 
*
* Return:
* 	0			注册成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_network_register_wait(uint8_t nSim, unsigned int timeout_s);


/*****************************************************************
* Function: ql_set_data_call_asyn_mode
*
* Description:
* 	设置ql_start_data_call和ql_stop_data_call接口的执行模式，并设置异步模式的回调函数， 
* 
* Parameters:
*   nSim				[in] 	SIM卡索引，取值0-1。
*   deactive_cb         [in]    PDP索引，取值1-7。 
* 	enable	  			[in] 	1：使能异步模式；0：不使能异步模式 
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_set_data_call_asyn_mode(uint8_t nSim, int profile_idx, bool enable);


/*****************************************************************
* Function: ql_start_data_call
*
* Description:
*	启动拨号。默认为同步模式，可由ql_set_data_call_asyn_mode设置为异步模式
* 
* Parameters:
*   nSim				[in] 	SIM卡索引，取值0-1。
* 	profile_idx	  		[in] 	PDP索引，取值1-7。 
                                第1路配置会复制给默认承载,配置改变后需要重新注网才生效.
* 	ip_version	  		[in] 	IP类型：1 为IPv4，2 为IPv6，3 为IPv4v6。
* 	apn_name	  		[in] 	apn名称。
* 	username	  		[in] 	用户名称。
* 	password	  		[in] 	用户密码。
* 	auth_type	  		[in] 	认证类型: 0 NONE
*										  1 PAP
*										  2 CHAP
*                                         3 PAP or CHAP
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_start_data_call(uint8_t nSim, int profile_idx, int ip_version, char *apn_name, char *username, char *password, int auth_type);

/*****************************************************************
* Function: ql_get_data_call_info
*
* Description:
* 	获取拨号信息。 
* 
* Parameters:
*   nSim				[in] 	SIM卡索引，取值0-1。
* 	profile_idx	  		[in] 	PDP索引，取值1-7。 
*	info				[out] 	获取拨号信息。 
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_get_data_call_info(uint8_t nSim, int profile_idx, ql_data_call_info_s *info);

/*****************************************************************
* Function: ql_stop_data_call
*
* Description:
* 	关闭拨号。 默认为同步模式，可由ql_set_data_call_asyn_mode设置为异步模式
* 
* Parameters:
*   nSim				[in] 	SIM卡索引，取值0-1。
* 	profile_idx	  		[in] 	PDP索引，取值1-7。 
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_stop_data_call(uint8_t nSim, int profile_idx);

/*****************************************************************
* Function: ql_datacall_register_cb
*
* Description: 注册数据拨号的回调函数
* 
* Parameters:
*   nSim			[in] 	SIM卡索引，取值0-1。
*   profile_idx     [in]    PDP索引，取值1-7。 
*   datacall_cb     [in]    需要注册的回调函数
*   ctx             [in]    回调函数的参数
*
* Return:
* 	0			注册成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_register_cb(uint8_t nSim, int profile_idx, ql_datacall_callback datacall_cb, void *ctx);

/*****************************************************************
* Function: ql_datacall_unregister_cb
*
* Description: 取消已注册的数据拨号回调函数
* 
* Parameters:
*   nSim			[in] 	SIM卡索引，取值0-1。
*   profile_idx     [in]    PDP索引，取值1-7。 
*   datacall_cb     [in]    需要取消注册的回调函数
*   ctx             [in]    回调函数的参数
*
* Return:
* 	0			取消注册成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_unregister_cb(uint8_t nSim, int profile_idx, ql_datacall_callback datacall_cb, void *ctx);

/*****************************************************************
* Function: ql_datacall_get_sim_profile_is_active
*
* Description: 获取当前PDP是否激活
* 
* Parameters:
*   nSim			[in] 	SIM卡索引，取值0-1。
*   profile_idx     [in]    PDP索引，取值1-7。 
*
* Return:
* 	0			未激活
*	1 	        已激活
*
*****************************************************************/
bool ql_datacall_get_sim_profile_is_active(uint8_t nSim, int profile_idx);


/*****************************************************************
* Function: ql_bind_sim_and_profile
*
* Description:
* 	bind sim_id and profile_id to get sim_cid
* 
* Parameters:
*   nSim				[in] 	SIM卡索引，取值0-1。
* 	profile_idx	  		[in] 	PDP索引，取值1-7。 
*	sim_cid				[out]	nSim << 5 | profile_idx
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_bind_sim_and_profile(uint8_t nSim, int profile_idx, uint16_t *sim_cid);


/*****************************************************************
* Function: ql_get_sim_and_profile
*
* Description:
* 	get sim_id and profile_id from sim_cid
* 
* Parameters: 
*	sim_cid				[in]	nSim << 5 | profile_idx
*   nSim				[out] 	SIM卡索引，取值0-1。
* 	profile_idx	  		[out] 	PDP索引，取值1-7。
*
* Return:
* 	0			成功。
*	other 	    错误码。
*
*****************************************************************/
ql_datacall_errcode_e ql_get_sim_and_profile(uint16_t sim_cid, uint8_t *nSim, int *profile_idx);


/*****************************************************************
* Function: ql_datacall_set_nat
*
* Description:
* 	Enable NAT function for sim id and profile id, reboot to take effect
* 
* Parameters: 
*	sim_profile_list				[in]	A list of combinations of SIM and profile, the low 16 bits represent SIM 0 and the 
*											high 16 bits represent SIM 1。 Each bit represents a corresponding profile id.
*											
*									example: 0x10005 ===》0001    0000 0000 0000 0101	profile 1 and 3 of SIM 0 and profile 1 of SIM 1 are enabled
*														  ----	 -------------------
*														  SIM1			SIM0
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_nat(uint32_t sim_profile_list);


/*****************************************************************
* Function: ql_datacall_get_nat
*
* Description:
* 	get sim id and profile id that enabled NAT function
* 
* Parameters: 
*	sim_profile_list				[out]	A list of combinations of SIM and profile
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_nat(uint32_t *sim_profile_list);

/*****************************************************************
* Function: ql_datacall_set_subnet
*
* Description:
* 	If NAT function is enabled, the network address and subnet mask can be set through this API, 
*	configuration parameters are not lost when power is down
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
* 	profile_idx	  		[in] 	PDP context index, range: 1-7
*	config				[in]	subnet config, When equal to NULL, the record will be removed from NV
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_subnet(uint8_t nSim, int profile_idx, ql_nat_subnet_config_s *config);

/*****************************************************************
* Function: ql_datacall_get_subnet
*
* Description:
* 	get the saved network address and subnet mask
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
* 	profile_idx	  		[in] 	PDP context index, range: 1-7
*	config				[out]	subnet config
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_subnet(uint8_t nSim, int profile_idx, ql_nat_subnet_config_s *config);

/*****************************************************************
* Function: ql_datacall_set_nat_assign
*
* Description:
* 	set the assign IP/GW/DNS address, and whether the port is fixed.
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
*   type                [in]    type: 0:usbnet 1:ppp
*   config				[in]
*                           mode: 0:disable 1:use the assigned IP address 2:use the assigned IP address and fixed port
*                           ip_addr/gw_addr/dns_addr: as 192.168.1.10
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_nat_assign(uint8_t nSim, ql_nat_assign_type_e type, ql_nat_assign_cfg_s *config);

/*****************************************************************
* Function: ql_datacall_get_nat_assign
*
* Description:
* 	get the assign IP/GW/DNS address, and whether the port is fixed.
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
*   type                [in]    type: 0:usbnet 1:ppp
*   config				[out]
*                           mode: 0:disable 1:use the assigned IP address 2:use the assigned IP address and fixed port
*                           ip_addr/gw_addr/dns_addr: assign IP/GW/DNS address
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_nat_assign(uint8_t nSim, ql_nat_assign_type_e type, ql_nat_assign_cfg_s *config);


/*****************************************************************
* Function: ql_datacall_set_dns_addr
*
* Description:
* 	set dns address
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
* 	profile_idx	  		[in] 	PDP context index, range: 1-7
*	new_pri				[in]	first dns address
*	new_sec				[in]	second dns address
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_dns_addr(uint8_t nSim, int profile_idx, ql_datacall_dns_info_s *new_pri, ql_datacall_dns_info_s *new_sec);

/*****************************************************************
* Function: ql_datacall_get_dns_addr
*
* Description:
* 	get dns address
* 
* Parameters: 
*   nSim				[in] 	SIM index，range: 0-1
* 	profile_idx	  		[in] 	PDP context index, range: 1-7
*	new_pri				[out]	first dns address
*	new_sec				[out]	second dns address
*
* Return:
* 	0			success
*	other		error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_dns_addr(uint8_t nSim, int profile_idx, ql_datacall_dns_info_s *new_pri, ql_datacall_dns_info_s *new_sec);

/*****************************************************************
* Function: ql_datacall_set_default_pdn_cfg
*
* Description:
*   set default pdn config
* 
* Parameters: 
*   nSim                [in]    SIM index，range: 0-1
*   ctx                 [in]    pdn config
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_default_pdn_cfg(uint8_t nSim, ql_data_call_default_pdn_cfg_s* ctx);

/*****************************************************************
* Function: ql_datacall_get_default_pdn_cfg
*
* Description:
*   get default pdn config
* 
* Parameters: 
*   nSim                [in]    SIM index，range: 0-1
*   ctx                 [out]   pdn config
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_default_pdn_cfg(uint8_t nSim, ql_data_call_default_pdn_cfg_s* ctx);

/*****************************************************************
* Function: ql_datacall_get_default_pdn_info
*
* Description:
*   get default pdn info
* 
* Parameters: 
*   nSim                [in]    SIM index，range: 0-1
*   ctx                 [out]   pdn info
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_default_pdn_info(uint8_t nSim, ql_data_call_default_pdn_info_s* ctx);

/*****************************************************************
* Function: ql_datacall_get_usbnet_customer_cfg
*
* Description:
*   get customer's setting usbnet info
* 
* Parameters: 
*   nSim                [in]    SIM index，range: 0-1
*   usbnet_cfg          [out]   usbnet info
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_usbnet_customer_cfg(ql_datacall_usbnet_customer_cfg_s *usbnet_cfg);

/*****************************************************************
* Function: ql_datacall_set_usbnet_customer_cfg
*
* Description:
*   set customer's setting usbnet info
* 
* Parameters: 
*   nSim                [in]    SIM index，range: 0-1
*   usbnet_cfg          [in]   usbnet info
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_usbnet_customer_cfg(ql_datacall_usbnet_customer_cfg_s *usbnet_cfg);

/*****************************************************************
* Function: ql_datacall_get_usbnet_internal_forward
*
* Description:
*   get usbnet data Internal forwarding config
* 
* Parameters: 
*   flag         [out]   usbnet internal forward config
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_get_usbnet_internal_forward(bool *flag);

/*****************************************************************
* Function: ql_datacall_set_usbnet_internal_forward
*
* Description:
*   set usbnet data Internal forwarding config
* 
* Parameters: 
*   flag          [in]   usbnet internal forward config
*
* Return:
*   0           success
*   other       error code
*
*****************************************************************/
ql_datacall_errcode_e ql_datacall_set_usbnet_internal_forward(bool flag);

#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*QL_API_DATACALL_H*/


