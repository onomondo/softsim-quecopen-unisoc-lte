/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: HiLink主流程框架集成头文件
 * Create: 2018-06-22
 */
#ifndef _HILINK_H_
#define _HILINK_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * HiLink SDK属性结构体，开发者可以通过HILINK_GetSdkAttr查看当前系统的属性值，通过HILINK_SetSdkAttr设置新的属性值；
 * 注意: 1) 普通设备形态和网桥设备形态的主线程任务栈大小，开发者根据产品形态，仅需设置对应产品形态的属性值即可；
 *       2) 使用HiLink SDK升级架构时，需要设置升级检查任务的栈大小和升级任务的栈大小
 *       3) 如果开发者未注册软重启接口rebootSoftware和硬重启接口rebootHardware，使用HiLink SDK默认实现接口(硬重启)；
 */
typedef struct {
    unsigned long monitorTaskStackSize;    /* 监控任务栈大小，开发者根据具体情况调整，默认为1024字节 */
    unsigned long deviceMainTaskStackSize; /* 普通设备形态，HiLink SDK运行主任务栈大小，开发者根据具体情况调整 */
    unsigned long bridgeMainTaskStackSize; /* 网桥设备形态，HiLink SDK运行主任务栈大小，开发者根据具体情况调整 */
    unsigned long otaCheckTaskStackSize;   /* HiLink OTA检查升级版本任务栈大小，开发者根据具体情况调整 */
    unsigned long otaUpdateTaskStackSize;  /* HiLink OTA升级任务栈大小，开发者根据具体情况调整 */
    int (*rebootSoftware)(void);           /* 异常场景软重启接口，不影响硬件状态，如果用户注册，首先使用此接口 */
    int (*rebootHardware)(void);           /* 异常场景硬重启接口，影响硬件状态，如果用户没有注册软重启，使用此接口重启 */
} HILINK_SdkAttr;

/*
 * HiLink SDK入口函数
 * 返回0表示成功，返回-1表示失败
 */
int hilink_main(void);

#ifdef NETCFG
int hilink_stop(void);
#endif

/* 获取当前设备状态，如配网状态、连接云端、在线、离线等 */
int hilink_get_devstatus(void);

/*
 * 开发者直接调用该接口完成设备恢复出厂设置
 * 返回0表示恢复出厂成功,返回-1表示恢复出厂失败
 * 该接口会设置恢复出厂标志，设置成功后会清理掉ssid账号信息，并重启模组
 */
int hilink_restore_factory_settings(void);

/*
 * 获取存储在Flash的用户配置信息
 * 参数len表示用户需要获取的配置信息长度
 * 参数config表示用户获取的配置信息缓存
 * 返回0表示获取成功，返回-1表示获取失败
 * config的内存分配和初始化由调用者完成;用户可获取的配置信息最大长度为32字节
 * 注意:1、需要在hilink_main初始化之后使用
 *      2、如果用户存储的是字符串，请用户保证存储的字符串长度不超过31字节，保证预留字符串结束符
 */
int HilinkGetUserConfig(unsigned short len, char *config);

/*
 * 设置用户配置信息到flash
 * 参数config表示用户配置信息
 * 参数len表示用户配置信息长度
 * 返回0表示设置成功，返回-1表示设置失败
 * 信息写入为覆盖更新，每次写入前会清空旧的数据;用户可存储的配置信息最大长度为32字节
 * 注意:1、需要在hilink_main初始化之后使用
 *      2、如果用户存储的是字符串，请用户保证存储的字符串长度不超过31字节，保证预留字符串结束符
 */
int HilinkSetUserConfig(const char *config, unsigned short len);

typedef struct {
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned short ms;
    /* bit0置位表示星期一, 以此类推, 星期日为bit6置位, bit7无意义 */
    unsigned char w_day;
} stHILINK_TIME_INFO, *pstHILINK_TIME_INFO;

/* 时区字符串最大长度 */
#define TIME_ZONE_STR_MAX_LEN 64

/*
 * 时间同步模块获取本地日期信息
 * 返回0表示成功，返回非0表示失败
 */
int hilink_get_local_time_info(pstHILINK_TIME_INFO p_time_info);

/*
 * 时间同步模块获取当前UTC时间和时区信息
 * 返回0表示成功，返回非0表示失败
 */
int hilink_get_utc_time_ms(unsigned long long *p_time_ms, char *time_zone, unsigned int len);

/*
 * 时间同步模块获取当前UTC日期信息
 * 返回0表示成功，返回非0表示失败
 */
int hilink_get_utc_time_info(pstHILINK_TIME_INFO p_time_info);

/*
 * 时间同步模块转换ms为日期
 * 返回0表示成功，返回非0表示失败
 */
int hilink_convert_time(unsigned long long time_ms, pstHILINK_TIME_INFO p_time_info);

/*
 * 设备离线时，如果在App上删除了设备，设备再次上线时云端会给设备下发Errcode=5或Errcode=6错误码。
 * 该接口用于使能SDK处理云端下发的Errcode=5或Errcode=6错误码。
 * enable为0表示SDK不处理云端下发的Errcode=5或Errcode=6错误码，此时SDK不会清除设备端注册信息，
 * 需要用户手动硬件恢复出厂设置，设备才能重新进行配网状态。
 * enable为非0表示SDK处理云端下发的Errcode=5或Errcode=6错误码，此时SDK会清除设备端注册信息，重新进行配网状态
 * 默认enable为1
 */
void HILINK_EnableProcessDelErrCode(int enable);

/*
 * 获取SoftAp配网PIN码
 * 返回值为8位数字PIN码,返回-1表示使用HiLink SDK的默认PIN码
 * 该接口需设备开发者实现
 */
int HiLinkGetPinCode(void);

/*
 * 设置配网信息
 * 入参数据为json格式字符串，具体内容字段如下：
 * {
 *   "ssid": "HUAWEI-Router", //路由器SSID字段,必选
 *   "password": "m12345678", //路由器密码字段,必选
 *   "devId": "f0356deb-4f18-42a3-b205-d2878b4adeae", //注册信息-设备ID字段,必选
 *   "psk": "69a4d3d1ac0022775dd5df0a5f7329cf", //注册信息-预制秘钥字段(转换十六进制字符串),必选
 *   "code": "cD9EZsJn", //注册信息-激活码字段,必选
 *   "cloudPrimaryUrl": "whiomplatform.hwcloudtest.cn", //主域名字段(主备域名),必选
 *   "cloudStandbyUrl": "whiomplatform.hwcloudtest.cn", //备份域名字段(主备域名),必选
 *   "cloudUrl": "whiomplatform.hwcloudtest.cn", //域名字段(兼容方案),可选
 *   "WifiBgnSwitch": 1, //WiFi工作模式字段,可选
 *   "timeZoneDisplay": "GMT+08:00", //时区信息,可选
 *   "timeZoneId": "Asia/Shanghai" //时区ID,可选
 * }
 * 返回0表示设置成功，其他表示设置失败(-2表示HiLink未处于接收配网数据状态)
 */
int HILINK_SetNetConfigInfo(const char *info);

/*
 * 获取配网信息回调
 * 入参数据为json格式字符串，具体内容字段如下：
 * {
 *   "ssid": "HUAWEI-Router", //路由器SSID字段
 *   "password": "m12345678", //路由器密码字段
 * }
 * 该接口需设备开发者实现
 */
void HILINK_GetNetConfigInfo(const char *info);

/*
 * HiLink SDK外部诊断信息记录接口
 * 该接口已经对外提供给第三方厂商使用，为了前向兼容，暂不按最新编码规范整改
 */
void hilink_diagnosis_record_ex(int errCode);

/*
 * 上报report能力属性状态，该接口为同步接口
 * 服务字段状态发生改变主动上报到云平台(连接云平台时)或者HiLink网关(连接HiLink网关时)
 * svcId表示服务ID
 * payload表示json格式数据
 * len表示payload长度
 * taskId表示调用该接口的线程的id
 * 返回0表示服务状态上报成功，返回-1表示服务状态上报失败
 * 该接口已经对外提供给第三方厂商使用，为了前向兼容，暂不按最新编码规范整改
 */
int hilink_report_char_state(const char *svcId, const char *payload, unsigned int len, int taskId);

/*
 * 上报服务状态，该接口为异步接口
 * 服务字段状态发生改变主动上报到云平台(连接云平台时)或者HiLink网关(连接HiLink网关时)
 * svcId表示服务ID
 * payload为json格式数据
 * len表示payload长度
 * 返回0表示服务状态上报成功，返回-1表示服务状态上报失败
 * 该接口已经对外提供给第三方厂商使用，为了前向兼容，暂不按最新编码规范整改
 */
int hilink_upload_char_state(const char *svcId, const char *payload, unsigned int len);

/* 设置HiLink SDK属性，返回0表示设置成功，否则，设置失败 */
int HILINK_SetSdkAttr(HILINK_SdkAttr sdkAttr);

/* 查询HiLink SDK属性 */
HILINK_SdkAttr *HILINK_GetSdkAttr(void);

/*
 * 厂家需要实现此接口实现license的写入，写入flash位置或者写入文件由厂家决定。
 * 厂家需要保证备份机制，防止突然断电导致license信息丢失，如果license信息丢失，将
 * 无法继续绑定设备，设备将不能再使用。
 * 执行成功返回0，执行失败返回-1
 */
int HILINK_WriteLicense(const unsigned char *license, unsigned int len);

/*
 * 厂家需要实现此接口实现license读取，读取flash位置或者写入文件由厂家决定。
 * 执行成功返回0，执行失败返回-1
 */
int HILINK_ReadLicense(unsigned char *license, unsigned int len);

/*
 * 查询设备是否已被注册
 * 返回非0，已注册；返回0，未注册；
 */
int HILINK_IsRegister(void);

/*
 * 设置HiLink SDK配置信息保存路径，仅linux和android版本适用
 * path为路径信息，绝对路径长度不超过127；
 * 返回0，设置成功；返回非0，设置失败。
 * 注意: 非linux和android系统无此接口实现
 */
int HILINK_SetConfigInfoPath(const char *path);

/*
 * 查询HiLink SDK配置信息保存路径，仅linux和android版本适用
 * path为保存路径的缓冲区；len为缓冲区长度；
 * 返回0，获取成功；返回非0，获取失败。
 * 注意: 非linux和android系统无此接口实现
 */
int HILINK_GetConfigInfoPath(char *path, unsigned int len);

/*
 * 查询当前设备敏感性标识。
 * 返回0为非敏感设备，返回1为敏感设备
 */
int HILINK_IsSensitiveDevice(void);

/* 厂商调用该接口获取匿名结构体 */
const void *HILINK_GetVoiceContext(void);

/*
 * 设备处于待用户识别状态时通知用户: 表现为持续蜂鸣或闪灯2s.
 * 参数enable表示识别状态,1为开始蜂鸣或闪灯,0为结束蜂鸣或闪灯.
 * 注意: 函数由设备开发者或厂商实现,仅在hi3861模组使用.
 */
void HILINK_SetNanIdentifyStatus(int enable);

/*
 * 设置WIFI安全距离的功率
 * 参数power:表示安全距离对应的发射通道功率,该值不能低于-70db;
 * 需要保证空口功率小于等于-65dBm,根据真实设备来调整.
 * 返回值:0表示设置成功,-1表示设置失败.
 * 注意: 函数由设备开发者或厂商调用，仅在hi3861模组使用.
 */
int HILINK_SetSafeDistancePower(char power);

/*
 * 设置是否使能PKI特性
 * 参数enable:表示使能还是不使能,传1表示使能,传0表示不使能
 * 注意: (1)函数由设备开发者或厂商调用,仅在支持PKI特性的模组使用
 *       (2)仅限新品类产品使用,已经商用的产品的新版本不能开启
 *       (3)开启该特性时,务必确保产线有相应的测试流程
 */
void HILINK_EnablePkiVerify(int enable);

/*
 * 产测模式下使能预置PKI证书模式, 使能后才能通过AT命令写入证书
 * 注意: 函数由设备开发者或厂商调用,仅在hi3861模组使用.
 */
int HILINK_EnableFactoryPkiMode(void);

/*
 * 获取设备的子型号，长度固定两个字节
 * subProdId为保存子型号的缓冲区，len为缓冲区的长度
 * 如果产品定义有子型号，则填入两字节子型号，并以'\0'结束, 返回0
 * 没有定义子型号，则返回-1
 * 该接口需设备开发者实现
 */
int HILINK_GetSubProdId(char *subProdId, int len);

/*
 * 获取设备表面的最强点信号发射功率强度，最强点位置的确定以及功率测试方
 * 法，参照hilink认证wifi靠近发现功率设置及测试方法指导文档，power为出参
 * ，单位dbm，返回设备表面的最强信号强度值，如果厂商不想使用wifi靠近发现功
 * 能，接口直接返-1，sdk就不做wifi靠近发现的初始化，如果需要使用wifi靠近
 * 发现，则接口返回0，power返回对应的功率值，power的有效值必须<=20dbm，如
 * 果接口返回0，但power大于20，则也不做wifi靠近发现的初始化，功能不可用
 */
int HILINK_GetDevSurfacePower(char *power);

/*
 * 如果使用双备份功能，则分别设置云端注册信息备份存储地址、时间信息备份存储地址、wifi账号密码备份存储地址
 * 返回0表示设置成功，-1表示设置失败
 * 注意：1) 三种信息地址可分别存储，传入参数必须大于等于0，如果参数传入0，表示双备份中该参数对应的信息不进行备份保存；
 *          每个参数存储地址按照4k对齐(hi3861除外)，不能与现有数据区存在重叠。
 *       2) hi3861模组注册信息至少分配2k存储空间，时间管理信息至少分配3k存储空间；wifi信息至少分配1k存储空间；
 *          其余模组按照4k大小来分配。
 *       3) ESP32、MTK7682模组wifi账号信息由系统保存，不进行备份，wifiInfoAddr必须直接传入0
 *       4) ESP8266模组wifi账号信息由系统保存，注册信息采用系统自带的双备份功能，因此这两个参数必须直接传入0
 *       5) LPB130模组wifi账号信息由系统保存，该参数必须直接传入0,在该函数中传入的各个参数地址为相对于0x000D4000的偏移地址。
 *       6) RTL8711AM为桥设备，不具有时间管理功能，timeInfoAddr必须直接传入0。
 */
int HILINK_SetBakAddr(unsigned long regInfoAddr, unsigned long timeInfoAddr, unsigned long wifiInfoAddr);

/*
 * 设置HiLink SDK主任务调度时间间隔，默认50ms
 * 如果设置失败返回-1，成功返回0
 * 该函数由设备开发者或厂商调用
 * interval: HiLink SDK主任务调度时间间隔，单位为毫秒，取值范围[5,100]
 * 注意：Hi3681L模组(支持低功耗)interval无取值范围限制
 */
int HILINK_SetScheduleInterval(unsigned long interval);

/*
 * 设置HiLink SDK守护任务调度时间间隔，默认1000ms
 * 如果设置失败返回-1，成功返回0
 * 该函数由设备开发者或厂商调用
 * interval: HiLink SDK守护任务调度时间间隔，单位为毫秒
 */
int HILINK_SetMonitorScheduleInterval(unsigned long interval);

/*
 * 设置HiLink SDK配网超时时间，单位为秒
 * 该函数由设备开发者或厂商调用
 * 注意：户外设备默认超时时间为2分钟，其余设备默认10分钟
 */
void HILINK_SetNetConfigTimeout(unsigned long netConfigTimeout);

/*
 * 设置HiLink SDK 复位
 * 该函数由设备开发者或厂商调用
 */
void HILINK_Reset(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _HILINK_H_ */
