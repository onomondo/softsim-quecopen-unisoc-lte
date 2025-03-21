/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"

#include "ql_gpio.h"

#include "wifi_demo_macro.h"

#include "ql_api_datacall.h"
#include "ql_api_nw.h"
#include "ql_api_wifi.h"
#include "ql_power.h"

#include "sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/apps/lwiperf.h"

#if defined QL_WIFI_FC41D || defined QL_WIFI_FCM360W || defined QL_WIFI_FGM842D
#define QL_WIFI_DEVICE
#else
#undef QL_WIFI_DEVICE
#endif

#ifdef QL_WIFI_DEVICE
#include "ql_wifi_app.h"
#endif

/*========================================================================
 *	Macro Definition
 *========================================================================*/

#define QL_WIFI_DEMO_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_DEMO_LOG(msg, ...) QL_LOG(QL_WIFI_DEMO_LOG_LEVEL, "phy_demo", msg, ##__VA_ARGS__)

#define QL_WIFI_DEMO_MSG_QUEUE_CNT 10

#define QL_WIFI_DEMO_DISCONNECT_RETRY_TIME (20 * 1000)
#define QL_WIFI_DEMO_RETRY_CNT_MAX 3

#define QL_WIFI_DEMO_SIM_ID 0
#define QL_WIFI_DEMO_PROFILE_IDX 5
#define QL_WIFI_DEMO_SOCKET_BUF_SIZE 128

#define QL_WIFI_DEMO_TCP_CLIENTS_CNT 1

#define QL_WIFI_USE_BLE_CONFIG 0 //是否使用蓝牙配置wifi的ssid & pwd,目前仅FGM842D支持

#if QL_WIFI_USE_BLE_CONFIG && !defined(QL_WIFI_FGM842D)
#error "Only FGM842D supported"
#endif
/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    WIFI_DEMO_RETRY_CONNECT_START = 0,
    WIFI_DEMO_RETRY_CONNECT_END,
} wifi_demo_retry_state_e;

typedef enum
{
    WIFI_DEMO_EVENT_INIT = 0,
    WIFI_DEMO_EVENT_TRY_CONNECT,
    WIFI_DEMO_EVENT_CONNECTED,
    WIFI_DEMO_EVENT_DISCONNECT,
    WIFI_DEMO_EVENT_APP_TCP_CONNECT,
    WIFI_DEMO_EVENT_APP_TCP_DISCONNECT,
    WIFI_DEMO_EVENT_GET_STATUS,
#if QL_WIFI_USE_BLE_CONFIG
    WIFI_DEMO_EVENT_BLE_INIT,
    WIFI_DEMO_EVENT_BLE_GET_WIFI_CONFIG,
#endif
} wifi_demo_event_e;

/*========================================================================
 *  Type Definition
 *=========================================================================*/
#ifdef QL_WIFI_DEVICE
typedef struct
{
    int id;
    int fd;
    ql_task_t task;
    uint8_t close_flag;
} wifi_demo_tcp_client_s;
#endif

typedef struct
{
    ql_task_t task;
#ifdef QL_WIFI_DEVICE
    uint8_t sim_id;
    int profile_idx;
    ql_wifi_state_e state;
    ql_queue_t msg_queue;
    ql_timer_t retry_timer;
    wifi_demo_retry_state_e retry_state;
    int retry_cnt;
    // tcp client
    wifi_demo_tcp_client_s tcp_client[QL_WIFI_DEMO_TCP_CLIENTS_CNT];
    int tcp_client_cnt;
#endif
} wifi_demo_manager_s;

typedef struct
{
    uint32_t id;
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
} wifi_demo_event_s;
/*========================================================================
 *   Global Variable
 *========================================================================*/
wifi_demo_manager_s wifi_demo_manager = {0};

#ifdef QL_WIFI_DEVICE
ql_mutex_t wifi_demo_mutex = NULL;

static char send_buf[QL_WIFI_DEMO_SOCKET_BUF_SIZE] = {0};
static int send_len = 0;
static char recv_buf[QL_WIFI_DEMO_SOCKET_BUF_SIZE] = {0};
static int recv_len = 0;
#endif

/*========================================================================
 *	function Definition
 *========================================================================*/
#ifdef QL_WIFI_DEVICE
void wifi_demo_send_event(ql_task_t task, uint32_t id, uint32_t param1, uint32_t param2, uint32_t param3)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    ql_queue_t *msg_queue = NULL;
    if (task == manager->task)
    {
        msg_queue = manager->msg_queue;
    }
    else
    {
        return;
    }

    QlOSStatus err = QL_OSI_SUCCESS;
    wifi_demo_event_s event = {.id = id, .param1 = param1, .param2 = param2, .param3 = param3};
    err = ql_rtos_queue_release(msg_queue, sizeof(wifi_demo_event_s), (uint8 *)&event, QL_NO_WAIT);
    if (err)
    {
        QL_WIFI_DEMO_LOG("release failed");
    }
    return;
}

static void wifi_demo_retry_timer_callback(void *ctx)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("timer retry");
    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
    return;
}

static void wifi_demo_connect_status(void* ctx)
{
    uint32_t value = *((uint32_t*)ctx);
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("wifi connect %d", value);
    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_GET_STATUS, value, 0, 0);
    return;
}

#if QL_WIFI_USE_BLE_CONFIG
static void wifi_demo_ble_gatt_data_callback(void* ctx)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    /* 
    ctx的数据是 ql_ble_send_data_info 结构体
    wifi的ssid和pwd位于ql_ble_send_data_info结构体中的data成员里, data的数据格式为:|ssid_len|ssid|pwd_len|pwd| 
    */

    /*获取ql_ble_send_data_info中的uuid_len*/
    uint8_t *ptr = (uint8_t *)ctx;
    uint16_t uuid_len = *ptr | (*(ptr + 1)) << 8;
    if(uuid_len != 2 && uuid_len != 16){
        QL_WIFI_DEMO_LOG("uuid_len:%d err",uuid_len);
        return;
    }

    /*获取ql_ble_send_data_info中的uuid*/
    uint8_t uuid[16] = {0};
    ptr += 2;
    memcpy(uuid, ptr, 16);

    /*获取ql_ble_send_data_info中的data_len*/
    ptr += 16;
    uint16_t data_len = *ptr | (*(ptr + 1)) << 8;
    //GATT的最大数据长度为MTU-3 = 23-3=20
    if(data_len > 20){
        QL_WIFI_DEMO_LOG("data_len:%d err",data_len);
        return;
    }

    //获取ql_ble_send_data_info中的data中的ssid_len
    ptr += 2;
    uint8_t ssid_len = *ptr++;
    
    if(ssid_len > 18){
        QL_WIFI_DEMO_LOG("ssid_len:%d err",ssid_len);
        return;
    }

    //获取ql_ble_send_data_info中的data中的ssid
    ql_wifi_app_user_ap_info_s ap_user_info = {0};
    memcpy(ap_user_info.ssid, ptr, ssid_len);

    //获取ql_ble_send_data_info中的data中的pwd_len
    ptr += ssid_len;
    uint8_t pwd_len = *ptr++;
    if(ssid_len + pwd_len > 18){
        QL_WIFI_DEMO_LOG("ssid_len:%d + pwd_len:%d err",ssid_len,pwd_len);
        return;
    }

    //获取ql_ble_send_data_info中的data中的pwd_len
    memcpy(ap_user_info.password, ptr, pwd_len);

    //设置wifi的ssid和密码
    QL_WIFI_DEMO_LOG("ssid = %s, pwd=%s", ap_user_info.ssid, ap_user_info.password);
    ql_wifi_app_set_user_ap_info(&ap_user_info);

    /*回复OK*/
    ql_ble_send_data_info ble_send_data = {0};
    ble_send_data.uuid_len = uuid_len;
    memcpy(ble_send_data.uuid, uuid, uuid_len);
    ble_send_data.data_len = strlen("OK");
    strcpy((char *)&ble_send_data.data[0], "OK");
    ql_ble_sent_gatt_data(&ble_send_data);
            
    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
    return;
}
#endif

void wifi_demo_tcp_client_thread(void *arg)
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    wifi_demo_tcp_client_s *ctx = (wifi_demo_tcp_client_s *)arg;
    ctx->close_flag = 0;
    int ret = 0;
    uint8_t sim_id = manager->sim_id;
    int profile_idx = manager->profile_idx;
    ql_data_call_info_s info = {0};
    // tcp socket
    int i = 0;
    int flags = 0;
    int connected = 0;
    int socket_fd = -1;
    fd_set read_fds;
    fd_set write_fds;
    fd_set exp_fds;
    int fd_changed = 0;
    int closing = false;
    char ip4_addr_str[16] = {0};
    struct sockaddr_in local4, server_ipv4;

    uint8_t destip[] = {220, 180, 239, 212};
    uint16_t destport = 8305;
    ip4_addr_t int_destip;
    IP4_ADDR(&int_destip, destip[0], destip[1], destip[2], destip[3]);

    ret = ql_get_data_call_info(sim_id, profile_idx, &info);
    if (ret != QL_DATACALL_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("datacall get err");
        goto exit;
    }
    QL_WIFI_DEMO_LOG("info->profile_idx: %d", info.profile_idx);
    QL_WIFI_DEMO_LOG("info->ip_version: %d", info.ip_version);

    QL_WIFI_DEMO_LOG("info->v4.state: %d", info.v4.state);
    inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
    QL_WIFI_DEMO_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

    inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
    QL_WIFI_DEMO_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

    inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
    QL_WIFI_DEMO_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

    memset(&local4, 0x00, sizeof(struct sockaddr_in));
    local4.sin_family = AF_INET;
    local4.sin_port = 0;
    inet_aton(ip4addr_ntoa(&info.v4.addr.ip), &local4.sin_addr);

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exp_fds);

    QL_WIFI_DEMO_LOG("socket start!");
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        goto exit;
    }
    ret = bind(socket_fd,(struct sockaddr *)&local4,sizeof(struct sockaddr));
    if(ret < 0)
    {
        close(socket_fd);
        socket_fd = -1;
        goto exit;
    }
    flags |= O_NONBLOCK;
    fcntl(socket_fd, F_SETFL,flags);
    memset(&server_ipv4, 0x00, sizeof(struct sockaddr_in));
    server_ipv4.sin_family = AF_INET;
    server_ipv4.sin_port = htons(destport);
    memcpy(&(server_ipv4.sin_addr),&int_destip,sizeof(int_destip));	
    ret = connect(socket_fd, (struct sockaddr *)&server_ipv4, sizeof(server_ipv4));	
    if(ret == 0)
    {
        connected = 1;
    }
    else
    {
        if(lwip_get_error(socket_fd) != EINPROGRESS)
        {
            close(socket_fd);
            socket_fd = -1;
            goto exit;
        }
    }

    if (connected == 1)
    {
        QL_WIFI_DEMO_LOG("tcp connected");
        ql_rtos_mutex_try_lock(wifi_demo_mutex);
        ctx->fd = socket_fd;
        ql_rtos_mutex_unlock(wifi_demo_mutex);
        char con[128] = {0};
        int len = 0;
        FD_SET(socket_fd, &read_fds);
        len = snprintf(con, 128, "tcp connected");

        if (write(socket_fd, con, len) != len)
        {
            FD_SET(socket_fd, &write_fds);
        }
    }
    else
    {
        FD_SET(socket_fd, &write_fds);
    }
    FD_SET(socket_fd, &exp_fds);
    while (1)
    {
        fd_changed = select(socket_fd + 1, &read_fds, &write_fds, &exp_fds, NULL);
        if (fd_changed > 0)
        {
            if (FD_ISSET(socket_fd, &write_fds))
            {
                FD_CLR(socket_fd, &write_fds);
                if (connected == 0)
                {
                    int value = 0;
                    char con[128] = {0};
                    int len = sizeof(value);
                    getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &value, &len);
                    if (value == 0 || value == EISCONN)
                    {
                        QL_WIFI_DEMO_LOG("tcp connected");
                        ql_rtos_mutex_try_lock(wifi_demo_mutex);
                        ctx->fd = socket_fd;
                        ql_rtos_mutex_unlock(wifi_demo_mutex);
                        connected = 1;
                        FD_SET(socket_fd, &read_fds);
                        len = snprintf(con, 128, "tcp connected");
                        write(socket_fd, con, len);
                    }
                    else
                    {
                        QL_WIFI_DEMO_LOG("tcp connect failed");
                        close(socket_fd);
                        socket_fd = -1;
                        break;
                    }
                }
                else
                {
                    memset(send_buf, 0x00, 128);
                    send_len = snprintf(send_buf, 128, "%d%s%d\r\n", i, "startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend", i);
                    write(socket_fd, send_buf, send_len);
                    i++;
                }
            }
            else if (FD_ISSET(socket_fd, &read_fds))
            {
                FD_CLR(socket_fd, &read_fds);
                memset(recv_buf, 0x00, 128);
                recv_len = read(socket_fd, recv_buf, 128);
                if (recv_len > 0)
                {
                    QL_WIFI_DEMO_LOG("recv: %s", recv_buf);
                    memset(send_buf, 0x00, 128);
                    send_len = snprintf(send_buf, 128, "%d%s%d\r\n", i, "startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend", i);
                    write(socket_fd, send_buf, send_len);
                    i++;
                    FD_SET(socket_fd, &read_fds);
                }
                else if (recv_len == 0)
                {
                    if (closing == false)
                    {
                        QL_WIFI_DEMO_LOG("shut down");
                        shutdown(socket_fd, SHUT_WR);
                        closing = true;
                        FD_SET(socket_fd, &read_fds);
                    }
                    else
                    {
                        close(socket_fd);
                        socket_fd = -1;
                        break;
                    }
                }
                else
                {
                    if (lwip_get_error(socket_fd) == EAGAIN)
                    {
                        FD_SET(socket_fd, &read_fds);
                    }
                    else
                    {
                        close(socket_fd);
                        socket_fd = -1;
                        break;
                    }
                }
            }
            else if (FD_ISSET(socket_fd, &exp_fds))
            {
                FD_CLR(socket_fd, &exp_fds);
                close(socket_fd);
                socket_fd = -1;
                break;
            }
        }
    }
exit:
    QL_WIFI_DEMO_LOG("wifi tcp client %d end,%p", ctx->id, ctx->task);
    close(socket_fd);
    ql_rtos_mutex_try_lock(wifi_demo_mutex);
    memset(ctx, 0, sizeof(wifi_demo_tcp_client_s));
    ctx->id = -1;
    ctx->fd = -1;
    manager->tcp_client_cnt--;
    ql_rtos_mutex_unlock(wifi_demo_mutex);
    ql_rtos_task_delete(NULL);
}

static int wifi_demo_tcp_client_create()
{
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    wifi_demo_tcp_client_s *ctx = NULL;
    // User defined.Only support one client here.
    if (manager->tcp_client_cnt >= 1)
    {
        QL_WIFI_DEMO_LOG("only support one client");
        return -1;
    }
    // Find NULL task
    int i = 0;
    for (i = 0; i < QL_WIFI_DEMO_TCP_CLIENTS_CNT; i++)
    {
        ctx = &(manager->tcp_client[i]);
        QL_WIFI_DEMO_LOG("ctx->task %p", ctx->task);
        if (ctx->task == NULL)
        {
            break;
        }
    }

    if (i >= QL_WIFI_DEMO_TCP_CLIENTS_CNT)
    {
        QL_WIFI_DEMO_LOG("client full");
        return -1;
    }
    if (QL_OSI_SUCCESS != ql_rtos_task_create(&(ctx->task), 4 * 1024, APP_PRIORITY_NORMAL, "wifi_tcp_client", wifi_demo_tcp_client_thread, ctx, 5))
    {
        QL_WIFI_DEMO_LOG("task created failed");
        return -1;
    }
    ctx->id = i;
    ctx->fd = -1;
    manager->tcp_client_cnt++;
    return 0;
}

#endif
static void wifi_demo_thread(void *arg)
{
#ifdef QL_WIFI_DEVICE
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    QL_WIFI_DEMO_LOG("wifi demo task start");
    int ret = 0;
    uint8_t sim_id = QL_WIFI_DEMO_SIM_ID;
    int profile_idx = QL_WIFI_DEMO_PROFILE_IDX; // range 1 to 7

    /* Nat should be off in STA mode. */
    uint8_t reset_flag = false;
    uint32_t saved_list = 0;
    uint32_t target_list = ((sim_id << 8)| 0);
    ret = ql_datacall_get_nat(&saved_list);
    QL_WIFI_DEMO_LOG("get datacall saved nat ret: 0x%x, %d", ret, saved_list);
    if(0 != ret)
    {
        goto exit;
    }
    if(saved_list != target_list)
    {
        ret = ql_datacall_set_nat(target_list);
        QL_WIFI_DEMO_LOG("ql_datacall_set_nat err, ret=0x%x", ret);
        if(0 != ret)
        {
            goto exit;
        }
        /* NAT function reboot to take effect. */
        reset_flag = true;
    }
    if(reset_flag)
    {
        ql_power_reset(RESET_NORMAL);
    }

    char ssid_str[] = {"enp0s11u"};
    char password_str[] = {"l123654789"};

    // Initialize client ctx
    manager->tcp_client_cnt = 0;
    for (int i = 0; i < QL_WIFI_DEMO_TCP_CLIENTS_CNT; i++)
    {
        manager->tcp_client[i].id = -1;
        manager->tcp_client[i].fd = -1;
        manager->tcp_client[i].task = NULL;
    }

    // Create mutex
    ret = ql_rtos_mutex_create(&wifi_demo_mutex);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("create mutex ret: %x", ret);
        goto exit;
    }
    // Create timer
    ret = ql_rtos_timer_create(&(manager->retry_timer), NULL, wifi_demo_retry_timer_callback, NULL);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("create timer ret: %x", ret);
        goto exit;
    }

    // Create message queue
    ret = ql_rtos_queue_create(&(manager->msg_queue), sizeof(wifi_demo_event_s), QL_WIFI_DEMO_MSG_QUEUE_CNT);
    if (ret != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("msg queue ret: %x", ret);
        goto exit;
    }

    // wifi init
    ret = ql_wifi_app_init(NULL);
    if (ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("ql_wifi_app_init err %x", ret);
        goto exit;
    }
    QL_WIFI_DEMO_LOG("ql_wifi_app_init done");
    // enable dhcp
    char dhcp = 1;
    ret = ql_wifi_opt_set(QL_WIFI_OPT_DHCP, &dhcp, sizeof(char));
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("dhcp set err %x", ret);
        goto exit;
    }
    QL_WIFI_DEMO_LOG("QL_WIFI_OPT_DHCP done");
    // specify wifi cid
    ql_wifi_cid_s ctx =
        {
            .sim_id = sim_id,
            .cid = profile_idx,
            .enable = 1,
        };
    ret = ql_wifi_opt_set(QL_WIFI_OPT_CID, &ctx, sizeof(ql_wifi_cid_s));
    if (ret != QL_WIFI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("cid set err %x", ret);
        goto exit;
    }
    QL_WIFI_DEMO_LOG("QL_WIFI_OPT_CID done");
    manager->sim_id = sim_id;
    manager->profile_idx = profile_idx;

    // ssid & password
    ql_wifi_app_user_ap_info_s *ap_user_info = (ql_wifi_app_user_ap_info_s *)calloc(1,sizeof(ql_wifi_app_user_ap_info_s));
    if(!ap_user_info)
    {
        QL_WIFI_DEMO_LOG("calloc faild!");
        goto exit;
    }
    int ssid_len = strlen(ssid_str);
    int password_len = strlen(password_str);
    if(ssid_len >= QL_WIFI_SSID_MAX_LEN || password_len >= QL_WIFI_PASSWORD_MAX_LEN)
    {
        goto exit;
    }
    memcpy(ap_user_info->ssid,ssid_str,ssid_len);
    memcpy(ap_user_info->password,password_str,password_len);
    ret = ql_wifi_app_set_user_ap_info(ap_user_info);
    if(ret != QL_WIFI_APP_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("get ssid/password faild!");
        free(ap_user_info);
        goto exit;
    }
    free(ap_user_info);

    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_INIT, 0, 0, 0);
    while (1)
    {
        wifi_demo_event_s event = {0};
        if (QL_OSI_SUCCESS != ql_rtos_queue_wait(manager->msg_queue, (uint8 *)&event, sizeof(wifi_demo_event_s), QL_WAIT_FOREVER))
        {
            QL_WIFI_DEMO_LOG("queue wait error");
            goto exit;
        }
        QL_WIFI_DEMO_LOG("wifi_demo_thread %x", event.id);
        switch (event.id)
        {
        case WIFI_DEMO_EVENT_INIT: {
            // wifi software reset,better add hardware reset
            ret = ql_wifi_app_reset(NULL);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("reset err %x", ret);
                goto exit;
            }
            ql_rtos_task_sleep_ms(5000);
            QL_WIFI_DEMO_LOG("wifi init");
#if QL_WIFI_USE_BLE_CONFIG
            wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_BLE_INIT, 0, 0, 0);
#else
            wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
#endif
            break;
        }
#if QL_WIFI_USE_BLE_CONFIG
        case WIFI_DEMO_EVENT_BLE_INIT:{
            ql_ble_app_service_info ble_service = {0};
            ble_service.uuid_len = 2;
            ble_service.uuid[0] = 0xFF;
            ble_service.uuid[1] = 0xF1;
            ql_ble_init(&ble_service);

            ql_ble_app_service_info ble_characteristic = {0};
            ble_characteristic.uuid_len = 2;
            ble_characteristic.uuid[0] = 0xFF;
            ble_characteristic.uuid[1] = 0xF2;
            ql_ble_set_characteristic(&ble_characteristic);

            ql_ble_register_gatt_data_cb(wifi_demo_ble_gatt_data_callback);

            ql_ble_adv_start(NULL);
            break;
        }
#endif
        case WIFI_DEMO_EVENT_TRY_CONNECT: {
            manager->retry_cnt++;
            if (manager->retry_cnt >= QL_WIFI_DEMO_RETRY_CNT_MAX)
            {
                manager->retry_cnt = 0;
                wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_INIT, 0, 0, 0);
                break;
            }
            // start timer
            ret = ql_rtos_timer_start(manager->retry_timer, QL_WIFI_DEMO_DISCONNECT_RETRY_TIME, 0);
            QL_WIFI_DEMO_LOG("timer start %x", ret);
            if (ret != QL_OSI_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("manager->retry_timer start failed");
                wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
            }

            QL_WIFI_DEMO_LOG("wifi connect start");
            // start to connect wifi
            ret = ql_wifi_app_sta_disable(NULL);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("sta disable err %x", ret);
                goto exit;
            }
            ql_rtos_task_sleep_ms(10);
            ret = ql_wifi_app_sta_enable(wifi_demo_connect_status);
            if (ret != QL_WIFI_APP_SUCCESS)
            {
                QL_WIFI_DEMO_LOG("sta enable err %x", ret);
                goto exit;
            }
            QL_WIFI_DEMO_LOG("ql_wifi_app_sta_enable done");
            // Set retry_state end.When we get disconnect status,
            // we check this state for ignoring to send WIFI_DEMO_EVENT_TRY_CONNECT again.
            manager->retry_state = WIFI_DEMO_RETRY_CONNECT_END;
            break;
        }
        case WIFI_DEMO_EVENT_CONNECTED: {
            ql_data_call_info_s info = {0};
            ret = ql_get_data_call_info(sim_id, profile_idx, &info);
            QL_WIFI_DEMO_LOG("ql_get_data_call_info ret: 0x%x", ret);
            QL_WIFI_DEMO_LOG("info.profile_idx: %d, info.ip_version: %d", info.profile_idx, info.ip_version);
            QL_WIFI_DEMO_LOG("info->v4.state: %d, info.v6.state: %d", info.v4.state, info.v6.state);
            if (info.v4.state)
            {
                QL_WIFI_DEMO_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info.v4.addr.ip)));
                QL_WIFI_DEMO_LOG("info.v4.addr.pri_dns: %s", ip4addr_ntoa(&(info.v4.addr.pri_dns)));
                QL_WIFI_DEMO_LOG("info.v4.addr.sec_dns: %s", ip4addr_ntoa(&(info.v4.addr.sec_dns)));
            }

#if 0  //If you want to test psm of wifi module, please open this part of the code.
       //Currently only supports FGM842D.
#ifdef QL_WIFI_FGM842D
            quec_wifi_psm_info psm_info = {
                .enable = QUEC_WIFI_PSM_ENABLE,
                .dtim   = 10
            };
            ql_wifi_app_psm(&psm_info);
#endif
#endif

#if 0   //If you want to use the iperf tool to measure speed, please open this part of the code.
#if 1
            //start iperf server
            ip_addr_t local_addr = {0};
            local_addr.u_addr.ip4 = info.v4.addr.ip;
            void* lwiperf_conn = lwiperf_start_tcp_server(&local_addr, 5001, NULL, NULL);
            QL_WIFI_DEMO_LOG("lwiperf_conn = %p",lwiperf_conn);
#else
            //start iperf client
            // !Please replace the server ip with your own
            ip_addr_t remote_addr = {0};
            inet_pton(AF_INET, "192.168.1.1", &(remote_addr.u_addr.ip4));

            void * lwiperf_conn = lwiperf_start_tcp_client_default(&remote_addr,NULL,NULL);
            QL_WIFI_DEMO_LOG("lwiperf_conn = %p",lwiperf_conn);
#endif
#else
            wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_APP_TCP_CONNECT, 0, 0, 0);
#endif
            break;
        }
        case WIFI_DEMO_EVENT_APP_TCP_CONNECT: {
            ql_rtos_mutex_try_lock(wifi_demo_mutex);
            wifi_demo_tcp_client_create();
            ql_rtos_mutex_unlock(wifi_demo_mutex);
            break;
        }
        case WIFI_DEMO_EVENT_GET_STATUS: {
            uint32_t value = event.param1;
            if (value == QL_WIFI_STATE_CONNECTED)
            {
                manager->retry_state = WIFI_DEMO_RETRY_CONNECT_END;
                ql_rtos_timer_stop(manager->retry_timer);
                //From others to QL_WIFI_STATE_CONNECTED
                if(manager->state != QL_WIFI_STATE_CONNECTED)
                {
                    wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_CONNECTED, 0, 0, 0);
                }
            }
            else if (value == QL_WIFI_STATE_DISCONNECT)
            {
                if (manager->retry_state == WIFI_DEMO_RETRY_CONNECT_END)
                {
                    manager->retry_state = WIFI_DEMO_RETRY_CONNECT_START;
                    ql_rtos_timer_stop(manager->retry_timer);
                    int ret = 0;
                    ret = ql_rtos_timer_start(manager->retry_timer, QL_WIFI_DEMO_DISCONNECT_RETRY_TIME, 0);
                    QL_WIFI_DEMO_LOG("timer start %x", ret);
                    if (ret != QL_OSI_SUCCESS)
                    {
                        QL_WIFI_DEMO_LOG("manager->retry_timer start failed");
                        wifi_demo_send_event(manager->task, WIFI_DEMO_EVENT_TRY_CONNECT, 0, 0, 0);
                    }
                }
            }
            manager->state = value;
            manager->retry_cnt = 0;
            break;
        }
        default: {
            break;
        }
        }
    }
exit:
    if (manager->retry_timer)
    {
        ql_rtos_timer_delete(manager->retry_timer);
    }
    if (manager->msg_queue)
    {
        ql_rtos_queue_delete(manager->msg_queue);
        manager->msg_queue = NULL;
    }
    if(wifi_demo_mutex)
    {
        ql_rtos_mutex_delete(wifi_demo_mutex);
        wifi_demo_mutex = NULL;
    }
#endif
    QL_WIFI_DEMO_LOG("wifi demo task end");
    ql_rtos_task_delete(NULL);
}

void ql_wifi_demo_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    wifi_demo_manager_s *manager = &wifi_demo_manager;
    err = ql_rtos_task_create(&(manager->task), 4 * 1024, APP_PRIORITY_NORMAL, "wifi_demo", wifi_demo_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_WIFI_DEMO_LOG("task created failed");
        return;
    }
}
