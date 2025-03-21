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
#include "iot_pwm.h"
#include "ql_ohos_hal_pwm.h"
#include <sys/unistd.h>

ql_ohos_pwm_t  *g_ql_ohos_pwm_handle = NULL;

unsigned int IoTPwmInit(unsigned int port)
{
    if(g_ql_ohos_pwm_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_pwm_handle->pwm_init(port);
}

unsigned int IoTPwmDeinit(unsigned int port)
{
    if(g_ql_ohos_pwm_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_pwm_handle->pwm_deinit(port);
}

unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq)
{
    if(g_ql_ohos_pwm_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_pwm_handle->pwm_start(port, duty, freq);
}

unsigned int IoTPwmStop(unsigned int port)
{
    if(g_ql_ohos_pwm_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_pwm_handle->pwm_stop(port);
}

int ql_ohos_pwm_api_register(ql_ohos_pwm_t *pwm)
{
    if(pwm == NULL)
    {
        return -1;
    }

    g_ql_ohos_pwm_handle = pwm;

    return 0;
}

int ql_ohos_pwm_api_unregister(void)
{
    g_ql_ohos_pwm_handle = NULL;

    return 0;
}