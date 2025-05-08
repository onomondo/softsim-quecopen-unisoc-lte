/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ql_fs.h"
#include "ss_bootstrap.h"
#include "ss_fs_utils.h"
#include "ss_static_files_hex.h"
#include "onomondo/softsim/log.h"
#include "onomondo/softsim/fs.h"
#include "onomondo/utils/ss_profile.h"

#define TOP_LEVEL_PATH SS_FS_STORAGE_PATH "/3f00.def"
#define ICCID_PATH SS_FS_STORAGE_PATH "/3f00/2fe2"
#define IMSI_PATH SS_FS_STORAGE_PATH "/3f00/7ff0/6f07"

int softsim_dir_is_valid()
{
    if (!ss_dir_exists(SS_FS_STORAGE_PATH))
        return 0;

    if (!ss_dir_exists(SS_FS_STORAGE_PATH "/3f00"))
        return 0;

    ss_FILE f = ss_fopen(TOP_LEVEL_PATH, "r");
    if (f == NULL)
        return 0;
    else
        ss_fclose(f);

    f = ss_fopen(ICCID_PATH, "r");
    if (f == NULL)
        return 0;
    else
        ss_fclose(f);

    f = ss_fopen(IMSI_PATH, "r");
    if (f == NULL)
        return 0;
    else
        ss_fclose(f);

    return 1;
}


int recreate_fs()
{
    SS_LOGP(SSTORAGE, LINFO, "Recreating SoftSIM filesystem\n");

    int ret = ss_create_dir(SS_FS_STORAGE_PATH, 0);
    if(ret == QL_DIR_DIR_ALREADY_EXIST)
        SS_LOGP(SSTORAGE, LERROR, "dir exist, not create: %s\n", SS_FS_STORAGE_PATH);
    else if(ret != QL_FILE_OK)
        SS_LOGP(SSTORAGE, LERROR, "Failed to create storage path: %s\n", SS_FS_STORAGE_PATH);

    for (int i = 0; i < ss_dirs_len-1; i++)
    {
        ret = ss_create_dir(ss_dirs[i].name, 0);

        if(ret == QL_DIR_DIR_ALREADY_EXIST)
            SS_LOGP(SSTORAGE, LERROR, "dir exist, not create: %s\n", SS_FS_STORAGE_PATH);
        else if(ret != 0)
            SS_LOGP(SSTORAGE, LERROR, "Failed to create storage path: %s\n", ss_dirs[i].name);
    }

    for (int i = 0; i < ss_files_len; i++)
    {
        ss_FILE f = ss_fopen((char *)ss_files[i].name, "w");
        if (f == NULL)
        {
            SS_LOGP(SSTORAGE, LERROR, "Failed to create file: %s\n", ss_files[i].name);
            continue;
        }

        size_t written = ss_fwrite(ss_files[i].data, 1, ss_files[i].size, f);
        if (written != ss_files[i].size)
            SS_LOGP(SSTORAGE, LERROR, "Failed to write to file: %s\n", ss_files[i].name);

        ss_fclose(f);
    }

    SS_LOGP(SSTORAGE, LINFO, "Successfully recreated SoftSIM filesystem\n");
    return 0;
}
