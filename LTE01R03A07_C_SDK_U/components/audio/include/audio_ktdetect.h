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

#ifndef _AUDIO_KTDETECT_H_
#define _AUDIO_KTDETECT_H_

#include "osi_api.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief opaque data structure of keytone detecter
 */
typedef struct auKeytoneDetecter auKeytoneDetecter_t;

/**
 * \brief function type, keytone detect result report callback
 */
typedef void (*keytonedetectHook_t)(char keytone);

/**
 * \brief check whether keytone detect is enabled
 *
 * \return
 *      - true if keytone detect is enabled
 *      - false if disabled
 */
bool auIsKTDetectEnable(void);

/**
 * \brief enable or disable keytone detect function
 *
 * \param en      true for enable keytone detect
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
void auKTDetectEnable(bool en);

/**
 * \brief register hook function for keytone detect report result
 *
 * \param func the callback function when keytone detect some result
 */
void auSetKtdetectCB(keytonedetectHook_t func);

void auKTDetectInit(void);

OSI_EXTERN_C_END
#endif
