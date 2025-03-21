
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------
20230328         Jamie.li    Init version
=================================================================*/

#ifndef _QL_OHOS_HAL_FILE_H_
#define _QL_OHOS_HAL_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ql_ohos_file_s
{
    int  open_cnt;
    int  file_max_num;
    int  (*file_open)(const char* path, int oflag, int mode);
    int  (*file_close)(int fd);
    int  (*file_read)(int fd, char *buf, unsigned int len);
    int  (*file_write)(int fd, const char *buf, unsigned int len);
    int  (*file_delete)(const char *path);
    int  (*file_stat)(const char *path, unsigned int *fileSize);
    int  (*file_seek)(int fd, int offset, unsigned int whence);
}ql_ohos_file_t;

int ql_ohos_file_api_register(ql_ohos_file_t *file);

int ql_ohos_file_api_unregister(void);

int ql_ohos_file_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_FILE_H_ */

