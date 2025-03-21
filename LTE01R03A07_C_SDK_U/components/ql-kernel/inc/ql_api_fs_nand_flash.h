/**  @file
  ql_api_fs_nand_flash.h

  @brief
  quectel fs nand flash info interface definition.

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


#ifndef QL_API_FS_NAND_FLASH_H
#define QL_API_FS_NAND_FLASH_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
 
#define QL_NAND_FLASH_ERRCODE_BASE (QL_COMPONENT_STORAGE_EXT_NANDFLASH<<16)
/*========================================================================
*  Enumeration Definition
*========================================================================*/
typedef enum
{
	QL_NAND_FLASH_SUCCESS  						= QL_SUCCESS,
	QL_NAND_FLASH_INVALID_PARAM_ERR				= 1|QL_NAND_FLASH_ERRCODE_BASE,
	QL_NAND_FLASH_MOUNT_ERR,
	QL_NAND_FLASH_MKFS_ERR, 
	QL_NAND_FLASH_NOT_MOUNT,
	QL_NAND_FLASH_MUTEX_CREATE_ERR,
	QL_NAND_FLASH_MUTEX_LOCK_ERR,
} ql_errcode_nand_flash_e;


typedef struct
{
    uint32_t mid;           //Manufacturer ID
    uint32_t blknum;        //block count
    uint32_t blksize;       //block size
} ql_nand_hw_info_t;



/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_nand_flash_mkfs
*
* Description:   format nand flash in FM_FAT32
* 
* Parameters:
* 	opt	  		[in]                FM_FAT32(0x02)
*
* Return:
* 	QL_NAND_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nand_flash_e ql_nand_flash_mkfs(uint8_t opt);

/*****************************************************************
* Function: ql_nand_flash_umount
*
* Description:   Uninitialize the nand flash driver, umount nand flash
* 
* Parameters:	
*   NULL
*
* Return:
* 	QL_NAND_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nand_flash_e ql_nand_flash_umount(void);

/*****************************************************************
* Function: ql_nand_flash_mount
*
* Description:   Initialize the nand flash driver,and mount nand flash
* 
* Parameters:	
*   NULL
*
* Return:
* 	QL_NAND_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nand_flash_e ql_nand_flash_mount(void);

/*****************************************************************
* Function: ql_nand_get_hw_info
*
* Description: get information about nand
*
* Parameters:
*   info         [out] Pointer to info data to read
*
* Return:
* 	QL_NAND_FLASH_SUCCESS           success
*	other 	                        error code
*
*****************************************************************/
ql_errcode_nand_flash_e ql_nand_get_hw_info(ql_nand_hw_info_t *info);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_FS_NAND_FLASH_H */


