/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ql_fs.h"
#include "ss_fs_utils.h"
#include "onomondo/softsim/fs.h"
#include "onomondo/softsim/mem.h"
#include "onomondo/softsim/log.h"

ss_FILE ss_fopen(char *path, char *mode)
{
    QFILE *fd = SS_ALLOC(QFILE);

    if (!fd)
        return NULL;

    SS_LOGP(SSTORAGE, LINFO, "opening: %s\n", path);

    *fd = ql_fopen(path, mode);

    if (*fd < 0)
    {
        SS_LOGP(SSTORAGE, LERROR, "failed to open file: %s, got error: %d\n", path, *fd);
        SS_FREE(fd);
        return NULL;
    }

    return (ss_FILE)fd;
};


int ss_fclose(ss_FILE fp)
{
    if (!fp)
    {
        SS_LOGP(SSTORAGE, LERROR, "failed to close file\n");
        return -1;
    }

    int64 ret = ql_fclose(*((QFILE *)fp));
    SS_FREE(fp);
    return ret;
};


size_t ss_fread(void *ptr, size_t size, size_t nmemb, ss_FILE fp)
{
    QFILE file = *((QFILE *)fp);
    if (!fp || !ptr || !size || !nmemb)
    {
        SS_LOGP(SSTORAGE, LERROR, "failed to read file\n");
        return 0;
    }
    return ql_fread(ptr, size, nmemb, file) / size;
};


size_t ss_fwrite(const void *prt, size_t size, size_t count, ss_FILE f)
{
    QFILE file = *((QFILE *)f);
    if (!f || !prt || !size || !count)
        return 0;

    int bytes = ql_fwrite((void *)prt, size, count, file);
    if (bytes < 0)
        return 0;

    return bytes / size;
};


int ss_file_size(char *path)
{
    QFILE f = ql_fopen(path, "r");
    if (f < 0)
    {
        SS_LOGP(SSTORAGE, LERROR, "failed to open file\n");
        return -1;
    }

    int size = ql_fsize(f);
    ql_fclose(f);
    return size;
};


int ss_delete_file(const char *path)
{
    return ql_remove(path);
};


int ss_delete_dir(const char *path)
{
    // we have faith they wont change the str
    return ql_rmdir_ex((char *)path) == QL_FILE_OK ? 0 : -1;
};


int ss_fseek(ss_FILE fp, long offset, int whence)
{
    int rc = ql_fseek(*(QFILE *)fp, offset, whence);
    if (rc != offset) {
        SS_LOGP(SSTORAGE, LERROR, "seeking file at offset: %ld, got return code: %d\n", offset, rc);
        return -1;
    }
    return 0;
};


int ss_access(const char *path, int amode)
{
    QFILE f = ql_fopen(path, "r");
    if (f < 0)
        return -1;

    ql_fclose(f);
    return 0;
};


int ss_create_dir(const char *path, uint32_t mode)
{
    return ql_mkdir(path, mode) == QL_FILE_OK ? 0 : -1;
};


int ss_dir_exists(const char *path)
{
    int err = QL_FILE_OK;
    err = ql_mkdir(path, 0);
    if (err == QL_DIR_DIR_ALREADY_EXIST)
        return 1;

    return 0;
}

int ss_file_exists(const char *path)
{
    return ql_file_exist(path);
}

int ss_file_rename(const char *oldpath, const char *newpath)
{
    return ql_rename(oldpath, newpath);
}
