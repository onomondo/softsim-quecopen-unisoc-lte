/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef _POWER_H
#define _POWER_H

#include "ql_power.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QUEC_RESUME_SRC_PMIC 			(1 << 0)
#define QUEC_RESUME_SRC_VAD 			(1 << 1)
#define QUEC_RESUME_SRC_KEY 			(1 << 2)
#define QUEC_RESUME_SRC_GPIO1 			(1 << 3)
#define QUEC_RESUME_SRC_UART1 			(1 << 4 | 1<<5)  //UART1 and UART1_RXD
#define QUEC_RESUME_SRC_WCN2SYS 		(1 << 6)
#define QUEC_RESUME_SRC_WCN_OSC			(1 << 7)
#define QUEC_RESUME_SRC_IDLE_TIMER1		(1 << 8)
#define QUEC_RESUME_SRC_IDLE_TIMER2 	(1 << 9)
#define QUEC_RESUME_SRC_SELF		    (1 << 10)
#define QUEC_RESUME_SRC_USB_MON 		(1 << 11)
/*===========================================================================
 * Struct
 ===========================================================================*/

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void ql_power_app_init(void);

void ql_pwrkey_app_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _POWER_H */


