/**  
  @file
  osi_demo.h

  @brief
  This file provides the definitions for nw demo, and declares the 
  API functions.

*/
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

#ifndef _MXML_DEMO_H_
#define _MXML_DEMO_H_


#ifdef __cplusplus
extern "C" {
#endif

#define XML_TEST_DISK    "UFS:"
#define XML_TEST_DIR	 "xml_dir"
#define XML_FILE_NAME	 "test_xml.xml"



#define XML_DIR_PATH     ""XML_TEST_DISK"/"XML_TEST_DIR""
#define XML_FILE_PATH	 ""XML_DIR_PATH"/"XML_FILE_NAME""


/*========================================================================
 *  function Definition
 *========================================================================*/

int ql_mxml_app_init(void);

#ifdef __cplusplus
}/*"C" */
#endif

#endif   
