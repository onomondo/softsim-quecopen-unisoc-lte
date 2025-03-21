
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

#ifndef _QL_OHOS_HAL_I2C_H_
#define _QL_OHOS_HAL_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "iot_errno.h"
#include "iot_i2c.h"


    typedef struct ql_ohos_i2c_s
    {
        unsigned int (*i2c_init)(unsigned int id, unsigned int baudrate);
        unsigned int (*i2c_deinit)(unsigned int id);
        unsigned int (*i2c_write
        )(unsigned int         id,
          unsigned short       deviceAddr,
          const unsigned char *data,
          unsigned int         dataLen);
        unsigned int (*i2c_read
        )(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen);
        unsigned int (*i2c_set_baudrate)(unsigned int id, unsigned int baudrate);
    } ql_ohos_i2c_t;

    int ql_ohos_i2c_api_register(ql_ohos_i2c_t *i2c);

    int ql_ohos_i2c_api_unregister(void);

    int ql_ohos_i2c_api_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _QL_OHOS_HAL_I2C_H_ */