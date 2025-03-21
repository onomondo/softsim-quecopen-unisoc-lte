/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _QUEC_BOOT_LOG_H_
#define _QUEC_BOOT_LOG_H_

#include "osi_compiler.h"
#include <stdio.h>
#include "quec_proj_config.h"

//剪裁boot中QUEC_BOOT_LOG打印,只添加打印函数地址,不添加函数名和行号
//如果调试需要打印函数名和行号可以将此宏设为0
#define QUEC_BOOT_LOG_CUT_  1

#ifndef _RET_IP_
//0表示当前函数的返回地址，1表示当前函数的调用者的返回地址
#define _RET_IP_         (unsigned long)__builtin_return_address(0)
#endif

#ifndef _THIS_IP_
//获取当前函数的地址
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
#endif

#define QUEC_BOOT_LEVEL OSI_LOG_LEVEL_INFO
#define QUEC_BOOT_LOG_TAG OSI_MAKE_LOG_TAG('Q', 'B', 'O', 'T')
#define QUEC_BOOT_PRINTF(level, fmt, ...)                                        \
    do                                                                          \
    {                                                                           \
        if (QUEC_BOOT_LEVEL >= level)                                            \
            osiTracePrintf((level << 28) | (QUEC_BOOT_LOG_TAG), fmt, ##__VA_ARGS__); \
    } while (0)

//CSDK compilation will generate CONFIG_QL_CCSDK_BUILD_ON,otherwise only CONFIG_QL_CCSDK_BUILD_ will be generated
#ifdef CONFIG_QL_CCSDK_BUILD_ON
//csdk build use this
//打印中添加函数名和行号
#define custom_boot_log(user_str, msg, ...)		QUEC_BOOT_PRINTF(QUEC_BOOT_LEVEL, "%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
//local or kernel build use this
#if QUEC_BOOT_LOG_CUT_
//打印中只添加函数地址
//#define custom_boot_log(user_str, msg, ...)		QUEC_BOOT_PRINTF(QUEC_BOOT_LEVEL, "%p "msg"", _THIS_IP_, ##__VA_ARGS__)
//打印中添加函数地址和行号
#define custom_boot_log(user_str, msg, ...)		QUEC_BOOT_PRINTF(QUEC_BOOT_LEVEL, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#else
//打印中添加函数名和行号
#define custom_boot_log(user_str, msg, ...)		QUEC_BOOT_PRINTF(QUEC_BOOT_LEVEL, "%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif/* QUEC_BOOT_LOG_CUT_ */
#endif/* CONFIG_QL_CCSDK_BUILD */

#define QUEC_BOOT_LOG(msg, ...)					custom_boot_log("Qboot", msg, ##__VA_ARGS__)

#endif /* _QUEC_BOOT_LOG_H_ */
