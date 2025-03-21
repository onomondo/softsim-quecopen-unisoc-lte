
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

#ifndef _QL_OHOS_HAL_PWM_H_
#define _QL_OHOS_HAL_PWM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "iot_errno.h"
#include "iot_pwm.h"

    typedef struct ql_ohos_pwm_s
    {
        unsigned int (*pwm_init)(unsigned int port);
        unsigned int (*pwm_deinit)(unsigned int port);
        unsigned int (*pwm_start)(unsigned int port, unsigned short duty, unsigned int freq);
        unsigned int (*pwm_stop)(unsigned int port);
    } ql_ohos_pwm_t;

    int ql_ohos_pwm_api_register(ql_ohos_pwm_t *pwm);

    int ql_ohos_pwm_api_unregister(void);

    int ql_ohos_pwm_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_PWM_H_ */