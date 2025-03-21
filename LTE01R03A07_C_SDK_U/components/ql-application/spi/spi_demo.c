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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_api_spi.h"

#include "ql_log.h"
#include "spi_demo.h"
#include "ql_gpio.h"
#include "ql_power.h"


#define QL_SPI_DEMO_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_SPI_DEMO_LOG(msg, ...)			    QL_LOG(QL_SPI_DEMO_LOG_LEVEL, "ql_SPI_DEMO", msg, ##__VA_ARGS__)
#define QL_SPI_DEMO_LOG_PUSH(msg, ...)	        QL_LOG_PUSH("ql_SPI_DEMO", msg, ##__VA_ARGS__)



/**
 *  使用SPI DMA注意事项：
 *  1. SPI DMA POLLING和SPI DMA IRQ只支持8bit和16bit的数据传输，不支持32bit数据传输
 *  2. 在使用16bit传输数据时，DMA实际使用的是32bit位宽，需要对输出数据插入一些无效数据，对输入数据去除无效数据，
 *     因此新增16bit dma api用于16bit情况下的读写，api包含ql_spi_write_16bit_dma、ql_spi_read_16bit_dma、
 *     ql_spi_write_read_16bit_dma，这部分代码在demo中开源，客户可自行优化，或直接使用
 *  3. QL_SPI_16BIT_DMA置为1表示使用16bit DMA demo
 */
#define QL_SPI_16BIT_DMA                   0        //16bit DMA demo
#define QL_SPI_DEMO_LOW_POWER_USE          0        //0-not run in lower power mode；1-run in lower power mode

ql_task_t spi_demo_task = NULL;
ql_sem_t  spi_demo_write;
ql_sem_t  spi_demo_read;
int spi_power_lock = 0;


#define QL_SPI_DEMO_WAIT_NONE              0
#define QL_SPI_DEMO_WAIT_WRITE             1
#define QL_SPI_DEMO_WAIT_READ              2

unsigned char spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_NONE;

#define QL_CUR_SPI_PORT             QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI1_DI_FUNC



#define QL_TYPE_SHIFT_8             8

uint32_t g_inbuf[QL_SPI_DMA_IRQ_SIZE/4] OSI_CACHE_LINE_ALIGNED;
uint32_t g_outbuf[QL_SPI_DMA_IRQ_SIZE/4] OSI_CACHE_LINE_ALIGNED;

void ql_spi_read_data_transform(unsigned char *buf, unsigned int len)
{
    if(len%2 != 0 || len > QL_SPI_DMA_IRQ_SIZE/2)
    {
        QL_SPI_DEMO_LOG("invalid parm");
        return;
    }

    for(int i = 0; i < len/2; i++)
    {
        buf[i*2] = (g_inbuf[i] >> QL_TYPE_SHIFT_8) & 0xFF;
        buf[i*2+1] = g_inbuf[i] & 0xFF;
    }
}

ql_errcode_spi_e ql_spi_write_16bit_dma(ql_spi_port_e port, unsigned char *buf, unsigned int len)
{
    if(len%2 != 0 || len > QL_SPI_DMA_IRQ_SIZE/2)
    {
        QL_SPI_DEMO_LOG("invalid parm");
        return QL_SPI_PARAM_DATA_ERROR;
    }
    
    unsigned short out_temp = 0;

    for(int i = 0; i < len/2; i++)
    {
        out_temp = buf[i*2];
        g_outbuf[i] = (out_temp << QL_TYPE_SHIFT_8) + buf[i*2+1];
    }

    return ql_spi_write(port, (unsigned char*)g_outbuf, len*2);
}

ql_errcode_spi_e ql_spi_read_16bit_dma(ql_spi_port_e port, unsigned char *buf, unsigned int len)
{
    if(len%2 != 0 || len > QL_SPI_DMA_IRQ_SIZE/2)
    {
        QL_SPI_DEMO_LOG("invalid parm");
        return QL_SPI_PARAM_DATA_ERROR;
    }

    return ql_spi_read(port, (unsigned char*)g_inbuf, len*2);
}

ql_errcode_spi_e ql_spi_write_read_16bit_dma(ql_spi_port_e port, unsigned char *inbuf, unsigned char *outbuf, unsigned int len)
{
    if(len%2 != 0 || len > QL_SPI_DMA_IRQ_SIZE/2)
    {
        QL_SPI_DEMO_LOG("invalid parm");
        return QL_SPI_PARAM_DATA_ERROR;
    }
    unsigned short out_temp = 0;
    
    for(int i = 0; i < len/2; i++)
    {
        out_temp = outbuf[i*2];
        g_outbuf[i] = (out_temp << QL_TYPE_SHIFT_8) + outbuf[i*2+1];
    }
    
    return ql_spi_write_read(port, (unsigned char*)g_inbuf, (unsigned char*)g_outbuf, len*2);
}

void ql_spi_flash_data_printf(unsigned char *data, int len)
{
    int i = 0;
    int count_len = 256;
    int count = 0;
    int exit = 0;
    unsigned int buf_pos = 0;

    unsigned char *str_data = (unsigned char *)malloc(len*2+64);
    if (str_data == NULL)
    {
        QL_SPI_DEMO_LOG("malloc err");
        return ; 
    }

    QL_SPI_DEMO_LOG("read len=%d", len);
    
    while ((exit == 0))
    {
        if (len - count > 256)
        {
            count_len = 256;
        }
        else 
        {
            count_len = len - count;
            exit = 1;
        }
        
        memset(str_data, 0, len*2+64);
        buf_pos = 0;
        for (i=count; i<count+count_len; i++)
        {
            //sprintf((char *)str_data,"%s%02x", str_data, data[i]);
            buf_pos += sprintf((char *)(str_data + buf_pos),"%02x",data[i]);
        }
        
        QL_SPI_DEMO_LOG("data[%d-%d]=%s", count, count+count_len, str_data);
        count += count_len;
    }

    free(str_data);
}

void ql_spi_demo_cb_handler(ql_spi_irq_s cause)
{
    if (cause.tx_dma_done == 1 && spi_demo_wait_write_read == QL_SPI_DEMO_WAIT_WRITE)
    {
        spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_NONE;
        ql_rtos_semaphore_release(spi_demo_write);
    }

    if (cause.rx_dma_done == 1 && spi_demo_wait_write_read == QL_SPI_DEMO_WAIT_READ)
    {
        spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_NONE;
        ql_rtos_semaphore_release(spi_demo_read);
    }

    QL_SPI_DEMO_LOG("cause.tx_dma_done=%d", cause.tx_dma_done);
    QL_SPI_DEMO_LOG("cause.rx_dma_done=%d", cause.rx_dma_done);

}

static void ql_spi_demo_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    unsigned char *outdata = NULL;
    unsigned short outlen;
    unsigned char *indata = NULL;
    unsigned short inlen;
    unsigned char *out_mal_data = NULL;
    unsigned char *in_mal_data = NULL;
    ql_errcode_gpio ret;
    ql_spi_clk_e spiclk;
    ql_spi_transfer_mode_e transmode;
    unsigned int framesize;
    unsigned int tx_free = 0;
    ql_spi_config_s spi_config = {0};
    
    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || \
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
        QL_SPI_DEMO_LOG("pin err");
        goto QL_SPI_EXIT;
    }

    ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_DEMO_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    
    ret = ql_pin_set_func(QL_CUR_SPI_CLK_PIN, QL_CUR_SPI_CLK_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_DEMO_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_DEMO_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    if (ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_DEMO_LOG("set pin err");
        goto QL_SPI_EXIT;
    }
    
    //If you use the default parameters, you can initialize it with ql_spi_init
  /*transmode = QL_SPI_DMA_IRQ;
    spiclk = QL_SPI_CLK_25MHZ;
    framesize = 8;
    ql_spi_init(QL_CUR_SPI_PORT, transmode, spiclk);
  */

    spi_config.input_mode = QL_SPI_INPUT_TRUE;
    spi_config.port = QL_CUR_SPI_PORT;

#if QL_SPI_16BIT_DMA    
    framesize = 16;
#else
    framesize = 8;
#endif
    transmode = QL_SPI_DMA_IRQ; //使用QL_SPI_DMA_IRQ模式，传输一次最大的数据量为512个字节
    spiclk = QL_SPI_CLK_1_5625MHZ;
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
    
    //使用QL_SPI_DMA_IRQ模式才会使用到信号量
    if(transmode == QL_SPI_DMA_IRQ)
    {
        ql_spi_irq_s mask = {0};
        mask.rx_dma_done = 1;
        mask.tx_dma_done = 1;
        //mask.tx_threshold = QL_SPI_TRIGGER_4_DATA;
        //mask.rx_threshold = QL_SPI_TRIGGER_4_DATA;
        
        ql_rtos_semaphore_create(&spi_demo_write, 0);
        ql_rtos_semaphore_create(&spi_demo_read, 0);
        ql_spi_set_irq(QL_CUR_SPI_PORT, mask, ql_spi_demo_cb_handler);
    }
    
    //用来测试SPI CS脚可以强制拉低，CS脚恢复成系统控制后，不需要强制拉高拉低
    ql_spi_cs_low(QL_CUR_SPI_PORT);
    ql_rtos_task_sleep_s(3);
    ql_spi_cs_auto(QL_CUR_SPI_PORT);

    //使用QL_SPI_DMA_IRQ模式，传输一次最大的数据量为512个字节,如果采用16bit传输数据，需要填充256字节无效数据用于DMA对齐，实际有效最大数据量为256字节
#if QL_SPI_16BIT_DMA
    outlen = QL_SPI_DMA_IRQ_SIZE/2;
    inlen = QL_SPI_DMA_IRQ_SIZE/2;
#else
    outlen = QL_SPI_DMA_IRQ_SIZE;
    inlen = QL_SPI_DMA_IRQ_SIZE;
#endif
    out_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN+outlen);
    in_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN+outlen);
    if (out_mal_data == NULL || in_mal_data == NULL)
    {
        QL_SPI_DEMO_LOG("malloc err");
        goto QL_SPI_EXIT;
    }

    //使用QL_SPI_DMA_POLLING和QL_SPI_DMA_IRQ模式，使用的地址必须4字节对齐
    if(transmode == QL_SPI_DMA_POLLING || transmode == QL_SPI_DMA_IRQ)
    {
        outdata = (unsigned char *)OSI_ALIGN_UP(out_mal_data, QL_SPI_DMA_ADDR_ALIN);
        indata = (unsigned char *)OSI_ALIGN_UP(in_mal_data, QL_SPI_DMA_ADDR_ALIN);
    }
    else
    {
        outdata = out_mal_data;
        indata = in_mal_data;
    }
    
    memset(outdata, 0x00, outlen);
    memset(indata, 0x00, inlen);
    int i = 0, temp = 0;
    for (i=0; i<outlen; i++)
    {
        temp = i % 256;
        outdata[i] = temp;
    }

    ql_spi_flash_data_printf(outdata, outlen);
#if QL_SPI_DEMO_LOW_POWER_USE    
    ql_autosleep_enable(QL_ALLOW_SLEEP);
    ql_rtos_task_sleep_s(2);
#endif
    while(1)
    {
#if QL_SPI_DEMO_LOW_POWER_USE
        ql_lpm_wakelock_lock(spi_power_lock);
#endif
        ql_spi_cs_low(QL_CUR_SPI_PORT);
    
        if(transmode == QL_SPI_DMA_IRQ)
        {
            spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_WRITE;
            //不允许进入慢时钟
            ql_spi_request_sys_clk(QL_CUR_SPI_PORT);
        }
#if QL_SPI_16BIT_DMA
        ql_spi_write_16bit_dma(QL_CUR_SPI_PORT, outdata, outlen);
#else
        ql_spi_write(QL_CUR_SPI_PORT, outdata, outlen);
#endif
        if(transmode == QL_SPI_DMA_IRQ)
        {
            ql_rtos_semaphore_wait(spi_demo_write, QL_WAIT_FOREVER);
            //tx_dma_done只是DMA完成了，但是SPI的FIFO还可能存在数据未发送，在进入慢时钟或clk频率较低时出现
            ql_spi_get_tx_fifo_free(QL_CUR_SPI_PORT, &tx_free);
            QL_SPI_DEMO_LOG("tx_free=%d",tx_free);
            ql_delay_us((framesize+2)*(QL_SPI_FIFO_SIZE - tx_free)*1000000/spiclk);
            //恢复允许进入慢时钟
            ql_spi_release_sys_clk(QL_CUR_SPI_PORT);
        }

        if(transmode == QL_SPI_DMA_IRQ)
        {
            spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_READ;
            //不允许进入慢时钟
            ql_spi_request_sys_clk(QL_CUR_SPI_PORT);
        }
#if QL_SPI_16BIT_DMA
        ql_spi_read_16bit_dma(QL_CUR_SPI_PORT, indata, inlen);
#else
        ql_spi_read(QL_CUR_SPI_PORT, indata, inlen);
#endif
        if(transmode == QL_SPI_DMA_IRQ)
        {
            ql_rtos_semaphore_wait(spi_demo_read, QL_WAIT_FOREVER);
            //恢复允许进入慢时钟
            ql_spi_release_sys_clk(QL_CUR_SPI_PORT);
#if QL_SPI_16BIT_DMA
            //清除dma输入的无效数据
            ql_spi_read_data_transform(indata, inlen);
#endif
        }
       
        if(transmode == QL_SPI_DMA_IRQ)
        {
            spi_demo_wait_write_read = QL_SPI_DEMO_WAIT_READ;
            //不允许进入慢时钟
            ql_spi_request_sys_clk(QL_CUR_SPI_PORT);
        }
#if QL_SPI_16BIT_DMA
        ql_spi_write_read_16bit_dma(QL_CUR_SPI_PORT, indata, outdata, inlen);
#else        
        ql_spi_write_read(QL_CUR_SPI_PORT, indata, outdata, inlen);
#endif
        if(transmode == QL_SPI_DMA_IRQ)
        {
            ql_rtos_semaphore_wait(spi_demo_read, QL_WAIT_FOREVER);
            //恢复允许进入慢时钟
            ql_spi_release_sys_clk(QL_CUR_SPI_PORT);
#if QL_SPI_16BIT_DMA
            //清除dma输入的无效数据
            ql_spi_read_data_transform(indata, inlen);
#endif
        }
        //ql_spi_flash_data_printf(indata, inlen);

#if 0
//测试同步发送接收，需要将模块MOSI和MISO脚短接起来
        if (memcmp(indata, outdata, inlen))
        {
            QL_SPI_DEMO_LOG("write_read not match");
            ql_spi_flash_data_printf(indata, inlen);
            ql_spi_flash_data_printf(outdata, inlen);
            ql_spi_release(QL_CUR_SPI_PORT);
            free(out_mal_data);
            free(in_mal_data);
            goto QL_SPI_EXIT;
        }
#endif
        
        ql_spi_cs_high(QL_CUR_SPI_PORT);
#if QL_SPI_DEMO_LOW_POWER_USE
        ql_lpm_wakelock_unlock(spi_power_lock);
#endif
        //ql_spi_read_follow_write接口不支持QL_SPI_DMA_IRQ模式
        if(transmode != QL_SPI_DMA_IRQ)
        {
            ql_spi_read_follow_write(QL_CUR_SPI_PORT, outdata, outlen, indata, inlen);
            ql_rtos_task_sleep_ms(100);
        }
        ql_rtos_task_sleep_s(2);
    }   
    ql_spi_release(QL_CUR_SPI_PORT);
QL_SPI_EXIT:
    if(out_mal_data != NULL)
    {
        free(out_mal_data);
        out_mal_data = NULL;
    }
    if(in_mal_data != NULL)
    {
        free(in_mal_data);
        in_mal_data = NULL;
    }
    QL_SPI_DEMO_LOG("ql_rtos_task_delete");
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI_DEMO_LOG("task deleted failed");
	}
}

QlOSStatus ql_spi_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;
#if QL_SPI_DEMO_LOW_POWER_USE
    spi_power_lock = ql_lpm_wakelock_create("spi_irq", strlen("spi_irq"));
#endif
	err = ql_rtos_task_create(&spi_demo_task, SPI_DEMO_TASK_STACK_SIZE, SPI_DEMO_TASK_PRIO, "ql_spi_demo", ql_spi_demo_task_pthread, NULL, SPI_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI_DEMO_LOG("demo_task created failed");
        return err;
	}
    
    return err;
}

