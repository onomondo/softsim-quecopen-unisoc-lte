/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: SoftAP适配层接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_SOFTAP_ADAPTER_H
#define HILINK_OPEN_SOFTAP_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 获取广播IP
 * broadcastIp表示存放IP的缓冲
 * len表示存放IP的缓冲长度
 * 返回0表示成功，返回-1表示失败
 * 注意: broadcastIp为点分十进制格式
 */
int HILINK_GetBroadcastIp(char *broadcastIp, unsigned char len);

/*
 * 将网卡切为AP模式并开启SoftAP热点
 * ssid 表示用于创建SoftAP的ssid
 * ssidLen表示ssid长度, 最大取值64
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_StartSoftAp(const char *ssid, unsigned int ssidLen);

/*
 * 关闭SoftAP热点并将网卡切回station模式
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_StopSoftAp(void);

/*
 * 靠近发现功能初始化
 * power 表示设备表面功率
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NearDiscoveryInit(signed char power);

/*
 * 靠近发现功能关闭
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NearDiscoveryUninit(void);

#ifdef __cplusplus
}
#endif
#endif
