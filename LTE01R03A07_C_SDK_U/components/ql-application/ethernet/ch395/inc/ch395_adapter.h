/**  @file
ethernet_phy_adapter.h

  @brief
  This file is used to define bt api for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
=================================================================*/
#ifndef _CH395_ADAPTER_H_
#define _CH395_ADAPTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"
#include <stdint.h>

#include "ql_api_spi.h"
#include "ql_api_osi.h"
#include "ql_api_ethernet.h"
#include "ethernet_demo.h"

#define mDelaymS(x);                ({ql_rtos_task_sleep_ms(x);})
#define mDelayuS(x);                ({ql_delay_us(x);})

#define xCH395CmdStart()           ({uint8_t __i = 0;ql_ethernet_phy_append((void*)ethernet_phy_set_cs,&__i);})
#define xEndCH395Cmd()              ({uint8_t __i = 1;ql_ethernet_phy_append((void*)ethernet_phy_set_cs,&__i);})

#define xWriteCH395Cmd(cmd)         ({uint8_t __i = (cmd);xEndCH395Cmd();xCH395CmdStart();ql_ethernet_phy_write(&__i,sizeof(__i));mDelayuS(2);})
#define xWriteCH395Data(data)       ({uint8_t __i = (data);ql_ethernet_phy_write(&__i,sizeof(__i));})
#define xReadCH395Data()            ({uint8_t __i = (0);ql_ethernet_phy_read(&__i,sizeof(__i));(uint8_t)__i;})

//Write data block for increaing the rate.
#define xWriteCH395DataBlock(data,len)  ({ql_ethernet_phy_write(data,len);})
#define xReadCH395DataBlock(data,len)  ({ql_ethernet_phy_read(data,len);})

#ifdef __cplusplus
}
#endif

#endif