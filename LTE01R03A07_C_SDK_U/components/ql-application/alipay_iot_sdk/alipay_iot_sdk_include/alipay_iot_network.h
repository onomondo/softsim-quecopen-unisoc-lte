#ifndef _ALIPAY_IOT_NETWORK_H_
#define _ALIPAY_IOT_NETWORK_H_

#include "alipay_iot_kal.h"


#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef struct {
    char* name;
    char* value;
} HeaderItem;

typedef struct {
    HeaderItem* headers;
    unsigned int header_num;
} Headers;

typedef struct {
    int32_t   cmdType;
    char      *content;
}ALIAPY_IOT_GENERAL_MSG;


/**
* mqtt下行消息回调
*
* @param handle      注册回调时传入的handle
* @param topicName   mqtt topic name
* @param topicLen    topic name length
* @param message     message payload
*
* 说明: 1. 此回调内部需要过滤判断是否为自己的topic，如果不是，切勿操作，也不要做任何内存释放；
*
* @return  0
*/
typedef int alipay_iot_mqtt_messageArrived(void* handle, char* topicName, int topicLen, void* message);

typedef enum ALIPAY_IOT_MQTT_delivery_state {
    ALIPAY_IOT_MQTT_delivery_state_succ,      //succ。
    ALIPAY_IOT_MQTT_Network_state_fail,   //fail。
}ALIPAY_IOT_MQTT_delivery_state_t;

typedef int ALIPAY_IOT_MQTTClient_deliveryToken;

typedef void alipay_iot_mqtt_delivery_callback(void* handle, ALIPAY_IOT_MQTTClient_deliveryToken dt,ALIPAY_IOT_MQTT_delivery_state_t delivery_state, char* topic);

typedef struct {
    char *topicName;
    void *handle;
    int qos;
    alipay_iot_mqtt_messageArrived *ma;
}ALIPAY_IOT_MQTT_TOPIC_CB;

/**
 * This is a subscribe callback function.
 * @param count The number of topics for which the client is requesting
 * subscriptions.
 * @param handle handle maybe NULL
 * @param topic An array (of length <i>count</i>) of pointers to
 * topics, each of which may include wildcards.
 * @param qos An array (of length <i>count</i>) of @ref qos
 * values. qos[n] is the requested QoS for topic[n].
 * to this callback.
 */
typedef void alipay_iot_mqtt_subscribe_callback(void* handle, int count, char* const* topic, int* qos);

typedef enum ALIPAY_IOT_MQTT_Network_state {
    ALIPAY_IOT_MQTT_Network_state_broken,      //tcp broken
    ALIPAY_IOT_MQTT_Network_state_linking,     //tcp linking
    ALIPAY_IOT_MQTT_Network_state_connected,   //tcp connected
} ALIPAY_IOT_MQTT_Network_state_t;
/*
 * This is a network callback function，just tcp/ip.
 * @param handle A pointer to the <i>context</i> value originally passed to
 * alipay_iot_mqtt_client_register_callbacks(), which contains any application-specific handle.
 * @param state MQTT_Network_state
 */
typedef void alipay_iot_mqtt_network_callback(void* handle, ALIPAY_IOT_MQTT_Network_state_t state);

typedef enum ALIPAY_IOT_MQTT_CONNACK_CODE {
    ALIPAY_IOT_MQTT_CONNACK_CODE_ACCEPTED = 0,
    ALIPAY_IOT_MQTT_CONNACK_CODE_REFUSED_UNACCEPTABLE_PROTOCOL_VERSION = 1,
    ALIPAY_IOT_MQTT_CONNACK_CODE_REFUSED_IDENTIFIER_REJECTED = 2,
    ALIPAY_IOT_MQTT_CONNACK_CODE_REFUSED_SERVER_UNAVAILABLE = 3,
    ALIPAY_IOT_MQTT_CONNACK_CODE_REFUSED_BAD_USERDATA = 4,
    ALIPAY_IOT_MQTT_CONNACK_CODE_REFUSED_NOT_AUTHORIZED = 5
} ALIPAY_IOT_MQTT_CONNACK_CODE_t;

/*
* This is a mqtt connection callback function.
* @param handle A pointer to the <i>context</i> value originally passed to
* alipay_iot_mqtt_client_register_callbacks(), which contains any application-specific handle.
* @param state MQTT_CONNACK_CODE_t
*/
typedef void alipay_iot_mqtt_connack_callback(void* handle, ALIPAY_IOT_MQTT_CONNACK_CODE_t code);

/**
* mqtt回调注册接口
*
* @param handle         注册回调时传入的handle
* @param nc             网络连接是否ready
* @param cc             mqtt建链是否可用
* @param dc             暂时不用，传入NULL，后续扩展
* @param sc             下行订阅结果回调
* 说明: 1.一次上电过程只能调用一次，若不关注相关的回调，可以传入NULL。
       2.此接口注册的各个回调内部需要过滤判断是否为自己的topic，如果不是，切勿处理;
*
* @return  0:success; others: fail
*/
int alipay_iot_mqtt_client_register_callbacks(void* handle,
                                          alipay_iot_mqtt_network_callback* nc,
                                          alipay_iot_mqtt_connack_callback* cc,
                                          alipay_iot_mqtt_delivery_callback* dc,
                                          alipay_iot_mqtt_subscribe_callback* sc);


/**
* 订阅业务使用的下行mqtt topic
*
* @param handle         调用方传入的handle
* @param topic          topic 名称
* @param qos            订阅mqtt消息的qos: 0/1
* 说明: 1.qos需与服务端约定好，如果不一致，会采用较低值
*
* @return  0
*/
int alipay_iot_mqtt_client_subscribe(int count, ALIPAY_IOT_MQTT_TOPIC_CB pstTopicCB[]);

/**
* 在收到alipay_iot_mqtt_messageArrived回调后，调用本接口进行判断是否为自己的topic
*
* @param topicFromMA    alipay_iot_mqtt_messageArrived回调传入的topic
* @param topicSelf      自己关注的topic
*
* @return  0:success，表示匹配，其他值表示不匹配，不匹配时应该立刻返回不做任何处理
*/
int alipay_iot_mqtt_client_compare(const char *topicFromMA, const char *topicSelf);



/**
* 在收到alipay_iot_mqtt_messageArrived回调后，并且判断为自己需要的topic时，调用此接口进行unpack
*
* @param mqtt_message   alipay_iot_mqtt_messageArrived回调传入的message
* @param content        输出的unpack后的消息内容，消费完成后需要使用者释放pMsg->content内存
*
* @return  0:success
*/
int alipay_iot_mqtt_client_unpack(void *mqtt_message, ALIAPY_IOT_GENERAL_MSG *pMsg);

/**
* 上行mqtt消息发送接口
*
* @param handle         调用方自定义的handle
* @param topic          topic name
* @param qos            发送消息的qos值
* @param headers        消息配置
* @param pstMsg         消息内容
* @return  0:success, 其他值：错误
*/
int alipay_iot_mqtt_client_publish(void *handle, const char* topic, const int qos, Headers *headers, ALIAPY_IOT_GENERAL_MSG *pstMsg);

/**
* 上行mqtt消息发送接口
*
* @param errcode        0:rpc成功，其他值表示rpc错误
* @param pstResp        rpc响应，消费完成后，需释放pstResp->content内存
* @return
*/
typedef void alipay_iot_rpc_send_callback(int errcode, ALIAPY_IOT_GENERAL_MSG *pstResp);

/**
* 发送rpc消息，消费完后使用者需释放pstMsgSend和rsp的content内存
*
* @param operation      rpc operation
* @param headers        消息配置
* @param pstMsgSend     rpc消息内容
* @param callback       rpc响应回调
* @param timeout        rpc同步超时时间
* @return  本接口若返回非0，表示rpc出错，若返回0，具体rpc结果需要根据callback的errcode来判断。
*/
int alipay_iot_rpc_client_send(const char* operation, Headers *headers, ALIAPY_IOT_GENERAL_MSG *pstMsgSend, alipay_iot_rpc_send_callback *callback, unsigned int timeout);

#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
