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
#include "ql_api_spi.h"
#include "ethernet_demo_macro.h"
#include "ql_api_ethernet.h"
#include "ql_api_datacall.h"
#include "ql_api_nw.h"
#include "ql_power.h"

#include "sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#include "lwip/inet.h"
#include "lwip/tcp.h"

#ifdef ETHERNET_PHY_CH395
#include "CH395.H"
#endif

/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_ETHERNET_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_ETHERNET_DEMO_LOG(msg, ...) QL_LOG(QL_ETHERNET_LOG_LEVEL, "phy_demo", msg, ##__VA_ARGS__)

#ifdef ETHERNET_PHY_CH395
// SPI
#define QL_CUR_SPI_PORT QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC QL_CUR_SPI1_DI_FUNC
// GPIO
#define QL_CUR_INT_PIN 59
#define QL_CUR_INT_PIN_GPIO_FUNC 0
#define QL_CUR_INT_GPIO_NUM GPIO_2

#define QL_ETHERNET_HOT_PLUG_PIN 121
#define QL_ETHERNET_HOT_PLUG_PIN_GPIO_FUNC 0
#define QL_ETHERNET_HOT_PLUG_GPIO_NUM GPIO_23

#define QL_ETHERNET_HOT_PLUG_DEBOUNCE_TIME 100
// TCP
#define QL_ETHERNET_PHY_TCP_IP4_SIZE 4
#define QL_ETHERNET_PHY_TCP_CLIENTS_CNT 6
// HTTP
#define QL_ETHERNET_HTTP_ALIGN 8
#define QL_ETHERNET_HTTP_BUFSIZE 8096

#define QL_ETHERNET_HTTP_SEND_BYTES (2 * 1024)          // send 2KB each time
#define QL_ETHERNET_HTTP_CONTENT_LEN (10 * 1024 * 1024) // 10MB
#endif

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
#ifdef ETHERNET_PHY_CH395

typedef struct
{
    int id;
    int fd;
    uint8_t srcip[QL_ETHERNET_PHY_TCP_IP4_SIZE];
    uint16_t srcport;
    uint8_t destip[QL_ETHERNET_PHY_TCP_IP4_SIZE];
    uint16_t destport;
    ql_task_t task;
    struct sockaddr_in addr_in;
} ethernet_phy_tcp_ctx_s;

typedef struct
{
    ql_spi_port_e port;
    ql_LvlMode lvl;
} ethernet_phy_spi_cs_s;
#endif

typedef struct
{
    ql_task_t task;
#ifdef ETHERNET_PHY_CH395
    ql_mutex_t mutex;
    // TCP
    ethernet_phy_tcp_ctx_s server_ctx;
    ethernet_phy_tcp_ctx_s client_ctx[QL_ETHERNET_PHY_TCP_CLIENTS_CNT];
    int client_num;
	// Debounce
    ql_timer_t timer;
    ql_GpioNum gpio_num;
#endif
} ethernet_phy_manager_s;
/*========================================================================
 *   Global Variable
 *========================================================================*/
ethernet_phy_manager_s ethernet_phy_manager = {0};

/*========================================================================
 *	function Definition
 *========================================================================*/
#ifdef ETHERNET_PHY_CH395
void ethernet_phy_create_mutex(ql_mutex_t *lockaddr)
{
    if (!lockaddr)
    {
        return;
    }
    if (NULL == *lockaddr)
    {
        ql_rtos_mutex_create(lockaddr);
    }
}

void ethernet_phy_delete_mutex(ql_mutex_t lock)
{
    if (NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_delete(lock);
}

void ethernet_phy_try_lock(ql_mutex_t lock)
{
    if (NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_lock(lock, 0xffffffffUL);
}

void ethernet_phy_unlock(ql_mutex_t lock)
{
    if (NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_unlock(lock);
}

void ethernet_phy_send_event(uint32_t id, uint32_t param1, uint32_t param2, uint32_t param3)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    ql_event_t event;
    event.id = id;
    event.param1 = param1;
    event.param2 = param2;
    event.param3 = param3;
    ethernet_phy_try_lock(manager->mutex);
    ql_rtos_event_send(manager->task, &event);
    ethernet_phy_unlock(manager->mutex);
}

void ethernet_phy_ch395_app_reset_cb(void *ctx)
{
    // Do hardware reset here.
    QL_ETHERNET_DEMO_LOG("ch395 reset cb");
}

void ethernet_phy_ch395_app_notify_cb(void *ctx)
{
    ch395_app_net_status_e status = *((ch395_app_net_status_e *)ctx);
    QL_ETHERNET_DEMO_LOG("get phy status: %d", status);
    if (status == CH395_APP_NET_CONNECTED)
    {
        ethernet_phy_send_event(QUEC_ETHERNET_APP_CONNECTED, 0, 0, 0);
    }
    else if (status == CH395_APP_NET_DISCONNECTED)
    {
        ethernet_phy_send_event(QUEC_ETHERNET_APP_DISCONNECTED, 0, 0, 0);
    }
}

void ethernet_phy_hot_plug_timer_cb(void *ctx)
{
    ql_GpioNum gpio_num = *((ql_GpioNum*)ctx);
    ql_LvlMode lvl = LVL_LOW;

    ql_gpio_get_level(QL_ETHERNET_HOT_PLUG_GPIO_NUM, &lvl);
    if(lvl == LVL_LOW)
    {
        QL_ETHERNET_DEMO_LOG("eth plug in");
        ch395_app_reset();
    }
    else 
    {
        QL_ETHERNET_DEMO_LOG("eth plug out");
    }
    if(QL_GPIO_SUCCESS !=ql_int_enable(gpio_num))
    {
        return;
    }
}

void ethernet_phy_hot_plug_cb(void *ctx)
{
    ql_GpioNum gpio_num = *((ql_GpioNum*)ctx);
    if(QL_GPIO_SUCCESS !=ql_int_disable(gpio_num))
    {
        return;
    }

    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    if(manager->timer == NULL || ql_rtos_timer_is_running(manager->timer))
    {
        return;
    }
    QL_ETHERNET_DEMO_LOG("hot plug debounce");
    ql_rtos_timer_start(manager->timer, QL_ETHERNET_HOT_PLUG_DEBOUNCE_TIME, 0);
}

int ethernet_phy_int_gpio(void *cb, void *ctx)
{
    int err = -1;
    // ch395 interrupt
    if (QL_GPIO_SUCCESS != ql_pin_set_func(QL_CUR_INT_PIN, QL_CUR_INT_PIN_GPIO_FUNC))
    {
        goto exit;
    }
    if (QL_GPIO_SUCCESS != ql_int_register(QL_CUR_INT_GPIO_NUM, EDGE_TRIGGER, DEBOUNCE_DIS, EDGE_FALLING, PULL_UP, cb, ctx))
    {
        goto exit;
    }
    if (QL_GPIO_SUCCESS != ql_int_enable(QL_CUR_INT_GPIO_NUM))
    {
        goto exit;
    }

    // ch395 hot plug
    /*
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    manager->gpio_num = QL_ETHERNET_HOT_PLUG_GPIO_NUM;
    if (QL_GPIO_SUCCESS != ql_pin_set_func(QL_ETHERNET_HOT_PLUG_PIN, QL_ETHERNET_HOT_PLUG_PIN_GPIO_FUNC))
    {
        goto exit;
    }
    if (QL_GPIO_SUCCESS != ql_int_register(QL_ETHERNET_HOT_PLUG_GPIO_NUM, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_BOTH, PULL_UP, ethernet_phy_hot_plug_cb, &(manager->gpio_num)))
    {
        goto exit;
    }
    if (QL_GPIO_SUCCESS != ql_int_enable(QL_ETHERNET_HOT_PLUG_GPIO_NUM))
    {
        goto exit;
    }
    */
    err = 0;
exit:
    return err;
}

void ethernet_phy_set_cs(void *ctx)
{
    if (!ctx)
    {
        return;
    }
    ql_LvlMode lvl = (ql_LvlMode)(*((uint8_t *)ctx));
    if (lvl == LVL_HIGH)
    {
        ql_spi_cs_high(QL_CUR_SPI_PORT);
    }
    else
    {
        ql_spi_cs_low(QL_CUR_SPI_PORT);
    }
}

static void ethernet_phy_http_error(int nClientScok, int nStatusCode)
{
    char str[256] = {0};
    int ret = 0;

    char *pStrError = "HTTP/1.1 %d NOT FOUND\
        Content-Type: text/html\
        Accept-Ranges: bytes\
        Server: HFS 2.3k\
        Set-Cookie: HFS_SID_=0.974440259393305; path=/; HttpOnly\
        Content-Encoding: gzip";

    QL_ETHERNET_DEMO_LOG("httpd status code:%d", nStatusCode);
    ret = snprintf(str, 256, pStrError, nStatusCode);

    if (0 != ret)
    {
        if (ret != write(nClientScok, str, ret))
        {
            QL_ETHERNET_DEMO_LOG("socket send failed\n");
        }
    }
}

static void ethernet_phy_http_send(int nClientScok)
{
    int ret = 0, len = 0;
    int len_temp = 0;

    char *buffer_align = NULL;
    char *buffer = NULL;
    buffer_align = (char *)malloc(QL_ETHERNET_HTTP_ALIGN + QL_ETHERNET_HTTP_BUFSIZE);
    if (NULL == buffer_align)
    {
        ethernet_phy_http_error(nClientScok, 500);
        close(nClientScok);
        return;
    }
    buffer = (char *)OSI_ALIGN_UP(buffer_align, QL_ETHERNET_HTTP_ALIGN);
    memset(buffer, 0, QL_ETHERNET_HTTP_BUFSIZE + QL_ETHERNET_HTTP_ALIGN);

    // read any http streams,no need to parse
    struct timeval timeout = {3,0};
    setsockopt(nClientScok,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
    ret = read(nClientScok, buffer, QL_ETHERNET_HTTP_BUFSIZE);
    if (ret <= 0)
    {
        ethernet_phy_http_error(nClientScok, 400);
        goto exit;
    }
    else
    {
        QL_ETHERNET_DEMO_LOG("recv :  %s\n", buffer);
    }
    len = QL_ETHERNET_HTTP_CONTENT_LEN;
    snprintf(buffer, QL_ETHERNET_HTTP_BUFSIZE, "HTTP/1.1 200 OK\nServer: Quectel\nContent-Length: %d\nConnection: close\nContent-Type: %s\n\n", len, "application/octet-stream");
    // send http header
    len_temp = strlen(buffer);
    if (len_temp != write(nClientScok, buffer, len_temp))
    {
        goto exit;
    }

    // send QL_ETHERNET_HTTP_SEND_BYTES each time
    int send_bytes = 0;
    int has_sent = 0;
    while (has_sent < len)
    {
        if (has_sent + QL_ETHERNET_HTTP_SEND_BYTES > len)
        {
            send_bytes = len - QL_ETHERNET_HTTP_SEND_BYTES;
        }
        else
        {
            send_bytes = QL_ETHERNET_HTTP_SEND_BYTES;
        }
        ret = write(nClientScok, buffer, send_bytes);
        if (ret < 0)
        {
            QL_ETHERNET_DEMO_LOG("socket send failed, return %d\n", ret);
            break;
        }
        QL_ETHERNET_DEMO_LOG("phy http send %d/%d/%d", send_bytes, has_sent, len);
        has_sent += send_bytes;
    }
exit:
    close(nClientScok);
    free(buffer_align);
    return;
}

static void ethernet_phy_client_thread(void *argv)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    ethernet_phy_tcp_ctx_s *ctx = (ethernet_phy_tcp_ctx_s *)argv;

    QL_ETHERNET_DEMO_LOG("client start:%s:%d", inet_ntoa(ctx->addr_in.sin_addr), ctx->addr_in.sin_port);
    // Better use "select" for deleting clients quickly and safely once detecting phy status change.
    ethernet_phy_http_send(ctx->fd);
    QL_ETHERNET_DEMO_LOG("client end:%s:%d", inet_ntoa(ctx->addr_in.sin_addr), ctx->addr_in.sin_port);

    ethernet_phy_try_lock(manager->mutex);
    memset(ctx, 0, sizeof(ethernet_phy_tcp_ctx_s));
    ctx->id = -1;
    ctx->fd = -1;
    manager->client_num--;
    ethernet_phy_unlock(manager->mutex);

    ql_rtos_task_delete(NULL);
}

int ethernet_phy_tcp_client_create(void *argv)
{
    if (!argv)
    {
        QL_ETHERNET_DEMO_LOG("client param err");
        return -1;
    }
    ethernet_phy_tcp_ctx_s *ctx = (ethernet_phy_tcp_ctx_s *)argv;
    QlOSStatus err = QL_OSI_SUCCESS;
    err = ql_rtos_task_create(&(ctx->task), 4 * 1024, APP_PRIORITY_NORMAL, "phy_client", ethernet_phy_client_thread, argv, 10);
    if (err != QL_OSI_SUCCESS)
    {
        QL_ETHERNET_DEMO_LOG("task created failed");
        return -1;
    }
    return 0;
}

void ethernet_phy_tcp_server(void *argv)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    ethernet_phy_tcp_ctx_s *ctx = (ethernet_phy_tcp_ctx_s *)argv;
    int socket_fd = -1;
    int ret = 0;
    struct sockaddr_in local4, client4;

    ip4_addr_t int_srcip;

    IP4_ADDR(&int_srcip, ctx->srcip[0], ctx->srcip[1], ctx->srcip[2], ctx->srcip[3]);

    memset(&local4, 0x00, sizeof(struct sockaddr_in));
    local4.sin_family = AF_INET;
    local4.sin_port = htons(ctx->srcport);
    memcpy(&(local4.sin_addr), &int_srcip, sizeof(int_srcip));

    QL_ETHERNET_DEMO_LOG("socket start!");
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        goto exit;
    }
    ret = bind(socket_fd, (struct sockaddr *)&local4, sizeof(struct sockaddr));
    if (ret < 0)
    {
        goto exit;
    }
    ret = listen(socket_fd, 1);
    if (ret < 0)
    {
        goto exit;
    }
    ethernet_phy_try_lock(manager->mutex);
    ctx->fd = socket_fd;
    ethernet_phy_unlock(manager->mutex);
    QL_ETHERNET_DEMO_LOG("tcp server wait for clients");
    // Better use "select" for deleting server quickly and safely once detecting phy status change.
    while (1)
    {
        int len = sizeof(struct sockaddr);
        int con_fd = accept(socket_fd, (struct sockaddr *)&client4, &len);
        if (con_fd < 0)
        {
            QL_ETHERNET_DEMO_LOG("con fd err");
            goto exit;
        }
        ethernet_phy_try_lock(manager->mutex);

        //Find NULL task
        ethernet_phy_tcp_ctx_s *client_ctx = NULL;
        int i = 0;
        for (i = 0; i < QL_ETHERNET_PHY_TCP_CLIENTS_CNT; i++)
        {
            client_ctx = &(manager->client_ctx[i]);
            if(NULL == client_ctx->task)
            {
                break;
            }
        }
        if(i < QL_ETHERNET_PHY_TCP_CLIENTS_CNT)
        {
            client_ctx->id = i;
            client_ctx->fd = con_fd;
            memcpy((void *)(&(client_ctx->addr_in)), &client4, sizeof(struct sockaddr));
            if (0 == ethernet_phy_tcp_client_create((void *)client_ctx))
            {
                manager->client_num++;
                QL_ETHERNET_DEMO_LOG("client %d", manager->client_num);
            }
            else
            {
                QL_ETHERNET_DEMO_LOG("client create failed %d/%d",manager->client_num,QL_ETHERNET_PHY_TCP_CLIENTS_CNT);
            }
        }
        else
        {
            QL_ETHERNET_DEMO_LOG("client max %d/%d,end", manager->client_num,QL_ETHERNET_PHY_TCP_CLIENTS_CNT);
        }
        ethernet_phy_unlock(manager->mutex);
    }
exit:
    QL_ETHERNET_DEMO_LOG("tcp server end");
    close(socket_fd);
    ethernet_phy_try_lock(manager->mutex);
    memset(ctx, 0, sizeof(ethernet_phy_tcp_ctx_s));
    ctx->fd = -1;
    ethernet_phy_unlock(manager->mutex);
    ql_rtos_task_delete(NULL);
    return;
}

void ethernet_phy_tcp_server_create(void *argv)
{
    if (!argv)
    {
        QL_ETHERNET_DEMO_LOG("server param err");
        return;
    }
    ethernet_phy_tcp_ctx_s *ctx = (ethernet_phy_tcp_ctx_s *)argv;
    if (ctx->task)
    {
        QL_ETHERNET_DEMO_LOG("server exist");
        return;
    }
    QlOSStatus err = QL_OSI_SUCCESS;
    err = ql_rtos_task_create(&(ctx->task), 4 * 1024, APP_PRIORITY_NORMAL, "phy_server", ethernet_phy_tcp_server, argv, 10);
    if (err != QL_OSI_SUCCESS)
    {
        QL_ETHERNET_DEMO_LOG("task created failed");
        return;
    }
}
#endif

static void ethernet_phy_thread(void *argv)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
#ifdef ETHERNET_PHY_CH395
    uint8_t srcip[QL_ETHERNET_PHY_TCP_IP4_SIZE] = {192, 168, 1, 1};
    uint16_t srcport = 8252; // Zero for random port.

    uint8_t gw[QL_ETHERNET_PHY_TCP_IP4_SIZE] = {192, 168, 1, 1};
    uint8_t netmask[QL_ETHERNET_PHY_TCP_IP4_SIZE] = {255, 255, 255, 0};

    ethernet_phy_create_mutex(&(manager->mutex));

    /*
        ch395q init
    */
    ql_ethernet_phy_s spi_ctx =
    {
        .mode = QL_ETHERNET_PHY_HW_SPI_MODE,
        .hw_spi.mosi_pin_num = QL_CUR_SPI_DO_PIN,
        .hw_spi.mosi_func_sel = QL_CUR_SPI_DO_FUNC,
        .hw_spi.miso_pin_num = QL_CUR_SPI_DI_PIN,
        .hw_spi.miso_func_sel = QL_CUR_SPI_DO_FUNC,
        .hw_spi.clk_pin_num = QL_CUR_SPI_CLK_PIN,
        .hw_spi.clk_func_sel = QL_CUR_SPI_CLK_FUNC,
        .hw_spi.cs_pin_num = QL_CUR_SPI_CS_PIN,
        .hw_spi.cs_func_sel = QL_CUR_SPI_CS_FUNC,
        /*********************************************/
        .hw_spi.config.input_mode = QL_SPI_INPUT_TRUE,
        .hw_spi.config.port = QL_CUR_SPI_PORT,
        .hw_spi.config.spiclk = QL_SPI_CLK_12_5MHZ,
#if QL_SPI_16BIT_DMA
        .hw_spi.config.framesize = 16,
#else
        .hw_spi.config.framesize = 8,
#endif
        .hw_spi.config.cs_polarity0 = QL_SPI_CS_ACTIVE_LOW,
        .hw_spi.config.cs_polarity1 = QL_SPI_CS_ACTIVE_LOW,
        .hw_spi.config.cpol = QL_SPI_CPOL_LOW,
        .hw_spi.config.cpha = QL_SPI_CPHA_1Edge,
        .hw_spi.config.input_sel = QL_SPI_DI_1,
        .hw_spi.config.transmode = QL_SPI_DMA_IRQ,
        .hw_spi.config.cs = QL_SPI_CS0,
        .hw_spi.config.clk_delay = QL_SPI_CLK_DELAY_0,
    };
    if (QL_ETHERNET_SUCCESS != ql_ethernet_phy_init(&spi_ctx))
    {
        goto exit;
    }

    if(QL_OSI_SUCCESS != ql_rtos_timer_create(&(manager->timer),manager->task,ethernet_phy_hot_plug_timer_cb,&(manager->gpio_num)))
    {
        QL_ETHERNET_DEMO_LOG("timer create fail!");
        goto exit;
    }
    if (!ch395_app_cb_register(CH395_APP_CB_TYPE_RESET, ethernet_phy_ch395_app_reset_cb))
    {
        QL_ETHERNET_DEMO_LOG("ch395 register fail!");
        goto exit;
    }

    if (!ch395_app_cb_register(CH395_APP_CB_TYPE_NOTIFY, ethernet_phy_ch395_app_notify_cb))
    {
        QL_ETHERNET_DEMO_LOG("ch395 register fail!");
        goto exit;
    }

    /*
            QUECTEL------------------CH395Q--------------------PC
    IP_LAN|192.168.1.100(port)      192.168.1.100(port)       192.168.1.200(8252)
    GW    |192.168.1.1              192.168.1.1               Any except for 192.168.1.100
    */
    ql_ethernet_ctx_s ctx =
        {
            .ip4 = srcip,
            .gw = gw,
            .netmask = netmask,
            .mode = QL_ETHERNET_MODE_NONE,
            .option.cover = 1,
            .option.dhcp = 2,   /* Support DHCP server. */
        };
    if (!ch395_app_init(&ctx))
    {
        QL_ETHERNET_DEMO_LOG("ch395 init fail!");
        goto exit;
    }
    ql_rtos_task_sleep_s(1);
    if (0 != ethernet_phy_int_gpio(ch395_app_get_gpio_cb(), NULL))
    {
        goto exit;
    }
    while (1)
    {
        ql_event_t event;
        if (ql_event_try_wait(&event) != 0)
        {
            continue;
        }
        if (event.id == 0)
        {
            continue;
        }
        QL_ETHERNET_DEMO_LOG("ethernet event:%x", event.id);
        switch (event.id)
        {
        case QUEC_ETHERNET_APP_CONNECTED: {
            ethernet_phy_tcp_ctx_s *ctx = &(manager->server_ctx);
            ethernet_phy_try_lock(manager->mutex);
            memcpy(ctx->srcip, srcip, QL_ETHERNET_PHY_TCP_IP4_SIZE);
            ctx->srcport = srcport;
            ethernet_phy_unlock(manager->mutex);
            ethernet_phy_tcp_server_create(ctx);
            break;
        }
        case QUEC_ETHERNET_APP_DISCONNECTED: {
            QL_ETHERNET_DEMO_LOG("phy discon");
            /*
                Server and clients can be set deleting flag here.
                Better use "select" for deleting server/clients quickly and safely once detecting phy status change.
            */
            break;
        }
        default: {
            break;
        }
        }
    }
exit:
    QL_ETHERNET_DEMO_LOG("ethernet demo end");
    if(manager->timer)
    {
        ql_rtos_timer_stop(manager->timer);
        ql_rtos_timer_delete(manager->timer);
        manager->timer = NULL;
    }
    if (manager->mutex)
    {
        ql_rtos_mutex_delete(manager->mutex);
        manager->mutex = NULL;
    }
#endif
    if (manager->task)
    {
        manager->task = NULL;
        ql_rtos_task_delete(NULL);
    }
}

void ql_ethernet_demo_init(void)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    QlOSStatus err = QL_OSI_SUCCESS;
    err = ql_rtos_task_create(&(manager->task), 4 * 1024, APP_PRIORITY_NORMAL, "q_phy_demo", ethernet_phy_thread, NULL, 10);
    if (err != QL_OSI_SUCCESS)
    {
        QL_ETHERNET_DEMO_LOG("task created failed");
        return;
    }
}
