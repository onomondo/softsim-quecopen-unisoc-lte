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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_cnktd_uart.h"
#include "ql_log.h"


#define QL_CNKTD_UART_LOG_LEVEL                 QL_LOG_LEVEL_INFO
#define QL_CNKTD_UART_LOG(msg, ...)             QL_LOG(QL_CNKTD_UART_LOG_LEVEL, "ql_cnktd", msg, ##__VA_ARGS__)
#define QL_CNKTD_UART_LOG_PUSH(msg, ...)        QL_LOG_PUSH("ql_cnktd", msg, ##__VA_ARGS__)

#define QL_CNKTD_UART_RX_BUFF_SIZE                2048
#define QL_CNKTD_UART_TX_BUFF_SIZE                2048

typedef struct
{
    ql_uart_port_number_e port;
    qbool_t enable_Loopback;
    qapi_UART_Callback_Fn_t tx_CB_ISR;
    qapi_UART_Callback_Fn_t rx_CB_ISR;
    int *buff;
    int readed_len;
    int unread_len;
}ql_cnktd_uart_s;

ql_cnktd_uart_s *ql_cnktd_uart[QL_PORT_MAX] = {0};

/**
 * @param[in] handle       UART handle provided by qapi_UART_Open().
 * @param[in] buf          Buffer to be filled with data.                                         
 * @param[in] buf_Size     Buffer size. Must be @ge 4 and a multiple of 4. 
 * @param[in] cb_Data      Callback data to be passed when rx_cb_isr is called 
 *                         during Rx completion.
 *  
 * @return 
 * QAPI_OK      Queuing of the receive buffer was successful. \n
 * QAPI_ERROR   Queuing of the receive buffer failed.
 *  
 */

qapi_Status_t qapi_UART_Receive
(
   qapi_UART_Handle_t   handle,
   char*                buf, 
   uint32_t             buf_Size, 
   void*                cb_Data
)
{
    int read_len = 0;
    if(handle == NULL || buf == NULL || buf_Size == 0 || cb_Data == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_cnktd_uart_s *cnktd_uart = (ql_cnktd_uart_s *)handle;
    read_len = ql_uart_read(cnktd_uart->port, (unsigned char *)buf, buf_Size);
    QL_CNKTD_UART_LOG("uart read_len=%d", read_len);
    if (read_len != buf_Size)
    {
        cnktd_uart->readed_len = read_len;
        cnktd_uart->unread_len = buf_Size - read_len;
        cnktd_uart->buff = cb_Data;
        memcpy(cnktd_uart->buff, buf, read_len);
    }
    else 
    {
        cnktd_uart->readed_len = 0;
        cnktd_uart->unread_len = 0;
        cnktd_uart->buff = NULL;
    }
    return QAPI_OK;	
}

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
qapi_Status_t qapi_QT_Main_UART_Enable_Set(qapi_QT_Main_UART_Mode_e mode)
{	
    return QAPI_QT_ERR_OK;  
}

void ql_cnktd_uart_notify_cb(uint32 ind_type, ql_uart_port_number_e port, uint32 size)
{
    unsigned char *recv_buff = NULL;
    unsigned int real_size = 0;
    int read_len = 0;
    int cnktd_len = 0;

    ql_cnktd_uart_s *cnktd_uart = ql_cnktd_uart[port];
    if (cnktd_uart == NULL)
    {
        QL_CNKTD_UART_LOG("uart port err");
        return;
    }

    recv_buff  = calloc(1, QL_CNKTD_UART_RX_BUFF_SIZE+1);
    
    if (recv_buff == NULL)
    {
        QL_CNKTD_UART_LOG("no memery");
        return;
    }
    
    switch(ind_type)
    {
        case QUEC_UART_RX_OVERFLOW_IND:  //rx buffer overflow
        case QUEC_UART_RX_RECV_DATA_IND:
        {
            while(size > 0)
            {
                memset(recv_buff, 0, QL_CNKTD_UART_RX_BUFF_SIZE+1);
                real_size= CNKTD_MIN(size, QL_CNKTD_UART_RX_BUFF_SIZE);
                
                read_len = ql_uart_read(port, recv_buff, real_size);
                if((read_len > 0) && (size >= read_len))
                {
                    size -= read_len;
                }

                if (cnktd_uart->rx_CB_ISR)
                {
                    if (cnktd_uart->buff && cnktd_uart->unread_len > 0)
                    {
                        if (cnktd_uart->unread_len > read_len)
                        {
                            cnktd_len = read_len;
                        }
                        else 
                        {
                            cnktd_len = cnktd_uart->unread_len;
                            
                        }
                        QL_CNKTD_UART_LOG("cnktd uart %d %d %d", cnktd_uart->readed_len, cnktd_uart->unread_len, cnktd_len);
                        memcpy(cnktd_uart->buff+cnktd_uart->readed_len, recv_buff, cnktd_len);
                        cnktd_uart->unread_len -= cnktd_len;
                        cnktd_uart->readed_len += cnktd_len;
                        if (cnktd_uart->unread_len == 0)
                        {
                            cnktd_uart->rx_CB_ISR(cnktd_uart->readed_len, cnktd_uart->buff);
                        }
                    }
                    else 
                    {
                        cnktd_uart->rx_CB_ISR(read_len, recv_buff);
                    }
                    
                }

                if (cnktd_uart->enable_Loopback)
                {
                    ql_uart_write(port, recv_buff, read_len);
                }

                
                if (read_len == 0 || read_len > size)
                {
                    break;
                }
            }
            break;
        }
        case QUEC_UART_TX_FIFO_COMPLETE_IND: 
        {
            if (cnktd_uart->rx_CB_ISR)
            {
                cnktd_uart->tx_CB_ISR(0, recv_buff);
            }
            break;
        }
    }
    free(recv_buff);
    recv_buff = NULL;
}


/**
 * Initializes the UART port. Not to be called from ISR context.
 *
 * Opens the UART port and configures the corresponding clocks, interrupts, and GPIO.
 * 
 * @note1hang Do not call this API from ISR context.
 * 
 * @param[in] handle UART handle. 
 * @param[in] id ID of the port to be opened.
 * @param[in] config Structure that holds all configuration data.
 *  
 * @return 
 * QAPI_OK      Port open successful. \n
 * QAPI_ERROR   Port open failed.
 *  
 *  
 */
qapi_Status_t qapi_UART_Open
(
   qapi_UART_Handle_t*        handle, 
   qapi_UART_Port_Id_e        id, 
   qapi_UART_Open_Config_t*   config
)
{
    ql_cnktd_uart_s *cnktd_uart = NULL;
    ql_uart_config_s dcb = {0};
    
    if((id >= QAPI_UART_MAX_PORTS_E) || (handle == NULL) || (config == NULL))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if ((int)id >= (int)QL_PORT_MAX)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    dcb.baudrate = (ql_uart_baud_e)config->baud_Rate;
    if (config->parity_Mode == QAPI_UART_NO_PARITY_E)
    {
        dcb.parity_bit = QL_UART_PARITY_NONE;
    }
    else if (config->parity_Mode == QAPI_UART_ODD_PARITY_E)
    {
        dcb.parity_bit = QL_UART_PARITY_ODD;
    }
    else if (config->parity_Mode == QAPI_UART_EVEN_PARITY_E)
    {
        dcb.parity_bit = QL_UART_PARITY_EVEN;
    }
    else 
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (config->num_Stop_Bits == QAPI_UART_1_0_STOP_BITS_E)
    {
        dcb.parity_bit = QL_UART_STOP_1;
    }
    else if (config->num_Stop_Bits == QAPI_UART_2_0_STOP_BITS_E)
    {
        dcb.parity_bit = QL_UART_STOP_2;
    }
    else
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (config->bits_Per_Char == QAPI_UART_7_BITS_PER_CHAR_E)
    {
        dcb.parity_bit = QL_UART_DATABIT_7;
    }
    else if (config->bits_Per_Char == QAPI_UART_8_BITS_PER_CHAR_E)
    {
        dcb.parity_bit = QL_UART_DATABIT_8;
    }
    else
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (config->enable_Flow_Ctrl == true)
    {
        dcb.flow_ctrl = QL_FC_HW;
    }
    else
    {
        dcb.flow_ctrl = QL_FC_NONE;
    }
    
    cnktd_uart = (ql_cnktd_uart_s *)malloc(sizeof(ql_cnktd_uart_s));
    if (cnktd_uart == NULL)
    {
        return QAPI_ERR_NO_MEMORY;
    }

    memset(cnktd_uart, 0x00, sizeof(ql_cnktd_uart_s));
    cnktd_uart->port = (ql_uart_port_number_e)id;
    cnktd_uart->enable_Loopback = config->enable_Loopback;
    cnktd_uart->tx_CB_ISR = config->tx_CB_ISR;
    cnktd_uart->rx_CB_ISR = config->rx_CB_ISR;
    if (QL_UART_SUCCESS != ql_uart_get_dcbconfig(cnktd_uart->port , &dcb))
    {
        goto err_exit;
    }

    if (QL_UART_SUCCESS != ql_uart_open(cnktd_uart->port))
    {
        goto err_exit;
    }
    
    if (QL_UART_SUCCESS != ql_uart_register_cb(cnktd_uart->port, ql_cnktd_uart_notify_cb))
    {
        goto err_exit;
    }

    *handle = cnktd_uart;

    ql_cnktd_uart[id] = cnktd_uart;
    return QAPI_OK;
err_exit:
    if (cnktd_uart)
    {
        free(cnktd_uart);
    }
    return QAPI_ERROR;
}

/**
*  Enables the UART Hardware resources for an UART transaction.
*
*  This function enables all resources required for a successful UART transaction. This 
*  includes clocks, power resources and pin multiplex functions. This function should 
*  be called before a transfer or a batch of UART transfers.
* 
*  @param[in] UART_Handle  Driver handle returned by \c qapi_UART_Open.
*
*  @return
*  QAPI_OK      UART powered on successfully. \n
*  QAPI_ERROR   UART power on is failed. 
*/
qapi_Status_t
qapi_UART_Power_On
(
    qapi_UART_Handle_t handle
)
{
    if(handle == NULL)
    {
        return QAPI_ERROR;
    }
    return QAPI_OK;
}

/*==================================================================================
  FUNCTION      qapi_UART_Power_Off
==================================================================================*/
/**
*  Disables the UART Hardware resources for an UART transaction.
*
*  This function turns off all resources used by the UART master. This includes clocks, 
*  power resources and GPIOs. This function should be called to put the UART master
*  in its lowest possible power state.
* 
*  @param[in] UART_Handle  Driver handle returned by \c qapi_UART_Open.
*
*  @return
*  QAPI_OK      UART powered off successfully. \n
*  QAPI_ERROR   UART power off is failed.
*/
qapi_Status_t 
qapi_UART_Power_Off 
(
    qapi_UART_Handle_t handle
)
{
    return QAPI_OK;
}



