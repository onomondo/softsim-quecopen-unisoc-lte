#ifndef __AID_CODE_H__
#define __AID_CODE_H__

#include "alipay_common.h"

/**
 * 获取帮助码
 * @param [out] aid_code 存放数据的指针，建议buffer大小为256
 * @param [inout] len_aid_code 帮助码大小，输入时为最大允许读取长度，输出时为实际长度
 * @param [in] isSupportBle 若为true,表示支持蓝牙，进入帮助码页面后需要打开蓝牙和3802服务
 * @return  RV_OK：获取成功
 *          RV_WRONG_PARAM：参数错误
 *          RV_UNKNOWN：不应该出现
 *          RV_JS_ERROR：不应该出现
 *          RV_BUF_TOO_SHORT：缓存太小
 */
EXTERNC retval_e alipay_get_aid_code(PARAM_OUT char aid_code[256], PARAM_INOUT uint32_t* len_aid_code, PARAM_IN bool isSupportBle);

#endif