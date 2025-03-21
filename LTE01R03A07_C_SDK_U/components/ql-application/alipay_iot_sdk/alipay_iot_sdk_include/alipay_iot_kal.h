#ifndef ALIPAY_IOT_KAL_H
#define ALIPAY_IOT_KAL_H

/*
* 此文件定义的所有函数，都需要接入SDK的厂商实现。
*/
#include "alipay_iot_type.h"

typedef struct {
  int32_t year;
  int32_t month;
  int32_t wday;
  int32_t day;
  int32_t hour;
  int32_t minute;
  int32_t second;
  int32_t isdst;
} alipay_iot_local_time;

typedef struct
{
    long tv_sec;//秒
    long tv_usec;//单位微妙
} alipay_iot_timeval;

/*
* @brief: 获取UTC时间
* note:  1.精确级别,微妙级别
*        2.受系统时间修改影响
*        3.返回的秒数是从1970年1月1日0时0分0秒开始
*/
extern void alipay_iot_gettimeofday(alipay_iot_timeval *now);

/*
* 获取系统当前时间
* month 为1表示1月，day 为1表示1号，year为2018表示2018年。
* hour 取值0-23.  min取值0-59. second取值0-59.
*/
extern void alipay_iot_get_local_time(alipay_iot_local_time *time);

/*
* 获取系统启动时间
* 开机后每次调用返回的时间固定系统启动时刻的UTC时间
*/
extern void alipay_iot_get_bootup_time(alipay_iot_timeval *time);
/*
* 设定系统的UTC时间
* UTC时间是从1970年01月01日 0:00:00起到现在的秒数
*/
extern void alipay_iot_set_utc_time(int64_t utc);

/*
 * @brief: 任务的创建函数
 * @input: task_entry_function  任务实现函数的指针
 * @input: task_name 任务名
 * @input: stack_size 任务栈大小
 * @input: queue_length 任务消息队列长度
 * @input: param 传给任务的参数
 * @input: priority 任务优先级，优先级越高，数字越小。
 * @return: >=0 创建成功,且返回值为任务的id
 *          -1 创建失败
 * @note: 调用此函数的任务的任务优先级需比该函数创建的任务优先级高，或者新任务内部执行适当等待。
 *         否则会出现此函数内部初始化还未完成， 就被新创建的任务抢占系统资源。导致程序执行异常。
 */
typedef void (*alipay_iot_task_function)( void * );
extern int alipay_iot_task_create(alipay_iot_task_function task_entry_function,
                    const char * const task_name,
                    unsigned int stack_size,
                    unsigned int queue_length,
                    void *param,
                    int priority);

/*
 * @brief: 任务的销毁函数
 * @input: task_id  任务的id
 * @return: 1 销毁成功
 *          -1 销毁失败
 * @note:  mtk平台的sdk中的任务不支持销毁
 */
extern int alipay_iot_task_delete(int task_id);

/*
 * @brief: 发送消息给其他任务
 * @input: target_id 目的任务的id
 * @input: src_id 源任务的id,如果传入-1，表示为当前任务
 * @input: msg_id 消息id
 * @input: data 消息体的指针
 * @return: 1 成功
 *          -1 失败
 * @note: 发送的只是消息体的首地址
 * @note: 消息的创建者，需要实现消息的创建函数和销毁函数。创建函数中对内存进行分配，销毁函数中对内存进行释放。
 *        消息的发送者，调用创建函数生成消息体，然后调用本函数将消息体指针发送出去。
 *        消息的接受者，消费消息后，调用销毁函数，释放消息占用的内存。
 */
extern int alipay_iot_task_send_message(int target_id, int src_id, int msg_id, void *data);

/*
 * @brief: 任务中读取其他任务发过来的消息，若无消息，则该函数阻塞
 * @output: src_id 源任务的id
 * @output: msg_id 消息id
 * @output: data 消息体的指针的指针
 * @return: 1 成功
 *          -1 失败
 * @note: 注意该函数是阻塞调用的
 * @note: 接收的只是消息体的指针的指针
 * @note: 消息的创建者，需要实现消息的创建函数和销毁函数。创建函数中对内存进行分配，销毁函数中对内存进行释放。
 *        消息的发送者，调用创建函数生成消息体，然后调用本函数将消息体指针发送出去。
 *        消息的接受者，消费消息后，调用销毁函数，释放消息占用的内存。
 */
extern int alipay_iot_task_recevie_message(int *src_id, int *msg_id, void **data);

/*
 * @brief: 申请一块连续的指定大小的内存块
 * @input: size 内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值不确定)
 *			NULL 分配失败
 */
extern void *alipay_iot_malloc(size_t size);

/*
 * @brief: 在内存的动态存储区中分配nmemb个长度为size的连续空间
 * @input: nmemb 内存块的个数
 * @input: size 每个内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值自动初始化为0)
 *			NULL 分配失败
 */
extern void *alipay_iot_calloc(size_t nmemb, size_t size);

/*
 * @brief: 释放动态分配的内存
 * @input: s 内存块的首地址
 */
extern void alipay_iot_std_free(void *ptr);

/*
 * @brief: 释放动态分配的内存，并将指针置NULL
 * @input: s 内存块的首地址
 */
#define alipay_iot_free(ptr) do{\
	if(ptr != NULL)\
	{\
		alipay_iot_std_free(ptr);\
		ptr = NULL;\
	}\
}while(0)

/*
 * @brief: 将s所指向的某一块内存中的前n个字节的内容全部设置为c
 * @input: s 内存块的首地址
 * @input: c 要设置的值
 * @input: n 要设置的字节个数
 * @return: 内存块的首地址
 */
extern void *alipay_iot_memset(void *s, int c, size_t n);

/*
 * @brief: 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
 * @input: dest 目标内存块的首地址
 * @input: src 源内存块的首地址
 * @input: n 要拷贝的字节个数
 * @return: 指向dest的指针
 */
extern void *alipay_iot_memcpy(void *dest, const void *src, size_t n);


/*
 * @brief: 存储区str1和str2的前n个字节进行比较
 * @input: str1 指向内存块地址
 * @input: str2 指向内存块地址
 * @input: n 需要比较字节个数
 * @return: 如果返回值 < 0，则表示 str1 小于 str2。
            如果返回值 > 0，则表示 str2 小于 str1。
            如果返回值 = 0，则表示 str1 等于 str2。
 */
extern int alipay_iot_memcmp(const void *str1, const void *str2, size_t n);


/*
 * @brief: 将参数src字符串拷贝至参数dest所指的地址
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  如果参数dest所指的内存空间不够大，可能会造成缓冲溢出的错误情况，
 *         在编写程序时需特别留意，或者用strncpy()来取代
 */
extern char *alipay_iot_strcpy(char *dest, const char *src);

/*
 * @brief: 将字符串src前n个字符拷贝到字符串dest
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  不像strcpy()，strncpy()不会向dest追加结束标记'\0'；
 *         src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置n个字符
 */
extern char *alipay_iot_strncpy(char *dest, const char *src, size_t n);

/*
 * @brief: 把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置src的字符串
 */
extern char *alipay_iot_strcat(char *dest, const char *src);

/*
 * @brief: 把src所指字符串的前n个字符添加到dest所指字符串的结尾处，并覆盖dest所指字符串结尾的'\0'
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置要拷贝的字符串
 */
extern char *alipay_iot_strncat(char *dest, const char *src, size_t n);

/*
 * @brief: 在参数s所指向的字符串中搜索第一次出现字符c的位置
 * @param: s 要被检索的字符串
 * @param: c 要被检索的字符
 * @return: c在str中第一次出现的位置，如果未找到，则返回NULL
 */
extern char *alipay_iot_strchr(const char *s, int c);
/*
 * @brief: 查找字符串str2在str1出现的位置
 * @param: str1 被查找目标
 * @param: str2 要查找对象
 * @return:若str2是str1的子串，则返回str2在str1的首次出现的地址；如果str2不是str1的子串，则返回NULL。
 */
extern char *alipay_iot_strstr(const char *str1, const char *str2);
/*
 * @brief: 把 s1 所指向的字符串和 s2 所指向的字符串进行比较
 * @input: s1 要进行比较的第一个字符
 * @input: s2 要进行比较的第二个字符
 * @return: 如果返回值 < 0，则表示 s1 < s2
 *          如果返回值 = 0，则表示 s1 = s2
 *          如果返回值 > 0，则表示 s1 > s2
 */
extern int alipay_iot_strcmp(const char *s1, const char *s2);


/*
 * @brief: 由src所指内存区域复制count个字节到dest所指内存区域。
 * @input: dest 目标地址
 * @input: src 源地址
 * @count: 拷贝长度
 * @return: 正确返回dest地址，错误返回NULL
 */
extern void *alipay_iot_memmove( void* dest, const void* src, size_t count );
/*
 * @brief: 计算字符串s的长度，直到空结束字符，但不包括空结束字符
 * @input: s 要计算长度的字符串
 * @return: 字符串的长度
 */
extern size_t alipay_iot_strlen(const char *s);

/*
 * @brief: 将字符串转换成浮点数
 * @input: nptr 要转换的字符串
 * @input: endptr 遇到不合条件而终止的nptr中的字符指针由endptr传回
 * @return: 转换后的浮点数
 */
extern double alipay_iot_strtod(const char *nptr,char **endptr);

/*
 * @brief: 分解字符串 str 为一组字符串，delim 为分隔符。
 * @input: str 要被分解成一组小字符串的字符串。
 * @input: delim 包含分隔符的 C 字符串。
 * @return: 返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
 */
extern char *alipay_iot_strtok(char *str, const char *delim);

/*
 * @brief: 将字符串转换成整数值
 * @input: nptr 要转换的字符串
 * @return: 转换后的整数值
 */
int64_t alipay_iot_atoll(const char *p);

/*
 * @brief: 字符串转换成整型数
 * @input: nptr 要转换的字符串
 * @return: 转换后的整型
 */
extern int alipay_iot_atoi(const char *nptr);
/*
 * @brief: 根据参数base来转换成无符号的长整型数
 * @input: nptr 要转换的字符串
 * @input: endptr 遇到不合条件而终止的nptr中的字符指针由endptr传回
 * @input: base 代表进制，base范围从2至36，或0
 * @return: 转换后的长整型
 */
extern unsigned long alipay_iot_strtoul(const char *nptr,char **endptr,int base);
/*
 * @brief: 字母字符转换成小写，非字母字符不做出处理
 * @input: c 要转换的字符
 * @return: 转换后的字符
 */
extern int alipay_iot_tolower(int c);
/*
 * @brief: 比较两个字符串
 * @input: str1 第一个需要比较的字符串
 * @input: str2 第二个需要比较的字符串
 * @input: n 需要比较的字符串长度
 * @return: 若str1与str2的前n个字符相同，则返回0；若s1大于s2，则返回大于0的值；若s1 小于s2，则返回小于0的值
 */
extern int alipay_iot_strncmp(const char *str1, const char *str2, size_t n);
/*
 * @brief: 读取格式化的字符串中的数据
 * @input: buffer 存储的数据
 * @input: format 窗体控件字符串
 * @input: ... 可选自变量
 * @return: 成功返回赋值的字段个数
 */
extern int alipay_iot_sscanf(const char *buffer, const char *format, ...);
/*
 * @brief: 将可变个参数(...)按照format格式化成字符串，然后将其复制到str中
 * @input: str 存储的数据
 * @input: size str数据空间长度
 * @input: format 窗体控件字符串
 * @return: 若成功则返回预写入的字符串长度，若出错则返回负值。
 */
extern int alipay_iot_snprintf(char *str, size_t size, const char *format, ...);

/*
 * @brief: 创建定时周期为timer_period的定时器，定时时间到，回调执行callback_function
 * @param  timer_name 定时器的名称
 * @param: timer_period 定时周期，单位ms
 * @param: auto_reload 1 溢出后自动重载 0 溢出后停止定时器
 * @param: callback_function 回调函数指针
 * @return: NULL 失败
 *            其他 新创建的定时器的指针
 */
typedef void * alipay_iot_sys_timer;
typedef void (*alipay_iot_timer_callback_function)(void* param);
extern alipay_iot_sys_timer alipay_iot_timer_create(const char *timer_name,
                                const int timer_period,
                                const bool auto_reload,
                                alipay_iot_timer_callback_function callback_function,
                                void* callback_param);
/*
* @brief: 启动定时器
* @input: timer 要启动的定时器
* @return: 1 启动成功
*          -1 启动失败
*/
extern int alipay_iot_timer_start(alipay_iot_sys_timer timer);

/*
* @brief: 停止定时器
* @input: timer 要停止的定时器
* @return: 1 停止成功
*          -1 停止失败
*/
extern int alipay_iot_timer_stop(alipay_iot_sys_timer timer);

/*
* @brief: 删除定时器
* @input: timer 要删除的定时器
* @return: 1 删除成功
*          -1 删除失败
*/
extern int alipay_iot_timer_delete(alipay_iot_sys_timer timer);

/*
* @brief: 使任务休眠milliseconds毫秒
* @input: milliseconds 休眠的时间，单位毫秒
*/
extern void alipay_iot_sleep(unsigned long milliseconds);

/*
* @brief: 断言，若exception为false, 则触发OS断言
*/
extern void alipay_iot_assert( bool expression );

typedef void * alipay_iot_mutex;
/*
* @brief: 创建互斥量
* @input: mutex_name 互斥量的名称
* @return: 其他 创建成功
*          NULL 创建失败
*/
extern alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name);

/*
* @brief: 对互斥量加锁
* @input: mutex 要加锁的互斥量
* @return: 0 加锁成功
*          -1 加锁失败
*/
extern int alipay_iot_mutex_lock(alipay_iot_mutex mutex);

/*
* @brief: 对互斥量解锁
* @input: mutex 要解锁的互斥量
* @return: 0 解锁成功
*          -1 解锁失败
*/
extern int alipay_iot_mutex_unlock(alipay_iot_mutex mutex);

/*
* @brief: 删除互斥量
* @input: mutex 要删除的互斥量
* @return: 0 删除成功
*          -1 删除失败
*/
extern int alipay_iot_mutex_delete(alipay_iot_mutex mutex);


typedef void * alipay_iot_semphore;
typedef enum {
    ALIPAY_IOT_NO_WAIT,       /* don't wait if the condition is not satisfied and return error code */
    ALIPAY_IOT_INFINITE_WAIT  /* wait until the condition is satisfied */
} ALIPAY_IOT_WAIT_MODE;

/**
 * FUNCTION
 *  alipay_iot_create_sem
 * DESCRIPTION
 *  create a couting semaphore.
 * PARAMETERS
 *  sem_name  : [IN] semaphore name. it's used for debug only, and only the first 8 characters retain.
 *  initial_count : [IN] semaphore initial count.
 * RETURNS
 *  Success:        semaphore pointer.
 *  Error:          system enter fatal error handling.
 ******************************************************************************/
alipay_iot_semphore alipay_iot_create_semphore(char *sem_name, uint32_t initial_count);
/**
 * FUNCTION
 *  alipay_iot_take_semphore
 * DESCRIPTION
 *  Obtains an instance of the specified semaphore.
 * PARAMETERS
 *  sem_ptr        : [IN] semaphore pointer.
 *  wait_mode      : [IN] wait mode, specify the behavior when the semaphore is
 *                   not ready immediately
 * RETURN VALUES
 *  0    : the operation is done successfully
 *  others : the semaphore is unavailable immediately
 * NOTE
 *  (1)Semaphore is a FIFO queue. If multiple tasks is waiting on a semaphore,
 *     the first waiting task is selected to be the next owner.
 ******************************************************************************/
int16_t alipay_iot_take_semphore(alipay_iot_semphore sem_ptr, ALIPAY_IOT_WAIT_MODE wait_mode);
/**
 * FUNCTION
 *  alipay_iot_give_sem
 * DESCRIPTION
 *  Give up 1 semaphore ownership. If any task is waiting on this semaphore, give the ownership to first waiting task.
 * PARAMETERS
 *  sem_ptr        : [IN] semaphore pointer.
 */
void alipay_iot_give_semphore(alipay_iot_semphore sem_ptr);
/**
 * FUNCTION
 *  aipay_iot_destory_sem
 * DESCRIPTION
 *  destory semphore. It will free resource owned by sem_ptr.
 * PARAMETERS
 *  sem_ptr        : [IN] semaphore pointer.
*/
void aipay_iot_destory_semphore(alipay_iot_semphore sem_ptr);

/*
 * @brief: 打开block
 * @input: block_name block名称
 * @return: NULL 打开失败
 *          非NULL  打开成功,返回指向block的句柄
 * @note:  保证线程安全
 */
extern void *alipay_iot_kv_open(const char *block_name);

/*
 * @brief: 获取某个block中指定key的value值
 * @input: h block句柄
 * @input: key key字符串
 * @output: value 值的地址
 * @output: len 返回的实际的value的长度
 * @return: <0 失败
 *           0 未找到key
 *          =1  成功
 * @note: 使用完毕后需要人为释放value指针，否则会内存泄漏
 * @note: 如果value是字符串，不能保证以'\0'结尾，除非调用alipay_iot_kv_set的时候，将'\0'存入了
 * @note: 保证线程安全
 */
extern int alipay_iot_kv_get(const void *h, const char *key, void **value, size_t *len);

/*
 * @brief: 设置某个block中指定key的值为value
 * @input: h block句柄
 * @input: key key字符串
 * @input: value 值的地址
 * @input: len 要设置的value的长度
 * @return: <0 失败
 *          0 没有足够的空间存储
 *          =1  成功
 * @note:  保证线程安全
 */
extern int alipay_iot_kv_set(const void *h, const char *key, const void *value, size_t len);

/*
 * @brief: 根据key删除某个block中指定的kv
 * @input: h block句柄
 * @input: key key字符串; 若key为NULL，则删除block对应的所有kv
 * @return: <0 失败
 *           0 未找到key
 *          =1  成功
 * @note:  保证线程安全
 */
extern int alipay_iot_kv_delete(const void *h, const char *key);

/*
 * @brief: 关闭block
 * @input: h block句柄
 * @note:  保证线程安全
 */
extern void alipay_iot_kv_close(const void *handle);

typedef enum alipay_iot_log_lvl {
    ALIPAY_IOT_LOG_LVL_VBS, /**verbose*/
    ALIPAY_IOT_LOG_LVL_DBG, /**debug*/
    ALIPAY_IOT_LOG_LVL_INF, /**inform*/
    ALIPAY_IOT_LOG_LVL_WRN, /**warn*/
    ALIPAY_IOT_LOG_LVL_ERR, /**error*/
    ALIPAY_IOT_LOG_LVL_FTL  /**fatal*/
} alipay_iot_log_lvl_t;

/*
* @input: levle 日志过滤等级
*/
extern void alipay_iot_log_level_set(alipay_iot_log_lvl_t level);

/*
 * @brief: 日志输出
 * @input: file_name 日志打印位置所在的文件名
 * @input: line      日志打印位置的行数
 * @input: level     日志等级
 * @input: level_name日志等级名称
 * @input: format    日志格式
 * @input: ...       和format对应的参数
 */
extern void alipay_iot_log(const char *file_name,
               uint32_t line,
               alipay_iot_log_lvl_t level,
               const char *tag,
               const char* format, ...);

#ifndef __FILE__
#define __FILE__    ""
#endif
#ifndef __LINE__
#define __LINE__    ""
#endif
 /*
 * @brief: 各种等级的日志输出
 * @input: format 日志格式
 * @input: ... 和format对应的参数
 * @note:  每条日志自动换行，建议的格式为: 时间 文件名 行数 日志等级 日志内容，如: 2018-08-18 18:06:09|main.c|69|INFO| hello world
 */
#ifdef __ALIPAY_IOT_DEBUG__
#define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#else
#define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#endif
#define ALIPAY_IOT_LOG_INFO(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_WARN(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_WRN, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_ERROR(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_ERR, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_FATAL(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_FTL, tag, format, ##__VA_ARGS__)


/**
 * SDK调用此函数获取设备MAC地址
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。格式通常为6个字节的二进制代码（以6组16进制数表示）如：“00-01-6C-06-A6-29”
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_mac(char *inOut, size_t size);


/**
 * 获取设备蓝牙MAC地址
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。格式通常为6个字节的二进制代码（以6组16进制数表示）如：“00-01-6C-06-A6-29”
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_bt_mac(char *inOut, size_t size);


/**
 * 获取设备ssid
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_ssid(char *inOut, size_t size);


/**
 * 获取设备IP地址
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_ip(char *inOut, size_t size);


/**
 * 获取设备NetMask
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_netmask(char *inOut, size_t size);


/**
 * 获取网关地址
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_gateway(char *inOut, size_t size);


/**
 * 获取操作系统版本
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_os_version(char *inOut, size_t size);

/**
 * 获取操作系统名称，比如"Nucleus"
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_os_name(char *inOut, size_t size);


/**
 * 获取处理器架构，比如"ARM920T"
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_chip_arch(char *inOut, size_t size);


/**
 * 获取内存总大小
 * 返回值：
 *  内存总大小，单位是字节
 */
extern long alipay_iot_get_total_memory(void);


/**
 * 获取可用内存大小
 * 返回值：
 *  可用内存大小，单位是字节
 */
extern long alipay_iot_get_available_memory(void);


/**
 * 获取sdk所在模组的的序列号
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_module_sn(char *inOut, size_t size);

/**
 * 获取产品终端编号，此值不能够填写IMEI
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_terminal_id(char *inOut, size_t size);

/**
 * 获取厂商名称
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_vendor_name(char *inOut, size_t size);

/**
 * 获取设备型号
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_device_model(char *inOut, size_t size);

/**
 * 获取设备名称
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_device_name(char *inOut, size_t size);

/**
 * 获取（移动网络接入）设备IMEI，此接口可选实现（空实现，返回NULL），对于移动数据网络接入的设备必选
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
 */
extern int alipay_iot_get_device_imei(char *inOut, size_t size);

/**
*  返回芯片camera ID字符串
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
*/
extern int alipay_iot_get_cameraid(char *inOut, size_t size);

/**
*  返回芯片的类型，比如MT6261
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，适配层实现方填写内存值。
 *  size，调用方内部申请的buffer的最大长度，适配层实现方将数据copy到buffer中时，长度不能大于size的值。
 * 返回值：
 *  小于0，表示出错的错误码；
 *  大于等于0，表示copy到buffer的数据实际长度；如果此项数据不存在，需要返回0.
*/
extern int alipay_iot_get_cputype(char *inOut, size_t size);

/**
* 获取一个非0的随机数
*/
extern int alipay_iot_rand(void);
/**
* 设定随机数种子
*/
extern void alipay_iot_srand(uint32_t seed);


/*
 * Protocol Familty.  Currently, we only support Internet
 */
#define ALIPAY_IOT_SOC_PF_INET (0)

/* Socket Type */
typedef enum
{
    ALIPAY_IOT_SOC_SOCK_STREAM = 0,  /* stream socket, TCP */
    ALIPAY_IOT_SOC_SOCK_DGRAM,       /* datagram socket, UDP */
}alipay_socket_type_enum;

/* Socket Options */
typedef enum
{
    ALIPAY_IOT_SOC_OOBINLINE     = 0x01 << 0,  /* not support yet */
    ALIPAY_IOT_SOC_LINGER        = 0x01 << 1,  /* linger on close */
    ALIPAY_IOT_SOC_NBIO          = 0x01 << 2,  /* Nonblocking */
    ALIPAY_IOT_SOC_ASYNC         = 0x01 << 3,  /* Asynchronous notification */

    ALIPAY_IOT_SOC_NODELAY       = 0x01 << 4,  /* disable Nagle algorithm or not */
    ALIPAY_IOT_SOC_KEEPALIVE     = 0x01 << 5,  /* enable/disable the keepalive */
    ALIPAY_IOT_SOC_RCVBUF        = 0x01 << 6,  /* set the socket receive buffer size */
    ALIPAY_IOT_SOC_SENDBUF       = 0x01 << 7,  /* set the socket send buffer size */
}alipay_soc_option_enum;

#define ALIPAY_IOT_MAX_IP_SOCKET_NUM   10
typedef struct
{
    uint8_t fds_bits[ALIPAY_IOT_MAX_IP_SOCKET_NUM];
} alipay_iot_fd_set;


extern void alipay_iot_fd_zero(alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_clrbit(int fd, alipay_iot_fd_set* fdset);
/*
* 检查fdset中对应的fd是否有被selected
*/
extern int  alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset);

/**
 * 返回-1表示失败，返回-2表示应再次尝试，
 * 返回0表示没有事件，返回正数表示有事件的句柄数。
 * 不会返回别的值。
 */
extern int alipay_iot_select(int          maxfdp1,
                      alipay_iot_fd_set  *readset,
                      alipay_iot_fd_set  *writeset,
                      alipay_iot_fd_set  *exceptset,
                      alipay_iot_timeval *timeout);

/**
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_dns(const char *name, unsigned char ip[4]);

/**
 * 创建套接口。
 * domain目前只支持ALIPAY_IOT_SOC_PF_INET
 * type为SOC_SOCK_STREAM表示TCP，为SOC_SOCK_DGRAM表示UDP。
 * protocol填0，除非type填了SOC_SOCK_RAW，则要指定如下之一：
 *     SOC_IPPROTO_IP
 *     SOC_IPPROTO_HOPOPTS
 *     SOC_IPPROTO_ICMP
 *     SOC_IPPROTO_IGMP
 *     SOC_IPPROTO_IPV4
 *     SOC_IPPROTO_IPIP
 *     SOC_IPPROTO_TCP
 *     SOC_IPPROTO_UDP
 *     SOC_IPPROTO_ESP
 *     SOC_IPPROTO_AH
 *     SOC_IPPROTO_IPCOMP
 *     SOC_IPPROTO_RAW
 *     SOC_IPPROTO_DONE
 * 返回-1表示失败，返回其他为套接口的描述符。
 */
extern int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol);

/**
 * 关闭套接口。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_close(int s);


struct alipay_iot_sockaddr {
  unsigned char sa_len;      //此值忽略。
  unsigned char sa_family;   //目前只支持ALIPAY_IOT_SOC_PF_INET。
  union {
    struct {
        unsigned short port;
        unsigned char  ip[4];
    }    sin_data;
    char sa_data[14];
  }             data;
};

/**
 * 设置套接口的本地地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址
 * namelen无效。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_bind(int                        s,
                    const struct alipay_iot_sockaddr *name,
                    unsigned int               namelen);

/**
 * 若是TCP套接口，则向指定地址发起连接；
 * 若是UDP套接口，则设置缺省的对端地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址
 * namelen无效。
 * 返回-1表示失败，返回-2表示异步操作正在进行中，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_connect(int                            s,
                        const struct alipay_iot_sockaddr *name,
                        unsigned int                    namelen);

/**
 * 向指定地址发送数据。一般用于UDP套接口。
 * flags无效。
 * to->sa_len无效，
 * to->sa_family必须是SOC_PF_INET，
 * to->data.sin_data.port和to->data.sin_data.ip分别填入端口号和IPv4地址
 * tolen无效。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
extern int alipay_iot_socket_sendto(int                           s,
                        const void                  *dataptr,
                        size_t                          size,
                        int                            flags,
                        const struct alipay_iot_sockaddr *to,
                        unsigned int                    tolen);

/**
 * 发送数据。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
extern int alipay_iot_socket_write(int s, const void *dataptr, size_t len);

/**
 * 接收数据并告知对端地址。一般用于UDP套接口。
 * flags可以用或操作组合以下任意值：
 *     SOC_MSG_OOB
 *     SOC_MSG_PEEK
 *     SOC_MSG_DONTROUTE
 *     SOC_MSG_EOR
 *     SOC_MSG_TRUNC
 *     SOC_MSG_CTRUNC
 *     SOC_MSG_WAITALL
 *     SOC_MSG_DONTWAIT
 * from->sa_len无效，
 * from->sa_family必须是SOC_PF_INET，
 * from->data.sin_data.port和from->data.sin_data.ip分别填入端口号和IPv4地址
 * fromlen指向的值无效。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
extern int alipay_iot_socket_recvfrom(int                     s,
                        void                        *mem,
                        size_t                       len,
                        int                        flags,
                        struct alipay_iot_sockaddr *from,
                        unsigned int            *fromlen);

/**
 * 接收数据。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
extern int alipay_iot_socket_read(int s, void *mem, size_t len);

/**
 * 设置套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_setsockopt(int          s,
                          int          level,
                          int          optname,
                          const void  *opval,
                          unsigned int optlen);

/**
 * 获取套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_NREAD，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_getsockopt(int         s,
                          int            level,
                          int          optname,
                          void          *opval,
                          unsigned int *optlen);



/**
 enum of network type 网络类型
 */
typedef enum{
    network_type_2g   = 0,/**2G*/
    network_type_3g   = 1,/**3G*/
    network_type_4g   = 2,/**4G*/
    network_type_wifi = 3,/**WIFI*/
    network_type_lan  = 4,/**有线*/
} alipay_iot_network_type_t;

typedef enum{
    coord_type_wgs84 = 0,/**WGS 84*/
    coord_type_gcj02 = 1,/**GCJ 02*/
} alipay_iot_coord_type_t;

#define ALIPAY_IOT_MAX_NODE_COUNT 3

typedef struct alipay_iot_ap_gsm_info{
    int mcc;        //移动国家代码
    int mnc;        //移动网络代码
    int lac;        //位置区域代码
    int cid;        //基站编号
    int rssi;       //信号强度
    int connected;  // 是否当前连接基站（1->当前连接基站，0->周边基站）
}alipay_iot_ap_gsm_info_t;

typedef struct alipay_iot_ap_gsm_info_node{
    int count;          //gsm_info具体个数
    alipay_iot_ap_gsm_info_t gsm_info[ALIPAY_IOT_MAX_NODE_COUNT];
}alipay_iot_ap_gsm_info_node_t;

typedef struct alipay_iot_ap_cdma_info{
    int sid;            //CDMA基站系统ID
    int nid;            //CDMA网络ID
    int bsid;           //CDMA基站系统ID
    int rssi;           //信号强度
    int connected;      // 是否当前连接基站（1表示当前连接基站，0表示周边基站）
}alipay_iot_ap_cdma_info_t;

typedef struct alipay_iot_ap_cdma_info_node{
    int count;          //cdma_info具体个数
    alipay_iot_ap_cdma_info_t cdma_info[ALIPAY_IOT_MAX_NODE_COUNT];
}alipay_iot_ap_cdma_info_node_t;

typedef struct alipay_iot_ap_wifi_info{
    char *ssid;         //ssid
    char *mac;          //mac地址
    int rssi;           //信号强度
    int connected;      // 是否当前连接基站（1表示当前连接wifi，0表示周边wifi）
}alipay_iot_ap_wifi_info_t;

typedef struct alipay_iot_ap_wifi_info_node{
    int count;//wifi_info具体个数
    alipay_iot_ap_wifi_info_t wifi_info[ALIPAY_IOT_MAX_NODE_COUNT];
}alipay_iot_ap_wifi_info_node_t;


typedef struct alipay_iot_bluetooth_info{
    char *address;      //蓝牙mac地址
    char *uuid;         //蓝牙uuid
    char *name;         //蓝牙名称
    int connected;      // 是否当前连接蓝牙（1表示当前连接，0表示周边）
}alipay_iot_bluetooth_info_t;

typedef struct alipay_iot_bluetooth_info_node{
    int count;          //bluetooth_info具体个数
    alipay_iot_bluetooth_info_t bluetooth_info[ALIPAY_IOT_MAX_NODE_COUNT];
}alipay_iot_bluetooth_info_node_t;

typedef struct alipay_iot_latitude_longitude_info{
    char* latitude;                   /**纬度，比如:39.21；正值表示北纬，负值表示南纬*/
    char* longitude;                  /**经度，比如:104.04；正值表示东经，负值表示西经*/
    alipay_iot_coord_type_t coordtype;      /**经纬度坐标系类型"*/
}alipay_iot_latitude_longitude_info_t;


typedef struct {
    alipay_iot_network_type_t net_type;                     /**网络类型(枚举)*/
    alipay_iot_ap_gsm_info_node_t *gsm_info_node;           /**gsm信息，如果没有则gsm_info_node=NULL*/
    alipay_iot_ap_cdma_info_node_t *cdma_info_node;         /**cdma信息，如果没有则cdma_info_node=NULL*/
    alipay_iot_ap_wifi_info_node_t *wifi_info_node;         /**wifi信息，如果没有则wifi_info_node=NULL*/
    alipay_iot_bluetooth_info_node_t *bluetooth_info_node;  /**蓝牙信息，如果没有则bluetooth_info_node=NULL*/
    alipay_iot_latitude_longitude_info_t *ll_info;          /**经纬度信息，没有则ll_info=NULL*/
}alipay_iot_coll_lbs_t;

/**
* 获取位置信息
* 建议十分钟更新一次
* 返回值：
* 位置信息指针，位置信息建议十分钟更新一次。该指针会被SDK调用alipay_iot_free_lbs_info()释放
*/
extern alipay_iot_coll_lbs_t *alipay_iot_get_lbs_info(void);

/**
* 释放alipay_iot_get_lbs_info中返回的指针数据。
*/
extern void alipay_iot_free_lbs_info(alipay_iot_coll_lbs_t *lbs);

#ifdef __ALIPAY_IOT_KV__
/*
* @brief: flash写入
* @param: poff, offset of regisn provided by kv flash.
* @param: buf, pointer to data ready to write.
* @param: buf_size, size of data ready to write.
* @ret: 0 write success. other write fail.
*/
int32_t alipay_iot_hal_flash_write(uint32_t poff, const void* buf ,uint32_t buf_size);

/*
* @brief: flash读取
* @param: poff, offset of regisn provided by kv flash.
* @param: buf, pointer to buffer ready for reading to.
* @param: buf_size, size of data want to read.
* @ret: 0 read success. other read fail.
*/
int32_t alipay_iot_hal_flash_read(uint32_t poff, void* buf, uint32_t buf_size);

/*
* @brief: flash擦除
* @param: poff, offset of regisn provided by kv flash.
* @param: size, size of data want to erase.
* @ret: 0 erase success. other erase fail.
*/
int32_t alipay_iot_hal_flash_erase(uint32_t poff,uint32_t size);
#endif

#ifdef __ALIPAY_IOT_OTA_SDK__

/*
* @brief: 按flash绝对地址擦除
* @param: uwAddr, flash的绝对地址.
* @param: size, size of data want to erase.
* @ret: 0 erase success. other erase fail.
*/
int32_t _alipay_iot_flash_erase(uint32_t uwAddr, uint32_t size);

/*
* @brief: 按flash绝对地址读取
* @param: uwAddr, flash的绝对地址.
* @param: buf, pointer to buffer ready for reading to.
* @param: size, size of data want to read.
* @ret: 0 read success. other read fail.
*/
int32_t _alipay_iot_flash_read(uint32_t uwAddr, void* buf, uint32_t size);

/*
* @brief: 按flash绝对地址写入
* @param: uwAddr, flash的绝对地址.
* @param: buf, pointer to data ready to write.
* @param: size, size of data ready to write.
* @ret: 0 write success. other write fail.
*/
int32_t _alipay_iot_flash_write(uint32_t uwAddr, void* buf, uint32_t size);

/*
* @brief: OTA升级刷写开始接口，接口完成必要的刷写初始化，如确定刷写地址，
擦除空间等，每次升级只执行一次。
* @param:
* @ret: 0 write success. other write fail.
*/
int32_t alipay_iot_ota_app_upgrade_start(uint32_t size);

/*
* @brief: OTA升级刷写操作接口，每次升级会连续执行该函数，直到本次升级的固件全部刷写完成。
* @param: buff, pointer to data ready to write.
* @param: size, size of data want to write
* @ret: 0 write success. other write fail.
*/
int32_t alipay_iot_ota_app_upgrade_write(void *buff, uint32_t len);

/*
* @brief: OTA升级刷写结束接口，接口完成必要的刷写状态更新，如OTA标记等。
* @param:
* @ret: 0 write success. other write fail.
*/
int32_t alipay_iot_ota_app_upgrade_finish(void);

/*
* @brief: 复位请求接口，SDK调用此接口向系统请求复位。
* @param:
* @ret: 0 write success. other write fail.
*/
void alipay_iot_reboot_request(void);


#endif

#endif

