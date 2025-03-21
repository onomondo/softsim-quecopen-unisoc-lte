#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_log.h"
#include "quec_boot_log.h"
#include "ql_boot_spi_nand_flash.h"


#define QL_BOOT_NAND_F_QE       (1<<0)                      //QE bit。4线SPI，需要清除这个bit
#define QL_BOOT_NAND_F_BUF      (1<<3)                      //BUF bit。有些型号，如w25n01gw需要设置BUF BIT
#define QL_BOOT_NAND_F_ECC      (1<<4)                      //ECC bit。默认设置，客户不能修改

extern ql_boot_spi_port_e ql_boot_spi_port;     //由quec_boot_nand_init函数参数传入
extern ql_boot_errcode_pin_e quec_boot_spi_pin_init(ql_boot_spi_port_e port);

ql_boot_errcode_spi_e ql_boot_nand_spi_init()
{ 
    ql_boot_errcode_spi_e ret;
    quec_boot_spi_pin_init(ql_boot_spi_port);
    QUEC_BOOT_LOG("spi port=%d", ql_boot_spi_port);
#if 0
    ret = ql_spi_nand_init(QL_CUR_SPI_PORT, QL_SPI_CLK_25MHZ);
#else
    ql_boot_spi_nand_config_s spi_nand_config = {0};
    spi_nand_config.port = ql_boot_spi_port;
    spi_nand_config.spiclk = QL_BOOT_SPI_CLK_25MHZ;
    spi_nand_config.input_sel = QL_BOOT_SPI_DI_1;
    spi_nand_config.transmode = QL_BOOT_SPI_DIRECT_POLLING;
    spi_nand_config.cs = QL_BOOT_SPI_CS0;
    ret = ql_boot_spi_nand_init_ext(spi_nand_config);
#endif
    if (ret != QL_BOOT_SPI_SUCCESS)
    {
        QUEC_BOOT_LOG("init err");
        ql_boot_spi_release(ql_boot_spi_port);
        return ret;
    }

#if 1
    //根据不同的型号的nand flash进行配置。ql_boot_spi6_nand_init里默认是配置了QL_BOOT_NAND_F_ECC和QL_BOOT_NAND_F_QE
    char status = QL_BOOT_NAND_F_BUF | QL_BOOT_NAND_F_ECC;
    ret = ql_boot_spi_nand_write_status(spi_nand_config.port, QL_BOOT_NAND_FLASH_STATUS_2, status);
    if (ret != QL_BOOT_SPI_SUCCESS)
    {
        QUEC_BOOT_LOG("init err");
        return ret;  
    }
#endif
    
    return QL_BOOT_SPI_SUCCESS;
}

