/**  @file
  quec_cust_patch.h

  @brief
  This file is used to define macro of patch for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2018 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
11/30/2018        Jensen     Init version
=================================================================*/

/*=============================================================================================================================

                        Patch List

This section contains comments describing patch list to the module.
Notice that changes are listed in reverse order.

NO.  PATCH NO.      RELEASE TIME  MERGE TIME   WHO        LIB   CODE MACRO                             WHAT, WHERE, WHY
---  ----------     ------------  -----------  ---------  ---   -----------     		---------------------------------------
 	
=============================================================================================================================*/


#ifndef QUEC_CUST_PATCH_H
#define QUEC_CUST_PATCH_H

#include "QuecPrjName.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************** platform/bsp/rtos patch**************************/
#if defined(__QUEC_OEM_VER_HZR__) || defined (__QUEC_OEM_VER_TY__)
#define QUEC_PATCH_FACTORY_UPDATE    1  
#endif
//add wifiscan SPCSS00750018
#define QUEC_PATCH_WIFISCAN_RSSI    1  

#define QUEC_PATCH_SPI_DMA_IRQ      1  

#define QUEC_PATCH_SDMMC_WRITE_READ_BIG_FILE    1
#define QUEC_PATCH_SDMMC_IFC_DCACHE    1

//SPCSS00934930,added by ryan.yi
#define QUEC_PATCH_FLASH_SUSPEND    1

//SPCSS01039199
#define QUEC_PATCH_BOOT_CODE_CUT    1
/**************************** AT COMMAND patch*********************************/



/******************************** UART patch*********************************/



/******************************** USB patch*********************************/
#define QUEC_PATCH_USB_MTP

/******************************** PPP patch*********************************/



/***************************** POWER_MANAGER patch******************************/



/******************************** NETWORK patch*********************************/



/******************** SMS/PHB/CALL/STK/BIP/USSD/(U)SIM patch********************/


/******************************** TCPIP patch*********************************/


/******************************** FS patch*********************************/


/******************************** AUDIO PATCH *****************************/



#ifdef __cplusplus
} /*"C" */ 
#endif

#endif

