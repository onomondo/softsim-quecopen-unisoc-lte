#ifndef __ALIPAY_CERT_H__
#define __ALIPAY_CERT_H__

#include "alipay_common.h"

/***
 * 在证书空发流程中需要用到，若采用产线烧录则忽略该接口，
 * 获取用于去云端兑换证书的数据
 * @param [out]auth_data        去兑换证书的数据，建议大小为2k
 * @param [inout]len_auth_data  输入的时候为允许获取的最大长度，输出的时候为实际获取的长度
 * @return  RV_OK               成功
 *          RV_JS_ERROR         Json操作错误
 *          RV_BUF_TOO_SHORT    允许获取的最大长度过短
 *          RV_UNKNOWN          未知错误
 */
EXTERNC retval_e alipay_secure_get_auth_data(PARAM_OUT char* auth_data, PARAM_INOUT uint32_t* len_auth_data);

/***
 * 在证书空发流程中需要用到，若采用产线烧录则忽略该接口，
 * 解析接收到的数据
 * @param [in]auth_data        接收到的数据
 * @return  RV_OK              解析成功
 *          RV_IO_ERROR             文件系统操作错误
 *          RV_UNKNOWN              未知错误
 */
EXTERNC retval_e alipay_secure_rcv_dev_cert(PARAM_IN char* data);

/**
 * 判断是否需要获取证书
 * @return  1：    本地没有找到证书，需要获取证书
 *          0：    本地有证书
 */
EXTERNC int alipay_is_need_to_request_cert(void);

#endif