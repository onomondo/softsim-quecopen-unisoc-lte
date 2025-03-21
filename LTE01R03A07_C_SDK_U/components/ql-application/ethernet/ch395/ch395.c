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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ql_log.h"
#include "ql_api_osi.h"
#include "ql_osi_def.h"
#include "ql_api_datacall.h"

#include "ch395_adapter.h"

#include "CH395CMD.H"
#include "CH395INC.H"
#include "CH395.H"

#include "lwipopts.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define CH395_LOG_LEVEL                     QL_LOG_LEVEL_INFO
#define CH395_DEMO_LOG(msg, ...)			QL_LOG(CH395_LOG_LEVEL, "ch395_log", msg, ##__VA_ARGS__)
//#define CH395_DEMO_LOG(msg, ...)


#define CH395_ERR_UNKNOW_RETRY_TIME (5 * 1000)
#define CH395_ERR_OPEN_RETRY_TIME (2 * 100)

#define CH395_CONNECTED_CHECK_STATUS_TIME (20 * 1000)
#define CH395_CHECK_STATUS_TIME (10 * 1000)
#define CH395_CHECK_STATUS_RESET_CNT (5)

#define CH395_ERROR_RETRY_TIMES     5

#define CH395_BUFFER_SIZE           1514       //max mac frame len
#define CH395_SOCKET_INDEX          0

#define CH395_SIM_ID_MAX    2

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/

/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct ch395_list_t
{
    struct ch395_list_t *next;
}ch395_list_t;

typedef struct
{
    struct ch395_list_t list;
    uint8_t *data;
    uint32_t len;
}ch395_elem_t;

struct _CH395_SYS
{
    uint8_t   IPAddr[4];                                           /* CH395IP地址 32bit*/
    uint8_t   GWIPAddr[4];                                         /* CH395网关地址 32bit*/
    uint8_t   MASKAddr[4];                                         /* CH395子网掩码 32bit*/
    uint8_t   MacAddr[6];                                          /* CH395MAC地址 48bit*/
    uint8_t   PHYStat;                                             /* CH395 PHY状态码 8bit*/
    uint8_t   MackFilt;                                            /* CH395 MAC过滤，默认为接收广播，接收本机MAC 8bit*/
    uint32_t  RetranCount;                                         /* 重试次数 默认为10次*/
    uint32_t  RetranPeriod;                                        /* 重试周期,单位MS,默认200MS */
    uint8_t   IntfMode;                                            /* 接口模式 */
    uint8_t   UnreachIPAddr[4];                                    /* 不可到达IP */
    uint16_t  UnreachPort;                                         /* 不可到达端口 */
};

struct _SOCK_INF
{
    uint8_t  IPAddr[4];                                           /* socket目标IP地址 32bit*/
    uint8_t  MacAddr[6];                                          /* socket目标地址 48bit*/
    uint8_t  ProtoType;                                           /* 协议类型 */
    uint8_t  ScokStatus;                                          /* socket状态，参考scoket状态定义 */
    uint8_t  TcpMode;                                             /* TCP模式 */
    uint32_t IPRAWProtoType;                                      /* IPRAW 协议类型 */
    uint16_t DesPort;                                             /* 目的端口 */
    uint16_t SourPort;                                            /* 目的端口 */
    uint16_t SendLen;                                             /* 发送数据长度 */
    uint16_t RemLen;                                              /* 剩余长度 */
    uint8_t  *pSend;                                              /* 发送指针 */
};

typedef struct
{
    /* API related. See ql_ethernet_ctx_s in ql_api_ethernet.h. */
    uint8_t ip4[4];
    uint8_t gw[4];
    uint8_t netmask[4];
    uint8_t mac[6];
    uint8_t dns1[4];
    uint8_t dns2[4];
    ql_ethernet_mode_e mode;
    bool cover;
    uint8_t dhcp;
    uint8_t sim_id;
    int profile_idx;
    ql_ethernet_route_prefer_e prefer;
    /* Task related. */
    ql_task_t task;
    ql_timer_t timer;
    uint32_t curr_id;
    ch395_app_net_status_e status;
    uint8_t status_reset_cnt;
    ch395_app_reset_cb_t reset_cb;
    ch395_app_notify_cb_t notify_cb;
    ql_mutex_t mutex;
}ch395_demo_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
/* 常用变量定义 */
uint8_t MyBuffer[CH395_BUFFER_SIZE];                              /* 数据缓冲区 */
uint8_t SendBuffer[CH395_BUFFER_SIZE];
uint32_t SendBufferLen = 0;
struct _SOCK_INF SockInf;                                       /* 保存Socket信息 */
struct _CH395_SYS CH395Inf;                                     /* 保存CH395信息 */

/* CH395相关定义 */
uint8_t CH395IPAddr[4] = {192,168,1,1};                         /* CH395IP地址 */
uint8_t CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395网关 */
uint8_t CH395IPMask[4] = {255,255,255,0};                        /* CH395子网掩码 */


ch395_demo_manager_s ch395_demo_manager = {0}; 

static ql_mutex_t list_lock = NULL;
ch395_list_t list_header;

/*========================================================================
 *	function Definition
 *========================================================================*/
uint8_t  CH395Init(void);
uint8_t CH395SocketInitOpen(void);
bool ch395_app_deinit(void* argv);

void ch395_create_mutex(ql_mutex_t* lockaddr)
{
    if(!lockaddr)
    {
        return;
    }
    if(NULL == *lockaddr)
    {
        ql_rtos_mutex_create(lockaddr);
    }
}

void ch395_delete_mutex(ql_mutex_t lock)
{
    if(NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_delete(lock);
}

void ch395_try_lock(ql_mutex_t lock)
{
    if(NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_lock(lock, 0xffffffffUL);
}

void ch395_unlock(ql_mutex_t lock)
{
    if(NULL == lock)
    {
        return;
    }
    ql_rtos_mutex_unlock(lock);
}

void ch395_list_init(ch395_list_t *header)
{
    header->next = NULL;
}

bool ch395_list_add_tail(ch395_list_t *header,uint8_t *data,uint32_t len)
{
    /* Create new node and add it to tail. */
    ch395_elem_t *elem = (ch395_elem_t *)calloc(1,sizeof(ch395_elem_t));
    if(!elem)
    {
        return false;
    }
    elem->list.next = NULL;
    elem->data = (uint8_t *)calloc(1,len);
    if(!(elem->data))
    {
        return false;
    }
    memcpy(elem->data,data,len);
    elem->len = len;
    ch395_list_t *p = header;
    while(p->next)
    {
        p = p->next;
    }
    p->next = &elem->list;
    return true;
};

bool ch395_list_is_empty(ch395_list_t *header)
{
    return !(header->next);
}

bool ch395_list_get_head(ch395_list_t *header,uint8_t *data,uint32_t *len)
{
    /* Get and delete the first node. */
    if(ch395_list_is_empty(header))
    {
        return false;
    }
    ch395_list_t *p = header;
    ch395_elem_t *elem = (ch395_elem_t*)(header->next);
    memcpy(data,elem->data,elem->len);
    *len = elem->len;
    p->next = elem->list.next;
    free(elem->data);
    free(elem);
    return true;
}

void ch395_app_send_event(ql_task_t task,uint32_t id, uint32_t param1, uint32_t param2, uint32_t param3)
{
    ql_event_t msg;
    msg.id = id;
    msg.param1 = param1;
    msg.param2 = param2;
    msg.param3 = param3;
    ql_rtos_event_send(task, &msg);
}

bool ch395_app_check_if_no_err(uint8_t err)
{
    if (err == CMD_ERR_SUCCESS) return true;                           /* 操作成功 */
    CH395_DEMO_LOG("CH395 Error: %02X\n", (uint16_t)err);                 /* 显示错误 */
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    int ret = 0;
    switch(err)
    {   
        //硬件错误
        default:
        case CH395_ERR_UNKNOW:
        {
            ret = ql_rtos_timer_start(manager->timer, CH395_ERR_UNKNOW_RETRY_TIME, 0);
            if (ret != QL_OSI_SUCCESS)
            {
                CH395_DEMO_LOG("timer start failed");
                ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_RESET,0,0,0);
            }
            break;
        }
        //CH395内部socket错误
        case CH395_ERR_OPEN:
        {
            CH395CloseSocket(CH395_SOCKET_INDEX);
            ret = ql_rtos_timer_start(manager->timer, CH395_ERR_OPEN_RETRY_TIME, 0);
            if (ret != QL_OSI_SUCCESS)
            {
                CH395_DEMO_LOG("timer start failed");
                ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_CONNECT,0,0,0);
            }
            break;
        }
    }
    return false;
}

void InitCH395InfParam(void)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    memset(&CH395Inf,0,sizeof(CH395Inf));                                 /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,manager->ip4,sizeof(CH395IPAddr));             /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,manager->gw,sizeof(CH395GWIPAddr));          /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,manager->netmask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */

    CH395CMDGetMACAddr(CH395Inf.MacAddr);
    CH395_DEMO_LOG("CH395EVT:get mac %02x:%02x:%02x:%02x:%02x:%02x",
    CH395Inf.MacAddr[0],CH395Inf.MacAddr[1],CH395Inf.MacAddr[2],
    CH395Inf.MacAddr[3],CH395Inf.MacAddr[4],CH395Inf.MacAddr[5]);
    memcpy((void*)manager->mac,(void*)CH395Inf.MacAddr,sizeof(manager->mac));
}

void InitSocketParam(void)
{
    memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
    SockInf.ProtoType = PROTO_TYPE_MAC_RAW;                          /* MAC RAW模式 */
}

uint8_t CH395SocketInitOpen(void)
{
    uint8_t i;
     /* socket 0为MAC RAW模式 */
    CH395SetSocketProtType(CH395_SOCKET_INDEX,SockInf.ProtoType);                     /* 设置socket 0协议类型 */
    i = CH395OpenSocket(CH395_SOCKET_INDEX);                                          /* 打开socket 0 */
    return i;
}

void CH395SocketInterrupt(uint8_t sockindex)
{
    uint8_t  sock_int_socket;
    uint16_t len;
    ch395_demo_manager_s *manager = &ch395_demo_manager;
    sock_int_socket = CH395GetSocketInt(sockindex);                   /* 获取socket 的中断状态 */
    CH395_DEMO_LOG("ch395_sock_int_socket:0x%x",sock_int_socket);

    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* 发送缓冲区空闲，可以继续写入要发送的数据 */
    {
        ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_DATA_OUTPUT,0,0,0);
    }

    if(sock_int_socket & SINT_STAT_SEND_OK)                           /* 发送完成中断 */
    {
        //should be disabled
    }
    if(sock_int_socket & SINT_STAT_RECV)                              /* 接收中断 */
    {
        len = CH395GetRecvLength(sockindex);                          /* 获取当前缓冲区内数据长度 */
        CH395_DEMO_LOG("receive len = %d",len);
        if(len == 0)
        {
            return;
        }
        if(len > CH395_BUFFER_SIZE)
        {
            len = CH395_BUFFER_SIZE;
        }                                                             /*MyBuffer缓冲区长度为1514，*/
        CH395GetRecvData(sockindex,len,MyBuffer);                     /* 读取数据 */
        ql_ethernet_data_input(MyBuffer,len);
    }
}

void CH395GlobalInterrupt(void)
{
    uint16_t  init_status;
    uint8_t  buf[10]; 
    ch395_demo_manager_s *manager = &ch395_demo_manager;
    init_status = CH395CMDGetGlobIntStatus_ALL();
    CH395_DEMO_LOG("ch395_init_status 0x%x",init_status);
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
        CH395_DEMO_LOG("Init status : GINT_STAT_PHY_CHANGE\n");
        ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_STATUS_CHECK,0,0,0);
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
}

uint8_t  CH395Init(void)
{
    CH395_DEMO_LOG("CH395init");
    uint8_t i;
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

void ch395_output(uint8_t * data,uint32_t len)
{
    if(len > CH395_BUFFER_SIZE || len == 0)
    {
        CH395_DEMO_LOG("CH395_drop %d",len);
        return;
    }
    ch395_demo_manager_s *manager = &ch395_demo_manager;

    //Put packet to cache.And send it later
    ch395_try_lock(list_lock);
    bool send = false;
    if(ch395_list_is_empty(&list_header))
    {
        send = true;
    }
    ch395_list_add_tail(&list_header,data,len);
    ch395_unlock(list_lock);

    CH395_DEMO_LOG("CH395_output,%d",send);

    if(true == send)
    {
        ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_DATA_OUTPUT,0,0,0);
    }
}

void _gpioint_cb(void *ctx)
{
    ch395_demo_manager_s *manager = &ch395_demo_manager;
    //INT# from 1 to 0
    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_INT,0,0,0);
    CH395_DEMO_LOG("CH395_gpioint_cb");
}

void ch395_app_reset(void)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    ql_rtos_timer_stop(manager->timer);
    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_RESET,0,0,0);
}

ch395_app_gpio_cb_t ch395_app_get_gpio_cb(void)
{
    return _gpioint_cb;
}
ch395_app_net_status_e ch395_app_net_status_get()
{
    uint8_t status = CH395CMDGetPHYStatus();
    uint8_t flag = PHY_DISCONN|PHY_10M_FLL|PHY_10M_HALF|PHY_100M_FLL|PHY_100M_HALF|PHY_AUTO;
    CH395_DEMO_LOG("ch395 get status %x",status);
    if(status == PHY_DISCONN)                     /* 查询CH395是否连接 */
    {
        return CH395_APP_NET_DISCONNECTED;
    }
    else
    {
        if(status & (~(flag)) || (status & (status - 1)) != 0)
        {
            return CH395_APP_NET_NONE;
        }
        else
        {
            return CH395_APP_NET_CONNECTED;
        }
    }
    return CH395_APP_NET_NONE;
}

void ch395_app_net_status_notify(ch395_app_net_status_e status)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    manager->status = status;
    if(manager->notify_cb)
    {
        manager->notify_cb(&status);
    }
}

static void ch395_app_net_status_cb(void* ctx)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    ql_ethernet_net_status_e *pCtx = (ql_ethernet_net_status_e*)ctx;
    ch395_app_net_status_e status = CH395_APP_NET_NONE;
    /* IP is configured successfully. */
    if(QL_ETHERNET_NET_STATUS_CONNECTED == *pCtx)
    {
        status = CH395_APP_NET_CONNECTED;
        if(2 == manager->dhcp)
        {
            /* Set dhcp server start. */
            uint8_t enable = 1;
            int ret = 0;
            ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_DHCP_SERVER,&enable,sizeof(uint8_t));
            if(QL_ETHERNET_SUCCESS != ret)
            {
                CH395_DEMO_LOG("opt set err %x",ret);
            }
        }
    }
    else if(QL_ETHERNET_NET_STATUS_DISCONNECTED == *pCtx)
    {
        status = CH395_APP_NET_DISCONNECTED;
    }
    if(status != manager->status)
    {
        ch395_app_net_status_notify(status);
    }
}

static void ch395_app_timer_cb(void *ctx)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    CH395_DEMO_LOG("ch395 timer expired,id %x",manager->curr_id);
    if(manager->curr_id == QUEC_ETHERNET_DRV_TRY_RESET 
    || manager->curr_id == QUEC_ETHERNET_DRV_TRY_CONNECT
    || manager->curr_id == QUEC_ETHERNET_DRV_STATUS_CHECK)
    {
        ch395_app_send_event(manager->task,manager->curr_id,0,0,0);
    }
    return;
}

static bool ch395_app_set_ctx(ch395_demo_manager_s* manager,ql_ethernet_ctx_s *ctx)
{
    int ret = false;
    do
    {
        if(!manager || !ctx)
        {
            break;
        }
        /* Mode must be set before all setting. */
        ql_ethernet_mode_e mode = (ql_ethernet_mode_e)manager->mode;
        ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_MODE,&mode,sizeof(ql_ethernet_mode_e));
        if(QL_ETHERNET_SUCCESS != ret)
        {
            CH395_DEMO_LOG("opt set err %x",ret);
            break;
        }
        if(QL_ETHERNET_MODE_ROUTER == manager->mode || QL_ETHERNET_MODE_AP == manager->mode)
        {
            if(manager->cover)
            {
                ctx->option.cover = manager->cover;
                /* Cid must be set in QL_ETHERNET_MODE_ROUTER mode. */
                if(QL_ETHERNET_MODE_ROUTER == manager->mode)
                {
                    ctx->option.ecid.enable = true;
                }
                else
                {
                    ctx->option.ecid.enable = false;
                }
                ctx->option.ecid.sim_id = manager->sim_id;
                ctx->option.ecid.cid = manager->profile_idx;
                /* Set dhcp. */
                ctx->option.dhcp = manager->dhcp;
                /* Set prefer. */
                ctx->option.prefer = manager->prefer;
            }
            else
            {
                /* Cid must be set in QL_ETHERNET_MODE_ROUTER mode. */
                ql_ethernet_cid_s cid = {.enable = true,.sim_id = manager->sim_id,.cid = manager->profile_idx};
                ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_CID,&cid,sizeof(ql_ethernet_cid_s));
                if(QL_ETHERNET_SUCCESS != ret)
                {
                    CH395_DEMO_LOG("opt set err %x",ret);
                    break;
                }
                /* Set dhcp. */
                ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_DHCP,&manager->dhcp,sizeof(uint8_t));
                if(QL_ETHERNET_SUCCESS != ret)
                {
                    CH395_DEMO_LOG("opt set err %x",ret);
                    break;
                }
                /* Set prefer. */
                ql_ethernet_route_prefer_e prefer = manager->prefer;
                ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_ROUTE_PREFER,&prefer,sizeof(ql_ethernet_route_prefer_e));
                if(QL_ETHERNET_SUCCESS != ret)
                {
                    CH395_DEMO_LOG("opt set err %x",ret);
                    break;
                }
            }
            ret = true;
        }
        else if(QL_ETHERNET_MODE_NONE == manager->mode)
        {
            if(manager->cover)
            {
                ctx->option.cover = manager->cover;
                ctx->option.dhcp = manager->dhcp;
            }
            else
            {
                /* Set dhcp. */
                ret = ql_ethernet_opt_set(QL_ETHERNET_OPT_DHCP,&manager->dhcp,sizeof(uint8_t));
                if(QL_ETHERNET_SUCCESS != ret)
                {
                    CH395_DEMO_LOG("opt set err %x",ret);
                    break;
                }
            }
            ret = true;
        }
    }
    while(0);
    return ret;
} 

void ch395_thread(void * arg)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    ch395_create_mutex(&list_lock);
    ch395_create_mutex(&(manager->mutex));
    /* Create a timer for status checking. */
    int ret = ql_rtos_timer_create(&manager->timer, manager->task, ch395_app_timer_cb, NULL);
    if (ret != QL_OSI_SUCCESS)
    {
        CH395_DEMO_LOG("create timer ret: %x", ret);
        goto exit;
    }
    /* Try to reset the ch395. */
    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_RESET,0,0,0);

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
        CH395_DEMO_LOG("ch395 read task event:%x",event.id);
        switch(event.id)
        {
            case QUEC_ETHERNET_DRV_TRY_RESET:
            {
                manager->curr_id = event.id;
                /* Notify reseting. */
                ch395_app_net_status_notify(CH395_APP_NET_RESETING);
                /* Better add the hardware reset in reset callback. */
                if(manager->reset_cb)
                {
                    manager->reset_cb(NULL);
                }
                /* Try ch395 software reset. */
                uint8_t i = 0;
                CH395CMDReset();
                ql_rtos_task_sleep_ms(200);
                CH395SetStartPara(SOCK_DISABLE_SEND_OK_INT);
                i = CH395Init();
                if(ch395_app_check_if_no_err(i))
                {
                    CH395_DEMO_LOG("ch395 try connect");
                    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_CONNECT,0,0,0);
                }
                break;
            }
            case QUEC_ETHERNET_DRV_TRY_CONNECT:
            {
                manager->curr_id = event.id;
                ch395_app_net_status_notify(CH395_APP_NET_CONNECTING);
                uint8_t i = 0;
                InitCH395InfParam();                                             /* 初始化CH395相关变量 */
                InitSocketParam();                                        /* 初始化socket相关变量 */

                CH395SetTCPMss(TCP_MSS);   /*设置MSS为1360*/

                /*socket 0接受缓冲区可选快为0-7，每个块的长度为 512 字节。此处设置2k缓冲区, 接收缓冲区的长度不得小于 TCP MSS*/
                CH395SetSocketRecvBuf(CH395_SOCKET_INDEX, 0, 4);
                /*socket 0接受缓冲区可选快为8-12，每个块的长度为 512 字节。此处设置2k缓冲区, 发送缓冲区的长度不得小于 TCP MSS*/
                CH395SetSocketSendBuf(CH395_SOCKET_INDEX, 8, 4);

                i = CH395SocketInitOpen();
                if(ch395_app_check_if_no_err(i))
                {
                    CH395_DEMO_LOG("ch395 check connect status");
                    ql_rtos_timer_start(manager->timer, CH395_CHECK_STATUS_TIME, 0);
                }
                break;
            }
            case QUEC_ETHERNET_DRV_NET_CREATE:
            {
                ql_ethernet_errcode_e ret = 0;
                ql_ethernet_ctx_s ctx = 
                {
                    .ip4 =          manager->ip4,
                    .gw =           manager->gw,
                    .netmask =      manager->netmask,
                    .mac =          manager->mac,
                    .mode =         manager->mode,
                    .dns1 =         manager->dns1,
                    .dns2 =         manager->dns2,
                    /* This cb is used for receiving the ethernet data. */
                    .cb =           ch395_output,
                    /* This net_cb is used for receiving the ip configuration message. */
                    .net_cb =       ch395_app_net_status_cb,
                };
                bool err_flag = true;
                do
                {
                    if(!ch395_app_set_ctx(manager,&ctx))
                    {
                        CH395_DEMO_LOG("ctx set err %x",ret);
                        break;
                    }
                    ret = ql_ethernet_register(&ctx);
                    if(QL_ETHERNET_SUCCESS != ret && QL_ETHERNET_REPEAT_REGISTER_ERR != ret)
                    {
                        CH395_DEMO_LOG("net create err %x",ret);
                        break;
                    }
                    err_flag = false;
                }while(0);

                if(err_flag)
                {
                    /* Some error occurs.Sleep for while and try again. */
                    CH395_DEMO_LOG("err,try again");
                    ql_ethernet_deregister();
                    ql_rtos_task_sleep_ms(200);
                    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_NET_CREATE,0,0,0);
                    break;
                }
                ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_NET_UP,0,0,0);
                break;
            }
            case QUEC_ETHERNET_DRV_NET_UP:
            {
                ql_ethernet_set_up();
                /* Wait for ch395_app_net_status_cb. Set CH395_APP_NET_CONNECTED/CH395_APP_NET_DISCONNECTED in it. */
                break;
            }
            case QUEC_ETHERNET_DRV_NET_DOWN:
            {
                ql_ethernet_set_down();
                ql_ethernet_deregister();
                ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_NET_CREATE,0,0,0);
                break;
            }
            case QUEC_ETHERNET_DRV_INT:
            {
                CH395GlobalInterrupt();
                break;
            }
            case QUEC_ETHERNET_DRV_DATA_OUTPUT:
            {
                //Send one packet to SPI each time.
                ch395_try_lock(list_lock);
                if(!ch395_list_is_empty(&list_header))
                {
                    ch395_list_get_head(&list_header,SendBuffer,&SendBufferLen);
                    CH395SendData(CH395_SOCKET_INDEX,(uint8_t*)SendBuffer,SendBufferLen);
                    CH395_DEMO_LOG("ch395 packet %d sent",SendBufferLen);
                }
                ch395_unlock(list_lock);
                break;
            }
            case QUEC_ETHERNET_DRV_STATUS_CHECK:
            {
                manager->curr_id = QUEC_ETHERNET_DRV_STATUS_CHECK;
                ch395_app_net_status_e status = ch395_app_net_status_get();
                CH395_DEMO_LOG("ch395 status %d",status);
                //Error status,try reset after reaching CH395_CHECK_STATUS_RESET_CNT times.
                if(status == CH395_APP_NET_NONE)
                {
                    manager->status_reset_cnt++;
                    if(manager->status_reset_cnt >= CH395_CHECK_STATUS_RESET_CNT)
                    {
                        manager->status_reset_cnt = 0;
                        ql_rtos_timer_stop(manager->timer);
                        ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_TRY_RESET,0,0,0);
                        CH395_DEMO_LOG("ch395 status read error at %d times,try reset",manager->status_reset_cnt);
                    }
                    break;
                }
                else
                {
                    manager->status_reset_cnt = 0;
                }

                if (manager->status == CH395_APP_NET_CONNECTING && status == CH395_APP_NET_CONNECTED)
                {
                    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_NET_CREATE,0,0,0);
                } 
                else if (manager->status == CH395_APP_NET_DISCONNECTED && status == CH395_APP_NET_CONNECTED)
                {
                    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_NET_DOWN,0,0,0);
                    break;
                }

                //Keep checking status no matter connected or disconnect
                uint32_t check_time = (status == CH395_APP_NET_CONNECTED)?CH395_CONNECTED_CHECK_STATUS_TIME:CH395_CHECK_STATUS_TIME;
                CH395_DEMO_LOG("ch395 check time %ds",check_time/1000);
                ret = ql_rtos_timer_start(manager->timer, check_time, 0);
                if (ret != QL_OSI_SUCCESS)
                {
                    CH395_DEMO_LOG("timer start failed");
                    ch395_app_send_event(manager->task,QUEC_ETHERNET_DRV_STATUS_CHECK,0,0,0);
                }

                //Only notify status to upper layer when status changes.
                if(manager->status != status)
                {
                    CH395_DEMO_LOG("ch395 status from %d to %d",manager->status,status);
                    ch395_app_net_status_notify(status);
                }
                break;
            }
            default:
            {
                break;
            }
        }
	}
exit:
    ch395_delete_mutex(list_lock);
    ch395_app_deinit(NULL);
}

bool ch395_app_cb_register(ch395_app_cb_type_e type,void* cb)
{
    if(type >= CH395_APP_CB_TYPE_MAX)
    {
        return false;
    }
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    switch(type)
    {
        case CH395_APP_CB_TYPE_RESET:
        {
            manager->reset_cb = cb;
            break;
        }
        case CH395_APP_CB_TYPE_NOTIFY:
        {
            manager->notify_cb = cb;
            break;
        }
        default:break;
    }
    return true;
}

bool ch395_app_init(void* argv)
{
    ql_ethernet_ctx_s *ctx = (ql_ethernet_ctx_s*)argv;
    bool ret = false;
    if(!ctx)
    {
        CH395_DEMO_LOG("ctx null");
        goto exit;
    }
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    uint8_t def_ip4[4] = {192,168,1,100};
    uint8_t def_gw[4] = {192,168,1,0};
    uint8_t def_netmask[4] = {255,255,255,0};
    uint8_t def_dns1[4] = {8,8,8,8};
    uint8_t def_dns2[4] = {114,114,114,114};
    /* Set ip. */
    memcpy((void*)(manager->ip4),(NULL == ctx->ip4)?def_ip4:ctx->ip4,sizeof(manager->ip4));
    /* Set gw. */
    memcpy((void*)(manager->gw),(NULL == ctx->gw)?def_gw:ctx->gw,sizeof(manager->gw));
    /* Set netmask. */
    memcpy((void*)(manager->netmask),(NULL == ctx->netmask)?def_netmask:ctx->netmask,sizeof(manager->netmask));
    /* No need to set mac.If we do want to change the mac of CH395,set in InitCH395InfParam. */
    memset((void*)(manager->mac),0,sizeof(manager->mac));
    /* Set dns. If no dns parameter,set default dns. */
    memcpy((void*)(manager->dns1),(NULL == ctx->dns1)? def_dns1:ctx->dns1,sizeof(manager->dns1));
    memcpy((void*)(manager->dns2),(NULL == ctx->dns2)? def_dns2:ctx->dns2,sizeof(manager->dns2));
    /* Set mode. */
    if(ctx->mode < QL_ETHERNET_MODE_MAX)
    {
        manager->mode = ctx->mode;
        /* Set the necessary parameters. */
        manager->cover = ctx->option.cover;
        manager->dhcp = ctx->option.dhcp;
        manager->prefer = ctx->option.prefer;
        manager->sim_id = ctx->option.ecid.sim_id;
        manager->profile_idx = ctx->option.ecid.cid;
    }
    else
    {
        CH395_DEMO_LOG("mode err");
        goto exit;
    }

    QlOSStatus err = QL_OSI_SUCCESS;
    err = ql_rtos_task_create(&(manager->task), 8*1024, APP_PRIORITY_REALTIME, "ch395_task", ch395_thread, NULL, 100);
    if(err != QL_OSI_SUCCESS)
    {
        CH395_DEMO_LOG("task created failed");
        goto exit;
    }
    ret = true;
exit:
    return ret;
}

bool ch395_app_deinit(void* argv)
{
    ch395_demo_manager_s* manager = &ch395_demo_manager;
    ch395_demo_manager_s cmp = {0};
    if(0 != memcmp(manager,&cmp,sizeof(ch395_demo_manager_s)))
    {
        ql_ethernet_deregister();
        if(manager->timer)
        {
            ql_rtos_timer_delete(manager->timer);
        }
        if(manager->task)
        {
            ql_rtos_task_delete(manager->task);
            manager->task = NULL;
        }
        memset(manager,0,sizeof(ch395_demo_manager_s));
        CH395CMDSleep();
    }
    else
    {
        CH395_DEMO_LOG("ch395 already deinit");
    }
    return true;
}