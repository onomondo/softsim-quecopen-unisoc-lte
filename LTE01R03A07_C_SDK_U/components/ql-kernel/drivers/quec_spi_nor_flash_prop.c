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
#include "quec_spi_nor_flash_prop.h"

//备注:mount_start_addr、mount_size、pb_size和eb_size参数调整后都需要重新格式化,否则会出现不确定的异常
//Note: The mount_start_addr, mount_size, pb_size, and eb_size parameters must be reformatted after they are adjusted. Otherwise, uncertain exceptions may occur.

/* 4-line SPI NOR Flash supported model list,can add by yourself*/
static quec_spi_nor_flash_info_s quec_spi_nor_flash_props[] = 
{
//You can cut out it by defining macros
//#define DISABLE_GD25LE64E_C
#ifndef DISABLE_GD25LE64E_C
    { 
        // GD25LE64E
        // GD25LE64C
        .mid = 0x1760c8,
        .capacity = 0x800000,
        .mode = QUEC_NOR_FLASH_MODE_16BIT,
        .mount_start_addr = 0x0000000,
        .mount_size = 0x800000,
        .pb_size = 0x200,
        .eb_size = 0x1000,
    },
#endif
#ifndef DISABLE_XM25QU64B
    {
        // XM25QU64B
        .mid = 0x175020,
        .capacity = 0x800000,
        .mode = QUEC_NOR_FLASH_MODE_8BIT,
        .mount_start_addr = 0x0000000,
        .mount_size = 0x800000,
        .pb_size = 0x200,
        .eb_size = 0x1000,
    },
#endif
#ifndef DISABLE_XM25QU128C
    {
        // XM25QU128C
        .mid = 0x184120,
        .capacity = 0x1000000,
        .mode = QUEC_NOR_FLASH_MODE_16BIT,
        .mount_start_addr = 0x0000000,
        .mount_size = 0x1000000,
        .pb_size = 0x200,
        .eb_size = 0x1000,
    }, 
#endif
#ifndef DISABLE_W25Q64JWSSIM
    {
        // W25Q64JWSSIM
        .mid = 0x1780ef,
        .capacity = 0x800000,
        .mode = QUEC_NOR_FLASH_MODE_16BIT,
        .mount_start_addr = 0x0000000,
        .mount_size = 0x800000,
        .pb_size = 0x200,
        .eb_size = 0x1000,
    },
#endif
#ifndef DISABLE_W25Q256JWFQ
    {
        // W25Q256JWFQ
        .mid = 0x1960ef,
        .capacity = 0x2000000,
        .mode = QUEC_NOR_FLASH_MODE_8BIT,
        .mount_start_addr = 0x0000000,
        .mount_size = 0x2000000,
        .pb_size = 0x400,           //256mbits容量，愿意空间换时间，加快扫描块的时间。块变大，数量变少，因此时间变快。
        .eb_size = 0x1000,          //可以调整此值为0x8000(32KB)来缩短格式化挂载的时间,但是文件系统分区(即mount_start_addr和mount_size)需要eb_size对齐
    },
#endif
};

static unsigned int quec_spi_nor_flash_prop_len = sizeof(quec_spi_nor_flash_props)/sizeof(quec_spi_nor_flash_props[0]);

bool quec_spi_nor_props_init(quec_spi_nor_flash_info_s **props_ptr,unsigned int *props_len)
{
	if(props_ptr == NULL || props_len == NULL)
	{
		return false;
	}
	
	*props_ptr = quec_spi_nor_flash_props;
	*props_len = quec_spi_nor_flash_prop_len;
	return true;	
}
