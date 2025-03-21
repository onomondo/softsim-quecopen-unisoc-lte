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

#include "osi_api.h"

static uint32_t gBootCauses = 0;
static osiBootCause_t gBootMode = OSI_BOOTMODE_NORMAL;

void osiSetUpTime(int64_t ms) {}
int64_t osiUpTime(void) { return 0; }
int64_t osiEpochSecond(void) { return 0; }
int64_t osiEpochTime(void) { return 0; }
bool osiSetEpochTime(int64_t ms) { return false; }
int64_t osiUpTimeToEpoch(int64_t uptime) { return 0; }
uint32_t osiGetBootCauses(void) { return gBootCauses; }
void osiSetBootCause(osiBootCause_t cause) { gBootCauses |= cause; }
osiBootMode_t osiGetBootMode(void) { return gBootMode; }
void osiSetBootMode(osiBootMode_t mode) { gBootMode = mode; }
osiMutex_t *osiMutexCreate(void) { return (osiMutex_t *)1; }
void osiMutexDelete(osiMutex_t *mutex) {}
void osiMutexLock(osiMutex_t *mutex) {}
void osiMutexUnlock(osiMutex_t *mutex) {}
void osiThreadSleep(uint32_t ms) { osiDelayUS(ms * 1000); }
void osiThreadSleepUS(uint32_t us) { osiDelayUS(us); }
osiPmSource_t *osiPmSourceCreate(uint32_t tag, const osiPmSourceOps_t *ops, void *ctx) { return (osiPmSource_t *)1; }
