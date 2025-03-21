#ifndef __ALIPAY_ACCOUNT_MANAGE_H__
#define __ALIPAY_ACCOUNT_MANAGE_H__

#include "alipay_common.h"

/**
 * 获取缓存的绑定状态
 * @param [in] void
 * @return 返回绑定状态(是否已经绑定)
 */
EXTERNC bool alipay_get_binding_status(void);

/**
 * 获取缓存的的logon_ID
 * @param [out] id 存放数据的指针，建议buffer大小为128
 * @param [inout] len_id数据大小，输入时为最大允许读取长度，输出时为实际长度
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 */
EXTERNC retval_e alipay_get_logon_ID(PARAM_OUT uint8_t *id,PARAM_INOUT uint32_t *len_id);

/**
 * 读取存储的nick_name
 * @param [out] nick_name 存放数据的指针，，建议buffer大小为128
 * @param [out] len_nick_name数据大小，输入时为最大允许读取长度，输出时为实际长度
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 */
EXTERNC retval_e alipay_get_nick_name(PARAM_OUT uint8_t *nick_name,PARAM_INOUT uint32_t *len_nick_name);

/**
 * 支付功能解绑，清除绑定数据
 * @return  RV_OK：    成功
 *          其他value：失败  频繁失败说明芯片可能出错，建议禁止支付功能
 */
EXTERNC retval_e alipay_unbinding(void);

/**
 * @brief 清除所有数据，包括安全环境(含加密芯片中的账户维度 + 设备维护的所有数据) 耗时较多
 * @return 
 *          RV_OK：    成功
 *          其他value：失败  频繁失败说明芯片可能出错，建议禁止支付功能
 */
EXTERNC retval_e alipay_reset_all(void);
#endif