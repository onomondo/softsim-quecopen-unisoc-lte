/**
  @file

  @brief

*/
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

/*===========================================================================
 * include files
 ===========================================================================*/
#include "ql_wifi_adapter.h"
#include "drv_espi.h"
#include "drvspi_platform.h"
#include "platform_memory.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_app.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/

/*========================================================================
 *   Global Variable
 *========================================================================*/

/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_adapter_errcode_e ql_wifi_adapter_server_init(ql_wifi_adapter_driver_s *driver)
{
    return (0 == drv_espi_service_init(driver)) ? QL_WIFI_ADAPTER_SUCCESS : QL_WIFI_ADAPTER_EXECUTE_ERR;
}

void ql_wifi_adapter_memory_free(void *ptr)
{
    platform_memory_free(ptr);
}

ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_dispatching(ql_wifi_adapter_msg_s *msg)
{
    ql_wifi_adapter_errcode_e ret = QL_WIFI_ADAPTER_SUCCESS;
    if (!msg)
    {
        return QL_WIFI_ADAPTER_PARAM_ERR;
    }
    switch (msg->msgType)
    {
    case DRV_ESPI_TYPE_MSG:
    case DRV_ESSPI_CONTROL_AP:
    case DRV_ESSPI_CONTROL_STA:{
        ql_wifi_msg_s wifi_msg = {0};
        wifi_msg.type = QL_WIFI_MSG_TYPE_READ;
        wifi_msg.msg = *msg;
        wifi_msg.msg.msgType = msg->msgType;
        if (QL_WIFI_MSG_SUCCESS != ql_wifi_msg_recv(&wifi_msg))
        {
            ret = QL_WIFI_ADAPTER_EXECUTE_ERR;
        }
        break;
    }
    default: {
        if (msg->msgAddr)
        {
            ql_wifi_adapter_memory_free(msg->msgAddr);
        }
        break;
    }
    }
    return ret;
}

ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_send(uint8_t *msg, uint32_t len)
{
    return (0 == drv_espi_send_type_data(DRV_ESPI_TYPE_MSG, msg, len)) ? QL_WIFI_ADAPTER_SUCCESS : QL_WIFI_ADAPTER_EXECUTE_ERR;
}

ql_wifi_adapter_errcode_e ql_wifi_adapter_data_send(uint8_t *data, uint32_t len, uint32_t mode)
{
    return (0 == drv_espi_sendto_peer(data, len, mode)) ? QL_WIFI_ADAPTER_SUCCESS : QL_WIFI_ADAPTER_EXECUTE_ERR;
}

ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_read()
{
    ql_wifi_adapter_msg_s msg = {0};
    unsigned char tbuf[DRV_ESSPI_RXDATA_MAX] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    int status = 0;

    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];

    spicfg->evt = DRV_ESPI_TYPE_MSG;
    spicfg->len = 0;
    spicfg->type = DRV_ESSPI_SERVICE_READ;

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0)
    {
        return QL_WIFI_ADAPTER_EXECUTE_ERR;
    }
    status = drv_espi_read_status();
    if (status < 0)
    {
        return QL_WIFI_ADAPTER_EXECUTE_ERR;
    }

    if (status == 0 || status == 0xFFFF)
    {
        QL_WIFI_ADAPTER_LOG("%s[%d] 0x%x\n", __FUNCTION__, __LINE__, status);
        return QL_WIFI_ADAPTER_EXECUTE_ERR;
    }

    if ((status & DRV_ESSPI_CONTROL_MSG) == DRV_ESSPI_CONTROL_MSG)
    {
        msg.msgType = DRV_ESPI_TYPE_MSG;
        msg.msgValue = (status & 0xFFF) + DRV_ESSPI_CMD_LEN;
    }
    else if ((status & DRV_ESSPI_CONTROL_INT) == DRV_ESSPI_CONTROL_INT)
    {
        msg.msgType = DRV_ESPI_TYPE_INT;
        msg.msgValue = status & 0xFFF;
    }
    else
    {
        QL_WIFI_ADAPTER_LOG("Unknown Status");
        return -1;
    }

    if (msg.msgType != DRV_ESPI_TYPE_INT)
    {
        msg.msgAddr = platform_memory_alloc(msg.msgValue);
        if (msg.msgAddr == NULL)
        {
            QL_WIFI_ADAPTER_LOG("%s[%d] alloc failed\n", __FUNCTION__, __LINE__);
            return QL_WIFI_ADAPTER_EXECUTE_ERR;
        }
        msg.msgAddr[0] = DRV_ESSPI_CMD_READ;
        msg.msgAddr[1] = DRV_ESSPI_CMD_DUMMY;
        drv_spi_platfrom_read(msg.msgAddr, msg.msgValue);
    }
    if (QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_msg_dispatching(&msg))
    {
        if (msg.msgAddr)
        {
            platform_memory_free(msg.msgAddr);
        }
        return QL_WIFI_ADAPTER_EXECUTE_ERR;
    }
    return QL_WIFI_ADAPTER_SUCCESS;
}

void ql_wifi_adapter_spi_set_cs(ql_LvlMode lvl)
{
    ql_ethernet_phy_append(ql_wifi_app_spi_set_cs, &lvl);
}

int ql_wifi_adapter_int_register(ql_GpioNum gpio_num, ql_TriggerMode gpio_trigger, ql_DebounceMode gpio_debounce,
                                  ql_EdgeMode gpio_edge, ql_PullMode gpio_pull,
                                  void *int_cb, void *cb_ctx)
{
    return (int)ql_int_register(gpio_num, gpio_trigger, gpio_debounce, gpio_edge, gpio_pull, int_cb, cb_ctx);
}

int ql_wifi_adapter_int_enable(ql_GpioNum gpio_num, ql_TriggerMode gpio_trigger, ql_DebounceMode gpio_debounce,
                                  ql_EdgeMode gpio_edge, ql_PullMode gpio_pull,
                                  void *int_cb, void *cb_ctx)
{
    int ret = 0;
    if(QL_GPIO_SUCCESS != (ret = ql_int_enable_ex(gpio_num, gpio_trigger, gpio_debounce, gpio_edge, gpio_pull, int_cb, cb_ctx)))
    {
        goto exit;
    }
exit:
    return ret;
}

int ql_wifi_adapter_int_disable(ql_GpioNum gpio_num)
{
    int ret = 0;
    if(QL_GPIO_SUCCESS != (ret = ql_int_disable_ex(gpio_num)))
    {
        goto exit;
    }
exit:
    return ret;
}