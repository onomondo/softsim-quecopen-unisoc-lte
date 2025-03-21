/**
 * @file    bt_a2dp.h
 * @brief    -
 * @details
 * @mainpage
 * @version  V1.0.0
 * @date     2019-10-11
 * @license  Copyright (C) 2019 Unisoc Communications Inc
 */



#ifndef __BT_A2DP_H__
#define __BT_A2DP_H__

/**
 * @addtogroup bluestack
 * @{
 * @defgroup a2dp
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "bluetooth/bt_types.h"
#include "bluetooth/bluetooth.h"
#include "connectivity_config.h"

typedef enum
{
    BTA2DP_CONNECTION_STATE_SIGNALING_DISCONNECTED = 0,
    BTA2DP_CONNECTION_STATE_SIGNALING_CONNECTED,
    BTA2DP_CONNECTION_STATE_MEDIA_DISCONNECTED,
    BTA2DP_CONNECTION_STATE_MEDIA_CONNECTED,
} bta2dp_src_connection_state_t;

typedef enum
{
    BTA2DP_CONNECTION_STATE_DISCONNECTED = 0,
    BTA2DP_CONNECTION_STATE_CONNECTING,
    BTA2DP_CONNECTION_STATE_CONNECTED,
    BTA2DP_CONNECTION_STATE_DISCONNECTING,
} bta2dp_connection_state_t;

typedef enum
{
    BTA2DP_AUDIO_STATE_REMOTE_SUSPEND = 0,
    BTA2DP_AUDIO_STATE_STOPPED,
    BTA2DP_AUDIO_STATE_START,
} bta2dp_audio_state_t;

#define A2DP_ADP_STATE_IDLE                     0
#define A2DP_ADP_STATE_CLOSE                    1
#define A2DP_ADP_STATE_PENDING                  2
#define A2DP_ADP_STATE_CONNECTED                3
#define A2DP_ADP_STATE_OPEN                     4
#define A2DP_ADP_STATE_PLAY                     5
#define A2DP_ADP_STATE_PAUSE                    6
#define A2DP_ADP_STATE_STOP                     7

typedef enum
{
    A2DP_SDP_STATUS_NONE,
    A2DP_SDP_STATUS_DISCOVRING,
    A2DP_SDP_STATUS_DONE,
    A2DP_SDP_STATUS_FAIL,
}A2DP_SDP_STATUS;

typedef struct
{
    UINT8 codec_type; // SBC, MP3
    UINT32 sample_rate;//44.1K...
}bt_a2dp_audio_codec_t;

/* AVDTP signals */
typedef enum
{
    A2DP_NULL = (0x00),
    A2DP_DISCOVER,
    A2DP_GET_CAPABILITIES,
    A2DP_SET_CONFIGURATION,
    A2DP_GET_CONFIGURATION,
    A2DP_RECONFIGURE,
    A2DP_OPEN,
    A2DP_START,
    A2DP_CLOSE,
    A2DP_SUSPEND,
    A2DP_ABORT,
    A2DP_SECURITY_CONTROL,
    A2DP_GET_ALL_CAPABILITIES,
    A2DP_DELAYREPORT
}a2dp_signal_id_t;

typedef struct
{
    bdaddr_t addr;
    a2dp_signal_id_t cmd;
}bt_a2dp_signaling_t;

typedef struct
{
    bdaddr_t addr;//target device addr
    UINT8 *sbc;//sbc data buffer
    UINT32 sbc_len;//sbc data len
    UINT8 sbc_frames;//sbc frames
    UINT16 seq_num;//sequence number
    UINT32 time_stamp;//timestamp
    UINT8 payload_type;//payload data type(0x60 for sbc)
    UINT8 marker;//default 0
}bt_a2dp_data_t;

typedef struct
{
    UINT16 signal_dcid;
    UINT16 media_dcid;
}bt_a2dp_cid_t;

#define A2DP_SAMPLERATE_8K      8000  /* 8  kHz */
#define A2DP_SAMPLERATE_11K     11025  /* 11.025 kHz */
#define A2DP_SAMPLERATE_12K     12000  /* 12  kHz */
#define A2DP_SAMPLERATE_16K     16000  /* 16  kHz */
#define A2DP_SAMPLERATE_22K     22050  /* 22.05kHz */
#define A2DP_SAMPLERATE_24K     24000  /* 24  kHz */
#define A2DP_SAMPLERATE_32K     32000  /* 32  kHz */
#define A2DP_SAMPLERATE_44K     44100  /* 44.1 kHz */
#define A2DP_SAMPLERATE_48K     48000  /* 48  kHz */
#define A2DP_SAMPLERATE_64K     64000  /* 64  kHz */
#define A2DP_SAMPLERATE_88K     88200  /* 88.2kHz */
#define A2DP_SAMPLERATE_96K     96000  /* 96  kHz */

typedef struct
{
    bdaddr_t a2dp_addr;
    UINT8 a2dp_device_name[MAX_BT_DEVICE_NAME];
    UINT8 seid;
    UINT16 cid;
    UINT8 connection_state;
    UINT8 signal_state;
    UINT32 a2dp_check_timer;

    UINT16 profile_ver;
    UINT16 sdp_feature;
    UINT8 sdp_status;
    UINT8 need_connect_after_sdp;
} a2dp_device_info_t;

typedef struct
{
    void (*connection_state_callback) (bta2dp_connection_state_t state, bdaddr_t *addr);
    void (*audio_state_callback) (bta2dp_audio_state_t state, bdaddr_t *addr);
    void (*audio_config_callback) (UINT8 codec_type, UINT32 sample_rate, bdaddr_t *addr);
    UINT8 (*audio_stream_callback) (UINT16 seq_num, hci_data_t *p_buf, UINT32 length);
    void (*cid_callback)(bt_a2dp_cid_t cid);
} bta2dp_callbacks_t;


typedef struct
{
    bt_status_t (*init) (bta2dp_callbacks_t *callbacks);
    bt_status_t (*connect) (bdaddr_t *addr);
    bt_status_t (*disconnect) (bdaddr_t *addr);
    a2dp_device_info_t* (*get_devices)(void);
    UINT8 (*get_max_devices_num)(void);
    void (*cleanup) (void);
} bta2dp_interface_t;


//define a2dp 2.0 interface
/**a2dp callback function structure*/
typedef struct
{
    void (*a2dp_signaling_connction_cb)(bt_conn_t *);
    void (*a2dp_signaling_disconnection_cb)(bt_conn_t *);
    void (*a2dp_media_connction_cb)(bt_conn_t *);
    void (*a2dp_media_disconnection_cb)(bt_conn_t *);
    void (*a2dp_data_cb)(UINT16 seq_num, hci_data_t * data, UINT32 length);
    void (*a2dp_signaling_cb)(a2dp_signal_id_t signal_id, bt_a2dp_audio_codec_t *audio_codec, bdaddr_t *addr);
} bt_a2dp_cb_t;

/**a2dp interface function structure */
typedef struct
{
    bt_status_t (*init)(bt_a2dp_cb_t *callbacks);
    bt_status_t (*connect)(bdaddr_t *addr);
    bt_status_t (*disconnect)(bdaddr_t *addr);
    void (*a2dp_send_data)(bt_a2dp_data_t *);
    void (*a2dp_send_signaling)(bt_a2dp_signaling_t *);
    void (*cleanup) (void);
} bt_a2dp_if_t;

bt_a2dp_if_t *bta2dp_get_interface(void);
bt_a2dp_cb_t *bta2dp_get_callback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

/** @} */


#endif
