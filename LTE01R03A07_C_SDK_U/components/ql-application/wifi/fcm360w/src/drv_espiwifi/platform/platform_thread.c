#include "string.h"
#include "platform_os.h"

#define QL_TASK_EVENT_CNT 5

int aos_task_new_ext(aos_task_t *task, const char *name, void (*fn)(void *),
                    void *arg, size_t stack_size, int32_t prio)
{
    return (QL_OSI_SUCCESS == ql_rtos_task_create(task,stack_size,prio,(char*)name,fn,arg,QL_TASK_EVENT_CNT))? 0 : -1;
}

void aos_task_exit(int32_t code)
{
    /* task exit by itself */
    ql_rtos_task_delete(NULL);
}

void aos_task_delete(aos_task_t *task)
{
    /* task exit by itself */
    ql_rtos_task_delete(*task);
}

platform_thread_t *platform_thread_init(const char *name, unsigned int pir, unsigned int stack_size,
    void (*entry)(void *), void *args)
{
    platform_thread_t *thread = NULL;
    int ret;

    if (entry == NULL || name == NULL) {
        platform_log_e("%s[%d] invalid input params\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    thread = platform_memory_alloc(sizeof(platform_thread_t));
    if (thread == NULL) {
        platform_log_e("%s[%d] alloc thread failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    memset(thread, 0, sizeof(platform_thread_t));
    strncpy(thread->name, name, sizeof(thread->name));
    ret = aos_task_new_ext(&(thread->handle), thread->name, entry, args, stack_size, pir);
    if (ret != 0) {
        platform_log_e("%s[%d] create thread failed\n", __FUNCTION__, __LINE__);
        platform_memory_free(thread);
        return NULL;
    }
    thread->state = PLATFORM_THREAD_INIT;

    return thread;
}

void platform_thread_stop(platform_thread_t *thread)
{
}

void platform_thread_start(platform_thread_t *thread)
{
}

void platform_thread_kill(platform_thread_t *thread)
{
    if (thread != NULL) {
        aos_task_exit(0);
        thread->state = PLATFORM_THREAD_STOP;
    }
}

void platform_thread_destory(platform_thread_t *thread)
{
    if (thread != NULL) {
        aos_task_delete(&(thread->handle));
        platform_memory_free(thread);
    }
}

