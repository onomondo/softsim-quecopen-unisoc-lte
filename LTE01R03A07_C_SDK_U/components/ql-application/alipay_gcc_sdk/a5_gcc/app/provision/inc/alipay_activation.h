#ifndef __ALIPAY_ACTIVATION_H__
#define __ALIPAY_ACTIVATION_H__

#include "alipay_common.h"

/**
 * @brief 检查设备授权状态
 * @return 是否授权
 */
EXTERNC bool alipay_check_active_status(void);


/**
 * @brief 获取设备端授权请求信息
 * @param buf_output 授权请求内容
 * @param len_output 授权请求长度
 * @return 错误码
 */
EXTERNC retval_e alipay_get_active_request(uint8_t* buf_output, uint32_t *len_output) __alipay_depricated__;

EXTERNC retval_e alipay_get_active_request_campt(INPUT_PARAM const char* model, uint8_t* buf_output, uint32_t *len_output); //阿里云专用


/**
 * @brief 同步蚂蚁链激活接口
 * @param buf_input 激活请求输入
 * @param len_input 激活请求输入长度
 * @param buf_output 激活请求
 * @param len_output 激活请求长度
 * @return EXTERNC 
 */
EXTERNC retval_e alipay_get_active_request_with_antchain(OUTPUT_PARAM uint8_t* buf_output, OUTPUT_PARAM uint32_t *len_output);


/**
 * @brief 保存授权信息
 * @param buf_input 授权信息内容
 * @param len_input 授权信息长度
 * @return 错误码
 */
EXTERNC retval_e alipay_save_active_response(uint8_t* buf_input, uint32_t len_input);


/**
 * 用于测试安全环境通用ecc算法
*/
EXTERNC retval_e alipay_check_ecc_alg();

#endif