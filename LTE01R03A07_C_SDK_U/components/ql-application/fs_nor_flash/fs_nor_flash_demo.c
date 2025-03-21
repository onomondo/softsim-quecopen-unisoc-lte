/**  
  @file
  fs_nor_flash_demo.c

  @brief
  quectel nor flash demo.

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
24/05/2021        Neo         Init version
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_gpio.h"
#include "ql_api_osi.h"
#include "ql_api_spi.h"
#include "ql_api_spi_flash.h"
#include "ql_api_spi_nor_flash.h"
#include "ql_api_fs_nor_flash.h"
#include "ql_log.h"
#include "ql_fs.h"
#include "fs_nor_flash_demo.h"
#include "ql_app_feature_config.h"


#define QL_NOR_FLASH_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_NOR_FLASH_LOG(msg, ...)			    QL_LOG(QL_NOR_FLASH_LOG_LEVEL, "ql_FS_NOR_DEMO", msg, ##__VA_ARGS__)
#define QL_NOR_FLASH_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_FS_NOR_DEMO", msg, ##__VA_ARGS__)

ql_task_t fs_nor_flash_demo_task = NULL;


/*
#define QL_CUR_SPI_PORT             QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI1_DI_FUNC
*/

#define QL_CUR_SPI_PORT             QL_SPI_PORT2
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI2_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI2_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI2_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI2_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI2_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI2_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI2_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI2_DI_FUNC

int ql_nor_flash_pin_init(void)
{
    ql_errcode_gpio gpio_ret;
    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || \
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
        QL_NOR_FLASH_LOG("pin err");
        return -1;
    }

    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_NOR_FLASH_LOG("set pin err");
        return -1;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CLK_PIN, QL_CUR_SPI_CLK_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_NOR_FLASH_LOG("set pin err");
        return -1;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_NOR_FLASH_LOG("set pin err");
        return -1;
    }
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_NOR_FLASH_LOG("set pin err");
        return -1;
    }

    return 0;
}

void nor_test()
{
#ifdef QL_APP_FEATURE_FILE
    int64 err = 0;
    int fd = 0;
    char buffer[500];
    char *str = TEST_STR_NOR;

    int file_number  = 0;
    char file_name[50];
    int64 r_size = 0;
    char path_t[] = "EXNOR:";
    int nor_press_count = 0;
    
    while(1)
    {
        r_size = ql_fs_free_size(path_t);
        QL_NOR_FLASH_LOG("flash space: %lld", r_size);
        if(r_size < 1000*1000)
        {
            QL_NOR_FLASH_LOG("flash overflow: %lld, nor_press_count:%d", r_size, nor_press_count);
            for(int j = 0;j < file_number; j++)
            {
                sprintf(file_name, "EXNOR:text%d.txt", j);
                err = ql_remove(file_name);
                QL_NOR_FLASH_LOG("remove file_name %s", file_name);
                if(err < 0)
                {
                    QL_NOR_FLASH_LOG("remove file failed");
                    ql_fclose(fd);
                    return;
                }
            }
            file_number = 0;
            nor_press_count++;
        }
        
        sprintf(file_name, "EXNOR:text%d.txt", file_number);
        fd = ql_fopen(file_name, "wb+");
        if(fd < 0)
        {
            QL_NOR_FLASH_LOG("open file failed");
            QL_NOR_FLASH_LOG("errcode is %x", err);
            return;
        }
        for(int i = 0;i < 2000 ; i++)
        {
            err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
            if(err < 0)
            {
                QL_NOR_FLASH_LOG("write file failed");
                ql_fclose(fd);
                return;
            }
        }

        err = ql_frewind(fd);
        if(err < 0)
        {
            QL_NOR_FLASH_LOG("rewind file failed");
            ql_fclose(fd);
            return;
        }

        for(int i = 0; i < 2000 ; i++)
        {
            err = ql_fread(buffer, strlen(str) + 1, 1, fd);
            if(err < 0)
            {
                QL_NOR_FLASH_LOG("read file failed");
                ql_fclose(fd);
                return;
            }
            err = strcmp(str, buffer);
            if(err != 0)
            {
                QL_NOR_FLASH_LOG("compare err, file_name %s, file read result is %s", file_name, buffer); 
            }
        }
        ql_rtos_task_sleep_ms(100);
        QL_NOR_FLASH_LOG("file_name %s", file_name); 
        ql_fclose(fd);
        file_number++;
    }
#endif
}

static void ql_fs_nor_flash_demo_task_thread(void *ctx)
{
    ql_errcode_spi_nor_e ret;
    ql_errcode_nor_flash_e nor_ret;
    ql_spi_nor_config_s spi_nor_config = {0};
    int64 err = 0;
    ql_nor_hw_info_t nor_info = {0};
#ifdef QL_APP_FEATURE_FILE
    int fd = 0;
	char buffer[200] = {0};
	char *str = TEST_STR_NOR;
	int64 r_size = 0;
#endif
    //step1:pin init----customers can use the custom pin to initialize
    if (ql_nor_flash_pin_init())
    {
        QL_NOR_FLASH_LOG("pin err");
        goto QL_FTL_TASK_EXIT;
    }

    //step2:spi init
    //If you use the default parameters, you can initialize it with ql_spi_nor_init
    //ret = ql_spi_nor_init(QL_SPI_PORT2, QL_SPI_CLK_25MHZ);
    spi_nor_config.port = QL_CUR_SPI_PORT;
    spi_nor_config.spiclk = QL_SPI_CLK_25MHZ;
    spi_nor_config.input_sel = QL_SPI_DI_1;
    spi_nor_config.transmode = QL_SPI_DIRECT_POLLING;
    spi_nor_config.cs = QL_SPI_CS0;
    ret = ql_spi_nor_init_ext(spi_nor_config);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        QL_NOR_FLASH_LOG("init err");
        goto QL_FTL_NOR_EXIT;
   }

    //If write protection is enabled on the chip by default, disable it first
    //ret = ql_spi_nor_write_8bit_status(spi_nor_config.port, 0x0);//disable wirte protection
    //QL_NOR_FLASH_STRESS_LOG("disable wirte protection");

    //step3:mkfs nor flash----the first time using flash or the file system is damaged needs to format the file system and mount file system 
    //mkfs operation can not be performed frequently. it will erase the whole block, which is very time-consuming
#if 0
    nor_ret = ql_nor_flash_mkfs(0x01);
    if(nor_ret != QL_NOR_FLASH_SUCCESS)
    {
        QL_NOR_FLASH_LOG("nor flash mkfs err");
        goto QL_FTL_NOR_EXIT;
    }
    ql_rtos_task_sleep_ms(100);
#else
    //step4:mount nor flash----mount file system
    nor_ret = ql_nor_flash_mount();
    if(nor_ret != QL_NOR_FLASH_SUCCESS)
    {
        nor_ret = ql_nor_flash_mkfs(0x01);
        if(nor_ret != QL_NOR_FLASH_SUCCESS)
        {
            QL_NOR_FLASH_LOG("nor flash mkfs err");
            goto QL_FTL_NOR_EXIT;
        }
        QL_NOR_FLASH_LOG("nor flash mount success");
    }
#endif

#if 1
    //step5:read id & read total size
    ql_nor_get_hw_info(&nor_info);
    QL_NOR_FLASH_LOG("mid=%x, total_size = %d", nor_info.mid, nor_info.blknum * nor_info.blksize);

#ifdef QL_APP_FEATURE_FILE
    r_size = ql_fs_free_size("EXNOR:");
    QL_NOR_FLASH_LOG("flash space: %lld  %lld", r_size,ql_fs_total_size("EXNOR:"));

    //step6:using file system API to complete read, write ect. 


	err = ql_mkdir(DIR_PATH_NOR, 0);
	if(err < 0)
	{
		if(err == QL_DIR_DIR_ALREADY_EXIST)
		{
			QL_NOR_FLASH_LOG("dir exist, not create");
		}
		else
		{
			QL_NOR_FLASH_LOG("make dir failed");
			goto QL_FTL_TASK_EXIT;
		}
	}

	fd = ql_fopen(FILE_PATH_NOR, "wb+");
	if(fd < 0)
	{
		QL_NOR_FLASH_LOG("open file failed");
		err = fd;
		goto QL_FTL_TASK_EXIT;
	}

	err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
	if(err < 0)
	{
		QL_NOR_FLASH_LOG("write file failed");
		ql_fclose(fd);
		goto QL_FTL_TASK_EXIT;
	}

	err = ql_frewind(fd);
	if(err < 0)
	{
		QL_NOR_FLASH_LOG("rewind file failed");
		ql_fclose(fd);
		goto QL_FTL_TASK_EXIT;
	}

	err = ql_fread(buffer, ql_fsize(fd), 1, fd);
	if(err < 0)
	{
		QL_NOR_FLASH_LOG("read file failed");
		ql_fclose(fd);
		goto QL_FTL_TASK_EXIT;
	}
    QL_NOR_FLASH_LOG("file read result is %s", buffer);

	ql_fclose(fd);
#endif
#else
            nor_test();
#endif

    //step7:umount nor flash----umount file system
    nor_ret = ql_nor_flash_umount();
    if(nor_ret != QL_NOR_FLASH_SUCCESS)
    {
        QL_NOR_FLASH_LOG("nor flash umount err");
        goto QL_FTL_NOR_EXIT;
    }

QL_FTL_NOR_EXIT:
    ql_spi_release(QL_CUR_SPI_PORT);
QL_FTL_TASK_EXIT:
    if(err < 0)
	{
		QL_NOR_FLASH_LOG("errcode is %x", err);
	}
    QL_NOR_FLASH_LOG("ql_rtos_task_delete");
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_NOR_FLASH_LOG("task deleted failed");
	}
}

QlOSStatus ql_fs_nor_flash_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&fs_nor_flash_demo_task, FS_NOR_FLASH_DEMO_TASK_STACK_SIZE, FS_NOR_FLASH_DEMO_TASK_PRIO, "ql_nor_flash", ql_fs_nor_flash_demo_task_thread, NULL, FS_NOR_FLASH_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_NOR_FLASH_LOG("demo_task created failed");
        return err;
	}

    return err;
}

