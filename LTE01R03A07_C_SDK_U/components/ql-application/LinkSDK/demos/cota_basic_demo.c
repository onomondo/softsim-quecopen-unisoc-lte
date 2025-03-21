/*
 *  这个例子主要用于演示如何从设备端获取存储在云端的关于这个设备的远程配置
 *
 * 需要用户关注或修改的部分, 已用 `TODO` 在注释中标明
 *
 */


/* TODO: 本例子用到了sleep函数, 所以包含了unistd.h. 如果用户自己的库中有可以替代的函数, 则可以将unistd.h替换掉
 *
 */

#include <stdio.h>
#include <string.h>

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
#include "aiot_ota_api.h"

#include "ali_demo.h"
#include "ql_api_osi.h"
#include "ql_api_nw.h"
#include "ali_demo.h"
#include "ql_log.h"
#include "ql_api_datacall.h"
#include "sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#include "lwip/netdb.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"

#define QL_SOCKET_LOG_LEVEL QL_LOG_LEVEL_INFO
#define uart_printf(msg, ...) QL_LOG(QL_SOCKET_LOG_LEVEL, "linkSDK ", msg, ##__VA_ARGS__)


#define cota_model_profile_idx	1

/* TODO: 替换为自己设备的三元组 */
static char *product_key = "a1TSu2F29co";
static char *device_name = "000001";
static char *device_secret = "bd220c3a9c6a5b3f5921485d091497e1";

/* 位于external/ali_ca_cert.c中的服务器证书 */
extern const char *ali_ca_cert;
static void *g_dl_handle = NULL;
ql_task_t ql_alisdk_cota_task_ref;


/* 位于portfiles/aiot_port文件夹下的系统适配函数集合 */
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;

static int datacall_satrt(void)
{
    int ret = 0;
    int i = 0;
    ql_data_call_info_s info;
    char ip4_addr_str[16] = {0};
    uint8_t nSim = 0;

    ql_rtos_task_sleep_s(10);
    uart_printf("========== socket demo start ==========");
    uart_printf("wait for network register done");

    while ((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10)
    {
        i++;
        ql_rtos_task_sleep_s(1);
    }
    if (ret == 0)
    {
        i = 0;
        uart_printf("====network registered!!!!====");
    }
    else
    {
        uart_printf("====network register failure!!!!!====");
        goto exit;
    }

    ql_set_data_call_asyn_mode(nSim, cota_model_profile_idx, 0);

    uart_printf("===start data call====");
    ret = ql_start_data_call(nSim, cota_model_profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
    uart_printf("===data call result:%d", ret);
    if (ret != 0)
    {
        uart_printf("====data call failure!!!!=====");
    }
    memset(&info, 0x00, sizeof(ql_data_call_info_s));

    ret = ql_get_data_call_info(nSim, cota_model_profile_idx, &info);
    if (ret != 0)
    {
        uart_printf("ql_get_data_call_info ret: %d", ret);
        ql_stop_data_call(nSim, cota_model_profile_idx);
        goto exit;
    }
    uart_printf("info->mqtt_profile_idx: %d", info.profile_idx);
    uart_printf("info->ip_version: %d", info.ip_version);

    uart_printf("info->v4.state: %d", info.v4.state);
    inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
    uart_printf("info.v4.addr.ip: %s\r\n", ip4_addr_str);

    inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
    uart_printf("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

    inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
    uart_printf("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

    return 0;
exit:
    return -1;
}


/* TODO: 如果要关闭日志, 就把这个函数实现为空, 如果要减少日志, 可根据code选择不打印
 *
 * 例如: [1578463098.611][LK-0309] pub: /ota/device/upgrade/a13FN5TplKq/ota_demo
 *
 * 上面这条日志的code就是0309(十六进制), code值的定义见core/aiot_state_api.h
 *
 */

/* 日志回调函数, SDK的日志会从这里输出 */
static int32_t demo_state_logcb(int32_t code, char *message)
{
    /* 下载远程配置的时候会有大量的HTTP收包日志, 通过code筛选出来关闭 */
    if (STATE_HTTP_LOG_RECV_CONTENT != code) {
        uart_printf("%s", message);
    }
    return 0;
}

/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
static void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *const event, void *userdata)
{
    switch (event->type) {
        /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
        case AIOT_MQTTEVT_CONNECT: {
            uart_printf("AIOT_MQTTEVT_CONNECT\r\n");
            /* TODO: 处理SDK建连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
        case AIOT_MQTTEVT_RECONNECT: {
            uart_printf("AIOT_MQTTEVT_RECONNECT\r\n");
            /* TODO: 处理SDK重连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
        case AIOT_MQTTEVT_DISCONNECT: {
            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                          ("heartbeat disconnect");
            uart_printf("AIOT_MQTTEVT_DISCONNECT: %s\r\n", cause);
            /* TODO: 处理SDK被动断连, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;
        default: {

        }
    }
}

/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
static void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *const packet, void *userdata)
{
    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            uart_printf("heartbeat response\r\n");
            /* TODO: 处理服务器对心跳的回应, 一般不处理 */
        }
        break;
        case AIOT_MQTTRECV_SUB_ACK: {
            uart_printf("suback, res: -0x%04X, packet id: %d, max qos: %d\r\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
            /* TODO: 处理服务器对订阅请求的回应, 一般不处理 */
        }
        break;
        case AIOT_MQTTRECV_PUB: {
            uart_printf("pub, qos: %d, topic: %d %s\r\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            uart_printf("pub, payload: %d %s\r\n", packet->data.pub.payload_len, packet->data.pub.payload);
            /* TODO: 处理服务器下发的业务报文 */
        }
        break;
        case AIOT_MQTTRECV_PUB_ACK: {
            uart_printf("puback, packet id: %d\r\n", packet->data.pub_ack.packet_id);
            /* TODO: 处理服务器对QoS1上报消息的回应, 一般不处理 */
        }
        break;
        default: {

        }
    }
}

/* 下载收包回调, 用户调用 aiot_download_recv() 后, SDK收到数据会进入这个函数, 把下载到的数据交给用户 */
/* TODO: 在用户收到远程配置的数据后, 需要自己决定如何处理 */
static void demo_download_recv_handler(void *handle, const aiot_download_recv_t *packet, void *userdata)
{
    /* 目前只支持 packet->type 为 AIOT_DLRECV_HTTPBODY 的情况 */
    if (!packet || AIOT_DLRECV_HTTPBODY != packet->type) {
        return;
    }
    int32_t percent = packet->data.percent;
    uint8_t *src_buffer = packet->data.buffer;
    uint32_t src_buffer_len = packet->data.len;

    /* 如果 percent 为负数, 说明发生了收包异常或者digest校验错误 */
    if (percent < 0) {
        /* 收包异常或者digest校验错误 */
        uart_printf("exception happend, percent is %d\r\n", percent);
        return;
    }
    aiot_download_report_progress(handle, percent);
    uart_printf("config len is %d, config content is %.*s\r\n", src_buffer_len, src_buffer_len, (char *)src_buffer);
}

/* 用户通过 aiot_ota_setopt() 注册的OTA消息处理回调, 如果SDK收到了OTA相关的MQTT消息, 会自动识别, 调用这个回调函数 */
static void demo_ota_recv_handler(void *ota_handle, aiot_ota_recv_t *ota_msg, void *userdata)
{
    switch (ota_msg->type) {
        case AIOT_OTARECV_COTA: {
            uint16_t port = 443;
            uint32_t max_buffer_len = 2048;
            aiot_sysdep_network_cred_t cred;
            void *dl_handle = aiot_download_init();

            if (NULL == dl_handle || NULL == ota_msg->task_desc) {
                return;
            }

            uart_printf("configId: %s, configSize: %u Bytes\r\n", ota_msg->task_desc->version,
                   ota_msg->task_desc->size_total);

            memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
            cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
            cred.max_tls_fragment = 16384;
            cred.x509_server_cert = ali_ca_cert;
            cred.x509_server_cert_len = strlen(ali_ca_cert);

            /* 设置下载时为TLS下载 */
            if ((STATE_SUCCESS != aiot_download_setopt(dl_handle, AIOT_DLOPT_NETWORK_CRED, (void *)(&cred))) ||
                /* 设置下载时访问的服务器端口号 */
                (STATE_SUCCESS != aiot_download_setopt(dl_handle, AIOT_DLOPT_NETWORK_PORT, (void *)(&port))) ||
                /* 设置下载的任务信息, 通过输入参数 ota_msg 中的 task_desc 成员得到, 内含下载地址, 远程配置的大小和版本号*/
                (STATE_SUCCESS != aiot_download_setopt(dl_handle, AIOT_DLOPT_TASK_DESC, (void *)(ota_msg->task_desc))) ||
                /* 设置下载内容到达时, SDK将调用的回调函数 */
                (STATE_SUCCESS != aiot_download_setopt(dl_handle, AIOT_DLOPT_RECV_HANDLER, (void *)(demo_download_recv_handler))) ||
                /* 设置单次下载最大的buffer长度, 每当这个长度的内存读满了后会通知用户 */
                (STATE_SUCCESS != aiot_download_setopt(dl_handle, AIOT_DLOPT_BODY_BUFFER_MAX_LEN, (void *)(&max_buffer_len))) ||
                /* 发送http的GET请求给http服务器 */
                (STATE_SUCCESS != aiot_download_send_request(dl_handle))) {
                    aiot_download_deinit(&dl_handle);
                    break;
            }
            g_dl_handle = dl_handle;
            break;
        }
        default:
            break;
    }
}


static void  ql_alisdk_cota_task(void *arg)
{
    int32_t res = STATE_SUCCESS;
    void *mqtt_handle = NULL;
    char *url = "iot-as-mqtt.cn-shanghai.aliyuncs.com"; /* 阿里云平台上海站点的域名后缀 */
    char host[100] = {0}; /* 用这个数组拼接设备连接的云平台站点全地址, 规则是 ${productKey}.iot-as-mqtt.cn-shanghai.aliyuncs.com */
    uint16_t port = 443; /* 无论设备是否使用TLS连接阿里云平台, 目的端口都是443 */
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
    void *ota_handle = NULL;
    uint32_t timeout_ms = 0;
	int time_delay_ms = 5000;
	int pdp = cota_model_profile_idx;

	/*执行拨号操作，设备联网*/
	if(datacall_satrt() != 0)
		goto exit;

    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);

    /* 配置SDK的日志输出 */
    aiot_state_set_logcb(demo_state_logcb);

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
    cred.x509_server_cert = ali_ca_cert;                 /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);     /* 用来验证MQTT服务端的RSA根证书长度 */

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        return;
    }

    snprintf(host, 100, "%s.%s", product_key, url);
    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)host);
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    /* 配置MQTT默认消息接收回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)demo_mqtt_default_recv_handler);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)demo_mqtt_event_handler);
	
	/* 配置MQTT连接超时时间 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_CONNECT_TIMEOUT_MS, (void *)&time_delay_ms);
	/* 配置拨号profile_idx信息(quectel平台必须执行此操作)*/
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PROFILE_IDX, (void *)&pdp);

    /* 与MQTT例程不同的是, 这里需要增加创建OTA会话实例的语句 */
    ota_handle = aiot_ota_init();
    if (NULL == ota_handle) {
        goto exit;
    }

    /* 用以下语句, 把OTA会话和MQTT会话关联起来 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_MQTT_HANDLE, mqtt_handle);
    /* 用以下语句, 设置OTA会话的数据接收回调, SDK收到OTA相关推送时, 会进入这个回调函数 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_RECV_HANDLER, demo_ota_recv_handler);

    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_mqtt_connect failed: -0x%04X\r\n", -res);
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        goto exit;
    }

    /* 发送请求, 获取云端的远程配置 */
    {
        char *topic_string = "/sys/a1dhWKuKqX5/cota_basic_demo/thing/config/get";
        char *payload_string = "{\"id\":\"123\",\"params\":{\"configScope\":\"product\",\"getType\":\"file\"}}";

        res = aiot_mqtt_pub(mqtt_handle, topic_string, (uint8_t *)payload_string, strlen(payload_string), 0);
        if (res < STATE_SUCCESS) {
            uart_printf("aiot_mqtt_pub failed: -0x%04X\r\n", -res);
            /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
            goto exit;
        }
    }

    while (1) {
        aiot_mqtt_process(mqtt_handle);
        res = aiot_mqtt_recv(mqtt_handle);

        if (res == STATE_SYS_DEPEND_NWK_CLOSED) {
            ql_rtos_task_sleep_s(1);
        }
        if (NULL != g_dl_handle) {
            /* 完成远程配置的接收前, 将mqtt的收包超时调整到100ms, 以减少两次远程配置的下载间隔*/
            int32_t ret = aiot_download_recv(g_dl_handle);
            timeout_ms = 100;

            if (STATE_DOWNLOAD_FINISHED == ret) {
                aiot_download_deinit(&g_dl_handle);
                /* 完成远程配置的接收后, 将mqtt的收包超时调整回到默认值5000ms */
                timeout_ms = 5000;
            }
            aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_TIMEOUT_MS, (void *)&timeout_ms);
        }
    }

    /* 断开MQTT连接, 一般不会运行到这里 */
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_mqtt_disconnect failed: -0x%04X\r\n", -res);
        goto exit;
    }

exit:
    while (1) {
        /* 销毁MQTT实例, 一般不会运行到这里 */
        res = aiot_mqtt_deinit(&mqtt_handle);

        if (res < STATE_SUCCESS) {
            uart_printf("aiot_mqtt_deinit failed: -0x%04X\r\n", -res);
            return;
        } else {
            break;
        }
    }

    /* 销毁OTA实例, 一般不会运行到这里 */
    aiot_ota_deinit(&ota_handle);

    return ;
}

#define QL_ALI_TASK_STACK_SIZE 10240
#define QL_ALI_TASK_PRIO APP_PRIORITY_NORMAL
#define QL_ALI_TASK_EVENT_CNT 5

int ql_ali_sdk_cota_test_init(void)
{
	QlOSStatus   status;
    int count = 0;
    /* 主循环进入休眠 */
    while (count < 10)
    {
        count++;
        ql_rtos_task_sleep_s(1);
        uart_printf("ql_ali_sdk_mqtt_client_test_init  -0x%04X\n", count);
    }
    status =ql_rtos_task_create(&ql_alisdk_cota_task_ref, QL_ALI_TASK_STACK_SIZE, APP_PRIORITY_NORMAL, "ali_cota_task", ql_alisdk_cota_task, NULL, QL_ALI_TASK_EVENT_CNT);
	
    if (status != QL_OSI_SUCCESS)
    {
        uart_printf("ql_ali_sdk_cota_test_init error");
        return -1;
    }

    return 0;
}



