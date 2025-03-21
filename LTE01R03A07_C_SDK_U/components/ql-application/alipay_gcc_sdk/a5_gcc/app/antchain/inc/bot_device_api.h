/**
 * @file bot_device_api.h
 * @brief 设备功能接口头文件
 *
 * @copyright Copyright (C) 2015-2023 Ant Group Holding Limited
 */

#ifndef __BOT_DEVICE_API_H__
#define __BOT_DEVICE_API_H__


#ifdef __cplusplus
extern "C"
{
#endif
/***********************************************************************************************************************
 * Including File
***********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

/***********************************************************************************************************************
 * Macro Definition
***********************************************************************************************************************/
// ECDSA and SHA256 params
#define BOT_PUBLIC_KEY_EX_LEN                   64u
#define BOT_SIGNATURE_EX_LEN                    64u
#define BOT_IOT_HASH_VALUE_LEN                  32u

/***********************************************************************************************************************
 * Enumeration Definition
***********************************************************************************************************************/


/***********************************************************************************************************************
 * Type & Structure Definition
***********************************************************************************************************************/
/* 需要用户自行保存设备状态值，在SDK初始化时，通过参数传入 */
typedef enum _bot_dev_status_e {
    BOT_DEV_STATUS_UNREG = 0, /* 设备未注册 */
    BOT_DEV_STATUS_REG,       /* 设备已注册 */
    BOT_DEV_STATUS_INV        /* 无效值 */
} bot_dev_status_e;

/* user's trust info struct */

/***********************************************************************************************************************
 * Global Variable Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Fuction Declaration
***********************************************************************************************************************/
/**
 * @brief bot_device_version_get: get SDK version for parsing in server.
 *
 * @param[inout]  version    			 version: SDK version
 *
 * @return 0 on success, otherwise negative error code indicating the cause of error will be returned.
 */
int bot_device_version_get(char *version);

/**
 * @brief device init: generate and recover ECDSA kaypair and get ASCON encrypt key (optional).
 * *
 * @return 0 on success, otherwise negative error code indicating the cause of error will be returned.
 */
int bot_device_init(void);

/**
 * @brief device asset register: generate the register data using device primary data.
 *
 * @param[out]  reg_out       pointer to the buffer containing the register data
 * @param[out]  reg_outlen    the length of output data
 * @param[in]   reg_in        pointer to the buffer containing the asset data
 * @param[in]   inlen         the length of input data
 *
 * @return 0 on success, otherwise negative error code indicating the cause of error will be returned.
 */
int bot_asset_register(uint8_t *reg_out, int *reg_outlen, uint8_t *reg_in, int inlen);

/**
 * @brief device data upload: generate the upload data by ECDSA signature.
 *
 * @param[out]  sig_out       the signature of register data
 * @param[out]  sig_outlen    the length of signature
 * @param[in]   data_in       pointer to the buffer containing the asset data
 * @param[in]   inlen         the length of input data
 *
 * @return 0 on success, otherwise negative error code indicating the cause of error will be returned.
 */
int bot_data_upload(uint8_t *sig_out, int *sig_outlen, uint8_t *data_in, int inlen);

//4. 注册状态设置接口
int bot_reg_status_set(bot_dev_status_e sta);

//5. 注册状态获取接口
int bot_reg_status_get(bot_dev_status_e *sta);
#ifdef __cplusplus
}
#endif

#endif /* __BOT_DEVICE_API_H__ */
