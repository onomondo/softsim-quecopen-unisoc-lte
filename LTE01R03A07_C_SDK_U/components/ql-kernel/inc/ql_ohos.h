
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


#ifndef _QL_OHOS_H_
#define _QL_OHOS_H_


#include "ql_api_common.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	QL_OHOS_SUCCESS             =     0,
	QL_OHOS_ERR_FAILURE         =     -1,
	QL_OHOS_ERR_NO_MEMORY       =     -2,
	QL_OHOS_ERR_LOG_ERR         =     -3,
	QL_OHOS_ERR_LOG_OUT_CNT     =     -4,
	QL_OHOS_ERR_LOG_REGED       =     -5,
	QL_OHOS_ERR_LOG_NOT_FOUND   =     -6,
	QL_OHOS_ERR_LOG_BUSY        =     -7,
}ql_ohos_ecode_e;

typedef int (*ql_ohos_log_output_cb)(const char* data, int size);

void ql_ohos_task_init(char version);

int ql_ohos_log_output_register(ql_ohos_log_output_cb output_func);
int ql_ohos_log_output_unregister(ql_ohos_log_output_cb output_func);

void ql_ohos_xts_start(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_H_ */


