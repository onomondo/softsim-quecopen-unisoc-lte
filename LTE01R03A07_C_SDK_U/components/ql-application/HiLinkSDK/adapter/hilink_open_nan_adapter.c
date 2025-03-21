/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: 模组系统适配层内存接口函数(需设备厂商实现)
 */
#include "hilink_open_nan_adapter.h"
#include <stdlib.h>

void HILINK_RegSoftApEventCallback(SoftApEventCallback callback)
{
    return;
}

int HILINK_NanInit(void)
{
    return 0;
}

int HILINK_NanUninit(void)
{
    return 0;
}

int HILINK_NanStartSubscribe(const char *serviceName, unsigned char localHandle, void *handle)
{
    if ((serviceName == NULL) || (handle == NULL)) {
        return -1;
    }

    return 0;
}

int HILINK_NanStopSubscribe(unsigned char localHandle)
{
    return 0;
}

int HILINK_NanSendPacket(unsigned char *macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char *msg, int len)
{
    if ((macAddr == NULL) || (msg == NULL)) {
        return -1;
    }

    return 0;
}

int HILINK_NanSwitchSafeDistance(void)
{
    return 0;
}

int HILINK_NanSwitchNormalDistance(void)
{
    return 0;
}

int HILINK_NanBeaconSwitch(unsigned char enable)
{
    return 0;
}

int HILINK_SetSafeDistancePower(signed char power)
{
    return 0;
}

int HILINK_SoftApDeauthStation(const unsigned char *mac, unsigned char len)
{
    if (mac == NULL) {
        return -1;
    }

    return 0;
}
