
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#ifndef _QL_API_OSI_H_
#define _QL_API_OSI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_osi_def.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"

#define QL_TIMER_IN_SERVICE ((void *)0xffffffff)

typedef int    QlOSStatus;
typedef void * ql_task_t;
typedef void * ql_sem_t;
typedef void * ql_mutex_t;
typedef void * ql_queue_t;
typedef void * ql_timer_t;
typedef void * ql_flag_t;
typedef void * ql_egroup_t;

typedef int    QuecOSStatus; 
typedef uint32    ql_event_bits_t;

typedef enum
{
	QL_WAIT_FOREVER = 0xFFFFFFFFUL,
	QL_NO_WAIT	  	= 0
} ql_wait_e;

typedef enum
{
	Running = 0,	/* A task is querying the state of itself, so must be running. */
	Ready,			/* The task being queried is in a read or pending ready list. */
	Blocked,		/* The task being queried is in the Blocked state. */
	Suspended,		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	Deleted,		/* The task being queried has been deleted, but its TCB has not yet been freed. */
	Invalid			/* Used as an 'invalid state' value. */
} ql_task_state_e;

typedef struct 
{
	ql_task_t			xHandle;			/* The handle of the task to which the rest of the information in the structure relates. */
	const char *		pcTaskName;			/* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */
	ql_task_state_e 	eCurrentState;		/* The state in which the task existed when the structure was populated. */
	unsigned long 		uxCurrentPriority;	/* The priority at which the task was running (may be inherited) when the structure was populated. */
	uint16 				usStackHighWaterMark;	/* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
    uint32              eventPendingCnt;   /*Thread event suspend count*/
} ql_task_status_t;

typedef struct
{
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
    uint32 param2; ///< 2nd parameter
    uint32 param3; ///< 3rd parameter
} ql_event_t;

/* Note: The (ql_timeval_t) and (struct timeval) data structures are independent of each other. Please do not equate them. */
typedef struct
{
	uint32 sec;
	uint32 usec;
}ql_timeval_t;

typedef struct
{
	uint total_idle_tick;
	uint sys_tick_old;
	uint cpu_using;
	uint idle_in_tick;
	uint idle_out_tick;
}ql_cpu_using_info_t;

/*========================================================================
 *	function Definition
 *========================================================================*/

typedef void (*ql_swdog_callback)(uint32 id_type, void *ctx);
typedef void (*lvgl_Callback_t)(void *ctx);

/**
 * \brief Call malloc()
 * size - The size of the memory block, in bytes
 * Attention: The allocated memory has been memset()
 *
 * \return  This function returns a pointer. If the request fails, NULL is returned.
 */
void* ql_malloc(int size);

/**
 * \brief Call free()
 * Free the memory space allocated by ql_malloc
 *
 * Attention: Contains null pointer judgments
 *
 * void ql_free(void *ptr);
*/
#define QL_FREE_PTR(p)    {if(p != NULL) {free(p);p = NULL;}}
#define ql_free(p)    	  QL_FREE_PTR(p)

extern QlOSStatus ql_rtos_task_create
(
	ql_task_t	 * taskRef, 			 /* OS task reference										   				  */
	uint32		   stackSize,			 /* number of bytes in task stack area						   				  */
	uint8 		   priority,			 /* task priority, users should use the defination in APP_ThreadPriority_e, 
											and not recommand beyond APP_PRIORITY_ABOVE_NORMAL 						  */
	char		 * taskName,			 /* task name, no more than 16 bytes, exceed 16 bytes will only save 16 bytes */
	void	   ( * taskStart)(void*),	 /* pointer to task entry point 			 				   				  */
	void*		   argv,				 /* task entry argument pointer 							   				  */
	uint32         event_count,          /* the max event count that the task can waiting to process   				  */ 
 	...
);

extern QlOSStatus ql_rtos_task_create_default
(
	ql_task_t	 * taskRef, 			 /* OS task reference										  				  */
	uint32		   stackSize,			 /* number of bytes in task stack area						   				  */
	uint8 		   priority,			 /* task priority, users should use the defination in APP_ThreadPriority_e, 
											and not recommand beyond APP_PRIORITY_ABOVE_NORMAL 						  */
	char		 * taskName,			 /* task name, no more than 16 bytes, exceed 16 bytes will only save 16 bytes */
	void	   ( * taskStart)(void*),	 /* pointer to task entry point 			 				   				  */
	void*		   argv,				 /* task entry argument pointer 							   				  */
 	...
);

//for app task, the event ID of the 2nd param, should between QL_EVENT_APP_START and QL_EVEN_MAX
//if target task has no space to store event(event is full), this api will not wait it, and will return QL_OSI_EVENT_SEND_FAIL
//The minimum event id value is 10 
extern QlOSStatus ql_rtos_event_send
(
	ql_task_t	task_ref,
	ql_event_t	*event
);

//for app task, the event ID of the 2nd param, should between QL_EVENT_APP_START and QL_EVEN_MAX
//if target task has no space to store event(event is full), this api will not wait it, and will return QL_OSI_EVENT_SEND_FAIL
//The minimum event id value is 10 
QlOSStatus ql_rtos_event_send_ex
(
	ql_task_t   task_ref,
	ql_event_t  *event,
	uint32 		timeout_ms
);

extern QlOSStatus ql_event_wait
(
	ql_event_t	 *event_strc,      /* event structure     */
	uint32		 timeout           /* event wait timeout  */
);

extern QlOSStatus ql_event_try_wait
(
	ql_event_t	 *event_strc	/* event structure     */
);

QlOSStatus ql_rtos_event_group_create
(
	ql_egroup_t	 * egroupRef                /* OS event group reference */
);

QlOSStatus ql_rtos_event_group_wait
(
    ql_egroup_t egroupRef,                 /* OS event group reference */
    const ql_event_bits_t uxBitsToWaitFor, /* A bitwise value that indicates the bit or bits to test inside the event group.
                                              ex:to wait for bit 0 and/or bit 2 set uxBitsToWaitFor to 0x05. */
    const bool ClearOnExit,                /* Whether the event group is cleared after the conditions are met. TRUE -- clear; FALSE -- not clear */
    const bool WaitForAllBits,             /* Whether to wait until all bits set by the uxBitsToWaitFor parameter are set or the timeout period expires */
    ql_event_bits_t *curr_bits,            /* The value of the event group at the time either the bits being waited for became set, or the block time expired */
    uint32 		timeout_ms                 /* event group wait timeout */
);

QlOSStatus ql_rtos_event_group_clear
(
    ql_egroup_t egroupRef,                 /* OS event group reference */
    const ql_event_bits_t uxBitsToClear,   /* The event group in which the bits are to be cleared*/
    ql_event_bits_t *curr_bits             /* The value of the current event group */
);

QlOSStatus ql_rtos_event_group_set
(
    ql_egroup_t egroupRef,                 /* OS event group reference */
    const ql_event_bits_t uxBitsToSet,     /* The event group in which the bits are to be set */
    ql_event_bits_t *curr_bits             /* The value of the current event group */
);

/*
Atomically set bits within an event group, then wait for a combination of bits to be set within the same event group.
This functionality is typically used to synchronise multiple tasks, where each task has to wait for the other tasks to reach a synchronisation point before proceeding. 
The function will return before its block time expires if the bits specified by the uxBitsToWait parameter are set, or become set within that time.
In this case all the bits specified by uxBitsToWait will be automatically cleared before the function returns.
*/
QlOSStatus ql_rtos_event_group_sync
(
    ql_egroup_t egroupRef,                 /* OS event group reference */
    const ql_event_bits_t uxBitsToSet,     /* The bits to set in the event group before determining if, and possibly waiting for,
                                              all the bits specified by the uxBitsToWait parameter are set */
    const ql_event_bits_t uxBitsToWaitFor, /* A bitwise value that indicates the bit or bits to test inside the event group. */
    ql_event_bits_t *curr_bits,            /* The value of the current event group */
    uint32 		timeout_ms                 /* event group wait timeout */
);

QlOSStatus ql_rtos_event_group_get
(
    ql_egroup_t egroupRef,                 /* OS event group reference */
    ql_event_bits_t *curr_bits             /* The value of the current event group */
);

QlOSStatus ql_rtos_event_group_delete
(
    ql_egroup_t egroupRef                /* OS event group reference */
);

//if you delete the current task itself, the code behind ql_rtos_task_delete will have no chance to run
extern QlOSStatus ql_rtos_task_delete
(
	ql_task_t taskRef		/* OS task reference	*/
);

extern QlOSStatus ql_rtos_task_suspend
(
	ql_task_t taskRef		/* OS task reference	*/
);

extern QlOSStatus ql_rtos_task_resume
(
	ql_task_t taskRef		/* OS task reference	*/
);

extern void ql_rtos_task_yield(void);

extern QlOSStatus ql_rtos_task_get_current_ref
(
	ql_task_t * taskRef		/* OS task reference	*/
);

extern QlOSStatus ql_rtos_task_change_priority
(
	ql_task_t 	taskRef,			/* OS task reference			*/
	uint8 		new_priority,		/* OS task new priority	for in	*/
	uint8	   *old_priority		/* OS task old priority	for out	*/
);

extern QlOSStatus ql_rtos_task_get_priority
(
	ql_task_t 	taskRef,		/* OS task reference			*/
	uint8 * 	priority_ptr	/* OS task priority for out		*/
);

extern QlOSStatus ql_rtos_task_get_status
(
	ql_task_t      		 task_ref,
	ql_task_status_t 	 *status
);

extern void ql_rtos_task_sleep_ms
(
	uint32 ms	   /* OS task sleep time for ms	*/
);

extern void ql_rtos_task_sleep_s
(
	uint32 s	   /* OS task sleep time for s		*/
);

QlOSStatus ql_rtos_task_set_userdata
(
	ql_task_t taskRef,  /* OS task reference	*/
	void *userData      /* The user data of pointer type */
);


QlOSStatus ql_rtos_task_get_userdata
(
	ql_task_t taskRef, /* OS task reference	*/
	void **userData     /* The user data of pointer type */
);

/*
临界区中需注意：
* 1. 临界区中不可有阻塞，延迟动作
* 2. 临界区中不可使用操作系统带阻塞类接口（事件发送且带timeout，信号量等待且带timeout等）
* 3. 临界区中不可调用Audio停止/开始相关接口, TTS停止/开始接口，文件读写接口
* 4. 临界区中不可调用CFW（RPC相关）接口
*/
extern uint32_t ql_rtos_enter_critical(void);

//the parameter is the return value from ql_rtos_enter_critical()
extern void ql_rtos_exit_critical(uint32_t critical);

extern QlOSStatus ql_rtos_semaphore_create
(
	ql_sem_t  	*semaRef,       /* OS semaphore reference                     	*/
	uint32      initialCount    /* initial count of the semaphore             	*/
);

extern QlOSStatus ql_rtos_semaphore_create_ex
(
	ql_sem_t	 *semaRef,		 /* OS semaphore reference						 */
	uint32		 initialCount,	 /* initial count of the semaphore				 */
	uint32  	 max_cnt		 /* max count of the semaphore				 	 */
);

extern QlOSStatus ql_rtos_semaphore_wait
(
	ql_sem_t  	semaRef,       /* OS semaphore reference                     	*/
	uint32      timeout    	   /* QL_WAIT_FOREVER, QL_NO_WAIT, or timeout	*/
);

extern QlOSStatus ql_rtos_semaphore_release
(
	ql_sem_t   semaRef        /* OS semaphore reference						*/
);

extern QlOSStatus ql_rtos_semaphore_get_cnt
(
	ql_sem_t  	semaRef,       /* OS semaphore reference           				*/
	uint32    * cnt_ptr    	   /* out-parm to save the cnt of semaphore      	*/
);

extern QlOSStatus ql_rtos_semaphore_delete
(
	ql_sem_t   semaRef        /* OS semaphore reference                  		*/
);

extern QlOSStatus ql_rtos_mutex_create
(
    ql_mutex_t  *mutexRef        /* OS mutex reference                         */
);

extern QlOSStatus ql_rtos_mutex_lock
(
	ql_mutex_t    mutexRef,       /* OS mutex reference                         */
	uint32        timeout   	  /* mutex wait timeout		             		*/
);

extern QlOSStatus ql_rtos_mutex_try_lock
(
	ql_mutex_t  mutexRef        /* OS mutex reference                         */
);

extern QlOSStatus ql_rtos_mutex_unlock
(
	ql_mutex_t  mutexRef        /* OS mutex reference                         */
);

extern QlOSStatus ql_rtos_mutex_delete
(
	ql_mutex_t  mutexRef        /* OS mutex reference                         */
);

extern QlOSStatus ql_rtos_queue_create
(
	ql_queue_t   	*msgQRef,       	/* OS message queue reference              */
	uint32         	maxSize,        	/* max message size the queue supports     */
	uint32         	maxNumber	      	/* max # of messages in the queue          */
);

extern QlOSStatus ql_rtos_queue_wait
(
	ql_queue_t   	msgQRef,		/* message queue reference                 		*/
	uint8  	      	*recvMsg,       /* pointer to the message received         		*/
	uint32         	size, 			/* size of the message                     		*/
	uint32         	timeout         /* QL_WAIT_FOREVER, QL_NO_WAIT, or timeout  */
);

extern QlOSStatus ql_rtos_queue_release
(
    ql_queue_t		msgQRef,        /* message queue reference                 			*/
    uint32         	size,           /* size of the message                     			*/
    uint8          	*msgPtr,        /* start address of the data to be sent    			*/
    uint32         	timeout         /* QL_WAIT_FOREVER, QL_NO_WAIT, or timeout   	*/
);

extern QlOSStatus ql_rtos_queue_get_cnt
(
	ql_queue_t		msgQRef,        /* message queue reference                 		*/
	uint32     		*cnt_ptr    	/* out-parm to save the cnt of message queue	*/
);

extern QlOSStatus ql_rtos_queue_get_free_cnt
(
	ql_queue_t		msgQRef,        /* message queue reference                 		*/
	uint32     		*cnt_ptr    	/* out-parm to save the cnt of message queue	*/
);

extern QlOSStatus ql_rtos_queue_reset
(
    ql_queue_t      msgQRef       /* message queue reference                      */
);

extern QlOSStatus ql_rtos_queue_delete
(
	ql_queue_t	msgQRef         /* message queue reference                 		*/
);
/*
* taskRef取值: 
* 若指定task去运行定时器回调函数，则填入任务句柄；若在系统service中运行定时器回调函数，填入QL_TIMER_IN_SERVICE
* 若需要在中断中运行定时器回调函数，则填入NULL, 定时器超时时将会产生中断并在中断中调用回调函数，但以下几点需注意：
* 1. 中断中不可有阻塞，延迟动作，否则可能会引起未知后果
* 2. 中断中不可进临界区(调用critical相关接口)，操作系统带阻塞类接口（事件发送且带timeout，信号量等待且带timeout等）
* 3. 中断中不可调用Audio停止/开始相关接口, TTS停止/开始接口，文件读写接口
* 4. 中断中不可调用CFW（RPC相关）接口
* 建议中断中只做置标致位，做简单运算，或者中断中调用无阻塞（Timeout设为0）的线程通信函数，通知自己的线程去处理
*/
extern QlOSStatus ql_rtos_timer_create
(
	ql_timer_t	   * timerRef,					/* OS supplied timer reference	*/
	ql_task_t	   taskRef,
	void		   (*callBackRoutine)(void *),	   /* timer call-back routine						   */
	void		   *timerArgc				   /* argument to be passed to call-back on expiration */
);

extern QlOSStatus ql_rtos_timer_start
(
	ql_timer_t		timerRef,					/* OS supplied timer reference						*/
	uint32			set_Time,					/* timer set value									*/
	unsigned char	cyclicalEn					/* wether to enable the cyclical mode or not		*/
);

extern QlOSStatus ql_rtos_timer_start_relaxed   		 /* for sleep mode, if timer finished but system is sleeping, timer will wakeup system after relax_time */
(
	ql_timer_t 		 timerRef,					 /* OS supplied timer reference 					 */
	uint32			 set_Time,				     /* timer set value									 */
	unsigned char	 cyclicalEn,  				 /* wether to enable the cyclical mode or not		 */
	uint32    		 relax_time					 /* the timer wakeup time, if not want timer to wake up system, input  QL_WAIT_FOREVER */
);

QlOSStatus ql_rtos_timer_start_us
(
	ql_timer_t 		 timerRef,					 /* OS supplied timer reference 					 */
	uint32			 set_Time_us				 /* timer set value									 */
);

extern QlOSStatus ql_rtos_timer_stop
(
	ql_timer_t timerRef 				/* OS supplied timer reference	*/
);

extern QlOSStatus ql_rtos_timer_is_running
(
	ql_timer_t timerRef					/* OS supplied timer reference	*/
);

extern QlOSStatus ql_rtos_timer_delete
(
	ql_timer_t timerRef 				/* OS supplied timer reference	*/
);

QlOSStatus ql_rtos_swdog_register
(
	ql_swdog_callback callback,			/* software watch dog callback  */
	ql_task_t taskRef					/* OS task reference  			*/
);

QlOSStatus ql_rtos_swdog_unregister
(
	ql_swdog_callback callback			/* software watch dog callback  */
);

QlOSStatus ql_rtos_feed_dog(void);

QlOSStatus ql_rtos_sw_dog_enable
(
	uint32 period_ms, 
	uint32 missed_cnt
);

QlOSStatus ql_rtos_sw_dog_disable(void);

QlOSStatus ql_gettimeofday
(
	ql_timeval_t *timeval
);

uint32 ql_rtos_get_system_tick(void);

void ql_assert(void);

/**
 * \brief monoclinic system time
 *
 * It is a relative time from system boot. Even after suspend and resume,
 * the monoclinic system time will be contiguous.
 * littlevgl need this time synchronization information 
 *
 * \return      monoclinic system time in milliseconds
 */
int64_t ql_rtos_up_time_ms();

/**
 * \brief get cpu utilization
 *
 * Starting from the first call of this function, the system will count 
 * the CPU utilization rate in the past 1s every 1s
 * 
 *
 * Attention: only this function be called once, then system can start count
 * the CPU utilization
 
 * \return  cpu utilization, 0~100; and <0 means system error
 */
int ql_rtos_get_cpu_using(void);

/**
 * \brief microsecond delay, range is 1~1000000, and will not release CPU unless interruped by higher priority task 
 *  
 */
QlOSStatus ql_delay_us(uint32_t us);

/**
 * suspend thread scheduler
 *
 * After scheduler is suspended, there are no thread context switch.
 * However, interrupt handlers will be executed.
 *
 */
extern void ql_rtos_scheduler_suspend(void);

/**
 * resume thread scheduler
 *
 * Scheduler suspend and resume is not *recursive*. That is, after
 * \p ql_rtos_scheduler_resume is called, scheduler is resumed no matter
 * how many times \p ql_rtos_scheduler_suspend are called.
 */
extern void ql_rtos_scheduler_resume(void);

/**
 * create a thread with specified stack
 *
 * It is similar to \p ql_rtos_task_create, and the stack won't be dynamic created.
 * Rather, the specified buffer \p stack will be used at the stack of the
 * thread. 
 *
 */
extern QlOSStatus ql_rtos_task_create_with_stack
(
	ql_task_t	 * taskRef, 			 /* OS task reference										   				  */
	void         * stack,                /* stack area                                                                */
	uint32		   stackSize,			 /* number of bytes in task stack area						   				  */
	uint8 		   priority,			 /* task priority, users should use the defination in APP_ThreadPriority_e, 
											and not recommand beyond APP_PRIORITY_ABOVE_NORMAL 						  */
	char		 * taskName,			 /* task name, no more than 16 bytes, exceed 16 bytes will only save 16 bytes */
	void	   ( * taskStart)(void*),	 /* pointer to task entry point 			 				   				  */
	void*		   argv,				 /* task entry argument pointer 							   				  */
	uint32         event_count,          /* the max event count that the task can waiting to process   				  */ 
    ...
);

#ifdef __cplusplus
} /*"C" */
#endif

#endif
