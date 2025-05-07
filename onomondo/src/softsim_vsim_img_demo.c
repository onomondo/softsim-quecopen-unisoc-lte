/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Author: Onomondo ApS
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Quectel includes
#include "ql_log.h"
#include "ql_api_osi.h"
#include "ql_api_vsim_adapt.h"
#include "quec_at_engine.h"
#include "quec_at_param.h"
#include "quec_atresp.h"

// Onomondo includes
#include "onomondo/softsim/softsim.h"
#include "onomondo/softsim/mem.h"
#include "onomondo/softsim/utils.h"
#include "onomondo/softsim/fs.h"
#include "onomondo/utils/ss_profile.h"
#include "ss_provisioning.h"
#include "ss_bootstrap.h"
#include "ss_fs_utils.h"

// Logging configuration
#define QL_VSIM_IMG_MODULE          "ql_vsim_img"
#define QL_VSIM_IMG_LOG_LEVEL       QL_LOG_LEVEL_INFO
#define QL_VSIM_IMG_LOG(...)        QL_LOG(QL_VSIM_IMG_LOG_LEVEL, QL_VSIM_IMG_MODULE, ##__VA_ARGS__)
#define QL_VSIM_IMG_LOG_PUSH(...)   QL_LOG_PUSH(QL_VSIM_IMG_MODULE, ##__VA_ARGS__)

// Constants
#define VSIM_SIM_NUMBER                 0
#define VSIM_TASK_PRIORITY              5
#define VSIM_TASK_STACK_SIZE            (4 * 1024)
#define SOFTSIM_ERROR_INVALID_PARAMS    -2

// APDU constants
#define APDU_MAX_LEN            256
#define APDU_MIN_RESPONSE_LEN   2
#define APDU_ERROR_UNSPECIFIED  0x6A82
#define ATR_DEFAULT_SIZE        25

// APDU response structure
typedef struct {
    uint8_t data[APDU_MAX_LEN];
    size_t len;
} ss_apdu_response_t;

// Global state
static ql_task_t vsim_task = NULL;
static ss_apdu_response_t apdu_rsp_buf = {0};
static struct ss_context *apductx = NULL;


/**
 * @brief Process an APDU request and generate a response.
 *
 * @param apdu_req Pointer to the APDU request data.
 * @param apdu_req_len Length of the APDU request data.
 * @param apdu_rsp Pointer to the buffer where the APDU response will be stored.
 * @param apdu_rsp_len Pointer to the variable where the length of the APDU response will be stored.
 * @param slot The slot number for the SIM.
 * @return Status code of the APDU processing.
 */
static int prv_process_apdu(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp, uint16_t *apdu_rsp_len, uint8_t slot)
{
    if (!apdu_req || !apdu_rsp || !apdu_rsp_len || !apductx) {
        QL_VSIM_IMG_LOG("Invalid parameters in prv_process_apdu");
        return SOFTSIM_ERROR_INVALID_PARAMS;
    }

    size_t request_len = apdu_req_len;

    apdu_rsp_buf.len = ss_application_apdu_transact(apductx, apdu_rsp_buf.data, APDU_MAX_LEN + 2, apdu_req, &request_len);

    if (apdu_rsp_buf.len < APDU_MIN_RESPONSE_LEN) {
        QL_VSIM_IMG_LOG("APDU response too short: %zu", apdu_rsp_buf.len);
        return APDU_ERROR_UNSPECIFIED;
    }

    // Extract response code from last two bytes
    int rsp = (apdu_rsp_buf.data[apdu_rsp_buf.len - 2] << 8) |
               apdu_rsp_buf.data[apdu_rsp_buf.len - 1];

    // Copy response data
    memcpy(apdu_rsp, apdu_rsp_buf.data, apdu_rsp_buf.len);
    *apdu_rsp_len = apdu_rsp_buf.len;

    return rsp;
}

/**
 * @brief Process the reset and retrieve ATR (Answer To Reset) data.
 *
 * @param atr_data Pointer to the buffer where ATR data will be stored.
 * @param atr_size Pointer to the size of the ATR data buffer. Will be updated with the actual size of the ATR data.
 * @param nSimID The SIM ID to reset.
 * @return 0 on success, -1 on error.
 */
static uint16_t prv_process_reset(uint8_t *atr_data, uint8_t *atr_size, uint8_t nSimID)
{
    if (!atr_data || !atr_size || !apductx) {
        QL_VSIM_IMG_LOG("Invalid parameters in prv_process_reset");
        return SOFTSIM_ERROR_INVALID_PARAMS;
    }

    *atr_size = ATR_DEFAULT_SIZE;
    size_t atr_len = ss_atr(apductx, atr_data, *atr_size);

    if (atr_len == 0)
    {
        QL_VSIM_IMG_LOG("ATR ERROR");
        return -1;
    }
    *atr_size = atr_len;
    return 0;
}


ql_vsim_adapt_handler_s adapt_handler = {.process_apdu = prv_process_apdu,
                                         .reset = prv_process_reset};


static void prvInvokeGlobalCtors(void)
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();

    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; ++i)
        __init_array_start[i]();
}


static void vsim_app_thread(void *arg)
{
    QL_VSIM_IMG_LOG("VSIM IMG - SoftSIM Thread");

    QlOSStatus err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_IMG_LOG("task deleted failed");
    }
}

/**
 * @brief Handle the VSIMDEMO command.
 * @param cmd AT command structure
 */
void ql_exec_vsimdemo_cmd(atCommand_t *cmd)
{
    if (cmd->type == AT_CMD_SET)
    {
        bool paramok = true;
        const char *input_str = quec_atParamStr(cmd->params[0], &paramok);
        if ((cmd->param_count != 1) || !paramok)
        {
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_PARAM_INVALID);
        }
        quec_atCmdRespInfoNText(cmd->engine, input_str, strlen(input_str), 1);

        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else if (cmd->type == AT_CMD_READ)
    {
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, "onomondo-uicc vsimdemo read resp", 1);
    }
    else if (cmd->type == AT_CMD_TEST)
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPTION_NOT_SURPORT);
    }
}

const char ql_ver_date[] = __DATE__;
const char ql_ver_time[] = __TIME__;
const char ql_ver_authors[] = "Authors: Onomondo";

/**
 * @brief Handle the QLVER command.
 * @param cmd AT command structure
 */
void ql_exec_ver_cmd(atCommand_t *cmd)
{
    char *verInfo = NULL;
	int verinfo_len;

	verinfo_len = strlen(QL_CORE_VERSION)+strlen(ql_ver_date)+strlen(ql_ver_time)+strlen(ql_ver_authors)+20;
	verInfo = (char *)malloc(verinfo_len);
	if (NULL == verInfo)
	{
		quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR);
		return;
	}

	memset(verInfo, 0, verinfo_len);
    /* get ver information */
    sprintf(verInfo, "VERSION: %s\r\n%s %s\r\n%s", QL_CORE_VERSION, ql_ver_date, ql_ver_time, ql_ver_authors);

    /*return the result code and response to the terminal */
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, verInfo, 1);

	free(verInfo);
	return;
}

/**
 * @brief Handle SoftSIM filesystem profile provisioning command
 * @param cmd AT command structure
 */
void ql_exec_softsim_cmd(atCommand_t *cmd)
{
    char *softsim_guide_txt = "+SOFTSIM=\"<profile>\" ";

    if (cmd->type == AT_CMD_SET && cmd->param_count == 1)
    {
        bool paramok = true;
        const char *data_str = quec_atParamStr(cmd->params[0], &paramok);

        if(onomondo_profile_provisioning(data_str) == 0)
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
        else
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_ERROR, ERR_AT_CME_SIM_FAILURE);
    }
    else if (cmd->type == AT_CMD_TEST)
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, softsim_guide_txt, 1);
    else
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_BURN_FLASH_FAIL);
}

/**
 * @brief Handle SoftSIM filesystem reset command
 * @param cmd AT command structure
 */
void ql_exec_softsim_reset_cmd(atCommand_t *cmd)
{
    char *softsim_reset_guide_txt = "+SOFTSIMKILLFS=1";

    if (cmd->type == AT_CMD_SET && cmd->param_count == 1)
    {
        bool paramok = true;
        int data = quec_atParamInt(cmd->params[0], &paramok);

        if (data == 1)
        {
            recreate_fs();
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
        }
        else
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_ERROR, ERR_AT_CME_OPTION_NOT_SURPORT);
    }
    else if (cmd->type == AT_CMD_TEST)
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, softsim_reset_guide_txt, 1);
    else
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPTION_NOT_SURPORT);
}

/**
 * @brief Handles the VSIM power-on events.
 *
 * This function is responsible for managing the events that occur when the
 * Virtual SIM (VSIM) is powered on.
 *
 * @param ind_type The type of event that triggered the power-on.
 * @param ctx A pointer to the context associated with the event.
 * @return 0 on success.
 */
int vsim_poweron_enter(uint32_t ind_type, void *ctx)
{
    switch (ind_type)
    {
    case QUEC_VSIM_ADAPT_POWERON_IND:
        QL_VSIM_IMG_LOG("SoftSIM Application Initialization");

        if (!softsim_dir_is_valid()) {
            QL_VSIM_IMG_LOG("Recreating SoftSIM Filesystem");
            if (recreate_fs() != 0)
            {
                QL_VSIM_IMG_LOG("Failed to recreate SoftSIM filesystem");
                return -1;
            }
        }

        apductx = ss_new_ctx();

        if (!apductx) {
            QL_VSIM_IMG_LOG("Failed to allocate SoftSIM context");
            return -1;
        }

        ss_reset(apductx);

        if (ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, &adapt_handler, VSIM_SIM_NUMBER) != QL_VSIM_ADAPT_SUCCESS) {
            QL_VSIM_IMG_LOG("Failed to set SIM type");
            return -1;
        }
        break;

    case QUEC_VSIM_ADAPT_RDY_IND: {
        QlOSStatus err = ql_rtos_task_create(&vsim_task, VSIM_TASK_STACK_SIZE, APP_PRIORITY_NORMAL, "VSIM", vsim_app_thread, NULL, 5);
        if (err != QL_OSI_SUCCESS) {
            QL_VSIM_IMG_LOG("vsim_adapt_app init failed");
        }
        break;
    }

    default:
        QL_VSIM_IMG_LOG("Unknown indication type: %u", ind_type);
        break;
    }
    return 0;
}


ql_at_desc_t app_at_desc[] = {
    {"+VSIMDEMO", ql_exec_vsimdemo_cmd, 0},

    {"+QLVER", ql_exec_ver_cmd, 0},

    {"+SOFTSIM", ql_exec_softsim_cmd, 0},

    {"+SOFTSIMKILLFS", ql_exec_softsim_reset_cmd, 0},

    //==>Warning: Please add new vsim AT cmd upper this line!!
    {NULL, NULL, 0}};


int appimg_enter(void *param)
{
    QL_VSIM_IMG_LOG("vSIM IMG - SoftSIM image demo enter");
    prvInvokeGlobalCtors();

    /* register AT lookup table */
    quec_app_at_add((const ql_at_desc_t *)app_at_desc, sizeof(app_at_desc)/sizeof(app_at_desc[0]));

    /* register VSIM event handler */
    ql_vsim_adapt_register_callback(vsim_poweron_enter);
    return 0;
}


void appimg_exit(void)
{
    QL_VSIM_IMG_LOG("vSIM IMG - SoftSIM init demo exit");
}
