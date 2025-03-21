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

#include "ql_api_spi_nand_flash.h"

#include "ql_log.h"
#include "spi_nand_flash_demo.h"
#include "ql_gpio.h"


#define QL_SPI_NAND_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_SPI_NAND_LOG(msg, ...)			    QL_LOG(QL_SPI_NAND_LOG_LEVEL, "ql_SPI_NAND_DEMO", msg, ##__VA_ARGS__)
#define QL_SPI_NAND_LOG_PUSH(msg, ...)	        QL_LOG_PUSH("ql_SPI_NAND_DEMO", msg, ##__VA_ARGS__)

ql_task_t spi_nand_flash_demo_task = NULL;

#define QL_CUR_SPI_PORT             QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI1_DI_FUNC

void ql_spi_nand_flash_data_prifnt(unsigned char *data, int len)
{
    int i = 0;
    int count_len = 256;
    int count = 0;
    int exit = 0;
    unsigned int buf_pos = 0;

    unsigned char *str_data = (unsigned char *)malloc(len*2+64);
    if (str_data == NULL)
    {
        QL_SPI_NAND_LOG("malloc err");
        return ; 
    }

    QL_SPI_NAND_LOG("read len=%d", len);
    
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
        
        QL_SPI_NAND_LOG("data[%d-%d]=%s", count, count+count_len, str_data);
        count += count_len;
    }

    free(str_data);
}
//extern ql_errcode_gpio ql_pin_set_func(uint8_t pin_num, uint8_t func_sel);
static void ql_spi_nand_flash_demo_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    ql_errcode_spi_nand_e ret;  
    ql_errcode_gpio gpio_ret;
    int block = 50;   //50的block改写了坏块标志就使用不了了？
    int page = 0;
    unsigned char *data = NULL;
    int data_len = QL_SPI_NAND_PAGE_SPARE_SIZE;
    unsigned int page_addr = block << 6 | page;
    unsigned short column_addr = 0;
    int i = 0;
    ql_spi_nand_config_s nand_config = {0};

    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || \
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
        QL_SPI_NAND_LOG("pin err");
        goto QL_SPI_TASK_EXIT;
    }

    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_NAND_LOG("set pin err");
        goto QL_SPI_NAND_EXIT;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CLK_PIN, QL_CUR_SPI_CLK_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_NAND_LOG("set pin err");
        goto QL_SPI_NAND_EXIT;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_NAND_LOG("set pin err");
        goto QL_SPI_NAND_EXIT;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI_NAND_LOG("set pin err");
        goto QL_SPI_NAND_EXIT;
    }

    data = (unsigned char *)malloc(data_len);
    if (data == NULL)
    {
        QL_SPI_NAND_LOG("malloc err");
        goto QL_SPI_NAND_EXIT; 
    }
    //If you use the default parameters, you can initialize it with ql_spi_nand_init
    //ret = ql_spi_nand_init(QL_CUR_SPI_PORT, QL_SPI_CLK_25MHZ);
    nand_config.port = QL_CUR_SPI_PORT;
    nand_config.spiclk = QL_SPI_CLK_25MHZ;
    nand_config.input_sel = QL_SPI_DI_1;
    nand_config.transmode = QL_SPI_DIRECT_POLLING;
    nand_config.cs = QL_SPI_CS0;
    ret = ql_spi_nand_init_ext(nand_config);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("init err");
        goto QL_SPI_NAND_EXIT;  
   }

    unsigned char mid[3] = {0};
    ret = ql_spi_nand_read_devid(QL_CUR_SPI_PORT, mid);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read devid err");
        goto QL_SPI_NAND_EXIT;  
   }
#if 1
    unsigned char status = 0;
    ret = ql_spi_nand_read_status(QL_CUR_SPI_PORT, QL_NAND_FLASH_STATUS_1, &status);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read status err");
        goto QL_SPI_NAND_EXIT;  
   } 
    QL_SPI_NAND_LOG("sr1 = %x", status);

    ret = ql_spi_nand_read_status(QL_CUR_SPI_PORT, QL_NAND_FLASH_STATUS_2, &status);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read status err");
        goto QL_SPI_NAND_EXIT;  
   } 
    QL_SPI_NAND_LOG("sr2 = %x", status);
    
    ret = ql_spi_nand_read_status(QL_CUR_SPI_PORT, QL_NAND_FLASH_STATUS_3, &status);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read status err");
        goto QL_SPI_NAND_EXIT;  
   } 
    QL_SPI_NAND_LOG("sr3 = %x", status);

    ret = ql_spi_nand_read_status(QL_CUR_SPI_PORT, QL_NAND_FLASH_STATUS_4, &status);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read status err");
        goto QL_SPI_NAND_EXIT;  
   } 
    QL_SPI_NAND_LOG("sr4 = %x", status);
#endif

    ret = ql_spi_nand_erase_block(QL_CUR_SPI_PORT, page_addr);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("erase err");
        goto QL_SPI_NAND_EXIT;  
   }  
 #if 1 
    memset(data, 0, data_len);
    for (i=0; i<data_len; i++)
    {
        data[i] = 'e';//i % 256;
    }
    
    ret = ql_spi_nand_write_page_spare(QL_CUR_SPI_PORT, page_addr, column_addr, data, data_len);
    //ret = ql_spi_nand_write_spare(QL_CUR_SPI_PORT, page_addr, data, 12);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read data err");
        goto QL_SPI_NAND_EXIT;  
   }  

    memset(data, 0, data_len);
    ret = ql_spi_nand_read_page_spare(QL_CUR_SPI_PORT, page_addr, column_addr, data, data_len);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_SPI_NAND_LOG("read data err");
        goto QL_SPI_NAND_EXIT;  
   }  
    
    ql_spi_nand_flash_data_prifnt(data, data_len);
#endif
QL_SPI_NAND_EXIT:
    free(data);
    ql_spi_release(QL_CUR_SPI_PORT);
QL_SPI_TASK_EXIT:
    QL_SPI_NAND_LOG("ql_rtos_task_delete");
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI_NAND_LOG("task deleted failed");
	}
}



QlOSStatus ql_spi_nand_flash_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    //QL_SPI_NAND_LOG("enter ql_spi_nand_flash_demo_init");
	err = ql_rtos_task_create(&spi_nand_flash_demo_task, SPI_NAND_FLASH_DEMO_TASK_STACK_SIZE, SPI_NAND_FLASH_DEMO_TASK_PRIO, "ql_spi_nand", ql_spi_nand_flash_demo_task_pthread, NULL, SPI_NAND_FLASH_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI_NAND_LOG("demo_task created failed");
        return err;
	}

    return err;
}

