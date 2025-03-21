/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层定时器接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_TIMER_ADAPTER_H
#define HILINK_OPEN_TIMER_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 将进程休眠milliSecond指定的时间
 * milliSecond为休眠的毫秒数
 * 返回 0表示成功，其他表示返回失败
 */
int HILINK_MilliSleep(unsigned long milliSecond);

#ifdef __cplusplus
}
#endif

#endif