/*
 * Tencent is pleased to support the open source community by making IoT Hub
 available.
 * Copyright (C) 2018-2020 THL A29 Limited, a Tencent company. All rights
 reserved.

 * Licensed under the MIT License (the "License"); you may not use this file
 except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND,
 * either express or implied. See the License for the specific language
 governing permissions and
 * limitations under the License.
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "ql_rtos.h"
//#include "utlTime.h"
#include "ql_api_osi.h"

#include "qcloud_iot_export_error.h"
#include "qcloud_iot_import.h"

void *HAL_MutexCreate(void)
{
#ifdef MULTITHREAD_ENABLED
	ql_mutex_t mutex = NULL;
	ql_rtos_mutex_create(&mutex);
    return (void *)mutex;
#else
    return (void *)0xFFFFFFFF;
#endif
}

void HAL_MutexDestroy(_IN_ void *mutex)
{
#ifdef MULTITHREAD_ENABLED
	if(mutex)
		ql_rtos_mutex_delete((ql_mutex_t)mutex);
#else
    return;
#endif
}

void HAL_MutexLock(_IN_ void *mutex)
{
#ifdef MULTITHREAD_ENABLED
	if(mutex)
		ql_rtos_mutex_lock((ql_mutex_t)mutex, QL_WAIT_FOREVER);
#else
    return;
#endif
}

int HAL_MutexTryLock(_IN_ void *mutex)
{
#ifdef MULTITHREAD_ENABLED
	if(mutex)
		return ql_rtos_mutex_try_lock((ql_mutex_t)mutex);
	return -1;
#else
    return 0;
#endif
}

void HAL_MutexUnlock(_IN_ void *mutex)
{
#ifdef MULTITHREAD_ENABLED
	if(mutex)
		ql_rtos_mutex_unlock((ql_mutex_t)mutex);
#else
    return;
#endif
}

void *HAL_Malloc(_IN_ uint32_t size)
{
    return malloc(size);
}

void HAL_Free(_IN_ void *ptr)
{
    if (ptr)
        free(ptr);
}

void HAL_Printf(_IN_ const char *fmt, ...)
{
//	char *print_buf = malloc(2048);
//	if(!print_buf) return;
//
//	va_list args;
//	int len = 0;
//
//	va_start(args, fmt);
//	len = vsnprintf(print_buf, 2048, fmt, args);
//	va_end(args);
//
//	if(len <= 0) return;
//
//	printf("%s", print_buf);
//
//	free(print_buf);
}

int HAL_Snprintf(_IN_ char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int     rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);

    return rc;
}

int HAL_Vsnprintf(_IN_ char *str, _IN_ const int len, _IN_ const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

uint32_t HAL_GetTimeMs(void)
{
    ql_timeval_t time_val = {0};
    uint32_t       time_ms;

    ql_gettimeofday(&time_val);
    time_ms = time_val.sec * 1000 + time_val.usec / 1000;

    return time_ms;
}

void HAL_SleepMs(_IN_ uint32_t ms)
{
    ql_rtos_task_sleep_ms(ms);
}

#if ((defined(MULTITHREAD_ENABLED)) || (defined AT_TCP_ENABLED))
// platform-dependant thread routine/entry function
//static void *_HAL_thread_func_wrapper_(void *ptr)
//{
//    ThreadParams *params = (ThreadParams *)ptr;
//
//    params->thread_func(params->user_arg);
//
//	ql_rtos_task_delete(NULL);
//    return NULL;
//}

// platform-dependant thread create function
int HAL_ThreadCreate(ThreadParams *params)
{
    if (params == NULL)
    {
        return QCLOUD_ERR_INVAL;
    }

	int ret = ql_rtos_task_create((ql_task_t *)&params->thread_id, params->stack_size, params->priority + 99, params->thread_name, params->thread_func, params->user_arg,5);
    if (ret) {
        HAL_Printf("%s: ql_rtos_task_create failed: %d\n", __FUNCTION__, ret);
        return QCLOUD_ERR_FAILURE;
    }

    return QCLOUD_RET_SUCCESS;
}

int HAL_ThreadDestroy(void *threadId)
{
    if (NULL == threadId) {
        return QCLOUD_ERR_FAILURE;
    }

	if(ql_rtos_task_delete((ql_task_t)threadId))
		return QCLOUD_ERR_FAILURE;

	return QCLOUD_RET_SUCCESS;
}

#endif

#ifdef AT_TCP_ENABLED

void HAL_DelayMs(_IN_ uint32_t ms)
{
    ql_rtos_task_sleep_ms(ms);
}

void *HAL_SemaphoreCreate(void)
{
	ql_sem_t sem = NULL;
	ql_rtos_semaphore_create(&sem, 0);
	return (void *)sem;
}

void HAL_SemaphoreDestroy(void *sem)
{
	if(sem)
		ql_rtos_semaphore_delete((ql_sem_t)sem);
}

void HAL_SemaphorePost(void *sem)
{
	if(sem)
    	ql_rtos_semaphore_release((ql_sem_t)sem);
}

int HAL_SemaphoreWait(void *sem, uint32_t timeout_ms)
{
	if(sem)
		ql_rtos_semaphore_wait((ql_sem_t)sem, timeout_ms);
}

#endif
