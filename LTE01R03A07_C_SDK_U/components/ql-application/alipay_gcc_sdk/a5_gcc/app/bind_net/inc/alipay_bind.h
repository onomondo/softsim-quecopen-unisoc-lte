#ifndef __ALIPAY_BIND_H__
#define __ALIPAY_BIND_H__

#include "alipay_common.h"

/**
 * 初始化该功能，启动设备或者进入支付宝前调用一次即可，若失败则表示功能不可用，不允许打开支付宝
 * @return  0：    初始化成功
 *          其他value：失败
 */
EXTERNC int alipay_pre_init(void);

/**
 * 获取绑定码
 * @param [out] result     存放绑定码的指针,建议buffer大小为256
 * @param [inout] len_result 绑定码数据大小，传入时为允许的最大长度，传出时为实际的长度
 * @return  RV_OK：    获取成功
 *          RV_WRONG_PARAM：入惨错误，result为空，或者*len_result为0
 *          RV_BUF_TOO_SHORT：缓存太小
 *          RV_NETWORK_ERROR：网络错误，可尝试重新调用此接口
 *          其他错误：不应该出现
 */
EXTERNC retval_e alipay_get_binding_code(PARAM_OUT char* result, PARAM_INOUT int *len_result);

/***
 * 轮询绑定结果
 * @param [out] status     当前的绑定状态
 * @return  RV_OK  绑定成功，数据已保存
 *          RV_PROCESSING  绑定过程进行中，请继续轮询绑定结果;
 *                         建议的轮询间隔为2s，超时时间建议为60s;超时后仍然返回processing，即可认为绑定失败
 *          RV_UNKNOWN   绑定失败，需要重新获取绑定码绑定。
 */
EXTERNC retval_e alipay_query_binding_result(PARAM_OUT binding_status_e* status);

/***
 * 异步通知绑定进度的回调
 * @param [out] status     当前的绑定进度
 * 注: 目前只会有一个中间的状态【开始】。如不关注中间进度，可空实现该函数。
 */
EXTERNC void alipay_binding_process_notify(int status);

#endif