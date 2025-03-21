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

#include "ql_cnktd_spi.h"

#include "ql_log.h"

#define QL_CNKTD_SPI_LOG_LEVEL                 QL_LOG_LEVEL_INFO
#define QL_CNKTD_SPI_LOG(msg, ...)             QL_LOG(QL_CNKTD_SPI_LOG_LEVEL, "ql_spi", msg, ##__VA_ARGS__)
#define QL_CNKTD_SPI_LOG_PUSH(msg, ...)        QL_LOG_PUSH("ql_cnktd", msg, ##__VA_ARGS__)

#define QL_CNKTD_TYPE_SHIFT_8             8


typedef struct
{
    bool is_init;
    ql_spi_port_e port;
    ql_spi_config_s spi_config;
    ql_sem_t write_read;
    ql_spi_callback cb;
    qapi_SPIM_Callback_Fn_t c_Fn;
    void *c_Ctxt;
}ql_cnktd_spi_info_s;

static ql_cnktd_spi_info_s ql_cnktd_spi_info[2] = {0};
uint32_t g_ql_cnktd_inbuf[QL_SPI_DMA_IRQ_SIZE/4] OSI_CACHE_LINE_ALIGNED;
uint32_t g_ql_cnktd_outbuf[QL_SPI_DMA_IRQ_SIZE/4] OSI_CACHE_LINE_ALIGNED;

void ql_cnktd_spi1_cb(ql_spi_irq_s cause)
{
    ql_cnktd_spi_info_s *spi_info = &ql_cnktd_spi_info[0];

    if (cause.rx_dma_done == 1)
    {
        ql_rtos_semaphore_release(spi_info->write_read);
    }

    QL_CNKTD_SPI_LOG("spi1 cause.tx_dma_done=%d", cause.tx_dma_done);
    QL_CNKTD_SPI_LOG("spi1 cause.rx_dma_done=%d", cause.rx_dma_done);

    if (spi_info->c_Fn)
    {
        spi_info->c_Fn(0, spi_info->c_Ctxt);
    }
}

void ql_cnktd_spi2_cb(ql_spi_irq_s cause)
{
    ql_cnktd_spi_info_s *spi_info = &ql_cnktd_spi_info[1];

    if (cause.rx_dma_done == 1)
    {
        ql_rtos_semaphore_release(spi_info->write_read);
    }

    QL_CNKTD_SPI_LOG("spi2 cause.tx_dma_done=%d", cause.tx_dma_done);
    QL_CNKTD_SPI_LOG("spi2 cause.rx_dma_done=%d", cause.rx_dma_done);

    if (spi_info->c_Fn)
    {
        spi_info->c_Fn(0, spi_info->c_Ctxt);
    }
}


int spi_qapi_handler(int request_ID, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8)
{
    qapi_Status_t result = QAPI_OK;
    qapi_SPIM_Descriptor_t *desc = NULL;
    void *manager_cb = NULL;

    switch(request_ID) 
    {
        case TXM_QAPI_SPI_OPEN:
            result = qapi_SPIM_Open ((qapi_SPIM_Instance_t)arg1, (void **)arg2);
        break;
        case TXM_QAPI_SPI_FULL_DUPLEX:
        {
            desc = (qapi_SPIM_Descriptor_t *)arg3;
    		if ((NULL == desc) || (desc->len == 0))
    		{
    			result = QAPI_ERR_INVALID_PARAM;
                break;
            }

    		if(NULL  == desc->tx_buf || NULL  == desc->rx_buf)
    		{
    			result = QAPI_ERR_INVALID_PARAM;
    			break;
    		}

            manager_cb = (void *)arg5;
            result = qapi_SPIM_Full_Duplex ((void *)arg1, (qapi_SPIM_Config_t *) arg2, 
                                     (qapi_SPIM_Descriptor_t *)arg3, (uint32_t) arg4,
									 (qapi_SPIM_Callback_Fn_t)manager_cb, (void *)arg6, (qbool_t) arg7);
        }
        break;	
        case TXM_QAPI_SPI_POWER_ON:
            result = qapi_SPIM_Power_On ((void *)arg1);
        break;		
        case TXM_QAPI_SPI_POWER_OFF:
            result = qapi_SPIM_Power_Off ((void *)arg1);
        break;				
        default:
            result = QAPI_ERROR;
        break;
    }
    return result;
}

qapi_Status_t qapi_SPIM_Open (qapi_SPIM_Instance_t instance, void **spi_Handle)
{
    if((instance != QAPI_SPIM_INSTANCE_1_E) && (instance != QAPI_SPIM_INSTANCE_2_E))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (instance == QAPI_SPIM_INSTANCE_1_E)
    {
        ql_cnktd_spi_info[0].port = QL_SPI_PORT1;
        ql_cnktd_spi_info[0].cb = ql_cnktd_spi1_cb;
        *spi_Handle = &ql_cnktd_spi_info[0];
    }
    else 
    {
        ql_cnktd_spi_info[1].port = QL_SPI_PORT2;
        ql_cnktd_spi_info[1].cb = ql_cnktd_spi2_cb;
        *spi_Handle = &ql_cnktd_spi_info[1];
    }
    
    return QAPI_OK;
}

qapi_Status_t qapi_SPIM_Power_On (void *spi_Handle)
{
    ql_errcode_gpio ret = QL_GPIO_SUCCESS;

    if (spi_Handle == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_cnktd_spi_info_s *spi_info = spi_Handle;

    ql_spi_port_e port = spi_info->port;
    QL_CNKTD_SPI_LOG("port=%d", port);

    if ((port != QL_SPI_PORT1) && (port != QL_SPI_PORT2))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if(port == QL_SPI_PORT1)
    {
        ret = ql_pin_set_func(QL_CUR_SPI1_CS_PIN, QL_CUR_SPI1_CS_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        
        ret = ql_pin_set_func(QL_CUR_SPI1_CLK_PIN, QL_CUR_SPI1_CLK_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        ret = ql_pin_set_func(QL_CUR_SPI1_DO_PIN, QL_CUR_SPI1_DO_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        ret = ql_pin_set_func(QL_CUR_SPI1_DI_PIN, QL_CUR_SPI1_DI_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
    }
    else 
    {
        ret = ql_pin_set_func(QL_CUR_SPI2_CS_PIN, QL_CUR_SPI2_CS_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        
        ret = ql_pin_set_func(QL_CUR_SPI2_CLK_PIN, QL_CUR_SPI2_CLK_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        ret = ql_pin_set_func(QL_CUR_SPI2_DO_PIN, QL_CUR_SPI2_DO_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
        ret = ql_pin_set_func(QL_CUR_SPI2_DI_PIN, QL_CUR_SPI2_DI_FUNC);
        if (ret != QL_GPIO_SUCCESS)
        {
            goto exit;
        }
    }

    return QAPI_OK;
exit:
    return QAPI_ERROR;
}

qapi_Status_t qapi_SPIM_Power_Off(void *spi_Handle)
{
    if (spi_Handle == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_cnktd_spi_info_s *spi_info = spi_Handle;

    ql_spi_port_e port = spi_info->port;
    QL_CNKTD_SPI_LOG("port=%d", port);

    if ((port != QL_SPI_PORT1) && (port != QL_SPI_PORT2))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (QL_SPI_SUCCESS != ql_spi_release(port))
    {
        return QAPI_ERROR;
    }
    
    return QAPI_OK;
}





qapi_Status_t qapi_SPIM_Full_Duplex (void *spi_Handle, qapi_SPIM_Config_t *config, 
	                                          qapi_SPIM_Descriptor_t *desc,uint32_t num_Descriptors,
	                                          qapi_SPIM_Callback_Fn_t c_Fn, void *c_Ctxt, qbool_t get_timestamp)
{
    ql_errcode_spi_e ret = QL_SPI_SUCCESS;
    ql_spi_config_s spi_config = {0};
    unsigned char *out_mal_data = NULL;
    unsigned char *in_mal_data = NULL;
    unsigned char *outdata = NULL;
    unsigned char *indata = NULL;
    unsigned int unsend_len = 0;
    unsigned int sended_len = 0;
    int len = 0;

    /* Check for valid input parameters */
	if ((config == NULL) || (desc == NULL) || (desc->len == 0) || (desc->tx_buf == 0 && desc->rx_buf == 0))
	{
        return QAPI_ERR_INVALID_PARAM;
	}
    
    ql_cnktd_spi_info_s *spi_info = spi_Handle;
    ql_spi_port_e port = spi_info->port;
    QL_CNKTD_SPI_LOG("port=%d", port);

    if ((port != QL_SPI_PORT1) && (port != QL_SPI_PORT2))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    spi_info->c_Fn = c_Fn;
    spi_info->c_Ctxt = c_Ctxt;

    out_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN+desc->len);
    in_mal_data = (unsigned char *)malloc(QL_SPI_DMA_ADDR_ALIN+desc->len);
    if (out_mal_data == NULL || in_mal_data == NULL)
    {
        return QAPI_ERR_NO_MEMORY;
    }
    
    outdata = (unsigned char *)OSI_ALIGN_UP(out_mal_data, QL_SPI_DMA_ADDR_ALIN);
    indata = (unsigned char *)OSI_ALIGN_UP(in_mal_data, QL_SPI_DMA_ADDR_ALIN);
    
    spi_config.port = port;
    if(config->SPIM_Mode == QAPI_SPIM_MODE_0_E)
    {
        spi_config.cpol = QL_SPI_CPOL_LOW;
        spi_config.cpha = QL_SPI_CPHA_1Edge;
    }
    else if(config->SPIM_Mode == QAPI_SPIM_MODE_1_E)
    {
        spi_config.cpol = QL_SPI_CPOL_LOW;
        spi_config.cpha = QL_SPI_CPHA_2Edge;
    }
    else if(config->SPIM_Mode == QAPI_SPIM_MODE_2_E)
    {
        spi_config.cpol = QL_SPI_CPOL_HIGH;
        spi_config.cpha = QL_SPI_CPHA_1Edge;
    }
    else if(config->SPIM_Mode == QAPI_SPIM_MODE_3_E)
    {
        spi_config.cpol = QL_SPI_CPOL_HIGH;
        spi_config.cpha = QL_SPI_CPHA_2Edge;
    }
    else 
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (config->SPIM_CS_Polarity == QAPI_SPIM_CS_ACTIVE_LOW_E)
    {
        spi_config.cs_polarity0 = QL_SPI_CS_ACTIVE_LOW;
    }
    else if (config->SPIM_CS_Polarity == QAPI_SPIM_CS_ACTIVE_HIGH_E)
    {
        spi_config.cs_polarity0 = QL_SPI_CS_ACTIVE_HIGH;
    }
    else 
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (config->SPIM_endianness != SPI_BIG_ENDIAN)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if ((config->SPIM_Bits_Per_Word != 8) && (config->SPIM_Bits_Per_Word != 16))
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    else 
    {
        spi_config.framesize = config->SPIM_Bits_Per_Word;
    }

    if (config->Clk_Freq_Hz > QL_SPI_CLK_50MHZ_MAX)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    else 
    {
        spi_config.spiclk = config->Clk_Freq_Hz;
    }

    if ((config->CS_Clk_Delay_Cycles != 0) && (config->CS_Clk_Delay_Cycles != 1))
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    else 
    {
        spi_config.clk_delay = config->CS_Clk_Delay_Cycles;
    }
 
    spi_config.input_mode = QL_SPI_INPUT_TRUE;
    spi_config.cs_polarity1 = QL_SPI_CS_ACTIVE_LOW;
    spi_config.input_sel = QL_SPI_DI_1;
    spi_config.transmode = QL_SPI_DMA_IRQ;  
    spi_config.cs = QL_SPI_CS0;
    spi_config.release_flag = QL_SPI_NOT_RELEASE;

    if(memcmp(&spi_config, &spi_info->spi_config, sizeof(spi_config)))
    {
        QL_CNKTD_SPI_LOG("spi config change");
        if (true == spi_info->is_init)
        {
            ql_spi_release(port);
        }

        ret = ql_spi_init_ext(spi_config);
        if (ret != QL_SPI_SUCCESS)
        {
            goto exit;
        }
        else 
        {
            memcpy(&spi_config, &spi_info->spi_config, sizeof(spi_config));
            spi_info->is_init = true;

            if (spi_info->write_read == NULL)
            {
                ql_rtos_semaphore_create(&spi_info->write_read, 0);
                ql_spi_irq_s mask = {0};
                mask.rx_dma_done = 1;
                mask.tx_dma_done = 1;
                ql_spi_set_irq(spi_info->port, mask, spi_info->cb);
            }
        }
    }
    else if (false == spi_info->is_init)
    {
        ret = ql_spi_init_ext(spi_config);
        if (ret != QL_SPI_SUCCESS)
        {
            goto exit;
        }
        else 
        {
            spi_info->is_init = true;
            if (spi_info->write_read == NULL)
            {
                ql_rtos_semaphore_create(&spi_info->write_read, 0);
                ql_spi_irq_s mask = {0};
                mask.rx_dma_done = 1;
                mask.tx_dma_done = 1;
                ql_spi_set_irq(spi_info->port, mask, spi_info->cb);
            }
        }
    }
    
    if (config->SPIM_Bits_Per_Word == 8)
    {
        unsend_len = desc->len;
        sended_len = 0;
        while(unsend_len > 0)
        {
            if (unsend_len > QL_SPI_DMA_IRQ_SIZE)
            {
                len = QL_SPI_DMA_IRQ_SIZE;
                
            }
            else 
            {
               len = unsend_len;
               
            }
            
            memcpy(outdata, desc->tx_buf + sended_len, len);            
            ql_spi_request_sys_clk(spi_info->port);           
            ql_spi_write_read(spi_info->port, indata, outdata, len);            
            
            ql_rtos_semaphore_wait(spi_info->write_read, QL_WAIT_FOREVER);
            ql_spi_release_sys_clk(spi_info->port);
            memcpy(desc->rx_buf + sended_len, indata, len);
            unsend_len -= len;
            sended_len += len;
        }
    }
    else 
    {
        unsend_len = desc->len;
        sended_len = 0;
        while(unsend_len > 0)
        {
            if (unsend_len > QL_SPI_DMA_IRQ_SIZE / 2)
            {
                len = QL_SPI_DMA_IRQ_SIZE / 2;
                
            }
            else 
            {
               len = unsend_len;
               
            }
           
            if(len%2 != 0)
            {
                QL_CNKTD_SPI_LOG("invalid parm");
                goto exit;
            }
            unsigned short out_temp = 0;
            
            for(int i = 0; i < len/2; i++)
            {
                out_temp = g_ql_cnktd_outbuf[i*2];
                g_ql_cnktd_outbuf[i] = (out_temp << QL_CNKTD_TYPE_SHIFT_8) + g_ql_cnktd_outbuf[i*2+1];
            }
            
            ql_spi_request_sys_clk(spi_info->port);
            ql_spi_write_read(port, (unsigned char*)g_ql_cnktd_inbuf, (unsigned char*)g_ql_cnktd_outbuf, len/2);
            ql_rtos_semaphore_wait(spi_info->write_read, QL_WAIT_FOREVER);
            ql_spi_release_sys_clk(spi_info->port);
            for(int i = 0; i < len/2; i++)
            {
                indata[i*2] = (g_ql_cnktd_inbuf[i] >> QL_CNKTD_TYPE_SHIFT_8) & 0xFF;
                indata[i*2+1] = g_ql_cnktd_inbuf[i] & 0xFF;
            }
            memcpy(desc->rx_buf + sended_len, indata, len);
            unsend_len -= len;
            sended_len += len;
        }
    }

    if (spi_info->c_Fn)
    {
        spi_info->c_Fn(1, spi_info->c_Ctxt);
    }
    if (out_mal_data)
    {
        free(out_mal_data);
    }
    
    if (in_mal_data)
    {
        free(in_mal_data);
    }
	
	return QAPI_OK;
exit:
    if (out_mal_data)
    {
        free(out_mal_data);
    }
    
    if (in_mal_data)
    {
        free(in_mal_data);
    }
    return QAPI_ERROR;
}



