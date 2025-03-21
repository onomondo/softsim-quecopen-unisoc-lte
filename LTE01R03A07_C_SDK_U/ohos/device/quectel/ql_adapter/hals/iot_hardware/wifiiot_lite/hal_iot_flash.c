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
#include "iot_flash.h"
#include "ql_ohos_hal_flash.h"
#include <sys/unistd.h>

ql_ohos_flash_t  *g_ql_ohos_flash_handle = NULL;

unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
    if(g_ql_ohos_flash_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_flash_handle->flash_read(flashOffset, size, ramData);
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size, const unsigned char *ramData, unsigned char doErase)
{
    if(g_ql_ohos_flash_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_flash_handle->flash_write(flashOffset, size, ramData, doErase);
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
    if(g_ql_ohos_flash_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_flash_handle->flash_erase(flashOffset, size);
}

unsigned int IoTFlashInit(void)
{
    if(g_ql_ohos_flash_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_flash_handle->flash_init();
}

unsigned int IoTFlashDeinit(void)
{
    if(g_ql_ohos_flash_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_flash_handle->flash_deinit();
}

int ql_ohos_flash_api_register(ql_ohos_flash_t *flash)
{
    if(flash == NULL)
    {
        return -1;
    }

    g_ql_ohos_flash_handle = flash;

    return 0;
}

int ql_ohos_flash_api_unregister(void)
{
    g_ql_ohos_flash_handle = NULL;

    return 0;
}