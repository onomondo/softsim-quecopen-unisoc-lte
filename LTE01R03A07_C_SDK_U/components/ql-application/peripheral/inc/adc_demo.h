/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef _ADCDEMO_H
#define _ADCDEMO_H

#include "ql_app_feature_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Enumeration Definition
 ===========================================================================*/

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef int    QlADCStatus;
typedef void * ql_task_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void ql_adc_app_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _ADCDEMO_H */


