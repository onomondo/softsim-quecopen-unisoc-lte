#include <stdio.h>
#include <unistd.h>

#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "ohos_init.h"

#define LED_TASK_STACK_SIZE (1024 * 4)
#define LED_TASK_PRIO 25
#define LED_GPIO 1

/**
 * @brief led task output high and low levels to turn on and off LED
 *
 */
static void LedTask(void)
{
    // init gpio of LED
    IoTGpioInit(LED_GPIO);

    // set GPIO_2 is output mode
    IoTGpioSetDir(LED_GPIO, IOT_GPIO_DIR_OUT);

    while (1) {
        // set GPIO_2 output high levels to turn on LED
        IoTGpioSetOutputVal(LED_GPIO, 1);

        // delay 1s
        osDelay(50);
        printf("jamie test 1!\n");

        // set GPIO_2 output low levels to turn off LED
        IoTGpioSetOutputVal(LED_GPIO, 0);

        // delay 1s
        osDelay(50);
        printf("jamie test 2!\n");
    }

    osThreadExit();
}

/**
 * @brief Main Entry of the Led Example
 *
 */
static void LedExampleEntry(void)
{
    osThreadAttr_t attr;

    printf("jamie test LedTask!\n");

    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)LedTask, NULL, &attr) == NULL) {
        printf("Failed to create LedTask!\n");
    }
}

SYS_RUN(LedExampleEntry);