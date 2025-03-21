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

#include "ql_cnktd_fs.h"

#include "ql_log.h"
#include "ql_embed_nor_flash.h"

#define QL_CNKTD_FS_LOG_LEVEL                   QL_LOG_LEVEL_INFO
#define QL_CNKTD_FS_LOG(msg, ...)               QL_LOG(QL_CNKTD_FS_LOG_LEVEL, "ql_cnktd", msg, ##__VA_ARGS__)
#define QL_CNKTD_FS_LOG_PUSH(msg, ...)          QL_LOG_PUSH("ql_cnktd", msg, ##__VA_ARGS__)


/*API to get file statistical information */
qapi_FS_Status_t qapi_FS_Stat(const char* path,
                              struct qapi_FS_Stat_Type_s* sbuf)
{ 
    int ret  = 0;
    struct stat st = {0};

    if (sbuf == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ret = ql_stat(path, &st);
    if (ret < 0)
    {
        return QAPI_ERROR;
    }

    sbuf->st_dev = st.st_dev;
    sbuf->st_ino = st.st_ino;
    sbuf->st_Mode = st.st_mode;
    sbuf->st_nlink = st.st_nlink;
    sbuf->st_uid = st.st_uid;
    sbuf->st_rdev = st.st_rdev;
    sbuf->st_size = st.st_size;
    sbuf->st_atime = st.st_atime;
    sbuf->st_mtime = st.st_mtime;
    sbuf->st_ctime = st.st_ctime;
    sbuf->st_blksize = st.st_blksize;
    sbuf->st_blocks = st.st_blocks;
    return QAPI_OK;
}

                              /* API for file seek upto (4GB-1) bytes */
qapi_FS_Status_t qapi_FS_Seek(int fd, 
                              qapi_FS_Offset_t offset, 
                              int whence,
                              qapi_FS_Offset_t *actual_offset_ptr)                   
{
    int len = 0;
    if(actual_offset_ptr==NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    len = ql_fseek(fd, offset, whence);
    if (len < 0)
    {
        return QAPI_ERROR;
    }

    *actual_offset_ptr = len;
    return QAPI_OK;
}

                              /* API for file read */
qapi_FS_Status_t qapi_FS_Read(int fd, 
                              uint8* buf, 
                              uint32 count,
                              uint32* bytes_read_ptr)
{
    int len = 0;
    if(bytes_read_ptr==NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    len = ql_fread(buf, count, 1, fd);
    if (len < 0)
    {
        return QAPI_ERROR;
    }

    *bytes_read_ptr = len;
    return QAPI_OK;
}

                              /* API for making directory */
qapi_FS_Status_t qapi_FS_Mk_Dir(const char* path, qapi_FS_Mode_t mode)
{
    int ret = ql_mkdir(path, mode);
    if (ret == QL_DIR_DIR_ALREADY_EXIST)
    {
        return QAPI_OK;
    }
    else if (QL_FILE_OK != ret)
    {
        return QAPI_ERROR;
    }
    
    return QAPI_OK;
}

/*
@func
  qapi_QT_DAM_CFG_Partition_Read.
@brief
  Read data from DAM CFG partition.
@param[in]
  data :Need to be read from the flash buffer. 
@param[in]
  len :Length of data read. 
@param[out]
  len :Actual length of data read.  
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_CFG_Partition_Read(void *data, int len)
{
	return QAPI_QT_ERR_NORMAL_FAIL;
}

/*
@func
  qapi_QT_DAM_CFG_Partition_Write.
@brief
  Write data to DAM CFG partition.
@param[in]
  data :Need to be written to the flash buffer. 
@param[in]
  len :Length of data written. 
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_CFG_Partition_Write(void *data, int len)
{
	return QAPI_QT_ERR_NORMAL_FAIL;
}

/* API for file opening */
qapi_FS_Status_t qapi_FS_Open(const char* path, 
                              int oflag,
                              int* fd_ptr)
{
    int fd  = 0;
    if(fd_ptr==NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    fd = ql_open(path, oflag);
    if (fd < 0)
    {
        return QAPI_ERROR;
    }

    *fd_ptr = fd;
    return QAPI_OK;                    
}


/* API for file close */
qapi_FS_Status_t qapi_FS_Close(int fd)
{
    if (ql_fclose(fd) < 0)
    {
        return QAPI_ERROR;
    }
    return QAPI_OK;
}

/* API for file write */
qapi_FS_Status_t qapi_FS_Write(int fd, 
                               const uint8* buf, 
                               uint32 count,
                               uint32* bytes_written_ptr)
{
    int len= 0;
    if(bytes_written_ptr==NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    len = ql_fwrite((void *)buf, count, 1, fd);
    if (len < 0)
    {
        return QAPI_ERROR;
    }

    *bytes_written_ptr = len;
    return QAPI_OK;
}

/* API for file truncate upto (4GB-1) bytes */
qapi_FS_Status_t qapi_FS_Truncate(const char* path, qapi_FS_Offset_t length)                 
{
    int ret = 0;

    ret = ql_truncate(path, length);
    if (ret < 0)
    {
        return QAPI_ERROR;
    }
    
    return QAPI_OK;
}

/* API to delete a file*/
qapi_FS_Status_t qapi_FS_Unlink(const char* path)                   
{
    if (QL_FILE_OK == ql_remove(path))
    {
        return QAPI_OK;
    }
    else 
    {
        return QAPI_ERROR;
    }
}


