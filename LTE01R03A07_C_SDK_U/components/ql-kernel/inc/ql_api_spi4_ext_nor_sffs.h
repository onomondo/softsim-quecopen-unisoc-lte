/**  @file
  ql_api_spi4_ext_nor_sffs.h

  @brief
  This file is used to define spi4 ext nor sffs api for different Quectel Project.

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


#ifndef QL_API_SPI4_EXT_NOR_SFFS_H
#define QL_API_SPI4_EXT_NOR_SFFS_H
#include "ql_api_common.h"
#include "ql_api_spi.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
/****************************  error code about ql spi nor flash ***************************/
typedef enum
{
    QL_SPI4_EXT_NOR_SFFS_SUCCESS                       =   0,
    QL_SPI4_EXT_NOR_SFFS_NOT_INIT_ERROR                =   100 | (QL_COMPONENT_STORAGE_EXTFLASH << 16), //外置flash初始化错误  
    QL_SPI4_EXT_NOR_SFFS_CREATE_FBDEV2SPI_ERROR,               //创建块设备分区错误
    QL_SPI4_EXT_NOR_SFFS_SFFS_MOUNT_ERROR,                     //挂载错误
    QL_SPI4_EXT_NOR_SFFS_SFFS_UNMOUNT_ERROR,                   //反挂载错误
    QL_SPI4_EXT_NOR_SFFS_SFFS_MKFS_ERROR,                      //格式化错误
    QL_SPI4_EXT_NOR_SFFS_LOCK_ERROR,                           //互斥锁上锁报错
}ql_errcode_spi4_extnsffs_e;

typedef enum
{
    QL_SPI4_EXT_NOR_SFFS_IS_UNMOUNTED  =   0,
    QL_SPI4_EXT_NOR_SFFS_IS_MOUNTED
}ql_spi4_extnsffs_status_e;

/*========================================================================
 *  function Definition
 *========================================================================*/
 
/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_set_port
*
* Description:
*   初始化4线spi EXT NOR FLASH 后告之sffs文件系统使用的是哪一路SPI
*   在初始化4线spi EXT NOR FLASH后必须调用此接口设置当前使用的SPI口，
*   否则使用默认的QL_SPI_PORT1。
* 
* Parameters:
*   ql_spi_port_e
*
* Return:NULL
*
*****************************************************************/
void ql_spi4_ext_nor_sffs_set_port(ql_spi_port_e spi_port);

/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_get_port
*
* Description:
*   获取sffs文件系统当前使用的是哪一路SPI口读写4线spi EXT NOR FLASH
* 
* Parameters:
*   NULL
*
* Return:ql_spi_port_e
*
*****************************************************************/
ql_spi_port_e ql_spi4_ext_nor_sffs_get_port(void);

/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_mount
*
* Description:
*   挂载4线 spi ext nor flash到sffs文件系统
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi4_extnsffs_e
*
*****************************************************************/
ql_errcode_spi4_extnsffs_e ql_spi4_ext_nor_sffs_mount(void);

/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_unmount
*
* Description:
*   从sffs文件系统中卸载4线 spi ext nor flash设备
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi4_extnsffs_e
*
*****************************************************************/
ql_errcode_spi4_extnsffs_e ql_spi4_ext_nor_sffs_unmount(void);

/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_mkfs
*
* Description:
*   格式化4线 spi ext nor flash sffs文件系统
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi4_extnsffs_e
*
*****************************************************************/
ql_errcode_spi4_extnsffs_e ql_spi4_ext_nor_sffs_mkfs(void);

/*****************************************************************
* Function: ql_spi4_ext_nor_sffs_is_mount
*
* Description:
*   是否已经挂载sffs文件系统
* 
* Parameters:
*   NULL
*
* Return:ql_spi4_extnsffs_status_e
*
*****************************************************************/
ql_spi4_extnsffs_status_e ql_spi4_ext_nor_sffs_is_mount(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI4_EXT_NOR_SFFS_H */



