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

#ifndef _QUEC_BOOT_VFS_H_
#define _QUEC_BOOT_VFS_H_

#include "osi_compiler.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "quec_boot_pff.h"
#include "quec_proj_config.h"

#define QUEC_BOOT_VFS_ECHO  0

#define QUEC_SD_FD  (4200)
#define	_FCREAT		0x0200	/* open with file create */

typedef enum {
    QUEC_BOOT_VFS_INIT  = 0,
    QUEC_BOOT_VFS_OPEN  = 1,
    QUEC_BOOT_VFS_CLOSE = 2,
} quec_boot_vfs_status_e;

OSI_EXTERN_C_BEGIN

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY
int quec_bootVfsOpen(const char *path,int flags);
int quec_bootVfsRead(int fd, void *data, size_t size);
bool quec_bootVfsClose(int fd);
int quec_bootVfsFileSize(int fd,int *size);
bool quec_bootVfsCheckPath(const char *path);
#endif 

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int quec_bootVfsOpen_ex(const char *path,int flags);
int quec_bootVfsRead_ex(int fd, void *data, size_t size);
bool quec_bootVfsClose_ex(int fd);
int quec_bootVfsFileSize_ex(int fd,int *size);
int quec_bootVfsWrite_ex(int fd, const void *data, size_t size);
int quec_bootVfsSfileWrite_ex(const char *path, const void *data, size_t size);
int quec_bootVfsOpen_ex(const char *path,int flags);
int quec_bootVfsRead_ex(int fd, void *data, size_t size);
bool quec_bootVfsClose_ex(int fd);

#endif


OSI_EXTERN_C_END
#endif
