/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层网络Socket接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_SOCKET_ADAPTER_H
#define HILINK_OPEN_SOCKET_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_IP_LEN   40

/* udp读取数据参数结构体 */
typedef struct {
    int fd;                 /* UDP通讯套接字 */
    unsigned char *buf;     /* 存放数据的缓冲 */
    unsigned short len;     /* 要读取的长度 */
    char *remoteIp;              /* 远端ip */
    unsigned short remoteIpLen;  /* 远端ip长度 */
    unsigned short *remotePort;   /* 远端端口 */
} UdpRecvParam;

/*
 * 获取远端主机名
 * 参数hostName 远端主机名称, url
 * 参数ipList 存放远端主机IP地址列表的数组
 * 参数num 存放远端主机IP地址列表的数组的大小
 * 返回0 成功, 其他 失败
 * 注意Contiki OS下该接口为异步获取, 失败后需再次获取
 */
int HILINK_GetHostByName(const char *hostName, char ipList[][MAX_IP_LEN], int num);

/*
 * 创建UDP通讯套接字
 * 参数localPort UDP通信端口号
 * 返回值: 返回值小于0 创建失败, 大于等于0 UDP套接字fd
 */
int HILINK_CreateUdp(unsigned short localPort);

/*
 * 关闭UDP通讯套接字
 * 输入参数int fd UDP通讯套接字
 */
void HILINK_RemoveUdp(int fd);

/*
 * 用于UDP发送数据
 * 参数fd UDP通讯套接字
 * 参数buf 发送的数据
 * 参数len 数据长度
 * 参数remoteIp 目的IP
 * 参数remotePort 目的端口
 * 返回值小于0 发送失败, 大于等于0 实际发送的字节数
 */
int HILINK_SendUdp(int fd, const unsigned char *buf, unsigned short len, const char *remoteIp,
    unsigned short remotePort);

/*
 * UDP读取数据
 * 参数udpRecvParam udp读取数据参数结构体
 */
int HILINK_RecvUdp(UdpRecvParam *udpRecvParam);

/*
 * 建立TCP连接
 * 参数dst 目的IP
 * 参数port 目的端口
 * 返回值小于0 连接失败, 大于等于0 TCP套接字
 */
int HILINK_ConnectTcp(const char *dst, unsigned short port);

/*
 * TCP连接状态获取
 * 参数fd TCP套接字
 * 返回0 连接正常; 非0 连接失败, 具体参见 hilink_socket_error_t定义
 */
int HILINK_GetTcpState(int fd);

/*
 * 断开TCP连接
 * 参数fd TCP套接字
 * 输入参数 int fd  TCP 套接字
 */
void HILINK_DisconnectTcp(int fd);

/*
 * TCP发送数据
 * 参数fd TCP通讯套接字
 * 参数buf 发送的数据
 * 参数len 数据长度
 * 返回值小于0 发送失败, 大于等于0 实际发送的字节数
 */
int HILINK_SendTcp(int fd, const unsigned char *buf, unsigned short len);

/*
 * 用于TCP读取数据
 * 参数fd TCP通讯套接字
 * 参数buf 存放数据的缓冲
 * 参数len 要读取的长度
 * 返回值小于0 读取失败, 大于等于0 实际读取的字节数
 */
int HILINK_RecvTcp(int fd, unsigned char *buf, unsigned short len);

/*
 * 短整形主机字节序转网络字节序
 * 参数hs 主机字节序
 * 返回网络字节序数字
 */
unsigned short HILINK_Htons(unsigned short hs);

/*
 * 短整形网络字节序转主机字节序
 * 参数ns 网络字节序
 * 返回主机字节序数字
 */
unsigned short HILINK_Ntohs(unsigned short ns);

/*
 * 获取TCP发送缓冲区剩余缓冲长度
 * 参数fd TCP通讯套接字
 * 参数taskType 0表示多任务系统，m2m运行于独立的task中;1表示单任务系统，m2m和其他任务运行在同个task中
 * 参数bufLeft 发送缓冲区剩余缓冲长度
 * 参数dataMax 数据payload的最大长度
 * 返回0表示成功，返回其他表示失败
 */
int HILINK_GetLeftTcpSendbuf(int fd, int *taskType, unsigned int *bufLeft, unsigned int *dataMax);

/*
 * 移除多余信道
 * 参数fd UDP通讯套接字
 * 返回0表示成功，返回其他表示失败
 */
int HILINK_DeleteMultiGroupUdp(int fd);

/*
 * 获取网络操作失败时的错误码errno
 * 参数fd
 */
int HILINK_GetNetWorkError(int fd);

/*
 * 获取socket状态
 * 参数fd TCP套接字
 * 参数flag TCP状态,包括1可读状态;2可写状态;4异常状态
 * 参数usec 状态检测等待时间,即此函数需要在该时间限制内返回,单位微秒
 * 返回0表示成功，返回其他表示失败
 */
int HILINK_GetSocketState(int fd, unsigned int flag, unsigned int usec);

#ifdef __cplusplus
}
#endif

#endif