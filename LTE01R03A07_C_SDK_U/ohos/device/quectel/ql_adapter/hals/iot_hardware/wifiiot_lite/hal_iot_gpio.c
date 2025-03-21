/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "iot_errno.h"
#include "iot_gpio.h"
#include "ql_ohos_hal_gpio.h"
#include <sys/unistd.h>

ql_ohos_gpio_t  *g_ql_ohos_gpio_handle = NULL;

unsigned int IoTGpioInit(unsigned int id)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_init == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_init(id);
}

unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_set_dir == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_set_dir(id, dir);
}

unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_get_dir == NULL || dir == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_get_dir(id, dir);
}

unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_set_output_val == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_set_output_val(id, val);
}

unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_get_output_val == NULL || val == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_get_output_val(id, val);
}

unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_get_input_val == NULL || val == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_get_input_val(id, val);
}

unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity,
                                    GpioIsrCallbackFunc func, char *arg)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_register_isr_func == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_register_isr_func(id, intType, intPolarity, func, arg);
}

unsigned int IoTGpioUnregisterIsrFunc(unsigned int id)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_unregister_isr_func == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_unregister_isr_func(id);
}

unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_set_isr_mask == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_set_isr_mask(id, mask);
}

unsigned int IoTGpioSetIsrMode(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_set_isr_mode == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_set_isr_mode(id, intType, intPolarity);
}

unsigned int IoTGpioDeinit(unsigned int id)
{
    if(g_ql_ohos_gpio_handle == NULL || id > g_ql_ohos_gpio_handle->gpio_index_max || g_ql_ohos_gpio_handle->gpio_deinit == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_gpio_handle->gpio_deinit(id);
}

int ql_ohos_gpio_api_register(ql_ohos_gpio_t *gpio)
{
    if(gpio == NULL)
    {
        return -1;
    }

    g_ql_ohos_gpio_handle = gpio;

    return 0;
}

int ql_ohos_gpio_api_unregister(void)
{
    g_ql_ohos_gpio_handle = NULL;

    return 0;
}