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
#include "ql_i2c.h"
#include "ql_gpio.h"
#include "ql_log.h"


//客户需要自己配置，GPIO使用的引脚
#define QL_ALIPAY_CERT_GPIO_PIN             118
#define QL_ALIPAY_CERT_GPIO_PIN_FUNC        0
#define QL_ALIPAY_CERT_GPIO_NUM             GPIO_7

#define QL_ALIPAY_CERT_I2C_SCL             11
#define QL_ALIPAY_CERT_I2C_SDA             12
#define QL_ALIPAY_CERT_I2C_FUNC            0


bool ql_alipay_gpio_init = false;
//客户需要自己配置I2C的引脚，以及选择哪组I2C引脚
ql_i2c_channel_e ql_alipay_i2c_no = i2c_1;

unsigned char * IIC_Master_Init(void)
{
    unsigned char *ql_i2c_data = NULL;
    ql_i2c_data = (unsigned char *)malloc(2048);
    if (ql_i2c_data)
    {
        if (QL_GPIO_SUCCESS != ql_pin_set_func(QL_ALIPAY_CERT_I2C_SCL, QL_ALIPAY_CERT_I2C_FUNC))
        {
            goto exit;
        }
        if (QL_GPIO_SUCCESS != ql_pin_set_func(QL_ALIPAY_CERT_I2C_SDA, QL_ALIPAY_CERT_I2C_FUNC))
        {
            goto exit;
        }
        if (QL_I2C_SUCCESS != ql_I2cInit(ql_alipay_i2c_no, STANDARD_MODE))
        {
            goto exit;
        }
    } 
    else 
    {
        goto exit;
    }
    return ql_i2c_data;
exit:
    if (ql_i2c_data)
    {
        free(ql_i2c_data);
    }
    return NULL;
}

void IIC_Master_Send(unsigned char byAddr, unsigned char *pData, unsigned short wLen)
{
    ql_i2c_write_slave_data(ql_alipay_i2c_no, byAddr, pData, wLen);
}

void IIC_Master_Receive(unsigned char byAddr, unsigned char *pData, unsigned short wLen)
{
    ql_i2c_read_slave_data(ql_alipay_i2c_no, byAddr, pData, wLen);
}

void ql_alipay_sema_release(ql_sem_t ql_alipay_sem)
{
    if (ql_alipay_sem)
    {
        ql_rtos_semaphore_release(ql_alipay_sem);
    }
}

void Delay_Ms(unsigned char byMilliSec)
{
    ql_sem_t ql_alipay_sem = NULL;
    ql_timer_t ql_alipay_timer = NULL;
    
    ql_rtos_semaphore_create(&ql_alipay_sem, 0);   
    ql_rtos_timer_create(&ql_alipay_timer, NULL, ql_alipay_sema_release, ql_alipay_sem);
    ql_rtos_timer_start(ql_alipay_timer, byMilliSec, 0);

    ql_rtos_semaphore_wait(ql_alipay_sem, QL_WAIT_FOREVER);

    ql_rtos_semaphore_delete(ql_alipay_sem);
    ql_rtos_timer_delete(ql_alipay_timer);
}

void Set_GPIO_State(unsigned char byState)
{
    if (ql_alipay_gpio_init == false)
    {
        ql_alipay_gpio_init = true;
        ql_pin_set_gpio(QL_ALIPAY_CERT_GPIO_PIN);
        if (byState == 1)
        {
            ql_gpio_init(QL_ALIPAY_CERT_GPIO_NUM, GPIO_OUTPUT, PULL_NONE, LVL_HIGH);
        }
        else 
        {
            ql_gpio_init(QL_ALIPAY_CERT_GPIO_NUM, GPIO_OUTPUT, PULL_NONE, LVL_LOW);
        }
    }
    else 
    {
        if (byState == 1)
        {
            ql_gpio_set_level(QL_ALIPAY_CERT_GPIO_NUM, LVL_HIGH);
        }
        else 
        {
            ql_gpio_set_level(QL_ALIPAY_CERT_GPIO_NUM, LVL_LOW);
        }
    }
}

void ql_alipay_cert_init()
{
    ql_pin_set_func(QL_ALIPAY_CERT_GPIO_PIN, QL_ALIPAY_CERT_GPIO_PIN_FUNC);
    ql_gpio_init(QL_ALIPAY_CERT_GPIO_NUM, GPIO_OUTPUT, PULL_NONE, LVL_HIGH);
    Delay_Ms(10);
    ql_gpio_set_level(QL_ALIPAY_CERT_GPIO_NUM, LVL_LOW);
    Delay_Ms(10);
    ql_gpio_set_level(QL_ALIPAY_CERT_GPIO_NUM, LVL_HIGH);
    Delay_Ms(50);
    ql_alipay_gpio_init = true;
}


