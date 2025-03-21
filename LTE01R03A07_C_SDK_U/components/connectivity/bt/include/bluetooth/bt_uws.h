/**
 * @file    bt_uws.h
 * @brief    -
 * @details
 * @mainpage
 * @version  V1.0.0
 * @date     2019-10-11
 * @license  Copyright (C) 2019 Unisoc Communications Inc
 */


#ifndef __BT_UWS_H__
#define __BT_UWS_H__

/**
 * @addtogroup bluestack
 * @{
 * @defgroup bt_uws
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "bluetooth/bt_types.h"
#include "bluetooth/bt_gatt.h"

typedef struct
{
    UINT8 (*is_right_ear) (void);
} btuws_callbacks_t;

typedef struct
{
    bt_status_t (*init) (btuws_callbacks_t *cb);
    bt_status_t (*adv_init) (gatt_extend_adv_param_t *adv_param);
    bt_status_t (*initiating_init) (le_ext_create_con_param_t *con_param);
    void (*host_transmit_audio_msg) (UINT32 msg_addr);
} btuws_interface_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

/** @} */
#endif /* __BT_UWS_H__ */

