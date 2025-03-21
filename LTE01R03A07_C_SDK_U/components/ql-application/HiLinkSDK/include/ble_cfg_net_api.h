/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: BLE辅助配网SDK API头文件
 */

#ifndef BLE_CFG_NET_API_H
#define BLE_CFG_NET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* WIFI信息长度宏定义 */
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PWD_MAX_LEN 64
#define WIFI_PSK_LEN 32
#define WIFI_BSSID_LEN 6

#define BLE_UUID_LEN 16

/* 用户发送的数据类型 */
typedef enum {
    NETCFG_DATA,
    CUSTOM_DATA,
    DATA_TYPE_BUTT
} BLE_DataType;

/* 属性类型定义 */
typedef enum {
    ATTR_TYPE_SERVICE = 0,
    ATTR_TYPE_CHAR,
    ATTR_TYPE_CHAR_VALUE,
    ATTR_TYPE_CHAR_CLIENT_CONFIG,
    ATTR_TYPE_CHAR_USER_DESC,
} BLE_AttrType;

/* UUID长度定义 */
typedef enum {
    UUID_TYPE_NULL = 0,
    UUID_TYPE_16_BIT,
    UUID_TYPE_32_BIT,
    UUID_TYPE_128_BIT,
} BLE_UuidType;

/* BLE辅助配网状态定义 */
typedef enum {
    CFG_NET_PROCESS_SUCCESS = 0x00,
    CFG_NET_BLE_CONNECT,
    CFG_NET_BLE_DIS_CONNECT,
    CFG_NET_SPEKE_SUCCESS,
    CFG_NET_PROCESS_START,
    CFG_NET_RECEIVE_PARA,
    CFG_NET_WIFI_CONNECT,
    CFG_NET_FAIL_UNKUNOWN = 0x100,
    CFG_NET_FAIL_WIFI_SSID,
    CFG_NET_FAIL_WIFI_PWD
} BLE_CfgNetStatus;

/* 手机侧传过来的WIFI信息，格式复用短距配网格式 */
typedef struct {
    unsigned char ssid[WIFI_SSID_MAX_LEN + 1];
    unsigned char pwd[WIFI_PWD_MAX_LEN + 1];
    unsigned char psk[WIFI_PSK_LEN + 1];
    unsigned char bssid[WIFI_BSSID_LEN + 1];
    unsigned char ssidLen;
    unsigned char pwdLen;
    unsigned char pskLen;
    unsigned char bssidLen;
    int authMode;
    int wifiInfoSrc;
    int channelNumber;
} BLE_WifiInfo;

/*
 * 获取设备PIN码函数类型
 * pincode: 存放pin码的缓冲区
 * size: 缓冲区的长度
 * len: 返回的pin码实际长度
 */
typedef int (*BLE_GetDevPinCode)(unsigned char *pinCode, unsigned int size, unsigned int *len);

/*
 * 获取设备信息函数类型，len即是入参也是出参，入参代表buff缓冲区长度，出参代表获取的设备信息实际长度
 * 格式要求：{"productId":"%s", "sn":"%s", "vendor":"%s"}
 */
typedef int (*BLE_GetDeviceInfo)(unsigned char *devInfo, unsigned int *len);

/*
 * 设置配网信息函数类型
.* 数据格式：{"ssid":"%s","password":"%s","devId":"%s","psk":"%s","code":"%s","random":"%s","vendorData":"%s"}
 */
typedef int (*BLE_SetCfgNetInfo)(const unsigned char *netInfo, unsigned int len);

/* 接收用户数据函数类型 */
typedef int (*BLE_RcvCustomData)(unsigned char *buff, unsigned int len);

/* 配网过程状态处理函数类型 */
typedef int (*BLE_CfgNetProcess)(BLE_CfgNetStatus status);

/* BLE GATT服务读函数类型 */
typedef int (*BLE_GattRead)(unsigned char *buff, unsigned int *len);

/* BLE GATT服务写函数类型 */
typedef int (*BLE_GattWrite)(const unsigned char *buff, unsigned int len);

/* BLE GATT服务指示函数类型 */
typedef int (*BLE_GattIndicate)(unsigned char *buff, unsigned int len);

/* BLE GATT回调函数指针 */
typedef struct {
    BLE_GattRead readCb;
    BLE_GattWrite writeCb;
    BLE_GattIndicate indicateCb;
} BLE_GattOperateFunc;

/* BLE配置参数 */
typedef struct {
    int isBlePair;
    int isDeinitBleStack;
    int data1; /* 为后期配置参数预留，暂不使用 */
    int data2; /* 为后期配置参数预留，暂不使用 */
    int data3; /* 为后期配置参数预留，暂不使用 */
} BLE_ConfPara;

/* BLE GATT服务 */
typedef struct {
    BLE_AttrType attrType;
    unsigned int permission;
    BLE_UuidType uuidType;
    unsigned char uuid[BLE_UUID_LEN];
    unsigned char *value;
    unsigned char valLen;
    unsigned char properties;
    BLE_GattOperateFunc func;
} BLE_GattAttr;

/* GATT服务(单个service及其下挂的全部characteristics和descriptions) */
typedef struct {
    unsigned int attrNum;
    BLE_GattAttr *attrList;
} BLE_GattService;

/* GATT列表(包含多个services和返回的handle) */
typedef struct {
    unsigned int num;
    BLE_GattService *service;
    int *handle;
} BLE_GattList;

/* GATT句柄列表 */
typedef struct {
    unsigned int num;
    int *handle;
} BLE_GattHandleList;

/* BLE的广播数据和扫描应答数据 */
typedef struct {
    unsigned char *advData;
    unsigned int advDataLen;
    unsigned char *rspData;
    unsigned int rspDataLen;
} BLE_AdvData;

/* BLE的广播参数 */
typedef struct {
    unsigned char advType;
    unsigned char discMode;
    unsigned char connMode;
    unsigned int minInterval;
    unsigned int maxInterval;
    unsigned int channelMap;
    unsigned int timeout;
    int txPower;
} BLE_AdvPara;

/* 广播参数和数据 */
typedef struct {
    BLE_AdvPara *advPara;
    BLE_AdvData *advData;
} BLE_AdvInfo;

/* BLE初始化参数 */
typedef struct {
    BLE_ConfPara *confPara;
    BLE_AdvInfo *advInfo;
    BLE_GattList *gattList;
} BLE_InitPara;

/* BLE配网回调函数 */
typedef struct {
    BLE_GetDevPinCode getDevPinCodeCb;
    BLE_GetDeviceInfo getDeviceInfoCb;
    BLE_SetCfgNetInfo setCfgNetInfoCb;
    BLE_RcvCustomData rcvCustomDataCb;
    BLE_CfgNetProcess cfgNetProcessCb;
} BLE_CfgNetCb;

/* BLE配网资源申请：BLE协议栈启动、配网回调函数挂接 */
int BLE_CfgNetInit(BLE_InitPara *para, BLE_CfgNetCb *cb);

/*
 * BLE配网资源注销：配网回调函数清理、BLE协议栈销毁
 * flag为0：只销毁控制和调度线程，flag为1销毁蓝牙协议栈，该函数不可重入
 */
int BLE_CfgNetDeInit(const BLE_GattHandleList *handleList, unsigned int flag);

/* BLE配网广播控制：参数代表广播时间，0:停止；0xFFFFFFFF:一直广播，其他：广播指定时间后停止，单位秒 */
int BLE_CfgNetAdvCtrl(unsigned int advSecond);

/*
 * 更新广播参数，更新完成后需调用BLE_CfgNetAdvCtrl启动广播
 * 传入空值时可启动hilink构造的广播
 */
int BLE_CfgNetAdvUpdate(const BLE_AdvInfo *advInfo);

/* BLE配网断开连接：防止其他任务长时间占用BLE连接 */
int BLE_CfgNetDisConnect(void);

/* BLE发送用户数据：用户数据发送，与接收回调函数配套使用 */
int BLE_SendCustomData(BLE_DataType dataType, const unsigned char *buff, unsigned int len);

#ifdef __cplusplus
}
#endif
#endif
