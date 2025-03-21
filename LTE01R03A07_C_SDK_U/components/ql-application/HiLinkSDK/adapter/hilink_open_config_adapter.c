/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 配置信息读写适配层接口 (需设备厂商实现)
 */
#include "hilink_open_config_adapter.h"
#include "hilink_typedef.h"

/*
 * 初始化配置保存分区
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_InitConfig(void)
{
    return 0;
}

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
int HILINK_WriteConfig(enum HILINK_ConfigType configType, const unsigned char *buf, unsigned int len)
{
    //int ret;  //Quectel larson.li 函数定义未使用临时注释掉

    if (buf == NULL) {
        return -1;
    }

    switch (configType) {
        case HILINK_RUNNING_CONFIG:
            /* HiLink运行配置数据 */
            break;
        case HILINK_TIMER_CONFIG:
            /* HiLink定时任务配置数据，需要定时功能需要适配 */
            break;
        case HILINK_BRIDGE_CONFIG:
            /* HiLink桥设备配置数据，当前为桥设备时需要适配 */
            break;
        case HILINK_HKS_CONFIG:
            /* 端到端安全hichain组件需要存储数据，需要端到端功能时需要适配 */
            break;
        case HILINK_URL_CONFIG:
            /* HiLink域名配置数据，需要域名功能需要适配 */
            break;
        case HILINK_URL_BAK_CONFIG:
            /* HiLink备份域名配置数据，需要备份域名功能需要适配 */
            break;
        case HILINK_BRG_SUBDEV_NODE:
            /* HiLink桥下子设备节点配置数据，使用mesh通用网关功能的需要适配 */
            break;
        case HILINK_BRG_SUBDEV_MAP:
            /* HiLink桥下子设备map配置数据，使用mesh通用网关功能的需要适配 */
            break;
        default:
            return -1;
    }

    return 0;
}

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
int HILINK_ReadConfig(enum HILINK_ConfigType configType, unsigned char *buf, unsigned int len)
{
    if (buf == NULL) {
        return -1;
    }

    switch (configType) {
        case HILINK_RUNNING_CONFIG:
            /* HiLink运行配置数据 */
            break;
        case HILINK_TIMER_CONFIG:
            /* HiLink定时任务配置数据，需要定时功能需要适配 */
            break;
        case HILINK_BRIDGE_CONFIG:
            /* HiLink桥设备配置数据，当前为桥设备时需要适配 */
            break;
        case HILINK_HKS_CONFIG:
            /* 端到端安全hichain组件需要存储数据，需要端到端功能时需要适配 */
            break;
        case HILINK_URL_CONFIG:
            /* HiLink域名配置数据，需要域名功能需要适配 */
            break;
        case HILINK_URL_BAK_CONFIG:
            /* HiLink备份域名配置数据，需要备份域名功能需要适配 */
            break;
        case HILINK_BRG_SUBDEV_NODE:
            /* HiLink桥下子设备节点配置数据，使用mesh通用网关功能的需要适配 */
            break;
        case HILINK_BRG_SUBDEV_MAP:
            /* HiLink桥下子设备map配置数据，使用mesh通用网关功能的需要适配 */
            break;
        default:
            return -1;
    }

    return 0;
}