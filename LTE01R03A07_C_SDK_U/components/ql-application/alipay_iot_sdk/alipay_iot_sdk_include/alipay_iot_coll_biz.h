#ifndef alipay_iot_coll_biz_h
#define alipay_iot_coll_biz_h

#include "alipay_iot_type.h"
#include "alipay_iot_kal.h"
#define __ALIPAY_IOT_MDAP__
typedef enum{
    alipay_iot_biz_result_succ          = 0,/**成功*/
    alipay_iot_biz_result_para_err      = 1,/**参数有误*/
    alipay_iot_biz_result_not_init      = 2,/**还没初始化*/
    alipay_iot_biz_result_buff_err      = 3,/**缓存队列访问错误*/
    alipay_iot_biz_result_mem_err       = 4,/**内存分配失败*/
    alipay_iot_biz_result_busy_err      = 5,/**mdap繁忙*/
    alipay_iot_biz_result_buff_full     = 6,/**缓存队列已满*/
    alipay_iot_biz_result_backup_fail   = 7,/**数据备份失败，但不影响L1缓存中数据的使用*/
} alipay_iot_biz_result_code_t;

typedef enum{
#ifdef __ALIPAY_IOT_MDAP__
    DATA_QUEUE_TYPE_MDAP,
#endif
#ifdef __ALIPAY_IOT_DATADRIVER__
    DATA_QUEUE_TYPE_DATADRIVER,
    DATA_QUEUE_TYPE_DATADRIVER_TRANSACTION,
#endif
    DATA_QUEUE_TYPE_MAX
}DATA_QUEUE_TYPE;

/**
* init data queue. then you can fill queue via alipay_iot_coll_xxx_send(...).
* but only when sdk network finished can data be sent. ref MSG_ID_ALIPAY_SDK_NW_INIT_REQ/MSG_ID_ALIPAY_SDK_NW_INIT_RSP.
*
* @param queue_L1_size, ram buffer number  platform can provide for data sending, must bigger than 5.
* @param queue_L2_size, buffer record number your platform can provide for data sending, must bigger than 5.
* @return: <0 fail;  0 success
* @note when you call alipay_iot_coll_cache_close()， records will be save to flash, the max record number is queue_L1_size + queue_L2_size .
*/
extern int32_t alipay_iot_coll_cache_open(DATA_QUEUE_TYPE queue_type, uint32_t queue_L1_size, uint32_t queue_L2_size);

/**
* flush queue to network.
* Note that this is only a trigger action, the completion doesn't means the data flush is completed.
*/
extern void alipay_iot_coll_cache_flush(DATA_QUEUE_TYPE queue_type);

/**
* close queue if alipay_iot_coll_cache_open called by sdk user.
* if not called before system power off, data in cache may be lost.
*/
extern void alipay_iot_coll_cache_close(DATA_QUEUE_TYPE queue_type);

#ifdef __ALIPAY_IOT_MDAP__

typedef enum{
    alipay_iot_sec_type_none,
    alipay_iot_sec_type_sha_256,
    alipay_iot_sec_type_sec_ts1,
} alipay_iot_sec_type;

#define MDAP_KEY_LENGTH     32
#define MDAP_VALUE_LENGTH   (3*1024)

/* key和val都是字符串，都要以'\0'结尾 */
typedef struct {
    char *key;      //key的长度最长为MDAP_KEY_LENGTH - 1
    char *val;      //vale的长度最长为MDAP_VALUE_LENGTH - 1
    alipay_iot_sec_type sec_type;
} alipay_iot_mdap_key_value;

/* kv数组，count是kv数组里面的元素个数 */
typedef struct {
    alipay_iot_mdap_key_value *kv;
    unsigned int               count;
    char                      *event_id;
    char                      *sub_type;
} alipay_iot_mdap_map;

//交易流水号长度
#define BUSINESS_NO_MAX_LEN 64
//付款码长度
#define QRCODE_MAX_LEN 64
//流量账户长度
#define ACOUNT_FLOW_MAX_LEN 64
//ICCID长度
#define ICCID_MAX_LEN 24

typedef enum{
    ALIPAY_IOT_TRANCTION_ERR_UNKNOWN           =-1,//交易结果无法获取
    ALIPAY_IOT_TRANCTION_ERR_NONE              = 0,//交易成功时设置
    ALIPAY_IOT_TRANCTION_ERR_TIMEOUT           = 1,//交易超时
    ALIPAY_IOT_TRANCTION_ERR_TRADE_FAIL        = 2,//交易返回失败
    ALIPAY_IOT_TRANCTION_ERR_PROCESSING        = 3,//支付处理中
    ALIPAY_IOT_TRANCTION_ERR_NETWORK_EXCEPTION = 4,//网络异常
    ALIPAY_IOT_TRANCTION_ERR_UNKNOW_EXCEPTION  = 5,//未知异常
    ALIPAY_IOT_TRANCTION_ERR_NOT_SUPPORT       = 6,//不支持的交易
}ALIPAY_IOT_TRANCTION_ERR_ENUM;

typedef struct{
    char business_no[BUSINESS_NO_MAX_LEN+1];  //交易成功返回的流水号, 必须以'\0'结尾; 第一个字节为数字0表示不传
    char qr_code[QRCODE_MAX_LEN+1];           //付款码, 必须以'\0'结尾; 第一个字节为数字0表示不传
    int32_t amount;                           //交易金额,单位为分; 为负数表示不传
    int32_t time_consuming;                   //交易耗时毫秒数; 为负数表示不传
    ALIPAY_IOT_TRANCTION_ERR_ENUM fail_reason;//交易结果
    uint32_t timestamp;                       //UTC时间戳, 仅仅为参考用。以SDK内部时间为准
}alipay_iot_transaction_data_struct;

typedef struct{
    uint32_t brocast_count;                     //语音播报增量次数
    uint32_t scan_count;                        //扫码次数增量
}alipay_iot_action_data_struct;

typedef enum
{
    ALIPAY_IOT_VERIFY_QRCODE        = 0x01,//二维码识别
    ALIPAY_IOT_VERIFY_FACE          = 0x02,//人脸识别
    ALIPAY_IOT_VERIFY_MSISDN        = 0x04,//手机号码识别(SMS/CALL)
    ALIPAY_IOT_VERIFY_VOICE_PRINT   = 0x08,//声纹识别
    ALIPAY_IOT_VERIFY_NFC           = 0x10,//NFC识别
    ALIPAY_IOT_VERIFY_FINGER_PRINT  = 0x20,//指纹识别
    ALIPAY_IOT_VERIFY_EMAIL         = 0x40,//邮箱识别
}ALIPAY_IOT_HUMAN_VERIFY;

typedef struct
{
    char account_flow[ACOUNT_FLOW_MAX_LEN+1]; //流量账户信息，默认和传入SIM卡的ICCID保持一致; 第一个字节为数字0表示不传
    char iccid[ICCID_MAX_LEN+1];              //SIM卡的ICCID; 第一个字节为数字0表示不传
    uint32_t human_verify;                    //身份核实方式，值为ALIPAY_IOT_HUMAN_VERIFY的按bit组合; 为0表示不传
}alipay_iot_production_info_struct;


/**
* send transaction data to alipay
* call this api each time you got a qrcode or business_no.
* @param transaction data
* @return return alipay_iot_result_code_t
*/
extern alipay_iot_biz_result_code_t alipay_iot_coll_transaction_data_send(const alipay_iot_transaction_data_struct* transaction);

/**
* send transaction data to alipay, the api will send to network immediately
* call this api each time you got a qrcode or business_no.
* @param transaction data
* @return return alipay_iot_result_code_t
*/
extern alipay_iot_biz_result_code_t alipay_iot_coll_transaction_data_send_rt(const alipay_iot_transaction_data_struct* transaction);

/**
* send action data to alipay
* call this api each time you scan qrcode or broadcast a voice
* @param action data
* @return return alipay_iot_result_code_t
*/
extern alipay_iot_biz_result_code_t alipay_iot_coll_action_data_send(const alipay_iot_action_data_struct* action);


/**
* send production info data to alipay
* call this api each time the sdk startup. (that means the device power on)
* @param production data
* @return return alipay_iot_result_code_t
*/
alipay_iot_biz_result_code_t alipay_iot_coll_production_info_send(const alipay_iot_production_info_struct* pd_info);
#endif

#ifdef __ALIPAY_IOT_DATADRIVER__

//属性值
typedef struct
{
  char*   id;      //唯一值，重复时会根据该值进行覆盖
  char*   value;   //属性值,是一个json值，随着协议的不同而不同
  int32_t status;   //是否有效， 2有效, -2失效， 其余待扩展
}property_value_struct;

//属性
typedef struct
{
    int32_t   scope;                //1:全量，2.增量
    char*     name;                 //metaData、tinyApp、nativeApp
    uint8_t   count_of_value;       //value数组个数
    property_value_struct *value;   //设备的属性值列表，指向一个或者多个连续的property_value_struct
}property_struct;

/**
 * send real-time property to alipay
 * data sent via this api may lost cause the limit of ram/rom or network quality.
 */
alipay_iot_biz_result_code_t alipay_iot_coll_datadriver_send(char* sub_device_sn, property_struct* property);

//datadriver 交易上报类型
typedef enum
{
    /**
     * 交易成功返回的支付宝交易号
     */
    TransactionDataTypeTradeId,
    /**
     * 支付宝付款二维码
     */
    TransactionDataTypeQrCode,
    /**
     * 刷脸sdk产生的ftoken
     */
    TransactionDataTypeFToken,
    /**
     * 刷脸sdk产生的28码
     */
    TransactionDataTypeBarCode
}TRANSACTION_DATA_TYPE;

/**
 * 基于datadriver的交易上报
 * 使用前必须先调用alipay_iot_coll_cache_open进行DATA_QUEUE_TYPE_DATADRIVER_TRANSACTION类型的缓存初始化.
 */
alipay_iot_biz_result_code_t alipay_iot_coll_datadriver_transaction_send(TRANSACTION_DATA_TYPE type, char *value);



#endif

#endif /* alipay_iot_coll_biz_h */
