/**
 * ant_system.h
 *
 * 系统相关接口
 */
#ifndef __ANT_SYSTEM_H__
#define __ANT_SYSTEM_H__
#include <stdarg.h>
#include "ant_typedef.h"
#include "ant_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ANT_POWER_ON_KEYPAD = 0,
    ANT_POWER_ON_SOFT_RESTART,
    ANT_POWER_ON_RESET,
    ANT_POWER_ON_CHARGER_IN,
    ANT_POWER_ON_EXCEPTION,
} ant_poweron_mode_e;


typedef enum {
    ANT_DEVICE_SN = 0,  		//设备SN，若为opencpu模式，则设备SN与模块SN相同
    ANT_DEVICE_MODULE_SN = 1,		//设备的模块SN
    ANT_DEVICE_IMEI = 2,			//设备的IMEI
    ANT_DEVICE_NAME = 3,			//设备名称
    ANT_DEVICE_MODEL = 4,			//设备的型号，指的是类别
    ANT_DEVICE_VENDOR = 5,			//设备的厂商名
    ANT_DEVICE_MAC = 6,				//设备的MAC地址，主要是WIFI MAC
    ANT_DEVICE_BT_MAC = 7,			//设备的蓝牙MAC
    ANT_DEVICE_CAMERA_ID = 8,		//设备的摄像头ID
    ANT_DEVICE_ICCID = 9,			//设备通讯卡的ICCID
    ANT_DEVICE_SSID = 10,			//设备所连WIFI的SSID
    ANT_DEVICE_IP = 11,				//设备当前的IP
    ANT_DEVICE_NETMASK = 12,			//设备网络掩码
    ANT_DEVICE_GATEWAY = 13,			//设备网关
    ANT_DEVICE_OS_NAME = 14,			//设备上的OS 操作系统名称
    ANT_DEVICE_OS_VERSION = 15,		//设备上的OS 操作系统版本号
    ANT_DEVICE_CHIP_ARCH = 16,		//设备芯片架构，如"arm"、"mips"等
    ANT_DEVICE_CPU_TYPE = 17,		//设备CPU类型，如
    ANT_DEVICE_MAX
} ant_device_info_e;

/**
 * 获取系统编译日期时间
 *
 * @return 返回系统编译日期时间，格式：“yyyy/MM/dd MM:hh:ss"
 */
char * ant_build_date_time(void);

ant_s32 ant_system_init(void);

/**
 * 获取设备的序列号
 *
 * @param sn 存储序列号的缓存
 * @param size 存储序列号缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_system_get_sn(char *sn, ant_u32 size);

/**
 * 获取设备的IMEI号
 *
 * @param imei 存储IMEI号的缓存
 * @param size 存储IMEI号缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_system_get_imei(ant_char *imei, ant_u32 size);

/**
 * 获取设备当前使用的流量卡的ICCID号
 *
 * @param imei 存储ICCID号的缓存
 * @param size 存储ICCID号缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_system_get_iccid(char *iccid, ant_u32 size);

/**
 * 获取设备的MAC地址
 *
 * @param mac 存储MAC地址的缓存
 * @param size 存储MAC地址缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_system_get_mac(char *mac, ant_u32 size);

/**
 * 系统重启
 *
 * @return  0： 系统重启成功
 *         -1： 系统重启失败
 */
ant_s32 ant_system_restart(void);

/**
 * 系统关机
 */
void    ant_system_shutdown(void);

/**
 * 获取系统启动模式
 *
 * @return 返回系统启动模式，取值参见 ant_poweron_mode_e
 */
ant_s32 ant_system_get_poweron_mode(void);

/**
 * 获取芯片cpuid
 *
 * @param cpuid  存储cpuid的缓存指针
 * @param size   存储cpuid缓存长度, 不小于16
 *
 * @return  0 获取成功
 *         -1 获取失败
 */
ant_s32 ant_system_get_cpuid(ant_char *cpuid, ant_u32 size);

/**
 * 获取主板的flashid
 *
 * @param flashid  存储flashid的缓存指针
 * @param size     存储flashid缓存长度, 不小于16
 *
 * @return  0 获取成功
 *         -1 获取失败
 */
ant_s32 ant_system_get_flashid(ant_char *flashid, ant_u32 size);

/**
 * 获取固件升级包的路径
 *
 * @return  非空: 获取成功, 固件升级包的文件系统绝对路径
 *          NULL: 获取失败
 */
char *ant_system_get_upgrade_path(void);

/**
 * 启动系统升级
 *
 * @param upgrade_img_path 固件升级包的文件系统绝对路径
 *
 * @return  0 升级启动成功
 *         -1 升级启动失败
 */
ant_s32 ant_system_upgrade(char *upgrade_img_path);


/**
 * 获取系统crash信息
 *
 * @param count 返回的crash次数
 * @param buffer 返回的dump信息
 *
 * @return  0 获取成功
 *         -1 获取失败
 */
ant_s32 ant_system_get_crash_info(ant_s32 * count,ant_char * buffer);


/**
 * 清除系统crash信息
 *
 * @return  0  清除成功
 *         -1  清除失败
 */

ant_s32 ant_system_reset_crash_info();

/**
 * 获取系统版本号
 *
 * @return  0  获取成功
 *         -1  获取失败
 */
char * ant_system_get_version(void);

/**
 * 获取系统安全标志(secureboot是否打开)
 *
 * @return  ANT_TRUE  secureboot已经打开
 *          ANT_FALSE secureboot未经打开
 */
ant_bool ant_system_get_security(void);

/**
 * 获取系统硬件信息
 *
 * @return  0  获取成功
 *         -1  获取失败
 */
char * ant_system_get_hardware_info(void);


/**
 * 获取系统正在使用的卡iccid
 *
 * @param iccid  用于存储返回的iccid
 * @param size   用于存储iccid的最大存储大小
 *
 * @return  0  获取成功
 *         -1  获取失败
 */
ant_s32 ant_system_get_iccid(char *iccid, ant_u32 size);

/**
 * 获取设备的信息
 *  专门用于iotsdk kal适配
 * @param out 存储获取到的信息的缓存
 * @param size 存储信息缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_system_get_device_info(ant_device_info_e type, char *out, size_t size);


char * ant_system_version_get(void);



    


#ifdef __cplusplus
} // extern "C"
#endif

#endif

