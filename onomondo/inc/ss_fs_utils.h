/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include "onomondo/softsim/storage.h"

int ss_dir_exists(const char *path);
int ss_file_exists(const char *path);
int ss_file_rename(const char *oldpath, const char *newpath);
