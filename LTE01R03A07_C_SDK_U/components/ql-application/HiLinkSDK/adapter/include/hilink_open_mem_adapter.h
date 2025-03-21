/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层内存接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_MEM_ADAPTER_H
#define HILINK_OPEN_MEM_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 申请内存空间
 * 参数size表示申请内存空间大小
 * 返回申请内存空间指针
 */
void *HILINK_Malloc(unsigned int size);

/*
 * 释放内存空间
 * 输入参数pt表示释放内存空间指针
 */
void HILINK_Free(void *pt);

/*
 * 内存比较
 * 返回0表示buf1和buf2指向的内存中内容相同, 大于0表示buf1中的内容大于buf2中的内容
 */
int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
