/**
 * @file    bt_avrcp.h
 * @brief    -
 * @details
 * @mainpage
 * @version  V1.0.0
 * @date     2019-10-11
 * @license  Copyright (C) 2019 Unisoc Communications Inc
 */


#ifndef __BT_AVRCP_H__
#define __BT_AVRCP_H__

/**
 * @addtogroup bluestack
 * @{
 * @defgroup avrcp
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "bluetooth/bt_types.h"
#include "bluetooth/bluetooth.h"

/** Avrcp connection state enumeration*/
typedef enum
{
    BTAVRCP_CONNECTION_STATE_DISCONNECTED = 0,     /**< Avrcp connection is disconnected */
    BTAVRCP_CONNECTION_STATE_CONNECTING,             /**< Avrcp connection is connecting */
    BTAVRCP_CONNECTION_STATE_CONNECTED,                /**< Avrcp connection is connected */
    BTAVRCP_CONNECTION_STATE_DISCONNECTING,         /**< Avrcp connection is disconnecting */
} btavrcp_connection_state_t;

typedef enum
{
    BTAVRCP_SDP_STATE_SUCCESS = 0,
    BTAVRCP_SDP_STATE_FAILED = 1,
} btavrcp_sdp_state_t;

/** Avrcp play status enumeration*/
typedef enum
{
    AVRCP_PLAYBACK_STATUS_STOPPED,                /**< The play is stopped */
    AVRCP_PLAYBACK_STATUS_PLAYING,                 /**< The play is playing */
    AVRCP_PLAYBACK_STATUS_PAUSED,                  /**< The play is paused */
    AVRCP_PLAYBACK_STATUS_FWD_SEEK,              /**< The play is seeking forward */
    AVRCP_PLAYBACK_STATUS_REV_SEEK,               /**< The play is seeking rewind */
    AVRCP_PLAYBACK_STATUS_ERROR = 0xff          /**< Error */
} avrcp_playback_status_t;


/** Avrcp event enumeration*/
typedef enum
{
    AVRCP_EVENT_PLAYBACK_STATUS_CHANGED = (0x01),                       /**< The playback status changed event */
    AVRCP_EVENT_TRACK_CHANGED = (0x02),                                          /**< The track changed event */
    AVRCP_EVENT_TRACK_REACHED_END = (0x03),                                  /**< The track reaches end event */
    AVRCP_EVENT_TRACK_REACHED_START = (0x04),                              /**< The track reaches start event */
    AVRCP_EVENT_PLAYBACK_POS_CHANGED = (0x05),                            /**< The palyback position changed event */
    AVRCP_EVENT_BATT_STATUS_CHANGED = (0x06),                              /**< The battery status changed event */
    AVRCP_EVENT_SYSTEM_STATUS_CHANGED = (0x07),                          /**< The system status changed event */
    AVRCP_EVENT_PLAYER_APPLICATION_SETTING_CHANGED = (0x08),  /**< The player application setting changed event */
    AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED = (0x09),             /**< The now playing content changed event */
    AVRCP_EVENT_AVAILABLE_PLAYERS_CHANGED = (0x0A),                    /**< The available players changed event */
    AVRCP_EVENT_ADDRESSED_PLAYERS_CHANGED = (0x0B),                   /**< The addressed players changed event */
    AVRCP_EVENT_UIDS_CHANGED = (0x0C),                                             /**< The UIDs changed event */
    AVRCP_EVENT_VOLUME_CHANGED = (0x0D),                                        /**< The volume changed event */
} avrcp_event_t;

/** AVRCP opcode used in app to parse avrcp command*/
typedef enum
{
    OPID_SELECT             = (0x0),
    OPID_UP,
    OPID_DOWN,
    OPID_LEFT,
    OPID_RIGHT,
    OPID_RIGHT_UP,
    OPID_RIGHT_DOWN,
    OPID_LEFT_UP,
    OPID_LEFT_DOWN,
    OPID_ROOT_MENU,
    OPID_SETUP_MENU,
    OPID_CONTENTS_MENU,
    OPID_FAVOURITE_MENU,
    OPID_EXIT,
    /* 0x0e to 0x1f Reserved */
    OPID_0                  = (0x20),
    OPID_1,
    OPID_2,
    OPID_3,
    OPID_4,
    OPID_5,
    OPID_6,
    OPID_7,
    OPID_8,
    OPID_9,
    OPID_DOT,
    OPID_ENTER,
    OPID_CLEAR,
    /* 0x2d - 0x2f Reserved */
    OPID_CHANNEL_UP         = (0x30),
    OPID_CHANNEL_DOWN,
    OPID_SOUND_SELECT,
    OPID_INPUT_SELECT,
    OPID_DISPLAY_INFORMATION,
    OPID_HELP,
    OPID_PAGE_UP,
    OPID_PAGE_DOWN,
    /* 0x39 - 0x3f Reserved */
    OPID_POWER              = (0x40),
    OPID_VOLUME_UP,
    OPID_VOLUME_DOWN,
    OPID_MUTE,
    OPID_PLAY,
    OPID_STOP,
    OPID_PAUSE,
    OPID_RECORD,
    OPID_REWIND,
    OPID_FAST_FORWARD,
    OPID_EJECT,
    OPID_FORWARD,
    OPID_BACKWARD,
    /* 0x4d - 0x4f Reserved */
    OPID_ANGLE              = (0x50),
    OPID_SUBPICTURE,
    /* 0x52 - 0x70 Reserved */
    OPID_F1                 = (0x71),
    OPID_F2,
    OPID_F3,
    OPID_F4,
    OPID_F5,
    OPID_VENDOR_UNIQUE      = (0x7e)
                              /* Ox7f Reserved */
} avrcp_opcode_t;

/** Avrcp device information structure */
typedef struct
{
    avrcp_playback_status_t Cur_Play_Status;                            /**< Current play status */
    UINT32 Cur_Song_Pos;                                                            /**< Current song position */
    UINT8 numberofevents;                                                           /**< Number of events */
    UINT8 Registerd_numberofevents;                                           /**< Registered number of events */
    UINT8 supported_events_id[AVRCP_EVENT_VOLUME_CHANGED];  /**< Supported events id  */
    UINT8 numberofattribures;                                                       /**< Number of attributes  */
    UINT8 attribures_index;                                                           /**< Attributes index  */
    UINT8 supported_attributes_type[4];                                        /**< Supported attributes type  */
    UINT8 supported_attributes_value_type[4][4];                           /**< Supported attributes value type  */
    UINT8 UID_Counter;                                                                 /**< UID counter  */
    UINT16 avrcp_cid;                                                                     /**< Avrcp channel id  */
    UINT16 avrcp_browse_cid;                                                        /**< Avrcp browser channel id  */
    UINT8 NumberofItems;                                                             /**< Number of items  */
    UINT8 folder_depth;                                                                 /**< Folder depth */
    UINT8 avrcp_adp_state;                                                           /**< Avrcp adp state */
    bdaddr_t avrcp_addr;
    //UINT8 Cur_Play_pause_key_status;
    UINT8 volume_changed_event_support;                                   /**< Volume changed event support */

    UINT16 profile_ver;                                                                  /**< Profile version */
    UINT16 ct_sdp_feature;                                                            /**< CT sdp feature */
    UINT16 tg_sdp_feature;                                                            /**< TG sdp feature */
    UINT8 sdp_status;                                                                     /**< Sdp status */
    UINT8 need_connect_after_sdp;                                                /**<Need connect after sdp */
} avrcp_device_info_t;

typedef struct
{
    UINT32 opcode;
    UINT8 *buff;
    UINT32 len;
}bt_avrcp_data_t;

typedef struct
{
    void (*bt_avrcp_connection_cb)(bt_conn_t *);
    void (*bt_avrcp_disconnection_cb)(bt_conn_t *);
    void (*bt_avrcp_data_cb)(bt_conn_t *, bt_avrcp_data_t *);
} bt_avrcp_cb_t;

typedef struct
{
    bt_status_t (*btavrcp_init)(bt_avrcp_cb_t *cb);
    bt_status_t (*btavrcp_connect)(bdaddr_t *addr, UINT32 timeout, void(*timeout_cb)(bt_conn_t *));
    bt_status_t (*btavrcp_disconnect)(bt_conn_t *avrcp_conn);
    bt_status_t (*btavrcp_notify_volume_change)(bdaddr_t addr);
    void (*btavrcp_send)(bdaddr_t *addr, UINT8 *pdu, UINT32 len);
    void (*btavrcp_close)(void);
} bt_avrcp_if_t;


/** Avrcp device callback structure */
typedef struct
{
    void (*connection_state_callback) (btavrcp_connection_state_t state,bdaddr_t *addr);/**<Connection state callback */
    void (*volume_changed_callback) (UINT8 volume, bdaddr_t *addr);                     /**<Volume changed callback */
    void (*get_cur_play_attr_callback) (UINT8 num, void *item);                         /**<Get current play attribute callback */

   /* sdp_status: see btavrcp_sdp_state_t
     * ct_feature: see btavrcp_sdp_ct_feature_t
     * tg_feature: see btavrcp_sdp_tg_feature_t
     * version: such as 0x0106
     */
    void (*get_sdp_feature_callback)(UINT8 sdp_status, bdaddr_t *addr, UINT32 ct_feature, UINT32 tg_feature, UINT16 version);
} btavrcp_callbacks_t;

//android callback

/*
typedef struct {
    size_t      size;//sizeof(BtRcCallbacks)
    btrc_remote_features_callback               remote_features_cb;
    btrc_get_play_status_callback               get_play_status_cb;
    btrc_list_player_app_attr_callback          list_player_app_attr_cb;
    btrc_list_player_app_values_callback        list_player_app_values_cb;
    btrc_get_player_app_value_callback          get_player_app_value_cb;
    btrc_get_player_app_attrs_text_callback     get_player_app_attrs_text_cb;
    btrc_get_player_app_values_text_callback    get_player_app_values_text_cb;
    btrc_set_player_app_value_callback          set_player_app_value_cb;
    btrc_get_element_attr_callback              get_element_attr_cb;
    btrc_register_notification_callback         register_notification_cb;
    btrc_volume_change_callback                 volume_change_cb;
    btrc_passthrough_cmd_callback               passthrough_cmd_cb;
} btrc_callbacks_t;
*/


/** Avrcp interface structure */
typedef struct
{
    bt_status_t (*init) (btavrcp_callbacks_t *callbacks); /**<Avrcp interface initialization function */
    bt_status_t (*connect) (bdaddr_t *addr);               /**<Avrcp connect function */
    bt_status_t (*disconnect) (bdaddr_t *addr);           /**<Avrcp disconnect function */
    avrcp_device_info_t* (*get_devices)(void);              /**<Avrcp get devices function */
    UINT8 (*get_max_devices_num)(void);                 /**<Avrcp get maximum device number function */
    bt_status_t (*send_key)(UINT8 key,UINT8 press, UINT8 param, bdaddr_t addr);/**<Avrcp send key function */
    bt_status_t (*sync_volume)( bdaddr_t addr,UINT8 volume);/**<Avrcp synchronize volume function */
    UINT8 (*get_volume)(void);/**<Avrcp get volume function */
    UINT8 (*get_play_state)(void);/**<Avrcp get play state function */
    UINT8 (*get_cur_play_info)(void);/**<Avrcp get current play information function */
    int (*is_support_volume_handling)(bdaddr_t addr);/**<Avrcp is support volume handling function */
    void (*reset_volume_change_state)(bdaddr_t addr);/**<Avrcp reset volume change state function */
    bt_status_t (*get_sdp_feature)(bdaddr_t *addr);
    bt_status_t (*sync_absolute_volume)(bdaddr_t addr, UINT8 volume);
    void  (*notify_volume_change)(bdaddr_t addr);
    bt_status_t (*send_key_volume_change)(bdaddr_t addr, UINT8 key);
} btavrcp_interface_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

/** @} */
bt_avrcp_if_t *btavrcp_get_interface(void);

#endif

