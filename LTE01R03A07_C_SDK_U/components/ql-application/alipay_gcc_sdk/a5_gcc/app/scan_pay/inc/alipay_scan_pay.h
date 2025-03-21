#ifndef __ALIPAY_SCAN_PAY_H__
#define __ALIPAY_SCAN_PAY_H__

#include "alipay_common.h"

/**
 * 聚合码相关接口(开发联调中)
 */ 
typedef struct {
    __readonly    bool is_success;        //是否成功识别;
    __readonly    char err_code[8];       //错误码
    //订单基本信息
    __readonly    char payee_name[128];   //收款方名称
    __readonly    char payee_logo[1024];  //收款方Logo URL
    __readwrite   char pay_amount[32];    //额度(仅在额度为空[strlen(pay_amount) <= 0]的时候，可以设置)    
} alipay_union_code_order_info_t;

/** 
 * 发起主扫支付(支持商家码、个人转账码、聚合码等)
 * 入参
 *  [in] buf_img 默认NV12格式的码图像
 *  [in] width 图像宽度
 *  [in] height 图像高度
 * 出参
 *  [out] order_info 订单信息
 * 返回值 仅在返回RV_OK时，订单信息内容才有效
 */ 
EXTERNC retval_e alipay_union_code_parse_with_img(PARAM_IN uint8_t *buf_img, PARAM_IN uint32_t width, PARAM_IN uint32_t height, 
 										  PARAM_OUT alipay_union_code_order_info_t *order_info);

/** 
 * 发起主扫支付(支持商家码、个人转账码、聚合码等)
 * 入参
 *  [in] code 识别出来的码串
 * 出参
 *  [out] order_info 订单信息
 * 返回值 仅在返回RV_OK时，订单信息内容才有效
 */ 
EXTERNC retval_e alipay_union_code_parse_with_code(PARAM_IN char* code, PARAM_OUT alipay_union_code_order_info_t *order_info);

typedef enum {
    ALIPAY_FAIL_RES_AMOUNT_LIMIT       = 0,  //超出设置的支付额度
	ALIPAY_FAIL_RES_BALANCE_NOT_ENOUGH = 1,  //零花钱不足，请存入
	ALIPAY_FAIL_RES_COMMON_FAIL        = 2,  //支付失败
    ALIPAY_FAIL_RES_ACCOUNT_NOT_EXIST  = 3,  //零花钱不存在    
    ALIPAY_FAIL_RES_CASE_NOT_SUPPORT   = 4,  //该场景不支持使用智能设备支付,显示“不支持使用该设备进行支付，请使用手机支付宝支付”
} alipay_scan_pay_fail_res_e; 

typedef enum {
    ALIPAY_SCAN_PAYMENT_STATUS_SUCCESS          = 0,
	ALIPAY_SCAN_PAYMENT_STATUS_FAIL             = 1,
	ALIPAY_SCAN_PAYMENT_STATUS_NOTHING          = 2,
	ALIPAY_SCAN_PAYMENT_STATUS_UNBIND_BY_USER   = 3,
	ALIPAY_SCAN_PAYMENT_STATUS_DISABLED_BY_USER = 4,
    ALIPAY_SCAN_PAYMENT_STATUS_UNKNOWN          = 5,
    ALIPAY_SCAN_PAYMENT_STATUS_PROCESSING       = 6,
} alipay_scan_payment_status_e;

typedef struct {
    __readonly    alipay_scan_payment_status_e status; 
    __readonly    alipay_scan_pay_fail_res_e res_fail;
} alipay_union_code_pay_result_t;

/**
 * 主扫确认支付(支持商家码、个人转账码、聚合码等)
 * 入参
 *  [in] order_info 订单信息
 * 出参
 *  [out] result 确认付款结果
 * 返回值 仅在返回RV_OK时，result中数据才有效
 */ 
EXTERNC retval_e alipay_union_code_confirm_payment(PARAM_IN const alipay_union_code_order_info_t *order_info,
                                           PARAM_OUT alipay_union_code_pay_result_t *result);

#endif