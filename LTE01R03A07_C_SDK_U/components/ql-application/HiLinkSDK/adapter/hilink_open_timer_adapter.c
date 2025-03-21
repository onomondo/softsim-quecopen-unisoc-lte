/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层定时器接口函数(需设备厂商实现)
 */
#include "hilink_open_timer_adapter.h"

/*
 * 将进程休眠milliSecond指定的时间
 * milliSecond为休眠的毫秒数
 * 返回 0表示成功，其他表示返回失败
 */
int HILINK_MilliSleep(unsigned long milliSecond)
{
    return 0;
}