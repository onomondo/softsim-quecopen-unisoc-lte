/**
  @file
  ql_wifi_spi.c

  @brief
  Quectel wifi spi api.

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

#include "ql_osi_def.h"
#include "ql_api_osi.h"
#include "ql_log.h"

#include "quec_common.h"
#include "ql_gpio.h"
#include "ql_api_spi.h"
#include "ql_api_wifi.h"

#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#include "ql_wifi_spi.h"
#include "ql_wifi_uart.h"

#include "lwip/pbuf.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_SPI_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_spi", msg, ##__VA_ARGS__)

#define QL_WIFI_SPI_TASK_EVENT_CNT 5

#define QL_WIFI_SPI_MAGIC0 0xAA
#define QL_WIFI_SPI_MAGIC1 0x55
#define QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE 9
#define QL_WIFI_SPI_DATA_LEN 1524

#define QL_CUR_SPI_PORT     QL_SPI_PORT2
#define QL_CUR_SPI_CS_PIN   QL_CUR_SPI2_CS_PIN
#define QL_CUR_SPI_CS_FUNC  QL_CUR_SPI2_CS_FUNC
#define QL_CUR_SPI_CLK_PIN  QL_CUR_SPI2_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC QL_CUR_SPI2_CLK_FUNC
#define QL_CUR_SPI_DO_PIN   QL_CUR_SPI2_DO_PIN
#define QL_CUR_SPI_DO_FUNC  QL_CUR_SPI2_DO_FUNC
#define QL_CUR_SPI_DI_PIN   QL_CUR_SPI2_DI_PIN
#define QL_CUR_SPI_DI_FUNC  QL_CUR_SPI2_DI_FUNC

#if 1
#define QL_SPI_RxPIN_GPIO_FUNC  0
#define QL_SPI_RxPIN_GPIO       QUEC_PIN_DNAME_GPIO_4 //pin19 --> 842D:GPIO23
#define QL_SPI_Rx_GPIO          GPIO_4

#else //客户配置
#define QL_SPI_RxPIN_GPIO      QUEC_PIN_DNAME_UART_1_RXD
#define QL_SPI_Rx_GPIO         GPIO_12
#define QL_PIN_GPIO_FUNC_GPIO  1
#endif
#define QL_SPI_CLK            QL_SPI_CLK_27_83MHZ //使用20M和以上的时钟, spi_config.cpha 需要配置 QL_SPI_CPHA_2Edge;
#define QL_SPI_TRANS_MODE     QL_SPI_DMA_IRQ
#define QL_SPI_DMA_WAIT_NONE  (0)
#define QL_SPI_DMA_WAIT_WRITE (1)
#define QL_SPI_DMA_WAIT_READ  (2)
/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t net_task;
    ql_task_t recv_task;
    ql_task_t send_task;
    ql_sem_t recv_sem;
    ql_mutex_t spi_mutex;
    ql_queue_t send_queue;
    ql_queue_t recv_queue;
    unsigned char *recv_data;
    unsigned char *recv_mal_data;
    // unsigned char *send_data;
    // unsigned char *send_mal_data;
} ql_wifi_spi_manager_s;

typedef struct
{
    unsigned char header[2];
    unsigned short int data_len;
    unsigned short int seq_num;
    unsigned short int reserved;
    unsigned char checksum;
} ql_wifi_spi_data_header_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_spi_manager_s ql_wifi_spi_manager = {0};

ql_sem_t spi_dma_read_sem = NULL;
/*========================================================================
 *	function Definition
 *========================================================================*/

static void _gpioint_callback(void *ctx)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_rtos_semaphore_release(manager->recv_sem);
}

int ql_wifi_spi_gpio_init(void)
{
    ql_errcode_gpio ret;

    ql_pin_set_func(QL_SPI_RxPIN_GPIO, QL_SPI_RxPIN_GPIO_FUNC);

    ret = ql_int_register(QL_SPI_Rx_GPIO, EDGE_TRIGGER, DEBOUNCE_DIS, EDGE_FALLING, PULL_UP, _gpioint_callback, NULL);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("gpio rx init err");
        return ret;
    }
    ret = ql_int_enable(QL_SPI_Rx_GPIO);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("gpio rx ini enable err");
        return ret;
    }

    return QL_GPIO_SUCCESS;
}

void ql_wifi_spi_gpio_deinit(void)
{
    ql_int_disable(QL_SPI_Rx_GPIO);
    ql_int_unregister(QL_SPI_Rx_GPIO);
    ql_gpio_deinit(QL_SPI_Rx_GPIO);
}

int ql_wifi_spi_read(unsigned char *data, unsigned short inlen)
{
//    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_errcode_spi_e ret;

//	ql_rtos_mutex_lock(manager->spi_mutex,QL_WAIT_FOREVER);
    ql_spi_cs_low(QL_CUR_SPI_PORT);
    if(QL_SPI_TRANS_MODE == QL_SPI_DMA_IRQ)
    {
        ret = ql_spi_read(QL_CUR_SPI_PORT, data, inlen);
        ql_rtos_semaphore_wait(spi_dma_read_sem, QL_WAIT_FOREVER);
    }
    else
    {
        ret = ql_spi_read(QL_CUR_SPI_PORT, data, inlen);
    }
    ql_spi_cs_high(QL_CUR_SPI_PORT);
//	ql_rtos_mutex_unlock(manager->spi_mutex);
    return ret;
}


void ql_spi_dms_cb_handler(ql_spi_irq_s cause)
{
    if (cause.rx_dma_done == 1)
    {
        //上位机通过CS引脚判断spi数据读取完成,这里及时拉CS通知上位机.
        ql_spi_cs_high(QL_CUR_SPI_PORT);
        ql_rtos_semaphore_release(spi_dma_read_sem);
    }
}

int ql_wifi_spi_port_init(void)
{
    ql_errcode_gpio ret;

    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE ||
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
        ret = QL_GPIO_INVALID_PARAM_ERR;
        QL_WIFI_SPI_LOG("pin err");
        goto QL_SPI_EXIT;
    }

    ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }

    ret = ql_pin_set_func(QL_CUR_SPI_CLK_PIN, QL_CUR_SPI_CLK_FUNC);
    ql_pin_set_drving(QL_CUR_SPI_CLK_PIN,12);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    ql_pin_set_drving(QL_CUR_SPI_DO_PIN,12);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    ql_pin_set_drving(QL_CUR_SPI_DI_PIN,12);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }

    ql_spi_config_s spi_config;
    spi_config.input_mode = QL_SPI_INPUT_TRUE;
    spi_config.port = QL_CUR_SPI_PORT;
    spi_config.spiclk = QL_SPI_CLK;
    spi_config.framesize = 8;
    spi_config.cs_polarity0 = QL_SPI_CS_ACTIVE_LOW;
    spi_config.cs_polarity1 = QL_SPI_CS_ACTIVE_LOW;
    spi_config.cpol = QL_SPI_CPOL_LOW;
    spi_config.cpha = QL_SPI_CPHA_2Edge;
    spi_config.input_sel = QL_SPI_DI_1;
    spi_config.transmode = QL_SPI_TRANS_MODE; // QL_SPI_DMA_IRQ模式，传输一次最大的数据量只有512个字节
    spi_config.cs = QL_SPI_CS0;
    spi_config.clk_delay = QL_SPI_CLK_DELAY_0;
    ql_spi_init_ext(spi_config);

    if(QL_SPI_TRANS_MODE == QL_SPI_DMA_IRQ)
    {
        ql_spi_irq_s mask = {0};
        mask.rx_dma_done = 1;
        mask.tx_dma_done = 1;
        ql_rtos_semaphore_create(&spi_dma_read_sem, 0);
        ql_spi_set_irq(QL_CUR_SPI_PORT, mask, ql_spi_dms_cb_handler);
    }

    ql_spi_cs_auto(QL_CUR_SPI_PORT); // 接受时自动控制会导致每512个字节，cs信号重新开始

    //不允许进入慢时钟
    ql_spi_request_sys_clk(QL_CUR_SPI_PORT);

QL_SPI_EXIT:
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("quec_sio_wifi_spi_init failed");
    }
    return ret;
}

void ql_wifi_spi_port_deinit(void)
{
    ql_spi_release(QL_CUR_SPI_PORT);
}

//output data msg
int ql_wifi_spi_output_data_msg(void *ctx)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    uint16_t offset = 0;
    struct pbuf *p = (struct pbuf *)ctx;
    struct pbuf *q = NULL;

    ql_wifi_msg_protocol_head_info_s *wifi_msg_ptr = calloc(1, p->tot_len+QUEC_PROTOCOL_HEAD_MAX_SIZE);
    if (wifi_msg_ptr == NULL){
        return -1;
    }
    wifi_msg_ptr->magic0 = QL_WIFI_UART_MAGIC0;
    wifi_msg_ptr->magic1 = QL_WIFI_UART_MAGIC1;
    wifi_msg_ptr->command = QL_WIFI_NPTO_802_3_STREAM;
    wifi_msg_ptr->cmdlen = p->tot_len + QUEC_PROTOCOL_HEAD_MAX_SIZE;
    wifi_msg_ptr->data = (unsigned char *)wifi_msg_ptr + QUEC_PROTOCOL_HEAD_MAX_SIZE;

    for (q = p; q != NULL; q = q->next)
    {
        memcpy(&wifi_msg_ptr->data[offset], q->payload, q->len);
        offset += q->len;
    }

    //wifi模组不检测校验码,可以去掉.
    //wifi_msg_ptr->checksum = npto_CRC(wifi_msg_ptr, wifi_msg_ptr->cmdlen);

    if (QL_OSI_SUCCESS != ql_rtos_queue_release(manager->send_queue, sizeof(ql_wifi_msg_protocol_head_info_s), (uint8 *)&wifi_msg_ptr, QL_NO_WAIT))
    {
        QL_WIFI_SPI_LOG("release msg_queue failed");
        return -1;
    }
    return 0;
}


int ql_wifi_spi_msg_parse(unsigned char *src, unsigned int src_len, unsigned char **dst, unsigned int *dst_len)
{
    unsigned char *ptr = src;

    // data protocol parse   从机第一个数据发送0x72，需要丢弃
    ptr++; // 丢弃该数据

    *dst_len = ptr[2] | ptr[3]<<8;
    if (ptr[0] != QL_WIFI_SPI_MAGIC0 || ptr[1] != QL_WIFI_SPI_MAGIC1)
    {
        QL_WIFI_SPI_LOG("header[0]= %x header[1]= %x", ptr[0], ptr[1]);
        OSI_LOGXI(OSI_LOGPAR_M, 0, "SPI Rx : %*s", *dst_len, ptr);
        return -1;
    }
    
    *dst = &ptr[QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE];
    return 0;
}


void ql_wifi_spi_rx_thread(void *argv)
{
    ql_wifi_msg_info_s msg_ptr;
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_rtos_semaphore_wait(manager->recv_sem, QL_NO_WAIT);
    int ret = 0;
    ql_LvlMode level = LVL_LOW; 
    while (1)
    {
        ql_gpio_get_level(QL_SPI_Rx_GPIO, &level);
        if(level == LVL_HIGH)
        {
            ql_rtos_semaphore_wait(manager->recv_sem, QL_WAIT_FOREVER);
        }

        ql_wifi_spi_read(manager->recv_data, QL_WIFI_SPI_DATA_LEN);

        ret = ql_wifi_spi_msg_parse(manager->recv_data, QL_WIFI_SPI_DATA_LEN, &msg_ptr.data, &msg_ptr.data_len);
        if (ret == 0)
        {
            struct pbuf *p = pbuf_alloc(PBUF_RAW, msg_ptr.data_len, PBUF_RAM);
            if (ERR_OK != pbuf_take(p, msg_ptr.data, msg_ptr.data_len))
            {
                QL_WIFI_SPI_LOG("pbuf_take error");
                continue;
            }

            ql_rtos_queue_release(manager->recv_queue, sizeof(struct pbuf), (uint8 *)&p, QL_NO_WAIT);
        }
        else
        {
            QL_WIFI_SPI_LOG("spi msg_parse failed");
        }
    }

    ql_rtos_task_delete(NULL);
    return;
}

void ql_wifi_spi_net_thread(void *argv)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    struct pbuf *p = NULL;

    while (1)
    {
        /* Wait for message queue. */
        ql_rtos_queue_wait(manager->recv_queue, (uint8 *)&p, sizeof(struct pbuf), QL_WAIT_FOREVER);
        ql_wifi_fgm842d_data_input(p);
    }
}

void ql_wifi_uart_uplink_thread(void *argv)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_wifi_msg_protocol_head_info_s *msg = NULL;
    while(1)
    {
        /* Wait for message queue. */
        ql_rtos_queue_wait(manager->send_queue, (uint8 *)&msg, sizeof(ql_wifi_msg_protocol_head_info_s), QL_WAIT_FOREVER);
        ql_wifi_uart_write((char *)msg, msg->cmdlen);
        QL_FREE_PTR(msg);
    }
}

ql_wifi_spi_errcode_e ql_wifi_spi_init(void *argv)
{
    int ret = QL_WIFI_SPI_SUCCESS;
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    QL_WIFI_SPI_LOG("ql_wifi_spi_init");
    if(manager->recv_task)
    {
        return QL_WIFI_SPI_SUCCESS;
    }

    manager->recv_mal_data = malloc(QL_WIFI_SPI_DATA_LEN+QL_SPI_DMA_ADDR_ALIN);
    if (manager->recv_mal_data == NULL)
    {
        QL_WIFI_SPI_LOG("spi recv_data malloc failed");
    }
    // 使用QL_SPI_DMA_POLLING和QL_SPI_DMA_IRQ模式，使用的地址必须4字节对齐
    manager->recv_data = (unsigned char *)OSI_ALIGN_UP(manager->recv_mal_data, QL_SPI_DMA_ADDR_ALIN);

    ret = ql_wifi_spi_port_init();
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG("spi init failed");
    }

    ret = ql_wifi_spi_gpio_init();
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG(" gpio init failed");
    }

    ret = ql_rtos_semaphore_create(&manager->recv_sem, 1);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG("spi sem created failed");
    }

    ret = ql_rtos_mutex_create(&manager->spi_mutex);
    if( ret != QL_OSI_SUCCESS )
    {
        QL_WIFI_SPI_LOG("spi  mutex created failed");
    }

    /* Create msg queue. */
    if (QL_OSI_SUCCESS != ql_rtos_queue_create(&manager->send_queue, sizeof(ql_wifi_msg_info_s), 100))
    {
        QL_WIFI_SPI_LOG("ql_rtos_queue_create failed");
    }

    if (QL_OSI_SUCCESS != ql_rtos_queue_create(&manager->recv_queue, sizeof(ql_wifi_msg_info_s), 100))
    {
        QL_WIFI_SPI_LOG("ql_rtos_queue_create failed");
    }

    if(QL_OSI_SUCCESS != ql_rtos_task_create(&manager->recv_task, 1024, APP_PRIORITY_REALTIME, "ql_wifi_spirx", ql_wifi_spi_rx_thread, NULL, QL_WIFI_SPI_TASK_EVENT_CNT))
    {
        QL_WIFI_SPI_LOG("ql_wifi_spi_init err");
        ret = QL_WIFI_SPI_TASK_CREATE_ERR;
        return ret;
    }

    if(QL_OSI_SUCCESS != ql_rtos_task_create(&manager->net_task, 4096, APP_PRIORITY_REALTIME-1, "ql_wifi_spi_net", ql_wifi_spi_net_thread, NULL, QL_WIFI_SPI_TASK_EVENT_CNT))
    {
        QL_WIFI_SPI_LOG("ql_wifi_spi_init err");
        ret = QL_WIFI_SPI_TASK_CREATE_ERR;
        return ret;
    }

    if(QL_OSI_SUCCESS != ql_rtos_task_create(&manager->send_task, 1024, APP_PRIORITY_REALTIME-1, "ql_wifi_uarttx", ql_wifi_uart_uplink_thread, NULL, QL_WIFI_SPI_TASK_EVENT_CNT))
    {
        QL_WIFI_SPI_LOG("ql_wifi_spi_init err");
        ret = QL_WIFI_SPI_TASK_CREATE_ERR;
        return ret;
    }


    return ret;
}

ql_wifi_spi_errcode_e ql_wifi_spi_deinit(void *argv)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_wifi_spi_manager_s cmp = {0};
    if(0 != memcpy(manager,&cmp,sizeof(ql_wifi_spi_manager_s)))
    {
        QL_WIFI_SPI_LOG("ql_wifi_spi_deinit");
        ql_wifi_spi_port_deinit();
        ql_wifi_spi_gpio_deinit();

        if (manager->recv_task)
        {
            ql_rtos_task_delete(manager->recv_task);
            manager->recv_task = NULL;
        }

        QL_FREE_PTR(manager->recv_mal_data);

        if(manager->spi_mutex){
            ql_rtos_mutex_unlock(manager->spi_mutex);
            ql_rtos_mutex_delete(manager->spi_mutex);
            manager->spi_mutex = NULL;
        }

        if (manager->recv_sem)
        {
            ql_rtos_semaphore_delete(manager->recv_sem);
            manager->recv_sem = NULL;
        }
        memset(manager,0,sizeof(ql_wifi_spi_manager_s));
    }
    else
    {
        QL_WIFI_SPI_LOG("wifi spi repeat deinit");
    }
    return QL_WIFI_SPI_SUCCESS;
}
