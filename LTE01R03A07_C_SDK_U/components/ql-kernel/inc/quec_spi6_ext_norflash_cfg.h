
/**  @file
  quec_spi6_ext_norflash_cfg.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QUEC_SPI6_EXT_NORFLASH_CFG_H
#define QUEC_SPI6_EXT_NORFLASH_CFG_H

#include "quec_proj_config.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
#include "hal_config.h"
#include <stdint.h>
#include <stdbool.h>
#include "osi_compiler.h"
#include <sys/queue.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define MOUNT_ONLY (0)
#define MOUNT_FORMAT_ON_FAIL (1)
#define MOUNT_FORCE_FORMAT (2)

#define HEADER_MAGIC OSI_MAKE_TAG('P', 'R', 'T', 'I')
#define TYPE_FBDEV2 OSI_MAKE_TAG('F', 'B', 'D', '2')
#define TYPE_SUBPART OSI_MAKE_TAG('B', 'P', 'R', 'T')
#define TYPE_SFFS OSI_MAKE_TAG('S', 'F', 'F', 'S')

#define NAME_FEXT OSI_MAKE_TAG('F', 'E', 'X', 'T')

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/


/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint32_t magic;
    uint32_t crc;
    uint32_t size;
    uint32_t version;
} descHeader_t;

typedef struct
{
    uint32_t type;
    uint32_t flash;
    uint32_t name;
    uint32_t offset;
    uint32_t size;
    uint32_t eb_size;
    uint32_t pb_size;
    uint32_t flags;
} descFbdev_t;

typedef struct
{
    uint32_t type;
    uint32_t device;
    uint32_t name;
    uint32_t offset;
    uint32_t count;
    uint32_t flags;
} descSubpart_t;

typedef struct
{
    uint32_t type;
    uint32_t device;
    char mount[64];
    uint32_t reserve_block;
    uint32_t flags;
} descSffs_t;

/*===========================================================================
 * Extern
 ===========================================================================*/
//
int quec_spi6_ext_nor_flash_capacity;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void ql_set_spi6_ext_nor_flash_status(bool onff);
bool ql_get_spi6_ext_nor_status(void);
void quec_set_spi6_ext_nor_flash_capacity(void);
int quec_spi6_ext_nor_flash_mount(bool format_on_fail);
int quec_spi6_ext_nor_flash_unmount(void);
void quec_boot_get_spi6_ext_norflash_gpio_default_fuction(void);
void quec_boot_spi6_ext_norflash_gpio_deinit(void);
void quec_boot_spi6_ext_norflash_gpio_init(void);
int quec_spi6_ext_nor_flash_mkfs(void);
void quec_boot_spi6_ext_norflash_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif/* CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR */
#endif /* QUEC_SPI6_EXT_NOR_FLASH_CFG_H */


