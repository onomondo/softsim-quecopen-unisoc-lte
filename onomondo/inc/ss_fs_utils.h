/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include "onomondo/softsim/storage.h"

/* Platform-specific filesystem definitions for the Quectel/Unisoc target. */
#define SS_PATH_MAX 100
#define SS_FS_DISK "UFS:"
#define SS_FS_BASE_DIR "softsim"
#define SS_FS_STORAGE_PATH SS_FS_DISK "/" SS_FS_BASE_DIR

int ss_dir_exists(const char *path);
int ss_file_exists(const char *path);
int ss_file_rename(const char *oldpath, const char *newpath);
