/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 模组系统适配层内存接口函数(需设备厂商实现)
 */
#include "hilink_open_mem_adapter.h"
#include <stdlib.h>

/*
 * 申请内存空间
 * 参数size表示申请内存空间大小
 * 返回申请内存空间指针
 */
void *HILINK_Malloc(unsigned int size)
{
    return NULL;
}

/*
 * 释放内存空间
 * 输入参数pt表示释放内存空间指针
 */
void HILINK_Free(void *pt)
{
    return;
}

/*
 * 内存比较
 * 返回0表示buf1和buf2指向的内存中内容相同, 大于0表示buf1中的内容大于buf2中的内容
 */
int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len)
{
    return 0;
}