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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
  
#include "ql_osi_def.h"
#include "ql_api_osi.h"

typedef enum
{
    QUEC_NAND_FLASH_ID_16BIT = 0,               //Manufacturer ID(8bit) + device ID(8bit)
    QUEC_NAND_FLASH_ID_24BIT,                   //Manufacturer ID(8bit) + device ID(16bit)
}quec_spi_nand_flash_id_type_e;

typedef struct
{
    unsigned short page_spare_shift;              //the offset of the spare area in page,bad block:2byte
    unsigned short block_postion_shift;           //offset in page,1byte
    unsigned short block_type_shift;              //offset in page,1byte
    unsigned short logic_addr_shift;              //offset in page,4byte
    unsigned short page_used_shift;               //offset in page,1byte
    unsigned short page_garbage_shift;            //offset in page,1byte
    unsigned short page_num_shift;                //offset in page,2byte
}quec_spi_nand_spare_info_s;

typedef struct
{
    unsigned int page_totalsize;                //each page bytes(main area + spare area),Internal ECC On
    unsigned int page_mainsize;                 //main area of the page
    unsigned int page_sparesize;                //spare area of the page,Internal ECC On & user area
    unsigned int block_pagenum;                 //pages num in each block,Maximum 64
    //unsigned int plane_blocknum;                //blocks num in each plane
    unsigned int block_totalnum;                //blocks num in each device
    unsigned int cache_blocknum;                //cache blocks num,the current value is fixed at 5 and cannot be changed
    quec_spi_nand_spare_info_s spare_info;      //spare area data division
    unsigned int nand_id;                       //nand id(9FH),=MID + DID2(MSB 8BIT), not contain low 8bits(DID1(LSB 8BIT))
    quec_spi_nand_flash_id_type_e nand_id_type; //nand id type
}quec_spi_nand_flash_info_s;
