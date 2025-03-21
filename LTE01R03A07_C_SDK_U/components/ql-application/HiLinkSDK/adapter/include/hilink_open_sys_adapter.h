/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层基础能力抽象层接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_SYS_ADAPTER_H
#define HILINK_OPEN_SYS_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void *HiLinkMutex;

typedef void *HiLinkSemaphore;

typedef void *(*TaskEntryFunc)(unsigned int arg);

typedef struct {
    TaskEntryFunc funTaskEntry;
    unsigned short taskPrio;
    void *arg;
    unsigned int stackSize;
    const char *name;
    unsigned short resved;
} TaskInitParam;

/*
 * IoT OS 创建线程接口
 * 返回值: 返回0 成功, 其他 失败
 */
int HILINK_CreateTask(const void *handle, TaskInitParam *initParam);

/*
 * IoT OS 删除线程接口
 * 输入参数handle 删除线程句柄
 * 返回0 成功, 其他 失败
 */
int HILINK_DeleteTask(const void *handle);

/*
 * IoT OS 创建锁
 * 输入参数 HiLinkMutex *mutex  返回创建的锁
 * 返回0 成功, 其他 失败
 */
int HILINK_MutexCreate(HiLinkMutex *mutex);

/*
 * IoT OS 锁定
 * 输入参数HiLinkMutex *mutex 锁定的资源
 * 返回0 成功, 其他 失败
 */
int HILINK_MutexLock(HiLinkMutex *mutex);

/*
 * IoT OS 解锁接口函数
 * 输入参数HiLinkMutex *mutex
 * 返回0 成功, 其他 失败
 */
int HILINK_MutexUnlock(HiLinkMutex *mutex);

/*
 * IoT OS 删除锁
 * 输入参数HiLinkMutex *mexut  删除锁
 * 返回0 成功, 其他 失败
 */
int HILINK_MutexDestroy(HiLinkMutex *mutex);

/*
 * 创建信号量
 * handle是指向信号量的对象，count指定信号量值的大小
 * 返回0表示成功, 其他值表示失败
 */
int HILINK_SemaphoreCreate(HiLinkSemaphore *handle, int count);

/*
 * 给信号量的值减1
 * ms指定在信号量为0无法减1的情况下阻塞的时间限制
 * 返回0表示成功, 其他值表示失败
 */
int HILINK_SemaphoreTimedWait(HiLinkSemaphore handle, int ms);

/*
 * 给信号量的值加1
 * 返回0表示成功, 其他值表示失败
 */
int HILINK_SemaphorePost(HiLinkSemaphore handle);

/* 销毁信号量 */
void HILINK_SemaphoreDestroy(HiLinkSemaphore handle);

/*
 * IoT OS 获取当前时间
 * 输出参数表示当前设备启动后运行时间 单位毫秒
 * 返回0 成功，其他失败
 */
int HILINK_GetCurrTime(unsigned long *ms);

/*
 * 获取当前运行线程的线程号
 * 注意:freeRTOS和mico2.0返回的是当前线程的Handle，Contiki不支持该接口.
 */
unsigned long HILINK_GetCurrentTaskId(void);

/*
 * 获取随机数，厂家实现该接口
 * 如果模组有硬件熵源，需要调用HILINK_RegisterRandomEntropy注册硬件随机源，然后调用HILINK_SecRandom返回安全随机数
 * 如果模组没有硬件熵源，则厂家自行生成随机数，同时需要向华为报备
 */
int HILINK_Rand(void);

#ifdef __cplusplus
}
#endif

#endif
