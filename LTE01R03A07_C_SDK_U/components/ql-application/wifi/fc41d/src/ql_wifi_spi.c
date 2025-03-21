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

#include "lwipopts.h"

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_SPI_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_spi", msg, ##__VA_ARGS__)

#define QL_WIFI_SPI_TASK_EVENT_CNT 5

#define QL_WIFI_SPI_MAGIC0 0xAA
#define QL_WIFI_SPI_MAGIC1 0x55
#define QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE 9
#define QL_WIFI_SPI_DATA_LEN 1600

#define QL_CUR_SPI_PORT QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC QL_CUR_SPI1_DI_FUNC

#define QL_SPI_TxPIN_GPIO QUEC_PIN_DNAME_GPIO_1
#define QL_SPI_Tx_GPIO GPIO_1
#define QL_SPI_RxPIN_GPIO QUEC_PIN_DNAME_GPIO_2
#define QL_SPI_Rx_GPIO GPIO_2
#define QL_PIN_GPIO_FUNC_GPIO 0
/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t recv_task;
    ql_sem_t recv_sem;
    ql_mutex_t recv_mutex;
    unsigned char *recv_data;
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

/*========================================================================
 *	function Definition
 *========================================================================*/
static void _gpioint_callback(void *ctx)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    ql_LvlMode gpio_lvl;
    ql_gpio_get_level(QL_SPI_Rx_GPIO, &gpio_lvl);
    QL_WIFI_SPI_LOG("gpio[%d] int lvl:[%d]", QL_SPI_Rx_GPIO, gpio_lvl);
    ql_rtos_semaphore_release(manager->recv_sem);
}

int ql_wifi_spi_gpio_init(void)
{
    ql_errcode_gpio ret;

    ql_pin_set_func(QL_SPI_TxPIN_GPIO, QL_PIN_GPIO_FUNC_GPIO);             // P58 set GPIO1
    ql_pin_set_func(QL_SPI_RxPIN_GPIO, QL_PIN_GPIO_FUNC_GPIO);             // P59 set GPIO2

    ql_gpio_deinit(QL_SPI_Tx_GPIO);

    ret = ql_gpio_init(QL_SPI_Tx_GPIO, GPIO_OUTPUT, PULL_NONE, LVL_HIGH);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("gpio tx init err");
        return ret;
    }

    ret = ql_int_register(QL_SPI_Rx_GPIO, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_FALLING, PULL_UP, _gpioint_callback, NULL);
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
    ql_gpio_deinit(QL_SPI_Tx_GPIO);
    ql_gpio_deinit(QL_SPI_Rx_GPIO);
}

int ql_wifi_spi_read(unsigned char *data, unsigned short inlen)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    unsigned char *indata = NULL;
    unsigned char *in_mal_data = NULL;
    ql_errcode_spi_e ret;

    if (data == NULL || inlen <= 0)
        return QL_SPI_PARAM_TYPE_ERROR;

    in_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN + inlen);

    if (in_mal_data == NULL)
    {
        QL_WIFI_SPI_LOG("spi read malloc err");
        return QL_SPI_MALLOC_MEM_ERROR;
    }

    // 使用QL_SPI_DMA_POLLING和QL_SPI_DMA_IRQ模式，使用的地址必须4字节对齐
    indata = (unsigned char *)OSI_ALIGN_UP(in_mal_data, QL_SPI_DMA_ADDR_ALIN);

    memset(indata, 0, inlen);

	ql_rtos_mutex_lock(manager->recv_mutex,QL_WAIT_FOREVER);
    ql_delay_us(300); // cbq
    ql_spi_cs_low(QL_CUR_SPI_PORT);
    ret = ql_spi_read(QL_CUR_SPI_PORT, indata, inlen);
    ql_spi_cs_high(QL_CUR_SPI_PORT);
	ql_rtos_mutex_unlock(manager->recv_mutex);

    if (ret == QL_SPI_SUCCESS)
    {
        memcpy(data, indata, inlen);
    }
    else
    {
        QL_WIFI_SPI_LOG("spi read ret = %d \n", ret);
    }

    free(in_mal_data);
    return ret;
}

int ql_wifi_spi_write(unsigned char *data, unsigned short outlen)
{
    unsigned char *outdata = NULL;
    unsigned char *out_mal_data = NULL;
    ql_errcode_spi_e ret;

    if (data == NULL || outlen <= 0)
        return QL_SPI_PARAM_TYPE_ERROR;

    out_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN + outlen);

    if (out_mal_data == NULL)
    {
        QL_WIFI_SPI_LOG("spi write malloc err");
        return QL_SPI_MALLOC_MEM_ERROR;
    }

    // 使用QL_SPI_DMA_POLLING和QL_SPI_DMA_IRQ模式，使用的地址必须4字节对齐
    outdata = (unsigned char *)OSI_ALIGN_UP(out_mal_data, QL_SPI_DMA_ADDR_ALIN);

    memcpy(outdata, data, outlen);

    ql_spi_cs_low(QL_CUR_SPI_PORT);
    ret = ql_spi_write(QL_CUR_SPI_PORT, outdata, outlen);
    ql_spi_cs_high(QL_CUR_SPI_PORT);

    if (ret != QL_SPI_SUCCESS)
    {
        QL_WIFI_SPI_LOG("spi write ret = %d \n", ret);
    }
    free(out_mal_data);

    return ret;
}

int ql_wifi_spi_port_init(void)
{
    ql_errcode_gpio ret;
    ql_spi_clk_e spiclk;
    ql_spi_transfer_mode_e transmode;
    unsigned int framesize;

    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE ||
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
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
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_WIFI_SPI_LOG("set pin err");
        goto QL_SPI_EXIT;
    }

    ql_spi_config_s spi_config;
    spi_config.input_mode = QL_SPI_INPUT_TRUE;
    spi_config.port = QL_CUR_SPI_PORT;
    framesize = 8;
    transmode = QL_SPI_DMA_POLLING; // QL_SPI_DMA_IRQ模式，传输一次最大的数据量只有512个字节
    spiclk = QL_SPI_CLK_5MHZ;
    spi_config.spiclk = spiclk;
    spi_config.framesize = framesize;
    spi_config.cs_polarity0 = QL_SPI_CS_ACTIVE_LOW;
    spi_config.cs_polarity1 = QL_SPI_CS_ACTIVE_LOW;
    spi_config.cpol = QL_SPI_CPOL_LOW;
    spi_config.cpha = QL_SPI_CPHA_1Edge;
    spi_config.input_sel = QL_SPI_DI_1;
    spi_config.transmode = transmode;
    spi_config.cs = QL_SPI_CS0;
    spi_config.clk_delay = QL_SPI_CLK_DELAY_0;
    ql_spi_init_ext(spi_config);

    ql_spi_cs_auto(QL_CUR_SPI_PORT); // 接受时自动控制会导致每512个字节，cs信号重新开始

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

// spi output data msg
int ql_wifi_spi_output_data_msg(void *ctx)
{
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    struct pbuf *p = (struct pbuf *)ctx;
    uint8_t *data = p->payload;
    int data_len = p->tot_len;

    unsigned char checksum;
    ql_wifi_spi_data_header_s ql_wifi_spi_data;

    unsigned char *output_data = malloc(QL_WIFI_SPI_DATA_LEN);
    if (output_data == NULL)
    {
        return -1;
    }
    ql_rtos_mutex_lock(manager->recv_mutex,QL_WAIT_FOREVER);
    ql_gpio_set_level(QL_SPI_Tx_GPIO, LVL_LOW); // 通知从机准备接收
    ql_delay_us(1700);
    memset(output_data, 0, QL_WIFI_SPI_DATA_LEN);

    QL_WIFI_SPI_LOG("spi send data len=%d", data_len);

    ql_wifi_spi_data.header[0] = QL_WIFI_SPI_MAGIC0;
    ql_wifi_spi_data.header[1] = QL_WIFI_SPI_MAGIC1;

    ql_wifi_spi_data.data_len = (unsigned short int)data_len;
    ql_wifi_spi_data.seq_num = 0;
    ql_wifi_spi_data.reserved = 0;
    ql_wifi_spi_data.checksum = 0;

    memcpy(output_data, &ql_wifi_spi_data, QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE);
    memcpy(output_data + QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE, data, data_len);

    checksum = npto_CRC(output_data, QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE + data_len);
    output_data[QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE - 1] = checksum;

    ql_wifi_spi_write(output_data, QL_WIFI_SPI_DATA_LEN);
    ql_gpio_set_level(QL_SPI_Tx_GPIO, LVL_HIGH);
    ql_rtos_mutex_unlock(manager->recv_mutex);

    QL_WIFI_SPI_LOG("wifi out size/%d ", data_len);

    if (output_data != NULL)
    {
        free(output_data);
        output_data = NULL;
    }

    return 0;
}


int ql_wifi_spi_msg_parse(unsigned char *src, unsigned int src_len, unsigned char **dst, unsigned int *dst_len)
{

    unsigned char *ptr = src;
    unsigned char checksum;
    ql_wifi_spi_data_header_s ql_wifi_spi_data;

#if 0 // debug
    int i = 0;
    for (i = 0; i < src_len; i++)
    {
        MSG_WIFI_PRINTF("src[%d]=%x", i, ptr[i]);
    }
#endif
    // data protocol parse   从机第一个数据发送0x72，需要丢弃
    ptr++; // 丢弃该数据

    memcpy(&ql_wifi_spi_data, ptr, QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE);
    if (ql_wifi_spi_data.header[0] != QL_WIFI_SPI_MAGIC0 || ql_wifi_spi_data.header[1] != QL_WIFI_SPI_MAGIC1 || ql_wifi_spi_data.data_len <= 0)

    {
        QL_WIFI_SPI_LOG("header[0]= %x header[1]= %x data len =%d", ql_wifi_spi_data.header[0], ql_wifi_spi_data.header[1], ql_wifi_spi_data.data_len);
        return -1;
    }

    ptr[QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE - 1] = 0; // set checksum zero for recalc
    checksum = npto_CRC(ptr, ql_wifi_spi_data.data_len + QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE);
    if (checksum != ql_wifi_spi_data.checksum)
    {
        QL_WIFI_SPI_LOG("crc check error checksum=%x ql_wifi_spi_data.checksum=%x", checksum, ql_wifi_spi_data.checksum);

        return -2;
    }
    *dst_len = ql_wifi_spi_data.data_len;
    *dst = &ptr[QL_WIFI_SPI_PROTOCOL_HEAD_MAX_SIZE];

    return 0;
}

void ql_wifi_spi_thread(void *argv)
{
    int err = 0;
    ql_wifi_msg_info_s msg_ptr;
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    manager->recv_data = malloc(QL_WIFI_SPI_DATA_LEN);
    if (manager->recv_data == NULL)
    {
        QL_WIFI_SPI_LOG("spi recv_data malloc failed");
        goto exit;
    }

    err = ql_wifi_spi_port_init();
    if (err != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG("spi init failed");
        goto exit;
    }

    err = ql_wifi_spi_gpio_init();
    if (err != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG(" gpio init failed");
        goto exit;
    }

    err = ql_rtos_semaphore_create(&manager->recv_sem, 1);
    if (err != QL_OSI_SUCCESS)
    {
        QL_WIFI_SPI_LOG("spi sem created failed");
        goto exit;
    }

    err = ql_rtos_mutex_create(&manager->recv_mutex);
    if( err != QL_OSI_SUCCESS )
    {
        QL_WIFI_SPI_LOG("spi  mutex created failed");
        goto exit;
    }

    while (1)
    {
        memset(manager->recv_data, 0, QL_WIFI_SPI_DATA_LEN);
        ql_rtos_semaphore_wait(manager->recv_sem, QL_WAIT_FOREVER);

        if (ql_wifi_spi_read(manager->recv_data, QL_WIFI_SPI_DATA_LEN) == 0)
        {

            QL_WIFI_SPI_LOG("spi  read success");

            if (ql_wifi_spi_msg_parse(manager->recv_data, QL_WIFI_SPI_DATA_LEN, &msg_ptr.data, &msg_ptr.data_len) == 0)
            {
                QL_WIFI_SPI_LOG("spi data parse success");
                ql_wifi_data_input(msg_ptr.data,msg_ptr.data_len);
            }
            else
            {
                QL_WIFI_SPI_LOG("spi msg_parse failed");
            }
        }
        else
        {
            QL_WIFI_SPI_LOG("spi read failed");
        }
    }

exit:
    ql_rtos_task_delete(NULL);
    return;
}

ql_wifi_spi_errcode_e ql_wifi_spi_init(void *argv)
{
    ql_wifi_spi_errcode_e ret = QL_WIFI_SPI_SUCCESS;
    ql_wifi_spi_manager_s *manager = &ql_wifi_spi_manager;
    QL_WIFI_SPI_LOG("ql_wifi_spi_init");
    if(manager->recv_task)
    {
        return QL_WIFI_SPI_SUCCESS;
    }
    if(QL_OSI_SUCCESS != ql_rtos_task_create(&manager->recv_task, 4096, APP_PRIORITY_ABOVE_NORMAL, "ql_wifi_spi", ql_wifi_spi_thread, NULL, QL_WIFI_SPI_TASK_EVENT_CNT))
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

        if (manager->recv_data)
        {
            free(manager->recv_data);
            manager->recv_data = NULL;
        }

        if(manager->recv_mutex){
            ql_rtos_mutex_unlock(manager->recv_mutex);
            ql_rtos_mutex_delete(manager->recv_mutex);
            manager->recv_mutex = NULL;
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
