/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#ifndef _QUEC_BOOT_SPI_H_
#define _QUEC_BOOT_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ql_api_common.h"
#include "quec_boot_pin_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "quec_pin_index.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        spi api接口
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define QL_BOOT_SPI_DMA_IRQ_SIZE 512         //QL_SPI_DMA_IRQ模式最多只有512个字节
#define QL_BOOT_SPI_DMA_ADDR_ALIN 4          //QL_SPI_DMA_IRQ和QL_SPI_DMA_POLLING模式，地址必须4字节对齐

/*========================================================================
 *  Variable Definition
 *========================================================================*/

/****************************  error code about ql spi/flash/nandflash/norflash    ***************************/
typedef enum
{
	QL_BOOT_SPI_SUCCESS                  =   0,
    
    QL_BOOT_SPI_ERROR                    =   1 | (QL_COMPONENT_BSP_SPI << 16),   //SPI总线其他错误
    QL_BOOT_SPI_PARAM_TYPE_ERROR,                        //参数类型错误
    QL_BOOT_SPI_PARAM_DATA_ERROR,                        //参数数据错误
    QL_BOOT_SPI_PARAM_ACQUIRE_ERROR,                     //参数无法获取
    QL_BOOT_SPI_PARAM_NULL_ERROR,                        //参数NULL错误
    QL_BOOT_SPI_DEV_NOT_ACQUIRE_ERROR,                   //无法获取SPI总线
    QL_BOOT_SPI_PARAM_LENGTH_ERROR,                      //参数长度错误
    QL_BOOT_SPI_MALLOC_MEM_ERROR,                        //申请内存错误

    QL_BOOT_SPI_FLASH_NOT_FLASH_CONN_ERROR,              //没有接入FLASH芯片
    QL_BOOT_SPI_FLASH_NOT_SUPPORT_ERROR,                 //FLASH型号不支持
    QL_BOOT_SPI_FLASH_OP_NOT_SUPPORT_ERROR,              //FLASH型号不支持此操作

    QL_BOOT_SPI_FLASH_ECC_ERROR,                         //nand flash ecc error
    QL_BOOT_SPI_FLASH_PROGRAM_ERROR,                     //flash program error
    QL_BOOT_SPI_FLASH_ERASE_ERROR,                       //flash erase error    
              
    QL_BOOT_NAND_SPI_INIT_ERR,
    QL_BOOT_NAND_READ_DATA_ERR,
    QL_BOOT_NAND_WRITE_DATA_ERR,
    QL_BOOT_NAND_READ_ECC_2BIT_ERR,
    QL_BOOT_NAND_READ_ECC_1BIT_ERR,
    QL_BOOT_NAND_WRITE_STATUS_ERR,
    QL_BOOT_NAND_ERASE_ERR,
    QL_BOOT_NAND_ERASE_STATUS_ERR,

	QL_BOOT_NOR_GET_FLASH_PROPS_ERR,
	QL_BOOT_NOR_NOT_SUPPORT_ERROR,
    QL_BOOT_NOR_WAIT_WRITE_FINISH_TIMEOUT_ERR,
    
}ql_boot_errcode_spi_e;

typedef enum
{
    QL_BOOT_SPI_CS0 = 0,                                 //选择cs0为SPI片选CS引脚
    QL_BOOT_SPI_CS1,                                     //选择cs1为SPI片选CS引脚
    QL_BOOT_SPI_CS2,                                     //选择cs2为SPI片选CS引脚,not use now
    QL_BOOT_SPI_CS3,                                     //选择cs3为SPI片选CS引脚,not use now
    QL_BOOT_SPI_GPIO,                                    //使用控制GPIO的方式来控制CS功能。注意，对应的CS引脚不要再配置成SPI CS功能，需要配置成GPIO
}ql_boot_spi_cs_sel_e;

typedef enum
{
    QL_BOOT_SPI_INPUT_FALSE,                             //SPI不允许输入（读取）
    QL_BOOT_SPI_INPUT_TURE,                              //SPI允许输入（读取）
}ql_boot_spi_input_mode_e;

typedef enum
{
    QL_BOOT_SPI_CS_ACTIVE_HIGH,                          //SPI总线操作时，CS脚为高
    QL_BOOT_SPI_CS_ACTIVE_LOW,                           //SPI总线操作时，CS脚为低
} ql_boot_spi_cs_pol_e;
    
typedef enum
{
    QL_BOOT_SPI_CPOL_LOW = 0,                            //SPI未使能时，CLK线为低电平，第一个边沿是上升沿
    QL_BOOT_SPI_CPOL_HIGH,                               //SPI未使能时，CLK线为高电平，第一个边沿是下降沿
} ql_boot_spi_cpol_pol_e;

typedef enum
{
    QL_BOOT_SPI_CPHA_1Edge,                              //MOSI延时一个边沿,CLK和MISO延时两个边沿，即发送的数据先准备好，才有CLK
    QL_BOOT_SPI_CPHA_2Edge,                              //MOSI延时两个边沿，CLK延时2个边沿，MISO延时3个边沿。数据和CLK同时准备好
}ql_boot_spi_cpha_pol_e;

//SPI mode0：ql_spi_cpol_pol_e选择QL_SPI_CPOL_LOW，ql_spi_cpha_pol_e选择QL_SPI_CPHA_1Edge
//SPI mode1：ql_spi_cpol_pol_e选择QL_SPI_CPOL_LOW，ql_spi_cpha_pol_e选择QL_SPI_CPHA_2Edge
//SPI mode2：ql_spi_cpol_pol_e选择QL_SPI_CPOL_HIGH，ql_spi_cpha_pol_e选择QL_SPI_CPHA_1Edge
//SPI mode3：ql_spi_cpol_pol_e选择QL_SPI_CPOL_HIGH，ql_spi_cpha_pol_e选择QL_SPI_CPHA_2Edge

typedef enum
{
    QL_BOOT_SPI_DI_0 = 0,                                //选择DI0为数据输入引脚,not use now
    QL_BOOT_SPI_DI_1,                                    //选择DI1为数据输入引脚
    QL_BOOT_SPI_DI_2,                                    //选择DI2为数据输入引脚,not use now
}ql_boot_spi_input_sel_e;

typedef enum
{
    QL_BOOT_SPI_DIRECT_POLLING = 0,                      //FIFO读写，轮询等待。
    QL_BOOT_SPI_DIRECT_IRQ,                              //FIFO读写，中断通知,not use now
    QL_BOOT_SPI_DMA_POLLING,                             //DMA读写，轮询等待。此模式下，和SD卡不能同时使用。使能SD卡情形下，申请DMA通道失败，SPI初始化会失败
    QL_BOOT_SPI_DMA_IRQ,                                 //DMA读写，中断通知,not use now
}ql_boot_spi_transfer_mode_e;

typedef enum
{	
	QL_BOOT_SPI_CLK_INVALID = -1,                        //无效时钟选择
	QL_BOOT_SPI_CLK_781_25KHZ = 781250,                  //时钟：781.25K
	QL_BOOT_SPI_CLK_1_5625MHZ = 1562500,                 //时钟：1.5625M
	QL_BOOT_SPI_CLK_3_125MHZ = 3125000,                  //时钟：3.125M
	QL_BOOT_SPI_CLK_6_25MHZ = 6250000,                   //时钟：6.25M
	QL_BOOT_SPI_CLK_10MHZ = 10000000,                    //时钟：10M
	QL_BOOT_SPI_CLK_12_5MHZ = 12500000,                  //时钟：12.5M	
	QL_BOOT_SPI_CLK_20MHZ = 20000000,                    //时钟：20M
	QL_BOOT_SPI_CLK_25MHZ = 25000000,                    //时钟：25M
	QL_BOOT_SPI_CLK_33_33MHZ = 33000000,                 //时钟：33.33M
	QL_BOOT_SPI_CLK_50MHZ_MAX = 50000000,                //时钟：50M
}ql_boot_spi_clk_e;

typedef struct
{
    ql_boot_spi_input_mode_e input_mode;
    ql_boot_spi_port_e port;
    unsigned int framesize;
    ql_boot_spi_clk_e spiclk;
    ql_boot_spi_cs_pol_e cs_polarity0;
    ql_boot_spi_cs_pol_e cs_polarity1;
    ql_boot_spi_cpol_pol_e cpol;
    ql_boot_spi_cpha_pol_e cpha;
    ql_boot_spi_input_sel_e input_sel;
    ql_boot_spi_transfer_mode_e transmode;
    ql_boot_spi_cs_sel_e cs;
    int cs_gpio;
} ql_boot_spi_config_s;

/*========================================================================
 *  function Definition
 *========================================================================*/
 /*****************************************************************
* Function: quec_boot_spi_pin_init
*
* Description:
*   设置相关pin脚的复用功能为spi功能
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_boot_errcode_pin_e
*
*****************************************************************/
ql_boot_errcode_pin_e quec_boot_spi_pin_init(ql_boot_spi_port_e port);
/*****************************************************************
* Function: ql_boot_spi_init
*
* Description:
*   初始化SPI总线
* 
* Parameters:
*   port        [in]    SPI总线选择
*   transmode   [in]    SPI总线工作的传输模式，仅支持QL_SPI_DIRECT_POLLING模式
*   spiclk      [in]    SPI总线传输速率配置
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_init(ql_boot_spi_port_e port, ql_boot_spi_transfer_mode_e transmode, ql_boot_spi_clk_e spiclk);

/*****************************************************************
* Function: ql_boot_spi_init_ext
*
* Description:
*   初始化SPI总线，与ql_boot_spi_init相比，有更多的配置选择
* 
* Parameters:
*   spi_config  [in]    SPI总线配置
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_init_ext(ql_boot_spi_config_s spi_config);

/*****************************************************************
* Function: ql_boot_spi_read_follow_write
*
* Description:
*   对SPI总线执行写和读2个操作
*   first write and after read.with the two operation,the cs is always low
* 
* Parameters:
*   port        [in]    SPI总线选择
*   outbuf      [in]    写入的数据
*   outlen      [in]    写入的数据长度
*   inbuf       [out]   读取的数据
*   inlen       [in]    需要读取的数据长度
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_read_follow_write(ql_boot_spi_port_e port, unsigned char *outbuf, unsigned int outlen, unsigned char *inbuf, unsigned int inlen);

/*****************************************************************
* Function: ql_boot_spi_write_read
*
* Description:
*   write and read simultaneously,use the same clock。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
* 
* Parameters:
*   port        [in]    SPI bus select
*   inbuf       [in]    the data will be read and saved to inbuf
*   outbuf      [out]   the data will be write and send to the SPI bus
*   len         [in]    data length,same length with write data and read data
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_write_read(ql_boot_spi_port_e port, unsigned char *inbuf, unsigned char *outbuf, unsigned int len);

/*****************************************************************
* Function: ql_boot_spi_read
*
* Description:
*   对SPI总线执行读操作。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
*
* Parameters:
*   port        [in]    SPI总线选择
*   buf         [out]   读取的数据
*   len         [in]    需要读取的数据长度
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_read(ql_boot_spi_port_e port, unsigned char *buf, unsigned int len);

/*****************************************************************
* Function: ql_boot_spi_write
*
* Description:
*   对SPI总线执行写操作。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
* 
* Parameters:
*   port        [in]    SPI总线选择
*   buf         [in]    写入的数据
*   len         [in]    写入的数据长度
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_write(ql_boot_spi_port_e port, unsigned char *buf, unsigned int len);

/*****************************************************************
* Function: ql_boot_spi_release
*
* Description:
*   释放SPI总线
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_release(ql_boot_spi_port_e port);

/*****************************************************************
* Function: ql_boot_spi_cs_low
*
* Description:
*   对SPI总线的CS引脚强制拉低
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_cs_low(ql_boot_spi_port_e port);

/*****************************************************************
* Function: ql_boot_spi_cs_high
*
* Description:
*   对SPI总线的CS引脚强制拉高
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_cs_high(ql_boot_spi_port_e port);

/*****************************************************************
* Function: ql_boot_spi_cs_auto
*
* Description:
*   SPI总线的CS引脚恢复成系统控制，系统默认CS引脚由系统控制
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_cs_auto(ql_boot_spi_port_e port);




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        spi flash api接口
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
    ql_boot_spi_port_e port;                             //SPI总线选择
    ql_boot_spi_clk_e spiclk;                            //SPI时钟配置
    ql_boot_spi_input_sel_e input_sel;                   //SPI输入引脚选择
    ql_boot_spi_transfer_mode_e transmode;               //SPI工作模式，目前仅支持QL_SPI_DIRECT_POLLING
    ql_boot_spi_cs_sel_e cs;                             //SPI片选CS引脚选择
    int cs_gpio;                                         //SPI CS使用GPIO来控制
} ql_boot_spi_flash_config_s;

#define QL_BOOT_FLASH_WRITE_ENABLE                   0x06
#define QL_BOOT_FLASH_READ_DEVID                     0x9F

typedef enum
{
    QL_BOOT_SPI_FLASH_NOR_FLASH = 0,                 //nor flash
    QL_BOOT_SPI_FLASH_NAND_FLASH,                    //nand flash
}ql_boot_spi_flash_type_e;

typedef enum
{
    QUEC_BOOT_NOR_FLASH_WAIT_DELAY_NONE = 0,                 //不需要释放CPU，读写操作，扇区，块擦除
    QUEC_BOOT_NOR_FLASH_WAIT_DELAY,                          //擦除整个芯片需要释放CPU，有些芯片擦除整个芯片耗时超过3S
}quec_boot_spi_nor_flash_wait_mode_e;


/*****************************************************************
* Function: ql_spi_flash_init_ext
*
* Description:
*   扩展初始化SPI FLASH，与ql_spi_flash_init有更多的设置选项，，调用此函数前，必须使用ql_pin_set_func设置好SPI引脚
* 
* Parameters:
*   flash_config    [in]  SPI flash配置参数
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_flash_init_ext(ql_boot_spi_flash_config_s flash_config);

/*****************************************************************
* Function: ql_spi_flash_enable_write
*
* Description:
*   使能flash操作，注意，此API没有等待操作完成，发送06指令
* 
* Parameters:
*   port     [in]  SPI总线选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_flash_enable_write(ql_boot_spi_port_e port);


#ifdef __cplusplus
} /*"C" */
#endif


#endif
