#ifndef __ALIPAY_PAY_RESULT_H__
#define __ALIPAY_PAY_RESULT_H__

//该功能需要网络能力

#include "alipay_common.h"

typedef enum {
    ALIPAY_PAYMENT_STATUS_SUCCESS          = 0,
	ALIPAY_PAYMENT_STATUS_FAIL             = 1,
	ALIPAY_PAYMENT_STATUS_NOTHING          = 2,
	ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER   = 3,
	ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER = 4,
    ALIPAY_PAYMENT_STATUS_UNKNOWN          = 5,
    ALIPAY_PAYMENT_STATUS_PROCESSING       = 6,
} alipay_payment_status_e;

typedef struct {
    __readonly    bool is_success;              //是否成功收款
    // __readonly    char amount[32];           //收款金额
    __readonly    char bitmap_amount[32][14];   //收款金额
    __readonly    char message[256];            //收款说明
} alipay_income_detail_info_t;

/***
 * 获取支付码支付结果
 * @param   [in]pay_code    用于查询的付款码码串，来自「alipay_get_paycode」接口
 *          [out]status      支付码支付结果，定义见alipay_payment_status_e，可能的结果：
                           ALIPAY_PAYMENT_STATUS_SUCCESS  支付成功，此时bitmap_amount有金额输出
                           ALIPAY_PAYMENT_STATUS_NOTHING  没有查到交易，请继续使用本接口查询
                           --------------------------其它输出均表示支付已经失败---------------------------
                           ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER  付款码支付功能已经被用户关闭
                           ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER  用户已在手机端解绑本设备
 * @param [out]bitmap_amount  支付金额的bitmap，二维数组，直接从左至右逐bit点阵输出即可（黑色bit为1，白色为0）
 * @return  RV_OK  查询接口调用成功，此时会输出status和bitmap_amount
 *          若返回值为其他，则说明查询接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_INPROCESSING 当前没有获取支付码，无法查询支付结果
 *          RV_READ_ERROR  本地数据读取失败，可能是数据损坏或已解绑
            -----------以下错误建议重试------------
 *          RV_UNKNOWN  未知错误
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常
 */
EXTERNC retval_e alipay_query_order_result(PARAM_IN char* pay_code, PARAM_OUT alipay_payment_status_e *status, PARAM_OUT char bitmap_amount[32][14]);


/**
 * @brief 同alipay_query_order_result, 区别为额外返回字符串形式的额度信息；
 * @param status 
 * @param pay_amount 
 * @param ext_data 
 * @param ext_data_len 
 * @return  RV_OK  查询接口调用成功，此时会输出status和bitmap_amount
 *          若返回值为其他，则说明查询接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_INPROCESSING 当前没有获取支付码，无法查询支付结果
 *          RV_READ_ERROR  本地数据读取失败，可能是数据损坏或已解绑
            -----------以下错误建议重试------------
 *          RV_UNKNOWN  未知错误
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常
 */
EXTERNC retval_e alipay_query_order_result_string(PARAM_IN char* pay_code, PARAM_OUT alipay_payment_status_e *status, PARAM_OUT char pay_amount[32], char *ext_data, PARAM_OUT uint32_t *ext_data_len);

/***
 * 获取支付码支付结果
 * @param   [in]collection_code    用于查询的收款码码串，来自「alipay_get_collectioncode」接口
 * @param   [out]alipay_income_detail_info_t  用法同「alipay_query_order_result」
 * @return  RV_OK  查询接口调用成功，此时会输出status和bitmap_amount
 *          若返回值为其他，则说明查询接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_INPROCESSING 当前没有获取支付码，无法查询支付结果
 *          RV_READ_ERROR  本地数据读取失败，可能是数据损坏或已解绑
            -----------以下错误建议重试------------
 *          RV_UNKNOWN  未知错误
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常
 */
EXTERNC retval_e alipay_query_income_details(PARAM_IN char* collection_code, PARAM_OUT alipay_income_detail_info_t *income_detail);

/***
 * 打断付款码支付结果query接口的执行，让接口立即返回
 * 注意：只允许用来打断alipay_query_order_result接口，请勿在其它接口执行时调用
 */
EXTERNC void alipay_query_flow_break(void);

#endif