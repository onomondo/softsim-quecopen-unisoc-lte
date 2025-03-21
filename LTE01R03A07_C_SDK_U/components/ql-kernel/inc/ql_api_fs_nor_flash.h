/**  @file
  ql_api_fs_nor_flash.h

  @brief
  quectel fs nor flash info interface definition.

*/
/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------
24/5/2021        Neo         Init version
=================================================================*/


#ifndef QL_API_FS_NOR_FLASH_H
#define QL_API_FS_NOR_FLASH_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
 
#define QL_NOR_FLASH_ERRCODE_BASE (QL_COMPONENT_STORAGE_EXTFLASH<<16)
/*========================================================================
*  Enumeration Definition
*========================================================================*/
typedef enum
{
	QL_NOR_FLASH_SUCCESS  						= QL_SUCCESS,
	QL_NOR_FLASH_INVALID_PARAM_ERR				= 1|QL_NOR_FLASH_ERRCODE_BASE,
	QL_NOR_FLASH_MOUNT_ERR,
	QL_NOR_FLASH_MKFS_ERR, 
	QL_NOR_FLASH_NOT_MOUNT,
} ql_errcode_nor_flash_e;


typedef struct
{
    uint32_t mid;           //Manufacturer ID
    uint32_t blknum;        //block count
    uint32_t blksize;       //block size
} ql_nor_hw_info_t;



/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_nor_flash_mkfs
*
* Description:   format nor flash in FM_FAT32
* 
* Parameters:
* 	opt	  		[in]                FM_FAT32(0x02)
*
* Return:
* 	QL_NOR_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nor_flash_e ql_nor_flash_mkfs(uint8_t opt);

/*****************************************************************
* Function: ql_nor_flash_umount
*
* Description:   Uninitialize the nor flash driver, umount nor flash
* 
* Parameters:	
*   NULL
*
* Return:
* 	QL_NOR_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nor_flash_e ql_nor_flash_umount(void);

/*****************************************************************
* Function: ql_nor_flash_mount
*
* Description:   Initialize the nor flash driver,and mount nor flash
* 
* Parameters:	
*   NULL
*
* Return:
* 	QL_NOR_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nor_flash_e ql_nor_flash_mount(void);

/*****************************************************************
* Function: ql_nor_get_hw_info
*
* Description: get information about nor
*
* Parameters:
*   info         [out] Pointer to info data to read
*
* Return:
* 	QL_NOR_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nor_flash_e ql_nor_get_hw_info(ql_nor_hw_info_t *info);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_FS_NOR_FLASH_H */


