/**  @file
  quec_spi_nand_flash_prop.h

  @brief
  This file is used to define spi NAND FLASH property for open Quectel Project.

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
#include "quec_spi_nand_flash_prop.h"

/* 4-line SPI NAND Flash supported model list,can add by yourself*/
static quec_spi_nand_flash_info_s quec_spi_nand_flash_props[] = 
{
//You can cut out it by defining macros
//#define DISABLE_W25N02JWXXIF
#ifndef DISABLE_W25N02JWXXIF
    {
        // W25N02JWxxIF/IC  MID=EFBF22,2--2G
        .page_totalsize = 2112,
        .page_mainsize  = 2048,
        .page_sparesize = 64,
        .block_pagenum  = 64,
        .block_totalnum = 2048,
        .cache_blocknum = 5,
        {
            .page_spare_shift    = 0 ,//=page_mainsize = 2048
            .block_postion_shift = 2 ,//2050
            .block_type_shift    = 3 ,//2051
            .logic_addr_shift    = 16,//2064
            .page_used_shift     = 32,//2080
            .page_garbage_shift  = 33,//2081
            .page_num_shift      = 34,//2082
        },
        .nand_id        = 0xEF0F,      //bit[7:4]don't compare,should be 0
        .nand_id_type   = QUEC_NAND_FLASH_ID_24BIT,
    },
#endif
#ifndef DISABLE_GD5F1GQ5XEXXG
    {
        // GD5F1GQ5xExxG  MID=C841/C851,1--1G
        .page_totalsize = 2112,
        .page_mainsize  = 2048,
        .page_sparesize = 64,
        .block_pagenum  = 64,
        .block_totalnum = 1024,
        .cache_blocknum = 5,
        {
            .page_spare_shift    = 0 ,//=page_mainsize = 2048
            .block_postion_shift = 2 ,//2050
            .block_type_shift    = 3 ,//2051
            .logic_addr_shift    = 16,//2064
            .page_used_shift     = 32,//2080
            .page_garbage_shift  = 33,//2081
            .page_num_shift      = 34,//2082
        },
        .nand_id        = 0xC801,     //bit[7:4]don't compare,should be 0
        .nand_id_type   = QUEC_NAND_FLASH_ID_16BIT,
    },
#endif
//You can cut out it by defining macros
//#define DISABLE_WN25NXXGW
#ifndef DISABLE_WN25NXXGW
    {
        // wn25n01gw  MID=EF0A
        .page_totalsize = 2112,
        .page_mainsize  = 2048,
        .page_sparesize = 64,
        .block_pagenum  = 64,
        .block_totalnum = 1024,
        .cache_blocknum = 5,
        {
            .page_spare_shift    = 0 ,//=page_mainsize = 2048
            .block_postion_shift = 2 ,//2050
            .block_type_shift    = 3 ,//2051
            .logic_addr_shift    = 16,//2064
            .page_used_shift     = 32,//2080
            .page_garbage_shift  = 33,//2081
            .page_num_shift      = 34,//2082
        },
        .nand_id        = 0xEF0A,     //bit[7:4]don't compare,should be 0
        .nand_id_type   = QUEC_NAND_FLASH_ID_24BIT,
    },
#endif
};

static unsigned int quec_spi_nand_flash_prop_len = sizeof(quec_spi_nand_flash_props)/sizeof(quec_spi_nand_flash_props[0]);

bool quec_spi_nand_props_init(quec_spi_nand_flash_info_s **props_ptr,unsigned int *props_len)
{
	if(props_ptr == NULL || props_len == NULL)
	{
		return false;
	}
	
	*props_ptr = quec_spi_nand_flash_props;
	*props_len = quec_spi_nand_flash_prop_len;
	return true;	
}