/**  @file
  embed_nor_flash_demo.c

  @brief
  This file is demo of embed nor flash read or wrtie.

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

/*===========================================================================
 * include files
 ===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"

#include "embed_nor_flash_demo.h"
#include "ql_embed_nor_flash.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define QL_APP_EMBED_NOR_FLASH_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_EMBED_NOR_FLASH_LOG(msg, ...)             QL_LOG(QL_APP_EMBED_NOR_FLASH_LOG_LEVEL, "QL_APP_EMBED_NOR_FLASH", msg, ##__VA_ARGS__)

#define SECTOR_SIZE  4096
#define FLASH_ADDR   0x60370000 //注意： demo 使用的预留flash分区地址，根据自己需求自行修改，确保读写位置为预留地址 

ql_task_t embed_nor_flash_task = NULL;

 void static embed_nor_flash_demo_thread(void *param)
{
    ql_errcode_e ret=QL_SUCCESS;

    char flash_buff[128]={0};

    char *test_write_str="123456789abcdefghijklmnqpqrst";

    QL_EMBED_NOR_FLASH_LOG("==========embed flash demo start============");

    ql_rtos_task_sleep_ms(10);

    //erase FLASH_ADDR sector
    ret=ql_embed_nor_flash_erase(FLASH_ADDR,SECTOR_SIZE);

    if(ret!=QL_SUCCESS)
    {
        QL_EMBED_NOR_FLASH_LOG("embed nor flash erase faild,erase addr:0x%X",FLASH_ADDR);
    }

    ret=ql_embed_nor_flash_write(FLASH_ADDR,(void *)test_write_str,strlen(test_write_str));
	
    if(ret!=QL_SUCCESS)
    {
        QL_EMBED_NOR_FLASH_LOG("write embed nor flash faild");
    }
	
    ql_rtos_task_sleep_ms(10);

    ret=ql_embed_nor_flash_read(FLASH_ADDR,(void *)flash_buff,strlen(test_write_str));
    
    if(ret!=QL_SUCCESS)
    {
        QL_EMBED_NOR_FLASH_LOG("read embed nor flash faild");
    }
	
    QL_EMBED_NOR_FLASH_LOG("read addr 0x%X,content:%s",FLASH_ADDR,flash_buff);
    
    ql_rtos_task_sleep_ms(10);

    QL_EMBED_NOR_FLASH_LOG("==========embed flash demo finished============");
    
  	ql_rtos_task_delete(embed_nor_flash_task);	
  
}

void ql_embed_nor_flash_app_init(void)
{
    int err = QL_OSI_SUCCESS;
        
    err = ql_rtos_task_create(&embed_nor_flash_task, 8*1024,23,"embed_flash_DEMO", embed_nor_flash_demo_thread, NULL,1);
	
    if (err != QL_OSI_SUCCESS)
    {
        QL_EMBED_NOR_FLASH_LOG("embed nor flash demo task created failed");
    }
}


