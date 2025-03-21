/**
 * ant_os.h
 *
 * 操作系统相关接口
 */
#ifndef __ANT_OS_H__
#define __ANT_OS_H__

#include "ant_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

#define TASK_ID_INVALID         (-1)
#define TASK_APP_PRIO_BASE      0
#define TASK_PRIORITY_DEFAULT   TASK_APP_PRIO_BASE
#define TASK_QUEUE_SIZE_DEFAULT 50

typedef enum {
    ANT_NO_WAIT         =  0,   /* 获取不到消息队列或信号量时不阻塞，立即返回 */
    ANT_INFINITE_WAIT   = -1    /* 获取不到消息队列或信号量时永久阻塞 */
} ant_wait_mode_e;

typedef enum {
    ANT_QUE_SEM_TIMEOUT =  0,   /* 消息队列或信号量超时返回, 没有拿到消息或没有获得信号量 */
    ANT_QUE_SEM_SUCCESS =  1,   /* 消息队列或信号量正常返回, 拿到消息或获得信号量 */
    ANT_QUE_SEM_FAILURE = -1,   /* 消息队列或信号量错误返回, 没有拿到消息或没有获得信号量 */
} ant_que_sem_status_e;


typedef void*   ant_mutex;
typedef void*   ant_sem;
typedef void*   ant_cond;
typedef void*   ant_queue;
typedef ant_s32 ant_task_t;
typedef void (* ant_task_function_t)(void *);

/**
 * 线程休眠
 *
 * @param ms 休眠时间，单位为毫秒
 */
void ant_task_sleep_ms(ant_u32 ms);

/**
 * 创建互斥量
 *
 * @return  非空：创建成功，返回互斥量
 *         NULL: 创建失败
 */
ant_mutex ant_mutex_create(void);

/**
 * 互斥量加锁
 *
 * @param mutex 互斥量
 */
void ant_mutex_lock(ant_mutex mutex);

/**
 * 互斥量解锁
 *
 * @param mutex 互斥量
 */
void ant_mutex_unlock(ant_mutex mutex);

/**
 * 释放互斥量
 *
 * @param mutex
 */
void ant_mutex_free(ant_mutex mutex);

/**
 * 创建信号量
 *
 * @return  非空：创建成功，返回信号量
 *         NULL: 创建失败
 */
ant_sem ant_sem_create(ant_u32 initial_count);

/**
 * 等待信号量
 *
 * @param sem 信号量
 * @param time_out 超时时间，单位为毫秒
 * @return ANT_QUE_SEM_SUCCESS: 等待成功，获取到信号量
 *         ANT_QUE_SEM_TIMEOUT： 等待超时，没有获取到信号量
 *         ANT_QUE_SEM_FAILURE： 等待失败，没有获取到信号量，可能没有等到超时时间
 */
ant_s32 ant_sem_wait(ant_sem sem, ant_s32 time_out);

/**
 * 发送信号量
 *
 * @param sem 信号量
 */
void ant_sem_signal(ant_sem sem);

/**
 * 释放信号量对象。sem指向的信号量不可再用
 *
 * @param sem 要释放的信号量对象
 */
void ant_sem_free(ant_sem sem);

/**
 * 创建条件量
 *
 * @return  非空：创建成功，返回条件量
 *         NULL: 创建失败
 */
ant_cond ant_cond_create(void);

/**
 * 发送条件量
 *
 * @param cond 条件量，
 * @param events 条件发生标识，bit[31:0]每1位为1时表示一个条件发生
 */
void ant_cond_signal(ant_cond cond, ant_u32 events);

/**
 * 等待条件
 *
 * @param cond 条件量
 * @param events 要等待的条件标识，bit[31:0]位为1时表示需要等待的条件
 * @param events_got 等到的条件标识，bit[31:0]位为1时表示该位指示的条件发生
 * @param timeout 超时时间，单位为毫秒
 * @return ANT_QUE_SEM_SUCCESS: 等待成功，至少等到一个条件发生
 *         ANT_QUE_SEM_TIMEOUT： 等待超时，没有等到任何条件发生
 *         ANT_QUE_SEM_FAILURE： 等待失败，没有等到条件发生，可能没有等到超时时间
 */
ant_s32 ant_cond_wait(ant_cond cond, ant_u32 events, ant_u32 *events_got, ant_u32 timeout);

/**
 * 销毁条件量
 *
 *  @param cond 条件量
 */
void ant_cond_destroy(ant_cond cond);


/**
 * 创建任务(线程），同时创建任务关联的队列
 *
 * @param entry_function 任务执行函数入口
 * @param task_name 任务名称
 * @param stack_size 任务线程栈大小
 * @param queue_msg_size 任务消息队列消息长度
 * @param queue_length 任务消息队列大小
 * @param param 任务执行函数的参数
 * @param priority 任务优先级
 * @return 非空： 成功，任务创建成功并立即执行，并返回任务（线程）对象
 *         NULL: 失败
 */
ant_task_t ant_task_create(ant_task_function_t entry_function,
                           const char * const task_name,
                           ant_u32 stack_size,
                           ant_u32 queue_msg_size,
                           ant_u32 queue_length,
                           void *param,
                           int priority);

/**
 * 销毁任务。
 *
 * @param task 任务ID
 * @return  0： 成功
 *         -1： 失败
 */
ant_s32 ant_task_destroy(ant_task_t task);

/**
 * 返回当前线程的任务ID
 *
 * @return  >=0:  成功，任务ID
 *           -1:  失败
 */
ant_task_t ant_task_self(void);

/**
 * 返回当前线程的任务名称
 *
 * @return  任务名称
 */
const char *ant_task_self_name(void);

/**
 * 返回当前任务的本地线程对象
 *
 * @return  非空:  平台本地线程对象
 *          NULL:  失败
 */
void* ant_task_get_thread(ant_task_t task);

/**
 * 返回指定名称的任务ID
 *
 * @return  >=0:  成功，任务ID
 *           -1:  失败
 */
ant_task_t ant_task_by_name(const char *name);

/**
 * 获取任务名称
 *
 * @param task 任务对象，如果为空，返回当前任务名称；如果非空，返回task指向的任务名称。
 * @return
 */
const char * ant_task_name(ant_task_t task);

/**
 * 获取任务栈大小
 *
 * @param task 任务对象，如果为空，返回当前任务栈大小；如果非空，返回task指向的任务栈大小。
 * @return 任务栈大小
 */
ant_u32 ant_task_get_stack_size(ant_task_t task);

/**
 * 获取系统的最高优先级数值
 *
 * @return 最高优先级数值
 */
ant_s32 ant_task_get_max_priority(void);

/**
 * 向指定的任务发送消息
 *
 * @param task  任务ID
 * @param msg   消息指针, 消息指针缓存长度必须是创建任务时指定的queue_msg_size
 *
 * @return  0 发送成功
 *         -1 发送失败
 */
ant_s32 ant_task_send_event(ant_task_t task, void *msg);

/**
 * 当前任务接收消息
 *
 * @param msg      消息指针, 消息指针缓存长度必须是创建任务时指定的queue_msg_size
 * @param timeout  接收超时时间, 单位为毫秒, ANT_WAIT_FOREVER(-1)表示永久等待
 *
 * @return  0 接收消息超时
 *          1 接收消息成功
 *         -1 接收消息失败
 *         参考 ant_que_sem_status_e
 *
 */
ant_s32 ant_task_recv_event(void *msg, ant_s32 timeout);

/**
 * task任务优先级调整
 * 用于适配iotsdk等优先级根据不同系统来调整
 *
 * @param pri  原优先级
 *
 * @return 调整之后的优先级
 *
 */
ant_s32 ant_task_priority_adjust(ant_s32 pri);


#ifdef __cplusplus
}
#endif

#endif

