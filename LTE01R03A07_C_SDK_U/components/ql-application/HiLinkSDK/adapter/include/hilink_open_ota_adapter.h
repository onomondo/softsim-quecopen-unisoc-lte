/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: OTA适配层接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_OTA_ADAPTER_H
#define HILINK_OPEN_OTA_ADAPTER_H

#include "hilink_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OTA升级分区1 */
#ifndef UPGRADE_FW_BIN1
#define UPGRADE_FW_BIN1 0x00
#endif

/* OTA升级分区2 */
#ifndef UPGRADE_FW_BIN2
#define UPGRADE_FW_BIN2 0x01
#endif

/* 只有MCU升级时立即重启 */
#ifndef RESTART_FLAG_NOW
#define RESTART_FLAG_NOW 0x01
#endif

/* 有模组时切换分区后再重启 */
#ifndef RESTART_FLAG_LATER
#define RESTART_FLAG_LATER 0x02
#endif

/*
 * Flash初始化
 * 返回值是true时，表示初始化正常
 * 返回值是false时，表示初始化异常
 */
bool HILINK_OtaAdapterFlashInit(void);

/*
 * 判断需要升级的分区
 * 返回值是UPGRADE_FW_BIN1时，表示升级固件到分区1
 * 返回值是UPGRADE_FW_BIN2时，表示升级固件到分区2
 */
unsigned int HILINK_OtaAdapterGetUpdateIndex(void);

/*
 * 擦除需要升级的分区
 * size表示需要擦除的分区大小
 * 返回值是0时，表示擦除成功
 * 返回值是-1时，表示擦除失败
 */
int HILINK_OtaAdapterFlashErase(unsigned int size);

/*
 * 升级数据写入升级的分区
 * buf表示待写入数据
 * bufLen表示待写入数据的长度
 * 返回值是0时，表示写入成功
 * 返回值是-1时，表示写入失败
 */
int HILINK_OtaAdapterFlashWrite(const unsigned char *buf, unsigned int bufLen);

/*
 * 读取升级分区数据
 * offset表示读写偏移
 * buf表示输出数据的内存地址
 * bufLen表示输出数据的内存长度
 * 返回值是0时，表示读取成功
 * 返回值是-1时，表示读取失败
 */
int HILINK_OtaAdapterFlashRead(unsigned int offset, unsigned char *buf, unsigned int bufLen);

/*
 * 分区升级结束
 * 返回值是true时，表示结束正常
 * 返回值是false时，表示结束异常
 */
bool HILINK_OtaAdapterFlashFinish(void);

/* 获取升级区间最大长度 */
unsigned int HILINK_OtaAdapterFlashMaxSize(void);

/*
 * 根据标志重启模组
 * flag表示重启标志
 * 当flag是RESTART_FLAG_NOW时，表示只有MCU升级时立即重启
 * 当flag是RESTART_FLAG_LATER时，表示有模组时切换分区后再重启
 */
void HILINK_OtaAdapterRestart(int flag);

#ifdef __cplusplus
}
#endif

#endif