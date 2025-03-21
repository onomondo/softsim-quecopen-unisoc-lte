/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _FUPDATE_SECURITY_H_
#define _FUPDATE_SECURITY_H_
#include "fupdate.h"
#include "vfs.h"
#include "stdio.h"

/**
 * \brief check the availble space whether enough for update the fota pack 
 *
 * It returns the check result. It should be called after the fupdateCheckSignedPack() pass.
 *
 * \return
 *      - true if the availble space enough.
 *      - false if not.
 */
bool fupdateIsTmpSpaceEnough(void);
#endif
