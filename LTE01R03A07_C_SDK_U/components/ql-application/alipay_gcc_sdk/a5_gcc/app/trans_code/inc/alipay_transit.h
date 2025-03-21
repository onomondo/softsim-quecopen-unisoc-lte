#ifndef __ALIPAY_TRANSIT_H__
#define __ALIPAY_TRANSIT_H__

#include "alipay_common.h"
typedef struct alipay_tansit_CardBaseVO
{
    char  cardNo[40];               //卡号
    char  cardType[40];             //卡类型
    char  title[40];                //卡标题
}alipay_tansit_CardBaseVO_t;

typedef enum {
    TRANSIT_CARD_LIST_UNBIND        = 0,            //设备已经在服务端解绑，设备端也需要调用解绑接口
    TRANSIT_CARD_LIST_NET_ERROR     = 1,            //网络错误
    TRANSIT_CARD_LIST_OWN_TOO_MANY_CARD     = 2,    //用户开通了太多乘车码
    TRANSIT_CARD_LIST_FUNC_NOT_SUPPPORT     = 3,    //不支持该功能
    TRANSIT_CARD_LIST_DEV_ERROR     = 4,            //不该出现的错误
    TRANSIT_CARD_LIST_BUF_TOO_SHORT         = 5,    //入参的缓存太小
    TRANSIT_CARD_LIST_SUCCESS        = 99,          //获取成功
}transit_card_list_result_e;

typedef enum {
    TRANSIT_CODE_UNBIND             = 0,            //设备已经在服务端解绑，设备端也需要调用解绑接口
    TRANSIT_CODE_NET_ERROR          = 1,            //网络错误
    TRANSIT_CODE_DATA_LIMIT         = 2,            //因安全原因限制生码
    TRANSIT_CODE_NOT_SUPPORT        = 3,            //该乘车码不支持
    TRANSIT_CODE_SERVER_ERROR       = 4,            //服务端报错
    TRANSIT_CODE_DEV_ERROR          = 5,            //不该出现的错误
    TRANSIT_CODE_SUCCESS            = 99,           //生码或者更新乘车码信息成功
}transit_transit_code_result_e;

/***
 * 获取本地的开通状态(成人设备默认开通，无需关心该接口)
 * @param   [out]  activity_flag 开通状态，true表示已经开通，false表示未开通
 * @return  RV_OK  获取成功
 *          其他    获取失败，当未开通处理
 **/
EXTERNC retval_e alipay_transit_get_activity_status_offline(PARAM_OUT bool* activity_flag);

/***
 * 获取服务端的开通状态(成人设备默认开通，无需关心该接口)
 * @param   [out]  activity_flag 开通状态，true表示已经开通，false表示未开通
 * @return  RV_OK  获取成功
 *          其他    获取失败，当未开通处理
 **/
EXTERNC retval_e alipay_transit_get_activity_status_online(PARAM_OUT bool* activity_flag);

/***
 * 获取本地缓存的城市列表
 * @param   [out]  card_list 城市列表
 * @param   [inout]  len_card_list 传入时表示允许读取的最大长度，传出时为实际获取的长度
 * @param   [out]  card_num 实际的卡数量，返回值为RV_BUF_TOO_SHORT时，该参数仍有效为实际的卡数量
 * @return  RV_OK  获取成功
 *          RV_BUF_TOO_SHORT(15)    传入的buffer太小，需要根据实际的card_num扩大缓存，再次获取
 *          RV_IO_ERROR(5)         读取失败, 本地不存在，通过联网接口获取
 **/
EXTERNC retval_e alipay_transit_get_card_list_offline(PARAM_OUT alipay_tansit_CardBaseVO_t* card_list, PARAM_INOUT uint32_t* len_card_list, PARAM_OUT uint32_t* card_num);

/***
 * 获取服务端的城市列表
 * @param   [out]  card_list 城市列表
 * @param   [inout]  len_card_list 传入时表示允许读取的最大长度，传出时为实际获取的长度
 * @param   [out]  card_num 实际的卡数量，返回值为RV_BUF_TOO_SHORT时，该参数仍有效为实际的卡数量
 * @return  RV_OK  获取成功
 *          RV_BUF_TOO_SHORT(15)    传入的buffer太小，需要根据实际的card_num扩大缓存，再次获取
 *          RV_NETWORK_ERROR(7)     网络错误，可以再尝试获取一次
 *          其他错误                 不应该出现
 **/
EXTERNC retval_e alipay_transit_get_card_list_online(PARAM_OUT alipay_tansit_CardBaseVO_t* card_list, PARAM_INOUT uint32_t* len_card_list, PARAM_OUT uint32_t* card_num);

/***
 * 更新指定卡的数据
 * @param   [in]cardNo              卡序号
 *          [in]cardType            卡类型
 *          [out] error_message     错误原因，建议缓存大小为256，仅在返回值为RV_SERVER_FAIL_ERROR的时候使用
 *          [in] len_error_message  error_message的buffer大小，建议为256
 * @return  RV_OK(0)                更新成功
 *          RV_NETWORK_ERROR(5)     网络错误，可以再尝试更新一次
 *          其他错误                 不应该出现
 **/
EXTERNC retval_e alipay_transit_update_card_data(PARAM_IN char* cardNo, PARAM_IN char* cardType, PARAM_OUT char* error_message, PARAM_IN uint32_t len_error_message);

typedef struct {
    bool    is_exists;              //本地是否有缓存
    int32_t expire_timestamp;       //过期时间戳
    int32_t remain_use_count;       //未来12小时内剩余使用次数
} alipay_transit_card_status_t;

EXTERNC retval_e alipay_transit_check_card_status(PARAM_IN char* cardNo, PARAM_IN char* cardType, PARAM_OUT alipay_transit_card_status_t* status);

/***
 * 获取指定卡的乘车码码值
 * @param   [in]cardNo              卡序号
 *          [in]cardType            卡类型
 *          [out]transitcode        储存码值的缓存，buffer大小建议为512，字节码，不是字符串
 *          [inout]len_transitcode  输入时为允许获取的最大长度，输出时为实际获取的长度
 *          [out] error_message     错误原因，缓存大小建议为256，仅在返回值为RV_SERVER_FAIL_ERROR的时候使用
 *          [in] len_error_message  error_message的buffer大小，建议为256
 * @return  RV_OK 更新成功
 *          RV_NETWORK_ERROR(5)          网络错误
 *          RV_SERVER_FAIL_ERROR         生码失败
 *          RV_CARD_DATA_LIMITED(40)    由于风控原因生码受限，切换到手机侧使用
 *          RV_BUF_TOO_SHORT(15)        允许的最大长度小于实际获取的长度，不应该出现
 *          RV_UNSUPPORTED_CARD(43)     不支持该乘车码，显示“暂不支持，请重试或切换到手机侧使用”
 *          RV_BUSCARDDATA_INVALID(54)  需要联网更新数据
 *          其他错误                     不应该出现,显示“暂不支持，请重试或切换到手机侧使用”
 **/
EXTERNC retval_e alipay_transit_get_TransitCode(PARAM_IN char* cardNo,PARAM_IN char* cardType,PARAM_OUT uint8_t* transitcode,PARAM_INOUT uint32_t* len_transitcode, PARAM_OUT char* error_message, PARAM_IN uint32_t len_error_message);

/***
 * 获取最近一次使用的乘车码码值，断电后会清空
 * @param   [out]title              卡标题，缓存大小建议为40
 *          [out]transitcode        储存码值的缓存，buffer大小建议为512，字节码，不是字符串
 *          [inout]len_transitcode  输入时为允许获取的最大长度，输出时为实际获取的长度
 *          [out] error_message     错误原因，缓存大小建议为256，仅在返回值为RV_SERVER_FAIL_ERROR的时候使用
 *          [in] len_error_message  error_message的buffer大小，建议为256
 * @return  RV_OK 获取到默认卡 
 *          RV_BUF_TOO_SHORT（15）        允许的最大长度小于实际获取的长度，不应该出现
 *          RV_UNSUPPORTED_CARD（43）     不支持该乘车码，显示“暂不支持，请重试或切换到手机侧使用”
 *          RV_BUSCARDDATA_INVALID（54）  需要联网更新数据
 *          RV_CARD_DATA_LIMITED(40)     由于风控原因生码受限，切换到手机侧使用
 *          RV_COMMON_ERROR(56)          显示“暂不支持，请重试或切换到手机侧使用”
 *          其他错误                      未获取到上一次卡信息
 **/
EXTERNC retval_e alipay_transit_get_the_last_transitCode(PARAM_OUT char* title, PARAM_OUT uint8_t* transitcode, PARAM_INOUT uint32_t* len_transitcode, PARAM_OUT char* error_message, PARAM_IN uint32_t len_error_message);

/**
 * @brief 获取最近一次使用的乘车码卡信息, 断电后会丢失
 * @param card_info 
 * @return  RV_OK   获取到默认卡 
 *          其他错误 未获取到上一次卡信息
 */
EXTERNC retval_e alipay_transit_get_the_last_transcode_info(PARAM_OUT alipay_tansit_CardBaseVO_t* card_info);

/**
 * 初始化该功能，启动设备或者进入支付宝前调用一次即可，若失败则表示功能不可用，不允许打开支付宝
 * @return  0：    初始化成功
 *          其他value：失败
 */
EXTERNC int alipay_init_transit_module(void);

/**
 * @brief 中断网路请求(正在进行中的最近一次)
 */
EXTERNC void alipay_break_rpc();

/**
 * @brief 调用alipay_transit_get_card_list_offline和alipay_transit_get_card_list_online之后可以
 * 调用此接口查询具体错误原因
 */
EXTERNC void alipay_get_last_card_list_result(transit_card_list_result_e* request_result);

/**
 * @brief 调用alipay_transit_update_card_data、alipay_transit_get_the_last_transitCode
 * 、alipay_transit_get_TransitCode之后可以调用此接口查询具体错误原因
 */
EXTERNC void alipay_get_last_transit_code_result(transit_transit_code_result_e* request_result);

#endif
