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
#include "ql_api_spi_nor_flash.h"
#include "ql_api_spi4_ext_nor_sffs.h"
#include "ql_log.h"
#include "spi4_ext_nor_sffs_demo.h"
#include "ql_gpio.h"
#include "ql_fs.h"
#include "ql_app_feature_config.h"
#include "ql_api_dev.h"

/*===========================================================================
 *  Macro Definition
 ===========================================================================*/

#define QL_SPI4_EXTNSFFS_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_SPI4_EXTNSFFS_LOG(msg, ...)			QL_LOG(QL_SPI4_EXTNSFFS_LOG_LEVEL, "ql_SPI4_SFFS_DEMO", msg, ##__VA_ARGS__)
#define QL_SPI4_EXTNSFFS_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_SPI4_SFFS_DEMO", msg, ##__VA_ARGS__)

#ifdef QL_APP_FEATURE_FILE
#define QL_SPI4_EXTNSFFS_FILE_PATH              "EXNSFFS:test.txt"
#define QL_SPI4_EXTNSFFS_TEST_STR               "1234567890abcdefg"
#endif


#define QL_CUR_SPI_PORT             QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CS_GPIO_FUNC     0
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI1_DI_FUNC

/*
#define QL_CUR_SPI_PORT             QL_SPI_PORT2
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI2_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI2_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI2_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI2_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI2_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI2_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI2_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI2_DI_FUNC
*/

/*========================================================================
*  Type Definition
*========================================================================*/


/*===========================================================================
 *  Variate
 ===========================================================================*/
ql_task_t spi4_ext_nor_sffs_demo_task = NULL;

/*========================================================================
 *  function Definition
 *========================================================================*/
 bool ql_spi4_ext_nor_sffs_pin_init(void)
{
    ql_errcode_gpio gpio_ret;

    if (QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || QL_CUR_SPI_CS_PIN == QUEC_PIN_NONE || \
        QL_CUR_SPI_DO_PIN == QUEC_PIN_NONE || QL_CUR_SPI_DI_PIN == QUEC_PIN_NONE)
    {
        QL_SPI4_EXTNSFFS_LOG("pin err");
        return false;
    }
 #if 1
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set pin err");
        return false;
    }
#else
    //SPI CS引脚用GPIO来控制
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CS_PIN, QL_CUR_SPI_CS_GPIO_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set pin err");
        return false;
    }

    
    gpio_ret = ql_gpio_init(GPIO_10, GPIO_OUTPUT, PULL_NONE, LVL_HIGH);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set gpio err");
        return false;
    }
#endif
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_CLK_PIN, QL_CUR_SPI_CLK_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set pin err");
        return false;
    }

#if 0
    //设置引脚的驱动能力为8ma，默认是6ma
    gpio_ret = ql_pin_set_drving(QL_CUR_SPI_CLK_PIN, 3);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set drving err");
        return false;
    }
#endif    
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DO_PIN, QL_CUR_SPI_DO_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set pin err");
        return false;
    }
    
    gpio_ret = ql_pin_set_func(QL_CUR_SPI_DI_PIN, QL_CUR_SPI_DI_FUNC);
    if (gpio_ret != QL_GPIO_SUCCESS)
    {
        QL_SPI4_EXTNSFFS_LOG("set pin err");
        return false;
    }
    
    return true;
}
static void ql_spi4_ext_nor_sffs_demo_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    ql_errcode_spi4_extnsffs_e sffs_ret = QL_SPI4_EXT_NOR_SFFS_SUCCESS;
    ql_errcode_spi_nor_e ret = QL_SPI_FLASH_SUCCESS;
    ql_spi_nor_config_s nor_config = {0};
#ifdef QL_APP_FEATURE_FILE
    int fd = 0;
    int64 file_err = 0;
    char buffer[100] = {0};
    char *str = QL_SPI4_EXTNSFFS_TEST_STR;
    int64 free_size = 0;
#endif
    //step 1:spi pin init
    if(!ql_spi4_ext_nor_sffs_pin_init())
    {
        goto TASK_EXIT;
    }
    
    //step 2:spi nor flash config
    //If you use the default parameters, you can initialize it with ql_spi_nor_init
    //ret = ql_spi_nor_init(QL_SPI_PORT2, QL_SPI_CLK_26MHZ);
    nor_config.port = QL_CUR_SPI_PORT;
    nor_config.spiclk = QL_SPI_CLK_25MHZ;
    nor_config.input_sel = QL_SPI_DI_1;
    nor_config.transmode = QL_SPI_DIRECT_POLLING;
    //nor_config.transmode = QL_SPI_DMA_POLLING;
    nor_config.cs = QL_SPI_CS0;
    //使用CPIO的方式来控制SPI CS引脚。注意，CS引脚需要配置为GPIO功能
    //nor_config.cs = QL_SPI_GPIO;
    //nor_config.cs_gpio = GPIO_10;
    //nor_config.release_flag = QL_SPI_RELEASE;
    ret = ql_spi_nor_init_ext(nor_config);
    if (ret != QL_SPI_FLASH_SUCCESS)
    {
        goto SPI_EXIT;
    }
	//If write protection is enabled on the chip by default, disable it first
	//ret = ql_spi_nor_write_8bit_status(QL_CUR_SPI_PORT, 0x0);//disable wirte protection
	//ret = ql_spi_nor_write_16bit_status(QL_CUR_SPI_PORT, 0x0);//disable wirte protection

    //step 3:mount/mkfs sffs file system
    ql_spi4_ext_nor_sffs_set_port(QL_CUR_SPI_PORT);
    //mount
    if(ql_spi4_ext_nor_sffs_mount() != QL_SPI4_EXT_NOR_SFFS_SUCCESS)
    {
        //format
        ql_dev_feed_wdt();
        if(ql_spi4_ext_nor_sffs_mkfs()!= QL_SPI4_EXT_NOR_SFFS_SUCCESS)
        {
            QL_SPI4_EXTNSFFS_LOG("mksffs failed");
            goto SPI_EXIT;
        }
    }
    QL_SPI4_EXTNSFFS_LOG("sffs mount succeed");

    //step 4:operate nor Flash using the file system interface
#ifdef QL_APP_FEATURE_FILE
    free_size = ql_fs_free_size("EXNSFFS:");
    QL_SPI4_EXTNSFFS_LOG("fs free_size :%d", free_size);

    fd = ql_fopen(QL_SPI4_EXTNSFFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("open file failed");
        file_err = fd;
        goto exit;
    }
    
    file_err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
    if(file_err < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("write file failed");
        ql_fclose(fd);
        goto exit;
    }
    
    file_err = ql_frewind(fd);
    if(file_err < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("rewind file failed");
        ql_fclose(fd);
        goto exit;
    }

    memset(buffer, 0x00, sizeof(buffer));
    file_err = ql_fread(buffer, strlen(str), 1, fd);
    if(file_err < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("read file failed");
        ql_fclose(fd);
        goto exit;
    }   

    QL_SPI4_EXTNSFFS_LOG("file read result is %s", buffer);
    ql_fclose(fd); 

    ql_dev_feed_wdt();
#if 1
    //unmount
    if(ql_spi4_ext_nor_sffs_unmount() != QL_SPI4_EXT_NOR_SFFS_SUCCESS)
    {
      goto SPI_EXIT;
    }
    QL_SPI4_EXTNSFFS_LOG("sffs unmount succeed");
    fd = ql_fopen(QL_SPI4_EXTNSFFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("open file failed,errcode is %x",fd);

        //to mount
        sffs_ret = ql_spi4_ext_nor_sffs_mount();
        if(QL_SPI4_EXT_NOR_SFFS_SUCCESS != sffs_ret)
        {
            QL_SPI4_EXTNSFFS_LOG("sffs remount failed,ret %x", sffs_ret);
            goto SPI_EXIT;
        }
        QL_SPI4_EXTNSFFS_LOG("sffs remount succeed");
    }
    
    fd = ql_fopen(QL_SPI4_EXTNSFFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
        QL_SPI4_EXTNSFFS_LOG("open file failed");
        file_err = fd;
        goto exit;
    }
    ql_fclose(fd);
    QL_SPI4_EXTNSFFS_LOG("close file");
    goto SPI_EXIT;
#endif
exit:	
	if(file_err < 0)
	{
		QL_SPI4_EXTNSFFS_LOG("errcode is %x", file_err);
	}
#endif /* QL_APP_FEATURE_FILE*/
SPI_EXIT:
    //step 5:release spi interface if you need
    //ql_spi_release(QL_CUR_SPI_PORT);
TASK_EXIT:
    QL_SPI4_EXTNSFFS_LOG("ql_rtos_task_delete");
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI4_EXTNSFFS_LOG("task deleted failed");
	}
}



QlOSStatus ql_spi4_ext_nor_sffs_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    //QL_SPI4_EXTNSFFS_LOG("enter ql_spi4_ext_nor_sffs_demo_init");
	err = ql_rtos_task_create(&spi4_ext_nor_sffs_demo_task,
								SPI4_EXT_NOR_SFFS_DEMO_TASK_STACK_SIZE,
								SPI4_EXT_NOR_SFFS_DEMO_TASK_PRIO,
								"ql_spi4_sffs",
								ql_spi4_ext_nor_sffs_demo_task_pthread,
								NULL,
								SPI4_EXT_NOR_SFFS_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI4_EXTNSFFS_LOG("demo_task created failed");
        return err;
	}

    return err;
}

