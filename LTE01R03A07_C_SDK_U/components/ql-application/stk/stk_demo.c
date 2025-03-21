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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_stk.h"
#include "ql_api_osi.h"
#include "ql_power.h"
#include "ql_log.h"

#define QL_STK_DEMO_LOG_LEVEL              QL_LOG_LEVEL_INFO
#define QL_STK_DEMO_LOG(msg, ...)          QL_LOG(QL_STK_DEMO_LOG_LEVEL, "ql_STK_DEMO", msg, ##__VA_ARGS__)
#define QL_STK_DEMO_LOG_PUSH(msg, ...)     QL_LOG_PUSH("ql_STK_DEMO", msg, ##__VA_ARGS__)

#define QL_STK_ALPHABET_SET                (QL_STK_ALPHABET_SET_UCS2)

#ifndef ARR_SIZE
#define ARR_SIZE(a) (sizeof((a)) / sizeof((a[0])))
#endif

typedef enum
{
    MAIN_MENU = 0x00,
    SUB_MENU  = 0x01,
} ql_stk_menu_e;

static ql_task_t stk_task = NULL;
static ql_stk_alphabet_set_e stk_alphabet = QL_STK_ALPHABET_SET;

ql_stk_setup_menu_s menu_info = {0};
ql_stk_select_item_s submenu_info = {0};

static const ql_stk_errcode_e (*get_item_list_fp[])(uint8_t nSim, uint8_t item_identifier, ql_stk_item_s *info) = {
    [MAIN_MENU] = ql_stk_acquire_setup_menu_item_list, [SUB_MENU] = ql_stk_acquire_sub_menu_item_list};

static bool hex_to_string(uint8_t const *data, uint16_t length, char *hexString)
{
	uint16_t i = 0;

	for(i=0; i<length; i++)
	{
		sprintf(&hexString[i*2], "%02X", data[i]); 
	}

	return true;
}

void user_stk_event_callback(uint8_t nSim, uint32_t ind_type, void *ctx)
{
    ql_event_t event = {0};
    QL_STK_DEMO_LOG("nSim:%d, ind:%#02X", nSim, ind_type);

    switch (ind_type)
    {
    case QUEC_STK_PROACTIVE_CMD_IND: {
        QL_STK_DEMO_LOG("QUEC_STK_PROACTIVE_CMD_IND");

        ql_stk_proactive_cmd_s *cmd = (ql_stk_proactive_cmd_s *)ctx;
        event.id = QUEC_STK_PROACTIVE_CMD_IND;
        event.param1 = nSim;
        event.param2 = cmd->proactive_cmd_id;
        ql_rtos_event_send(stk_task, &event);
        break;
    }

    case QUEC_STK_TIMEOUT_IND: {
        QL_STK_DEMO_LOG("QUEC_STK_TIMEOUT_IND");

        ql_stk_timeout_s *cmd = (ql_stk_timeout_s *)ctx;
        event.id = QUEC_STK_TIMEOUT_IND;
        event.param1 = nSim;
        event.param2 = cmd->proactive_cmd_id;
        ql_rtos_event_send(stk_task, &event);
        break;
    }

    case QUEC_STK_SIM_LOST_IND: {
        QL_STK_DEMO_LOG("QUEC_STK_SIM_LOST_IND");

        event.id = QUEC_STK_SIM_LOST_IND;
        event.param1 = nSim;
        ql_rtos_event_send(stk_task, &event);
        break;
    }

    case QUEC_STK_NO_PROACTIVE_CMD_IND: {
        QL_STK_DEMO_LOG("QUEC_STK_NO_PROACTIVE_CMD_IND");

        event.id = QUEC_STK_NO_PROACTIVE_CMD_IND;
        event.param1 = nSim;
        ql_rtos_event_send(stk_task, &event);
        break;
    }
    default:
        break;
    }
}

static void stk_item_list_info(uint8_t nSim, ql_stk_menu_e menu_type, uint8_t *id_arr, uint8_t id_total)
{
    ql_stk_item_s item_info = {0};

    uint8_t *str_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX);
    if (str_buffer == NULL)
    {
        return;
    }
    item_info.item_text.text = str_buffer;
    item_info.item_text.length = QL_STK_STRING_LENGTH_MAX;

    char *item_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX * 2 + 1);
    if (item_buffer == NULL)
    {
        return;
    }

    for (size_t i = 0; i < id_total; i++)
    {
        item_info.item_text.length = QL_STK_STRING_LENGTH_MAX;
        ql_stk_errcode_e err = get_item_list_fp[menu_type](nSim, id_arr[i], &item_info);
        if (err == QL_STK_SUCCESS)
        {
            if (stk_alphabet == QL_STK_ALPHABET_SET_UCS2)
            {
                hex_to_string(item_info.item_text.text, item_info.item_text.length, item_buffer);
            }
            else
            {
                memcpy(item_buffer, item_info.item_text.text, strlen((const char *)item_info.item_text.text));
            }
            if (MAIN_MENU == menu_type)
            {
                QL_STK_DEMO_LOG(" |%d : %s", id_arr[i], item_buffer);
            }
            else
            {
                QL_STK_DEMO_LOG("  ||%d : %s", id_arr[i], item_buffer);
            }
        }
    }

    free(str_buffer);
    str_buffer = NULL;
    free(item_buffer);
    item_buffer = NULL;
}

static void stk_setup_menu_info(uint8_t nSim)
{
    uint8_t *str_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX);
    if (str_buffer == NULL)
    {
        return;
    }
    menu_info.title.text = str_buffer;
    menu_info.title.length = QL_STK_STRING_LENGTH_MAX;

    /* get proactive command SET UP MENU */
    ql_stk_errcode_e err = ql_stk_acquire_setup_menu_info(nSim, &menu_info);
    if (QL_STK_SUCCESS != err)
    {
        QL_STK_DEMO_LOG("failed to get setup menu info, error:%d", err);
        return;
    }

    /* command details */
    QL_STK_DEMO_LOG("qualifier:%d", menu_info.qualifier);

    char *title_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX * 2 + 1);
    if (title_buffer == NULL)
    {
        return;
    }

    if (stk_alphabet == QL_STK_ALPHABET_SET_UCS2)
    {
        hex_to_string(menu_info.title.text, menu_info.title.length, title_buffer);
    }
    else
    {
        memcpy(title_buffer, menu_info.title.text, strlen((const char *)menu_info.title.text));
    }

    QL_STK_DEMO_LOG("------- Title:%s -------", title_buffer);

    free(str_buffer);
    str_buffer = NULL;
    free(title_buffer);
    title_buffer = NULL;

    stk_item_list_info(nSim, MAIN_MENU, menu_info.item_id, menu_info.num_items);
}

static void stk_select_item_info(uint8_t nSim)
{
    uint8_t *str_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX);
    if (str_buffer == NULL)
    {
        return;
    }
    submenu_info.title.text = str_buffer;
    submenu_info.title.length = QL_STK_STRING_LENGTH_MAX;

    /* get proactive command SELECT ITEM */
    ql_stk_errcode_e err = ql_stk_acquire_sub_menu_info(nSim, &submenu_info);
    if (QL_STK_SUCCESS != err)
    {
        QL_STK_DEMO_LOG("failed to get sub menu info, error:%d", err);
        return;
    }

    /* command details */
    QL_STK_DEMO_LOG("qualifier:%d",submenu_info.qualifier);

    char *title_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX * 2 + 1);
    if (title_buffer == NULL)
    {
        return;
    }

    if (stk_alphabet == QL_STK_ALPHABET_SET_UCS2)
    {
        hex_to_string(submenu_info.title.text, submenu_info.title.length, title_buffer);
    }
    else
    {
        memcpy(title_buffer, submenu_info.title.text, strlen((const char *)submenu_info.title.text));
    }

    QL_STK_DEMO_LOG("======= Title:%s =======", title_buffer);

    free(str_buffer);
    str_buffer = NULL;
    free(title_buffer);
    title_buffer = NULL;

    stk_item_list_info(nSim, SUB_MENU, submenu_info.item_id, submenu_info.num_items);
}

static void stk_display_text_info(uint8_t nSim)
{
    ql_stk_display_text_s display_text_info = {0};
    
    uint8_t *str_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX);
    if (str_buffer == NULL)
    {
        return;
    }
    display_text_info.text_string.text = str_buffer;
    display_text_info.text_string.length = QL_STK_STRING_LENGTH_MAX;

    /* DISPLAY TEXT */
    ql_stk_errcode_e err = ql_stk_acquire_display_text_info(nSim, &display_text_info);
    if (QL_STK_SUCCESS != err)
    {
        QL_STK_DEMO_LOG("failed to get sub menu info, error:%d", err);
        return;
    }

    /* command details */
    QL_STK_DEMO_LOG("qualifier:%d", display_text_info.qualifier);

    char *text_buffer = calloc(1, QL_STK_STRING_LENGTH_MAX * 2 + 1);
    if (text_buffer == NULL)
    {
        return;
    }

    if (stk_alphabet == QL_STK_ALPHABET_SET_UCS2)
    {
        hex_to_string(display_text_info.text_string.text, display_text_info.text_string.length, text_buffer);
    }
    else
    {
        memcpy(text_buffer, display_text_info.text_string.text, strlen((const char *)display_text_info.text_string.text));
    }

    QL_STK_DEMO_LOG("******* text:%s *******", text_buffer);

    free(str_buffer);
    str_buffer = NULL;
    free(text_buffer);
    text_buffer = NULL;
}

void stk_proactive_cmd_handler(uint8_t nSim, uint8_t proactive_cmd)
{
    QL_STK_DEMO_LOG("get proactive command:'%02x' on sim:%d", proactive_cmd, nSim);

    switch (proactive_cmd)
    {
    case QL_STK_PROACTIVE_CMD_SETUP_MENU: {
        stk_setup_menu_info(nSim);
        /* response proactive command with result '00' */
        ql_stk_send_command_result(nSim, proactive_cmd, QL_STK_PERFORMED_SUCCESSFULLY, 0, NULL);
        break;
    }

    case QL_STK_PROACTIVE_CMD_SELECT_ITEM:
        stk_select_item_info(nSim);

        /* select an item, send terminal response(ok) with item ID  */
        //ql_stk_send_command_result(nSim, proactive_cmd, QL_STK_PERFORMED_SUCCESSFULLY, submenu_info.item_id[0], NULL);
        ql_stk_send_command_result(nSim, QSTK_TERMINATE_STK_SESSION, QL_STK_PROACTIVE_SIM_SESSION_TERMINATED_BY_THE_USER, 0, NULL);
        break;

    case QL_STK_PROACTIVE_CMD_DISPLAY_TEXT:
        stk_display_text_info(nSim);
        ql_stk_send_command_result(nSim, proactive_cmd, QL_STK_PERFORMED_SUCCESSFULLY, 0, NULL);
        break;

    default:
        /* terminate current command */
        ql_stk_send_command_result(nSim, QSTK_TERMINATE_STK_SESSION, QL_STK_PROACTIVE_SIM_SESSION_TERMINATED_BY_THE_USER, 0, NULL);
        break;
    }
}

static void stk_app_thread(void *arg)
{
    QlOSStatus err = 0;
    uint8_t nSim = 0;
    ql_stk_errcode_e ret = QL_STK_SUCCESS;
    ql_stk_mode_e stk_mode = QL_STK_MODE_DISABLE;
    ql_stk_alphabet_set_e stk_alphabet_type = QL_STK_ALPHABET_SET_GSM;
    uint16_t stk_auto_response_timeout = 300;
    ql_stk_profile_s profile = {0};
    char profile_str[100] = {0};

    QL_STK_DEMO_LOG("========== stk demo start ==========");

    ql_stk_register_cb(user_stk_event_callback);

    ql_stk_cfg_get(&stk_mode, &stk_alphabet_type, &stk_auto_response_timeout);
    QL_STK_DEMO_LOG("get stk config mode:%d alphabet type:%d auto response timeout:%d",
                    stk_mode, stk_alphabet_type, stk_auto_response_timeout);

    if (QL_STK_MODE_DISABLE == stk_mode)
    {
        /* config stk function */
        ret = ql_stk_cfg_set(QL_STK_MODE_ENABLE, QL_STK_ALPHABET_SET_UCS2, QL_STK_AUTO_RESPONSE_TIMEOUT_DEF);
        QL_STK_DEMO_LOG("ql_stk_cfg_set ret:%d", ret);
        ql_power_reset(RESET_NORMAL);
        goto exit;
    }

    /* get TERMINAL PROFILE data */
    ql_stk_get_terminal_profile(nSim, &profile);
    hex_to_string(profile.profile_buf, profile.profile_len, profile_str);
    QL_STK_DEMO_LOG("profile:%s", profile_str);

    while (1)
    {
        ql_event_t event = {0};
        ql_event_try_wait(&event);
        QL_STK_DEMO_LOG("get event: 0x%08x/0x%08x/0x%08x/0x%08x", event.id, event.param1, event.param2, event.param3);

        switch (event.id)
        {
        case QUEC_STK_PROACTIVE_CMD_IND: {
            uint8_t nSim = event.param1;
            uint8_t proactive_cmd = event.param2;
            stk_proactive_cmd_handler(nSim, proactive_cmd);
            break;
        }

        case QUEC_STK_TIMEOUT_IND: {
            break;
        }

        case QUEC_STK_SIM_LOST_IND: {
            break;
        }

        case QUEC_STK_NO_PROACTIVE_CMD_IND: {
            uint8_t nSim = event.param1;
            static bool select_setup_menu = true; /* avoid loopback */
            ql_stk_state_e state;
            ql_stk_proactive_cmd_e cur_cmd;

            if ((true == select_setup_menu) && (0 != menu_info.item_id[0]))
            {
                select_setup_menu = false;

                ql_stk_get_stk_proactive_cmd(nSim, &cur_cmd, &state);
                QL_STK_DEMO_LOG("command:%d state:%d", cur_cmd, state);

                /**
                 * allow to select setup menu item while TERMINAL_RSP_STATE and none proactive command,
                 * this will cause a ENVELOPE (MENU SELECTION) command to SIM
                 * 
                 */
                if ((QL_STK_PROACTIVE_CMD_TERMINAL_RSP_STATE == state) && (QL_STK_PROACTIVE_CMD_NONE == cur_cmd))
                {
                    // depend on SIM's MENU
                    // QL_STK_DEMO_LOG("select setup menu item:%d", menu_info.item_id[0]);
                    // ql_stk_send_command_result(nSim, QSTK_SIM_ENVELOPE_MENU_SELECTION, QL_STK_PERFORMED_SUCCESSFULLY, menu_info.item_id[0], NULL);
                }
            }
            break;
        }
        default:
            break;
        }
        ql_rtos_task_sleep_s(1);
    }
exit:
    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_STK_DEMO_LOG("task deleted failed");
    }
    return;
}

int ql_stk_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&stk_task, 8 * 1024, 23, "stk_app", stk_app_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_STK_DEMO_LOG("stk_app init failed");
    }

    return err;
}
