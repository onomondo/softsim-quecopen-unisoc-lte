/**  @file
  ql_api_stk.h

  @brief
  This file is used to define stk api for different Quectel Project.

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
#ifndef QL_API_STK_H
#define QL_API_STK_H

#include <stdbool.h>
#include <stdint.h>

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_STK_ERRCODE_BASE              (QL_COMPONENT_STK<<16)

/* The default cid of BIP is 3 */
#define QL_STK_BIP_DEFAULT_CID           (3)

#define QL_STK_STRING_LENGTH_MAX         (512)
#define QL_STK_MAX_MENU_ITEMS            (50)
#define QL_STK_AUTO_RESPONSE_TIMEOUT_DEF (300)
#define QL_STK_AUTO_RESPONSE_TIMEOUT_MAX (65535)

#define QSTK_SIM_ENVELOPE_MENU_SELECTION  (253) /* used to select setup menu item */
#define QSTK_TERMINATE_STK_SESSION        (254) /* terminate current STK proactive command */
/*========================================================================
*  Enumeration Definition
*========================================================================*/
typedef enum
{
    QL_STK_SUCCESS                   = 0,
    QL_STK_EXECUTE_ERR               = 1 | QL_STK_ERRCODE_BASE,
    QL_STK_MEM_ADDR_NULL_ERR,
    QL_STK_INVALID_PARAM_ERR,
    QL_STK_MALLOC_MEM_ERROR,
    QL_STK_OPERATION_NOT_SUPPORTED   = 5 | QL_STK_ERRCODE_BASE,
    QL_STK_OPERATION_NOT_ALLOWED,
} ql_stk_errcode_e;

typedef enum
{
    QL_STK_BIP_CONFIG_MODE_DISABLE, /* will disable BIP function */
    QL_STK_BIP_CONFIG_MODE_AUTO, /* When the specified cid is not available, find the available cid automatically, default value */
    QL_STK_BIP_CONFIG_MODE_FIXED, /* When the specified cid is not available, BIP function is not useable */
}ql_stk_bip_config_mode_e;

typedef enum
{
    QL_STK_MODE_DISABLE, /**< disable STK functionality */
    QL_STK_MODE_ENABLE   /**< enable STK functionality */
} ql_stk_mode_e;

typedef enum
{
    QL_STK_ALPHABET_SET_GSM, /**< gsm */
    QL_STK_ALPHABET_SET_UCS2 /**< ucs2 */
} ql_stk_alphabet_set_e;

typedef enum
{
    QL_STK_PROACTIVE_CMD_NONE                  = 0x00,
    QL_STK_PROACTIVE_CMD_PLAY_TONE             = 0x20,
    QL_STK_PROACTIVE_CMD_DISPLAY_TEXT          = 0x21,
    QL_STK_PROACTIVE_CMD_GET_INKEY             = 0x22,
    QL_STK_PROACTIVE_CMD_GET_INPUT             = 0x23,
    QL_STK_PROACTIVE_CMD_SELECT_ITEM           = 0x24,
    QL_STK_PROACTIVE_CMD_SETUP_MENU            = 0x25,
    QL_STK_PROACTIVE_CMD_SETUP_IDLE_MODE_TEXT  = 0x28,
    QL_STK_PROACTIVE_CMD_LANGUAGE_NOTIFICATION = 0x35,
} ql_stk_proactive_cmd_e;

typedef enum
{
    /* Results '0X' and '1X' indicate that the command has been performed. */
    QL_STK_PERFORMED_SUCCESSFULLY                                           = 0x00,
    QL_STK_PERFORMED_WITH_PARTIAL_COMPREHENSION                             = 0x01,
    QL_STK_PERFORMED_WITH_MISSING_INFORMATION                               = 0x02,
    QL_STK_REFRESH_PERFORMED_WITH_ADDITIONAL_EFS_READ                       = 0x03,
    QL_STK_PERFORMED_SUCCESSFULLY_BUT_REQUESTED_ICON_NOT_DISPLAYED          = 0x04,
    QL_STK_PERFORMED_BUT_MODIFIED_BY_CALL_CONTROL_BY_SIM                    = 0x05,
    QL_STK_PERFORMED_SUCCESSFULLY_LIMITED_SERVICE                           = 0x06,
    QL_STK_PERFORMED_WITH_MODIFICATION                                      = 0x07,
    QL_STK_PROACTIVE_SIM_SESSION_TERMINATED_BY_THE_USER                     = 0x10,
    QL_STK_BACKWARD_MOVE_IN_THE_PROACTIVE_SIM_SESSION_REQUESTED_BY_THE_USER = 0x11,
    QL_STK_NO_RESPONSE_FROM_USER                                            = 0x12,
    QL_STK_HELP_INFORMATION_REQUIRED_BY_THE_USER                            = 0x13,
    QL_STK_USSD_OR_SS_TRANSACTION_TERMINATED_BY_THE_USER                    = 0x14,

    /* Results '2X' indicate to the SIM that it may be worth re-trying the command at a later opportunity. */
    QL_STK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND                      = 0x20,
    QL_STK_NETWORK_CURRENTLY_UNABLE_TO_PROCESS_COMMAND                 = 0x21,
    QL_STK_USER_DID_NOT_ACCEPT_THE_PROACTIVE_COMMAND                   = 0x22,
    QL_STK_USER_CLEARED_DOWN_CALL_BEFORE_CONNECTION_OR_NETWORK_RELEASE = 0x23,
    QL_STK_ACTION_IN_CONTRADICTION_WITH_THE_CURRENT_TIMER_STATE        = 0x24,
    QL_STK_INTERACTION_WITH_CALL_CONTROL_BY_SIM_TEMPORARY_PROBLEM      = 0x25,
    QL_STK_LAUNCH_BROWSER_GENERIC_ERROR_CODE                           = 0x26,

    /* Results '3X' indicate that it is not worth the SIM re-trying with an identical command, as it will only get the same
       response. However, the decision to retry lies with the SIM application. */
    QL_STK_COMMAND_BEYOND_ME_CAPABILITIES                                          = 0x30,
    QL_STK_COMMAND_TYPE_NOT_UNDERSTOOD_BY_ME                                       = 0x31,
    QL_STK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME                                       = 0x32,
    QL_STK_COMMAND_NUMBER_NOT_KNOWN_BY_ME                                          = 0x33,
    QL_STK_SS_RETURN_ERROR                                                         = 0x34,
    QL_STK_SMS_RP_ERROR                                                            = 0x35,
    QL_STK_ERROR_REQUIRED_VALUES_ARE_MISSING                                       = 0x36,
    QL_STK_USSD_RETURN_ERROR                                                       = 0x37,
    QL_STK_MULTIPLECARD_COMMANDS_ERROR                                             = 0x38,
    QL_STK_INTERACTION_WITH_CALL_CONTROL_BY_SIM_OR_MO_SHORT_MESSAGE_CONTROL_BY_SIM = 0x39,
    QL_STK_BEARER_INDEPENDENT_PROTOCOL_ERROR                                       = 0x3A,
    QL_STK_COMMAND_RESULT_MAX                                                      = 0XFF,
}ql_stk_command_result_e;

typedef enum
{
    QL_STK_PRESENTATION_NOT_SPECIFIED   = 0x00, /* presentation type is not specified */
    QL_STK_DATA_VALUE_PRESENTATION      = 0x01, /* presentation as a choice of data values */
    QL_STK_PRESENTATION_NOT_SPECIFIED_2 = 0x02, /* presentation type is not specified */
    QL_STK_NAVIGATION_PRESENTATION      = 0x03, /* presentation as a choice of navigation options */
} ql_stk_item_presentation_e;

typedef enum
{
    QL_STK_DIGITS_ONLY  = 0x00, /* digits (0-9, *, #, and +) only */
    QL_STK_ALPHABET_SET = 0x01, /* alphabet set */
} ql_stk_input_type_e;

typedef enum
{
    QL_STK_SMS_DEFAULT_ALPHABET = 0x00,
    QL_STK_UCS2_ALPHABET        = 0x01,
} ql_stk_input_alphabet_e;

typedef enum
{
    QL_STK_INPUT_UNPACKED   = 0x00,
    QL_STK_INPUT_SMS_PACKED = 0x01,
} ql_stk_input_pack_format_e;

typedef enum
{
    QL_STK_PROACTIVE_CMD_AUTO_RSP_STATE     = 0x00, /* qstk is disabled, will reply proactive command automaticaly */
    QL_STK_PROACTIVE_CMD_TERMINAL_RSP_STATE = 0x01, /* qstk is enabled, a proactive command needs to be handled or a SETUP MENU item can be selected */
    QL_STK_PROACTIVE_CMD_WAIT_STATE         = 0x02  /* qstk is enabled, and a terminal response message has been sent to SIM, waiting for response */
} ql_stk_state_e;

typedef enum
{
    QL_STK_TONE_DIAL_TONE                    = 0x01,
    QL_STK_TONE_CALLED_SUBSCRIBER_BUSY       = 0x02,
    QL_STK_TONE_CONGESTION                   = 0x03,
    QL_STK_TONE_RADIO_PATH_ACKNOWLEDGE       = 0x04,
    QL_STK_TONE_RADIO_PATH_NOT_AVAILABLE     = 0x05,
    QL_STK_TONE_ERROR_OR_SPECIAL_INFORMATION = 0x06,
    QL_STK_TONE_CALL_WAITING_TONE            = 0x07,
    QL_STK_TONE_RINGING_TONE                 = 0x08,

    QL_STK_TONE_GENERAL_BEEP                           = 0x10,
    QL_STK_TONE_POSITIVE_ACKNOWLEDGEMENT_TONE          = 0x11,
    QL_STK_TONE_NEGATIVE_ACKNOWLEDGEMENT_OR_ERROR_TONE = 0x12
} ql_stk_tone_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    bool status_report;
    ql_stk_bip_config_mode_e mode; /* BIP cid mode */
    uint8_t cid; /* the cid used by BIP, start from 1, max is PROFILE_IDX_VOLTE_MAX or PROFILE_IDX_MAX */
} ql_stk_bip_config_s;

typedef struct
{
    uint8_t profile_len;
    uint8_t profile_buf[34];
} ql_stk_profile_s;

typedef struct
{
    uint16_t length; /**< Length of the text. */
    uint8_t *text;   /**< Text string. */
} ql_stk_string_s;

typedef struct
{
    uint8_t qualifier;                      /**< Qualifiers specific to the command */
    uint8_t num_items;                      /**< Number of items. */
    uint8_t item_id[QL_STK_MAX_MENU_ITEMS]; /**< array list for items. */
    ql_stk_string_s title;                  /**< Title string. */
} ql_stk_setup_menu_s;

typedef struct
{
    uint8_t item_id;           /**< array list for items. */
    ql_stk_string_s item_text; /**< Title string. */
} ql_stk_item_s;

typedef struct
{
    uint8_t qualifier;                      /**< Qualifiers specific to the command */
    uint8_t num_items;                      /**< Number of items. */
    uint8_t item_id[QL_STK_MAX_MENU_ITEMS]; /**< array list for items. */
    ql_stk_string_s title;                  /**< Title. */
} ql_stk_select_item_s;

typedef struct
{
    uint8_t alpha2[2];
} ql_stk_iso6391_s;

typedef struct
{
    uint8_t qualifier;            /**< Qualifiers specific to the command */
    ql_stk_string_s text_string;  /**< Title string. */
} ql_stk_display_text_s;

typedef struct
{
    uint8_t qualifier;            /**< Qualifiers specific to the command */
    uint8_t minimum_user_input;   /**< Minimum number of user input. */
    uint8_t maximum_user_input;   /**< Maximum number of user input. */
    ql_stk_string_s text_string;  /**< Title string. */
    ql_stk_string_s default_text; /**< Default text. */
} ql_stk_get_input_s;

typedef struct
{
    uint8_t qualifier;            /**< Qualifiers specific to the command */
    ql_stk_string_s text_string;  /**< Title string. */
} ql_stk_get_inkey_s;

typedef struct
{
    uint8_t qualifier;           /**< Qualifiers specific to the command */
    ql_stk_string_s text_string; /**< Title string. */
} ql_stk_setup_idle_mode_text_s;

typedef struct
{
    uint8_t qualifier;         /**< Qualifiers specific to the command */
    ql_stk_iso6391_s language; /**< language, each language code is a pair of alpha-numeric characters, defined in ISO 639 [29].  */
} ql_stk_language_notification_s;

typedef struct
{
    uint8_t qualifier;     /**< Qualifiers specific to the command */
    ql_stk_tone_e tone;    /**< Standard supervisory tones */
    uint8_t duration_unit; /**< duration unit */
    uint8_t duration;      /**< the length of time for which the ME shall generate the tone, if the tone is continuous or repeatable. */
    ql_stk_string_s title; /**< Title string. */
} ql_stk_play_tone_s;

typedef struct
{
    ql_stk_proactive_cmd_e proactive_cmd_id;
} ql_stk_proactive_cmd_s; /**< QUEC_STK_PROACTIVE_CMD_IND event content */

typedef struct
{
    ql_stk_proactive_cmd_e proactive_cmd_id; 
} ql_stk_timeout_s; /**< QUEC_STK_NO_PROACTIVE_CMD_IND event content */

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BIP
/*****************************************************************
* Function: ql_stk_get_bip_config
*
* Description:
*   Get BIP configuration parameters
*   
* 
* Parameters:
*   config        [out]    BIP configuration parameters
*
* Return:ql_stk_errcode_e
*
*****************************************************************/
ql_stk_errcode_e ql_stk_get_bip_config(ql_stk_bip_config_s *config);

/*****************************************************************
* Function: ql_stk_set_bip_config
*
* Description:
*   Set BIP configuration parameters
*   
* 
* Parameters:
*   config        [in]    BIP configuration parameters
*
* Return:ql_stk_errcode_e
*
*****************************************************************/
ql_stk_errcode_e ql_stk_set_bip_config(const ql_stk_bip_config_s* config);
#endif

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_stk_callback
*
* Description:
*   stk event callback, stk module will notify the occurrence of the event through this callback
* 
* Parameters:
*   nSim      [in] Index of sim, range:0-1.
*   ind_type  [in] event type, 
*                   QUEC_STK_PROACTIVE_CMD_IND: stk module fetched a proactive command from SIM.
*                   QUEC_STK_NO_PROACTIVE_CMD_IND: no more proactive command
*                   QUEC_STK_TIMEOUT_IND: If the proactive command is not replied through ql_stk_send_command_result 
*                      within the set time, this timeout event will be triggered, and the module will automatically 
*                      reply to the proactive command.
*                   QUEC_STK_SIM_LOST_IND: sim card removed
*   ctx       [in] event content
*
* Return:void
*****************************************************************/
typedef void (*ql_stk_callback)(uint8_t nSim, uint32_t ind_type, void *ctx);

/*****************************************************************
* Function: ql_stk_register_cb
*
* Description:
*   register stk module callback
* 
* Parameters:
*   stk_cb       [in] event  callback
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_register_cb(ql_stk_callback stk_cb);

/*****************************************************************
* Function: ql_stk_cfg_get
*
* Description:
*   get stk module configuration parameter
* 
* Parameters:
*   stk_mode                        [out] stk mode
*   stk_alphabet_type               [out] stk_alphabet_type
*   stk_auto_response_timeout       [out] stk_auto_response_timeout , unit:second
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_cfg_get(ql_stk_mode_e *stk_mode, ql_stk_alphabet_set_e *stk_alphabet_type, uint16_t *stk_auto_response_timeout);

/*****************************************************************
* Function: ql_stk_cfg_set
*
* Description:
*   set stk module configuration parameter
* 
* Parameters:
*   stk_mode                        [in] stk mode, enable or disable stk function, by enable stk function, related stk 
*                                         feature will be enabled in terminal profile.
*   stk_alphabet_type               [in] stk_alphabet_type
*   stk_auto_response_timeout       [in] stk_auto_response_timeout , unit:second
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_cfg_set(ql_stk_mode_e stk_mode, ql_stk_alphabet_set_e stk_alphabet_type, uint16_t stk_auto_response_timeout);

/*****************************************************************
* Function: ql_stk_get_terminal_profile
*
* Description:
*   get terminal profile data
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   profile                [out] profile data
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_get_terminal_profile(uint8_t nSim, ql_stk_profile_s *profile);

/*****************************************************************
* Function: ql_stk_acquire_setup_menu_info
*
* Description:
*   get proactive command SET UP MENU without items
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_setup_menu_info(uint8_t nSim, ql_stk_setup_menu_s *info);

/*****************************************************************
* Function: ql_stk_acquire_setup_menu_item_list
*
* Description:
*   get proactive command SET UP MENU items
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   item_identifier        [in] setup menu item ID
*   info                   [out] item information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_setup_menu_item_list(uint8_t nSim, uint8_t item_identifier, ql_stk_item_s *info);

/*****************************************************************
* Function: ql_stk_acquire_sub_menu_info
*
* Description:
*   get proactive command SELECT ITEM without items
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_sub_menu_info(uint8_t nSim, ql_stk_select_item_s *info);

/*****************************************************************
* Function: ql_stk_acquire_sub_menu_item_list
*
* Description:
*   get proactive command SELECT ITEM items
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   item_identifier        [in] setup menu item ID
*   info                   [out] item information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_sub_menu_item_list(uint8_t nSim, uint8_t item_identifier, ql_stk_item_s *info);

/*****************************************************************
* Function: ql_stk_acquire_display_text_info
*
* Description:
*   get proactive command DISPLAY TEXT
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_display_text_info(uint8_t nSim, ql_stk_display_text_s *info);

/*****************************************************************
* Function: ql_stk_acquire_get_input_info
*
* Description:
*   get proactive command GET INPUT
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_get_input_info(uint8_t nSim, ql_stk_get_input_s *info);

/*****************************************************************
* Function: ql_stk_acquire_get_inkey_info
*
* Description:
*   get proactive command GET INKEY
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_get_inkey_info(uint8_t nSim, ql_stk_get_inkey_s *info);

/*****************************************************************
* Function: ql_stk_acquire_setup_idle_mode_text_info
*
* Description:
*   get proactive command SET UP IDLE MODE TEXT
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_setup_idle_mode_text_info(uint8_t nSim, ql_stk_setup_idle_mode_text_s *info);

/*****************************************************************
* Function: ql_stk_acquire_language_notification_info
*
* Description:
*   get proactive command LANGUAGE NOTIFICATION
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_language_notification_info(uint8_t nSim, ql_stk_language_notification_s *info);

/*****************************************************************
* Function: ql_stk_acquire_play_tone_info
*
* Description:
*   get proactive command PLAY TONE
* 
* Parameters:
*   nSim                   [in] Index of sim, range:0-1.
*   info                   [out] command information
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_acquire_play_tone_info(uint8_t nSim, ql_stk_play_tone_s *info);

/*****************************************************************
* Function: ql_stk_send_command_result
*
* Description:
*   respond to the proactive command or select setup menu item
* 
* Parameters:
*   nSim         [in] Index of sim, range:0-1.
*   stk_cmd_id   [in] command information, nSim is requested in all scenes
                stk_cmd_id is filled with proactive command PLAY_TONE: result is requested
                stk_cmd_id is filled with proactive command SETUP_IDLE_MODE: result is requested
                stk_cmd_id is filled with proactive command LANGUAGE_NOTIFICATION: result is requested
                stk_cmd_id is filled with proactive command SETUP_MENU: result is requested
                stk_cmd_id is filled with proactive command DISPLAY_TEXT: result is requested    
                stk_cmd_id is filled with proactive command GET_INKEY: result and string is requested 
                stk_cmd_id is filled with proactive command GET_INPUT: result and string is requested 
                stk_cmd_id is filled with proactive command SELECT_ITEM: result and item_id are requested
                stk_cmd_id is filled with QSTK_SIM_ENVELOPE_MENU_SELECTION: result and item_id are requested
                        result: QL_STK_PERFORMED_SUCCESSFULLY, send ENVELOPE (MENU SELECTION) without Help request
                        result: QL_STK_HELP_INFORMATION_REQUIRED_BY_THE_USER, send ENVELOPE (MENU SELECTION) with  Help request
                stk_cmd_id is filled with QSTK_TERMINATE_STK_SESSION: will terminate current proactive command 
                                                        with result QL_STK_PROACTIVE_SIM_SESSION_TERMINATED_BY_THE_USER. 
*   result                 [in] general result, ql_stk_command_result_e
*   item_id                [in] item identifier, used in SELECT_ITEM or QSTK_SIM_ENVELOPE_MENU_SELECTION
*   string                 [in] input string, used in GET_INKEY or GET_INPUT
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_send_command_result(uint8_t nSim, uint8_t stk_cmd_id, uint8_t result, uint8_t item_id, ql_stk_string_s *string);

/*****************************************************************
* Function: ql_stk_get_stk_proactive_cmd
*
* Description:
*   get current proactive command id and stk state
* 
* Parameters:
*   nSim             [in] Index of sim, range:0-1.
*   proactive_cmd    [out] current proactive command
*   state            [out] current qstk module state
*
* Return:ql_stk_errcode_e
*****************************************************************/
ql_stk_errcode_e ql_stk_get_stk_proactive_cmd(uint8_t nSim, ql_stk_proactive_cmd_e *proactive_cmd, ql_stk_state_e *state);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_STK_H */


