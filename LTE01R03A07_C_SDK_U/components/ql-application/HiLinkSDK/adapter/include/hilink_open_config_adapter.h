/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 网络适配层接口 (需设备厂商实现)
 */
#ifndef HILINK_OPEN_CONFIG_ADAPTER_H
#define HILINK_OPEN_CONFIG_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* HiLink配置数据类型 */
enum HILINK_ConfigType {
    HILINK_RUNNING_CONFIG = 0,  /* HiLink运行配置数据 */
    HILINK_TIMER_CONFIG = 1,    /* HiLink定时器配置数据 */
    HILINK_BRIDGE_CONFIG = 2,   /* 桥设备配置参数 */
    HILINK_HKS_CONFIG = 3,      /* 端到端安全hichain的数据文件 */
    HILINK_URL_CONFIG = 4,      /* Url配置数据 */
    HILINK_URL_BAK_CONFIG = 5,  /* Url配置备份数据 */
    HILINK_BRG_SUBDEV_NODE = 6, /* 桥下子设备节点数据 */
    HILINK_BRG_SUBDEV_MAP = 7,  /* 桥下子设备map数据 */
};

/*
 * 初始化配置保存分区
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_InitConfig(void);

/*
 * 保存HiLink配置信息
 * configType表示HiLink配置类型
 *     HILINK_RUNNING_CONFIG HiLink运行配置数据，必须适配
 *     HILINK_TIMER_CONFIG   HiLink定时任务配置数据，需要定时功能时适配
 *     HILINK_BRIDGE_CONFIG  桥设备配置参数，当前为桥设备时需要适配
 *     HILINK_HKS_CONFIG     端到端安全hichain组件需要存储数据，需要端到端功能时需要适配
 * buf表示HiLink配置信息buf地址
 * len表示HiLink配置信息长度
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_WriteConfig(enum HILINK_ConfigType configType, const unsigned char *buf, unsigned int len);

/*
 * 读取HiLink配置信息
 * configType表示HiLink配置类型
 *     HILINK_RUNNING_CONFIG HiLink运行配置数据
 *     HILINK_TIMER_CONFIG   HiLink定时任务配置数据，需要定时功能时适配
 *     HILINK_BRIDGE_CONFIG  桥设备配置参数，当前为桥设备时需要适配
 *     HILINK_HKS_CONFIG     端到端安全hichain组件需要存储数据，需要端到端功能时需要适配
 * buf表示HiLink配置信息buf地址
 * len表示HiLink配置信息长度
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_ReadConfig(enum HILINK_ConfigType configType, unsigned char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif
#endif