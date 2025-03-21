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

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_sim.h"
#include "ql_sim_demo.h"
#include "ql_pin_cfg.h"

#define QL_SIM_LOG_LEVEL                    QL_LOG_LEVEL_INFO
#define QL_SIM_LOG(msg, ...)                QL_LOG(QL_SIM_LOG_LEVEL, "ql_sim_demo", msg, ##__VA_ARGS__)

#define QL_SIM_TASK_STACK_SIZE              (1024*4)
#define QL_SIM_TASK_PRIO                    APP_PRIORITY_NORMAL
#define QL_SIM_TASK_EVENT_CNT               5

ql_timer_t 	sim_write_timer = NULL;

static void user_sim_hotplug_callback(uint8_t physical_sim_id, ql_sim_hotplug_status_e status)
{
    if (QL_SIM_HOTPLUG_STATUS_IN == status)
    {
        QL_SIM_LOG("sim=%d,status=%d:plug in !!!", physical_sim_id, status);
    }
    else
    {
        QL_SIM_LOG("sim=%d,status=%d:plug out !!!", physical_sim_id, status);
    }
}


#if 0
void ql_sim_dsss_demo(void)
{
    ql_sim_errcode_e ret = QL_SIM_SUCCESS;
    char siminfo[64] = {0};
    ql_sim_status_e card_status = 0;

    uint8_t operate_id = ql_sim_get_operate_id();
    QL_SIM_LOG("ql_sim_get_operate_id:%d", operate_id);

    ret = ql_sim_get_card_status(0, &card_status);
    QL_SIM_LOG("ql_sim_get_card_status ret:0x%x, card_status: %d", ret, card_status);

    if (QL_SIM_STATUS_READY == card_status)
    {
        /* get imsi from current SIM, simID should be 0 in DSSS solution */
        ret = ql_sim_get_imsi(0, siminfo, 64);
        QL_SIM_LOG("ql_sim_get_imsi ret:0x%x, IMSI: %s", ret, siminfo);
    }

    for (size_t i = 0; i < 10; i++)
    {
        operate_id ^= 1;
        ret = ql_sim_set_operate_id(operate_id);
        QL_SIM_LOG("ql_sim_set_operate_id:%d ret:0x%x", operate_id, ret);

        /* should wait for PIN ready */
        ql_rtos_task_sleep_ms(5000);

        ret = ql_sim_get_card_status(0, &card_status);
        QL_SIM_LOG("ql_sim_get_card_status ret:0x%x, card_status: %d", ret, card_status);

        if (QL_SIM_STATUS_READY == card_status)
        {
            /* get imsi from current SIM, simID should be 0 in DSSS solution */
            ret = ql_sim_get_imsi(0, siminfo, 64);
            QL_SIM_LOG("ql_sim_get_imsi ret:0x%x, IMSI: %s", ret, siminfo);
        }
    }
}
#endif // QL_APP_FEATURE_DSSS

static void ql_sim_write_timer_handler(void *param)
{
    ql_sim_write_count_s count = {0};

    if(ql_sim_get_write_count(0, &count) == QL_SIM_SUCCESS)
    {
        QL_SIM_LOG("attemptCount=%d, realCount=%d",count.attemptCount, count.realCount);
    }
}

static void ql_sim_demo_thread(void *param)
{
	QlOSStatus err = QL_OSI_SUCCESS;
    ql_sim_errcode_e ret = QL_SIM_SUCCESS;
    char siminfo[64] = {0};
	uint8_t sim_id = 0;
    uint8_t physical_sim_id = 0;

    for (int n = 0; n < 5; n++)
    {
        QL_SIM_LOG("hello sim demo %d", n);
        ql_rtos_task_sleep_ms(2000);
    }

    ql_rtos_timer_create(&sim_write_timer, QL_TIMER_IN_SERVICE, ql_sim_write_timer_handler, NULL);
    ql_rtos_timer_start(sim_write_timer, 5000, true);


#ifdef QL_APP_FEATURE_DSSS
    //ql_sim_dsss_demo();
#endif // QL_APP_FEATURE_DSSS

	ret = ql_sim_get_imsi(sim_id, siminfo, 64);
	QL_SIM_LOG("ret:0x%x, IMSI: %s", ret, siminfo);

    ret = ql_sim_get_iccid(sim_id, siminfo, 64);
    QL_SIM_LOG("ret:0x%x, ICCID: %s", ret, siminfo);

    ret = ql_sim_get_phonenumber(sim_id, siminfo, 64);
    QL_SIM_LOG("ret:0x%x, phonenumber: %s", ret, siminfo);
#if 0
    ql_sim_status_e card_status = 0;
    ql_sim_verify_pin_info_s   pin = {0};
    ql_sim_unblock_pin_info_s  pin_un = {0};
    ql_sim_change_pin_info_s   pin_change = {0};
    /*
    !!!be careful to enable sim pin!!!
    */
    //enable pin verify and effective after reboot
    memset(&pin, 0, sizeof(ql_sim_verify_pin_info_s));	
	memcpy(pin.pin_value,"1234",5);
	ret = ql_sim_enable_pin(sim_id, &pin);
	QL_SIM_LOG("ql_sim_enable_pin ret:0x%x", ret);

    ret = ql_sim_get_card_status(sim_id, &card_status);
    QL_SIM_LOG("ret:0x%x, card_status: %d", ret, card_status);

    if(QL_SIM_STATUS_SIMPIN == card_status)
    {
        memset(&pin, 0, sizeof(ql_sim_verify_pin_info_s));
        memcpy(pin.pin_value, "1234", 5);
        ret = ql_sim_verify_pin(sim_id, &pin);
        QL_SIM_LOG("ql_sim_verify_pin ret:0x%x", ret);

        ret = ql_sim_get_card_status(sim_id, &card_status);
        QL_SIM_LOG("ret:0x%x, card_status: %d", ret, card_status);
    }
    else if(QL_SIM_STATUS_SIMPUK == card_status)
    {
        memset(&pin_un, 0, sizeof(ql_sim_unblock_pin_info_s));
        memcpy(pin_un.puk_value, "26601934", 9);
        memcpy(pin_un.new_pin_value, "1234", 5);
        ret = ql_sim_unblock_pin(sim_id, &pin_un);
        QL_SIM_LOG("ql_sim_unblock_pin ret:0x%x", ret);

        ret = ql_sim_get_card_status(sim_id, &card_status);
        QL_SIM_LOG("ret:0x%x, card_status: %d", ret, card_status);
    }

    memset(&pin_change, 0, sizeof(ql_sim_change_pin_info_s));
    memcpy(pin_change.old_pin_value, "1234", 5);
    memcpy(pin_change.new_pin_value, "4321", 5);
    ret = ql_sim_change_pin(sim_id, &pin_change);
    QL_SIM_LOG("ql_sim_change_pin ret:0x%x", ret);

    memset(&pin_change, 0, sizeof(ql_sim_change_pin_info_s));
    memcpy(pin_change.old_pin_value, "4321", 5);
    memcpy(pin_change.new_pin_value, "1234", 5);
    ret = ql_sim_change_pin(sim_id, &pin_change);
    QL_SIM_LOG("ql_sim_change_pin ret:0x%x", ret);

    memset(&pin,0,sizeof(ql_sim_verify_pin_info_s));	
	memcpy(pin.pin_value,"1234",5);
	ret = ql_sim_disable_pin(sim_id, &pin);
	QL_SIM_LOG("ql_sim_disable_pin ret:0x%x", ret);
#endif
	//sim hot plug
    ql_pin_set_func(QL_PIN_SIM0_HOTPLUG,QL_PIN_SIM0_HOTPLUG_FUNC_GPIO);
    ql_sim_hotplug_register_cb(user_sim_hotplug_callback);

    ql_sim_hotplug_gpio_s gpio_cfg={QL_GPIO_SIM0_HOTPLUG,LVL_LOW};
    ret = ql_sim_set_hotplug(   physical_sim_id,
                                QL_SIM_HOTPLUG_ENABLE,
                                &gpio_cfg);
    QL_SIM_LOG("ql_sim1_set_hotplug ret:0x%x", ret);

#if 0
    // sim send apdu
    ql_sim_apdu_t *apdu = malloc(sizeof(ql_sim_apdu_t));
    if (apdu)
    {
        memset(apdu, 0, sizeof(ql_sim_apdu_t));

        uint8_t send_apdu[] = {0x80, 0xf2, 0x00, 0x01, 0x2E};
        memcpy(apdu->req_apdu, send_apdu, sizeof(send_apdu) / sizeof(send_apdu[0]));
        apdu->req_apdu_len = sizeof(send_apdu) / sizeof(send_apdu[0]);

        OSI_LOGXI(OSI_LOGPAR_M, 0, "APDU REQ: %*s", apdu->req_apdu_len, apdu->req_apdu);
        ret = ql_sim_send_apdu(sim_id, QL_SIM_APDU_CHANNEL_DEF, apdu);
        QL_SIM_LOG("ql_sim_send_apdu ret:0x%x", ret);

        if ((QL_SIM_SUCCESS == ret) && (apdu->resp_apdu_len >= 2))
        {
            QL_SIM_LOG("sw1sw2:%#02x%02x len:%d",
                       apdu->resp_apdu[apdu->resp_apdu_len - 2],
                       apdu->resp_apdu[apdu->resp_apdu_len - 1], apdu->resp_apdu_len);

            OSI_LOGXI(OSI_LOGPAR_M, 0, "APDU RSP: %*s", apdu->resp_apdu_len, apdu->resp_apdu);
        }
        free(apdu);
    }
#endif

    err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SIM_LOG("task deleted failed");
	}
}

void ql_sim_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t sim_task = NULL;
	
	err = ql_rtos_task_create(&sim_task, QL_SIM_TASK_STACK_SIZE, QL_SIM_TASK_PRIO, "QSIMDEMO", ql_sim_demo_thread, NULL, QL_SIM_TASK_EVENT_CNT);
	if (err != QL_OSI_SUCCESS)
	{
		QL_SIM_LOG("task created failed");
	}
}

