/*
 * Copyright (c) 2019 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description:
 * aliyun_iot_smartcard_export.h: official export file
 *
 */
#ifndef __ALIYUN_IOT_SMARTCARD_EXPORT_H__
#define __ALIYUN_IOT_SMARTCARD_EXPORT_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    POLICY_RSSI = 0,
    POLICY_DELAY,
    POLICY_MIX,
    POLICY_MAX
} aliyun_iot_smartcard_policy_t;

typedef enum {
    CARRIER_NULL = 0,
    CHINA_MOBILE = 1,
    CHINA_UNICOM,
    CHINA_TELECOM,
    CARRIER_UNKNOWN
} aliyun_iot_smartcard_carrier_type_t;

/* Do NOT modify */
typedef enum {
    SIM_NULL = 0,
    SMARTCARD_SIM1 = 1,     /* SMARTCARD_SIM1 means CHINA_MOBILE  */
    SMARTCARD_SIM2,         /* SMARTCARD_SIM2 means CHINA_UNICOM  */
    SMARTCARD_SIM3,         /* SMARTCARD_SIM3 means CHINA_TELECOM */
    SMARTCARD_SIMEXT        /* SMARTCARD_SIMEXT means external slot sim */
} aliyun_iot_smartcard_slot_num_t;

typedef enum {
    ACT_NULL = 0,
    ACT_2G,
    ACT_3G,
    ACT_4G
} aliyun_iot_smartcard_act_t;

typedef struct {
    aliyun_iot_smartcard_slot_num_t slot_num;       /**< slot type: SIM1 / SIM2 / SIM3 */
    char iccid[32 + 1];                             /**< carrier iccid */
    char car_name[8 + 1];                           /**< carrier name */
    aliyun_iot_smartcard_carrier_type_t car_type;   /**< carrier type derived from iccid --> plmn*/
    bool active;                                    /**< current carrier is active or not */
    aliyun_iot_smartcard_act_t act;                 /**< access technology selected */
    int32_t rssi;                                   /**< rssi val */
    uint16_t delay;                                 /**< delay in ms */
    uint8_t score;                                  /**< score calculated by rssi and delay, lossrate */
} aliyun_iot_smartcard_sim_t;

/* smartcard error code definitions */
#define MN_SUCCESS                   0  /* success */
#define MN_ERROR_GENERIC            -1  /* generic error */
#define MN_ERROR_INIT               -2  /* init error */
#define MN_ERROR_COMMON_PARAMETER   -3  /* Invalid parameter */
#define MN_ERROR_NO_SIM             -4  /* no sim card detected */
#define MN_ERROR_SAME_CARD          -5  /* iccid is same after switch */
#define MN_ERROR_TIMEOUT            -6  /* timeout */
#define MN_ERROR_NOT_SMARTCARD      -7  /* not alibaba iot smart card */
#define MN_ERROR_SERIAL             -8  /* serial init error */
#define MN_ERROR_CHECK              -9  /* fail to parse alibaba iot smard card info */
#define MN_ERROR_DIAL_UP_FAIL       -10 /* dial up fail */
#define MN_ERROR_REG_FAIL           -11 /* network reg fail */
#define MN_ERROR_SDK_ENABLE         -12 /* SDK is disabled */
#define MN_ERROR_SCAN               -13 /* fail to scan carrier and select best carrier */
#define MN_ERROR_SWITCHING          -14 /* carrier switching is undergoing */
#define MN_ERROR_CARRIER_DISABLED   -15 /* carrier is disabled */
#define MN_ERROR_PARSE_CONFIG       -16 /* parse config file fail, default config is used */
#define MN_ERROR_SCAN_DISABLED      -17 /* scan is disabled */
#define MN_ERROR_SWITCH_DISABLED    -18 /* switch is disabled */
#define MN_ERROR_MSWITCH_DISABLED   -19 /* manu switch is disabled */

/* value MUST be same as aliyun_iot_smartcard_slot_num_t(logic slot) */
typedef enum {
    SWITCH_TO_SIM1 = 1, /* CHINA_MOBILE */
    SWITCH_TO_SIM2,     /* CHINA_UNICOM */
    SWITCH_TO_SIM3,     /* CHINA_TELECOM */
    SWITCH_TO_SIMEXT,   /* switch to external sim */
    SWITCH_TO_NEXT,     /* switch sequence: SIM1 -> SIM2 -> SIM3 -> SIM1 */
    SWITCH_MAX
} aliyun_iot_smartcard_type_t;

typedef enum {
    ALI_MN_STATE_NONE = 0,
    ALI_MN_STATE_IDLE,
    ALI_MN_STATE_INITING,
    ALI_MN_STATE_SWITCHING,
    ALI_MN_STATE_SCANNING,
    ALI_MN_STATE_CHECKING,
    ALI_MN_STATE_DATA_UPDATING,
    ALI_MN_STATE_DATA_SENDING,
    ALI_MN_STATE_MAX
} ali_mn_state_et;


typedef struct {
    int cnt;
    char ping_addr[3][256];
} ali_mn_ping_addr_t;

/*!
 * \brief   aliyun smart card initialization
 *
 * \retval  Possible returns are:
 *           0: init success
 *          -1: init fail
 *          -2: already inited
 *          -3: parameter error
 *          -4: sim card not detected
 *          -6: timeout
 *          -8: serial init error
 *          -9: fail to parse smartcard info
 */
int aliyun_iot_smartcard_init(void);

/*!
 * \brief   aliyun smart card deinitialization
 *
 * \retval  Possible returns are:
 *           0: deinit success
 *          -1: deinit fail
 *          -2: not inited
 */
int aliyun_iot_smartcard_deinit(void);

/*!
 * \brief   check if aliyun smart card or not
 *
 * \param   [OUT] : support: true is smartcard, false is not smartcard
 *
 * \retval  Possible returns are:
 *           0: success
 *          -1: fail
 */
int aliyun_iot_is_smartcard(bool *support);

/*!
 * \brief   start aliyun smart card monitor task
 *
 * \retval  Possible returns are:
 *          -1: fail to start task
 *          -2: sdk not inited
 *          -12: sdk is disabled
 *          -18: smart switch is disabled
 *           0: success
 * \attention  This function should be called after aliyun_iot_smartcard_init
 */
int aliyun_iot_smartcard_start_monitor(void);

/*!
 * \brief   stop aliyun smart card monitor task
 *
 * \retval  Possible returns are:
 *          -1: fail to stop task
 *          -12: sdk is disabled
 *          -18: smart switch is disabled
 *           0: success
 * \attention  This function should be called before aliyun_iot_smartcard_deinit
 */
int aliyun_iot_smartcard_stop_monitor(void);

/*!
 * \brief   aliyun smart card scan carrier
 *
 * \retval  Possible returns are:
 *          -12: sdk is disabled
 *          -13: scan fail
 *          -14: carrier switching is undergoing
 *          -17: scan is disabled
 *           0: success
 */
int aliyun_iot_smartcard_scan_carrier(void);

/*!
 * \brief   switch to specified operator
 *
 * \param   [IN] type: switch type
 *
 * \retval  Possible returns are:
 *            0: switch success
 *           -1: switch fail
 *           -2: not inited
 *           -3: parameter error
 *           -5: iccid is same after switch
 *           -6: timeout
 *           -7: not alibaba iot smart card
 *          -12: sdk is disabled
 *          -14: carrier switching is undergoing
 *          -19: manu switch is disabled
 */
int aliyun_iot_smartcard_manual_switch(aliyun_iot_smartcard_type_t type);

/*!
 * \brief   parse json config file
 *
 * \param   [IN] config file path
 *
 * \retval  Possible returns are:
 *          -1: fail to create default config
 *          -16: parse config file fail, default config is used
 *           0: success
 */
int aliyun_iot_smartcard_parse_configs(const char *config_file);

/*!
 * \brief   update json config file
 *
 * \retval  Possible returns are:
 *          -1: fail
 *           0: success
 */
int aliyun_iot_smartcard_update_configs(void);

/*!
 * \brief   aliyun smart card get sdk version
 *
 * \param   [OUT] major: major version
 * \param   [OUT] minor: minor version
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_version(int *major, int *minor);

/*!
 * \brief   get all sim status info
 *
 * \param   [INOUT] slot_cnt as input param, it stores max sim card number sim_info can hold
 *                  As output param, it receives the actual sim card number this API returns
 * \param   [OUT] sim_info sim_info array to hold the found sim card
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_all_sim_info(uint8_t *slot_cnt, aliyun_iot_smartcard_sim_t *sim_info);

/*!
 * \brief   get aliyun smart card smart switch policy
 *
 * \param   [IN] policy: pointer to policy val
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_switch_policy(aliyun_iot_smartcard_policy_t *policy);

/*!
 * \brief   set aliyun smart card smart switch policy
 *
 * \param   [IN] policy: policy val
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_set_switch_policy(aliyun_iot_smartcard_policy_t policy);

/*!
 * \brief   get rssi threshold
 *
 * \param   [OUT] rssi: current rssi
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_get_rssi_threshold(int32_t *rssi);

/*!
 * \brief   set rssi threshold
 *
 * \param   [IN] rssi: rssi to be set
 *
 * \retval  Possible returns are:
 *           0: success
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_set_rssi_threshold(int32_t rssi);

/*!
 * \brief   get delay threshold
 *
 * \param   [OUT] delay: current delay
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 *
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_get_delay_threshold(uint16_t *delay);

/*!
 * \brief   set delay threshold
 *
 * \param   [IN] delay: delay to be set
 *
 * \retval  Possible returns are:
 *           0: success
 *
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_set_delay_threshold(uint16_t delay);

/*!
 * \brief   get check cycle
 *
 * \param   [OUT] cycle: current check cycle
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_get_check_cycle(uint32_t *cycle);

/*!
 * \brief   set check cycle
 *
 * \param   [IN] cycle: check cycle to be set
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 * \attention  This function only used for smart switch
 */
int aliyun_iot_smartcard_set_check_cycle(uint32_t cycle);

/*!
 * \brief   get ping web site
 *
 * \retval  pointer to web
 */
const char *aliyun_iot_smartcard_get_ping_addr(void);

/*!
 * \brief   get ping web site parse result
 *
 * \retval  pointer to ali_mn_ping_addr_t
 */
const ali_mn_ping_addr_t *aliyun_iot_smartcard_get_ping_addr_info(void);

/*!
 * \brief   set ping addr
 *
 * \param   [IN] addr: ping addr to be set
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *          -1: parse error
 *           0: success
 */
int aliyun_iot_smartcard_set_ping_addr(const char *addr);

/*!
 * \brief   get disabled carrier string
 *
 * \retval  pointer to disabled carrier string
 */
const char *aliyun_iot_smartcard_get_disable_carrier(void);

/*!
 * \brief   set carrier to be disabled forcely
 *
 * \param   [IN] carrier: carrier to be set
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_set_disable_carrier(const char *carrier);

/*!
 * \brief   get sdk enabled or not
 *
 * \param   [OUT] enable
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_enable(bool *enable);

/*!
 * \brief   set sdk enabled or not
 *
 * \param   [IN] enable: enable or disable sdk support
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_enable(bool enable);

/*!
 * \brief   get carrier scanning flag
 *
 * \param   [OUT] enable
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_scan_flag(bool *enable);

/*!
 * \brief   set carrier scanning flag
 *
 * \param   [IN] enable: enable or disable scanning
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_scan_flag(bool enable);

/*!
 * \brief   get smart switch flag
 *
 * \param   [OUT] enable
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_switch_flag(bool *enable);

/*!
 * \brief   set smart switch flag
 *
 * \param   [IN] enable: enable or disable auto-switch
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_switch_flag(bool enable);

/*!
 * \brief   get carrier manu switch flag
 *
 * \param   [OUT] enable
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_manu_switch_flag(bool *enable);

/*!
 * \brief   set carrier manu switch flag
 *
 * \param   [IN] enable: enable or disable manu switch
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_manu_switch_flag(bool enable);

/*!
 * \brief   get check retry cnt
 *
 * \param   [OUT] cnt
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_check_retry_cnt(uint32_t *cnt);

/*!
 * \brief   set check retry cnt
 *
 * \param   [IN] cnt: retry cnt
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_check_retry_cnt(uint32_t cnt);

/*!
 * \brief   get ping cnt
 *
 * \param   [OUT] cnt
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_ping_cnt(uint32_t *cnt);

/*!
 * \brief   set ping cnt
 *
 * \param   [IN] cnt: ping cnt
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_set_ping_cnt(uint32_t cnt);

/*!
 * \brief   get silent time
 *
 * \param   [OUT] sec
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_silent_time(uint16_t *sec);

/*!
 * \brief   set silent time
 *
 * \param   [IN] sec: silent time
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_silent_time(uint16_t sec);

/*!
 * \brief   get conf file
 *
 * \retval  pointer to conf file
 */
const char *aliyun_iot_smartcard_get_conf_file(void);

/*!
 * \brief   set conf file
 *
 * \param   [IN] conf_file
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_set_conf_file(const char *conf_file);

/*!
 * \brief   get aliyun smart card sdk current state
 *
 * \param   [OUT] state
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_state(ali_mn_state_et *state);

/*!
 * \brief   get current selected card slot number
 *
 * \param   [OUT] slot_num: current card slot number, 1 is SIM1, 2 is SIM2, 3 is SIM3, 4 is SIMEXT
 *
 * \retval  Possible returns are:
 *          -1: fail
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_card_slot(aliyun_iot_smartcard_slot_num_t *slot_num);

/**
 * \brief get aliiccid by reading pbk directly
 *
 * \param [out] aliiccid: pointer to aliiccid str
 * \param [in] len: aliiccid str len
 *
 * \retval  Possible returns are:
 *          -1: fail
 *          -3: parameter error
 *           0: success
 *
 * \attention  This function can be called before aliyun_iot_smartcard_init
 */
int aliyun_iot_smartcard_get_aliiccid(char *iccid, uint8_t len);

/**
 * \brief get cached aliiccid without reading pbk
 *
 * \param [out] aliiccid: pointer to aliiccid str
 * \param [in] len: aliiccid str len
 *
 * \retval  Possible returns are:
 *          -1: fail
 *          -3: parameter error
 *           0: success
 *
 * \attention  This function should be called after aliyun_iot_smartcard_init
 */
int aliyun_iot_smartcard_get_cached_aliiccid(char *iccid, uint8_t len);

/*!
 * \brief   get if average value checking is supported or not
 *
 * \param   [OUT] enable: if enabled, support average value checking
 *
 * \retval  Possible returns are:
 *          -3: parameter error
 *           0: success
 */
int aliyun_iot_smartcard_get_average_check_flag(bool *enable);

/*!
 * \brief   set average value checking support flag
 *
 * \param   [IN] enable: enable or disable flag
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_set_average_check_flag(bool enable);

/*!
 * \brief   smart card wait pbk initialization
 *
 * \param   [IN] timeout: wait timeout, uint ms
 *
 * \retval  Possible returns are:
 *           0: success
 */
int aliyun_iot_smartcard_hal_pbk_wait(uint32     		 timeout);

#endif /* __ALIYUN_IOT_SMARTCARD_EXPORT_H__ */
