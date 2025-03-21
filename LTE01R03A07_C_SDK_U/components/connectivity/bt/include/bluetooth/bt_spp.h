/**
 * @file    bt_spp.h
 * @brief    -
 * @details
 * @mainpage
 * @version  V1.0.0
 * @date     2019-10-11
 * @license  Copyright (C) 2019 Unisoc Communications Inc
 */


#ifndef __BT_SPP_H__
#define __BT_SPP_H__

/**
 * @addtogroup bluestack
 * @{
 * @defgroup spp
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "bluetooth/bt_types.h"

#define SPP_FLOW_STOP 0
#define SPP_FLOW_GO 1

typedef struct
{
    bdaddr_t addr;
    UINT8 port;
    UINT8 state;
} sppif_device_info_t;

typedef struct
{
    UINT16 spp_sent_length;
    UINT16 spp_resend_length;
} t_spp_data_length;

typedef enum
{
    BTSPP_CONNECTION_STATE_DISCONNECTED = 0,
    BTSPP_CONNECTION_STATE_CONNECTING,
    BTSPP_CONNECTION_STATE_CONNECTED,
    BTSPP_CONNECTION_STATE_DISCONNECTING,
} btspp_connection_state_t;


typedef struct
{
    void (*connection_state_callback) (btspp_connection_state_t state,bdaddr_t *addr);
    UINT8 (*data_recv_callback) (UINT16 port, UINT8* data, UINT16 length);
    UINT8 (*flowctrl_ind_callback)(UINT32 result);
    UINT8 (*mtu_result_callback)(UINT16 frame_size);
    UINT8 (*resend_callback)(UINT16 spp_resend_data_length);
} btspp_callbacks_t;


typedef struct
{
    bt_status_t (*init) (btspp_callbacks_t *callbacks);
    bt_status_t (*connect) (bdaddr_t *addr);
    bt_status_t (*send) (bdaddr_t *addr, UINT8* data, UINT16 length);
    sppif_device_info_t* (*get_devices)(void);
    UINT8 (*get_max_devices_num)(void);
    bt_status_t (*disconnect) (bdaddr_t *addr);
} btspp_interface_t;

/**
 * @brief get bt spp interface
 * @return bt spp interface
 */
const btspp_interface_t *get_btspp_interface(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

/** @} */

#endif /* __BT_SPP_H__*/


