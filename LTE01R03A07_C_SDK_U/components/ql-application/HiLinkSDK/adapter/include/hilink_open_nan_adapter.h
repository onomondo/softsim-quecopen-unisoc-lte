/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: 系统适配层内存接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_MEM_ADAPTER_H
#define HILINK_OPEN_MEM_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * WIFI感知底层驱动初始化
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanInit(void);

/*
 * WIFI感知底层驱动去初始化
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanUninit(void);

/*
 * WIFI感知启动订阅服务
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanStartSubscribe(const char *serviceName, unsigned char localHandle, void *handle);

/*
 * WIFI感知停止订阅服务
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanStopSubscribe(unsigned char localHandle);

/*
 * WIFI感知发包接口
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSendPacket(unsigned char *macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char *msg, int len);

/*
 * WIFI感知切换到超短距状态
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSwitchSafeDistance(void);

/*
 * WIFI感知切换到正常天线状态
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSwitchNormalDistance(void);

/*
 * beacon帧开启和关闭
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanBeaconSwitch(unsigned char enable);

int HILINK_SoftApDeauthStation(const unsigned char *mac, unsigned char len);

typedef enum {
    EVT_AP_START = 1,
    EVT_STA_CONNECT,
    EVT_STA_DISCONNECT
} SoftApEventType;

typedef struct {
    char addr[6]; //Quectel larson.li MAC_ADDRESS_LEN 宏定义找不到，根据经验MAC LEN为6
} EventStaConnect;

typedef struct {
    unsigned char addr[6];//Quectel larson.li MAC_ADDRESS_LEN 宏定义找不到，根据经验MAC LEN为6
    unsigned short reasonCode;
} EventStaDisconnect;

typedef union {
    EventStaConnect staConnect;
    EventStaDisconnect staDisconnect;
} SoftApEventInfo;

typedef struct {
    SoftApEventType type;
    SoftApEventInfo info;
} SoftApEvent;

typedef void (*SoftApEventCallback)(const SoftApEvent *event);

/* 注册SoftAP事件处理回调函数 */
void HILINK_RegSoftApEventCallback(SoftApEventCallback callback);

#ifdef __cplusplus
}
#endif

#endif
