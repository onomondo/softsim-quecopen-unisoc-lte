/**  @file

  @brief
  This file is used to define bt api for different Quectel Project.

*/

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
#ifndef _WIFI_ADAPTER_H_
#define _WIFI_ADAPTER_H_

#include "ql_api_common.h"
#include <stdint.h>

#include "ql_api_wifi.h"
#include "ql_api_ethernet.h"
#include "ql_gpio.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_ADAPTER_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_ADAPTER_LOG(msg, ...) QL_LOG(QL_WIFI_ADAPTER_LOG_LEVEL, "wifi_adapter", msg, ##__VA_ARGS__)

/* Genernal. */
#define AOS_WAIT_FOREVER QL_WAIT_FOREVER
#define aos_msleep(x) ql_rtos_task_sleep_ms(x)

/* drv_espi.c */

#define ql_wifi_adapter_net_send_msg(x) esnet_wifi_send_msg(x)

/* drvspi_platform.c */
#define ql_wifi_adapter_spi_init(x) (int)ql_ethernet_phy_init(x)
#define ql_wifi_adapter_spi_deinit(x) (int)ql_ethernet_phy_deinit(x)
#define ql_wifi_adapter_spi_write(x, y) (int)ql_ethernet_phy_write(x, y)
#define ql_wifi_adapter_spi_read(x, y) (int)ql_ethernet_phy_read(x, y)
#define ql_wifi_adapter_spi_write_read(x, y, z) (int)ql_ethernet_phy_write_read(x, y, z)

#define ql_wifi_adapter_pin_set_func(x, y) (int)ql_pin_set_func(x, y)

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_ADAPTER_SUCCESS = 0,
    QL_WIFI_ADAPTER_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_ADAPTER_PARAM_ERR = 2 | (QL_COMPONENT_LWIP_WIFI << 16),
} ql_wifi_adapter_errcode_e;

typedef enum
{
    QL_WIFI_ADAPTER_MSG_TYPE_INFO,
    QL_WIFI_ADAPTER_MSG_TYPE_HTOS = 0x100,
    QL_WIFI_ADAPTER_MSG_TYPE_STOH = 0x200,
    QL_WIFI_ADAPTER_MSG_TYPE_OTA = 0x300,
    QL_WIFI_ADAPTER_MSG_TYPE_MSG = 0x500,
    QL_WIFI_ADAPTER_MSG_TYPE_INT,
    QL_WIFI_ADAPTER_MSG_TYPE_AP = 0x0000,
    QL_WIFI_ADAPTER_MSG_TYPE_STA = 0x1000,
} ql_wifi_adapter_msg_type_e;   /* See drv_espi_type_e. */
/*========================================================================
 *  Type Definition
 *=========================================================================*/
/* platform_mutex.h */
typedef ql_mutex_t aos_mutex_t;
/* platform_queue.h */
typedef ql_queue_t aos_queue_t;
/* platform_sem.h */
typedef ql_sem_t aos_sem_t;
/* platform_thread.h */
typedef ql_task_t aos_task_t;
/* Genernal. */
typedef ql_ethernet_phy_s ql_wifi_spi_s;

typedef struct
{
    /* Set with ql_pin_set_func. */
    uint8_t pin_num;
    uint8_t func_sel;
    /* Set with ql_int_register & ql_int_enable. */
    ql_GpioNum gpio_num;
    ql_TriggerMode gpio_trigger;
    ql_DebounceMode gpio_debounce;
    ql_EdgeMode gpio_edge;
    ql_PullMode gpio_pull;
    void *int_cb;
    void *cb_ctx;
} ql_wifi_gpio_s;

typedef struct
{
    ql_wifi_spi_s spi;       /* SPI host. */
    ql_wifi_gpio_s hrx_gpio; /* GPIO trigger by slave.RX for host. */
} ql_wifi_adapter_driver_s;

typedef struct
{
    unsigned int msgType;
    unsigned int msgValue;
    unsigned char *msgAddr;
} ql_wifi_adapter_msg_s;

typedef ql_wifi_adapter_msg_s esnet_wifi_msg_t;
/*========================================================================
 *	function Definition
 *========================================================================*/
void ql_wifi_adapter_memory_free(void *ptr);

ql_wifi_adapter_errcode_e ql_wifi_adapter_server_init(ql_wifi_adapter_driver_s *driver);
ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_dispatching(ql_wifi_adapter_msg_s *msg);

ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_send(uint8_t *msg, uint32_t len);
ql_wifi_adapter_errcode_e ql_wifi_adapter_data_send(uint8_t *data, uint32_t len, uint32_t mode);
ql_wifi_adapter_errcode_e ql_wifi_adapter_msg_read();

void ql_wifi_adapter_spi_set_cs(ql_LvlMode lvl);

int ql_wifi_adapter_int_enable(ql_GpioNum gpio_num, ql_TriggerMode gpio_trigger, ql_DebounceMode gpio_debounce,
                                  ql_EdgeMode gpio_edge, ql_PullMode gpio_pull,
                                  void *int_cb, void *cb_ctx);
int ql_wifi_adapter_int_disable(ql_GpioNum gpio_num);
#ifdef __cplusplus
}
#endif

#endif