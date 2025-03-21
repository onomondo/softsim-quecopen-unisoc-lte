/**  @file
  nor_flash_demo.h

  @brief
  This file is used to define nor flash demo for different Quectel Project.

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


#ifndef FS_NOR_FLASH_DEMO_H
#define FS_NOR_FLASH_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define FS_NOR_FLASH_DEMO_TASK_PRIO           12
#define FS_NOR_FLASH_DEMO_TASK_STACK_SIZE     10*1024
#define FS_NOR_FLASH_DEMO_TASK_EVENT_CNT      4

#define TEST_DISK_NOR   "EXNOR:"
#define TEST_DIR_NOR	 "test_dir"
#define FILE_NAME_NOR	 "test_file1.txt"

#define DIR_PATH_NOR    ""TEST_DISK_NOR"/"TEST_DIR_NOR""
#define FILE_PATH_NOR	 ""DIR_PATH_NOR"/"FILE_NAME_NOR""


#define TEST_STR_NOR     "0123456789ABCDEFGHIGKLMNOPQRSTUVWXYZ"


/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_fs_nor_flash_demo_init(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* FS_NOR_FLASH_DEMO_H */


