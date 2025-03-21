#ifndef __H_ALIPAY_IOT_DISTRIBUTION
#define __H_ALIPAY_IOT_DISTRIBUTION
#ifdef __ALIPAY_IOT_DISTRIBUTION__



typedef enum
{
    APP_TYPE_NONE,             // default value
    APP_TYPE_NATIVE,           // native APP
    APP_TYPE_TINY,             // tiny APP
    APP_TYPE_LITE_TINY,        // Lite tiny APP
    APP_TYPE_MAX
}DISTRIBUTION_APP_TYPE;


/**
* logical application, ether tiny APP or native APP.
*/
typedef struct
{
    char*    app_id;            // 目标应用的id（包名）
    char*    version;           // 当前版本号
    char*    task_id;           // 云端分发任务ID
    uint32_t action;            // 1安装，-1卸载
    bool     is_main;           // 是否为主程序

    char*    target_id;         // 目标应用的id（包名）, 用途？
    char*    download_url;      // 下载地址
    char*    msg;               // 描述信息，最大长度128字节
    char*    flag;              // 端测提示标识（预留字段）
    uint64_t file_size;         // 分发应用包大小
    char*    md5;               // 分发应用包md5值，大小写？
    char*    app_action;        // 应用启动参数，对应开机配置的action
    char*    app_name;          // 应用名称
    DISTRIBUTION_APP_TYPE app_type;
}alipay_iot_distribution_app;

/**
* 分发内容查询消息:MSG_ID_ALIPAY_SDK_DISTRIBUTION_INFO_QUERY
* 会以MSG_ID_ALIPAY_SDK_DISTRIBUTION_INFO_IND作为回复.
*/

/**
* 分发内容更新：MSG_ID_ALIPAY_SDK_DISTRIBUTION_INFO_IND
* 数据为 alipay_iot_distribution_info_ind_struct，释放函数为 alipay_iot_distribution_info_ind_struct_destory().
*/
typedef struct
{
    int8_t                        count;
    alipay_iot_distribution_app** app_list;
}alipay_iot_distribution_info_ind_struct;

//释放 alipay_iot_distribution_info_ind_struct
void alipay_iot_distribution_info_ind_struct_destory(alipay_iot_distribution_info_ind_struct* ind);
#endif
#endif
