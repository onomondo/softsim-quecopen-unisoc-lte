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
#define QL_ETHERNET_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_ETHERNET_DEMO_LOG(msg, ...)			QL_LOG(QL_ETHERNET_LOG_LEVEL, "phy_demo", msg, ##__VA_ARGS__)

ql_task_t ethernet_phy_task = NULL;


#ifdef ETHERNET_PHY_CH395
// SPI
#define QL_CUR_SPI_PORT             QL_SPI_PORT1
#define QL_CUR_SPI_CS_PIN           QL_CUR_SPI1_CS_PIN
#define QL_CUR_SPI_CS_FUNC          QL_CUR_SPI1_CS_FUNC
#define QL_CUR_SPI_CLK_PIN          QL_CUR_SPI1_CLK_PIN
#define QL_CUR_SPI_CLK_FUNC         QL_CUR_SPI1_CLK_FUNC
#define QL_CUR_SPI_DO_PIN           QL_CUR_SPI1_DO_PIN
#define QL_CUR_SPI_DO_FUNC          QL_CUR_SPI1_DO_FUNC
#define QL_CUR_SPI_DI_PIN           QL_CUR_SPI1_DI_PIN
#define QL_CUR_SPI_DI_FUNC          QL_CUR_SPI1_DI_FUNC
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
#define QL_ETHERNET_PHY_TCP_CLIENTS_CNT 1
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
    int close_flag;
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
}ethernet_phy_spi_cs_s;

typedef struct
{
    ql_task_t task;
#ifdef ETHERNET_PHY_CH395
    ql_mutex_t mutex;
    // TCP
    ethernet_phy_tcp_ctx_s client_ctx[QL_ETHERNET_PHY_TCP_CLIENTS_CNT];
    int client_num;
	// Debounce
    ql_timer_t timer;
    ql_GpioNum gpio_num;
#endif
} ethernet_phy_manager_s;

#endif
/*========================================================================
 *   Global Variable
 *========================================================================*/
ethernet_phy_manager_s ethernet_phy_manager = {0};
#ifdef ETHERNET_PHY_CH395
static char send_buf[128]={0};
static int  send_len = 0;
static char recv_buf[128]={0};
static int  recv_len = 0;
#endif

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

int ethernet_phy_int_gpio(void* cb,void *ctx)
{
    int err = -1;
    if(QL_GPIO_SUCCESS != ql_pin_set_func(QL_CUR_INT_PIN, QL_CUR_INT_PIN_GPIO_FUNC))
    {
        goto exit;
    }
    if(QL_GPIO_SUCCESS != ql_int_register(QL_CUR_INT_GPIO_NUM,EDGE_TRIGGER,DEBOUNCE_DIS,EDGE_FALLING, PULL_UP,cb,ctx))
    {
        goto exit;
    }
    if(QL_GPIO_SUCCESS != ql_int_enable(QL_CUR_INT_GPIO_NUM))
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
    if(!ctx)
    {
        return;
    }
    ql_LvlMode lvl = (ql_LvlMode)(*((uint8_t*)ctx));
    if(lvl == LVL_HIGH)
    {
        ql_spi_cs_high(QL_CUR_SPI_PORT);
    }
    else
    {
        ql_spi_cs_low(QL_CUR_SPI_PORT);
    }
}


static void ethernet_phy_client_thread(void *argv)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
    ethernet_phy_tcp_ctx_s *ctx = (ethernet_phy_tcp_ctx_s *)argv;
    ctx->close_flag = 0;
	int i = 0;
    int ret = 0;
	int socket_fd = -1;
	int flags = 0;
	int connected = 0;
	fd_set read_fds;
	fd_set write_fds;
	fd_set exp_fds;
	int fd_changed = 0;
	int closing = false;
	struct sockaddr_in local4, server_ipv4;

    ip4_addr_t int_srcip;
    ip4_addr_t int_destip;

    IP4_ADDR(&int_srcip,  ctx->srcip[0],ctx->srcip[1],ctx->srcip[2],ctx->srcip[3]);
    IP4_ADDR(&int_destip, ctx->destip[0],ctx->destip[1],ctx->destip[2],ctx->destip[3]);

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&exp_fds);

	memset(&local4, 0x00, sizeof(struct sockaddr_in));
	local4.sin_family = AF_INET;
	local4.sin_port = htons(ctx->srcport);;
    memcpy(&(local4.sin_addr),&int_srcip,sizeof(int_srcip));


    QL_ETHERNET_DEMO_LOG("socket start!");
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
    server_ipv4.sin_port = htons(ctx->destport);
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
    if(connected == 1)
    {
        ethernet_phy_try_lock(manager->mutex);
        ctx->fd = socket_fd;
        ethernet_phy_unlock(manager->mutex);

        FD_SET(socket_fd, &read_fds);	
        QL_ETHERNET_DEMO_LOG("=====connect to \"tcp\" success=====");
        memset(send_buf, 0x00, 128);
        send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
        
        if(write(socket_fd, send_buf, send_len) != send_len){
            FD_SET(socket_fd, &write_fds);	
        }else{
            i++;
        }
	}
    else
    {
        FD_SET(socket_fd, &write_fds);	
    }	
    FD_SET(socket_fd, &exp_fds);
    while(1)
    {
        if(ctx->close_flag)
        {
            QL_ETHERNET_DEMO_LOG("client close flag");
            goto exit;
        }
        if(connected == 1)
        {
            FD_SET(socket_fd, &read_fds);
        }
        else
        {
            FD_SET(socket_fd, &write_fds);
        }

        FD_SET(socket_fd, &exp_fds);
        struct timeval timeout = {3,0};
        fd_changed = select(socket_fd+1, &read_fds, &write_fds, &exp_fds, &timeout);
        if(fd_changed > 0)
        {
            if(FD_ISSET(socket_fd, &write_fds))
            {
                FD_CLR(socket_fd, &write_fds);
                if(connected== 0)
                {
                    int value = 0;
                    int len = 0;
                        
                    len = sizeof(value);
                    getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &value, &len);
                    if(value == 0 || value == EISCONN )
                    {
                        ethernet_phy_try_lock(manager->mutex);
                        ctx->fd = socket_fd;
                        ethernet_phy_unlock(manager->mutex);
                        QL_ETHERNET_DEMO_LOG("=====connect to \"192.168.1.200:8252\" success=====");
                        connected = 1;						
                        FD_SET(socket_fd, &read_fds);	
                        memset(send_buf, 0x00, 128);
                        send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
                        write(socket_fd, send_buf, send_len);
                        i++;
                    }
                    else
                    {
                        QL_ETHERNET_DEMO_LOG("=====connect to \"192.168.1.200:8252\" failed=====");
                        close(socket_fd);
                        socket_fd = -1;
                        break;
                    }
                }
                else
                {
                    memset(send_buf, 0x00, 128);
                    send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
                    write(socket_fd, send_buf, send_len);
                    i++;
                }
            }
            else if(FD_ISSET(socket_fd, &read_fds))
            {
                FD_CLR(socket_fd, &read_fds);
                memset(recv_buf,0x00, 128);
                recv_len = read(socket_fd, recv_buf, 128);
                if(recv_len > 0)
                {
                    QL_ETHERNET_DEMO_LOG(">>>>Recv: %s", recv_buf);
                    memset(send_buf, 0x00, 128);
                    send_len = snprintf(send_buf, 128,"%d%s%d\r\n",i,"startAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAend",i);
                    write(socket_fd, send_buf, send_len);
                    i++;
                    FD_SET(socket_fd, &read_fds);
                }
                else if(recv_len == 0)
                {
                    if(closing == false)
                    {
                        QL_ETHERNET_DEMO_LOG("===passive close!!!!");
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
                    if(lwip_get_error(socket_fd) == EAGAIN)
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
            else if(FD_ISSET(socket_fd, &exp_fds))
            {
                FD_CLR(socket_fd, &exp_fds);
                close(socket_fd);
                socket_fd = -1;
                break;
            }
        }
    }
exit:
    QL_ETHERNET_DEMO_LOG("client end");
    close(socket_fd);
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
#endif

static void ethernet_phy_thread(void * argv)
{
    ethernet_phy_manager_s *manager = &ethernet_phy_manager;
#ifdef ETHERNET_PHY_CH395
    uint8_t srcip[] =    {192,168,1,100};
    uint16_t srcport =      0;                  //Zero for random port.

    uint8_t destip[] =   {192,168,1,200};
    uint16_t destport =     8252;
    
    uint8_t gw[] =       {192,168,1,1};
    uint8_t netmask[] =  {255,255,255,0};

    ethernet_phy_create_mutex(&(manager->mutex));
    /*
        ch395q init
    */
    ql_ethernet_phy_s spi_ctx = 
    {
        .mode                          = QL_ETHERNET_PHY_HW_SPI_MODE,
        .hw_spi.mosi_pin_num           = QL_CUR_SPI_DO_PIN,
        .hw_spi.mosi_func_sel          = QL_CUR_SPI_DO_FUNC,
        .hw_spi.miso_pin_num           = QL_CUR_SPI_DI_PIN,
        .hw_spi.miso_func_sel          = QL_CUR_SPI_DO_FUNC,
        .hw_spi.clk_pin_num            = QL_CUR_SPI_CLK_PIN,
        .hw_spi.clk_func_sel           = QL_CUR_SPI_CLK_FUNC,
        .hw_spi.cs_pin_num             = QL_CUR_SPI_CS_PIN,
        .hw_spi.cs_func_sel            = QL_CUR_SPI_CS_FUNC,
        /*********************************************/
        .hw_spi.config.input_mode      = QL_SPI_INPUT_TRUE,
        .hw_spi.config.port            = QL_CUR_SPI_PORT,
        .hw_spi.config.spiclk          = QL_SPI_CLK_1_5625MHZ,
#if QL_SPI_16BIT_DMA  
        .hw_spi.config.framesize       = 16,
#else
        .hw_spi.config.framesize       = 8,
#endif
        .hw_spi.config.cs_polarity0    = QL_SPI_CS_ACTIVE_LOW,
        .hw_spi.config.cs_polarity1    = QL_SPI_CS_ACTIVE_LOW,
        .hw_spi.config.cpol            = QL_SPI_CPOL_LOW,
        .hw_spi.config.cpha            = QL_SPI_CPHA_1Edge,
        .hw_spi.config.input_sel       = QL_SPI_DI_1,
        .hw_spi.config.transmode       = QL_SPI_DMA_IRQ,
        .hw_spi.config.cs              = QL_SPI_CS0,
        .hw_spi.config.clk_delay       = QL_SPI_CLK_DELAY_0,
    };
    if(QL_ETHERNET_SUCCESS != ql_ethernet_phy_init(&spi_ctx))
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

    if(0 != ethernet_phy_int_gpio(ch395_app_get_gpio_cb(),NULL))
    {
        goto exit;
    }
    /*
            QUECTEL------------------CH395Q--------------------PC
    IP_LAN|192.168.1.100(port)      192.168.1.100(port)       192.168.1.200(8252)
    GW    |192.168.1.1              192.168.1.1               Any except for 192.168.1.100
    */
    ql_ethernet_ctx_s ctx =
    {
        .ip4 =      srcip,
        .gw =       gw,
        .netmask =  netmask,
        .mode =     QL_ETHERNET_MODE_NONE,
    };
    if(!ch395_app_init(&ctx))
    {
        QL_ETHERNET_DEMO_LOG("ch395 init fail!");
        goto exit;
    }

    while(1)
    {
        ql_event_t event;
        if(ql_event_try_wait(&event) != 0)
        {
            continue;
        }
        if (event.id == 0)
        {
            continue;
        }
        QL_ETHERNET_DEMO_LOG("ch395 event:%x",event.id);
        switch(event.id)
        {
            case QUEC_ETHERNET_APP_CONNECTED:
            {
                ethernet_phy_try_lock(manager->mutex);
                //Find NULL task
                ethernet_phy_tcp_ctx_s *ctx = NULL;
                int i = 0;
                for (i = 0; i < QL_ETHERNET_PHY_TCP_CLIENTS_CNT; i++)
                {
                    ctx = &(manager->client_ctx[i]);
                    if(NULL == ctx->task)
                    {
                        break;
                    }
                }
                if(i < QL_ETHERNET_PHY_TCP_CLIENTS_CNT)
                {
                    memcpy(ctx->srcip, srcip, QL_ETHERNET_PHY_TCP_IP4_SIZE);
                    ctx->srcport = srcport;
                    memcpy(ctx->destip, destip, QL_ETHERNET_PHY_TCP_IP4_SIZE);
                    ctx->destport = destport;
                    if (0 == ethernet_phy_tcp_client_create((void *)ctx))
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
                break;
            }
            case QUEC_ETHERNET_APP_DISCONNECTED: {
                QL_ETHERNET_DEMO_LOG("phy discon");
                ethernet_phy_try_lock(manager->mutex);
                ethernet_phy_tcp_ctx_s *ctx = NULL;
                int i = 0;
                for (i = 0; i < QL_ETHERNET_PHY_TCP_CLIENTS_CNT; i++)
                {
                    ctx = &(manager->client_ctx[i]);
                    if(ctx->task)
                    {
                        ctx->close_flag = 1;
                    }
                }
                ethernet_phy_unlock(manager->mutex);
                break;
            }
            default:
            {
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
	err = ql_rtos_task_create(&(manager->task), 4*1024, APP_PRIORITY_NORMAL, "q_phy_demo", ethernet_phy_thread, NULL, 10);
    if(err != QL_OSI_SUCCESS)
	{
		QL_ETHERNET_DEMO_LOG("task created failed");
        return;
	}
    
}

