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

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief set gpio as input mode
 *
 * \param gpio gpio id
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halGpioSetInput(unsigned gpio);

/**
 * \brief get gpio value
 *
 * \param gpio gpio id
 * \return
 *      - true on high level
 *      - false on low level, or invalid parameter
 */
bool halGpioGetValue(unsigned gpio);

/**
 * \brief set gpio as output
 *
 * \param gpio gpio id
 * \param hi true for high level
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halGpioSetOutput(unsigned gpio, bool hi);

/**
 * \brief set gpo output level
 *
 * \param gpo gpo id
 * \param hi true for high level
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halGpoSetOutput(unsigned gpo, bool hi);

/**
 * \brief set pmic gpio as input mode
 *
 * \param gpio pmic gpio id
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halPmicGpioSetInput(unsigned gpio);

/**
 * \brief get pmic gpio value
 *
 * \param gpio pmic gpio id
 * \return
 *      - true on high level
 *      - false on low level, or invalid parameter
 */
bool halPmicGpioGetValue(unsigned gpio);

/**
 * \brief set pmic gpio as output
 *
 * \param gpio pmic gpio id
 * \param hi true for high level
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halPmicGpioSetOutput(unsigned gpio, bool hi);

OSI_EXTERN_C_END
#endif
