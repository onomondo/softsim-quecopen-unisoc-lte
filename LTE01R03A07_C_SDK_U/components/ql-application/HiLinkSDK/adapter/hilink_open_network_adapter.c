/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 网络适配实现 (需设备厂商实现)
 */
#include "hilink_open_network_adapter.h"

/* 重启原因 */
typedef enum {
    HILINK_NORMAL_BOOT = 0,      /* 正常上电 */
    HILINK_WIFIDOG_REBOOT = 100, /* WIFI挂死重启 */
    HILINK_SOFTDOG_REBOOT = 101, /* HiLink任务挂死重启 */
    HILINK_HARDDOG_REBOOT = 102, /* 整机挂死导致的硬件狗复位重启 */
    HILINK_HOTA_REBOOT = 103,    /* Hota升级成功后重启 */
    HILINK_BOOT_REASON_BUTT      /* 获取失败 */
} HilinkBootReason;

/*
 * 获取本地IP
 * localIp表示存放IP的缓冲
 * len表示存放IP的缓冲长度
 * 返回0表示成功，返回-1表示失败
 * 注意: localIp为点分十进制格式
 */
int HILINK_GetLocalIp(char *localIp, unsigned char len)
{
    return 0;
}

/*
 * 获取网络MAC地址
 * MAC表示存放MAC地址的缓冲
 * len表示缓冲长度
 * 返回0表示成功，返回-1表示失败
 * 注意: MAC格式为a1b2c3d4e5f6
 */
int HILINK_GetMacAddr(unsigned char *mac, unsigned char len)
{
    return 0;
}

/*
 * 获取WiFi ssid
 * ssid表示存放WiFi ssid的缓冲
 * ssidLen表示WiFi ssid的长度
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_GetWiFiSsid(char *ssid, unsigned int *ssidLen)
{
    return 0;
}

/*
 * 设置WiFi账号信息
 * ssid表示WiFi ssid
 * ssidLen表示WiFi ssid的长度
 * pwd表示WiFi密码
 * pwdLen表示WiFi密码长度
 * 返回0表示成功，返回-1表示失败
 * 注意：(1) ssid和pwd为空表示清除WiFi信息;
 *       (2) 设置的WiFi信息需要持久化，以确保设备重启后依然可以获得WiFi配置信息
 */
int HILINK_SetWiFiInfo(const char *ssid, unsigned int ssidLen, const char *pwd, unsigned int pwdLen)
{
    return 0;
}

/* 断开并重连WiFi */
void HILINK_ReconnectWiFi(void)
{
    return;
}

/*
 * 连接WiFi
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_ConnectWiFi()
{
    return 0;
}

/*
 * 获取网络状态
 * state为0表示网络断开或已连接但网卡未分配得ip，state为1表示已连接且分配得ip
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_GetNetworkState(int *state)
{
    return 0;
}

/*
 * 获取当前连接的WiFi的 bssid
 * bssid表示存放WiFi bssid的缓冲
 * bssidLen表示WiFi bssid长度
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_GetWiFiBssid(unsigned char *bssid, unsigned char *bssidLen)
{
    return 0;
}

/*
 * 获取当前连接的WiFi信号强度，单位db
 * rssi表示信号强度
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_GetWiFiRssi(signed char *rssi)
{
    return 0;
}

/*
 * 重启HiLink SDK
 * 若系统不可重启，建议重启HiLink进程
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_Restart(void)
{
    return 0;
}

/* 设置SoftAP配网同时的最大接入station数
 * 用于PIN码配网功能中
 * 建议开发者设置SoftAP热点的最大允许接入STA数为2
 */
void HILINK_SetStationLimitNum(void)
{
    return;
}

/* SoftAP配网过程中，根据IP踢除对应的station
 * 用于PIN码配网功能中
 */
void HILINK_DisconnectStationByIp(const char *ip)
{
    return;
}

/*
 * 获取系统启动原因
 * 返回值: HILINK_NORMAL_BOOT = 0,      正常上电
 *         HILINK_WIFIDOG_REBOOT = 100, WIFI挂死重启
 *         HILINK_SOFTDOG_REBOOT = 101, hilink任务挂死重启
 *         HILINK_HARDDOG_REBOOT = 102, 整机挂死导致的硬件狗复位重启
 *         HILINK_HOTA_REBOOT = 103,    Hota升级成功后重启
 *         HILINK_BOOT_REASON_BUTT      获取失败
 */
unsigned char HILINK_GetSystemBootReason(void)
{
    return 0;
}