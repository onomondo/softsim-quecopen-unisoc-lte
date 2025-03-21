/**  @file
  ql_nor_block_device.h

  @brief
  This file is used to define log interface for different Quectel Project.

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
	
#ifndef QL_NOR_BLOCK_DEVICE_H
#define QL_NOR_BLOCK_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "block_device.h"

/*========================================================================
 *  Variable Definition
 *========================================================================*/

#define CONFIG_FS_NOR_FLASH_MOUNT_POINT     "/exnor"


/*========================================================================
 *  function Definition
 *========================================================================*/

void ql_fs_umount_nor_flash();

bool ql_fs_mount_nor_flash();

blockDevice_t *ql_nor_block_create(uint32_t name);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_NOR_BLOCK_DEVICE_H */

