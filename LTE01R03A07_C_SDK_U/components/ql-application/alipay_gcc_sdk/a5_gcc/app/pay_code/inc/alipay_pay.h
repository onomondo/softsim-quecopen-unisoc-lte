#ifndef __ALIPAY_PAY_H__
#define __ALIPAY_PAY_H__

#include "alipay_common.h"

/**
 * 获取支付码，支付码为18或19位纯数字字符串，条形码编码和显示时应注意
 * @param [out] paycode     存放支付码的指针，建议缓存大小为20
 * @param [inout] len_paycode 支付码数据大小，传入时为paycode buff大小，传出时为实际获取的大小
 * @return  RV_OK：    获取成功
 *          其他value：获取失败
 */
EXTERNC retval_e alipay_get_paycode(PARAM_OUT uint8_t *paycode, PARAM_INOUT uint32_t *len_paycode);

/***
 * 获取收钱码，仅儿童设备可用，用法及返回值状态同alipay_get_paycode()
 * 收钱码为长度40或41的字符串，含有字母和数字
 * @param [out] paycode     存放支付码的指针，建议缓存大小为64
 * @param [inout] len_paycode 支付码数据大小，传入时为允许的最大大小，传出时为实际获取的大小
 * @return  RV_OK：    获取成功
 *          其他value：获取失败
 */
EXTERNC retval_e alipay_get_collectioncode(PARAM_OUT uint8_t *code, PARAM_INOUT uint32_t *len_code);

#endif