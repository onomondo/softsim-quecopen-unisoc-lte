/**  @file
  nand_flash_demo.h

  @brief
  This file is used to define nand flash demo for different Quectel Project.

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
24/5/2021        Neo         Init version
=================================================================*/


#ifndef FS_NAND_FLASH_DEMO_H
#define FS_NAND_FLASH_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define FS_NAND_FLASH_DEMO_TASK_PRIO           12
#define FS_NAND_FLASH_DEMO_TASK_STACK_SIZE     8*1024
#define FS_NAND_FLASH_DEMO_TASK_EVENT_CNT      4

#define TEST_DISK_NAND   "EXNAND:"
#define TEST_DIR_NAND	 "test_dir"
#define FILE_NAME_NAND	 "test_file1.txt"

#define DIR_PATH_NAND    ""TEST_DISK_NAND"/"TEST_DIR_NAND""
#define FILE_PATH_NAND	 ""DIR_PATH_NAND"/"FILE_NAME_NAND""


#define TEST_STR_NAND     "0123456789ABCDEFGHIGKLMNOPQRSTUVWXYZ"


/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_fs_nand_flash_demo_init(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* FS_NAND_FLASH_DEMO_H */


