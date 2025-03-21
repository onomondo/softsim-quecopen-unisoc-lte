/**  @file
  ql_cnktd_uart.h

  @brief
  This file is used to define cnktd uart for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_CNKTD_UART_H
#define QL_CNKTD_UART_H

#include "ql_cnktd.h"
#include "ql_uart.h"


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/** @addtogroup qapi_uart
@{ */

/**
 * UART handle that is passed to the client when a UART port is opened.
 */
typedef void* qapi_UART_Handle_t;

/*used to set main uart AT function mode.*/
typedef enum
{
	QT_MAIN_UART_DISABLE,
	QT_MAIN_UART_ENABLE,
	QT_MAIN_UART_MAX
}qapi_QT_Main_UART_Mode_e;

/**
 * UART port ID enumeration. 
 *  
 * This enumeration is used to specify which port is to be opened during the uart_open 
 * call.
 *  
 */
typedef enum
{
    QAPI_UART_PORT_001_E = 0, /**< UART core 01. */
    QAPI_UART_PORT_002_E,     /**< UART core 02. */
    QAPI_UART_PORT_003_E,     /**< UART core 03. */
    QAPI_UART_PORT_004_E,     /**< UART core 04. */
    QAPI_UART_PORT_005_E,     /**< UART core 05. */
    QAPI_UART_PORT_006_E,     /**< UART core 06. */
    QAPI_UART_PORT_007_E,     /**< UART core 07. */
    QAPI_UART_PORT_008_E,     /**< UART core 08. */
    QAPI_UART_PORT_009_E,     /**< UART core 09. */
    QAPI_UART_PORT_010_E,     /**< UART core 10. */
    QAPI_UART_PORT_011_E,     /**< UART core 11. */
    QAPI_UART_PORT_012_E,     /**< UART core 12. */
    QAPI_UART_PORT_013_E,     /**< UART core 13. */	
    QAPI_UART_PORT_014_E,     /**< UART core 14. */	
    QAPI_UART_PORT_015_E,     /**< UART core 15. */	
    QAPI_UART_PORT_016_E,     /**< UART core 16. */	
    QAPI_UART_PORT_017_E,     /**< UART core 17. */	
    QAPI_UART_PORT_018_E,     /**< UART core 18. */	
    QAPI_UART_PORT_019_E,     /**< UART core 19. */	
    QAPI_UART_PORT_020_E,     /**< UART core 20. */	
    QAPI_UART_PORT_021_E,     /**< UART core 21. */	
    QAPI_UART_PORT_022_E,     /**< UART core 22. */	
    QAPI_UART_PORT_023_E,     /**< UART core 23. */	
    QAPI_UART_PORT_024_E,     /**< UART core 24. */	

	QAPI_UART_MAX_PORTS_E,
}qapi_UART_Port_Id_e;

/**
 * Enumeration for UART parity mode configuration.  
 *   
 */
typedef enum
{
  QAPI_UART_NO_PARITY_E        = 0,  /**< No parity. */ 
  QAPI_UART_ODD_PARITY_E       = 1,  /**< Odd parity. */
  QAPI_UART_EVEN_PARITY_E     = 2,   /**< Even parity. */
  QAPI_UART_SPACE_PARITY_E     = 3,  /**< Space parity. */
} qapi_UART_Parity_Mode_e;

/**
 * Enumeration for UART number of stop bits configuration.  
 *   
 */
typedef enum
{
  QAPI_UART_0_5_STOP_BITS_E    = 0,  /**< 0.5 stop bits. */
  QAPI_UART_1_0_STOP_BITS_E    = 1,  /**< 1.0 stop bit. */
  QAPI_UART_1_5_STOP_BITS_E    = 2,  /**< 1.5 stop bits. */
  QAPI_UART_2_0_STOP_BITS_E    = 3,  /**< 2.0 stop bits. */
} qapi_UART_Num_Stop_Bits_e;

/**
 * UART bits per character configuration enumeration.
 *  
 * Enumeration to specify how many UART bits are to be used per character configuration.
 *  
 */
typedef enum
{
  QAPI_UART_5_BITS_PER_CHAR_E  = 0,  /**< 5 bits per character. */
  QAPI_UART_6_BITS_PER_CHAR_E  = 1,  /**< 6 bits per character. */
  QAPI_UART_7_BITS_PER_CHAR_E  = 2,  /**< 7 bits per character. */
  QAPI_UART_8_BITS_PER_CHAR_E  = 3,  /**< 8 bits per character. */
} qapi_UART_Bits_Per_Char_e;

/**
 * Transmit and receive operation callback type.
 *    
 * @param[in] num_bytes  Number of bytes.
 * @param[out] cb_data   Pointer to the callback data.
 */
typedef void(*qapi_UART_Callback_Fn_t)(uint32_t num_bytes, void *cb_data);


/** Structure for UART configuration. */
typedef struct
{
   uint32_t                     baud_Rate; /**< Supported baud rates are 115200 bps, 1 Mbps,
                                                       2 Mbps, 3 Mbps, and 4 Mbps. */
   qapi_UART_Parity_Mode_e      parity_Mode; /**< Parity mode. */
   qapi_UART_Num_Stop_Bits_e    num_Stop_Bits; /**< Number of stop bits. */
   qapi_UART_Bits_Per_Char_e    bits_Per_Char; /**< Bits per character. */
   qbool_t                      enable_Loopback; /**< Enable loopback. */
   qbool_t                      enable_Flow_Ctrl; /**< Enable flow control. */
   boolean                      user_mode_client; /**< reserve for Qapi UART Driver. */
   qapi_UART_Callback_Fn_t      tx_CB_ISR;       /**< Transmit callback, called from ISR context. \n
                                                       Be sure not to violate ISR guidelines. \n
                                                       @latexonly \textbf{Note:} @endlatexonly
                                                       Do not call uart_transmit or uart_receive
                                                       APIs from this callback. */
   qapi_UART_Callback_Fn_t      rx_CB_ISR;      /**< Receive callback, called from ISR context. \n
                                                       Be sure not to violate ISR guidelines. \n
                                                       @latexonly \textbf{Note:} @endlatexonly
                                                       Do not call uart_transmit or uart_receive
                                                       APIs from this callback. */
}qapi_UART_Open_Config_t;

/*========================================================================
 *  function Definition
 *========================================================================*/

/**
 * Queues the buffer provided for receiving the data.
 *
 * This is an asynchronous call. rx_cb_isr is called when the Rx transfer completes.
 * The buffer is owned by the UART driver until rx_cb_isr is called.
 * 
 * There must always be a pending Rx. The UART hardware has a limited buffer
 * (FIFO), and if there is no software buffer available for HS-UART,
 * the flow control will de-assert the RFR line.
 * 
 * Call uart_receive immediately after uart_open to queue a buffer.
 * After every rx_cb_isr, from a different non-ISR thread, queue the next transfer.
 * 
 * There can be a maximum of two buffers queued at a time.
 * 
 * @note1hang Do not call this API from ISR context.
 * 
 * @param[in] handle       UART handle provided by qapi_UART_Open().
 * @param[in] buf          Buffer to be filled with data.
 * @param[in] buf_Size     Buffer size. Must be @ge 4 and a multiple of 4. 
 * @param[in] cb_Data      Callback data to be passed when rx_cb_isr is called 
 *                         during Rx completion.
 *  
 * @return 
 * - QAPI_OK     -- Queuing of the receive buffer was successful.
 * - QAPI_ERROR  -- Queuing of the receive buffer failed.
 *
 *  @dependencies
 *  - Before calling this API qapi_UART_Open() and qapi_UART_Power_On() APIs must be called.
 *  
 */

qapi_Status_t qapi_UART_Receive
(
   qapi_UART_Handle_t   handle,
   char*                buf, 
   uint32_t             buf_Size, 
   void*                cb_Data
);

/*
@func
  qapi_QT_Main_UART_Enable_Set
@brief
  To change currently main uart AT function mode.It is vadlid in next restart
@param [in]
  mode enable or disable main uart AT function to change in general uart
       1  close main uart AT function
       0  open main uart AT function

*/
qapi_Status_t qapi_QT_Main_UART_Enable_Set(qapi_QT_Main_UART_Mode_e mode);

/**
 * Initializes the UART port.
 *
 * Opens the UART port and configures the corresponding clocks, interrupts, and GPIO.
 * 
 * @note1hang Do not call this API from ISR context.
 * 
 * @param[in] handle  UART handle. 
 * @param[in] id      ID of the port to be opened.
 * @param[in] config  Structure that holds all configuration data.
 *  
 * @return 
 * - QAPI_OK     -- Port open was successful.
 * - QAPI_ERROR  -- Port open failed.
 *  
 *  
 */

qapi_Status_t qapi_UART_Open
(
   qapi_UART_Handle_t*        handle, 
   qapi_UART_Port_Id_e        id, 
   qapi_UART_Open_Config_t*   config
);

/**
*  Enables the UART hardware resources for a UART transaction.
*
*  This function enables all resources required for a successful UART transaction. This 
*  includes clocks, power resources, and pin multiplex functions. This function should 
*  be called before a transfer or a batch of UART transfers.
* 
*  @param[in] UART_Handle  Driver handle returned by qapi_UART_Open().
*
*  @return
*  - QAPI_OK     -- UART powered on successfully.
*  - QAPI_ERROR  -- UART power on failed. 
*
*  @dependencies
*  - Before calling this API qapi_UART_Open() API must be called.
*/
qapi_Status_t
qapi_UART_Power_On
(
    qapi_UART_Handle_t UART_Handle
);

/*==================================================================================
  FUNCTION      qapi_UART_Power_Off
==================================================================================*/
/**
*  Disables the UART hardware resources for a UART transaction.
*
*  This function turns off all resources used by the UART master. This includes clocks, 
*  power resources, and GPIOs. This function should be called to put the UART master
*  in its lowest possible power state.
* 
*  @param[in] UART_Handle  Driver handle returned by qapi_UART_Open().
*
*  @return
*  - QAPI_OK     -- UART powered off successfully.
*  - QAPI_ERROR  -- UART power off failed.
*
*  @dependencies
*  - Before calling this API qapi_UART_Open() and qapi_UART_Power_On() APIs must be called.
*/
qapi_Status_t 
qapi_UART_Power_Off 
(
    qapi_UART_Handle_t UART_Handle
);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_UART_H */


