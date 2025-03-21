/**  
  @file
  ql_boot_spi6_nand_flash_cfg.c

  @brief
  quectel boot spi6 ext nand flash init param configuration .

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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_log.h"
#include "quec_boot_log.h"
#include "ql_boot_spi_nand_flash.h"
#include "quec_boot_pin_cfg.h"

#define QL_BOOT_NAND_F_QE       (1<<0)                      //QE bit。4线SPI，需要清除这个bit
#define QL_BOOT_NAND_F_BUF      (1<<3)                      //BUF bit。有些型号，如w25n01gw需要设置BUF BIT
#define QL_BOOT_NAND_F_ECC      (1<<4)                      //ECC bit。默认设置，客户不能修改


extern ql_boot_errcode_pin_e quec_boot_spi6_nand_pin_init(ql_boot_spi_port_e port);

ql_boot_spi6_nand_config_s boot_spi6_nand_config = {
    .port = QL_BOOT_SPI_PORT_1,                         //实际用的是quec_boot_nand_init_spi6传入的port
    .clk = QL_BOOT_SPI6_NAND_FLASH_CLK_SRC_200MHZ,
    .clk_div = 5,
    .quad_mode = 1,
    .sample_delay = 2,
    .isslow = 0,
};

ql_boot_errcode_spi6_nand_e ql_boot_nand_spi6_init()
{ 
    ql_boot_errcode_spi6_nand_e ret;
    quec_boot_spi6_nand_pin_init(boot_spi6_nand_config.port);

    ql_boot_spi6_nand_config_s nand_config = {0};
    nand_config.port = boot_spi6_nand_config.port;
    nand_config.clk = boot_spi6_nand_config.clk;
    nand_config.clk_div = boot_spi6_nand_config.clk_div;
    nand_config.quad_mode = boot_spi6_nand_config.quad_mode;
    nand_config.sample_delay = boot_spi6_nand_config.sample_delay;
    nand_config.isslow = boot_spi6_nand_config.isslow;
    ret = ql_boot_spi6_nand_init(nand_config);
    if (ret != QL_BOOT_SPI6_NAND_SUCCESS)
    {
        QUEC_BOOT_LOG("init err");
        return ret;
    }
#if 1
    //根据不同的型号的nand flash进行配置。ql_boot_spi6_nand_init里默认是配置了QL_BOOT_NAND_F_ECC和QL_BOOT_NAND_F_QE
    char status = QL_BOOT_NAND_F_BUF | QL_BOOT_NAND_F_ECC;
    ret = ql_boot_spi6_nand_write_status(nand_config.port, QL_BOOT_NAND_FLASH_STATUS_2, status);
    if (ret != QL_BOOT_SPI6_NAND_SUCCESS)
    {
        QUEC_BOOT_LOG("init err");
        return ret;  
    }
#endif
    return QL_BOOT_SPI6_NAND_SUCCESS;
}


