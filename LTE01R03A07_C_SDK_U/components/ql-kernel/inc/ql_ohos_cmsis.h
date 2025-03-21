
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------
20230328         Jamie.li    Init version
=================================================================*/

#ifndef _QL_OHOS_CMSIS_H_
#define _QL_OHOS_CMSIS_H_

#include "target_config.h"
#include "cmsis_os2.h"
#include "los_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ql_rtos interface adaptation completion */
#ifndef QL_OHOS_CMSIS_COMPLETION
#define QL_OHOS_CMSIS_COMPLETION 1
#endif

#if QL_OHOS_CMSIS_COMPLETION
// [QUECTEL][CMSIS][TODO] 这部分待迁移至ql_rtos接口实现的文件以实现更多ohos接口。
// #define QL_OSI_TICK_RATE_HZ configTICK_RATE_HZ
// #define QL_OSI_MINIMAL_STACK_SIZE configMINIMAL_STACK_SIZE
#define QL_OSI_TICK_RATE_HZ (50U)
#define QL_OSI_MINIMAL_STACK_SIZE ((uint16_t)(1024))
typedef enum
{
	QL_OSI_KERNEL_INACTIVE = 0,
    QL_OSI_KERNEL_READY,
    QL_OSI_KERNEL_RUNNING, 
    QL_OSI_KERNEL_LOCKED, 
    QL_OSI_KERNEL_SUSPENDED, 
    QL_OSI_KERNEL_ERROR, 
    QL_OSI_KERNEL_RESERVED, 
}ql_osi_kernel_state_e;
#endif

#define QL_OHOS_CMSIS_TICK_RATE_HZ              QL_OSI_TICK_RATE_HZ
#define QL_OHOS_CMSIS_MINIMAL_STACK_SIZE        QL_OSI_MINIMAL_STACK_SIZE

#define QL_OHOS_CMSIS_THREAD_SUSPEND_RESUME     1
#define QL_OHOS_CMSIS_THREAD_ENUMERATE          1
#define QL_OHOS_CMSIS_THREAD_FLAGS              0
#define QL_OHOS_CMSIS_EVENT_GROUP               1
#define QL_OHOS_CMSIS_TIMER                     LOSCFG_BASE_CORE_SWTMR
#define QL_OHOS_CMSIS_MUTEX                     LOSCFG_BASE_IPC_MUX
#define QL_OHOS_CMSIS_SEM                       LOSCFG_BASE_IPC_SEM
#define QL_OHOS_CMSIS_QUEUE                     LOSCFG_BASE_IPC_QUEUE

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_CMSIS_H_ */

