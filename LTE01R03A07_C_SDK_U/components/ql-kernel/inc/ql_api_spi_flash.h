/**  @file
  ql_api_spi_flash.h

  @brief
  This file is used to define bt api for different Quectel Project.

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


#ifndef QL_API_SPI_FLASH_H
#define QL_API_SPI_FLASH_H
#include "ql_api_common.h"
#include "ql_api_spi.h"
#include "ql_gpio.h"


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
/****************************  error code about ql spi flash ***************************/
typedef enum
{
    QL_SPI_FLASH_SUCCESS                  =   0,
    //is the same with nomal spi
    QL_SPI_FLASH_ERROR                    =   1 | (QL_COMPONENT_STORAGE_EXTFLASH << 16),    //SPI总线其他错误
    QL_SPI_FLASH_PARAM_TYPE_ERROR,                  //参数类型错误
    QL_SPI_FLASH_PARAM_DATA_ERROR,                  //参数数据错误
    QL_SPI_FLASH_PARAM_ACQUIRE_ERROR,               //参数无法获取
    QL_SPI_FLASH_PARAM_NULL_ERROR,                  //参数NULL错误
    QL_SPI_FLASH_DEV_NOT_ACQUIRE_ERROR,             //无法获取SPI总线
    QL_SPI_FLASH_PARAM_LENGTH_ERROR,                //参数长度错误
    QL_SPI_FLASH_MALLOC_MEM_ERROR,                  //申请内存错误
    QL_SPI_FLASH_ALIGNED_ERROR,                     //地址不是4字节对齐

    QL_SPI_FLASH_NOT_FLASH_CONN_ERROR,              //没有接入FLASH芯片
    QL_SPI_FLASH_NOT_SUPPORT_ERROR,                 //FLASH型号不支持
    QL_SPI_FLASH_OP_NOT_SUPPORT_ERROR,              //FLASH型号不支持此操作

    QL_SPI_FLASH_ECC_ERROR,                         //nand flash ecc error
    QL_SPI_FLASH_PROGRAM_ERROR,                     //flash program error
    QL_SPI_FLASH_ERASE_ERROR,                       //flash erase error
    QL_SPI_FLASH_MUTEX_CREATE_ERROR,                //互斥锁创建失败报错
    QL_SPI_FLASH_MUTEX_LOCK_ERROR,                  //互斥锁上锁报错
    QL_SPI_FLASH_SET_GPIO_ERROR,                    //设置GPIO出错
}ql_errcode_spi_flash_e;

typedef struct
{
    ql_spi_port_e port;                             //SPI总线选择
    ql_spi_clk_e spiclk;                            //SPI时钟配置
    ql_spi_input_sel_e input_sel;                   //SPI输入引脚选择
    ql_spi_transfer_mode_e transmode;               //SPI工作模式，目前仅支持QL_SPI_DIRECT_POLLING和QL_SPI_DMA_POLLING
    ql_spi_cs_sel_e cs;                             //SPI片选CS引脚选择
    ql_GpioNum cs_gpio;                             //SPI片选CS使用的GPIO引脚
    ql_spi_release_e release_flag;                  //使用完SPI总线，是否需要释放总线
} ql_spi_flash_config_s;


/*========================================================================
 *  function Definition
 *========================================================================*/
 


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI_FLASH_H */



