/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: SoftAP适配实现 (需设备厂商实现)
 */
#include "hilink_open_softap_adapter.h"

/*
 * 将网卡切为AP模式并开启SoftAP热点
 * ssid 表示用于创建SoftAP的ssid
 * ssidLen表示ssid长度, 最大取值64
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_StartSoftAp(const char *ssid, unsigned int ssidLen)
{
    return 0;
}

/*
 * 获取广播IP
 * broadcastIp表示存放IP的缓冲
 * len表示存放IP的缓冲长度
 * 返回0表示成功，返回-1表示失败
 * 注意: broadcastIp为点分十进制格式
 */
int HILINK_GetBroadcastIp(char *broadcastIp, unsigned char len)
{
    return 0;
}

/*
 * 关闭SoftAP热点并将网卡切回station模式
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_StopSoftAp(void)
{
    return 0;
}