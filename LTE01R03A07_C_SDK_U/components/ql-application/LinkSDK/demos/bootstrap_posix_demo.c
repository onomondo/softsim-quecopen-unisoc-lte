/* 这个例程演示了用SDK配置Bootstrap会话实例的参数, 并发起请求和接收应答, 之后
 *
 * + 如果接收应答失败了, 销毁实例, 回收资源, 结束程序退出
 * + 如果接收应答成功, 在`demo_bootstrap_recv_handler()`的应答处理回调函数中, 演示解析获取服务端应答的内容
 *
 * 需要用户关注或修改的部分, 已用 `TODO` 在注释中标明
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_bootstrap_api.h"
#include "aiot_mqtt_api.h"

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


#define bootstrap_profile_idx	1

typedef struct {
    uint32_t code;
    char *host;
    uint16_t port;
} demo_info_t;

/* 位于portfiles/aiot_port文件夹下的系统适配函数集合 */
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;

/* 位于external/ali_ca_cert.c中的服务器证书 */
extern const char *ali_ca_cert;

ql_task_t ql_alisdk_bootstrap_task_ref;
ql_task_t ql_alisdk_bootstrap_process_task_ref;
ql_task_t ql_alisdk_bootstrap_recv_task_ref;
static uint8_t g_bootstrap_process_task_running = 0;
static uint8_t g_bootstrap_recv_task_running = 0;


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

    ql_set_data_call_asyn_mode(nSim, bootstrap_profile_idx, 0);

    uart_printf("===start data call====");
    ret = ql_start_data_call(nSim, bootstrap_profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
    uart_printf("===data call result:%d", ret);
    if (ret != 0)
    {
        uart_printf("====data call failure!!!!=====");
    }
    memset(&info, 0x00, sizeof(ql_data_call_info_s));

    ret = ql_get_data_call_info(nSim, bootstrap_profile_idx, &info);
    if (ret != 0)
    {
        uart_printf("ql_get_data_call_info ret: %d", ret);
        ql_stop_data_call(nSim, bootstrap_profile_idx);
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
 * 例如: [1580829318.855][LK-040B] > POST /auth/bootstrap HTTP/1.1
 *
 * 上面这条日志的code就是040B(十六进制), code值的定义见components/bootstrap/aiot_bootstrap_api.h
 *
 */

/* 日志回调函数, SDK的日志会从这里输出 */
static int32_t demo_state_logcb(int32_t code, char *message)
{
    uart_printf("%s", message);
    return 0;
}

/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
static void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
        /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
        case AIOT_MQTTEVT_CONNECT: {
            uart_printf("AIOT_MQTTEVT_CONNECT\n");
        }
        break;

        /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
        case AIOT_MQTTEVT_RECONNECT: {
            uart_printf("AIOT_MQTTEVT_RECONNECT\n");
        }
        break;

        /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
        case AIOT_MQTTEVT_DISCONNECT: {
            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                          ("heartbeat disconnect");
            uart_printf("AIOT_MQTTEVT_DISCONNECT: %s\n", cause);
        }
        break;

        default: {

        }
    }
}

/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
static void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            uart_printf("heartbeat response\n");
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
            uart_printf("suback, res: -0x%04X, packet id: %d, max qos: %d\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            uart_printf("pub, qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            uart_printf("pub, payload: %.*s\n", packet->data.pub.payload_len, packet->data.pub.payload);
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            uart_printf("puback, packet id: %d\n", packet->data.pub_ack.packet_id);
        }
        break;

        default: {

        }
    }
}

/* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息重发 */
static void demo_mqtt_process_thread(void *args)
{
    int32_t res = STATE_SUCCESS;

    while (g_bootstrap_process_task_running) {
        res = aiot_mqtt_process(args);
        if (res == STATE_USER_INPUT_EXEC_DISABLED) {
            break;
        }
        ql_rtos_task_sleep_s(1);
    }
}

/* 执行aiot_mqtt_recv的线程, 包含网络自动重连和从服务器收取MQTT消息 */
static void demo_mqtt_recv_thread(void *args)
{
    int32_t res = STATE_SUCCESS;

    while (g_bootstrap_recv_task_running) {
        res = aiot_mqtt_recv(args);
        if (res < STATE_SUCCESS) {
            if (res == STATE_USER_INPUT_EXEC_DISABLED) {
                break;
            }
            ql_rtos_task_sleep_s(1);
        }
    }
}

static int32_t demo_mqtt_start(void *mqtt_handle, char *product_key, char *device_name, char *device_secret, char *host, uint16_t port)
{
    int32_t     res = STATE_SUCCESS;
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
    cred.x509_server_cert = ali_ca_cert;                 /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);     /* 用来验证MQTT服务端的RSA根证书长度 */

    /* TODO: 如果以下代码不被注释, 则例程会用TCP而不是TLS连接云平台 */
    
    {
        memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
        cred.option = AIOT_SYSDEP_NETWORK_CRED_NONE;
    }
    

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
	int time_delay_ms = 5000;
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_CONNECT_TIMEOUT_MS, (void *)&time_delay_ms);
	/* 配置拨号profile_idx信息(quectel平台必须执行此操作)*/
	int pdp = bootstrap_profile_idx;
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PROFILE_IDX, (void *)&pdp);


    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_mqtt_deinit(&mqtt_handle);
        uart_printf("aiot_mqtt_connect failed: -0x%04X\n", -res);
        return -1;
    }

    /* 创建一个单独的线程, 专用于执行aiot_mqtt_process, 它会自动发送心跳保活, 以及重发QoS1的未应答报文 */
    g_bootstrap_process_task_running = 1;
    res = ql_rtos_task_create(&ql_alisdk_bootstrap_process_task_ref, 16 * 1024, 24, "client_proc", demo_mqtt_process_thread, (void*)mqtt_handle, 5);

    if (res < 0) {
        uart_printf("pthread_create demo_mqtt_process_thread failed: %d\n", res);
        aiot_mqtt_deinit(&mqtt_handle);
        return -1;
    }

    /* 创建一个单独的线程用于执行aiot_mqtt_recv, 它会循环收取服务器下发的MQTT消息, 并在断线时自动重连 */
    g_bootstrap_recv_task_running = 1;
    res = ql_rtos_task_create(&ql_alisdk_bootstrap_recv_task_ref, 16 * 1024, 25, "recv_thread", demo_mqtt_recv_thread, (void*)mqtt_handle, 5);
    if (res < 0) {
        uart_printf("pthread_create demo_mqtt_recv_thread failed: %d\n", res);
        g_bootstrap_process_task_running = 0;
        ql_rtos_task_delete(ql_alisdk_bootstrap_process_task_ref);
        aiot_mqtt_deinit(&mqtt_handle);
        return -1;
    }

    return 0;
}

static int32_t demo_mqtt_stop(void **handle)
{
    int32_t res = STATE_SUCCESS;
    void *mqtt_handle = NULL;

    mqtt_handle = *handle;

    g_bootstrap_process_task_running = 0;
    g_bootstrap_recv_task_running = 0;
    ql_rtos_task_delete(ql_alisdk_bootstrap_process_task_ref);
    ql_rtos_task_delete(ql_alisdk_bootstrap_recv_task_ref);

    /* 断开MQTT连接 */
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        uart_printf("aiot_mqtt_disconnect failed: -0x%04X\n", -res);
        return -1;
    }

    /* 销毁MQTT实例 */
    res = aiot_mqtt_deinit(&mqtt_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_mqtt_deinit failed: -0x%04X\n", -res);
        return -1;
    }

    return 0;
}

/* Bootstrap事件回调函数, 当SDK发生内部事件时被调用 */
static void demo_bootstrap_event_handler(void *handle, const aiot_bootstrap_event_t *event, void *userdata)
{
    switch (event->type) {
        case AIOT_BOOTSTRAPEVT_INVALID_RESPONSE: {
            uart_printf("AIOT_BOOTSTRAPEVT_INVALID_RESPONSE\n");
        }
        break;
        case AIOT_BOOTSTRAPEVT_INVALID_CMD: {
            uart_printf("AIOT_BOOTSTRAPEVT_INVALID_CMD\n");
        }
        break;
        default: {

        }
        break;
    }
}

/* Bootstrap接收消息处理回调, 当SDK从服务器收到Bootstrap消息时被调用 */
static void demo_bootstrap_recv_handler(void *handle, const aiot_bootstrap_recv_t *packet, void *userdata)
{
    demo_info_t *demo_info = (demo_info_t *)userdata;

    switch (packet->type) {
        case AIOT_BOOTSTRAPRECV_STATUS_CODE: {
            demo_info->code = packet->data.status_code.code;
        }
        break;
        case AIOT_BOOTSTRAPRECV_CONNECTION_INFO: {
            demo_info->host = malloc(strlen(packet->data.connection_info.host) + 1);
            if (demo_info->host != NULL) {
                memset(demo_info->host, 0, strlen(packet->data.connection_info.host) + 1);
                /* TODO: 回调中需要将packet指向的空间内容复制保存好, 因为回调返回后, 这些空间就会被SDK释放 */
                memcpy(demo_info->host, packet->data.connection_info.host, strlen(packet->data.connection_info.host));
                demo_info->port = packet->data.connection_info.port;
            }
        }
        break;
        case AIOT_BOOTSTRAPRECV_NOTIFY: {
            uart_printf("AIOT_BOOTSTRAPRECV_NOTIFY, cmd: %d\n", packet->data.notify.cmd);

        }
        default: {

        }
        break;
    }
}

static void  ql_alisdk_boostrap_task(void *arg)
{
    int32_t res = STATE_SUCCESS;
    void *bootstrap_handle = NULL, *mqtt_handle = NULL;
    aiot_sysdep_network_cred_t cred;
    demo_info_t demo_info;

    /* TODO: 替换为自己设备的productKey和deviceName */
    char *product_key = "a1TSu2F29co";
    char *device_name = "000001";
    char *device_secret = "bd220c3a9c6a5b3f5921485d091497e1";

    memset(&demo_info, 0, sizeof(demo_info_t));

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

    /* 创建1个Bootstrap会话实例并内部初始化默认参数 */
    bootstrap_handle =  aiot_bootstrap_init();
    if (bootstrap_handle == NULL) {
        uart_printf("aiot_bootstrap_init failed\n");
        goto exit;
    }

    /* 配置Bootstrap会话实例
     *
     * 配置项详细说明可参考: http://gaic.alicdn.com/ztms/linkkit/html/aiot__bootstrap__api_8h.html#a9cc177610fa0842f8287ed4fa438d56d
     *
     */
    uint16_t bootstrap_pdp = bootstrap_profile_idx;
	res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_PROFILE_IDX, (void *)&bootstrap_pdp);
		if (res < STATE_SUCCESS) {
			uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_PROFILE_IDX failed, res: -0x%04X\n", -res);
			aiot_bootstrap_deinit(&bootstrap_handle);
			goto exit;
		}
    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_PRODUCT_KEY, (void *)product_key);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_PRODUCT_KEY failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_DEVICE_NAME, (void *)device_name);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_DEVICE_NAME failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_NETWORK_CRED, (void *)&cred);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_NETWORK_CRED failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_RECV_HANDLER, (void *)demo_bootstrap_recv_handler);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_RECV_HANDLER failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_EVENT_HANDLER, (void *)demo_bootstrap_event_handler);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_EVENT_HANDLER failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_USERDATA, (void *)&demo_info);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_USERDATA failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        goto exit;
    }

    /* 发送Bootstrap服务请求 */
    res = aiot_bootstrap_send_request(bootstrap_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_send_request failed, res: -0x%04X\n", -res);
        goto exit;
    }

    /* 接收Bootstrap服务应答 */
    res = aiot_bootstrap_recv(bootstrap_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_recv failed, res: -0x%04X\n", -res);
        goto exit;
    }

    uart_printf("status code: %d\n", demo_info.code);

    /* 把服务应答中的MQTT域名地址和端口号打印出来 */
    if (demo_info.host != NULL) {
        uart_printf("host: %s, port: %d\n", demo_info.host, demo_info.port);
    }else{
       goto exit;
    }

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        uart_printf("aiot_mqtt_init failed\n");
       goto exit;
    }

    /* 建立MQTT连接, 并开启保活线程和接收线程 */
    res = demo_mqtt_start(mqtt_handle, product_key, device_name, device_secret, demo_info.host, demo_info.port);
    free(demo_info.host);
    if (res < 0) {
        uart_printf("demo_mqtt_start failed\n");
        goto exit;
    }

    res = aiot_bootstrap_setopt(bootstrap_handle, AIOT_BOOTSTRAPOPT_MQTT_HANDLE, (void *)mqtt_handle);
    if (res < STATE_SUCCESS) {
        uart_printf("aiot_bootstrap_setopt AIOT_BOOTSTRAPOPT_MQTT_HANDLE failed, res: -0x%04X\n", -res);
        aiot_bootstrap_deinit(&bootstrap_handle);
        demo_mqtt_stop(&mqtt_handle);
        goto exit;
    }

    while(1) {
        ql_rtos_task_sleep_s(1);
    }

    /* 销毁Bootstrap会话实例, 一般不会运行到这里 */
    res = aiot_bootstrap_deinit(&bootstrap_handle);
    if (res < 0) {
        uart_printf("demo_start_stop failed\n");
        goto exit;
    }

    /* 销毁MQTT实例, 退出线程, 一般不会运行到这里 */
    res = demo_mqtt_stop(&mqtt_handle);
    if (res < 0) {
        uart_printf("demo_start_stop failed\n");
        goto exit;
    }

    return;
		
exit:
	ql_rtos_task_delete(NULL);

}

#define QL_ALI_TASK_STACK_SIZE 10240
#define QL_ALI_TASK_PRIO APP_PRIORITY_NORMAL
#define QL_ALI_TASK_EVENT_CNT 5

int ql_ali_sdk_bootstrap_test_init(void)
{
	QlOSStatus   status;
    int count = 0;
    /* 主体进入休眠 */
    while (count < 10)
    {
        count++;
        ql_rtos_task_sleep_s(1);
        uart_printf("ql_ali_sdk_mqtt_client_test_init  -0x%04X\n", count);
    }
	status =ql_rtos_task_create(&ql_alisdk_bootstrap_task_ref, QL_ALI_TASK_STACK_SIZE, APP_PRIORITY_NORMAL, "ali_bootstart", ql_alisdk_boostrap_task, NULL, QL_ALI_TASK_EVENT_CNT);
	
    if (status != QL_OSI_SUCCESS)
    {
        uart_printf("ql_ali_sdk_bootstrap_test_init error");
        return -1;
    }
    
    return 0;
}


