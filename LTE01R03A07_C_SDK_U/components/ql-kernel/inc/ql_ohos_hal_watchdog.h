
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
20230428         Jamie.li    Init version
=================================================================*/

#ifndef _QL_OHOS_HAL_WATCHDOG_H_
#define _QL_OHOS_HAL_WATCHDOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "iot_errno.h"
#include "iot_watchdog.h"

    typedef struct ql_ohos_watchdog_s
    {
        void (*watchdog_enable)(void);
        void (*watchdog_disable)(void);
        void (*watchdog_kick)(void);
    } ql_ohos_watchdog_t;

    int ql_ohos_watchdog_api_register(ql_ohos_watchdog_t *watchdog);

    int ql_ohos_watchdog_api_unregister(void);

    int ql_ohos_watchdog_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_WATCHDOG_H_ */