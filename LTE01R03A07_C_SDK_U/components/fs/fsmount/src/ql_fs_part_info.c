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

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern unsigned gPartInfo[];

bool quec_fs_part_info_init(unsigned **PartInfo_ptr)
{
	if(PartInfo_ptr == NULL)
	{
		return false;
	}
	
	*PartInfo_ptr = gPartInfo;
	return true;	
}