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
#include "iot_uart.h"
#include "ql_ohos_hal_uart.h"
#include <sys/unistd.h>

ql_ohos_uart_t  *g_ql_ohos_uart_handle = NULL;

unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)
{
    if(g_ql_ohos_uart_handle == NULL || param == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_uart_handle->uart_init(id, param);
}

int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)
{
    if(g_ql_ohos_uart_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_uart_handle->uart_read(id, data, dataLen);
}

int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)
{
    if(g_ql_ohos_uart_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_uart_handle->uart_write(id, data, dataLen);
}

unsigned int IoTUartDeinit(unsigned int id)
{
    if(g_ql_ohos_uart_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_uart_handle->uart_deinit(id);
}

unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl)
{
    if(g_ql_ohos_uart_handle == NULL)
    {
        return IOT_FAILURE;
    }

    return g_ql_ohos_uart_handle->uart_set_flow_ctrl(id, flowCtrl);
}

int ql_ohos_uart_api_register(ql_ohos_uart_t *uart)
{
    if(uart == NULL)
    {
        return -1;
    }

    g_ql_ohos_uart_handle = uart;

    return 0;
}

int ql_ohos_uart_api_unregister(void)
{
    g_ql_ohos_uart_handle = NULL;

    return 0;
}