#ifndef __ANT_TASK_H__
#define __ANT_TASK_H__
#include "ant_os.h"
#ifdef __cpluslus
extern "C" {
#endif

typedef void * ant_task;

/**
 * 创建任务(线程)
 *
 * @param task_name 任务(线程)名称
 * @param stack_size 任务(线程)栈大小
 * @param priority 任务(线程)优先级
 * @param entry_function 任务(线程)执行函数入口 
 * @param param 任务(线程)执行函数的参数
 *
 * 
 * @return 非空：成功，返回本地任务(线程)对象, 任务(线程)创建成功并立即执行
 *         NULL: 失败
 */
ant_task ant_task_new(const char * const task_name,
                      ant_u32 stack_size,
                      int priority,
                      ant_task_function_t entry_function,
                      void *param);

/**
 * 释放任务(线程)资源
 *
 * @param task 本地任务(线程)对象
 *
 */
void ant_task_free(ant_task task);

/**
 * 返回当前执行线程的本地任务(线程)对象
 *
 * @return  非空：成功，返回本地任务(线程)对象
 *          NULL: 失败
 */
ant_task ant_task_current(void);

/**
 * 挂起当前线程
 *
 * @return 空
 */

void ant_task_halt(void);

/**
 * 线程休眠
 *
 * @param ms 休眠时间，单位为毫秒
 */
void ant_task_msleep(ant_u32 ms);

#ifdef __cplusplus
}
#endif

#endif

