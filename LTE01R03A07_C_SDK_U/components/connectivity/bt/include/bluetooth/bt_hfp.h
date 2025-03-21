/**
 * @file    bt_hfp.h
 * @brief    -
 * @details
 * @mainpage
 * @version  V1.0.0
 * @date     2019-10-11
 * @license  Copyright (C) 2019 Unisoc Communications Inc
 */


#ifndef __BT_HFP_H__
#define __BT_HFP_H__

/**
 * @addtogroup bluestack
 * @{
 * @defgroup hfp
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "bluetooth/bt_types.h"
#include "bluetooth/bluetooth.h"

#define HFP_VOLUME_MAX_VALUE    15

/** hfp status*/
typedef enum
{
    HF_STATUS_NONE=0,
    HF_STATUS_INCOMING_CALL_IN_PROGRESS,
    HF_STATUS_OUTGOING_CALL_IN_PROGRESS,
    HF_STATUS_LOCAL_RING,
    HF_STATUS_REMOTE_RING,
}bthfp_status_t;

/** hfp call command*/
typedef enum
{
    HF_CMD_ANSWER=0,
    HF_CMD_REJECT,
    HF_CMD_RELEASE,
    HF_CMD_MUTI_CALL,
    AG_CMD_RING,
}hfp_call_cmd_t;

/* second argument of hf_call_request */
typedef enum
{
    /* use by hf_call_request */
    HF_CALL_INCOMMING,
    HF_CALL_OUTGOING,
    HF_CALL_START,
    HF_CALL_ALERT,
    HF_CALL_RELEASE,
    HF_CALL_NUM,
    HF_CALL_MEM,
    HF_CALL_REDIAL,
    HF_CALL_SWAP,
    HF_CALL_HOLD,

    /* use by hf_call_answer */
    HF_CALL_ACCEPT,
    HF_CALL_REJECT,

    /* use by hf_call_hold */
    HF_CALL_HELD,
    HF_CALL_WAIT,
    HF_CALL_ACTIVE,
    /* use by hf_call_status */
    HF_SERVICE_INDICATOR,
    HF_CALL_INDICATOR,
    HF_CALLSETUP_INDICATOR,
    HF_CALLHOLD_INDICATOR,
    HF_BATTCHG_INDICATOR,
    HF_SIGNAL_INDICATOR,
    HF_ROAM_INDICATOR,

    /* use by hf_voice_recognition */
    HF_VR_START,
    HF_VR_STOP
}bt_call_type_t;

/**Hfp connection state enumeration*/
typedef enum
{
    BTHFP_CONNECTION_STATE_DISCONNECTED = 0,   /**< Hfp connection is disconnected */
    BTHFP_CONNECTION_STATE_CONNECTING,            /**< Hfp connection is connecting */
    BTHFP_CONNECTION_STATE_CONNECTED,              /**< Hfp connection is connected */
    BTHFP_CONNECTION_STATE_DISCONNECTING,       /**< Hfp connection is disconnecting */
} bthfp_connection_state_t;

/**Hfp audio state enumeration*/
typedef enum
{
    BTHFP_AUDIO_STATE_DISCONNECTED = 0,              /**< Audio is disconnected */
    BTHFP_AUDIO_STATE_CONNECTING,                       /**< Audio is connecting */
    BTHFP_AUDIO_STATE_CONNECTED,                         /**< Audio is connected */
    BTHFP_AUDIO_STATE_DISCONNECTING                   /**< Audio is disconnecting */
} bthfp_audio_state_t;

/**Hfp volume type enumeration*/
typedef enum
{
    BTHFP_VOLUME_TYPE_SPK = 0,                               /**< Volume type is speaker */
    BTHFP_VOLUME_TYPE_MIC                                      /**< Volume type is microphone */
} bthfp_volume_type_t;

/**Hfp codec type enumeration*/
typedef enum
{
    BTHFP_CODEC_TYPE_CVSD = 1,                           /**< Codec type is CVSD */
    BTHFP_CODEC_TYPE_MSBC                                 /**< Codec type is mSBC */
} bthfp_codec_type_t;

/**Hfp service type enumeration*/
typedef enum
{
    BTHFP_SERVICE_TYPE_HOME = 0,                    /**< Service type is home */
    BTHFP_SERVICE_TYPE_ROAMING,                    /**< Service type is roaming */
} bthfp_service_type_t;

/**Hfp network state enumeration*/
typedef enum
{
    BTHFP_NETWORK_STATE_NOT_AVAILABLE = 0,/**< Network is not available */
    BTHFP_NETWORK_STATE_AVAILABLE,              /**< Network is available */
} bthfp_network_state_t;

/**Hfp call state enumeration*/
typedef enum
{
    BTHFP_CALL_NO_CALL_IN_PROGRESS = 0,/**<  No calls is in progress */
    BTHFP_CALL_CALL_IN_PROGRESS,            /**<  At least one call is in progress */
} bthfp_call_state_t;

/**Hfp callsetup state enumeration*/
typedef enum
{
    BTHFP_CALLSETUP_NONE = 0,      /**<  Not currently in call set up */
    BTHFP_CALLSETUP_INCOMING,    /**<  An incoming call process ongoing */
    BTHFP_CALLSETUP_OUTGOING,   /**<  An outcoming call set up is ongoing */
    BTHFP_CALLSETUP_ALERTING     /**<  Remote party being alerted in an outgoing call */
} bthfp_callsetup_state_t;

/**Hfp callheld state enumeration*/
typedef enum
{
    BTHFP_CALLHELD_NONE = 0,                /**<  No call held */
    BTHFP_CALLHELD_HOLD_AND_ACTIVE, /**<  Call is placed on held or active/held calls swapped */
    BTHFP_CALLHELD_HOLD,                      /**<  Call on hold,no active call */
} bthfp_callheld_state_t;

/**Hfp ciev type enumeration*/
/**CIND is define with internal as below:*/
/** "\r\n+CIND:(\"service\",(0,1)),(\"call\",(0,1)),(\"callsetup\",(0,3)),(\"callheld\",(0,2)),(\"battchg\",(0,5)),(\"signal\",(0,5)),(\"roam\",(0,1))\r\n";*/
typedef enum
{
    SERVICE_IND = 15, /**<  indicator of network service */
    CALL_IND,        /**<  indicator of call */
    CALLSETUP_IND,   /**<  indicator of callsetup */
    CALLHELD_IND,    /**<  indicator of call hold */
    BATTCHG_IND,     /**<  indicator of battery information */
    SIGNAL_IND,      /**<  indicator of signal with modem */
    ROAM_IND         /**<  indicator of roam with network service */
}hfp_cind_t;


/**Hfp device information structure*/
typedef struct
{
    bdaddr_t hfp_addr;              /**<  Hfp address */
    UINT32 hfp_call_status;       /**<  Hfp call status */
    UINT8 hfp_adp_state;         /**<  Hfp adp state */
    UINT32 hfp_check_timer;    /**<  Hfp check timer */
    UINT8 check_cnt;                /**<  Hfp check counter */
    UINT8 callhold_state;           /**<  Hfp callheld state */
    UINT32 supported_features;/**<  Hfp supported feature  */
    UINT8 bvra_flag;                 /**<  Hfp bluetooth voice recognition activate flag*/
    bt_conn_t hfp_conn;          /**<  Hfp connection information*/
} hfp_device_info_t;


/**Hfp callback function structure*/
typedef struct
{
    void (*connection_state_callback) (bthfp_connection_state_t state, bdaddr_t *addr);          /**<  Hfp connection state callback function*/
    void (*audio_state_callback) (bthfp_audio_state_t state, bdaddr_t *addr);                         /**<  Hfp audio state callback function*/
    void (*volume_change_callback) (bthfp_volume_type_t type, int volume, bdaddr_t *addr);   /**<  Hfp volume change callback function*/
    void (*call_callback) (bthfp_call_state_t state, bdaddr_t *addr);                                         /**<  Hfp call callback function*/
    void (*callsetup_callback) (bthfp_callsetup_state_t state, bdaddr_t *addr);                         /**<  Hfp call setup callback function*/
    void (*callheld_callback) (bthfp_callheld_state_t state, bdaddr_t *addr);                             /**<  Hfp call held callback function*/
    void (*battery_level_callback) (int level, bdaddr_t *addr);                                                 /**<  Hfp battery level callback function*/
    void (*network_signal_callback) (int signal_strength, bdaddr_t *addr);                               /**<  Hfp network signal callback function*/
    void (*network_state_callback) (bthfp_network_state_t state, bdaddr_t *addr);                  /**<  Hfp network state callback function*/
    void (*network_type_callback) (bthfp_service_type_t type, bdaddr_t *addr);                       /**<  Hfp network type callback function*/
    void (*ring_indication_callback) (bdaddr_t *addr);                                                            /**<  Hfp ring indication callback function*/
    void (*cmd_response_callback) (char *response, UINT32 length, bdaddr_t *addr);                /**<  Hfp command response callback function*/
    void (*codec_type_callback) (bthfp_codec_type_t type, bdaddr_t *addr);                             /**<  Hfp codec type callback function*/
    bt_status_t (*sco_data_callback) (hci_data_t *p_buf, UINT32 length);                                /**<  Hfp sco data callback function*/
    void (*atcmd_response_callback)(char *response, const char *at_hfp_cmd);
} bthfp_callbacks_t;

/**Hfp interface function structure*/
typedef struct
{
    bt_status_t (*init) (bthfp_callbacks_t *callbacks);                                           /**<  Hfp initialization  function*/
    UINT32 (*get_call_status)(void);                                                                 /**<  Hfp get call status function*/
    void (*connect)(bdaddr_t *pAddr);                                                               /**<  Hfp connect function*/
    void (*disconnect)(bdaddr_t *pAddr);                                                           /**<  Hfp disconnect function*/
    void (*call_accept)(bdaddr_t *pAddr, UINT8 call_status);                                /**<  Hfp call accept function*/
    void (*call_reject)(bdaddr_t *pAddr);                                                            /**<  Hfp cal reject function*/
    void (*three_way_calling)(bdaddr_t *pAddr, UINT8 call_type, UINT8 param);    /**<  Hfp three way calling function*/
    void (*call_req)(bdaddr_t *pAddr, UINT8 call_type, UINT8 *phone_number);      /**<  Hfp call request function*/
    void (*set_volume)(bdaddr_t addr, UINT8 vol);                                              /**<  Hfp set volume function*/
    void (*set_mic_volume)(bdaddr_t addr, UINT8 vol);                                       /**<  Hfp set microphone volume function*/
    void (*set_voice_recognition)(bdaddr_t *addr, UINT8 status);                                             /**<  Hfp voice recognition function*/
    void (*send_battery)(UINT8 battery);                                                           /**<  Hfp send battery level function*/
    void (*audio_connect)(bdaddr_t addr);                                                          /**<  Hfp audio connect function*/
    void (*audio_connect_setup)(bdaddr_t addr);                                                /**<  Hfp audio connect setup function*/
    void (*audio_disconnect)(void);                                                                    /**<  Hfp audio disconnect function*/
    void (* disable_nrec)(bdaddr_t *pAddr);                                                         /**<  Hfp disable NREC(Noise reduction and echo canceling) function*/
    void (* notify_indicators_change)(bdaddr_t addr, UINT16 indicator, UINT32 value);/**<  Hfp notify indicators change function*/
    UINT32 (*get_remote_feature)(bdaddr_t *addr);
} bthfp_interface_t;


//define hfp 2.0 interface
/**Hfp callback function structure*/
typedef struct
{
    void (*slc_connection_cb)(bt_conn_t *);                              /**<  Hfp connected callback function*/
    void (*slc_disconnection_cb)(bt_conn_t *);                           /**<  Hfp disconnected callback function*/
    void (*ciev_cb)(bt_conn_t *, hfp_cind_t ind, UINT32 value);          /**<  CIEV state callback function, used to HS&HF process CIEV msg of AG*/
    void (*bt_hfp_ring_cb)(bt_conn_t *);                                 /**<  ring callback function, used to HS&HF notified app layer call in*/
    void (*sco_connection_cb)(bt_conn_t *);                              /**<  Hfp sco connected callback function*/
    void (*sco_disconnection_cb)(bt_conn_t *);                           /**<  Hfp sco disconnected callback function*/
    void (*call_identification_cb)(bt_conn_t *, UINT8 *buff, UINT32 len);/**<  call numer notify callback, used to HS&HF notified app layer call number*/
    void (*volume_cb)(bt_conn_t *, bthfp_volume_type_t type, UINT32 volume);                       /**<  Hfp volume adjust callback function*/
    void (*subscriber_cb)(bt_conn_t *, UINT8 *buff, UINT32 len);         /**<  pbap callback function, used to HS&HF notified app layer with pbap update*/
    void (*at_cb)(bt_conn_t *, UINT8 *buff, UINT32 len);                 /**<  Hfp custmom AT callback function*/
    void (*dial_cb)(bt_conn_t *, bt_call_type_t type, UINT8 *call_num);                       /**<  dial callback function, used to AG redial phone number, if call_num is NULL, then redial last call record*/
    void (*call_status_cb)(bt_conn_t *, bt_call_type_t type);                  /**<  call state change callback function, used to AG notified app layer call state, like answer/reject/release with HS&HF*/
}bt_hfp_cb_t;

/**Hfp interface function structure */
typedef struct
{
    int (*bt_hfp_init)(bt_hfp_cb_t *);                           /**<  Hfp initialization */
    int (*bt_hfp_connect)(bdaddr_t *, UINT32 timeout, void(*)(bt_conn_t *));  /**<  Hfp connect device function*/
    int (*bt_hfp_disconnect)(bt_conn_t *);                                    /**<  Hfp disconnect function*/
    int (*bt_hfp_audio_switch)(bt_conn_t *);                                  /**<  Hfp switch audio function, used to SCO connect/disconnect */
    int (*bt_hfp_call_answer)(bt_conn_t *, UINT32 cmd);                       /**<  Hfp call control function, used to HS&HF, and cmd is answer/reject/release*/
    int (*bt_hfp_call_outgoing)(bt_conn_t *, UINT8 *call_num);                /**<  Hfp call dail number, used to HS&HF, */
    int (*bt_hfp_adjust_volume)(bt_conn_t *, UINT8 type, UINT8 vol);          /**<  Hfp adjust volume with mic and speaker of AG and HS&HF*/
    int (*bt_hfp_send_raw_data)(bt_conn_t *, UINT8 *buff, UINT32 len);        /**<  Hfp send raw data with customization function*/
    int (*bt_hfp_send_ciev)(bt_conn_t *, hfp_cind_t ind, UINT8 value);        /**<  Hfp send CIEV msg to HS&HF function*/
    int (*bt_hfp_ring)(bt_conn_t *, UINT8 ring);                              /**<  Hfp start(ring=1) or stop(ring=0) ring function*/
    int (*bt_hfp_set_op_name)(bt_conn_t *, char *name, char *short_name);    /**<  Hfp set operator name, used to AG send info of +COPS when HF request COPS*/
    int (*bt_hfp_set_phone_num)(bt_conn_t *, UINT8 *number);                  /**<  Hfp set incoming phone num to stack*/
    int (*bt_hfp_close)(void);                                                /**<  Hfp close all connection and uninit hfp stack*/
} bt_hfp_if_t;

/**
* @brief   get hfp interface with version of 2.0
* @details get hfp 2.0 interface
* @param void
* @return bt_hfp_if_t structure of HFP 2.0 interface
*/
bt_hfp_if_t *bthfp_get_interface(void);

/**
* @brief   get hfp callback with version of 2.0
* @details get hfp callback structure
* @param void
* @return bt_hfp_cb_t structure of HFP 2.0 callback
*/
bt_hfp_cb_t *bthfp_get_callback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

/** @} */

#endif
