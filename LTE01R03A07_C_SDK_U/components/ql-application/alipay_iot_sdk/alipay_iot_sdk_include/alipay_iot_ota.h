/*
 * 此文件用于对iot sdk提供的OTA功能的使用者显示所有sdk提供的对外接口
 * author: yuyang
 */
#ifndef __ALIPAY_IOT_OTA_H
#define __ALIPAY_IOT_OTA_H
#include "alipay_iot_type.h"


#define OTA_MAX_APP_NUM 5
#define OTA_APP_NAME_MAX_LEN 32
#define OTA_APP_VERSION_MAX_LEN 32

/**
* @brief: 应用在使用OTA服务之前，需要注册自己的名称和版本信息、是否需要OTA
* @input: app_name，应用名称，长度不得超过32
* @input: app_version,应用版本，长度不得超过32
* @input: need_ota，扩展用途，用来指示是否需要ota，目前约定总是为true
* @return: 结果：
*             0 表示添加成功;
*            -1 未知原因添加失败;
*            -2 支持个数已达最大值(目前只支持1个APP);
*            -3 参数错误
*/
int32_t alipay_iot_ota_app_register(char* app_name, char* app_version, bool need_ota);


/**
* APP触发服务端查询自身的OTA信息，对应 MSG_ID_ALIPAY_SDK_OTA_CHECK_REQ
* 数据为 alipay_sdk_ota_check_req_struct，
* 数据创建函数为 alipay_iot_ota_check_req_struct_create()
* 数据释放函数为 alipay_iot_ota_check_req_struct_destory()
*/
typedef struct
{
    char* app_name;
}alipay_sdk_ota_check_req_struct;

/**
* 反馈APP触发服务端查询自身的OTA信息的执行结果，对应 MSG_ID_ALIPAY_SDK_OTA_CHECK_RSP
* 数据为：alipay_sdk_ota_check_rsp_struct，
* 数据释放函数为：alipay_iot_ota_check_rsp_struct_destory()
*/
typedef struct
{
    char* app_name;
    int32_t result; //结果. 0表示检查请求发送成功; -1 未知原因添加失败; -2 应用未注册；-3 网络未就绪
}alipay_sdk_ota_check_rsp_struct;

alipay_sdk_ota_check_req_struct* alipay_iot_ota_check_req_struct_create(char* app_name);
void alipay_iot_ota_check_req_struct_destory(alipay_sdk_ota_check_req_struct *rsp);
void alipay_iot_ota_check_rsp_struct_destory(alipay_sdk_ota_check_rsp_struct *rsp);

/**
* OTA消息通知给APP使用 MSG_ID_ALIPAY_SDK_EVENT_NOTIFY_IND
* 数据为：alipay_sdk_event_notify_struct，其中：
*     对应event为EVENT_ID_OTA_NOTIFY，code为0，
*     arg1为指向alipay_iot_ota_event_struct的结构体指针，arg2为NULL
* 数据释放函数为：alipay_sdk_event_notify_struct_destory()
*/
typedef struct
{
    char*    target_id;      // OTA目标对象ID，包名或程序名（全路径）; 此处的内容XDM持久化存储
    char*    task_id;        // 发布任务ID，用于回传状态和输出日志; 此处的内容XDM持久化存储
    char*    download_url;   // 包下载地址
    uint32_t package_size;   // 包大小
    char*    version;        // 目标版本
    char*    decrypt_key;    // 升级包解密（AES）密钥，为空表示非加密包
    char*    digest;         // 包文件信息摘要，使用SHA256算法，16进制字符串表示，长度为64
    char*    signature;      // 升级摘要的RSA签名，为空则不进行签名校验，RSA算法采用RSA2048
    bool     reset;          // 升级时是否复位的标志，目前请忽略此值
    int32_t  delay;          // 固定延时，单位为秒
    int32_t  random_range;   // 随机延时，单位为秒
    int32_t  silent_type;    // 静默类型
    int32_t  perform_mode;   // 执行模式，立即执行(0)或重启后执行(1).
    char*    tips_title;     // 弹出提示标题，最大长度64字节
    char*    tips_text;      // 弹出提示内容，最大长度256字节
}alipay_iot_ota_event_notify_struct;

typedef enum
{
    ALIPAY_IOT_OTA_UNKNOWN        = 0, // N/A
    ALIPAY_IOT_OTA_ERROR          = 1, // OTA出错
    ALIPAY_IOT_OTA_PREPARED       = 2, // 已就绪，已收到OTA指令
    ALIPAY_IOT_OTA_DOWNLOADING    = 3, // 下载中
    ALIPAY_IOT_OTA_DOWNLOADED     = 4, // 下载完成
    ALIPAY_IOT_OTA_RESTARTING     = 5, // 重启中
    ALIPAY_IOT_OTA_REVERTING      = 6, // 回退中
    ALIPAY_IOT_OTA_REVERTED       = 7, // 回退完成
    ALIPAY_IOT_OTA_COMPLETED      = 8, // 升级完成
    ALIPAY_IOT_OTA_SUCCESS        = 9, // 运行成功
}ALIPAY_IOT_OTA_EVENT_TYPE;

/**
* APP报告OTA进展的消息接口，通过消息 MSG_ID_ALIPAY_SDK_OTA_EVENT_REPORT_IND
* 注意：若应用升级成功，需要在发送ALIPAY_IOT_OTA_COMPLETED之前注册一次新的版本号(alipay_iot_ota_app_register())
* 数据结构为：alipay_sdk_ota_event_report_struct,
* 数据创建函数为：alipay_iot_ota_event_report_ind_struct_create()
* 数据释放函数为：alipay_iot_ota_event_report_ind_struct_destory()
*/
typedef struct
{
    ALIPAY_IOT_OTA_EVENT_TYPE event_type;
    char* task_id;                          //job id, 用于区分任务
    char* target_id;
    char* from_version;
    char* to_version;
    char* msg;                              //期望服务端看到的可读文本，可以为NULL
}alipay_sdk_ota_event_report_ind_struct;

alipay_sdk_ota_event_report_ind_struct *alipay_iot_ota_event_report_ind_struct_create(ALIPAY_IOT_OTA_EVENT_TYPE event_type, char * task_id, char * target_id, char* from_version, char* to_version, char *msg);
void alipay_iot_ota_event_report_ind_struct_destory(alipay_sdk_ota_event_report_ind_struct *ind);


#endif
