/**  @file
  quec_at_engine.c

  @brief
  This file is used to define at command engine functions for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

/*===========================================================================
* include files
===========================================================================*/
#include "quec_at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "ql_api_dev.h"
#include "ql_at_cmd_table_std.h"
#include "ql_log.h"

/*===========================================================================
* Macro Define
===========================================================================*/
#define ql_at_search_log(msg, ...)  QL_LOG(QL_LOG_LEVEL_INFO, "QATSC", msg, ##__VA_ARGS__)


/*===========================================================================
* Variable
===========================================================================*/
static const ql_at_desc_t *ql_sAppAtCmdTable = NULL;
static size_t ql_sListLength = 0;

#ifdef CONFIG_QUEC_BUILD_IN_CSDK
extern const ql_at_desc_t *ql_atcmd_search_desc(const char *name, size_t length);
#endif
/*===========================================================================
* Function
===========================================================================*/
void quec_app_at_add(const ql_at_desc_t *desc, size_t list_length)
{
	ql_at_search_log("add desc:%#x l:%d", desc, list_length);
	ql_sAppAtCmdTable = desc;
	ql_sListLength = list_length;
}

const ql_at_desc_t *quec_app_atcmd_search_desc(const char *name, size_t length)
{
	if (!ql_sAppAtCmdTable)
	{
		return NULL;
	}

	for (size_t i = 0; i < ql_sListLength; i++)
	{
		if (ql_sAppAtCmdTable[i].name)
		{
			if (0 == strncasecmp(name, ql_sAppAtCmdTable[i].name, length))
			{
				return (ql_at_desc_t *)&ql_sAppAtCmdTable[i];
			}
		}
	}

	return NULL;
}

ql_at_desc_t *quec_at_search_desc(const char *name, size_t length)
{
	ql_at_desc_t * desc = NULL;

	if (length == 0)
		return NULL;
 
	if (length <= MAX_WORD_LENGTH && length >= MIN_WORD_LENGTH)
	{
		unsigned key = ql_atCmdNameHash(name, length);
		if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
		{
			unsigned idx = ql_gAtCmdNameHashIndex[key];
			if (idx < TOTAL_KEYWORDS)
			{
				if(0 == strncasecmp(name, ql_gAtCmdTable[idx].name, length))
				{
					return(ql_at_desc_t *)&ql_gAtCmdTable[idx];
				}
			}
		}
	}

/* for app image */
#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) || defined(CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD))
	if (!desc)
	{
		desc = (ql_at_desc_t *)quec_app_atcmd_search_desc(name, length);
	}
#endif

	//quec_at_log("desc null");
	return desc;
}

uint quec_at_get_count(void)
{
    return TOTAL_KEYWORDS;
}

const ql_at_desc_t *quec_at_get_table(size_t n)
{
    if (n >= TOTAL_KEYWORDS)
        return NULL;
    return &ql_gAtCmdTable[n];
}

