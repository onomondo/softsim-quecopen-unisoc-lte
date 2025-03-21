#ifndef _IOT_SINGER_H_
#define _IOT_SINGER_H_

#include "alipay_iot_kal.h"

typedef enum {
    ALIPAY_IOT_SIGN_RV_OK = 0,             //操作成功
    ALIPAY_IOT_SIGN_RV_PARAM_ERROR,        //参数错误
    ALIPAY_IOT_SIGN_RV_BUF_NOT_ENOUGHT,    //传入的内存不足
    ALIPAY_IOT_SIGN_RV_IO_ERROR,           //IO错误
    ALIPAY_IOT_SIGN_RV_SEC_INTERNAL_ERROR, //安全库内部错误
    ALIPAY_IOT_SIGN_RV_SIGN_ERROR,         //签名接口调用错误
    ALIPAY_IOT_SIGN_RV_UNKNOWN = 99        //未知错误
}ALIPAY_IOT_SIGN_RET;

#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/**
 * 反扫（用户支付二维码）支付模式交易加签。
 * @param [in]code  机具扫描到的消费者付款码长度=19位的数字字符串）
 * @param [in]amount 金额，单位为元，可以带小数点。不签金额时amount传入NULL
 * @param [out]result 创建结果（json字符串）
 * @param [out]len_result   创建结果（json字符串）的长度，传入时值应当是result buffer的长度
 *                       执行成功或返回RV_BUF_NOT_ENOUGHT后，会被更新为实际产生的签名数据（所需要的）的长度
 * @return
 */
ALIPAY_IOT_SIGN_RET alipay_iot_sign_qrcode(const char * code, const char * amount, char * result, int * len_result);


/**
 * 主扫（商家收款二维码）支付模式交易加签
 * @param [in]tid  交易单号，可以是支付宝交易单号，也可以是外部订单号
 * @param amount 金额，单位为元，可以带小数点。不签金额时amount传入NULL
 * @param [out]result 创建结果（json字符串）
 * @param [out]len_result   创建结果（json字符串）的长度，传入时值应当是result buffer的长度
 *                       执行成功或返回RV_BUF_NOT_ENOUGHT后，会被更新为实际产生的签名数据（所需要的）的长度
 * @return
 */
ALIPAY_IOT_SIGN_RET alipay_iot_sign_recv_qrcode(const char * tid,const char * amount, char * result, int * len_result);

/**
 * 刷脸（用户人脸token）支付模式交易加签
 * @param faceToken 人脸token(长度36的字符串)
 * @param amount 金额，单位为元，可以带小数点。不签金额时amount传入NULL
 * @param [out]result 创建结果（json字符串）
 * @param [out]len_result   创建结果（json字符串）的长度，传入时值应当是result buffer的长度
 *                       执行成功或返回RV_BUF_NOT_ENOUGHT后，会被更新为实际产生的签名数据（所需要的）的长度
 * @return
 */
ALIPAY_IOT_SIGN_RET alipay_iot_sign_face_token(const char * faceToken, const char * amount, char * result, int * len_result);

/**
 * 代扣（交易标识）支付模式交易加签
 * @param tid 代扣协议号
 * @param amount 金额，单位为元，可以带小数点。不签金额时amount传入NULL
 * @param [out]result 创建结果（json字符串）
 * @param [out]len_result   创建结果（json字符串）的长度，传入时值应当是result buffer的长度
 *                       执行成功或返回RV_BUF_NOT_ENOUGHT后，会被更新为实际产生的签名数据（所需要的）的长度
 * @return
 */
ALIPAY_IOT_SIGN_RET alipay_iot_sign_delegated(const char * tid, const char * amount, char * result, int * len_result);


#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
