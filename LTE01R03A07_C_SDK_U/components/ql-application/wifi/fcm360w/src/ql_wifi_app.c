/**
  @file
  ql_wifi_app.c

  @brief
  Quectel wifi app api.

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lwip/inet.h>
#include "netif/etharp.h"

#include "ql_osi_def.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#include "quec_common.h"
#include "ql_api_datacall.h"

#include "ql_api_wifi.h"
#include "ql_wifi_adapter.h"
#include "ql_wifi_app.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_APP_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_app", msg, ##__VA_ARGS__)

#define QL_WIFI_APP_TASK_EVENT_CNT 5
#define QL_WIFI_APP_MSG_QUEUE_CNT 128

#define QL_WIFI_APP_TASK_QUEUE_WAIT 2000
#define QL_WIFI_APP_TASK_DEINIT_WAIT 5000

/* SPI. */
#define QL_CUR_SPI_PORT             QL_SPI_PORT2
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI2_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI2_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI2_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI2_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI2_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI2_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI2_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI2_DI_FUNC
/* GPIO. */
#define QL_CUR_INT_PIN 127
#define QL_CUR_INT_PIN_GPIO_FUNC 0
#define QL_CUR_INT_GPIO_NUM GPIO_23

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_APP_TASK_TX,
    QL_WIFI_APP_TASK_RX,
    QL_WIFI_APP_TASK_MAX,
} ql_wifi_app_task_type_e;

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t task;
    ql_queue_t queue;
    ql_sem_t task_delete_sem;
} ql_wifi_app_task_s;

typedef struct
{
    ql_wifi_app_task_s task[QL_WIFI_APP_TASK_MAX];
    ql_wifi_app_sta_status_cb sta_status_cb;
    ql_wifi_app_ap_status_cb ap_status_cb;
    ql_wifi_app_user_ap_info_s ap_info;
} ql_wifi_app_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_app_manager_s ql_wifi_app_manager = {0};
ql_mutex_t ql_wifi_app_mutex = NULL;

/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_app_errcode_e ql_wifi_app_recv(void *argv)
{
    ql_wifi_app_task_s *task = &ql_wifi_app_manager.task[QL_WIFI_APP_TASK_RX];
    ql_wifi_app_s *msg = (ql_wifi_app_s *)argv;
    ql_wifi_app_errcode_e ret = QL_WIFI_APP_SUCCESS;
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);

    if (QL_OSI_SUCCESS != ql_rtos_queue_release(task->queue, sizeof(ql_wifi_app_s), (uint8 *)msg, QL_NO_WAIT))
    {
        QL_WIFI_APP_LOG("release msg_queue failed");
        ret = QL_WIFI_APP_EXECUTE_ERR;
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    return ret;
}

ql_wifi_app_errcode_e ql_wifi_app_msg_write(ql_wifi_msg_tag_e tag, uint8_t *data, uint32_t len)
{
    return QL_WIFI_MSG_SUCCESS == ql_wifi_msg_write(tag,data,len)? QL_WIFI_APP_SUCCESS : QL_WIFI_APP_EXECUTE_ERR;
}

void ql_wifi_app_specify_interface(ql_wifi_mode_e mode,uint8_t id)
{
    ql_wifi_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    /* Set mode. */
    int val_len = 0;
    val_len = sizeof(ql_wifi_mode_e);
    ret = ql_wifi_opt_set(QL_WIFI_OPT_MODE, &mode, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_APP_LOG("set mode faild 0x%x",ret);
        goto exit;
    }

    /* Set id. */
    val_len = sizeof(uint8_t);
    ret = ql_wifi_opt_set(QL_WIFI_OPT_ID, &id, val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_APP_LOG("set id faild 0x%x",ret);
        goto exit;
    }
    QL_WIFI_APP_LOG("wifi app specify %d/%d",mode,id);
exit:
    return;
}

ql_wifi_app_errcode_e ql_wifi_app_get_interface(ql_wifi_mode_e *mode,uint8_t *id)
{
    ql_wifi_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    /* Get mode. */
    int val_len = 0;
    val_len = sizeof(ql_wifi_mode_e);
    ret = ql_wifi_opt_get(QL_WIFI_OPT_MODE, mode, &val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        ret = QL_WIFI_APP_EXECUTE_ERR;
        QL_WIFI_APP_LOG("set mode faild 0x%x",ret);
        goto exit;
    }

    /* Get id. */
    val_len = sizeof(uint8_t);
    ret = ql_wifi_opt_get(QL_WIFI_OPT_ID, id, &val_len);
    if (ret != QL_WIFI_SUCCESS)
    {
        ret = QL_WIFI_APP_EXECUTE_ERR;
        QL_WIFI_APP_LOG("set id faild 0x%x",ret);
        goto exit;
    }
exit:
    return ret;
}

ql_wifi_app_errcode_e ql_wifi_app_get_user_ap_info(ql_wifi_app_user_ap_info_s *info)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    if(!info)
    {
        return QL_WIFI_APP_INVALID_PARAM_ERR;
    }
    memcpy(info,&(manager->ap_info),sizeof(ql_wifi_app_user_ap_info_s));
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_set_user_ap_info(ql_wifi_app_user_ap_info_s *info)
{
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    if(!info)
    {
        return QL_WIFI_APP_INVALID_PARAM_ERR;
    }
    memcpy(&(manager->ap_info),info,sizeof(ql_wifi_app_user_ap_info_s));
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_scan(ql_wifi_app_scan_result_cb cb)
{
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_sta_enable(ql_wifi_app_sta_status_cb sta_status_cb)
{
    int ret = 0;
    /* Get ssid & password. */
    ql_wifi_app_user_ap_info_s user_info = {0};

    /* Get user ssid/password. */
    ret = ql_wifi_app_get_user_ap_info(&user_info);
    if(ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_APP_LOG("get ssid/password faild!");
        ret = QL_WIFI_APP_INVALID_AP_USER_INFO_ERR;
        goto exit;
    }

    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    manager->sta_status_cb = sta_status_cb;
    /* Set ssid. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_STA_SSID,(uint8_t*)&user_info.ssid,strlen(user_info.ssid));
    /* Set password. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_STA_PASSWORD,(uint8_t*)&user_info.password,strlen(user_info.password));
    uint8_t channel = 1;
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_STA_CHANNEL,&channel,sizeof(uint8_t));
    /* Start STA. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_STA_START,NULL,0);
exit:
    return ret;
}

ql_wifi_app_errcode_e ql_wifi_app_sta_disable(void *argv)
{
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_STA_STOP,NULL,0);
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_msg_read(ql_wifi_msg_tag_e tag, uint8_t *data, uint32_t *len)
{
    return QL_WIFI_MSG_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_ap_enable(ql_wifi_app_ap_status_cb ap_status_cb)
{
    int ret = 0;
    /* Get ssid & password. */
    ql_wifi_app_user_ap_info_s user_info = {0};

    uint8_t sta_clients = QL_WIFI_APP_AP_STA_CLIENTS_MAX;

    /* Get user ssid/password. */
    ret = ql_wifi_app_get_user_ap_info(&user_info);
    if(ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_APP_LOG("get ssid/password faild!");
        ret = QL_WIFI_APP_INVALID_AP_USER_INFO_ERR;
        goto exit;
    }

    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    manager->ap_status_cb = ap_status_cb;
    /* Set DNS. */
    ql_wifi_msg_ip_info_s ip_info = {0};
    ql_data_call_info_s info ={0};
    ql_wifi_cid_s ctx = {0};
    int val_len = sizeof(ql_wifi_cid_s);
    if(QL_WIFI_SUCCESS != ql_wifi_opt_get(QL_WIFI_OPT_CID,&ctx,&val_len))
    {
        ret = QL_WIFI_APP_EXECUTE_ERR;
        goto exit;
    }
    if(QL_DATACALL_SUCCESS != ql_get_data_call_info(ctx.sim_id, ctx.cid, &info))
    {
        ret = QL_WIFI_APP_EXECUTE_ERR;
        goto exit;
    }
    //ip_info.ip.addr = info.v4.addr.ip.addr;
    ip_info.dns1.addr = info.v4.addr.pri_dns.addr;
    ip_info.dns2.addr = info.v4.addr.sec_dns.addr;

    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_IP_INFO,(uint8_t*)&ip_info,sizeof(ql_wifi_msg_ip_info_s));
    /* Set ssid. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_SSID,(uint8_t*)&user_info.ssid,strlen(user_info.ssid));
    /* Set password. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_PASSWORD,(uint8_t*)&user_info.password,strlen(user_info.password));
    /* Set max clients. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_STA_CLIENT_MAX,(uint8_t*)&sta_clients,sizeof(uint8_t));
    /* Start AP. */
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_START,NULL,0);
exit:
    return ret;
}

ql_wifi_app_errcode_e ql_wifi_app_ap_disable(void *argv)
{
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_AP_STOP,NULL,0);
    return QL_WIFI_APP_SUCCESS;
}

ql_wifi_app_errcode_e ql_wifi_app_reset(void *argv)
{
    ql_wifi_app_msg_write(QL_WIFI_MSG_TAG_SEND_RESET,NULL,0);
    return QL_WIFI_APP_SUCCESS;
}

void ql_wifi_app_delete_task(ql_wifi_app_task_s *task)
{
    if(!task)
    {
        return;
    }
    ql_wifi_app_s msg = {0};
    int ret = 0;
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    ql_rtos_semaphore_release(task->task_delete_sem);
    ql_rtos_semaphore_delete(task->task_delete_sem);
    uint32 cnt = 0;
    ql_rtos_queue_get_cnt(task->queue,&cnt);
    QL_WIFI_APP_LOG("del msg queue cnt %d/%p",cnt,task);
    while(0 < cnt)
    {
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_app_s), QL_WAIT_FOREVER);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_APP_LOG("wait error err= %d", ret);
            break;
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        cnt--;
        QL_WIFI_APP_LOG("remain msg %d",cnt);
    }
    ql_rtos_queue_delete(task->queue);
    memset(task, 0, sizeof(ql_wifi_app_task_s));
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    ql_rtos_task_delete(NULL);
}

void ql_wifi_app_rx_thread(void *argv)
{
    ql_wifi_app_task_s *task = (ql_wifi_app_task_s *)argv;
    ql_wifi_app_manager_s *manager = &ql_wifi_app_manager;
    QlOSStatus ret = QL_OSI_SUCCESS;
    ql_wifi_app_s msg = {0};
    QL_WIFI_APP_LOG("wifi app rx task start");
    while (1)
    {
        /* Sem take. */
        ret = ql_rtos_semaphore_wait(task->task_delete_sem,QL_NO_WAIT);
        if(QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_APP_LOG("rx deleting");
            /*User called ql_wifi_net_deinit and took the sem. */
            break;
        }
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_app_s), QL_WIFI_APP_TASK_QUEUE_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            if(QL_OSI_TASK_PARAM_INVALID == ret)
            {
                break;
            }
            ql_rtos_semaphore_release(task->task_delete_sem);
            continue;
        }
        QL_WIFI_APP_LOG("wifi app event 0x%x",msg.type);
        switch (msg.type)
        {
        case QL_WIFI_APP_STA_STATUS_CMD:{
            uint32_t status = (uint32_t)msg.msg.msgType;
            QL_WIFI_APP_LOG("status = %d", status);
            ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
            if (manager->sta_status_cb != NULL)
            {
                QL_WIFI_APP_LOG("wifi app ???");
                manager->sta_status_cb(&status);
            }
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            break;
        }
        case QL_WIFI_APP_AP_STATUS_CMD:{
            uint32_t status = (uint32_t)msg.msg.msgType;
            QL_WIFI_APP_LOG("status = %d", status);
            ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
            if (manager->ap_status_cb != NULL)
            {
                manager->ap_status_cb(&status);
            }
            ql_rtos_mutex_unlock(ql_wifi_app_mutex);
            break;
        }
        default: {
            //QL_WIFI_NET_LOG("cmd_info.msg_id=%d  ERROR", cmd_info.msg_id);
            break;
        }
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        ret = ql_rtos_semaphore_release(task->task_delete_sem);
    }
    QL_WIFI_APP_LOG("wifi app rx task end");
    ql_wifi_app_delete_task(task);
}

ql_wifi_app_errcode_e ql_wifi_app_task_create(ql_wifi_app_task_type_e type)
{
    ql_wifi_app_errcode_e ret = QL_WIFI_APP_SUCCESS;
    if (type >= QL_WIFI_APP_TASK_MAX)
    {
        return QL_WIFI_APP_INVALID_PARAM_ERR;
    }
    ql_wifi_app_task_s *task = &ql_wifi_app_manager.task[type];
    /* Create task delete sem. */
    if (QL_OSI_SUCCESS != ql_rtos_semaphore_create(&(task->task_delete_sem), 1))
    {
        ret = QL_WIFI_APP_SEM_CREATE_ERR;
        goto exit;
    }
    /* Create msg queue. */
    if (QL_OSI_SUCCESS != ql_rtos_queue_create(&task->queue, sizeof(ql_wifi_app_s), QL_WIFI_APP_MSG_QUEUE_CNT))
    {
        ret = QL_WIFI_APP_MSG_QUEUE_CREATE_ERR;
        goto exit;
    }
    /* Create msg task. */

    char name[][20] = {"ql_wifi_app_tx", "ql_wifi_app_rx"};

    void (*thread[])(void *) = {NULL, ql_wifi_app_rx_thread};

    if (QL_OSI_SUCCESS != ql_rtos_task_create(&task->task, 4096, APP_PRIORITY_REALTIME, (char *)name[type], thread[type], task, QL_WIFI_APP_TASK_EVENT_CNT))
    {
        QL_WIFI_APP_LOG("%s create err", name[type]);
        ret = QL_WIFI_APP_TASK_CREATE_ERR;
        goto exit;
    }
exit:
    if (QL_WIFI_APP_SUCCESS != ret)
    {
        if (task->task_delete_sem)
        {
            ql_rtos_semaphore_delete(task->task_delete_sem);
        }
        if (task->queue)
        {
            ql_rtos_queue_delete(task->queue);
        }
        memset(task, 0, sizeof(ql_wifi_app_task_s));
    }
    return ret;
}

void ql_wifi_app_spi_set_cs(void *ctx)
{
    if(!ctx)
    {
        return;
    }
    ql_LvlMode lvl = (ql_LvlMode)(*((uint8_t*)ctx));
    if(lvl == LVL_HIGH)
    {
        ql_spi_cs_high(QL_CUR_SPI_PORT);
    }
    else
    {
        ql_spi_cs_low(QL_CUR_SPI_PORT);
    }
}


ql_wifi_app_errcode_e ql_wifi_app_init(void *argv)
{
    ql_wifi_app_task_s *rx_task = &ql_wifi_app_manager.task[QL_WIFI_APP_TASK_RX];
    ql_wifi_app_errcode_e ret = QL_WIFI_APP_SUCCESS;
    /* Create mutex.Once mutex has been created,it will never delete for safety(e.g. Deleting task while sending msg queue). */
    if (NULL == ql_wifi_app_mutex)
    {
        ret = ql_rtos_mutex_create(&(ql_wifi_app_mutex));
        if (ret != 0)
        {
            ret = QL_WIFI_APP_MUTEX_CREATE_ERR;
            goto exit;
        }
    }
    if (rx_task->task)
    {
        goto exit;
    }
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    /* Create RX task. */
    if (QL_WIFI_APP_SUCCESS != (ret = ql_wifi_app_task_create(QL_WIFI_APP_TASK_RX)))
    {
        ql_rtos_mutex_unlock(ql_wifi_app_mutex);
        ret = QL_WIFI_APP_TASK_CREATE_ERR;
        goto exit;
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);

    /* Create wifi msg task. */
    if(QL_WIFI_MSG_SUCCESS != ql_wifi_msg_init(NULL))
    {
        ret = QL_WIFI_APP_INIT_MSG_ERR;
        goto exit;
    }

    /* Create wifi net task. */
    if(QL_WIFI_NET_SUCCESS != ql_wifi_net_init(NULL))
    {
        ret = QL_WIFI_APP_INIT_NET_ERR;
        goto exit;
    }

    ql_wifi_adapter_driver_s driver = 
    {
        /* SPI. */
        .spi.mode                          = QL_ETHERNET_PHY_HW_SPI_MODE,
        .spi.hw_spi.mosi_pin_num           = QL_CUR_SPI_DO_PIN,
        .spi.hw_spi.mosi_func_sel          = QL_CUR_SPI_DO_FUNC,
        .spi.hw_spi.miso_pin_num           = QL_CUR_SPI_DI_PIN,
        .spi.hw_spi.miso_func_sel          = QL_CUR_SPI_DO_FUNC,
        .spi.hw_spi.clk_pin_num            = QL_CUR_SPI_CLK_PIN,
        .spi.hw_spi.clk_func_sel           = QL_CUR_SPI_CLK_FUNC,
        .spi.hw_spi.cs_pin_num             = QL_CUR_SPI_CS_PIN,
        .spi.hw_spi.cs_func_sel            = QL_CUR_SPI_CS_FUNC,
        .spi.hw_spi.cs_is_auto             = 0,
        .spi.hw_spi.config.input_mode      = QL_SPI_INPUT_TRUE,
        .spi.hw_spi.config.port            = QL_CUR_SPI_PORT,
        .spi.hw_spi.config.spiclk          = QL_SPI_CLK_20MHZ,
#if QL_SPI_16BIT_DMA  
        .spi.hw_spi.config.framesize       = 16,
#else
        .spi.hw_spi.config.framesize       = 8,
#endif
        .spi.hw_spi.config.cs_polarity0    = QL_SPI_CS_ACTIVE_LOW,
        .spi.hw_spi.config.cs_polarity1    = QL_SPI_CS_ACTIVE_LOW,
        .spi.hw_spi.config.cpol            = QL_SPI_CPOL_LOW,
        .spi.hw_spi.config.cpha            = QL_SPI_CPHA_1Edge,
        .spi.hw_spi.config.input_sel       = QL_SPI_DI_1,
        .spi.hw_spi.config.transmode       = QL_SPI_DMA_IRQ,
        .spi.hw_spi.config.cs              = QL_SPI_CS0,
        .spi.hw_spi.config.clk_delay       = QL_SPI_CLK_DELAY_0,
        /*********************************************/
        /* GPIO */
        .hrx_gpio.pin_num                  = QL_CUR_INT_PIN,
        .hrx_gpio.func_sel                 = QL_CUR_INT_PIN_GPIO_FUNC,
        .hrx_gpio.gpio_num                 = QL_CUR_INT_GPIO_NUM,
        .hrx_gpio.gpio_trigger             = LEVEL_TRIGGER,
        .hrx_gpio.gpio_debounce            = DEBOUNCE_DIS,
        .hrx_gpio.gpio_edge                = EDGE_RISING,
        .hrx_gpio.gpio_pull                = PULL_DOWN,
        .hrx_gpio.int_cb                   = NULL,  /* No used in app.Set in driver. */
        .hrx_gpio.cb_ctx                   = NULL,  /* No used in app.Set in driver. */
    };
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_server_init(&driver))
    {
        ret = QL_WIFI_APP_INIT_SPI_ERR;
        goto exit;
    }

exit:
    if(QL_WIFI_APP_SUCCESS != ret)
    {
        ql_wifi_app_deinit(NULL);
    }
    return ret;
}

ql_wifi_app_errcode_e ql_wifi_app_deinit(void *argv)
{
    ql_wifi_msg_deinit(NULL);
    ql_wifi_net_deinit(NULL);

    ql_wifi_app_task_s *rx_task = &ql_wifi_app_manager.task[QL_WIFI_APP_TASK_RX];
    int ret = 0;
    /* All tasks have been deleted if both tasks are NULL. */
    if (!rx_task->task)
    {
        return QL_WIFI_APP_SUCCESS;
    }
    ql_rtos_mutex_try_lock(ql_wifi_app_mutex);
    /* Take sem. */
    if (QL_WIFI_SUCCESS != (ret = ql_rtos_semaphore_wait(rx_task->task_delete_sem, QL_WIFI_APP_TASK_DEINIT_WAIT)))
    {
        QL_WIFI_APP_LOG("wait rx sem %x", ret);
    }
    ql_rtos_mutex_unlock(ql_wifi_app_mutex);
    return QL_WIFI_APP_SUCCESS;
}

