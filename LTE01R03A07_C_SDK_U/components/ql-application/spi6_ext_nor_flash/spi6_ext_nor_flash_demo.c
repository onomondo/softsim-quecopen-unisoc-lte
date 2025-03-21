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
#include "ql_log.h"
#include "ql_api_spi6_ext_nor_flash.h"
#include "spi6_ext_nor_flash_demo.h"
#include "ql_fs.h"
#include "ql_app_feature_config.h"

#define QL_SPI6_EXT_NOR_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_SPI6_EXT_NOR_LOG(msg, ...)			QL_LOG(QL_SPI6_EXT_NOR_LOG_LEVEL, "ql_SPI6_NOR_DEMO", msg, ##__VA_ARGS__)
#define QL_SPI6_EXT_NOR_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_SPI6_NOR_DEMO", msg, ##__VA_ARGS__)

#ifdef QL_APP_FEATURE_FILE
#define QL_EFS_FILE_PATH              "EFS:test.txt"
#define QL_EFS_TEST_STR               "1234567890abcdefg"
#endif

ql_task_t spi6_ext_nor_flash_demo_task = NULL;

static void ql_spi6_ext_nor_flash_demo_task_pthread(void *ctx)
{
    QlOSStatus erro = 0;
    ql_errcode_spi6_nor_e ret; 
/* 
If you want to change the clock rate of flash, you can do the following.
*/
    //ql_spi6_ext_nor_flash_clk_set(SPI6_EXT_NOR_FLASH_CLK_83MHZ);
/*
1、引脚选择。平台只有一个外置flash控制器,但是可以指定将哪些引脚复用成外置flash功能,
   为了简化使用,用PORT_1或者PORT_2来代表当前使用哪一组引脚复用成外置flash功能.
   通过quec_boot_pin_cfg.h中的QL_BOOT_CUR_SPI6_PORT宏来配置。如下:
   #define QL_BOOT_CUR_SPI6_PORT    QL_BOOT_SPI_PORT_1   //当前boot中使用的6线spi口
2、引脚配置。PORT_1指定的引脚的电压域只能1.8v不能修改。PORT_2指定的引脚的电压域可以支持修改,最大3200MV。
   在quec_boot_pin_map.c中通过quec_boot_spi6_pin_cfg_map数组定义。参数可以根据实际情况进行修改。
   ql_spi6_ext_nor_flash_init()就是根据quec_boot_spi6_pin_cfg_map中的参数进行引脚初始化的。
*/
	//ql_spi6_ext_nor_flash_clk_set(SPI6_EXT_NOR_FLASH_CLK_83MHZ);
    ql_spi6_ext_nor_flash_init();
    ret = ql_spi6_ext_nor_flash_sffs_mount();
    if(QL_SPI6_EXT_NOR_FLASH_SUCCESS != ret)
    {
        //格式化
        ret = ql_spi6_ext_nor_flash_sffs_mkfs();
        if(QL_SPI6_EXT_NOR_FLASH_SUCCESS != ret)
        {
          QL_SPI6_EXT_NOR_LOG("mks fail,err=%x",ret);
          goto QL_SPI6_TASK_EXIT;
        }
        QL_SPI6_EXT_NOR_LOG("mks success");           
    }
#ifdef QL_APP_FEATURE_FILE
	int fd = 0;
	int64 err = 0;
	char buffer[100];
	char *str = QL_EFS_TEST_STR;

    int64 free_size = ql_fs_free_size("EFS:");
    QL_SPI6_EXT_NOR_LOG("fs free_size :%d", free_size);
    
    fd = ql_fopen(QL_EFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("open file failed");
    	err = fd;
    	goto exit;
    }

    err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
    if(err < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("write file failed");
    	ql_fclose(fd);
    	goto exit;		
    }

    err = ql_frewind(fd);
    if(err < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("rewind file failed");
    	ql_fclose(fd);
    	goto exit;					
    }

    err = ql_fread(buffer, ql_fsize(fd), 1, fd);
    if(err < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("read file failed");
    	ql_fclose(fd);
    	goto exit;					
    }

    QL_SPI6_EXT_NOR_LOG("file read result is %s", buffer);
    ql_fclose(fd);
#if 0
    ret = ql_spi6_ext_nor_flash_sffs_unmount();
    if(QL_SPI6_EXT_NOR_FLASH_SUCCESS != ret)
    {
      goto QL_SPI6_TASK_EXIT;
    }    
    fd = ql_fopen(QL_EFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("open file failed,errcode is %x",fd);

        //重新挂载
        ret = ql_spi6_ext_nor_flash_sffs_mount();
        QL_SPI6_EXT_NOR_LOG("sffs mount ret %x", ret);
        if(QL_SPI6_EXT_NOR_FLASH_SUCCESS != ret)
        {
          goto QL_SPI6_TASK_EXIT;
        }
    }
    
    fd = ql_fopen(QL_EFS_FILE_PATH, "wb+");
    if(fd < 0)
    {
    	QL_SPI6_EXT_NOR_LOG("open file failed");
    	err = fd;
    	goto exit;
    }
    ql_fclose(fd);
#endif
    QL_SPI6_EXT_NOR_LOG("close file");
   
exit:	
	if(err < 0)
	{
		QL_SPI6_EXT_NOR_LOG("errcode is %x", err);
	}
	
#endif

QL_SPI6_TASK_EXIT:
    QL_SPI6_EXT_NOR_LOG("ql_rtos_task_delete");
	erro = ql_rtos_task_delete(NULL);
	if(erro != QL_OSI_SUCCESS)
	{
		QL_SPI6_EXT_NOR_LOG("task deleted failed");
	}
}



QlOSStatus ql_spi6_ext_nor_flash_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    //QL_SPI6_EXT_NOR_LOG("enter ql_spi6_ext_nor_flash_demo_init");
	err = ql_rtos_task_create(&spi6_ext_nor_flash_demo_task, SPI6_EXT_NOR_FLASH_DEMO_TASK_STACK_SIZE, SPI6_EXT_NOR_FLASH_DEMO_TASK_PRIO, "ql_spi6_nor", ql_spi6_ext_nor_flash_demo_task_pthread, NULL, SPI6_EXT_NOR_FLASH_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SPI6_EXT_NOR_LOG("demo_task created failed");
        return err;
	}

    return err;
}

