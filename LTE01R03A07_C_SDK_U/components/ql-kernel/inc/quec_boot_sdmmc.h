/**  @file
  quec_boot_sdmmc.h

  @brief
  This file is used to define boot uart api for different Quectel Project.

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
24/01/2021        Neo         Init version
=================================================================*/

#ifndef QUEC_BOOT_SDMMC_H
#define QUEC_BOOT_SDMMC_H

#include "quec_boot_pff.h"
#include "quec_proj_config.h"


#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define TCARD_SECTOR_SIZE           (512)
#define TCARD_CACHE_SECTOR_COUNT    (64)
#define SDMMC_OPT_BLOCK_COUNT       (1)

#define QUEC_ECHO_ENBLE              0
#define QUEC_MINI_SD_INIT            1  //剪裁boot空间

#define QL_BOOT_SDMMC_CLK_FREQ_SDHC  50000000  //50MHz
#define QL_BOOT_SDMMC_CLK_FREQ_SD    25000000  //25MHz

/*===========================================================================
 * Enum
 ===========================================================================*/
typedef uint32_t (*ql_boot_sdmmc_get_fatfs_offset_cb)(uint32_t block_count);

typedef struct
{
	uint32_t sdmmc_clk;			//sdmmc时钟频率,400000 <= clk <= 50000000,默认25MHz
	uint32_t power_mv;			//sdmmc电压,SD卡默认3.2v,emmc默认1.8v
	uint32_t fatfs_offset;//用户SD卡文件系统前裸写分区block大小,默认为0,针对分区以外情况
	ql_boot_sdmmc_get_fatfs_offset_cb cb;//调用回调传给客户SD卡总扇区数,让客户动态返回文件系统偏移量
} ql_boot_sdmmc_cfg_t;

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
extern ql_boot_sdmmc_cfg_t ql_boot_sdmmc_cfg;			//sdmmc配置
#endif

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)
/*****************************************************************
* Function: quec_boot_sdmmc_init
* Description: 初始化sd驱动并挂载只读文件系统
* Parameters:
*   pt            [in]  分区号
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_sdmmc_init(ql_boot_fat_mount_pt_e pt);

/*****************************************************************
* Function: quec_boot_sdmmc_destroy与quec_boot_sdmmc_close功能一样
* Description: 关闭sd驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_sdmmc_destroy();

/*****************************************************************
* Function: quec_boot_sdmmc_open
* Description: 初始化sd驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
extern int quec_boot_sdmmc_open(void);

/*****************************************************************
* Function: quec_boot_sdmmc_close
* Description: 关闭sd驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
extern int quec_boot_sdmmc_close(void);

/*****************************************************************
* Function: quec_boot_sdmmc_read
* Description: 一次只能读一个扇区
* Parameters:
*   nr            [in]  扇区号
*   offset        [in]  数据相对扇区开始的偏移
*   size          [in]  读数据的大小，最大512字节
*   data          [out] 读数据存放的buffer
* Return:
*      返回读的大小,单位字节
*      负值,failure
*****************************************************************/
extern int quec_boot_sdmmc_read(uint64_t nr, unsigned int offset, int size, void *data);

/*****************************************************************
* Function: quec_boot_sdmmc_write
* Description: 一次可以写一个或者多个扇区
* Parameters:
*   nr            [in]  扇区号
*   count         [in]  扇区个数
*   data          [in]  写数据存放的buffer
* Return:
*     返回写的扇区数,单位:一个扇区的大小
*     负值，failure
*****************************************************************/
extern int quec_boot_sdmmc_write(uint64_t nr,int count,const void *data);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
/*****************************************************************
* Function: quec_boot_sdmmc_init_ex
* Description: 初始化sd驱动并挂载可读写文件系统
* Parameters:
*   pt            [in]  分区号
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_sdmmc_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
/*****************************************************************
* Function: quec_boot_emmc_init
* Description: 初始化emmc驱动并挂载只读文件系统
* Parameters:
*   pt            [in]  分区号
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_emmc_init(ql_boot_fat_mount_pt_e pt);

/*****************************************************************
* Function: quec_boot_emmc_destroy与quec_boot_emmc_close功能一样
* Description: 关闭sd驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_emmc_destroy();

/*****************************************************************
* Function: quec_boot_emmc_open
* Description: 初始化emmc驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
extern int quec_boot_emmc_open(void);

/*****************************************************************
* Function: quec_boot_emmc_close
* Description: 关闭emmc驱动
* Parameters:	void
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
extern int quec_boot_emmc_close(void);

/*****************************************************************
* Function: quec_boot_emmc_read
* Description: 一次只能读一个扇区
* Parameters:
*   nr            [in]  扇区号
*   offset        [in]  数据相对扇区开始的偏移
*   size          [in]  读数据的大小，最大512字节
*   data          [out] 读数据存放的buffer
* Return:
*      返回读的大小,单位字节
*      负值,failure
*****************************************************************/
extern int quec_boot_emmc_read(uint64_t nr, unsigned int offset, int size, void *data);

/*****************************************************************
* Function: quec_boot_emmc_write
* Description: 一次可以写一个或者多个扇区
* Parameters:
*   nr            [in]  扇区号
*   count         [in]  扇区个数
*   data          [in]  写数据存放的buffer
* Return:
*     返回写的扇区数,单位:一个扇区的大小
*     负值，failure
*****************************************************************/
extern int quec_boot_emmc_write(uint64_t nr,int count,const void *data);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
/*****************************************************************
* Function: quec_boot_emmc_init_ex
* Description: 初始化emmc驱动并挂载可读写文件系统
* Parameters:
*   pt            [in]  分区号
* Return:
*     0        succeed
*     OTHER    failure
*****************************************************************/
int quec_boot_emmc_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_SDMMC_H */


