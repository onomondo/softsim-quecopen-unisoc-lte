/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: 蓝牙SDK API头文件
 */
#ifndef HILINK_BT_API_H
#define HILINK_BT_API_H

#include "ble_cfg_net_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 广播最大长度 */
#define ADV_VALUE_MAX_LEN 31

/* 销毁蓝牙协议栈标志 */
#define BLE_DEINIT_FALG 0x1234ABCD

typedef enum {
    HILINK_BT_SDK_STATUS_SVC_RUNNING = 0, /* 正常运行 */
    HILINK_BT_SDK_STATUS_DEINIT, /* 注销 */
    HILINK_BT_SDK_STATUS_NAME_SET_ABNORM, /* 蓝牙名称设置异常 */
    HILINK_BT_SDK_STATUS_DISCOVER_MODE_SET_ABNORM, /* 蓝牙可发现模式设置异常 */
    HILINK_BT_SDK_STATUS_REG_APP_ABNORM, /* 注册BLE应用异常 */
    HILINK_BT_SDK_STATUS_SVC_CREATE_ABNORM, /* 服务创建异常 */
    HILINK_BT_SDK_STATUS_CHAR_ADD_ABNORM, /* 属性添加异常 */
    HILINK_BT_SDK_STATUS_DESC_ADD_ABNORM, /* 描述添加异常 */
    HILINK_BT_SDK_STATUS_SVC_START_ABNORM, /* 服务启动异常 */
    HILINK_BT_SDK_STATUS_ADV_PARA_SET_ABNORM, /* 广播参数设置异常 */
    HILINK_BT_SDK_STATUS_ADV_DATA_SET_ABNORM, /* 广播数据设置异常 */
    HILINK_BT_SDK_STATUS_ADV_START_ABNORM, /* 广播启动异常 */
} HILINK_BT_SdkStatus;

/* GATTS char属性取值 */
typedef enum {
    HILINK_BT_CHAR_PROP_WRITE = 0x08,
    HILINK_BT_CHAR_PROP_READ = 0x02,
    HILINK_BT_CHAR_PROP_NOTIFY = 0x10,
    HILINK_BT_CHAR_PROP_INDICATE = 0x20
} HILINK_BT_CharProperty;

/* GATTS char权限取值 */
typedef enum {
    HILINK_BT_CHAR_PERM_READ  = 0x01,
    HILINK_BT_CHAR_PERM_READ_ENCRYPTED = 0x02,
    HILINK_BT_CHAR_PERM_READ_ENCRYPTED_MITM = 0x04,
    HILINK_BT_CHAR_PERM_WRITE = 0x10,
    HILINK_BT_CHAR_PERM_WRITE_ENCRYPTED = 0x20,
    HILINK_BT_CHAR_PERM_WRITE_ENCRYPTED_MITM = 0x40,
    HILINK_BT_CHAR_PERM_WRITE_SIGNED = 0x80,
    HILINK_BT_CHAR_PERM_WRITE_SIGNED_MITM = 0x100,
} HILINK_BT_CharPermission;

/* GATTS desc属性取值 */
typedef enum {
    HILINK_BT_DESC_PERM_WRITE = 0x01,
    HILINK_BT_DESC_PERM_READ = 0x02
} HILINK_BT_DescPermission;

/* 属性值类型: 整型和属性 */
typedef enum {
    HILINK_BT_CMD_DATA_TYPE_INT,
    HILINK_BT_CMD_DATA_TYPE_STR,
} HILINK_BT_CmdDataType;

/* hilink蓝牙应用层数据编码类型 */
typedef enum {
    HILINK_BT_CMD_DATA_MODE_TLV = 0x00, /* TLV格式: 降低报文占用的空间 */
    HILINK_BT_CMD_DATA_MODE_JSON = 0x01 /* JSON格式: 扩展性更好，默认格式 */
} HILINK_BT_CmdDataMode;

/* 发送蓝牙SDK状态回调函数类型 */
typedef void (*HILINK_BT_SdkEventCallBack)(HILINK_BT_SdkStatus event, const void *param);

/* 发送蓝牙数据回调函数类型 */
typedef int (*HILINK_BT_SendBtDataCallback)(const unsigned char *buf, unsigned int len);

/* 自定义gatt服务读事件回调 */
typedef int (*HILINK_BT_GattReadCallback)(unsigned char *out, unsigned int *outLen);

/* 自定义gatt服务写事件回调 */
typedef int (*HILINK_BT_GattWriteCallback)(const unsigned char *in, unsigned int inLen);

/* 获取蓝牙mac地址的回调函数 */
typedef int (*HILINK_BT_GetBtMacCallBack)(const unsigned char *buf, unsigned int len);

/* 产品信息定义结构体 */
typedef struct {
    char *manuName;
    char *devName;
    char *productId;
    char *sn;
    char *mac;
    char *subProductId;
} HILINK_BT_DevInfo;

/* 产品功能命令定义结构体 */
typedef struct {
    unsigned char attrIdx;
    char *attr;
    HILINK_BT_CmdDataType dataType;
    int (*putFunc)(const void *data, unsigned int len);
    int (*getFunc)(void *buf, unsigned int *bufLen, unsigned int len);
} HILINK_BT_AttrInfo;

/* 产品功能定义结构体 */
typedef struct {
    unsigned char svcIdx;
    char *service;
    int (*putFunc)(const void *svc, const unsigned char *in, unsigned int inLen,
        unsigned char **out, unsigned int *outLen);
    int (*getFunc)(const void *svc, const unsigned char *in, unsigned int inLen,
        unsigned char **out, unsigned int *outLen);
    unsigned char attrNum;
    HILINK_BT_AttrInfo *attrInfo;
} HILINK_BT_SvcInfo;

/* 产品Profile定义结构体 */
typedef struct {
    unsigned int svcNum;
    HILINK_BT_SvcInfo *svcInfo;
} HILINK_BT_Profile;

/* 蓝牙gatt character描述 */
typedef struct {
    char *descUuid;
    /* gatt属性描述读写权限：取值由HILINK_BT_DescPermission类型的成员或运算得出 */
    unsigned int descPermission;
} HILINK_BT_GattProfileDesc;

/* 蓝牙gatt character */
typedef struct {
    char *charUuid;
    /* gatt char权限:取值由HILINK_BT_CharPermission类型的成员或运算得出 */
    unsigned int charPermission;
    /* gatt char属性:取值由HILINK_BT_CharProperty类型的成员或运算得出 */
    unsigned int charProperty;
    HILINK_BT_GattReadCallback readFunc;
    HILINK_BT_GattWriteCallback writeFunc;
    HILINK_BT_GattProfileDesc *desc;
    unsigned char descNum;
} HILINK_BT_GattProfileChar;

/* 蓝牙gatt 服务 */
typedef struct {
    char *svcUuid;
    int isPrimary;
    HILINK_BT_GattProfileChar *character;
    unsigned char charNum;
} HILINK_BT_GattProfileSvc;

/* 厂商自定义蓝牙gatt服务列表 */
typedef struct {
    HILINK_BT_GattProfileSvc *service;
    unsigned char serviceNum;
} HILINK_BT_GattServiceList;

/* 配置保存回调结构体 */
typedef struct {
    int (*createItem)(const char *name, unsigned int size);
    int (*readItem)(const char *name, unsigned char *buf, unsigned int len);
    int (*writeItem)(const char *name, const unsigned char *buf, unsigned int len);
    int (*deleteItem)(const char *name);
} HILINK_BT_ConfigInterface;

/* 获取广播数据结构体 */
typedef struct {
    unsigned int advSvcDataLen;
    unsigned char advSvcData[ADV_VALUE_MAX_LEN];
    unsigned int advRspDataLen;
    unsigned char advRspData[ADV_VALUE_MAX_LEN];
} HILINK_BT_AdvertiseData;

/* 设置应用层编码模式 */
int HILINK_BT_SetEncodeMode(HILINK_BT_CmdDataMode mode);

/* 查询应用层编码模式 */
HILINK_BT_CmdDataMode HILINK_BT_GetEncodeMode(void);

/* 初始化启动HiLink Bluetooth SDK */
int HILINK_BT_Init(const HILINK_BT_Profile *profile);

/* 启动HiLink BT SDK处理，调用HiLink协议栈 */
int HILINK_BT_Process(void);

/*
 * 结束HiLink Bluetooth SDK
 * flag为0：只销毁控制和调度线程，flag为1销毁蓝牙协议栈，该函数不可重入
 */
int HILINK_BT_DeInit(unsigned int flag);

/* 添加HiLink服务信息service信息 */
int HILINK_BT_AddHiLilnkService(const HILINK_BT_SvcInfo *serviceArray, unsigned int serviceNum);

/* 通知服务状态 */
int HILINK_BT_ReportServiceState(const void *service, const void *buf, unsigned int len);

/* 通知属性状态 */
int HILINK_BT_ReportAttrState(const void *svc, const void *attr, const void *buf, unsigned int len);

/* 处理蓝牙数据 */
int HILINK_BT_ProcessBtData(const unsigned char *buf, unsigned int len);

/* 注册蓝牙数据发送接口 */
int HILINK_BT_RegisterBtDataSendCallback(HILINK_BT_SendBtDataCallback callback);

/* 查询蓝牙数据发送接口 */
HILINK_BT_SendBtDataCallback HILINK_BT_GetBtDataSendCallback(void);

/* 初始化发送蓝牙mac地址的函数 */
int HILINK_BT_BtMacSendInit(HILINK_BT_GetBtMacCallBack callback);

/* 查询蓝牙mac地址的发送接口 */
HILINK_BT_GetBtMacCallBack HILINK_BT_GetBtMacSendCallback(void);

/* 设置蓝牙SDK事件处理函数 */
int HILINK_BT_SetSdkEventCallback(HILINK_BT_SdkEventCallBack callback);

/*
 * 设置BLE最大连接数量
 * 入参connNum的范围为[1,10]
 * 最大连接数上限为10，超过10个按10个执行
 * 最小连接数为1，小于1按1个执行
 * 若不调用该接口，默认最大连接数为1
 */
void HILINK_BT_SetMaxConnNum(int connNum);

/* 查询蓝牙SDK最大连接数量 */
int HILINK_BT_GetMaxConnNum(void);

/* 添加蓝牙SDK自定义gatt服务 */
int HILINK_BT_SetGattProfile(HILINK_BT_GattServiceList *gattServiceList);

/* 注册配置保存回调函数到HiLink Bluetooth SDK，若不调用该函数，则默认使用HiLink Bluetooth SDK保存配置实现 */
int HILINK_BT_RegisterConfigInterface(const HILINK_BT_ConfigInterface *interface);

/* 启动广播 */
int HILINK_BT_StartAdvertise(void);

/* 停止广播 */
int HILINK_BT_StopAdvertise(void);

/* 上报蓝牙反馈数据 */
int HILINK_BT_IndicateSvcCharData(const char *svcUuid, const char *charUuid, const char *buf, unsigned int len);

/*
 * 获取设备表面的最强点信号发射功率强度，最强点位置的确定以及功率测试方
 * 法，参照hilink认证蓝牙靠近发现功率设置及测试方法指导文档，power为出参
 * ，单位dbm，返回设备表面的最强信号强度值，如果厂商不想使用蓝牙靠近发现功
 * 能，接口直接返-1，如果需要使用蓝牙靠近发现，则接口返回0，如需及时生效，需
 * 调用HILINK_BT_StartAdvertise()方法启动广播
 */
int HILINK_BT_GetDevSurfacePower(char *power);

/*
 * 获取设备的子型号，长度固定两个字节
 * subProdId为保存子型号的缓冲区，len为缓冲区的长度
 * 如果产品定义有子型号，则填入两字节子型号，并以'\0'结束, 返回0
 * 没有定义子型号，则返回-1
 * 该接口需设备开发者实现
 * 与hilink sdk相同定义，双模组模式只需一份，已提供给第三方厂家，暂不按编程规范整改
 */
int HILINK_GetSubProdId(char *subProdId, int len);

/*
 * 获取设备SN
 * 与hilink sdk相同定义，双模组模式只需一份，已提供给第三方厂家，暂不按编程规范整改
 */
void HilinkGetDeviceSn(unsigned int len, char *sn);

/* 获取蓝牙SDK设备相关信息 */
HILINK_BT_DevInfo *HILINK_BT_GetDevInfo(void);

/* 获取靠近发现中广播数据 */
int HILINK_BT_GetAdvertiseData(HILINK_BT_AdvertiseData *advertiseData);

/* 获取BLE厂商注册的回调函数 */
BLE_CfgNetCb *GetBleCfgNetCallback(void);

/* 断开蓝牙连接时，释放通道占用资源 */
void HILINK_BT_DisconnectFreeResource(void);

/*
 * 厂家设置linux系统读写文件的路径,路径长度不能超过33个字节,包含结束符
 * 要求存储路径是非易失性的，版本升级不影响该路径文件
 */
void HILINK_BT_SetConfigDirPath(const char *configDirPath);

#ifdef __cplusplus
}
#endif
#endif
