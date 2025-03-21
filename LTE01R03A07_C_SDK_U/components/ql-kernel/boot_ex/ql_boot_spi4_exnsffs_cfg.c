/**  
  @file
  ql_boot_spi4_exnsffs_cfg.c

  @brief
  quectel boot spi4 ext nor flash init param configuration .

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

#include "quec_boot_spi.h"
#include "osi_log.h"
#include "drv_names.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
//the SPI port currently used in boot,it can be configured in quec_boot_pin_cfg.h
extern uint8_t ql_boot_cur_spi4_port;
extern void quec_boot_spi4_extnsffs_set_port(uint8_t spi_port);

/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_boot_spi4_nor_flash_init(void)
{
	//Initialize pin to SPI function
	quec_boot_spi_pin_init(ql_boot_cur_spi4_port);    

	//Configure nor Flash initialization parameters
	ql_boot_spi_flash_config_s flash_config = {0};
	flash_config.port = ql_boot_cur_spi4_port;
	flash_config.spiclk = QL_BOOT_SPI_CLK_25MHZ;
	flash_config.input_sel = QL_BOOT_SPI_DI_1;
	flash_config.transmode = QL_BOOT_SPI_DIRECT_POLLING;//BOOT中不支持DMA模式
	flash_config.cs = QL_BOOT_SPI_CS0;
	//使用GPIO的方式来控制SPI CS引脚。注意，CS引脚需要配置为GPIO功能
	//flash_config.cs = QL_BOOT_SPI_GPIO;
    //flash_config.cs_gpio = 10;
	ql_boot_spi_flash_init_ext(flash_config);

	//Sets the SPI port currently used by NOR Flash
	quec_boot_spi4_extnsffs_set_port(ql_boot_cur_spi4_port);
	
	//OSI_LOGI(0, "ql_boot_spi4_nor_flash_init");
}


