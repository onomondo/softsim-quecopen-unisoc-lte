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

#include "ql_cnktd_coap.h"

#include "ql_log.h"

/*
 * @fn     void *qapi_coap_register_client() 
 * @brief  This function is used to send client registration request to coap
 * @param  coap_register_clientP - pointer to qapi coap client context  
 * @return on success            - void pointer of type coap handler 
 *         on failure            - NULL 
 */
qapi_Status_t qapi_Coap_Create_Session(qapi_Coap_Session_Hdl_t  *session, qapi_Coap_Session_Info_t * coap_session_config)
{
    return QAPI_ERR_NOT_SUPPORTED;
}

qapi_Status_t qapi_Coap_Pass_Pool_Ptr(qapi_Coap_Session_Hdl_t hndl, void *coap_byte_pool_ptr)
{
    return QAPI_ERR_NOT_SUPPORTED;
}

/* 
 * @fn     void * qapi_coap_create_connection() 
 * @brief  This functionis used to send connection request to coap
 * @param  contextP   - pointer to  coap client context 
 *         host       - pointer to ip address string
 *         port       - pointer to port string 
 *         ssl_cfg    - pointer to certificate name
 *         cfg        - pointer to SSL configuration
 * @return on success - connection_t pointer 
 *         on failure - NULL
 */ 
qapi_Status_t qapi_Coap_Create_Connection(qapi_Coap_Session_Hdl_t sessionHandle,qapi_Coap_Connection_Cfg_t * coap_conn_config )
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/*
 * @fn     void qapi_coap_close_connection()
 * @brief  This function is used to close the session by invoking coap_close_connection 
 * @param  coap_handle  - pointer to client context 
 * @return void 
 */
qapi_Status_t qapi_Coap_Close_Connection(qapi_Coap_Session_Hdl_t sessionHandle)
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/*
 * @fn      void qapi_coap_deregister_client()
 * @brief   This function is used to close coap connection by invoking coap_close 
 * @param   coapHandle  - coap handle
 * @return   void 
 */
qapi_Status_t qapi_Coap_Destroy_Session(qapi_Coap_Session_Hdl_t  session)
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/* 
 * @fn      void *qapi_coap_create_message()
 * @brief   This function is used to create new request message
 * @param   coap_hanlde    - coap client context handle
 *          coap_msg_type  - qapi coap message type 
 *          msg_code       - request message code 
 * @return  on success     - pointer to coap_packet_t 
            on error       - NULL
 */ 

qapi_Status_t qapi_Coap_Init_Message(qapi_Coap_Session_Hdl_t sessionHandle , qapi_Coap_Packet_t ** message , qapi_Coap_Message_Type_t coap_msg_type ,uint8_t msg_code )
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/*
 * @fn      int qapi_coap_set_payload()
 * @brief   This function is used to set coap payload
 * @param   packet     - pointer to qapi coap packet
 *          payload    - pointer to payload
 *          length     - length of payload 
 * @return  on success - 1 
 *          on error   - 0
 */       

qapi_Status_t qapi_Coap_Set_Payload(qapi_Coap_Session_Hdl_t sessionhandle , qapi_Coap_Packet_t *packet, const void *payload, size_t length)
{
  return QAPI_ERR_NOT_SUPPORTED;
}

/*
 * @fn     int qapi_coap_send_message()
 * @brief  This function is used to send the message to coap. Failures would internally
 *         release the allocated resources. 
 * @param  qapi_message - pointer to qapi coap message 
 * return  on success   -  0
 *         on error     - -1 
 */

qapi_Status_t qapi_Coap_Send_Message(qapi_Coap_Session_Hdl_t  sessionHandle , qapi_Coap_Packet_t * pkt, qapi_Coap_Message_Params_t * msg_conf)
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/* 
 * @fn      void qapi_coap_free_message()
 * @brief   This function is used to free the coap message 
 * @param   messageP - pointer to qapi coap message 
 * @retrun  void
 */   
qapi_Status_t qapi_Coap_Free_Message(qapi_Coap_Session_Hdl_t sessionhandle ,qapi_Coap_Packet_t *messageP)
{
   return QAPI_ERR_NOT_SUPPORTED;
}

/** 
 * @fn  qapi_Status_t qapi_Coap_Set_Header_Ext
 *
 * @brief  This function is used to set extended header parameters.
 *  
 * @param session      CoAP Session Handle returned by Create Session .
 * @param message      Pointer to coap message header to be filled.
 * @param header_type  Option number of header field to be filled.
 * @param header_val   Pointer to header value.
 * @param val_len      Length of header value.
 *
 * @return  on success - QAPI_OK 
 *          on error   - QAPI_ERROR
 */
 qapi_Status_t qapi_Coap_Set_Header_Ext(qapi_Coap_Session_Hdl_t sessionhandle ,qapi_Coap_Packet_t *message , uint16_t header_type , const void *header_val , size_t val_len)
 {
   return QAPI_ERR_NOT_SUPPORTED;
 }

 /* @fn     int qapi_coap_set_header()
  * @brief  This function is used to set header parameters 
  * @param  message      - pointer to coap message header to be fill
  *         header_type  - type of header field to be fill
  *         header_val   - pointer to header value 
  *         val_len      - length of header value 
  * @return int value 
  */
qapi_Status_t qapi_Coap_Set_Header(qapi_Coap_Session_Hdl_t sessionhandle ,qapi_Coap_Packet_t *message , qapi_Coap_Header_type header_type , const void *header_val , size_t val_len)
{
    return QAPI_ERR_NOT_SUPPORTED;
}



