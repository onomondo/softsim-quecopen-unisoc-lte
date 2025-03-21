/**  @file
  ql_cnktd_nw.h

  @brief
  This file is used to define cnktd nw for different Quectel Project.

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


#ifndef QL_CNKTD_NW_H
#define QL_CNKTD_NW_H

#include "ql_cnktd.h"
#include "ql_api_datacall.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

#define QT_DS_PROFILE_MAX_APN_STRING_LEN  APN_LEN_MAX
#define QT_DS_PROFILE_MAX_USERNAME_LEN    USERNAME_LEN_MAX
#define QT_DS_PROFILE_MAX_PASSWORD_LEN    PASSWORD_LEN_MAX

#define QAPI_DSS_IP_VERSION_4      QT_NW_DS_PROFILE_PDP_IPV4   /**< IP version v4. */
#define QAPI_DSS_IP_VERSION_6      QT_NW_DS_PROFILE_PDP_IPV6   /**< IP version v6. */
#define QAPI_DSS_IP_VERSION_4_6    QT_NW_DS_PROFILE_PDP_IPV4V6   /**< IP version v4v6. */

typedef struct {
    uint8_t alloc_psm_enabled;
    uint32_t alloc_active_timer_value;
    uint32_t alloc_periodic_tau_timer_value;
}qapi_QT_NW_Alloc_PSM_Cfg_t;

typedef enum {
	QT_NW_EMTC = 0,
	QT_NW_NB_IOT = 1,
	QT_NW_GSM = 2,   /*GSM not support eDRX and PSM*/
}qapi_QT_NW_RAT_e;

typedef struct {
	uint8_t alloc_edrx_enable;
	uint8_t alloc_ptw_cycle;
	uint8_t alloc_edrx_cycle;
	uint8_t req_ptw_cycle;
	uint8_t req_edrx_cycle;
}qapi_QT_NW_Alloc_eDRX_Cfg_t;


typedef enum
{
	QT_NW_DS_PROFILE_PDP_IPV4 = 0,
	QT_NW_DS_PROFILE_PDP_IPV6 = 1,
	QT_NW_DS_PROFILE_PDP_IPV4V6 = 2,
	QT_NW_DS_PROFILE_PDP_MAX
}qapi_QT_NW_DS_PROFILE_PDP_TYPE_e;

typedef enum {
	QT_NW_DS_PROFILE_AUTH_NONE = 0,
	QT_NW_DS_PROFILE_AUTH_PAP = 1,
	QT_NW_DS_PROFILE_AUTH_CHAP = 2,
	QT_NW_DS_PROFILE_AUTH_PAP_CHAP = 3,
	QT_NW_DS_PROFILE_AUTH_TYPE_MAX
}qapi_QT_NW_DS_PROFILE_AUTH_TYPE_e;


typedef struct {
	qapi_QT_NW_DS_PROFILE_PDP_TYPE_e pdp_type;
	qapi_QT_NW_DS_PROFILE_AUTH_TYPE_e auth_type;
	uint8_t apn[QT_DS_PROFILE_MAX_APN_STRING_LEN + 1];
	uint8_t user_name[QT_DS_PROFILE_MAX_USERNAME_LEN + 1];
	uint8_t pass_word[QT_DS_PROFILE_MAX_PASSWORD_LEN + 1];
}qapi_QT_NW_DS_Profile_PDP_Context_t;

typedef struct
{
    uint8_t Profile_PDP_number;
    qapi_QT_NW_DS_Profile_PDP_Context_t Profile_PDP_Context;
}qapi_DSS_Hndl_s;

typedef enum {
	QT_NW_CFUN_MIN_FUNC = 0,  
	QT_NW_CFUN_FUNN_FUNC = 1,
	QT_NW_CFUN_SHUT_DOWN = 2,
	QT_NW_CFUN_RESET = 3,
	QT_NW_CFUN_FTM = 4,

	QT_NW_CFUN_MAX
} qapi_QT_NW_CFUN_MODE_e;

typedef enum {
	QT_NW_PREF_GSM = 0,
	QT_NW_PREF_CAT1 = 1,
	QT_NW_PREF_GSM_CAT1 = 2,
	QT_NW_PREF_RAT_MAX
}qapi_QT_NW_RAT_PREF_e;

typedef enum {
	QT_NW_PREF_SCAN_CATM_CATNB_GSM = 0,
	QT_NW_PREF_SCAN_CATM_GSM_CATNB = 1,
	QT_NW_PREF_SCAN_CATNB_CATM_GSM = 2,
	QT_NW_PREF_SCAN_CATNB_GSM_CATM = 3,
	QT_NW_PREF_SCAN_GSM_CATM_CATNB = 4,
	QT_NW_PREF_SCAN_GSM_CATNB_CATM = 5,
	QT_NW_PREF_RAT_SCAN_ORDER_MAX
}qapi_QT_NW_RAT_SCAN_ORDER_e;

typedef enum {
	QT_NW_PREF_CS_ONLY = 0,
	QT_NW_PREF_PS_ONLY = 1,
	QT_NW_PREF_CS_PS = 2,
	QT_NW_PREF_SRV_DOMAIN_MAX
} qapi_QT_NW_SRV_DOMAIN_PREF_e;

typedef struct {
	uint32_t earfcn;
	uint16_t mcc;
	uint16_t mnc;
	uint16_t tac;
	uint32_t cell_id;
	uint8_t freq_band;
	uint16_t pci;
	int16 rsrp;
	int16 rsrq;
	int16 rssi;
	int16 sinr;
}qapi_QT_NW_LTE_Meas_Info_t;

/** Maximum size of #qapi_Device_Info_t valuebuf. */
#define QAPI_DEVICE_INFO_BUF_SIZE 128
#define MAX_LEN_VAL 5

/** Device information types. */
typedef enum {

/** 
  Response type: Return value of the Device ID as specified in the enum qapi_Device_Info_Type_t.
  Operation: Query      -  Queries the device information using the API qapi_Device_Info_Get_v2().
             Set        -  Sets the device information using the API qapi_Device_Info_Set().
             Indication -  Sets the device information callback, registering for various indications 
                           using the API qapi_Device_Info_Set_Callback_v2().
 */

 QAPI_DEVICE_INFO_BUILD_ID_E,
 /**< Device BUILD_ID. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_IMEI_E,
 /**< Device IMEI. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_IMSI_E,
 /**< UIM IMSI. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_OS_VERSION_E,
 /**< Device OS version. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MANUFACTURER_E,
 /**< Device manufacturer. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MODEL_ID_E,
 /**< Device model ID. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_BATTERY_STATUS_E,
 /**< Device battery status. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_BATTERY_PERCENTAGE_E,
 /**< Device battery percentage. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEIVCE_INFO_TIME_ZONE_E,
 /**< Device time zone. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEIVCE_INFO_ICCID_E,
 /**< Device ICCID. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_4G_SIG_STRENGTH_E,
 /**< Network signal strength. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_BASE_STATION_ID_E,
 /**< Network base station ID. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MCC_E,
 /**< Network MCC. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_MNC_E,
 /**< Network MNC. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_SERVICE_STATE_E,
 /**< Network service status. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MDN_E,
 /**< Device MDN. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_TAC_E,
 /**< Network tracking area code. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_CELL_ID_E,
 /**< Network cell ID. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_RCCS_E,
 /**< Network RRC state. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_EMMS_E,
 /**< Network EMM state. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 DEPRACATED1,
 /* Information to keep enum numbering consistent. */

 QAPI_DEVICE_INFO_SERVING_PCI_E,
 /**< Network serving cell PCI. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SERVING_RSRQ_E,
 /**< Serving cell RSRQ. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SERVING_EARFCN_E,
 /**< Serving cell EARFCN. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 DEPRACATED2,
 /* Information to keep enum numbering consistent. */
 DEPRACATED3,
 /* Information to keep enum numbering consistent. */
 DEPRACATED4,
 /* Information to keep enum numbering consistent. */
 DEPRACATED5,
 /* Information to keep enum numbering consistent. */
 DEPRACATED6,
 /* Information to keep enum numbering consistent. */

 QAPI_DEVICE_INFO_NETWORK_IND_E,
 /**< Network indication. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_ROAMING_E,
 /**< Roaming status. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_LAST_POWER_ON_E,
 /**< Last power on time. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_CHIPID_STRING_E,
 /**< Chipset name. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_APN_PROFILE_INDEX_E,
 /**< APN profile index. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SIM_STATE_E,
 /**< SIM state. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_NETWORK_BEARER_E,
 /**< Network bearer. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_LINK_QUALITY_E,
 /**< Network link quality. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_TX_BYTES_E,
 /**< Device Tx bytes. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_RX_BYTES_E,
 /**< Device Rx bytes. Response type:QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_ANY,
 /**< Any device information. */

 QAPI_DEVICE_INFO_CIOT_LTE_OP_MODE_PREF_E,
 /**< Device lte op mode preference. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_M1_BAND_PREF_E,
 /**< Device lte m1 band preference. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_NB1_BAND_PREF_E,
 /**< Device lte nb1 band preference. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_MODE_PREF_E,
 /**< Device mode preference Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query.. */

 QAPI_DEVICE_INFO_RAT_ACQ_PREF_E,
 /**< RAT Acquisition order preference. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_PS_DETACH_E,
 /**< Device PS Detach. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set. */

 QAPI_DEVICE_INFO_PS_ATTACH_E,
 /**< Device PS Attach. Response type:QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set. */

 QAPI_DEVICE_INFO_PSM_TIMER_E,
 /**< Device PSM timer. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_ACTIVE_TIMER_E,
 /**< Device Power save mode Active timer value. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_OP_MODE_E,
/**< Device LTE operational mode. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_LAC_E,
/**< Location Area Code. Response type:QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

} qapi_Device_Info_ID_t;

/** Device information response types. */
typedef enum {
 QAPI_DEVICE_INFO_TYPE_BOOLEAN_E,
 /**< Response type is Boolean. */
 QAPI_DEVICE_INFO_TYPE_INTEGER_E,
 /**< Response type is integer. */
 QAPI_DEVICE_INFO_TYPE_BUFFER_E,
 /**< Response type is buffer. */
 QAPI_DEVICE_INFO_TYPE_ARRAY_E,
 /**< Response type is array. */
} qapi_Device_Info_Type_t;

/**
 QAPI device information structure.
*/
typedef struct {
  qapi_Device_Info_ID_t id; /**< Required information ID. */
  qapi_Device_Info_Type_t info_type; /**< Response type. */
  union {
    struct {
      char buf[QAPI_DEVICE_INFO_BUF_SIZE]; /**< Response buffer. */
      uint32_t len; /**< Length of the response string. */
    }valuebuf;    /**< Union of buffer values. */
    int64_t valueint; /**< Response integer value. */
    bool valuebool; /**< Response Boolean value.*/
    struct
    {
      int buf[MAX_LEN_VAL]; /**< Response type: integer array. */
      uint32_t len; /**< Length of the array. */
    }valint;
  }u;   /**< Union of values. */
}qapi_Device_Info_t;

/** Structure to represent the IP address. */
typedef struct qapi_DSS_Addr_s
{
  char valid_addr; /**< Indicates whether a valid address is available. */   
  union qapi_dss_ip_address_u /** Union of DSS IP addresses. */
  {
    uint32_t v4;            /**< Used to access the IPv4 address. */
    uint64_t v6_addr64[2];  /**< Used to access the IPv6 address. */
    uint32_t v6_addr32[4];  /**< Used to access the IPv6 address as four 32-bit integers. */
    uint16_t v6_addr16[8];  /**< Used to access octets of the IPv6 address. */
    uint8_t  v6_addr8[16];  /**< Used to access octets of the IPv6 address as 16 8-bit integers. */
  } addr;                   /**< Union of DSS IP addresses. */
} /** @cond */qapi_DSS_Addr_t/** @endcond */;

/** IP address-related information. */
typedef struct qapi_DSS_Addr_Info_s
{
  qapi_DSS_Addr_t   iface_addr_s; /**< Network interface address. */
  unsigned int      iface_mask;   /**< Interface subnet mask. */
  qapi_DSS_Addr_t   gtwy_addr_s;  /**< Gateway server address. */
  unsigned int      gtwy_mask;    /**< Gateway subnet mask. */
  qapi_DSS_Addr_t   dnsp_addr_s;  /**< Primary DNS server address. */  
  qapi_DSS_Addr_t   dnss_addr_s;  /**< Secondary DNS server address. */
} /** @cond */qapi_DSS_Addr_Info_t/** @endcond */;

typedef struct {
	uint8_t req_psm_enable;
	uint32_t req_active_timer_value;
	uint32_t req_periodic_tau_timer_value;
}qapi_QT_NW_Req_PSM_Cfg_t;

typedef struct {
	uint8_t req_edrx_enable;
	qapi_QT_NW_RAT_e rat_mode;
	uint8_t req_ptw_cycle;
	uint8_t req_edrx_cycle;
}qapi_QT_NW_Req_eDRX_Cfg_t;

typedef struct {
	uint8_t gsm_band;
	uint64_t cat1_band_low;
	uint64_t nb_band_low;
    uint64_t cat1_band_high;
}qapi_QT_NW_Band_Params_t;

/**
   QAPI DSS event names.
   Event names are sent along with the registered user callback. 
*/
typedef enum qapi_DSS_Net_Evt_e
{
  QAPI_DSS_EVT_INVALID_E = 0x00,   /**< Invalid event. */
  QAPI_DSS_EVT_NET_IS_CONN_E,      /**< Call connected. */
  QAPI_DSS_EVT_NET_NO_NET_E,       /**< Call disconnected. */
  QAPI_DSS_EVT_NET_RECONFIGURED_E, /**< Call reconfigured. */
  QAPI_DSS_EVT_NET_NEWADDR_E,      /**< New address generated. */
  QAPI_DSS_EVT_NET_DELADDR_E,      /**< Delete generated. */
  QAPI_DSS_EVT_NIPD_DL_DATA_E,     /**< Non-IP downlink data. */
  QAPI_DSS_EVT_MAX_E
} /** @cond */qapi_DSS_Net_Evt_t/** @endcond */;

/** Event payload sent with event callbacks. */
typedef struct qapi_DSS_Evt_Payload_s
{
  uint8_t *data;	  /**< Payload data. */
  uint32_t data_len;  /**< Payload data length. */
} /** @cond */qapi_DSS_Evt_Payload_t/** @endcond */;


/** Callback function prototype for DSS events.

   @param[in] hndl Handle to which this event is associated.
   @param[in] user_data Application-provided user data.
   @param[in] evt Event identifier.
   @param[in] payload_ptr Pointer to associated event information.

   @return
   None.
   */

typedef void (*qapi_DSS_Net_Ev_CB_t)
(
  qapi_DSS_Hndl_t         hndl,          /**< Handle for which this event is associated. */
  void                   *user_data,     /** Application-provided user data. */
  qapi_DSS_Net_Evt_t      evt,           /** Event identifier. */
  qapi_DSS_Evt_Payload_t *payload_ptr    /** Associated event information. */
); 

/** Call parameter identifier.  */
typedef enum qapi_DSS_Call_Info_Enum_e
{
  QAPI_DSS_CALL_INFO_MIN_E = 0x00,
  QAPI_DSS_CALL_INFO_UMTS_PROFILE_IDX_E,   /**< UMTS profile ID. */
  QAPI_DSS_CALL_INFO_APN_NAME_E,           /**< APN name. */
  QAPI_DSS_CALL_INFO_USERNAME_E,           /**< APN user name. */
  QAPI_DSS_CALL_INFO_PASSWORD_E,           /**< APN password. */
  QAPI_DSS_CALL_INFO_AUTH_PREF_E,          /**< Authentication preference. */
  QAPI_DSS_CALL_INFO_CDMA_PROFILE_IDX_E,   /**< CDMA profile ID. */
  QAPI_DSS_CALL_INFO_TECH_PREF_E,          /**< Technology preference. */
  QAPI_DSS_CALL_INFO_IP_VERSION_E,         /**< Preferred IP family for the call. */
  QAPI_DSS_CALL_INFO_EXT_TECH_E,           /**< Extended technology preference. */
  QAPI_DSS_CALL_INFO_MO_EXCEPTION_DATA_E,  /**< MO exception data. */
  QAPI_DSS_CALL_INFO_LTE_PROFILE_IDX_E,    /**< LTE profile ID. */
  QAPI_DSS_CALL_INFO_MAX_E
} /** @cond */qapi_DSS_Call_Param_Identifier_t/** @endcond */;

/** Specifies call parameter values. */
typedef struct qapi_DSS_Call_Param_Value_s
{
  char *buf_val;  /**<  Pointer to the buffer containing the parameter value that is to be set. */
  int   num_val;  /**<  Size of the parameter buffer. */
} /** @cond */qapi_DSS_Call_Param_Value_t/** @endcond */;


/*========================================================================
 *  function Definition
 *========================================================================*/
 /**
* Initializes the device information context.
*
* @param[in] device_info_hndl Pointer to device info hndl
*
*
* @return QAPI_OK on success, QAPI_ERROR on failure.
*/
qapi_Status_t qapi_Device_Info_Init_v2(qapi_Device_Info_Hndl_t *device_info_hndl);

/*
@func
  qapi_QT_NW_PSM_Cfg_Set
@brief
  This function is used to set configuration of PSM of UE.
@param [in]
  psm_cfg - Pointer. Used to configure the parameter of PSM.
*/
qapi_Status_t qapi_QT_NW_PSM_Cfg_Set(qapi_QT_NW_Req_PSM_Cfg_t *psm_cfg);


/*
@func
  qapi_QT_NW_PSM_Cfg_Get
@brief
  This function is used to get network allocated parameters of PSM. 
@param [out]
  psm_cfg - Pointer. Used to store the parameters of PSM that allocated by network.
*/
qapi_Status_t qapi_QT_NW_PSM_Cfg_Get(qapi_QT_NW_Alloc_PSM_Cfg_t* psm_cfg);


/*
@func
  qapi_QT_NW_eDRX_Cfg_Set
@brief
  This function is used to set configuration of eDRX of UE. 
@param [in]
  edrx_cfg - Pointer. Used to configure eDRX of UE.
*/
qapi_Status_t qapi_QT_NW_eDRX_Cfg_Set(qapi_QT_NW_Req_eDRX_Cfg_t *edrx_cfg);


/*
@func
  qapi_QT_NW_eDRX_Cfg_Get
@brief
  This function is used to get network allocated parameters of eDRX. 
@param [in]
  rat_mode - Pointer. RAT that needs the eDRX parameters.
@param [out]
  edrx_cfg - Pointer. Store that network allocated parameters of eDRX.
*/
qapi_Status_t qapi_QT_NW_eDRX_Cfg_Get(qapi_QT_NW_RAT_e *rat_mode, qapi_QT_NW_Alloc_eDRX_Cfg_t* edrx_cfg);


/*
@func
  qapi_QT_Phone_Func_Set
@brief
  This function is used to set phone functionality
@param [in]
  fun - Used to set the module functionality. Please refer to the enumeration qapi_QT_NW_CFUN_MODE_e
*/
qapi_Status_t qapi_QT_Phone_Func_Set(qapi_QT_NW_CFUN_MODE_e* fun);


/*
@func
  qapi_QT_Phone_Func_Get
@brief
  This function is used to get current functionality configuration
@param [in]
  fun - Used to store the current functionality configuration
*/
qapi_Status_t qapi_QT_Phone_Func_Get(qapi_QT_NW_CFUN_MODE_e *fun);


/*
@func
  qapi_QT_NW_Rat_Pref_Set
@brief
  This function is used to set preferred RAT (Radio Access Technology).
@param [in]
  mode - Used to configure preferred RAT.
*/
qapi_Status_t qapi_QT_NW_Rat_Pref_Set(qapi_QT_NW_RAT_PREF_e *mode);


/*
@func
  qapi_QT_NW_Rat_Pref_Get
@brief
  This function is used to get preferred RAT (Radio Access Technology).
@param [out]
  mode - Used to store the current configuration of preferred RAT.
*/
qapi_Status_t qapi_QT_NW_Rat_Pref_Get(qapi_QT_NW_RAT_PREF_e *mode);


/*
@func
  qapi_QT_NW_Rat_Scan_Pre_Set
@brief
  This function is used to set configuration of preference of RAT scan.
@param [in]
  order - Used to configure the RAT scan preference.
*/
qapi_Status_t qapi_QT_NW_Rat_Scan_Pre_Set(qapi_QT_NW_RAT_SCAN_ORDER_e *order);


/*
@func
  qapi_QT_NW_Rat_Scan_Pre_Get
@brief
  This function is used to get configuration of preference of RAT scan.
@param [in]
  order - Used to store the current configuration of preference of RAT scan.
*/
qapi_Status_t qapi_QT_NW_Rat_Scan_Pre_Get(qapi_QT_NW_RAT_SCAN_ORDER_e *order);


/*
@func
  qapi_QT_NW_Srv_Domain_Pref_Set
@brief
  This function is used to set configuration of preferred service domain
@param [in]
  domain - Pointer. Used to configure the preferred service domain.
*/
qapi_Status_t qapi_QT_NW_Srv_Domain_Pref_Set(qapi_QT_NW_SRV_DOMAIN_PREF_e *domain);


/*
@func
  qapi_QT_NW_Srv_Domain_Pref_Get
@brief
  This function is used to get configuration of preferred service domain.
@param [out]
  domain - Used to store the current configuration of preferred service domain.
*/
qapi_Status_t qapi_QT_NW_Srv_Domain_Pref_Get(qapi_QT_NW_SRV_DOMAIN_PREF_e *domain);

/*
@func
  qapi_QT_NW_LTE_Meas_Info_Get
@brief
  This function is used to get information of measurement under LTE. 
@param [out]
  meas_info - Pointer. Used to store information of measurement under LTE.
*/
qapi_Status_t qapi_QT_NW_LTE_Meas_Info_Get(qapi_QT_NW_LTE_Meas_Info_t* meas_info);

/**
 * Gets the device information for specified ID.
 *
 * @param[in] device_info_hndl  Device info hndl.
 * @param[in] id                Information ID.
 * @param[out] info             Information received for the specified ID.
 *
 * @return QAPI_OK on success, QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this API, qapi_Device_Info_Init_v2() must have been called.
 */
qapi_Status_t qapi_Device_Info_Get_v2(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id, qapi_Device_Info_t *info);

/*
@func
  qapi_QT_NW_Band_Pref_Set
@brief
  This function is used to set preferred band
@param [in]
  band_pref - Used to set the preferred band.
*/
qapi_Status_t qapi_QT_NW_Band_Pref_Set(qapi_QT_NW_Band_Params_t *band_pref);

/*
@func
  qapi_QT_NW_Band_Pref_Get
@brief
  This function is used to get preferred band
@param [out]
  band_pref - Used to store the preferred band.
*/
qapi_Status_t qapi_QT_NW_Band_Pref_Get(qapi_QT_NW_Band_Params_t *band_pref);


/*
@func
  qapi_QT_NW_Extend_Band_Pref_Set
@brief
  This function is used to get band of configuration (B65 ~ B128).
@param [in]
  cat1_band - Used to store the CAT1 preferred band (B65 ~ B128)..
  nb_band - Used to store the NB preferred band (B65 ~ B128).
*/
qapi_Status_t qapi_QT_NW_Extend_Band_Pref_Set(uint64 *cat1_band, uint64 *nb_band);


/*
@func
  qapi_QT_NW_Extend_Band_Pref_Get
@brief
  This function is used to config preferred band (B65 ~ B128)
@param [out]
  cat1_band - Used to store the CAT1 preferred band (B65 ~ B128)..
  nb_band - Used to store the NB preferred band (B65 ~ B128).
*/
qapi_Status_t qapi_QT_NW_Extend_Band_Pref_Get(uint64 *cat1_band, uint64 *nb_band);


/*
@func
  qapi_QT_MP_FW_Ver_Get
@brief
  Get module MP firmware version number.
@param [in]
  version - pointer, to store the MP firmware number.It supports up to 64 bytes.
  len - pointer, the length of version string.
*/
qapi_Status_t qapi_QT_MP_FW_Ver_Get(char* version, uint16* len);


/*===========================================================================
  FUNCTION:  qapi_DSS_Get_IP_Addr_Count
===========================================================================*/
/** @ingroup qapi_DSS_Get_IP_Addr_Count

    Queries the number of IP addresses (IPv4 and global IPv6) associated with
    the DSs interface.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().
    @param[in] ip_addr_cnt Pointer to where to retrieve the number of IP addresses associated with
               the DSS interface.

    @return
    QAPI_OK -- IP address count query was successful. \n
    QAPI_ERROR -- IP address count query was unsuccessful.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl().
*/
/*=========================================================================*/
qapi_Status_t qapi_DSS_Get_IP_Addr_Count(qapi_DSS_Hndl_t hndl, unsigned int *ip_addr_cnt);

/** @ingroup qapi_DSS_Get_IP_Addr

    Queries the IP address information structure (network order).

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().
    @param[out] info_ptr Buffer containing the IP address information.
    @param[in] len Number of IP address buffers

    @return
    QAPI_OK -- IP address query was successful. \n
    QAPI_ERROR -- IP address query was unsuccessful.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl(). \n
    The length parameter can be obtained by calling qapi_DSS_Get_IP_Addr_Count(). \n
    It is assumed that the client has allocated memory for enough structures
    specified by the len field.
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Get_IP_Addr(qapi_DSS_Hndl_t hndl, qapi_DSS_Addr_Info_t  *info_ptr, int len);


/*
@func
  qapi_QT_NW_PDP_Cfg_Set
@brief
  This function is used to configure the specific PDP context. 
@param [in]
  pdp_context_number - Pointer. Indicate specific PDP context that needs to be configured.
  profile - Pointer. Used to configure the specific PDP context.
*/
qapi_Status_t qapi_QT_NW_PDP_Cfg_Set(uint8 *pdp_context_number, qapi_QT_NW_DS_Profile_PDP_Context_t *profile);

/*
@func
  qapi_QT_NW_PDP_Cfg_Get
@brief
  This function is used to get configuration of specific PDP context. 
@param [in]
  pdp_context_number - Pointer. Indicate the specific PDP context whose configuration is needed.
@param [out]
  profile - Pointer. Used to store the configuration of the specific PDP context.
*/
qapi_Status_t qapi_QT_NW_PDP_Cfg_Get(uint8 *pdp_context_number, qapi_QT_NW_DS_Profile_PDP_Context_t* profile);


/** @ingroup qapi_DSS_Init

    Initializes the DSS netctrl library for the specified operating mode.
    This function must be invoked once per process, typically on process startup.

    @note1hang
    Only QAPI_DSS_MODE_GENERAL is to be used by applications.

    @param[in] mode Mode of operation in which to initialize the library.

    @return
    QAPI_OK -- Initialization was successful. \n
    QAPI_ERROR -- Initialization failed.

    @dependencies
    None.
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Init(int mode);

/** @ingroup qapi_DSS_Get_Data_Srvc_Hndl

    Gets an opaque data service handle. All subsequent functions use this
    handle as an input parameter.

    @note1hang
    DSS netctrl library waits for initialization from the lower layers
    (QMI ports being opened, the RmNet interfaces being available, etc.) to
    support data services functionality. During initial bootup scenarios,
    these dependencies may not be available, which will cause an error to
    be returned by dss_get_data_srvc_hndl. In such cases, clients are
    asked to retry this function call repeatedly using a 500 ms timeout
    interval. Once a non-NULL handle is returned, clients can exit out of
    the delayed retry loop.

    @param[in] user_cb_fn Client callback function used to post event indications.
    @param[in] user_data Pointer to the client context block (cookie). The
                         value may be NULL.
    @param[in] hndl Pointer to data service handle.
    
    @return    
    QAPI_OK -- Operation was successful. \n
    QAPI_ERROR -- Operation failed.


    @dependencies
    qapi_DSS_Init() must have been called first.
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Get_Data_Srvc_Hndl(qapi_DSS_Net_Ev_CB_t user_cb_fn, void *user_data, qapi_DSS_Hndl_t *hndl);

/** @ingroup qapi_DSS_Set_Data_Call_Param

    Sets the data call parameter before trying to start a data call. Clients 
    may call this function multiple times with various types of parameters 
    that need to be set.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().
    @param[in] identifier Identifies the parameter information.
    @param[in] info Parameter value that is to be set.

    @return
    QAPI_OK -- Data call parameter was set successfully. \n
    QAPI_ERROR -- Data call parameter was not set successfully.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl(). 
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Set_Data_Call_Param(qapi_DSS_Hndl_t hndl, qapi_DSS_Call_Param_Identifier_t identifier, qapi_DSS_Call_Param_Value_t *info);

/*===========================================================================
  FUNCTION:  qapi_DSS_Start_Data_Call
===========================================================================*/
/** @ingroup qapi_DSS_Start_Data_Call

    Starts a data call.

    An immediate call return value indicates whether the request was sent
    successfully. The client receives asynchronous notfications via a callback
    registered with qapi_DSS_Get_Data_Srvc_Hndl() indicating the data call bring-up
    status.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().

    @return
    QAPI_OK -- Data call start request was sent successfully. \n
    QAPI_ERROR -- Data call start request was unsuccessful.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl(). 
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Start_Data_Call(qapi_DSS_Hndl_t hndl);

/*===========================================================================
  FUNCTION:  qapi_DSS_Stop_Data_Call
===========================================================================*/
/** @ingroup qapi_DSS_Stop_Data_Call

    Stops a data call.

    An immediate call return value indicates whether the request was sent
    successfully. The client receives asynchronous notification via a callback
    registered with qapi_DSS_Get_Data_Srvc_Hndl() indicating the data call tear-down
    status.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().

    @return
    QAPI_OK -- Data call stop request was sent successfully.	\n
    QAPI_ERROR -- Data call stop request was unsuccessful.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl(). \n
    The data call must have been brought up using qapi_DSS_Start_Data_Call().
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Stop_Data_Call(qapi_DSS_Hndl_t hndl);

/*===========================================================================
  FUNCTION:  qapi_DSS_Rel_Data_Srvc_Hndl
===========================================================================*/
/** @ingroup qapi_DSS_Rel_Data_Srvc_Hndl

    Releases a data service handle. All resources associated with the handle
    in the library are released.

    @note1hang
    If the user starts an interface with this handle, the corresponding 
    interface is stopped before the DSS handle is released.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().

    @return    
    QAPI_OK -- Operation was successful. \n
    QAPI_ERROR -- Operation failed.


    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl().
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Rel_Data_Srvc_Hndl(qapi_DSS_Hndl_t hndl);


/*===========================================================================
  FUNCTION:  qapi_DSS_Release
===========================================================================*/
/** @ingroup qapi_DSS_Release
 
    Cleans up the DSS netctrl library. This function must be invoked once per 
    process, typically at the end to clean up the resources.
    
    @note1hang
    Only QAPI_DSS_MODE_GENERAL is to be used by applications.

    @param[in] mode Mode of operation in which to de-initialize the library.

    @return    
    QAPI_OK -- Cleanup was successful. \n
    QAPI_ERROR -- Cleanup failed.

    @dependencies
    None.
*/
/*=========================================================================*/

qapi_Status_t qapi_DSS_Release(int mode);

/*
@func
  qapi_QT_IMEI_Get
@brief
  Get module IMEI number.
@param [in]
  imei - pointer, to store the IMEI number.
  len - pointer, the length of imei string.
*/
qapi_Status_t qapi_QT_IMEI_Get(char* imei, uint16* len);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_NW_H */


