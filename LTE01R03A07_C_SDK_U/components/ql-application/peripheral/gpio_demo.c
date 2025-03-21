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

#include "gpio_demo.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_GPIODEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_GPIODEMO_LOG(msg, ...)         QL_LOG(QL_GPIODEMO_LOG_LEVEL, "ql_GPIODEMO", msg, ##__VA_ARGS__)
#define QL_GPIODEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_GPIODEMO", msg, ##__VA_ARGS__)

/*===========================================================================
 * Variate
 ===========================================================================*/
static ql_gpio_cfg _ql_gpio_cfg[] =
{   /* gpio_num   gpio_dir     gpio_pull    gpio_lvl    */
    {  GPIO_0,   GPIO_INPUT,   PULL_DOWN,   0xff     },   // set input pull-down
    {  GPIO_1,   GPIO_OUTPUT,  0xff,        LVL_HIGH }    // set output high-level
};

/*===========================================================================
 * Functions
 ===========================================================================*/
void _ql_gpio_demo_init(  void)
{
    uint16_t num;
    for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
    {
        ql_gpio_deinit(_ql_gpio_cfg[num].gpio_num);
        ql_gpio_init(_ql_gpio_cfg[num].gpio_num, _ql_gpio_cfg[num].gpio_dir, _ql_gpio_cfg[num].gpio_pull, _ql_gpio_cfg[num].gpio_lvl);
    }
}

static void ql_gpio_demo_thread(void *param)
{
    QL_GPIODEMO_LOG("gpio demo thread enter, param 0x%x", param);

    ql_event_t event;

    uint8_t change_flg = 0;
    uint16_t num;

    ql_GpioDir  gpio_dir;
    ql_PullMode gpio_pull;
    ql_LvlMode  gpio_lvl;

    /* init demo gpio array */
    ql_pin_set_func(QL_TEST1_PIN_GPIO0, QL_TEST1_PIN_GPIO0_FUNC_GPIO);      // TEST1_PIN set GPIO0
    ql_pin_set_func(QL_TEST1_PIN_GPIO1, QL_TEST1_PIN_GPIO1_FUNC_GPIO);      // TEST1_PIN set GPIO1

    ql_pin_set_func(QL_TEST2_PIN_GPIO0, QL_TEST2_PIN_GPIO0_FUNC_LCD_SIO);   // TEST2_PIN set spi_lcd_sio
    ql_pin_set_func(QL_TEST2_PIN_GPIO1, QL_TEST2_PIN_GPIO1_FUNC_LCD_SDC);   // TEST2_PIN set spi_lcd_sdc

    _ql_gpio_demo_init();

    /* get init info */
    for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
    {
        ql_gpio_get_direction(_ql_gpio_cfg[num].gpio_num, &gpio_dir);
        ql_gpio_get_pull(_ql_gpio_cfg[num].gpio_num, &gpio_pull);
        ql_gpio_get_level(_ql_gpio_cfg[num].gpio_num, &gpio_lvl);

        QL_GPIODEMO_LOG("gpio[%d] init", _ql_gpio_cfg[num].gpio_num);
        QL_GPIODEMO_LOG("gpio[%d] get dir:[%d], pull:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_pull, gpio_lvl);
    }
    ql_rtos_task_sleep_s(3);

    while(1)
    {
        ql_event_wait(&event, 1);

        /* output low test */
        gpio_dir  = GPIO_OUTPUT;
        gpio_lvl  = LVL_LOW;
        for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
        {
            /* set output low */
            ql_gpio_set_direction(_ql_gpio_cfg[num].gpio_num, gpio_dir);
            ql_gpio_set_level(_ql_gpio_cfg[num].gpio_num, gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] output low-level", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] set dir:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_lvl);

            /* get output low */
            ql_gpio_get_direction(_ql_gpio_cfg[num].gpio_num, &gpio_dir);
            ql_gpio_get_level(_ql_gpio_cfg[num].gpio_num, &gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] output low-level", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] get dir:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_lvl);
        }
        ql_rtos_task_sleep_s(3);

        /* output high test */
        gpio_dir  = GPIO_OUTPUT;
        gpio_lvl  = LVL_HIGH;
        for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
        {
            /* set output high */
            ql_gpio_set_direction(_ql_gpio_cfg[num].gpio_num, gpio_dir);
            ql_gpio_set_level(_ql_gpio_cfg[num].gpio_num, gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] output high-level", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] set dir:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_lvl);

            /* get output high */
            ql_gpio_get_direction(_ql_gpio_cfg[num].gpio_num, &gpio_dir);
            ql_gpio_get_level(_ql_gpio_cfg[num].gpio_num, &gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] output high-level", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] get dir:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_lvl);
        }
        ql_rtos_task_sleep_s(3);

        /* input pull-down test */
        gpio_dir  = GPIO_INPUT;
        gpio_pull = PULL_DOWN;
        for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
        {
            /* set input pull-down */
            ql_gpio_set_direction(_ql_gpio_cfg[num].gpio_num, gpio_dir);
            ql_gpio_set_pull(_ql_gpio_cfg[num].gpio_num, gpio_pull);

            QL_GPIODEMO_LOG("gpio[%d] input pull-down", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] set dir:[%d], pull:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_pull);

            /* get input pull-down */
            ql_gpio_get_direction(_ql_gpio_cfg[num].gpio_num, &gpio_dir);
            ql_gpio_get_pull(_ql_gpio_cfg[num].gpio_num, &gpio_pull);
            ql_gpio_get_level(_ql_gpio_cfg[num].gpio_num, &gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] input pull-down", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] get dir:[%d], pull:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_pull, gpio_lvl);
        }
        ql_rtos_task_sleep_s(3);

        /* input pull-up test */
        gpio_dir  = GPIO_INPUT;
        gpio_pull = PULL_UP;
        for( num = 0; num < sizeof(_ql_gpio_cfg)/sizeof(_ql_gpio_cfg[0]); num++ )
        {
            /* set input pull-up */
            ql_gpio_set_direction(_ql_gpio_cfg[num].gpio_num, gpio_dir);
            ql_gpio_set_pull(_ql_gpio_cfg[num].gpio_num, gpio_pull);

            QL_GPIODEMO_LOG("gpio[%d] input pull-up", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] set dir:[%d], pull:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_pull);

            /* get input pull-up */
            ql_gpio_get_direction(_ql_gpio_cfg[num].gpio_num, &gpio_dir);
            ql_gpio_get_pull(_ql_gpio_cfg[num].gpio_num, &gpio_pull);
            ql_gpio_get_level(_ql_gpio_cfg[num].gpio_num, &gpio_lvl);

            QL_GPIODEMO_LOG("gpio[%d] input pull-up", _ql_gpio_cfg[num].gpio_num);
            QL_GPIODEMO_LOG("gpio[%d] get dir:[%d], pull:[%d], lvl:[%d]", _ql_gpio_cfg[num].gpio_num, gpio_dir, gpio_pull, gpio_lvl);
        }
        ql_rtos_task_sleep_s(3);

        /* change GPIO's Pin */
        if( change_flg == 0 )
        {
            ql_pin_set_func(QL_TEST1_PIN_GPIO0, QL_TEST1_PIN_GPIO0_FUNC_FLASH1_CLK);    // TEST1_PIN set spi_flash1_clk
            ql_pin_set_func(QL_TEST1_PIN_GPIO1, QL_TEST1_PIN_GPIO1_FUNC_FLASH1_CS);     // TEST1_PIN set spi_flash1_cs

            ql_pin_set_func(QL_TEST2_PIN_GPIO0, QL_TEST2_PIN_GPIO0_FUNC_GPIO);          // TEST2_PIN set GPIO0
            ql_pin_set_func(QL_TEST2_PIN_GPIO1, QL_TEST2_PIN_GPIO1_FUNC_GPIO);          // TEST2_PIN set GPIO1

            QL_GPIODEMO_LOG("GPIO0/1 pin is TEST1 -> TEST2");
            change_flg = 1;
        }
        else
        {
            ql_pin_set_func(QL_TEST1_PIN_GPIO0, QL_TEST1_PIN_GPIO0_FUNC_GPIO);          // TEST1_PIN set GPIO0
            ql_pin_set_func(QL_TEST1_PIN_GPIO1, QL_TEST1_PIN_GPIO1_FUNC_GPIO);          // TEST1_PIN set GPIO1

            ql_pin_set_func(QL_TEST2_PIN_GPIO0, QL_TEST2_PIN_GPIO0_FUNC_LCD_SIO);       // TEST2_PIN set spi_lcd_sio
            ql_pin_set_func(QL_TEST2_PIN_GPIO1, QL_TEST2_PIN_GPIO1_FUNC_LCD_SDC);       // TEST2_PIN set spi_lcd_sdc

            QL_GPIODEMO_LOG("GPIO0/1 pin is TEST2 -> TEST1");
            change_flg = 0;
        }
    }

    ql_rtos_task_delete(NULL);
}

void ql_gpio_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_task_t gpio_task = NULL;

    err = ql_rtos_task_create(&gpio_task, 1024, APP_PRIORITY_NORMAL, "ql_gpiodemo", ql_gpio_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_GPIODEMO_LOG("gpio demo task created failed");
    }
}


