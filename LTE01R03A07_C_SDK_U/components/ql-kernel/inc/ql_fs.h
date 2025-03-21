
/**  @file
  ql_fs.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
#ifndef _QL_FS_H_
#define _QL_FS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>    
#include <sys/stat.h>  
#include "ql_api_common.h"

#define QL_SEEK_SET	0	/* Seek from beginning of file.  */
#define QL_SEEK_CUR	1	/* Seek from current position.  */
#define QL_SEEK_END	2	/* Seek from end of file.  */

#define QL_MAX_FAC_NVITEM_COUNT	10  /*max factory nvitem, do not modify*/


#ifndef uint8
typedef unsigned char uint8;
#endif

#ifndef uint16
typedef unsigned short uint16;
#endif 

#ifndef int16
typedef short int16;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#ifndef size_t
typedef unsigned int size_t;
#endif

#ifndef int64
typedef long long int int64;
#endif

typedef int QFILE;          /* file handler, returned by ql_fopen  */

typedef int QL_STATE;

typedef struct
{
    int16 fs_index;  ///< internal fs index
    int16 _reserved; ///< reserved
} QDIR;

typedef struct
{
    int d_ino;            ///< inode number, file system implementation can use it for any purpose
    uint8 d_type;		  ///< type of file
    char d_name[256];     ///< file name
}qdirent;

typedef enum
{
	QL_FILE_OK = 0,
	
    QL_FILE_INVALID_INPUT_VALUE		 = (QL_COMPONENT_STORAGE_FILE << 16) | 400,     /* invalid input value       */
    QL_FILE_LARGER_THAN_FILESIZE	 = (QL_COMPONENT_STORAGE_FILE << 16) | 401,     /* large than file size      */
    QL_FILE_READ_ZERO                = (QL_COMPONENT_STORAGE_FILE << 16) | 402,     /* read zero byte            */
    QL_FILE_DRIVE_FULL               = (QL_COMPONENT_STORAGE_FILE << 16) | 403,     /* drive full                */
    QL_FILE_MOV_ERROR                = (QL_COMPONENT_STORAGE_FILE << 16) | 404,     /* move error                */
    QL_FILE_FILE_NOT_FOUND       	 = (QL_COMPONENT_STORAGE_FILE << 16) | 405,     /* file not found            */
    QL_FILE_INVALID_FILE_NAME        = (QL_COMPONENT_STORAGE_FILE << 16) | 406,     /* invalidate file name      */
    QL_FILE_FILE_ALREADY_EXISTED     = (QL_COMPONENT_STORAGE_FILE << 16) | 407,     /* file already exist        */
    QL_FILE_FAILED_TO_CREATE_FILE    = (QL_COMPONENT_STORAGE_FILE << 16) | 408,     /* failed to create file     */
    QL_FILE_FAILED_TO_WRITE_FILE     = (QL_COMPONENT_STORAGE_FILE << 16) | 409,     /* failed to write file      */
    QL_FILE_FAILED_TO_OPEN_FILE   	 = (QL_COMPONENT_STORAGE_FILE << 16) | 410,     /* failed to open file       */
    QL_FILE_FAILED_TO_READ_FILE      = (QL_COMPONENT_STORAGE_FILE << 16) | 411,     /* failed to read file       */
    QL_FILE_EXCEED_MAX_LENGTH        = (QL_COMPONENT_STORAGE_FILE << 16) | 412,     /* exceed max length         */
    QL_FILE_REACH_MAX_OPENFILE_NUM   = (QL_COMPONENT_STORAGE_FILE << 16) | 413,     /* reach max opened file num */
    QL_FILE_IS_READONLY              = (QL_COMPONENT_STORAGE_FILE << 16) | 414,     /* file is the read-only     */
    QL_FILE_GET_SIZE_FAIL        	 = (QL_COMPONENT_STORAGE_FILE << 16) | 415,     /* get file size failed      */
    QL_FILE_INVALID_FILE_DESCRIPTOR  = (QL_COMPONENT_STORAGE_FILE << 16) | 416,     /* description invalidate    */
    QL_FILE_LIST_FILE_FAIL           = (QL_COMPONENT_STORAGE_FILE << 16) | 417,     /* failed to list file       */
    QL_FILE_DELETE_FILE_FAIL         = (QL_COMPONENT_STORAGE_FILE << 16) | 418,     /* failed to delete file     */
    QL_FILE_GET_DISK_INFO_FAIL       = (QL_COMPONENT_STORAGE_FILE << 16) | 419,     /* failed to get disk info   */
    QL_FILE_NO_SPACE             	 = (QL_COMPONENT_STORAGE_FILE << 16) | 420,     /* disk no space             */
    QL_FILE_TIME_OUT                 = (QL_COMPONENT_STORAGE_FILE << 16) | 421,     /*time out                  */
    QL_FILE_FILE_NOT_FOUND_2         = (QL_COMPONENT_STORAGE_FILE << 16) | 422,     /* file not found            */
    QL_FILE_FILE_TOO_LARGE           = (QL_COMPONENT_STORAGE_FILE << 16) | 423,     /* file too large            */
    QL_FILE_FILE_ALREADY_EXIST       = (QL_COMPONENT_STORAGE_FILE << 16) | 424,     /* file already exist        */
    QL_FILE_INVALID_PARAMETER    	 = (QL_COMPONENT_STORAGE_FILE << 16) | 425,     /* invalidate parameter      */
    QL_FILE_ALREADY_OPERATION        = (QL_COMPONENT_STORAGE_FILE << 16) | 426,     /* file already open         */
    QL_FILE_ERROR_GENERAL			 = (QL_COMPONENT_STORAGE_FILE << 16) | 427,
    QL_FILE_RENAME_ERROR 			 = (QL_COMPONENT_STORAGE_FILE << 16) | 428,
    QL_FILE_QUIT_DATE_MODE 			 = (QL_COMPONENT_STORAGE_FILE << 16) | 429,
	QL_FILE_CLOSE_FAIL			     = (QL_COMPONENT_STORAGE_FILE << 16) | 430, 
	QL_FILE_SEEK_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 431,
	QL_FILE_TELL_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 432,
	QL_FILE_FAILED_TO_GET_STAT		 = (QL_COMPONENT_STORAGE_FILE << 16) | 433,
	QL_FILE_NOT_EXIST				 = (QL_COMPONENT_STORAGE_FILE << 16) | 434,
	QL_RENAME_FAIL					 = (QL_COMPONENT_STORAGE_FILE << 16) | 435,
	QL_FILE_TRUNCATE_FAIL            = (QL_COMPONENT_STORAGE_FILE << 16) | 436,
	QL_FILE_NOT_OPEN				 = (QL_COMPONENT_STORAGE_FILE << 16) | 437,
	QL_FILE_DEVICE_READONLY          = (QL_COMPONENT_STORAGE_FILE << 16) | 438,
	
	QL_FILE_CREATE_EXCEED            = (QL_COMPONENT_STORAGE_FILE << 16) | 450,     /* create too many files     */
	QL_FILE_LOG_NO_SPACE			 = (QL_COMPONENT_STORAGE_FILE << 16) | 451, 	/* log area no space         */

    QUEC_FILE_ZIP_INVALID_FILE_NAME  = (QL_COMPONENT_STORAGE_FILE << 16) | 439,
    QUEC_FILE_ZIP_FILE_NOT_FOUND     = (QL_COMPONENT_STORAGE_FILE << 16) | 440,
    QUEC_FILE_ZIP_DEL_ERROR          = (QL_COMPONENT_STORAGE_FILE << 16) | 441,
    QUEC_FILE_ZIP_UNZIP_ERROR        = (QL_COMPONENT_STORAGE_FILE << 16) | 442,

	QL_DIR_MAKE_FAIL 				 = (QL_COMPONENT_STORAGE_FILE << 16) | 600,
	QL_DIR_OPEN_FAIL   				 = (QL_COMPONENT_STORAGE_FILE << 16) | 601,
	QL_DIR_CLOSE_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 602,	
	QL_DIR_READ_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 603,
	QL_DIR_TELL_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 604,
	QL_DIR_SEEK_FAIL				 = (QL_COMPONENT_STORAGE_FILE << 16) | 605,
	QL_DIR_REMOVE_FAIL			     = (QL_COMPONENT_STORAGE_FILE << 16) | 606,
	QL_DIR_DIR_ALREADY_EXIST		 = (QL_COMPONENT_STORAGE_FILE << 16) | 607,
	QL_DIR_DIR_NOT_EXIST			 = (QL_COMPONENT_STORAGE_FILE << 16) | 608,

	
    QL_FILE_ERROR_MAX,
}ql_file_errcode_e;



/*****************************************************************
* Function: ql_fopen
*
* Description:
*	打开文件。UFS、EFS、EXNSFFS使用SFFS文件系统，在根目录下只能创建234个文件或目录，在目录下，只能创建202个文件或目录
* 
* Parameters:
*	file_name		[in]		文件名
*	mode			[in]		模式
*
* Return:
*	> 0						成功返回文件ID。
*	other					错误码。
*
*****************************************************************/
QFILE ql_fopen(const char *file_name, const char *mode);

/*****************************************************************
* Function: ql_open
*
* Description:
*   请使用ql_fopen代替。Generally not used, try to use fopen instead
*	打开文件。UFS、EFS、EXNSFFS使用SFFS文件系统，在根目录下只能创建234个文件或目录，在目录下，只能创建202个文件或目录
* 
* Parameters:
*	file_name		[in]		文件名
*	mode			[in]		模式
*
* Return:
*	> 0						成功返回文件ID。
*	other					错误码。
*
*****************************************************************/
int ql_open(const char *file_name, int mode);


/*****************************************************************
* Function: ql_fclose
*
* Description:
*	关闭文件。
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_fclose(QFILE fd);


/*****************************************************************
* Function: ql_remove
*
* Description:
*	删除文件。
* 
* Parameters:
*	file_name		[in]		文件名
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_remove(const char *file_name);


/*****************************************************************
* Function: ql_fread
*
* Description:
*	读取文件。
* 
* Parameters:
*	buffer			[out]		读取到的数据
*	size			[in]		要读取块大小
*	num				[in]		要读取块个数
*	fd				[in]		文件ID
*
* Return:
*	> 0						成功读取到的字节数。
*	other					错误码。
*
*****************************************************************/
int ql_fread(void * buffer, size_t size, size_t num, QFILE fd);


/*****************************************************************
* Function: ql_fwrite
*
* Description:
*	写入文件。
* 
* Parameters:
*	buffer			[in]		要写入的数据
*	size			[in]		要写入块大小
*	num				[in]		要写入块个数
*	fd				[in]		文件ID
*
* Return:
*	> 0						成功写入的字节数。
*	other					错误码。
*
*****************************************************************/
int ql_fwrite(void * buffer, size_t size, size_t num, QFILE fd);


/*****************************************************************
* Function: ql_nvm_fwrite
*
* Description:
*	写入配置文件。
* 
* Parameters:
*	config_file_name	[in]		配置文件名
*	buffer				[in]		要写入的数据
*	size				[in]		要写入块大小
*	num					[in]		要写入块个数
*
* Return:
*	> 0						成功写入的字节数。
*	other					错误码。
*
*****************************************************************/
int ql_nvm_fwrite(const char *config_file_name, void *buffer, size_t size, size_t num);


/*****************************************************************
* Function: ql_nvm_fread
*
* Description:
*	读取配置文件。
* 
* Parameters:
*	config_file_name	[in]		配置文件名
*	buffer				[out]		读取到的数据
*	size				[in]		要读取块大小
*	num					[in]		要读取块个数
*
* Return:
*	> 0						成功读取到的字节数。
*	other					错误码。
*
*****************************************************************/
int ql_nvm_fread(const char *config_file_name, void *buffer, size_t size, size_t num);

/*****************************************************************
* Function: ql_cust_nvm_fwrite
*
* Description:
*	写入客户配置文件。
* 
* Parameters:
*	buffer				[in]		要写入的数据,不大于1KB
*	size				[in]		要写入块大小
*	num					[in]		要写入块个数
*
* Return:
*	> 0						成功写入的字节数。
*	other					错误码。
*
*****************************************************************/
int ql_cust_nvm_fwrite(void *buffer, size_t size, size_t num);

/*****************************************************************
* Function: ql_cust_nvm_fread
*
* Description:
*	读取客户配置文件
* 
* Parameters:
*	buffer				[out]		读取到的数据,不大于1KB
*	size				[in]		要读取块大小
*	num					[in]		要读取块个数
*
* Return:
*	> 0						成功读取到的字节数
*	other					错误码
*
*****************************************************************/
int ql_cust_nvm_fread(void *buffer, size_t size, size_t num);


/*****************************************************************
* Function: ql_fac_cust_nvm_fread
*
* Description:
*	读取工厂分区的客户配置文件,与at+qfaccustnvm指令搭配使用
* 
* Parameters:
*	nvitem_id			[in]		配置数据编号，范围[0-(QL_MAX_FAC_NVITEM_COUNT-1)]
*	buffer				[out]		读取到的数据,不大于2000B
*	size				[in]		要读取块大小
*	num					[in]		要读取块个数,size*num不大于2000B
*
* Return:
*	> 0						成功读取到的字节数
*	other					错误码
*
*****************************************************************/
int ql_fac_cust_nvm_fread(uint8_t nvitem_id, void *buffer, size_t size, size_t num);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
/*****************************************************************
* Function: ql_fac_vsim_nvm_fwrite
*
* Description:
*	VSIM配置文件写入工厂分区, 仅用于VSIM应用备份还原, 只能产线使用, 不能擦写
* 
* Parameters:
*	buffer				[in]		写入的数据,不大于16 * 1024B
*	size				[in]		要读取块大小
*	num					[in]		要读取块个数,size*num不大于16 * 1024B
*
* Return:
*	> 0						成功写入的字节数。
*	other					错误码
*
*****************************************************************/
int ql_fac_vsim_nvm_fwrite(void *buffer, size_t size, size_t num);

/*****************************************************************
* Function: ql_fac_vsim_nvm_fread
*
* Description:
*	读取工厂分区的VSIM配置文件, 仅用于VSIM应用备份还原, 只能产线使用, 不能擦写
* 
* Parameters:
*	buffer				[out]		读取到的数据,不大于16 * 1024B
*	size				[in]		要读取块大小
*	num					[in]		要读取块个数,size*num不大于16 * 1024B
*
* Return:
*	> 0						成功读取到的字节数
*	other					错误码
*
*****************************************************************/
int ql_fac_vsim_nvm_fread(void *buffer, size_t size, size_t num);
#endif

/*****************************************************************
* Function: ql_sfs_setkey
*
* Description:
*	设置SFS的秘钥key。
* 
* Parameters:
*	key				[in]		要设置的秘钥
*	len				[in]		要设置的秘钥长度
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_sfs_setkey(void *key, uint8_t len);


/*****************************************************************
* Function: ql_fseek
*
* Description:
*	重定位文件位置指针。
* 
* Parameters:
*	fd				[in]		文件ID
*	offset			[in]		偏移量
*	origin			[in]		偏移起始位置
*
* Return:
*	>= 0					文件位置指针当前位置相对于文件首的偏移字节数。
*	other					错误码。
*
*****************************************************************/
int ql_fseek(QFILE fd, long offset, int origin);


/*****************************************************************
* Function: ql_ftell
*
* Description:
*	获取文件位置指针当前位置相对于文件首的偏移字节数。
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	>= 0					文件位置指针当前位置相对于文件首的偏移字节数。
*	other					错误码。
*
*****************************************************************/
int ql_ftell(QFILE fd);


/*****************************************************************
* Function: ql_fstat
*
* Description:
*	获取文件状态。
* 
* Parameters:
*	fd				[in]		文件ID
*	st				[out]		文件状态
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_fstat(QFILE fd, struct stat *st);


/*****************************************************************
* Function: ql_stat
*
* Description:
*	获取文件状态。
* 
* Parameters:
*	file_name		[in]		文件名
*	st				[out]		文件状态
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_stat(const char *file_name, struct stat *st);


/*****************************************************************
* Function: ql_fsize
*
* Description:
*	获取文件大小。
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	>= 0					文件大小。
*	other					错误码。
*
*****************************************************************/
int ql_fsize(int fd);


/*****************************************************************
* Function: ql_file_exist
*
* Description:
*	判断文件是否存在。
* 
* Parameters:
*	file_name		[in]		文件名
*
* Return:
*	QL_FILE_OK				文件存在。
*	other					错误码。
*
*****************************************************************/
int ql_file_exist(const char *file_name);


/*****************************************************************
* Function: ql_frewind
*
* Description:
*	将文件位置指针移动到文件开始。
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_frewind(QFILE fd);


/*****************************************************************
* Function: ql_ftruncate
*
* Description:
*	更改文件大小。
* 
* Parameters:
*	fd				[in]		文件ID
*	length			[in]		指定的大小
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_ftruncate(QFILE fd, uint length);

/*****************************************************************
* Function: ql_truncate
*
* Description:
*	更改文件大小。
* 
* Parameters:
*   file_name		[in]		文件名
*	length			[in]		指定的大小
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_truncate(const char *file_name, uint length);


/*****************************************************************
* Function: ql_mkdir
*
* Description:
*	创建目录。
* 
* Parameters:
*	path			[in]		路径
*	mode			[in]		模式
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_mkdir(const char *path, uint mode);


/*****************************************************************
* Function: ql_opendir
*
* Description:
*	打开目录句柄。
* 
* Parameters:
*	path			[in]		路径
*
* Return:
*	NULL					失败。
*	other					目录句柄。
*
*****************************************************************/
QDIR *ql_opendir(const char *path);


/*****************************************************************
* Function: ql_closedir
*
* Description:
*	关闭目录句柄。
* 
* Parameters:
*	pdir			[in]		目录句柄
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_closedir(QDIR *pdir);


/*****************************************************************
* Function: ql_readdir
*
* Description:
*	读取目录。
* 
* Parameters:
*	pdir			[in]		目录句柄
*
* Return:
*	NULL					失败。
*	other					目录信息。
*
*****************************************************************/
qdirent * ql_readdir(QDIR *pdir);


/*****************************************************************
* Function: ql_telldir
*
* Description:
*	获取目录流的当前位置。
* 
* Parameters:
*	pdir			[in]		目录句柄
*
* Return:
*	>= 0					目录流的当前位置。
*	other					错误码。
*
*****************************************************************/
int ql_telldir(QDIR *pdir);


/*****************************************************************
* Function: ql_seekdir
*
* Description:
*	设置目录流读取位置。
* 
* Parameters:
*	pdir			[in]		目录句柄
*	offset			[in]		距离目录文件开头的偏移量
*
* Return:
*	>= 0					距离目录文件开头的偏移量。
*	other					错误码。
*
*****************************************************************/
int ql_seekdir(QDIR *pdir, int offset);


/*****************************************************************
* Function: ql_rewinddir
*
* Description:
*	设置目录流目前的读取位置为原来开头的读取位置。
* 
* Parameters:
*	pdir			[in]		目录句柄
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_rewinddir(QDIR *pdir);


/*****************************************************************
* Function: ql_rmdir
*
* Description:
*	删除目录。
* 
* Parameters:
*	path			[in]		目录路径
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_rmdir(const char *path);


/*****************************************************************
* Function: ql_rmdir_ex
*
* Description:
*	删除目录，并删除目录下的子目录和文件。
*   谨慎操作，不要删除根目录。
* 
* Parameters:
*	path			[in]		目录路径
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_rmdir_ex(char *path);


/*****************************************************************
* Function: ql_rename
*
* Description:
*	重命名文件。
* 
* Parameters:
*	oldpath			[in]		原文件名
*	newpath			[in]		新文件名
*
* Return:
*	QL_FILE_OK				成功。
*	other					错误码。
*
*****************************************************************/
int ql_rename(const char *oldpath, const char *newpath);


/*****************************************************************
* Function: ql_fs_fat32_free_size
*
* Description:
*	使用轮询FAT表的方式进行FAT32文件系统的SD卡进行剩余内存查询，并将计算出的剩余内存写入FsInfo扇区
* 	此方法会长时间阻塞系统
*
* Parameters:
*	path			[in]		(文件)路径
*
* Return:
*	>= 0					空闲空间大小。
*	other					错误码。
*
*****************************************************************/
int64 ql_fs_fat32_free_size(const char *path);


/*****************************************************************
* Function: ql_fs_free_size
*
* Description:
*	查询空闲空间。
* 
* Parameters:
*	path			[in]		(文件)路径
*
* Return:
*	>= 0					空闲空间大小。
*	other					错误码。
*
*****************************************************************/
int64 ql_fs_free_size(const char *path);

/*****************************************************************
* Function: ql_fs_total_size
*
* Description:
*	查询路径分区总空间。
* 
* Parameters:
*	path			[in]		(文件)路径
*
* Return:
*	>= 0					空闲空间大小。
*	other					错误码。
*
*****************************************************************/
int64 ql_fs_total_size(const char *path);


/*****************************************************************
* Function: ql_fs_free_size_by_fd
*
* Description:
*	查询空闲空间。
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	>= 0					空闲空间大小。
*	other					错误码。
*
*****************************************************************/
int64 ql_fs_free_size_by_fd(int fd);


/*****************************************************************
* Function: ql_fputc
*
* Description:
*	将字符chr写到文件的当前写指针的位置。当正确写入一个字符或一个
	字节的数据后，文件内部写指针会自动后移一个字节的位置
* 
* Parameters:
*	fd				[in]		文件ID
*   chr				[in]        带写入的字符
*
* Return:
*	>= 0					写入字符的ASCII值。
*	-1					    读取失败。
*
*****************************************************************/
int ql_fputc(int chr, int fd);

/*****************************************************************
* Function: ql_fputs
*
* Description:
*	将字符串chr写到文件的当前写指针的位置。当正确写入一个字符或一个
	字节的数据后，文件内部写指针会自动后移一个字节的位置
* 
* Parameters:
*	fd				[in]		文件ID
*   chr				[in]        带写入的字符串
*
* Return:
*	>= 0					成功写入的字节数
*	other					错误码。
*
*****************************************************************/
int ql_fputs(const char * chr, int fd);

/*****************************************************************
* Function: ql_fgetc
*
* Description:
*	从文件中读取一个字符。读取一个字节后，光标位置后移一个字节
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	>= 0					所读取字符的ASCII值。
*	-1					    读取失败。
*
*****************************************************************/
int ql_fgetc(int fd);

/*****************************************************************
* Function: ql_fgets
*
* Description:
*	从文件中读取字符串。读取后，光标位置后移一个字节
* 
* Parameters:
*   str             [out]       读到的字符串
*   n               [in]        读取长度
*   fd				[in]        文件ID
* Return:
*	str                     所读到的字符串
*	other					NULL。
*
*****************************************************************/
char *ql_fgets(char *str, int n, int fd);

/*****************************************************************
* Function: ql_ferror
*
* Description:
*	判断当前fd是否有效
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	= 0					fd有效
*	other				错误码。
*
*****************************************************************/
int ql_ferror(int fd);

/*****************************************************************
* Function: ql_fsync
*
* Description:
*	同步内存中已修改的文件数据到储存设备
* 
* Parameters:
*	fd				[in]		文件ID
*
* Return:
*	= 0					同步成功
*	other				错误码。
*
*****************************************************************/

int ql_fsync(int fd);

/*****************************************************************
* Function: ql_fscanf
*
* Description:
*	从文件流读取格式化输入(长度限制1024)
* 
* Parameters:
*	fd				[in]		文件ID
*   format          [in]        格式化字符串
* Return:
*	>0				    成功（格式化参数个数）
*	-1  				失败。
*
*****************************************************************/

int ql_fscanf(int fd, const char *format, ...);

/*****************************************************************
* Function: ql_fprintf
*
* Description:
*   根据格式将字符串写入文件(长度限制2048)
* 
* Parameters:
*   fd              [in]        文件ID
*   format          [in]        格式化字符串
* Return:
*                   Real write size.
*
*****************************************************************/
int ql_fprintf(int fd, const char *format, ...);

#ifdef __cplusplus
} /*"C" */
#endif

#endif
