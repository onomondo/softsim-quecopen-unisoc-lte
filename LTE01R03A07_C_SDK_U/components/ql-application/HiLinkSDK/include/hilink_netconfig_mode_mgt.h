/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: HiLink SDK配网模式管理
 */
#ifndef HILINK_NETCONFIG_MODE_MGT_H
#define HILINK_NETCONFIG_MODE_MGT_H

#ifdef __cplusplus
extern "C" {
#endif

/* HiLink SDK支持的配网模式 */
enum HILINK_NetConfigMode {
    HILINK_NETCONFIG_NONE, /* 不用配网, 通过网线等手段连接到网络 */
    HILINK_NETCONFIG_WIFI, /* HiLink SDK提供的WiFi自动配网 */
    HILINK_NETCONFIG_OTHER, /* 其他配网模式, APP发送WiFi的信息, 集成方收到WiFi信息数据后, 设置到HiLink SDK */
    HILINK_NETCONFIG_BOTH, /* 其他配网模式和WiFi配网组合 */
    HILINK_NETCONFIG_REGISTER_ONLY, /* HiLink SDK SoftAp配网仅接收注册信息 */
    HILINK_NETCONFIG_NO_SOFTAP_REGISTER_ONLY, /* 不启动SoftAp, PIN码配网仅接收注册信息(通过网线/4G/5G等接入网络) */
    HILINK_NETCONFIG_NAN_SOFTAP, /* WiFi感知超短距配网和SoftAp组合 */
    HILINK_NETCONFIG_BUTT /* 非法配网模式 */
};

/* 设置产品配网模式, 注意: 需要在启动HiLink SDK任务之前调用本接口设置配网模式 */
int HILINK_SetNetConfigMode(enum HILINK_NetConfigMode netConfigMode);

/* 查询当前产品的配网模式, 返回值为当前产品的配网模式 */
enum HILINK_NetConfigMode HILINK_GetNetConfigMode(void);

/*
 * 用于蓝牙辅助配网，蓝牙数据发送接口的函数类型
 * 参数buf表示待发送数据的缓冲区地址；len为待发送数据的长度
 * 返回0表示蓝牙数据发送成功，返回非0表示发送失败
 */
typedef int (*HILINK_BT_SendBtDataCallback)(const unsigned char *buf, unsigned int len);

/*
 * 用于蓝牙辅助配网，将蓝牙数据发送接口注册进HiLink SDK中
 * 参数callback表示被注册的蓝牙数据发送接口
 * 返回0表示回调函数注册成功，返回-1表示回调函数注册失败
 * 注意: 函数由设备开发者或厂商在调用hilink_main之前调用
 */
int HILINK_BT_RegisterBtDataSendCallback(HILINK_BT_SendBtDataCallback callback);

/*
 * 用于蓝牙辅助配网。当厂商通过蓝牙接收到配网数据后，调用此接口将数据透传给HiLink SDK
 * 参数buf表示接收到的数据包缓冲，len表示数据包缓冲大小
 * 返回0表示处理成功，返回-1表示处理失败
 */
int HILINK_BT_ProcessBtData(const unsigned char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
