/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hal_file.h"
#include "utils_file.h"
#include "ql_ohos_hal_file.h"
#include <sys/unistd.h>

ql_ohos_file_t  *g_ql_ohos_file_handle = NULL;

int HalFileOpen(const char* path, int oflag, int mode)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_open == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_open(path, oflag, mode);
}

int HalFileClose(int fd)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_close == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_close(fd);
}

int HalFileRead(int fd, char *buf, unsigned int len)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_read == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_read(fd, buf, len);
}

int HalFileWrite(int fd, const char *buf, unsigned int len)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_write == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_write(fd, buf, len);
}

int HalFileDelete(const char *path)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_delete == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_delete(path);
}

int HalFileStat(const char *path, unsigned int *fileSize)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_stat == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_stat(path, fileSize);
}

int HalFileSeek(int fd, int offset, unsigned int whence)
{
    if(g_ql_ohos_file_handle == NULL || g_ql_ohos_file_handle->file_seek == NULL)
    {
        return -1;
    }

    return g_ql_ohos_file_handle->file_seek(fd, offset, whence);
}

int ql_ohos_file_api_register(ql_ohos_file_t *file)
{
    if(file == NULL)
    {
        return -1;
    }

    g_ql_ohos_file_handle = file;

    return 0;
}

int ql_ohos_file_api_unregister(void)
{
    g_ql_ohos_file_handle = NULL;

    return 0;
}