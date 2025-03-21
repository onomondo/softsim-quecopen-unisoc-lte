/**
  @file
  ql_wifi_spi.h

  @brief
  Quectel wifi spi api header.

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

#ifndef QL_WIFI_SPI_H
#define QL_WIFI_SPI_H

#include "ql_osi_def.h"
#include "ql_api_osi.h"

#ifdef __cplusplus
extern "C" {
#endif
/*========================================================================
 *	Macro Definition
 *========================================================================*/

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_SPI_SUCCESS = 0,
    QL_WIFI_SPI_EXECUTE_ERR = 1 | (QL_COMPONENT_LWIP_WIFI << 16),
    QL_WIFI_SPI_INVALID_PARAM_ERR,
    QL_WIFI_SPI_TASK_CREATE_ERR,
} ql_wifi_spi_errcode_e;

/*========================================================================
*  Type Definition
*========================================================================*/

/*========================================================================
 *  function Definition
 *========================================================================*/

/*****************************************************************
 * Function: ql_wifi_spi_output_data_msg
 *
 * Description: Wi-Fi 802.3 data streams output function.Use this function to send data streams.
 *
 * Parameters:
 * ctx        [in]       See ql_wifi_data_info_s in ql_api_wifi.h
 * 
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
int ql_wifi_spi_output_data_msg(void *ctx);

/*****************************************************************
 * Function: ql_wifi_spi_init
 *
 * Description: Initialize Wi-Fi spi engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_spi_errcode_e ql_wifi_spi_init(void* argv);

/*****************************************************************
 * Function: ql_wifi_spi_deinit
 *
 * Description: Deinitialize Wi-Fi spi engine.
 *
 * Parameters:
 * argv        [in]        No defined yet.
 *
 * Return:
 *  0           success
 *  other       error code
 *
 *****************************************************************/
ql_wifi_spi_errcode_e ql_wifi_spi_deinit(void* argv);
#ifdef __cplusplus
} /*"C" */
#endif

#endif
