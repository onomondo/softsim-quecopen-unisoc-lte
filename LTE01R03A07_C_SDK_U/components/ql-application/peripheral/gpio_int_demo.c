/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_pin_cfg.h"

#include "gpio_int_demo.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_GPIOINTDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_GPIOINTDEMO_LOG(msg, ...)         QL_LOG(QL_GPIOINTDEMO_LOG_LEVEL, "ql_GPIOINTDEMO", msg, ##__VA_ARGS__)
#define QL_GPIOINTDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_GPIOINTDEMO", msg, ##__VA_ARGS__)

/*===========================================================================
 * Variate
 ===========================================================================*/
ql_IntSel ql_intsel_flg = INT_CB01;

/*===========================================================================
 * Functions
 ===========================================================================*/
static void _gpioint_callback01(void *ctx)
{
    ql_LvlMode  gpio_lvl;

    ql_gpio_get_level(GPIO_0, &gpio_lvl);
    QL_GPIOINTDEMO_LOG("gpio[%d] int lvl:[%d]", GPIO_0, gpio_lvl);

    QL_GPIOINTDEMO_LOG("Welcome to Quectel!");
}

static void _gpioint_callback02(void *ctx)
{
    ql_LvlMode  gpio_lvl;

    ql_gpio_get_level(GPIO_1, &gpio_lvl);
    QL_GPIOINTDEMO_LOG("gpio[%d] int lvl:[%d]", GPIO_1, gpio_lvl);

    QL_GPIOINTDEMO_LOG("Quectel is No.1!");
}

static void ql_gpioint_demo_thread(void *param)
{
    QL_GPIOINTDEMO_LOG("gpio int demo thread enter, param 0x%x", param);

    ql_event_t event;

    ql_pin_set_func(QL_TEST1_PIN_GPIO0, QL_TEST1_PIN_GPIO0_FUNC_GPIO);      // set GPIO0
    ql_pin_set_func(QL_TEST1_PIN_GPIO1, QL_TEST1_PIN_GPIO1_FUNC_GPIO);      // set GPIO1

    //ql_gpio_deinit(GPIO_0);
    //ql_gpio_deinit(GPIO_1);

    ql_int_register(GPIO_0, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_RISING, PULL_DOWN, _gpioint_callback01, NULL);
    ql_int_register(GPIO_1, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_FALLING, PULL_UP, _gpioint_callback02, NULL);

    ql_int_enable(GPIO_0);
    ql_int_enable(GPIO_1);

    ql_rtos_task_sleep_s(20);

    while(1)
    {
        ql_event_wait(&event, 1);

        if( ql_intsel_flg == INT_CB01 )
        {
            if( ql_int_disable(GPIO_0) == 0 )
            {
                QL_GPIOINTDEMO_LOG("gpio[%d] int disable, enable gpio[%d] int", GPIO_0, GPIO_1);
                ql_int_enable(GPIO_1);
                //ql_int_register(GPIO_30, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_FALLING, PULL_UP, _gpioint_callback02, NULL);
                ql_intsel_flg = INT_CB02;
            }
        }
        else/* ql_intsel_flg == INT_CB02 */
        {
            if( ql_int_disable(GPIO_1) == 0 )
            {
                QL_GPIOINTDEMO_LOG("gpio[%d] int disable, enable gpio[%d] int", GPIO_1, GPIO_0);
                ql_int_enable(GPIO_0);
                //ql_int_register(GPIO_29, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_RISING, PULL_DOWN, _gpioint_callback01, NULL);
                ql_intsel_flg = INT_CB01;
            }
        }

        ql_rtos_task_sleep_s(20);
    }

    ql_rtos_task_delete(NULL);
}

void ql_gpioint_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_task_t gpioint_task = NULL;

    err = ql_rtos_task_create(&gpioint_task, 1024, APP_PRIORITY_NORMAL, "ql_gpiointdemo", ql_gpioint_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_GPIOINTDEMO_LOG("gpio int demo task created failed");
    }
}


