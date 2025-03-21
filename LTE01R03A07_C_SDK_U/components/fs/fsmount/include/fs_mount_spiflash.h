/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _FS_MOUNT_SPIFLASH_H_
#define _FS_MOUNT_SPIFLASH_H_

#include "osi_compiler.h"
#include "quec_proj_config.h"

OSI_EXTERN_C_BEGIN

#include <stdbool.h>

/**
 * \brief mount sdcard
 *
 * \return
 *      - true on succeed else fail
 */
//blockDevice_t *__nwy_vfs_block_device_create(nwy_spi_flash_t *dev)
/**
 * \brief umount sdcard
 */
//static int __nwy_vfs_mount(const char *base_path, blockDevice_t *nwy_bdev)
#if ((defined CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS))
int fsMountGeneralSpiFlash(bool format);
bool quec_spi4_exnsffs_is_mount(void);
void fsUmountGeneralSpiFlash(void);
#else
bool fsMountGeneralSpiFlash(bool format);
#endif
OSI_EXTERN_C_END

#endif
