#ifndef __PLATFORM_OS_H__
#define __PLATFORM_OS_H__

#include "ql_wifi_adapter.h"

#include "platform_sem.h"
#include "platform_thread.h"
//#include "platform_timer.h"
//#include "platform_mutex.h"
//#include "platform_list.h"
#include "platform_memory.h"
#include "platform_queue.h"
#include "stdio.h"

//#define QUEC_PLATFORM_LOG_DEBUG

#ifdef QUEC_PLATFORM_LOG_DEBUG
#define platform_log_e    QL_WIFI_ADAPTER_LOG
#define platform_log_i    QL_WIFI_ADAPTER_LOG
#define platform_log_d    QL_WIFI_ADAPTER_LOG
#else
#define platform_log_e(msg,...)
#define platform_log_i(msg,...)
#define platform_log_d(msg,...)
#endif

#define PLATFORM_WAIT_FOREVER AOS_WAIT_FOREVER
#define platform_msleep(x) aos_msleep(x)
#define BIT16 (1<<16)
#define BIT15 (1<<15)

unsigned int platform_irq_save(void);
void platform_irq_restore(unsigned int flag);

#endif

