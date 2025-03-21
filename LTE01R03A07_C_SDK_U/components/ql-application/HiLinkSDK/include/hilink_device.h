/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink产品适配头文件
 * Notes: 该文件中的接口需要对外提供给第三方厂商使用，为了前向兼容，部分老接口暂不按最新编码规范整改.
 */
#ifndef HILINK_DEVICE_H
#define HILINK_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 设备版本信息 */
#define FIRMWARE_VER "1.0.0"
#define SOFTWARE_VER "1.0.0"
#define HARDWARE_VER "1.0.0"

/* 设备产品信息 */
#define PRODUCT_ID    "1011"
#define DEVICE_TYPE   "01D"
#define MANUAFACTURER "011"
#define DEVICE_MODEL  "SP mini3-HL"


/* 请确保设备类型英文名和厂商英文名长度之和不超过17字节 */
#define DEVICE_TYPE_NAME   "Switch"
#define MANUAFACTURER_NAME "Broadlink"

/* 通信协议类型: WiFi */
#define PROTOCOL_TYPE 1

/* HiLink SDK 通知厂商模组重启原因 */
#define HILINK_REBOOT_WATCHDOG  0
#define HILINK_REBOOT_DEVDELETE 1

/* 设备与云端连接断开(版本向前兼容) */
#define HILINK_M2M_CLOUD_OFFLINE       0
/* 设备连接云端成功，处于正常工作态(版本向前兼容) */
#define HILINK_M2M_CLOUD_ONLINE        1
/* 设备与云端连接长时间断开(版本向前兼容) */
#define HILINK_M2M_LONG_OFFLINE        2
/* 设备与云端连接长时间断开后进行重启(版本向前兼容) */
#define HILINK_M2M_LONG_OFFLINE_REBOOT 3
/* HiLink线程未启动 */
#define HILINK_UNINITIALIZED           4
/* 设备处于配网模式 */
#define HILINK_LINK_UNDER_AUTO_CONFIG  5
/* 设备处于10分钟超时状态 */
#define HILINK_LINK_CONFIG_TIMEOUT     6
/* 设备正在连接路由器 */
#define HILINK_LINK_CONNECTTING_WIFI   7
/* 设备已经连上路由器 */
#define HILINK_LINK_CONNECTED_WIFI     8
/* 设备正在连接云端 */
#define HILINK_M2M_CONNECTTING_CLOUD   9
/* 设备与路由器的连接断开 */
#define HILINK_M2M_CLOUD_DISCONNECT    10
/* 设备被注册 */
#define HILINK_DEVICE_REGISTERED       11
/* 设备被解绑 */
#define HILINK_DEVICE_UNREGISTER       12
/* 设备复位标记置位 */
#define HILINK_REVOKE_FLAG_SET         13
/* 设备协商注册信息失败 */
#define HILINK_NEGO_REG_INFO_FAIL      14

/* 设备序列号最大长度 */
#define MAX_SN_LENGTH 40

/* 获取BI 参数接口函数 */
char *hilink_get_auto_bi_rsa_cipher(void);

/* 获取AC 参数接口函数 */
unsigned char *hilink_get_auto_ac(void);

/*
 * 修改服务当前字段值
 * svcId为服务的ID，payload为接收到需要修改的Json格式的字段与其值，len为payload的长度
 * 返回0表示服务状态值修改成功，不需要底层设备主动上报，由HiLink SDK上报；
 * 返回-101表示获得报文不符合要求；
 * 返回-111表示服务状态值正在修改中，修改成功后底层设备必须主动上报；
 */
int hilink_put_char_state(const char *svcId, const char *payload, unsigned int len);

/*
 * 获取服务字段值
 * svcId表示服务ID。厂商实现该函数时，需要对svcId进行判断；
 * in表示接收到的Json格式的字段与其值；
 * inLen表示接收到的in的长度；
 * out表示保存服务字段值内容的指针,内存由厂商开辟，使用完成后，由Hilink Device SDK释放；
 * outLen表示读取到的payload的长度；
 * 返回0表示服务状态字段值获取成功，返回非0表示获取服务状态字段值不成功。
 */
int hilink_get_char_state(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen);

/*
 * 获取设备sn号
 * 参数len表示sn的最大长度,39字节
 * 参数sn表示设备sn
 * sn指向的字符串长度为0时将使用设备mac地址作为sn
 */
void HilinkGetDeviceSn(unsigned int len, char *sn);

/*
 * 获取设备相关版本号
 * 返回0表示版本号获取成功，返回其他表示版本号获取失败
 * 注意，此接口为HiLink内部调用函数
 */
int getDeviceVersion(char **firmwareVer, char **softwareVer, char **hardwareVer);

/*
 * 获取SoftAp配网PIN码
 * 返回值为8位数字PIN码, 返回-1表示使用HiLink SDK的默认PIN码
 * 该接口需设备开发者实现
 */
int HiLinkGetPinCode(void);

/*
 * 查询当前设备敏感性标识
 * 返回0为非敏感设备，返回1为敏感设备
 */
int HILINK_IsSensitiveDevice(void);

/*
 * 通知设备的状态
 * status表示设备当前的状态
 * 注意，此函数由设备厂商根据产品业务选择性实现
 */
void hilink_notify_devstatus(int status);

/*
 * 实现模组重启前的设备操作
 * flag为0表示HiLink SDK 线程看门狗触发模组重启; 为1表示APP删除设备触发模组重启
 * 返回0表示处理成功, 系统可以重启，使用硬重启; 返回1表示处理成功, 系统可以重启，使用软重启;
 * 返回负值表示处理失败, 系统不能重启
 * 注意，此函数由设备厂商实现；若APP删除设备触发模组重启时，设备操作完务必返回0，否则会导致删除设备异常
 */
int hilink_process_before_restart(int flag);

/*
 * 获取设备故障码，并通知APP
 * status表示是否发送故障，0表示不发送，1表示发送；code表示故障码
 * 返回0表示成功，返回非0失败
 */
int get_faultDetection_state(int *status, int *code);

#ifdef __cplusplus
}
#endif
#endif