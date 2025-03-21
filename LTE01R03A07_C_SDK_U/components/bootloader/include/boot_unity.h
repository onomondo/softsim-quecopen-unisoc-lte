/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _BOOT_UNITY_H_
#define _BOOT_UNITY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_PROVIDE_BOOT_START(fn) \
    extern void fn(uint32_t);       \
    void bootStart(uint32_t param) { fn(param); }

/**
 * \brief default bootloader unittest entry
 *
 * \param param parameter passed by ROM
 */
void bootUnityStart(uint32_t param);

#ifdef __cplusplus
}
#endif
#endif
