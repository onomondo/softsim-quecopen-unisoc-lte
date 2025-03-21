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
23/07/2022       Joe.tu          create

=================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_common.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#include "ql_api_vsim_adapt.h"

#define QL_VSIM_ADAPT_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_VSIM_ADAPT_DEMO_LOG(msg, ...)			QL_LOG(QL_VSIM_ADAPT_LOG_LEVEL, "vsim_adapt_demo", msg, ##__VA_ARGS__)

static ql_task_t vsim_adapt_task = NULL;

static int prv_process_apdu(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp, uint16_t *apdu_rsp_len, uint8_t slot)
{
    const char apdu[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB,
                         0xAA, 0x99, 0x88, 0x77, 0x66,
                         0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0x90, 0x00}; // response APDU should contain SW1 and SW2
    *apdu_rsp_len = sizeof(apdu);
    memcpy(apdu_rsp, apdu, *apdu_rsp_len);
    QL_VSIM_ADAPT_DEMO_LOG("return apdu:%d", *apdu_rsp_len);
    return 0x9000; /* return SW1 SW2 */
}

static uint16_t prv_process_reset(uint8_t *atr_data, uint8_t *atr_size, uint8_t nSimID)
{
    const char atr[] = {0x00, 0x11, 0x22, 0x33, 0x44,
                        0x55, 0x66, 0x77, 0x88, 0x99,
                        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

    uint8_t size = sizeof(atr);
    if (size > *atr_size)
        return -1;

    memcpy(atr_data, atr, *atr_size);
    QL_VSIM_ADAPT_DEMO_LOG("return atr:%d", *atr_size);
    return 0; /* return 0 on success */
}

ql_vsim_adapt_handler_s apapt_handler = {.process_apdu = prv_process_apdu,
                                         .reset = prv_process_reset};

/* Note: to implement a local soft sim or others at startup, should register vsim adapt callback before atEngineStart */


static void vsim_adapt_app_thread(void *arg)
{
    QlOSStatus err = 0;
    uint8_t nSim = 0;
    ql_vsim_adapt_errcode_e ret = 0;
    ql_vsim_adapt_sim_type_e sim_type = QL_VSIM_ADAPT_SIM_TYPE_TSIM;

    ql_rtos_task_sleep_s(10);
    QL_VSIM_ADAPT_DEMO_LOG("==========vsim adapt demo start ==========");

    sim_type = ql_vsim_adapt_get_sim_type(nSim);
    QL_VSIM_ADAPT_DEMO_LOG("get sim_type:%d", sim_type);
    if (QL_VSIM_ADAPT_SIM_TYPE_MAX == sim_type)
    {
        QL_VSIM_ADAPT_DEMO_LOG("get sim_type error");
        goto exit;
    }

    ret = ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, &apapt_handler, nSim);
    if (QL_VSIM_ADAPT_SUCCESS != ret)
    {
        goto exit;
    }

    QL_VSIM_ADAPT_DEMO_LOG("==============vsim adapt demo end================");
exit:
    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_ADAPT_DEMO_LOG("task deleted failed");
    }
}

int ql_vsim_adapt_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&vsim_adapt_task, 4 * 1024, APP_PRIORITY_NORMAL, "QVIMADAPT", vsim_adapt_app_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_ADAPT_DEMO_LOG("vsim_adapt_app init failed");
    }

    return err;
}
