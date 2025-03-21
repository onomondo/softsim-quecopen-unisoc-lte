/**  
  @file
  ql_api_fota.h

  @brief
  quectel fota interface header definition.

*/
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


#ifndef QL_API_FOTA_H
#define QL_API_FOTA_H

#include "quec_common.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QL_FOTA_ERRCODE_BASE (QL_COMPONENT_FOTA<<16)
#define QL_FOTA_PACK_NAME_MAX_LEN         (172)
/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
//fota component detail errcode, 4 bytes
typedef enum
{
    QL_FOTA_SUCCESS                       = 0,
    QL_FOTA_EXECUTE_ERR                   = 502|QL_FOTA_ERRCODE_BASE,//异常操作
#ifdef __QUEC_OEM_VER_LT__
    QL_FOTA_IMAGE_VERIFY_READY_ERR        = 507|QL_FOTA_ERRCODE_BASE,//fota更新包校验设置准备状态失败
    QL_FOTA_IMAGE_VERIFY_ERR              = 507|QL_FOTA_ERRCODE_BASE,//fota更新包校验不设置准备状态失败
#else
	QL_FOTA_IMAGE_VERIFY_READY_ERR        = 510|QL_FOTA_ERRCODE_BASE,//fota更新包校验设置准备状态失败
    QL_FOTA_IMAGE_VERIFY_ERR              = 547|QL_FOTA_ERRCODE_BASE,//fota更新包校验不设置准备状态失败
#endif

    QL_FOTA_FLAG_SET_ERR                  = 548|QL_FOTA_ERRCODE_BASE,//设置FOTA标志失败
    QL_FOTA_BUSY_ERR                      = 549|QL_FOTA_ERRCODE_BASE,//设置FOTA总线繁忙
    QL_FOTA_POINT_NULL_ERR                = 550|QL_FOTA_ERRCODE_BASE,//操作空指针错误。
    QL_FOTA_PARAM_INVALID                 = 551|QL_FOTA_ERRCODE_BASE,//输入参数无效
    QL_FOTA_PACKPATH_INVALID              = 552|QL_FOTA_ERRCODE_BASE,//输入的文件路径是无效的错误。
    QL_FOTA_ADAPTION_RESET_ERR            = 553|QL_FOTA_ERRCODE_BASE,//重置fota  更新包的路径信息，使其恢复为默认值失败。
    QL_FOTA_ADAPTION_SAVE_ERR             = 554|QL_FOTA_ERRCODE_BASE,//保存fota  更新包的路径信息到配置文件失败
    QL_FOTA_ADAPTION_DELETE_ERR           = 555|QL_FOTA_ERRCODE_BASE,//删除fota更新包的路径信息的配置文件失败。
    QL_FOTA_ADAPTION_LOAD_ERR             = 556|QL_FOTA_ERRCODE_BASE,//读取fota更新包的路径信息的配置文件失败。
    QL_FOTA_ADAPTION_LENGTH_ERR           = 557|QL_FOTA_ERRCODE_BASE,//保存 fota pack name 空间出错。 
} ql_errcode_fota_e;

typedef enum
{
    QL_FOTA_FINISHED = 0,   // FOTA 升级操作完成
    QL_FOTA_NOT_EXIST,      // 没有fota更新包，这不是一个错误状态，需要下载fota更新包
    QL_FOTA_READY,          // fota更新包已经被设置准备状态，等待更新
    QL_FOTA_STATUS_INVALID, // FOTA状态是无效的
    QL_FOTA_PACK_CHECK_ERR, // FOTA包校验失败
} ql_fota_result_e;


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_fota_get_pack_name
*
* Description:
* 	获得获得当前fota更新包的路径信息。
* Parameters:
*   p_pac_file_name  [in]   存放当前fota更新包路径空间的首地址      
*   length           [in]   空间的大小
* Return:
*   QL_FOTA_POINT_NULL_ERR, 操作空指针错误。
*   QL_FOTA_ADAPTION_LENGTH_ERR, 保存 fota pack name 空间出错。
*   QL_FOTA_PACKPATH_INVALID, 输入的文件路径是无效的错误。
*   QL_FOTA_SUCCESS，操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_get_pack_name(char *p_pac_file_name,int* length);
/*****************************************************************
* Function: ql_fota_image_verify
*
* Description:
* 	验证fota文件，如果有效，将fota标志设置为ready。如果无效，函数应该返回QL_FOTA_IMAGE_VERIFY_READY_ERR。检查文件要花些时间。
* Parameters:
*       PackFileName    [in]   设置fota更新包路径字符串的首地址 
* Attention:
*   校验完后，不要在往文件系统写入文件，否则可能会导致校验成功后，实际fota升级空间不足导致失败
* Return:
*   QL_FOTA_PACKPATH_INVALID, 输入的文件路径是无效的错误。
*   QL_FOTA_ADAPTION_LENGTH_ERR, 保存 fota pack name 空间出错
*   QL_FOTA_IMAGE_VERIFY_READY_ERR, fota更新包校验并设置准备状态失败。
*   QL_FOTA_ADAPTION_SAVE_ERR,保存fota  更新包的路径信息到配置文件失败
*   QL_FOTA_SUCCESS，操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_image_verify(char* PackFileName);
/*****************************************************************
* Function: ql_fota_image_verify_without_setflag
*
* Description:
* 	彻底更新fota文件内容，如果无效，函数应该返回QL_FOTA_IMAGE_VERIFY_ERR。更新文件要花些时间。建议直接调用ql_fota_image_verify函数
* Parameters:
*       PackFileName    [in]   设置fota更新包路径字符串的首地址 
* Return:
*   QL_FOTA_PACKPATH_INVALID, 输入的文件路径是无效的错误。
*   QL_FOTA_ADAPTION_LENGTH_ERR, 保存 fota pack name 空间出错
*   QL_FOTA_IMAGE_VERIFY_ERR, fota更新包校验不设置准备状态失败
*   QL_FOTA_ADAPTION_SAVE_ERR,保存fota  更新包的路径信息到配置文件失败
*   QL_FOTA_SUCCESS，操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_image_verify_without_setflag(char* PackFileName);
/*****************************************************************
* Function: ql_fota_file_reset
*
* Description:
* 	清除fota文件。当你将del_image设置为true时，你将删除fota文件，无论如何函数应该返回成功。
* Parameters:
*       del_image       [in]   del_image设置为true时，将删除fota图像文件,否则不删除 
* Return:
*   QL_FOTA_ADAPTION_DELETE_ERR, 删除fota更新包的路径信息的配置文件失败。
*   QL_FOTA_SUCCESS，操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_file_reset(bool del_image);
/*****************************************************************
* Function: ql_fota_get_result
*
* Description:
* 	通过param p_fota_result得到fota结果输出。如果p_fota_result不为NULL，函数应该返回成功
* Parameters:
*       p_fota_result       [in]   获取fota升级之后的完成状态。
* Return:
*   QL_FOTA_POINT_NULL_ERR, 操作空指针错误。
*   QL_FOTA_SUCCESS，操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_get_result(ql_fota_result_e *p_fota_result);
/*****************************************************************
* Function: ql_fota_pack_name_convert_fs
*
* Description:
*   将FOTA模块的使用的文件路径信息转化为模块文件系统使用的文件路径信息。内部使用/internally only
* Parameters:
*   fs_path         [in]   文件系统使用的文件路径信息。   
*   fota_path       [in]   FOTA模块的使用的文件路径信息。
*   fs_path_length  [in]   fs_path的空间大小，当返回QL_FOTA_SUCCESS时，表示转化后的fota更新包路径信息的长度。
* Return:
*   QL_FOTA_POINT_NULL_ERR, 操作空指针错误。 
*   QL_FOTA_PACKPATH_INVALID,输入的文件路径是无效的错误。
*   QL_FOTA_PACK_NAME_LENGTH_FAILED，保存 fota pack name 空间出错。
*   QL_FOTA_SUCCESS, 操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fota_pack_name_convert_fs(const char *fota_path,char* fs_path,int *fs_path_length);
/*****************************************************************
* Function: ql_fs_convert_fota_pack_name
*
* Description:
*   将模块文件系统使用的文件路径信息转化为FOTA模块的使用的文件路径信息。内部使用/internally only
* Parameters:
*   fs_path         [in]   文件系统使用的文件路径信息。   
*   fota_path       [in]   FOTA模块的使用的文件路径信息。
*   fs_path_length  [in]   fota_path的空间大小，当返回QL_FOTA_SUCCESS时，表示转化后的fota更新包路径信息的长度。
* Return:
*   QL_FOTA_POINT_NULL_ERR, 操作空指针错误。 
*   QL_FOTA_PACKPATH_INVALID,输入的文件路径是无效的错误。
*   QL_FOTA_PACK_NAME_LENGTH_FAILED，保存 fota pack name 空间出错。
*   QL_FOTA_SUCCESS, 操作成功。
*
*****************************************************************/
ql_errcode_fota_e ql_fs_convert_fota_pack_name(char *fs_path,char *fota_path,int *fota_path_length);

/******************************
* Function: ql_fota_get_image_version
* Description:
* Get version information in the upgrade file
* Example for dtools
* For diff pac
* .\dtools.exe fotacreate2 --pac .\app_new.pac,.\app_old.pac,.\setting\fota8850.xml output_hasversion.pac --oldversion "oldversion" --newversion "newverison"
* For single pac
* .\dtools.exe fotacreate2 --single-pac .\new_pac.pac,.\setting\fota8850.xml output_hasversion.pac --newversion "newversion"
* Note:
* This funtion should be called after ql_fota_image_verify return QL_FOTA_SUCCESS.
* If there is no version information in the upgrade file, the version information returns NULL.
* 
*  \p old_version and \p new_version are allocated inside. Caller should
* free them after used. When there are no version string inside update
* pack, the returned pointer will be NULL.

* @param param1                       [out] return pointer for old version in packed file , if not null need free after used
* @param param2                       [out] return pointer for new version in packed file , if not null need free after used
*
* @return OK                        	0   success
* @return others               	        failed
******************************/
ql_errcode_fota_e ql_fota_get_image_version(char **old_version,char **new_version);

#ifdef __cplusplus
    } /*"C" */
#endif

#endif /* QL_API_FOTA_H */

