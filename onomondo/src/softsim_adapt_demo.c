/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Author: Onomondo ApS
 */

/**
 * @file softsim_adapt_demo.c
 * @brief This file contains the implementation of the SoftSIM adaptation demo for Onomondo.
 *
 * It includes functions for processing APDU requests, handling power-on indications,
 * and initializing the VSIM adapter. The purpose of this file is to demonstrate the
 * integration of SoftSIM functionalities with the Quectel SDK.
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Quectel includes
#include "ql_log.h"
#include "ql_api_osi.h"
#include "ql_api_common.h"
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
#define QL_VSIM_ADAPT_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_VSIM_ADAPT_DEMO_LOG(msg, ...)    QL_LOG(QL_VSIM_ADAPT_LOG_LEVEL, "vsim_adapt_demo", msg, ##__VA_ARGS__)

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
static ql_task_t vsim_adapt_task = NULL;
static ss_apdu_response_t apdu_rsp_buf;
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
    size_t request_len = apdu_req_len;
    apdu_rsp_buf.len = ss_application_apdu_transact(apductx, apdu_rsp_buf.data, APDU_MAX_LEN + 2, apdu_req, &request_len);

    if (apdu_rsp_buf.len < APDU_MIN_RESPONSE_LEN) {
        QL_VSIM_ADAPT_DEMO_LOG("APDU response too short: %zu", apdu_rsp_buf.len);
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
    *atr_size = 25;
    size_t atr_len = ss_atr(apductx, atr_data, *atr_size);

    if (atr_len == 0)
    {
        QL_VSIM_ADAPT_DEMO_LOG("ATR ERROR");
        return -1;
    }
    *atr_size = atr_len;
    return 0;
}

ql_vsim_adapt_handler_s adapt_handler = {.process_apdu = prv_process_apdu,
                                         .reset = prv_process_reset};


/**
 * @brief Handle the power-on indication for the VSIM adapter.
 *
 * This function is called when a power-on indication is received. It initializes
 * the SoftSIM application, checks the validity of the SoftSIM filesystem, and sets
 * the SIM type to SSIM.
 *
 * @param ind_type The type of indication received.
 * @param ctx The context for the indication.
 * @return 0 on success.
 */
int vsim_adapt_poweron_enter(uint32_t ind_type, void *ctx)
{
    switch (ind_type)
    {
    case QUEC_VSIM_ADAPT_POWERON_IND:
        QL_VSIM_ADAPT_DEMO_LOG("VSIM SoftSIM Application Initialization");

        if (!softsim_dir_is_valid())
        {
            QL_VSIM_ADAPT_DEMO_LOG("Recreating SoftSIM Filesystem");
            recreate_fs();
        }

        apductx = ss_new_ctx();

        if (!apductx) {
            QL_VSIM_ADAPT_DEMO_LOG("Failed to allocate SoftSIM context");
            return -1;
        }

        ss_reset(apductx);

        if (ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, &adapt_handler, VSIM_SIM_NUMBER) != QL_VSIM_ADAPT_SUCCESS) {
            QL_VSIM_IMG_LOG("Failed to set SIM type");
            return -1;
        }
        break;

    default:
        break;
    }
    return 0;
}


static void vsim_adapt_app_thread(void *arg)
{
    ql_vsim_adapt_errcode_e ret = 0;
    ql_vsim_adapt_sim_type_e sim_type = QL_VSIM_ADAPT_SIM_TYPE_TSIM;

    ql_rtos_task_sleep_s(10);
    QL_VSIM_ADAPT_DEMO_LOG("==========vsim adapt demo start ==========");

    sim_type = ql_vsim_adapt_get_sim_type(VSIM_SIM_NUMBER);
    QL_VSIM_ADAPT_DEMO_LOG("get sim_type:%d", sim_type);
    if (QL_VSIM_ADAPT_SIM_TYPE_MAX == sim_type)
    {
        QL_VSIM_ADAPT_DEMO_LOG("get sim_type error");
        goto exit;
    }

    ret = ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, &adapt_handler, VSIM_SIM_NUMBER);
    if (QL_VSIM_ADAPT_SUCCESS != ret)
    {
        goto exit;
    }

    QL_VSIM_ADAPT_DEMO_LOG("==============vsim adapt demo end================");
exit:
    QlOSStatus err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_ADAPT_DEMO_LOG("task deleted failed");
    }
}


int ql_vsim_adapt_init(void)
{
    QlOSStatus err = ql_rtos_task_create(&vsim_adapt_task, VSIM_TASK_STACK_SIZE, APP_PRIORITY_NORMAL, "QVIMADAPT", vsim_adapt_app_thread, NULL, VSIM_TASK_PRIORITY);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_ADAPT_DEMO_LOG("vsim_adapt_app init failed");
    }

    return err;
}
