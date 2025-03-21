/**  @file
  quec_boot_nand.h

  @brief
  This file is used to define boot nand api for different Quectel Project.

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
19/06/2021        Neo         Init version
11/01/2023        Sum         add Quad SPI/Dual SPI nand flash
=================================================================*/

#ifndef QUEC_BOOT_NAND_H
#define QUEC_BOOT_NAND_H

#include "quec_boot_pin_cfg.h"
#include "ql_boot_spi_nand_flash.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/


/*===========================================================================
 * Enum
 ===========================================================================*/


/*===========================================================================
 * Functions declaration
 ===========================================================================*/
//Standard SPI nand flash api
int quec_boot_nand_init(ql_boot_spi_port_e port);
bool ql_boot_nand_flash_is_mount(void);
//Quad SPI/Dual SPI nand flash api
int quec_boot_nand_init_spi6(ql_boot_spi_port_e port,ql_boot_nand_spi_type_e spi_type);
int quec_boot_nand_init_spi6_ex(ql_boot_spi6_nand_config_s config);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_NAND_H */


