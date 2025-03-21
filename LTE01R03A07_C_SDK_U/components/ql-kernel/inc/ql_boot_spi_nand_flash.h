/**  @file
  ql_boot_spi_nand_flash.h

  @brief
  This file is used to define boot spi nand flash api for different Quectel Project.

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


#ifndef QL_BOOT_SPI_NAND_FLASH_H
#define QL_BOOT_SPI_NAND_FLASH_H
#include "ql_api_common.h"
#include "quec_boot_pin_cfg.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
#include "quec_boot_spi.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        spi nand flash api接口
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*========================================================================
 *  Variable Definition
 *========================================================================*/
 
#define QL_BOOT_SPI_NAND_PAGE_SIZE                       2048
#define QL_BOOT_SPI_NAND_SPARE_SIZE                      64
#define QL_BOOT_SPI_NAND_PAGE_SPARE_SIZE                 (QL_BOOT_SPI_NAND_PAGE_SIZE+QL_BOOT_SPI_NAND_SPARE_SIZE)

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
typedef ql_boot_spi_flash_config_s ql_boot_spi_nand_config_s ;
#endif

typedef enum
{
    QL_BOOT_NAND_FLASH_STATUS_1 = 0,                     //Status Register-1
    QL_BOOT_NAND_FLASH_STATUS_2,                         //Status Register-2
    QL_BOOT_NAND_FLASH_STATUS_3,                         //Status Register-3
    QL_BOOT_NAND_FLASH_STATUS_4,                         //Status Register-4
}ql_boot_spi_nand_status_reg_e;

typedef enum
{
    QL_BOOT_NAND_FLASH_ID_16BIT = 0,                    //Manufacturer ID(8bit) + device ID(8bit)
    QL_BOOT_NAND_FLASH_ID_24BIT,                        //Manufacturer ID(8bit) + device ID(16bit)
}ql_boot_spi_nand_id_type_e;

typedef enum
{
	QL_BOOT_SPI6_NAND_SUCCESS                  =   0,
    
    QL_BOOT_SPI6_NAND_ERROR                    =   1 | (QL_COMPONENT_STORAGE_EXT_NANDFLASH << 16),   //其他错误
    QL_BOOT_SPI6_NAND_PARAM_TYPE_ERROR,                        //参数类型错误
    QL_BOOT_SPI6_NAND_PARAM_DATA_ERROR,                        //参数数据错误
    QL_BOOT_SPI6_NAND_PARAM_ACQUIRE_ERROR,                     //参数无法获取
    QL_BOOT_SPI6_NAND_PARAM_NULL_ERROR,                        //参数NULL错误
    QL_BOOT_SPI6_NAND_DEV_NOT_ACQUIRE_ERROR,                   //无法获取SPI总线
    QL_BOOT_SPI6_NAND_PARAM_LENGTH_ERROR,                      //参数长度错误
    QL_BOOT_SPI6_NAND_MALLOC_MEM_ERROR,                        //申请内存错误
    QL_BOOT_SPI6_NAND_NOT_SUPPORT_ERROR,
    
    QL_BOOT_SPI6_NAND_ECC_ERROR,                         //nand flash ecc error
    QL_BOOT_SPI6_NAND_PROGRAM_ERROR,                     //flash program error
    QL_BOOT_SPI6_NAND_ERASE_ERROR,                       //flash erase error    
              
    QL_BOOT_SPI6_NAND_SPI6_INIT_ERR,
    QL_BOOT_SPI6_NAND_READ_DATA_ERR,
    QL_BOOT_SPI6_NAND_WRITE_DATA_ERR,
    QL_BOOT_SPI6_NAND_READ_ECC_2BIT_ERR,
    QL_BOOT_SPI6_NAND_READ_ECC_1BIT_ERR,
    QL_BOOT_SPI6_NAND_WRITE_STATUS_ERR,
    QL_BOOT_SPI6_NAND_ERASE_ERR,
    QL_BOOT_SPI6_NAND_ERASE_STATUS_ERR,
  
}ql_boot_errcode_spi6_nand_e;

typedef enum
{
    QL_BOOT_NAND_TYPE_STANDARD_SPI = 0,   //Standard SPI: SCLK, CS#, SI, SO 
    QL_BOOT_NAND_TYPE_QUAD_SPI,           //Quad SPI: SCLK, CS#, SIO0, SIO1, SIO2, SIO3
    QL_BOOT_NAND_TYPE_DUAL_SPI,           //Dual SPI: SCLK, CS#, SIO0, SIO1
}ql_boot_nand_spi_type_e;

typedef struct
{
    int  (*nand_init)(void);
    int  (*nand_uninit)(void);
    int  (*nand_read_page)(unsigned int block_num, unsigned short page_num, unsigned short column_addr, unsigned char *pbuffer, int len);
} ql_boot_nand_ops_t;

typedef enum
{
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_62MHZ = 1,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_82MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_124MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_142MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_166MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_182MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_200MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_222MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_250MHZ,
    QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_INVALID = 10,
}ql_boot_spi6_nand_flash_clk_e;

typedef struct
{
    ql_boot_spi_port_e port;                        //SPI6总线选择
    ql_boot_spi6_nand_flash_clk_e clk;              //SPI6时钟配置,默认BOOT_SPI6_NAND_FLASH_CLK_SRC_200MHZ
    uint8_t clk_div;                                //时钟分频,默认5,freq =200M/5=40M
    uint8_t quad_mode;                              //1--Quad SPII: SCLK, CS#, SIO0, SIO1, SIO2, SIO3  0--Dual SPI: SCLK, CS#, SIO0, SIO1
    uint8_t sample_delay;                           //延时采样,默认2
    uint8_t isslow;                                 //是否使用低速率时钟,1--low 0--fast(默认值为0),仅8910有效
} ql_boot_spi6_nand_config_s;
/*========================================================================
 *  function Definition
 *========================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
/*****************************************************************
* Function: ql_spi_nand_init
*
* Description:
*   初始化NAND FLASH
* 
* Parameters:
*   port     [in]  SPI总线选择
*   spiclk   [in]  SPI时钟选择
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_init(ql_boot_spi_port_e port, ql_boot_spi_clk_e spiclk);

/*****************************************************************
* Function: ql_spi_nand_init_ext
*
* Description:
*   扩展初始化NAND FLASH，与ql_spi_nand_init有更多的设置选项
* 
* Parameters:
*   nand_config   [in]  SPI配置参数
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_init_ext(ql_boot_spi_nand_config_s nand_config);

/*****************************************************************
* Function: ql_boot_spi_nand_read_devid_ex
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI bus select
*   mid      [out] manufacturer id
*   mid_type [out] manufacturer id type
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_read_devid_ex(ql_boot_spi_port_e port, unsigned  char *mid,ql_boot_spi_nand_id_type_e mid_type);

/*****************************************************************
* Function: ql_spi_nand_read_devid
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI bus select
*   mid      [out] manufacturer id
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
//ql_boot_errcode_spi_e ql_boot_spi_nand_read_devid(ql_boot_spi_port_e port, unsigned  char *mid);
#define ql_boot_spi_nand_read_devid(port, mid)  ql_boot_spi_nand_read_devid_ex(port, mid, QL_BOOT_NAND_FLASH_ID_24BIT)

/*****************************************************************
* Function: ql_spi_nand_read_page_spare
*
* Description:
*   read page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_read_page_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_write_page_spare
*
* Description:
*   write page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_write_page_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_write_spare
*
* Description:
*   write spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_write_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_read_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI bus select
*   reg             [in]  FLASH status register
*   status          [out] status register value
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_read_status(ql_boot_spi_port_e port, ql_boot_spi_nand_status_reg_e reg, unsigned char *status);

/*****************************************************************
* Function: ql_spi_nand_write_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI bus select
*   reg             [in]  FLASH status register
*   status          [in]  status register value
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_write_status(ql_boot_spi_port_e port, ql_boot_spi_nand_status_reg_e reg, unsigned char status);

/*****************************************************************
* Function: ql_spi_nand_erase_block
*
* Description:
*   erase block(128k):D8H
* 
* Parameters:
*   port            [in]  SPI bus selelct
*   page_addr       [in]  page address
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_erase_block(ql_boot_spi_port_e port, unsigned int page_addr);

/*****************************************************************
* Function: ql_spi_nand_reset
*
* Description:
*   reset flash device:FFH
* 
* Parameters:
*   port            [in]  SPI bus selelct
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_spi_nand_reset(ql_boot_spi_port_e port);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND

/*****************************************************************
* Function: ql_boot_spi6_nand_init
*
* Description:
*   初始化NAND FLASH
* 
* Parameters:
*   nand_config   [in]  SPI配置参数
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_init(ql_boot_spi6_nand_config_s nand_config);

/*****************************************************************
* Function: ql_boot_spi6_nand_read_devid_ex
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI6总线选择,此参数预留,目前没有实际意义
*   mid      [out] manufacturer id
*   mid_type [out] manufacturer id type
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_read_devid_ex(ql_boot_spi_port_e port, unsigned  char *mid,ql_boot_spi_nand_id_type_e mid_type);

/*****************************************************************
* Function: ql_boot_spi6_nand_read_devid
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI6 bus select,this parameter is reserved
*   mid      [out] manufacturer id
*
* Return:ql_boot_errcode_spi_e
*
*****************************************************************/
//ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_read_devid(unsigned  char *mid);
#define ql_boot_spi6_nand_read_devid(port, mid)  ql_boot_spi6_nand_read_devid_ex(port, mid, QL_BOOT_NAND_FLASH_ID_24BIT)

/*****************************************************************
* Function: ql_boot_spi6_nand_read_page_spare
*
* Description:
*   read page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_read_page_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_boot_spi6_nand_write_page_spare
*
* Description:
*   write page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_write_page_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_boot_spi6_nand_write_spare
*
* Description:
*   write spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   data            [out] data read buffer
*   len             [out] data read length
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_write_spare(ql_boot_spi_port_e port, unsigned int page_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_boot_spi6_nand_read_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   reg             [in]  FLASH status register
*   status          [out] status register value
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_read_status(ql_boot_spi_port_e port, ql_boot_spi_nand_status_reg_e reg, unsigned char *status);

/*****************************************************************
* Function: ql_boot_spi6_nand_write_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   reg             [in]  FLASH status register
*   status          [in]  status register value
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_write_status(ql_boot_spi_port_e port, ql_boot_spi_nand_status_reg_e reg, unsigned char status);

/*****************************************************************
* Function: ql_boot_spi6_nand_erase_block
*
* Description:
*   erase block(128k):D8H
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_erase_block(ql_boot_spi_port_e port, unsigned int page_addr);

/*****************************************************************
* Function: ql_boot_spi6_nand_reset
*
* Description:
*   reset flash device:FFH
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*
* Return:ql_boot_errcode_spi6_nand_e
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_spi6_nand_reset(ql_boot_spi_port_e port);
#endif
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                          spi/spi6 nand flash demo 实列
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*===========================================================================
 * Macro Definition
 *===========================================================================*/


typedef enum
{
    QL_BOOT_ECC_OK,
    QL_BOOT_ECC_1BIT,
    QL_BOOT_ECC_2BIT
}ql_boot_nand_ecc_status_e;
    
    
typedef struct
{
    unsigned int data_blocknum;
    unsigned int free_blocknum;
    unsigned int nand_id;
    unsigned int *logical_map_table;
    unsigned char *pbuffer;
}ql_boot_nand_config_s;


typedef struct
{
    unsigned int acture_block_num;
    unsigned short acture_page_num;
    unsigned int cache_block_num;
    unsigned short cache_page_num;
}ql_boot_nand_cache_page_s;

typedef struct
{
	unsigned int page_mainsize;
	unsigned int page_sparesize;
	unsigned int block_pagenum;
	unsigned int plane_blocknum;	//not use
	unsigned int block_totalnum;
	unsigned int cache_blocknum;
	unsigned int cache_total_pagenum;

	unsigned short page_spare_shift;
	unsigned short block_postion_shift;
	unsigned short block_type_shift;
	unsigned short logic_addr_shift; //offset in page
    unsigned short logic_addr_addr;  //addr in page,4byte
    unsigned short page_used_shift;  //offset in page
    unsigned short page_used_addr;   //addr in page,1byte
    unsigned short page_garbage_shift;
    unsigned short page_num_shift;
} ql_boot_nand_spec_s;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
/*****************************************************************
* Function: ql_nand_init
*
* Description: nand flash init
*
* Parameters:
*   NULL
*
* Return:
*   QL_BOOT_SPI_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_nand_init();

/*****************************************************************
* Function: ql_nand_uninit
*
* Description: nand flash uninit
*
* Parameters:
*   NULL
*
* Return:
*   QL_BOOT_SPI_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_nand_uninit();

/*****************************************************************
* Function: ql_nand_read_page
*
* Description: read page
*
* Parameters:
*   block_num      [in]     physical block number 
*   page_num       [in]     page number 
*   column_addr    [in]     column address
*   pbuffer        [out]    data buffer
*   len            [in]     data len
*
* Return:
*   QL_BOOT_SPI_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi_e ql_boot_nand_read_page(unsigned int block_num, unsigned short page_num, unsigned short column_addr, unsigned char *pbuffer, int len);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND
/*****************************************************************
* Function: ql_nand_init_spi6
*
* Description: nand flash init
*
* Parameters:
*   NULL
*
* Return:
*   QL_BOOT_SPI6_NAND_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_nand_init_spi6();

/*****************************************************************
* Function: ql_nand_uninit_spi6
*
* Description: nand flash uninit
*
* Parameters:
*   NULL
*
* Return:
*   QL_BOOT_SPI6_NAND_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_nand_uninit_spi6();

/*****************************************************************
* Function: ql_nand_read_page_spi6
*
* Description: read page
*
* Parameters:
*   block_num      [in]     physical block number 
*   page_num       [in]     page number 
*   column_addr    [in]     column address
*   pbuffer        [out]    data buffer
*   len            [in]     data len
*
* Return:
*   QL_BOOT_SPI6_NAND_SUCCESS     success
*	other 	                error code
*
*****************************************************************/
ql_boot_errcode_spi6_nand_e ql_boot_nand_read_page_spi6(unsigned int block_num, unsigned short page_num, unsigned short column_addr, unsigned char *pbuffer, int len);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI_NAND_FLASH_H */



