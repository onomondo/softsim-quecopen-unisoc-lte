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

#ifndef _AUD_RESAMPLE_H
#define _AUD_RESAMPLE_H

#include "osi_api.h"
//#include <string.h>
//
#ifdef __cplusplus
extern "C" {
#endif

typedef struct audResamCtx
{
    uint32_t playrate;     //output sample rate
    int16_t playrateStep;  //output step
    int16_t playScale;     //scale factor for calculate
    int16_t intpoint;      //input sample index
    int16_t floatpoint;    //resample float index
    int16_t dataratestep;  //input step
    int16_t blocklength;   //input sample number
    int16_t historyData_l; //history L sample
    int16_t historyData_r; //history R sample
    uint16_t chl;          //channel. 1 or 2. output same as input.
} audResamCtx_t;

int32_t audioResampleInitParam(audResamCtx_t *ptTransamObj, uint32_t inRate, uint32_t outRate, uint16_t chl);
uint32_t audioResample(audResamCtx_t *ptTransamObj, int16_t *inBuf, int16_t inum, int16_t *outBuf);

#ifdef __cplusplus
}
#endif

#endif
