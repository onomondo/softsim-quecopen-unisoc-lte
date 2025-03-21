/*
 * 此文件用于对iot sdk使用者显示所有sdk提供的对外接口
 * author: yuyang
 */
#ifndef __ALIPAY_IOT_SDK_H
#define __ALIPAY_IOT_SDK_H
#include "alipay_iot_type.h"

#define ALIPAY_SERVER_DOMAIN_MAX_LEN 64
#define ALIPAY_EVENT_NAME_MAX_LEN 16
#define ALIPAY_EVENT_DESC_LEN 64
#define SDK_DEVICE_ID_MAX_LEN 128
#define SDK_VERSION_MAX_LEN 32
#define SDK_SUPPLIER_NO_MAX_LEN 64
#define SDK_DEVICE_SUPPLIER_NO_MAX_LEN 32
#define SDK_MERCHANT_USER_ID_MAX_LEN 32
#define SDK_ITEM_ID_MAX_LEN 64
#define SDK_PRODUCT_KEY_MAX_LEN 64
#define SDK_DEVICE_FLASH_ID_MAX_LEN 64
#define SDK_DEVICE_CPU_ID_MAX_LEN 64
#define SDK_DEVICE_CAMERA_ID_MAX_LEN 64
#define SDK_DEVICE_MAC_MAX_LEN 17
#define SDK_DEVICE_SN_MAX_LEN 64
#define SDK_DEVICE_IMEI_MAX_LEN 64
#define SDK_DEVICE_MODEL_MAX_LEN 64
#define SDK_OS_VERSION_MAX_LEN 50



typedef enum
{
    MSG_ID_ALIPAY_SDK_START                       = 301, //上接 MSG_ID_ALIPAY_SEC_END

    MSG_ID_ALIPAY_SDK_INIT_REQ                    = MSG_ID_ALIPAY_SDK_START,
    MSG_ID_ALIPAY_SDK_INIT_RSP                    = 302,
    MSG_ID_ALIPAY_SDK_NW_INIT_REQ                 = 303,
    MSG_ID_ALIPAY_SDK_NW_INIT_RSP                 = 304,
#ifdef __ALIPAY_IOT_MQTT__
    MSG_ID_ALIPAY_NW_CONNECT_REQ                  = 305,
    MSG_ID_ALIPAY_NW_CONNECT_RSP                  = 306,
#endif
    MSG_ID_ALIPAY_SDK_CLOSE_IND                   = 307,
    MSG_ID_ALIPAY_SDK_CLOSE_CNF                   = 308,
    MSG_ID_ALIPAY_SYS_TIME_TRIGGER_IND            = 309, //指示SDK获取并设定UTC时间
    MSG_ID_ALIPAY_SYS_TIME_UPDATE_IND             = 310, //告诉SDK USER， SDK获取UTC时间的结果
    MSG_ID_ALIPAY_SDK_EVENT_NOTIFY_IND            = 311,
#ifdef __ALIPAY_IOT_CMD__
    MSG_ID_ALIPAY_SDK_CMD_IND                     = 312,
    MSG_ID_ALIPAY_SDK_CMD_CNF                     = 313,
#endif
#ifdef __ALIPAY_IOT_CONFIG__
    MSG_ID_ALIPAY_SDK_CONFIGURE_QUERY_REQ         = 314,
    MSG_ID_ALIPAY_SDK_CONFIGURE_QUERY_RSP         = 315,
    MSG_ID_ALIPAY_SDK_CONFIGURE_CHANGE_NOTIFY     = 316,
    MSG_ID_ALIPAY_SDK_CONFIGURE_EVENT_REPORT_REQ  = 317,
    MSG_ID_ALIPAY_SDK_CONFIGURE_EVENT_REPORT_RSP  = 318,
#endif
#if defined(__ALIPAY_IOT_OTA_SDK__) || defined(__ALIPAY_IOT_OTA_APP__)
    MSG_ID_ALIPAY_SDK_OTA_EVENT_REPORT_IND        = 319,
#ifdef __ALIPAY_IOT_OTA_APP__
    MSG_ID_ALIPAY_SDK_OTA_CHECK_REQ               = 320,
    MSG_ID_ALIPAY_SDK_OTA_CHECK_RSP               = 321,
#endif
#endif
#ifdef __ALIPAY_IOT_DATADRIVER__
    MSG_ID_ALIPAY_DATADRIVER_SEND_IND             = 322, //发送 data driver 数据
#endif
#ifdef __ALIPAY_IOT_NEWS__
    MSG_ID_ALIPAY_SDK_NEWS_IND                    = 323,
    MSG_ID_ALIPAY_SDK_NEWS_CNF                    = 324,
#endif
#ifdef __ALIPAY_IOT_DISTRIBUTION__
    MSG_ID_ALIPAY_SDK_DISTRIBUTION_INFO_IND       = 325,
    MSG_ID_ALIPAY_SDK_DISTRIBUTION_INFO_QUERY     = 326,
#endif
#ifdef __ALIPAY_IOT_DATADRIVER__
    MSG_ID_ALIPAY_DATADRIVER_TRANSACTION_SEND_IND = 327, //发送datadriver交易数据
#endif
    MSG_ID_QUEC_ALIPAY_SDK_SIGN_REQ               = 400,
    MSG_ID_QUEC_ALIPAY_SDK_ACT_REQ                = 401,
    MSG_ID_QUEC_ALIPAY_SDK_INFO_REQ               = 402,
    MSG_ID_QUEC_ALIPAY_SDK_REP_REQ                = 403,
    MSG_ID_QUEC_ALIPAY_SDK_DID_REQ                = 404,
    MSG_ID_ALIPAY_SDK_END
}ALIPAY_IOT_SDK_MSG_ENUM;

typedef enum
{
    EVENT_ID_EMPTY_NOTIFY    = 0,
    EVENT_ID_OTA_NOTIFY      = 1,
    EVENT_ID_MAX
}SDK_EVENT_ENUM;

typedef struct{
    SDK_EVENT_ENUM event_id;
    int32_t code;
    void *arg1;
    void *arg2;
}alipay_sdk_event_notify_struct;

typedef enum
{
    ALIPAY_SDK_MODE_NORMAL,// normal usage
    ALIPAY_SDK_MODE_FACTORY, //facotry mode. Got bizTid only, less memory usage;
    ALIPAY_SDK_MODE_MAX
}alipay_sdk_mode;

typedef struct
{
    //provided by ant platform, 每个数组需要以数字0结尾
    char item_id[SDK_ITEM_ID_MAX_LEN + 1];                        //item id provided by ant platform
    char module_supplier_no[SDK_SUPPLIER_NO_MAX_LEN + 1];         //hardware supplier NO. 硬件供应商编号
    char device_supplier_no[SDK_DEVICE_SUPPLIER_NO_MAX_LEN + 1];  //device supplier NO.  设备供应商编号
    char merchant_user_id[SDK_MERCHANT_USER_ID_MAX_LEN + 1];      //device operator ID.  设备运营商编号
    char product_key[SDK_PRODUCT_KEY_MAX_LEN + 1];                //product key  区分设备名称
    int32_t env;                                                  //env PROFILE对应的运行环境，0为正式运行环境
}alipay_sdk_profile_info_struct;

typedef struct
{
    int32_t result;//0 success; other fail.
}alipay_sdk_init_rsp_struct;

typedef struct
{
    int32_t result;//0 success; other fail.
}alipay_sdk_nw_init_rsp_struct;


typedef struct
{
    int32_t result;//0 success; other fail.
}alipay_sdk_nw_connect_rsp_struct;

typedef struct
{
    int32_t result;//0 success; other fail.
}alipay_sdk_close_cnf_struct;

typedef struct
{
    int32_t result;//0 success; other fail.
}alipay_sdk_sys_time_update_ind_struct;


/**
* @brief: set profile info for sdk. should be call only once before alipay_iot_start
* @return: 0 means success, other value means fail.
*/
int32_t alipay_iot_sdk_set_profile(alipay_sdk_profile_info_struct* profile);

/**
 * @brief: start iot task
 * @input: priority base priority of iot task. Less number, higher priority. Be futher than ends of all priority of system, at least 5.
 * @input: mode. runing mode: see alipay_sdk_mode;
 * @return: <=0 start fail with fail reason.
 *          >0  start success, value is sdk portal task id.
 */
int32_t alipay_iot_start(int32_t priority, alipay_sdk_mode mode);

/**
* @brief: clean all sdk storage
*   must be called when sdk created(by calling alipay_iot_start()) and not do initing yet.
*   when called, all data include biztid and coll data in storage, not in ram, will be clean.
*/
void alipay_iot_clear_sdk_cache(void);


/**
* @brief: get iot-id(biztid) of this device from alipay.
* @return: a string buffer contains iot id. this buffer need to be free by calling alipay_iot_free_biztid(biztid).
*/
char* alipay_iot_get_biztid(void);
/**
* @brief: free buffer return from alipay_iot_get_biztid().
*/
void alipay_iot_free_biztid(char* biztid);

/**
* @brief: get iotdid of this device from alipay.
* @return: a string buffer contains iot id. this buffer need to be free by calling alipay_iot_free_iotdid(iotdid).
*/
char *alipay_iot_get_iotdid(void);

/**
* @brief: free buffer return from alipay_iot_get_iotdid().
*/
void alipay_iot_free_iotdid(char *iotdid);

/**
 * @brief: destory an alipay_sdk_init_rsp_struct object
 * @input: rsp, reference to alipay_sdk_init_rsp_struct object
 */
void alipay_sdk_init_rsp_struct_destory(alipay_sdk_init_rsp_struct *rsp);
/**
 * @brief: destory an alipay_sdk_event_notify_struct object
 * @input: notify, reference to alipay_sdk_event_notify_struct object
 */
void alipay_sdk_event_notify_struct_destory(alipay_sdk_event_notify_struct *notify);

void alipay_sdk_nw_init_rsp_struct_destory(alipay_sdk_nw_init_rsp_struct* rsp);

void alipay_sdk_nw_connect_rsp_struct_destory(alipay_sdk_nw_connect_rsp_struct* rsp);

void alipay_sdk_close_cnf_struct_destory(alipay_sdk_close_cnf_struct* cnf);

void alipay_sdk_sys_time_update_ind_struct_destory(alipay_sdk_sys_time_update_ind_struct *ind);

/*
* @brief: get device supplier NO. filled by sdk user via alipay_iot_sdk_set_profile().
* @output: a char pointer of device supplier no, need not free.
*/
char* alipay_iot_get_device_supplier_no(void);

/*
* @brief: get sdk version
* @output: a char pointer of device supplier no, need not free.
*/
char* alipay_iot_get_sdk_version(void);

#endif
