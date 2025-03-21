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

#include "ql_api_osi.h"
#include "ql_fs.h"
#include "ql_log.h"
#include "vendor_file.h"
#include "vendor_os.h"


#define QL_ALIPAY_FILE_DIR      "alipay"

/*
 * 打开文件名为filename的文件，要求权限为可读写；
 * 参数：filename [in] 文件名(字符串)
 * 返回值: 文件句柄: 成功
 *        -1 或者 NULL: 失败
*/
void* alipay_open_rsvd_part(PARAM_IN char filename[128])
{
    QFILE ql_fd = 0;
    char name[128] = {0};
    if (strlen(filename) > 63)
    {
        //不支持大于63字节的文件名称
        return NULL;
    }

    sprintf(name, "%s/%s/%s", "UFS:", QL_ALIPAY_FILE_DIR, filename);

    int ret = ql_mkdir("UFS:/"QL_ALIPAY_FILE_DIR"", 0);
    if ((ret != QL_DIR_DIR_ALREADY_EXIST) && (ret != QL_FILE_OK))
    {
        return (void*)-1;
    }
    
    ql_fd = ql_fopen(name, "ab+");
    if (ql_fd <= 0)
    {
        return (void*)ql_fd;
    }

    ql_fseek(ql_fd, 0, QL_SEEK_SET);
    return (void*)ql_fd;
}

/*
 * 往文件中写数据
 * 参数：fd [in] 文件句柄
 *      data [in] 要写入的数据
 *      data_len [in] 要写入的数据的长度
 * 返回值: 0: 写入成功
 *        -1: 写入失败
*/
int alipay_write_rsvd_part(PARAM_IN void* fd, PARAM_IN void *data,PARAM_IN uint32_t data_len)
{
    QFILE ql_fd = (QFILE)fd;

    int ret = ql_fwrite(data, data_len, 1, ql_fd);
    return ret > 0 ? 0 : -1;
}

/*
 * 从提供给alipay kv存储的文件中读取数据
 * 参数：fd [in] 文件句柄
 *      buffer [out] 存放读取的数据的缓存
 *      read_len [in] 允许读取的最大长度
 *      read_len [out] 实际读取的数据长度
 * 返回值: 0: 读取成功
 *        -1: 读取失败,或者长度为0
*/
int alipay_read_rsvd_part(PARAM_IN void* fd,PARAM_OUT void *buffer,PARAM_INOUT uint32_t* read_len)
{
    QFILE ql_fd = (QFILE)fd;

    int ret = ql_fread(buffer, *read_len, 1, ql_fd);
    if (ret == QL_FILE_READ_ZERO)
    {
        return -1;
    }
    else if (ret > 0)
    {
        *read_len = ret;
        return 0;
    }
    else 
    {
        return -1;
    }
}

/*
 * 关闭打开的文件
 * 参数：fd [in] 文件句柄
 * 返回值: 0: 关闭成功
 *        -1: 关闭失败
*/
int alipay_close_rsvd_part(PARAM_IN void* fd)
{
    QFILE ql_fd = (QFILE)fd;
    int ret = ql_fclose(ql_fd);
    return ret == 0 ? 0 : -1;
}

/*
 * 查看文件是否存在
 * 参数: filename [in] 文件名(字符串)
 * 返回值: 1: 文件存在
          0: 文件不存在
 */
int alipay_access_rsvd_part(PARAM_IN char filename[128])
{
    char name[128] = {0};
    if (strlen(filename) > 63)
    {
        //不支持大于63字节的文件名称
        return 0;
    }

    sprintf(name, "%s/%s/%s", "UFS:", QL_ALIPAY_FILE_DIR, filename);
    int ret = ql_file_exist(name);
    return ret == 0 ? 1 : 0;
}

/*
 * 删除文件
 * 参数: filename [in] 文件名(字符串)
 * 返回值: 0: 删除成功
 *       -1: 删除失败
 */
int alipay_remove_rsvd_part(PARAM_IN char filename[128])
{  
    char name[128] = {0};
    if (strlen(filename) > 63)
    {
       //不支持大于63字节的文件名称
       return -1;
    }

    sprintf(name, "%s/%s/%s", "UFS:", QL_ALIPAY_FILE_DIR, filename);
    int ret = ql_remove(name);
    return ret == 0 ? 0 : -1;
}

/*
 * 清除所有通过alipay_write_rsvd_part存储的alipay文件
 * 返回值: 0: 清除成功
 *       -1: 清除失败
 */
int alipay_clear_rsvd_part(void)
{
    int ret = ql_rmdir_ex("UFS:/"QL_ALIPAY_FILE_DIR"");
    return ret == 0 ? 0 : -1;
}




