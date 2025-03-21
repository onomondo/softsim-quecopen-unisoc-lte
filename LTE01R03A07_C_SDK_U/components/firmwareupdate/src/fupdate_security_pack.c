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
#include <stdlib.h>
#include <string.h>
#include "fupdate_security.h"
#include "osi_log.h"

bool fupdateIsTmpSpaceEnough(void)
{
    int space_size = fupdateGetRequiredSpace();
    if (space_size < 0)
    {
        OSI_LOGE(0x1000a06f, "Fota error: avaliable space check failed %d", space_size);
        return false;
    }
    OSI_LOGI(0x1000a070, "fota pack need avalible space size %d", space_size);

    struct statvfs fsStat;
    int result = 0;
    result = vfs_statvfs(gFupdateTempFileName, &fsStat);
    if (result != 0)
    {
        OSI_LOGE(0x1000a071, "get fota tmp fs stat failed failed %d", result);
        return false;
    }

    if (space_size > fsStat.f_bavail * fsStat.f_bsize)
    {
        OSI_LOGE(0x1000a072, "avaliable size is not enough %d", fsStat.f_bavail * fsStat.f_bsize);
        return false;
    }
    return true;
}
