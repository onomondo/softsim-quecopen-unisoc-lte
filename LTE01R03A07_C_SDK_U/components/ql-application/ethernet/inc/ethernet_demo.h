
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#ifndef ETHERNET_DEMO_H
#define ETHERNET_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  function Definition
 *========================================================================*/
void ethernet_phy_set_cs(void *ctx);

void ql_ethernet_demo_init(void);

#ifdef __cplusplus
}/*"C" */
#endif

#endif   /*USBNET_DEMO_H*/