
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

=================================================================*/


#ifndef QL_ZIP_H
#define QL_ZIP_H

#include "ql_api_common.h"
#include "ql_uart.h"
#include "time.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================
 * Macro Definition
 ===========================================================================*/
 typedef enum
{
	QL_FILE_ZIP_OK = 0,
    QL_FILE_ZIP_INVALID_FILE_NAME      = (QL_COMPONENT_STORAGE_ZIP << 16) | 400,
    QL_FILE_ZIP_FILE_NOT_FOUND         = (QL_COMPONENT_STORAGE_ZIP << 16) | 401,
    QL_FILE_ZIP_DEL_ERROR              = (QL_COMPONENT_STORAGE_ZIP << 16) | 402,
    QL_FILE_ZIP_UNZIP_ERROR            = (QL_COMPONENT_STORAGE_ZIP << 16) | 403,
    QL_FILE_ZIP_CREATE_ZIP_ERROR       = (QL_COMPONENT_STORAGE_ZIP << 16) | 404,
    QL_FILE_ZIP_ADD_FILE_ZIP_ERROR     = (QL_COMPONENT_STORAGE_ZIP << 16) | 405,
    QL_FILE_ZIP_ADD_FOLDER_ZIP_ERROR   = (QL_COMPONENT_STORAGE_ZIP << 16) | 406,
    QL_FILE_ZIP_CLOSE_ERROR            = (QL_COMPONENT_STORAGE_ZIP << 16) | 407,
    QL_FILE_ZIP_UNZIP_NOSPACE          = (QL_COMPONENT_STORAGE_ZIP << 16) | 408,
    QL_FILE_ZIP_PARAM_ERR              = (QL_COMPONENT_STORAGE_ZIP << 16) | 409,
    QL_FILE_ZIP_UNZIP_ITEM_ERR         = (QL_COMPONENT_STORAGE_ZIP << 16) | 410,
 }ql_zip_errcode_e;
/*===========================================================================
 * Typedef
 ===========================================================================*/
typedef unsigned long DWORD;
typedef DWORD ZRESULT;
typedef struct HZIP 
{ 
   int unused; 
}*HZIP; 

typedef enum
{
	DELETE_COMPRESS_ENABLE,
	DELETE_COMPRESS_DISABLE
}ql_unzip_del_type_e;

typedef struct 
{ 
   ql_unzip_del_type_e delete_compress; 
}ql_zip_option_t; 

#define QL_UNZIP_ITEM_SIZE          32
#define QL_UNZIP_FILE_PATH_LEN      128
typedef enum
{
    QL_UNZIP_FILE = 0,                          //当前的item是文件
    QL_UNZIP_DIR,                               //当前的item是目录
}ql_unzip_file_type_e;
typedef struct
{
    void *item_data;                            //实际指向客户传入的内存地址
    int item_len;                               //当前item的长度
    char file_path[QL_UNZIP_FILE_PATH_LEN];     //当前文件的文件路径名称
    ql_unzip_file_type_e file_type;             //item文件类型
}ql_unzip_item_data_s;

typedef enum
{
    QL_UNZIP_TO_FILE = 0,                       //解压到文件系统里
    QL_UNZIP_TO_MEMERY,                         //解压到内存里
    QL_UNZIP_TO_MEMERY_EX,                      //解压到内存，item数量由客户端决定
}ql_unzip_type_e;

//当ql_unzip_type_e为1时，使用该结构体，用来存储unzip过程中，解压数据到内存里，item数量最大32，超过则解压报错
typedef struct
{  
    void *data;                                 //输入：使用解码到内存时，内存地址
    int total_len;                              //输入：内存总大小
    int used_len;                               //输出：当前内存已使用的长度
    ql_unzip_item_data_s item[QL_UNZIP_ITEM_SIZE]; //输出：保存的item信息
    int item_num;                               //输出：实际item个数
}ql_unzip_mem_data_s;

//当ql_unzip_type_e为2时，使用该结构体，item由客户侧决定且内存需客户侧申请，item数量不限制
typedef struct
{  
    void *data;                                 //输入：使用解码到内存时，内存地址
    int total_len;                              //输入：内存总大小
    int used_len;                               //输出：当前内存已使用的长度
    ql_unzip_item_data_s *item;                 //输出：保存的item信息，需用户侧申请内存空间
    int item_num;                               //输入：需要解压的item数，为0时默认全部解压。输出：实际item个数
}ql_unzip_mem_data_ex_s;

typedef struct
{  
    char *path_name;                //解压文件存放目录，默认解压到同级目录.
    int mode;                       //0不覆盖 1 覆盖.
}ql_unzip_file_data_s;

typedef enum 
{
    QL_FILE_ZIP_DELETE_COMPRESS_FILE=0,//设置解压完成后是否删除压缩文件
}QL_ZIP_OPT_E;

typedef struct
{  
    int item_num;                               //压缩文件总条目数量，包括压缩文件本身，文件夹等
}ql_zip_info_s;
/*===========================================================================
 * Functions declaration
 ===========================================================================*/
 
/*****************************************************************
* Function: ql_fs_unzip
*
* Description: 对文件系统中的压缩文件进行解压
* 
* Parameters:
*   unZipPath         [in]    待解压的文件名.
*   path_name         [in]    解压文件存放目录，默认解压到同级目录.
*   mode              [in]    解压后有同名文件时是否覆盖
                              0不覆盖 1 覆盖.
*
* Return:
*   0           解压成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_unzip(const char *unZipPath,char *path_name,int mode);

/*****************************************************************
* Function: ql_fs_unzip_ex
*
* Description: 对文件系统中的压缩文件进行解压
* 
* Parameters:
*   unZipPath         [in]    待解压的文件名.
*   unzip_type        [in]    为QL_UNZIP_TO_FILE时，unzip_data为ql_unzip_file_data_s结构体指针。为QL_UNZIP_TO_MEMERY，unzip_data为ql_unzip_mem_data_s结构体指针
*   unzip_data        [in]    根据unzip_type的类型，传入不同的参数
*
* Return:
*   0           解压成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_unzip_ex(const char *unZipPath,ql_unzip_type_e unzip_type,void *unzip_data);

/*****************************************************************
* Function: ql_fs_zip_create
*
* Description:
* 	创建一个空的zip。
* 
* Parameters:
* 	fn	           [in] 	某路径下压缩文件名，无路径默认当前工作目录。 
*   password       [in]     password默认无效,无需关注。 
*   errcode        [in]     错误码。
*
* Return: 
*   HZIP:zip的文件指针。
*
*****************************************************************/
HZIP ql_fs_zip_create(const char *unZipPath,const char *password,int *errcode);

/*****************************************************************
* Function: ql_fs_zip_add_file
*
* Description:
* 	将一个文件添加进行压缩。
* 
* Parameters:
* 	hz	          [in] 	   创建或者打开zip的文件指针。 
*   dstzn         [in]     在zip文件中显示的文件名。
*   fn            [in]     想要压缩的文件名。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_add_file(HZIP hz,const TCHAR *dstzn, const TCHAR *fn);

/*****************************************************************
* Function: ql_fs_zip_add_folder
*
* Description:
* 	将整个文件夹添加进行压缩。
* 
* Parameters:
* 	hz	          [in] 	   创建或者打开zip的文件指针。 
*   dstzn         [in]     添加压缩的文件夹名。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_add_folder(HZIP hz,const TCHAR *dstzn);

/*****************************************************************
* Function: ql_fs_zip_setopt
*
* Description:
* 	配置压缩功能参数。
* 
* Parameters:
* 	opt_tag	        [in] 	 zip中配置的参数处理类型。 
*   opt_val         [in]     配置的参数。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_setopt(QL_ZIP_OPT_E opt_tag, void *opt_val);

/*****************************************************************
* Function: ql_fs_zip_get_info
*
* Description:
* 	获取压缩文件信息。
* 
* Parameters:
* 	unZipPath	     [in] 	 压缩文件路径名。 
*   zip_info         [out]   压缩文件信息。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_get_info(const char *unZipPath, ql_zip_info_s *zip_info);
/*****************************************************************
* Function: ql_fs_zip_close
*
* Description:
* 	关闭压缩文件。
*   不可重复调用该函数去关闭一个已经成功关闭的hz。
* 
* Parameters:
* 	hz	       [in]   要操作的zip文件指针。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_close(HZIP hz);

/*****************************************************************
* Function: ql_fs_zip_close_ex
*
* Description:
* 	关闭压缩文件。
*   不可重复调用该函数去关闭一个已经成功关闭的hz。
* 
* Parameters:
* 	hz	       [in]   要操作的zip文件指针的指针。
*
* Return:
*   0           成功。
*   other       错误码。
*
*****************************************************************/
int ql_fs_zip_close_ex(HZIP *hz);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_ZIP_H */



