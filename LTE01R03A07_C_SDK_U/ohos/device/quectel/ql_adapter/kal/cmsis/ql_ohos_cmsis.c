
#include "los_config.h"
#include "ql_ohos_os.h"

ql_ohos_cmsis_t  *g_ql_ohos_cmsis_handle = NULL;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#if (CMSIS_OS_VER == 2)

osStatus_t osKernelInitialize(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelInitialize == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelInitialize();
}

osStatus_t osKernelGetInfo(osVersion_t *version, char *id_buf, uint32_t id_size)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetInfo == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetInfo(version, id_buf, id_size);
}

osKernelState_t osKernelGetState(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetState == NULL)
    {
        return osKernelError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetState();
}


osStatus_t osKernelStart(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelStart == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelStart();
}


int32_t osKernelLock(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelLock == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelLock();
}

int32_t osKernelUnlock(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelUnlock == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelUnlock();
}


int32_t osKernelRestoreLock(int32_t lock)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelRestoreLock == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osKernelRestoreLock(lock);
}


uint32_t osKernelGetTickCount(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetTickCount == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetTickCount();
}

uint32_t osKernelGetTickFreq(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetTickFreq == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetTickFreq();
}

uint32_t osKernelGetSysTimerCount(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetSysTimerCount == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetSysTimerCount();
}


uint32_t osKernelGetSysTimerFreq(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osKernelGetSysTimerFreq == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osKernelGetSysTimerFreq();
}


//  ==== Thread Management Functions ====

osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadNew == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osThreadNew(func, argument, attr);
}


const char *osThreadGetName(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetName == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetName(thread_id);
}


osThreadId_t osThreadGetId(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetId == NULL)
    {
        return NULL;        
    }

    return (osThreadId_t)(g_ql_ohos_cmsis_handle->osThreadGetId());
}

void *osThreadGetArgument(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetArgument == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetArgument();
}

osThreadState_t osThreadGetState(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetState == NULL)
    {
        return osThreadError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetState(thread_id);
}


uint32_t osThreadGetStackSize(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetStackSize == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetStackSize(thread_id);
}


uint32_t osTaskStackWaterMarkGet(UINT32 taskID)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTaskStackWaterMarkGet == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osTaskStackWaterMarkGet(taskID);
}


uint32_t osThreadGetStackSpace(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetStackSpace == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetStackSpace(thread_id);
}


osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadSetPriority == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadSetPriority(thread_id, priority);
}


osPriority_t osThreadGetPriority(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetPriority == NULL)
    {
        return osPriorityError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetPriority(thread_id);
}


osStatus_t osThreadYield(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadYield == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadYield();
}


osStatus_t osThreadSuspend(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadSuspend == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadSuspend(thread_id);
}


osStatus_t osThreadResume(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadResume == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadResume(thread_id);
}


osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadTerminate == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osThreadTerminate(thread_id);
}


uint32_t osThreadGetCount(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetCount == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osThreadGetCount();
}


//  ==== Generic Wait Functions ====
osStatus_t osDelay(uint32_t ticks)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadGetCount == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osDelay(ticks);
}


osStatus_t osDelayUntil(uint32_t ticks)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osDelayUntil == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osDelayUntil(ticks);
}

//  ==== Timer Management Functions ====
#if (LOSCFG_BASE_CORE_SWTMR == 1)
osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerNew == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osTimerNew(func, type, argument, attr);
}

osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerStart == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osTimerStart(timer_id, ticks);
}


const char *osTimerGetName(osTimerId_t timer_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerGetName == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osTimerGetName(timer_id);
}


osStatus_t osTimerStop(osTimerId_t timer_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerStop == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osTimerStop(timer_id);
}


uint32_t osTimerIsRunning(osTimerId_t timer_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerIsRunning == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osTimerIsRunning(timer_id);
}


osStatus_t osTimerDelete(osTimerId_t timer_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osTimerDelete == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osTimerDelete(timer_id);
}
#endif

osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsNew == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsNew(attr);
}


const char *osEventFlagsGetName(osEventFlagsId_t ef_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsGetName == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsGetName(ef_id);
}


uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsSet == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsSet(ef_id, flags);
}


uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsClear == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsClear(ef_id, flags);
}


uint32_t osEventFlagsGet(osEventFlagsId_t ef_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsGet == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsGet(ef_id);
}

uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsWait == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsWait(ef_id, flags, options, timeout);
}

osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osEventFlagsDelete == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osEventFlagsDelete(ef_id);
}

//  ==== Mutex Management Functions ====
#if (LOSCFG_BASE_IPC_MUX == 1)
osMutexId_t osMutexNew(const osMutexAttr_t *attr)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMutexNew == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osMutexNew(attr);
}


osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMutexAcquire == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osMutexAcquire(mutex_id, timeout);
}


osStatus_t osMutexRelease(osMutexId_t mutex_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMutexRelease == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osMutexRelease(mutex_id);
}


osThreadId_t osMutexGetOwner(osMutexId_t mutex_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMutexGetOwner == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osMutexGetOwner(mutex_id);
}


osStatus_t osMutexDelete(osMutexId_t mutex_id)
{
   if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMutexDelete == NULL)
   {
       return osError;        
   }

    return g_ql_ohos_cmsis_handle->osMutexDelete(mutex_id);
}
#endif

//  ==== Semaphore Management Functions ====
#if (LOSCFG_BASE_IPC_SEM == 1)

osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
   if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osSemaphoreNew == NULL)
   {
       return NULL;        
   }

    return g_ql_ohos_cmsis_handle->osSemaphoreNew(max_count, initial_count, attr);
}


osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osSemaphoreAcquire == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osSemaphoreAcquire(semaphore_id, timeout);
}


osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osSemaphoreRelease == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osSemaphoreRelease(semaphore_id);
}


uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osSemaphoreGetCount == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osSemaphoreGetCount(semaphore_id);
}


osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphore_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osSemaphoreDelete == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osSemaphoreDelete(semaphore_id);
}
#endif


//  ==== Message Queue Management Functions ====
#if (LOSCFG_BASE_IPC_QUEUE == 1)
osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueNew == NULL)
    {
        return NULL;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueNew(msg_count, msg_size, attr);
}


osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueuePut == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueuePut(mq_id, msg_ptr, msg_prio, timeout);
}


osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueGet == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout);
}

uint32_t osMessageQueueGetCapacity(osMessageQueueId_t mq_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueGetCapacity == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueGetCapacity(mq_id);
}

uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mq_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueGetMsgSize == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueGetMsgSize(mq_id);
}


uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueGetCount == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueGetCount(mq_id);
}


uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueGetSpace == NULL)
    {
        return 0;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueGetSpace(mq_id);
}

osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osMessageQueueDelete == NULL)
    {
        return osError;        
    }

    return g_ql_ohos_cmsis_handle->osMessageQueueDelete(mq_id);
}

void osThreadExit(void)
{
    if(g_ql_ohos_cmsis_handle == NULL || g_ql_ohos_cmsis_handle->osThreadExit == NULL)
    {
        return;        
    }

    g_ql_ohos_cmsis_handle->osThreadExit();
}
#endif

int ql_ohos_cmsis_api_register(ql_ohos_cmsis_t *cmsis_os)
{
    if(cmsis_os == NULL)
    {
        return -1;
    }

    g_ql_ohos_cmsis_handle = cmsis_os;

    return 0;
}

int ql_ohos_cmsis_api_unregister(void)
{
    g_ql_ohos_cmsis_handle = NULL;

    return 0;
}

#endif // (CMSIS_OS_VER == 2)
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */