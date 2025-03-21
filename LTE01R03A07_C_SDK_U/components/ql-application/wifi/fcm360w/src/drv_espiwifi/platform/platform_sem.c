#include "platform_os.h"

int aos_sem_new(aos_sem_t *sem, uint32_t count)
{
    if (sem == NULL)
        return -1;
    return (QL_OSI_SUCCESS == ql_rtos_semaphore_create(sem,count))? 0 : -1;
}

void aos_sem_signal(aos_sem_t *sem)
{
    ql_rtos_semaphore_release(*sem);
}

int aos_sem_wait(aos_sem_t *sem,uint32_t timeout)
{
    return (QL_OSI_SUCCESS == ql_rtos_semaphore_wait(*sem,timeout))? 0 : -1;
}

void aos_sem_free(aos_sem_t *sem)
{
    ql_rtos_semaphore_delete(*sem);
}

platform_sem_t *platform_sem_create(size_t maxcount, size_t initalcount)
{
    platform_sem_t *sem = platform_memory_alloc(sizeof(platform_sem_t));
    int ret;

    if (sem == NULL) {
        platform_log_e("%s[%d] alloc sem failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    ret = aos_sem_new(&(sem->sem), initalcount);
    if (ret != 0) {
        platform_log_e("%s[%d] create sem failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    return sem;
}

void platform_sem_post(platform_sem_t *sem)
{
    aos_sem_signal(&(sem->sem));
}

int platform_sem_wait(platform_sem_t *sem, size_t timeout_ms)
{
    return aos_sem_wait(&(sem->sem), timeout_ms);
}

void platform_sem_destroy(platform_sem_t *sem)
{
    aos_sem_free(&(sem->sem));
    platform_memory_free(sem);
}
