
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

#ifndef _QL_OHOS_HAL_FLASH_H_
#define _QL_OHOS_HAL_FLASH_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "iot_errno.h"
#include "iot_flash.h"

    typedef struct ql_ohos_flash_s
    {
        unsigned int (*flash_init)(void);
        unsigned int (*flash_deinit)(void);
        unsigned int (*flash_write
        )(unsigned int         flashOffset,
          unsigned int         size,
          const unsigned char *ramData,
          unsigned char        doErase);
        unsigned int (*flash_read
        )(unsigned int flashOffset, unsigned int size, unsigned char *ramData);
        unsigned int (*flash_erase)(unsigned int flashOffset, unsigned int size);
    } ql_ohos_flash_t;

    int ql_ohos_flash_api_register(ql_ohos_flash_t *flash);

    int ql_ohos_flash_api_unregister(void);

    int ql_ohos_flash_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_FLASH_H_ */