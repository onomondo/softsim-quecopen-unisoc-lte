#ifndef __H_ALIPAY_IOT_NEWS
#define __H_ALIPAY_IOT_NEWS
#include "alipay_iot_type.h"

/*
* 消息通知消息： MSG_ID_ALIPAY_SDK_NEWS_IND
* 数据为 alipay_sdk_news_ind_struct，释放函数为 alipay_iot_news_ind_struct_destory
* 此消息在极端情况下， 可能会连续发送 20 条， 需要交互的应用方注意预留消息队列大小
* 应用方收到此消息后，无论针对消息的内容支持与否，都需要通过 MSG_ID_ALIPAY_SDK_NEWS_CNF 来回复最终的操作结果
*/
typedef struct
{
    char*     news_id;         //消息 ID
    char*     news_route_key;  //消息路由key
    uint64_t  news_timestamp;  //生成时间-毫秒时间戳
    char*     news_content;    //消息内容
    char*     ext_info;        //扩展字段
}alipay_sdk_news_ind_struct;
//释放 alipay_sdk_news_ind_struct
void alipay_iot_news_ind_struct_destory(alipay_sdk_news_ind_struct* ind);

/*
* 指令执行结果报告消息： MSG_ID_ALIPAY_SDK_NEWS_CNF
* 数据为 alipay_sdk_news_cnf_struct，构造函数为 alipay_iot_news_cnf_struct_create
*/
typedef struct
{
    char*     news_id;        //消息 ID
    char*     ext_info;       //扩展字段
    bool      success;        //消息消费成功失败
    char*     err_cod;        //消息消费业务错误返回码
    char*     err_msg;        //消息消费业务错误原因
}alipay_sdk_news_cnf_struct;
//构造 alipay_sdk_news_cnf_struct
alipay_sdk_news_cnf_struct* alipay_iot_news_cnf_struct_create(char* news_id,char* ext_info,bool success, char* err_cod, char* err_msg);
//释放alipay_sdk_news_cnf_struct. 通常不需要释放，但在消息发送失败等情形时需要释放
void alipay_iot_news_cnf_struct_destory(alipay_sdk_news_cnf_struct* cnf);

#endif
