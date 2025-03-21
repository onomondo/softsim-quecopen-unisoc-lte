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
#include "iot_watchdog.h"
#include "ql_ohos_hal_watchdog.h"
#include <sys/unistd.h>

ql_ohos_watchdog_t  *g_ql_ohos_watchdog_handle = NULL;

void IoTWatchDogEnable(void)
{
    if(g_ql_ohos_watchdog_handle == NULL)
    {
        return;
    }

    return g_ql_ohos_watchdog_handle->watchdog_enable();
}

void IoTWatchDogKick(void)
{
    if(g_ql_ohos_watchdog_handle == NULL)
    {
        return;
    }

    return g_ql_ohos_watchdog_handle->watchdog_kick();
}

void IoTWatchDogDisable(void)
{
    if(g_ql_ohos_watchdog_handle == NULL)
    {
        return;
    }

    return g_ql_ohos_watchdog_handle->watchdog_disable();
}

int ql_ohos_watchdog_api_register(ql_ohos_watchdog_t *watchdog)
{
    if(watchdog == NULL)
    {
        return -1;
    }

    g_ql_ohos_watchdog_handle = watchdog;

    return 0;
}

int ql_ohos_watchdog_api_unregister(void)
{
    g_ql_ohos_watchdog_handle = NULL;

    return 0;
}