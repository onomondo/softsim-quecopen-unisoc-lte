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
#include "iot_i2c.h"
#include "ql_ohos_hal_i2c.h"
#include <sys/unistd.h>

ql_ohos_i2c_t  *g_ql_ohos_i2c_handle = NULL;

unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate)
{
    if(g_ql_ohos_i2c_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_i2c_handle->i2c_init(id, baudrate);
}

unsigned int IoTI2cDeinit(unsigned int id)
{
    if(g_ql_ohos_i2c_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_i2c_handle->i2c_deinit(id);
}

unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen)
{
    if(g_ql_ohos_i2c_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_i2c_handle->i2c_write(id, deviceAddr, data, dataLen);
}

unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen)
{
    if(g_ql_ohos_i2c_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_i2c_handle->i2c_read(id, deviceAddr, data, dataLen);
}

unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate)
{
    if(g_ql_ohos_i2c_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_i2c_handle->i2c_set_baudrate(id, baudrate);
}

int ql_ohos_i2c_api_register(ql_ohos_i2c_t *i2c)
{
    if(i2c == NULL)
    {
        return -1;
    }

    g_ql_ohos_i2c_handle = i2c;

    return 0;
}

int ql_ohos_i2c_api_unregister(void)
{
    g_ql_ohos_i2c_handle = NULL;

    return 0;
}