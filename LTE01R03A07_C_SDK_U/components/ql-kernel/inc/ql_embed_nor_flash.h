/**  @file
  ql_embed_nor_flash.h

  @brief
  This file is used to define embed_nor flash interface for different Quectel Project.

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
07/06/2021        Mahat        Init version
=================================================================*/

/*=================================================================
                                READ ME
API能够直接读写位于[0x60010000~0x60560000]中用户新增的自定义flash分区,
并判断读写地址的合法性，请确保读写区域在分区文件中已经被预留并定义，否则将返回失败
定义预留的内置flash分区时，如果要调整文件系统分区，注意文件系统分区的地址和大小必须为32K对齐，
而如果要调整AP固件分区或客户APPIMG固件分区，则地址和大小必须4K对齐


用户需在 components\ql-config\build\EXX00UXX_XX\8915DM_cat1_open\target.config  
文件中添加 CONFIG_QUEC_PROJECT_FEATURE_EMBED_NOR_FLASH=y 启用此功能
其中EXX00UXX_XX指代用户需要的实际型号
=================================================================*/

#ifndef QL_EMBED_NOR_RESERVED_FLASH_H
#define QL_EMBED_NOR_RESERVED_FLASH_H

#include "ql_api_common.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QL_EMBED_NOR_FLASH_ERRCODE_BASE (QL_COMPONENT_STORAGE_EMBED_NOR_FLASH<<16)

typedef enum
{
   QL_EMBED_NOR_FLASH_SUCCESS = QL_SUCCESS,  
   QL_EMBED_NOR_FLASH_WRITE_ERR = 1|QL_EMBED_NOR_FLASH_ERRCODE_BASE,
   QL_EMBED_NOR_FLASH_READ_ERR,
   QL_EMBED_NOR_FLASH_ERASE_ERR,
   QL_EMBED_NOR_FLASH_OPERATE_ERR,
   QL_EMBED_NOR_FLASH_ADDRESS_ERR,
}ql_embed_nor_flash_e;

/*****************************************************************
* Function: ql_embed_nor_flash_write
* Description: embed nor flash write data 
* This will just use flash PROGRAM command to write. Caller should ensure
* the write sector or block is erased before. Otherwise, the data on
* flash may be different from the input data. 
* Parameters:
*   write_addr    [in]    write address 
*   data          [in]    write data
*   size          [in]    the size of write data
*
* Return:
* 	success:QL_EMBED_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_embed_nor_flash_e ql_embed_nor_flash_write(uint32 write_addr,void *data,size_t size);

/*****************************************************************
* Function: ql_embed_nor_flash_read
* Description: embed nor flash read data 
* Parameters:
*   read_addr     [in]    read address 
*   data          [in]    read data
*   size          [in]    the size of read data
*
* Return:
* 	success:QL_EMBED_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_embed_nor_flash_e ql_embed_nor_flash_read(uint32 read_addr,void *data,size_t size);

/*****************************************************************
* Function: ql_embed_nor_flash_erase
*
* Description: erase embed nor flash,
* Both erase_addr and size must be sector (4KB) aligned.
* Parameters:
*   erase_addr    [in]    erase address 
*   size          [in]    the size of erase
*
* Return:
* 	success:QL_EMBED_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_embed_nor_flash_e ql_embed_nor_flash_erase(uint32_t erase_addr,size_t size);

#ifdef __cplusplus
} /*"C" */
#endif


#endif





