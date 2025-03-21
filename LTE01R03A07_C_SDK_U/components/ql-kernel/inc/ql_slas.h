/*****************************************************************//**
* \file ql_slas.h
* \brief 
* \author larson.li@quectel.com
* \date 2023-05-12
* 
* \copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd. 
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
* 
* \par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-05-12 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QL_SLAS_H__
#define __QL_SLAS_H__

typedef enum 
{
    QL_LIC_OK = 0,
    QL_LIC_ERROR_BASE = 0x80000000,
    QL_LIC_MALLOC_ERROR,
    QL_LIC_GET_LICENSE_INFO_ERROR,
    QL_LIC_JSON_PARSER_ERROR,
    QL_LIC_DEVICE_INEQUALITY_ERROR,
    QL_LIC_APP_NO_REGISTER_ERROR,
    QL_LIC_CHECK_APP_ID_ERROR,
    QL_LIC_JSON_PARSER_TIME_ERROR,
    QL_LIC_LICENSE_TIMEOUT_ERROR,
    QL_LIC_BASE64_DECODE_ERROR,
    QL_LIC_VERIFY_SIGNATURE_ERROR,
    QL_LIC_APP_SAVE_PUBLIC_KEY_ERROR,
    QL_LIC_APP_READ_PUBLIC_KEY_ERROR,
    QL_LIC_SAVE_LICENSE_ERROR,
    QL_LIC_INVALID_APP_ID,

    QL_LIC_ERROR_MAX
}ql_license_error_e;

typedef enum
{
    QL_LIC_APP_MIN_TYPE = -1,
    QL_LIC_APP_1_TYPE = 0, /*!< 校验的APP类型1 */
    QL_LIC_APP_2_TYPE,     /*!< 校验的APP类型2 */
    QL_LIC_APP_3_TYPE,     /*!< 校验的APP类型3 */
    QL_LIC_NEW_APP1_TYPE,
    QL_LIC_APP_MAX_TYPE
} ql_license_app_type_t;

/**
 * @brief 进行SLAS初始化
 *
 *
 */
void ql_slas_init(void);

/**
 * @brief 根据指定的APP ID检查是否通过了license授权
 *
 * @param[in] int appid
 *            对应要查询的APP ID
 *
 * @return ql_license_error_e
 *         返回 QL_LIC_OK (0)表示成功,
 *         返回其他表示执行失败
 */
ql_license_error_e ql_slas_get_active_result(int appid);

/**
 * @brief 获取模组设备信息接口
 *
 * @param[in] unsigned char * dev_info
 *            要获设备信息地址空间起始地址
 * 
 * @param[in] int * dev_info_len
 *            dev_info空间信息长度大小, 
 *
 * @return int
 *            返回0 表示执行成功,
 *            返回非0 表示执行失败
 *
 * @note 接口内部实现需要注意,返回的dev信息可以是IMEI+SN+CPUID+FlashID任意组合
 *       建议将唯一值信息放在前面如CPUID+FLASHID+SN
 */
int ql_slas_license_get_dev_info(unsigned char *dev_info, int *dev_info_len);

/**
 * @brief 计算摘要
 *
 * @param[in] unsigned char * plaintest
 *            计算摘要时所传递的明文字串
 * 
 * @param[in] int plaintext_len
 *            明文字串对应的长度
 * 
 * @param[in] unsigned char * digest_txt
 *            计算完成后用以保存摘要信息
 * 
 * @param[in] int *degest_len
 *            对应digest_txt空间长度,同时返回计算摘要的实际长度
 *
 * @return int
 *            返回0表示执行成功
 *            返回其他表示执行失败
 *
 * @note 摘要计算使用的是SHA256算法计算摘要
 */
int ql_slas_license_digest(unsigned char *plaintest, int plaintext_len, unsigned char *digest_txt, int *degest_len);

/**
 * @brief 用于appid转为apptype
 *
 * @param[in] int appid
 *            要转换的appid
 * 
 * @return ql_license_error_e
 *         返回 QL_LIC_OK 表示成功,
 *         返回其他表示执行失败
 */
ql_license_app_type_t ql_slas_license_appid_to_apptype(int appid);

/**
 * @brief 保存上层传递下来的license信息，其在保存前会校验当前要保存的license是否有效
 *
 * @param[in] ql_license_app_type_t app_type
 *            要保存license信息的app类型
 * 
 * @param[in] unsigned char * data
 *            保存license的详细信息
 * 
 * @param[in] int data_len
 *            license详细信息的长度
 *
 * @return ql_license_error_e
 *         返回 QL_LIC_OK 表示成功,
 *         返回其他表示执行失败
 *
 */
ql_license_error_e ql_slas_license_save_license_info(ql_license_app_type_t app_type, unsigned char *data, int data_len);

 /**
 * @brief 用于获取appid
 *
 * @param[in] ql_license_app_type_t app_type               
 *            要获取appid的app_type 
   @param[out]  int *appid
                获取的appid随参数返回
 * @return ql_license_error_e
 *         返回 QL_LIC_OK 表示成功,
 *         返回其他表示执行失败
 */   
ql_license_error_e ql_slas_license_get_apppid(ql_license_app_type_t app_type, int *appid);
/**
 * @brief 根据指定的APP ID检查是否通过了license授权
 *
 * @param[in] ql_license_app_type_t app_id
 *            对应要查询的APP ID
 *
 * @return ql_license_error_e
 *         返回 QL_LIC_OK 表示成功,
 *         返回其他表示执行失败
 */
ql_license_error_e ql_slas_license_get_active_result(ql_license_app_type_t app_type);

#endif /* __QL_SLAS_H__ */