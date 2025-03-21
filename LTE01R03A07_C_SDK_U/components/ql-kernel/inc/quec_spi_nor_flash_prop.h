/**  @file
  quec_spi_nor_flash_prop.h

  @brief
  This file is used to define spi NOR FLASH api for open Quectel Project.

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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
  
#include "ql_osi_def.h"
#include "ql_api_osi.h"

typedef enum
{
    QUEC_NOR_FLASH_MODE_8BIT = 0,               //8bit status register 8位的状态寄存器
    QUEC_NOR_FLASH_MODE_16BIT,                  //16bi status register 16位的状态寄存器
}quec_spi_nor_flash_status_mode_e;

typedef struct 
{
    unsigned int mid;                           //Product ID, the 3-byte product ID read with 9F  产品ID，用9F读到的3字节产品ID
    int64  capacity;                            //flash capacity, 32Mbit:0x400000 64Mbit:0x800000 128Mbit:0x1000000
    quec_spi_nor_flash_status_mode_e mode;      //FLASH mode
    //以下参数调整后都需要重新格式化,否则会出现不确定的异常
    unsigned int mount_start_addr;              //flash start addr to mount the file system,0x000000
    int32  mount_size;                          //the amount of space used to mount the file system,(0x000000---mount szie)
    uint32_t pb_size;                           //flash pb block size, general use 0x200,in special cases,use 0x400 to exchange space for time
    uint32_t eb_size;                           //flash erase block size, general use 0x1000(4KB),in special cases,use 0x8000(32KB) to reduce the formatting time of the file system
}quec_spi_nor_flash_info_s;


