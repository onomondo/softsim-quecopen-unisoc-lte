/**  @file
  quec_file_cfg_defs.h

  @brief
  This file is used to define boot file system config defines for different Quectel Project.

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

#ifndef QUEC_FILE_CFG_DEFS_H
#define QUEC_FILE_CFG_DEFS_H

#include "fs_config.h"

#ifdef __cplusplus
extern "C" {
#endif

//********************** file system mount point ***********************/
#define CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT     "/ext"         //SPI6 EXT NOR  FLASH,  mount sffs  file system
#define CONFIG_FS_SPI_EXT_NAND_FAT_MOUNT_POINT      "/exnand"      //SPI4/SPI6 EXT NAND FLASH,  mount fatfs file system
#define CONFIG_FS_SPI4_EXT_NOR_FAT_MOUNT_POINT      "/exnor"       //SPI4 EXT NOR  FLASH,  mount fatfs file system
#define CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT     "/ext4n"       //SPI4 EXT NOR  FLASH,  mount sffs  file system
#ifndef CONFIG_FS_SDCARD_MOUNT_POINT
#define CONFIG_FS_SDCARD_MOUNT_POINT                "/sdcard0"
#endif
#define CONFIG_FS_SDCARD_2ND_MOUNT_POINT            "/sdcard1"
#define CONFIG_FS_USBHOST_UDISK_MOUNT_POINT         "/udisk1"      //mass storage in USB host mode, mount FILE-X fs

#define CONFIG_FS_FACTORY_MOUNT_POINT               "/factory"
#define CONFIG_FS_MODEM_MOUNT_POINT                 "/modem" 

#define CONFIG_FS_UFS_SFFS_MOUNT_POINT              "/user" 
#define CONFIG_FS_SFS_SFFS_MOUNT_POINT              "/qsfs"

//********************** file system reserve size ***********************/
#define CONFIG_RESERVE_SIZE            80*1024     //Reserved space for internal flash file system(64KB is used for fota and the rest for QUEC-related nv)
#define QL_FS_ALIGEN_RESERVE_SIZE      3*1024	   //reserve it for byte alignment
#define UNISOC_EFS_RESERVE_SIZE        64*1024     //reserved by unisoc, we cannot write to ext flash when free size less than 64k, so cut it out from free space

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_FILE_CFG_DEFS_H */


