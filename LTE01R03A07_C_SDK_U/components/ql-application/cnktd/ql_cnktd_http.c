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

#include "ql_cnktd_http.h"

#include "ql_log.h"

qapi_Net_SSL_Obj_Hdl_t qapi_Net_SSL_Obj_New(qapi_Net_SSL_Role_t role)
{
    return 0;
}

/**
 * @brief Creates new HTTP client session 
 *
 * @details In order to create a client session the caller needs to invoke this function 
 *          and the handle to the newly created context is returned if success. As part 
 *          of the function call, an user callback function is registered with the HTTP 
 *          client module that gets invoked for that particular session if there is some
 *          response data from the HTTP server. Passing in the SSL context information
 *          would ensure that a secure session would get created.           
 *
 * @param[in] timeout   Timeout(ms) on session method (1s-65536s)
 * @param[in] ssl_ctx   SSL context for HTTPs connect (0-5)
 * @param[in] callback  Register callback function, NULL for not support callback
 * @param[in] arg       User data payload to be returned by the callback function
 * @param[in] httpc_max_body_length    Max body length on this session(This parameter is invalid)
 * @param[in] httpc_max_header_length  Max header length on this session(This parameter is invalid)
 *
 * @return On success, qapi_Net_HTTPc_handle_t is returned. NULL otherwise.
 */
qapi_Net_HTTPc_handle_t qapi_Net_HTTPc_New_sess(uint32_t timeout, qapi_Net_SSL_Obj_Hdl_t ssl_ctx, http_client_event_cb_t callback, void* arg, uint32_t httpc_max_body_length, uint32_t httpc_max_header_length)
{
	http_client_t client = 0;
	qapi_Net_HTTPc_handle_t HTTPc_handle_t = NULL;
	uint32_t timeout_s = timeout/1000;
	if ((callback == NULL) || (ssl_ctx < 0 || ssl_ctx > 5) || (timeout < 1 || timeout > 65536))
	{
		return NULL;
	}
    if(ql_httpc_new(&client, callback, arg) != HTTP_SUCCESS)
    {
    	return NULL;
	}
	ql_httpc_setopt(&client, HTTP_CLIENT_OPT_INTERVAL_TIME, timeout_s);
	ql_httpc_setopt(&client, HTTP_CLIENT_OPT_SSLCTXID, ssl_ctx);
	HTTPc_handle_t = (qapi_Net_HTTPc_handle_t)client;
    return HTTPc_handle_t;
}

/**
 * @brief Configure HTTP client session securely
 *
 * @details Configure the HTTP client session securely based on the application requirement.  
 *
 * @param[in] handle    Handle to the HTTP client session
 * @param[in] ssl_Cfg   SSL configuration information 
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Configure_SSL(qapi_Net_HTTPc_handle_t handle, qapi_Net_SSL_Config_t *ssl_Cfg)
{
    return 0;
}

int qapi_Net_HTTPc_Pass_Pool_Ptr(qapi_Net_HTTPc_handle_t hndl, void *httpc_byte_pool_ptr)
{
    return 0;
}

/**
 * @brief Release HTTP client session
 *
 * @details HTTP client session that is connected to the HTTP server is disconnected prior 
            to releasing the resources associated with that session. 
 *
 * @param[in] handle  Handle to the HTTP client session
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Free_sess(qapi_Net_HTTPc_handle_t handle)
{
    if(handle == NULL)
    {
		return QAPI_ERR_INVALID_PARAM;
	}
	ql_httpc_release((http_client_t *)&handle);
    return QAPI_OK;
}

qapi_Status_t qapi_Net_SSL_Obj_Free(qapi_Net_SSL_Obj_Hdl_t hdl)
{
    return 0;
}

/**
 * @brief Connect HTTP client session to the HTTP server
 *
 * @details HTTP client session is connected to the HTTP server in non-blocking mode. 
 *
 * @param[in] handle  Handle to the HTTP client session
 * @param[in] URL     Server URL informtion  
 * @param[in] port    Server port information 
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Connect(qapi_Net_HTTPc_handle_t handle, const char *URL, uint16_t port)
{
    return 0;
}

/**
 * @brief Disconnect HTTP client session from the HTTP server
 *
 * @details HTTP client session that is connected to the HTTP server is disconnected 
            from the HTTP server. 
 *
 * @param[in] handle  Handle to the HTTP client session
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Disconnect(qapi_Net_HTTPc_handle_t handle)
{ 
    return 0;
}

/**
 * @brief Add HTTP client session header field
 *
 * @details Multiple invocations of this function would result in appending the 
            header type-value pair information to the internal buffer. 
 *
 * @param[in] handle    Handle to the HTTP client session
 * @param[in] type      HTTP header type related inforamtion 
 * @param[in] value     HTTP value associated with the header type 
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Add_Header_Field(qapi_Net_HTTPc_handle_t handle, const char *type, const char *value)
{
    char header[256]={0};
	if ((handle == NULL) || (type == NULL) || (value == NULL))
    {
		return QAPI_ERR_INVALID_PARAM;
	}
	snprintf(header, 256,"%s: %s", type, value);
    if(ql_httpc_setopt((http_client_t *)&handle, HTTP_CLIENT_OPT_REQUEST_HEADER, header) != HTTP_SUCCESS)
    {
    	return QAPI_ERR_INVALID_PARAM;
	}
    return QAPI_OK;
}

/**
 * @brief Process the HTTP client session requests
 *
 * @details HTTP client session requests are processed and sent to the HTTP server. 
 *
 * @param[in] handle   Handle to the HTTP client session
 * @param[in] cmd      HTTP request method information 
 * @param[in] URL      Server URL information
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Request(qapi_Net_HTTPc_handle_t handle, qapi_Net_HTTPc_Method_e cmd, const char *URL)
{
//    if(cmd == QAPI_NET_HTTP_CLIENT_GET_E)
//    {
//		cmd = HTTP_METHOD_GET;
//	}
//	else if(cmd == QAPI_NET_HTTP_CLIENT_POST_E)
//    {
//		cmd = HTTP_METHOD_POST;
//	}
//	else if(cmd == QAPI_NET_HTTP_CLIENT_PUT_E)
//    {
//		cmd = HTTP_METHOD_PUT;
//	}
////	else if(cmd == QAPI_NET_HTTP_CLIENT_PATCH_E)
////    {
////		cmd = HTTP_METHOD_GET;
////	}
//	else if(cmd == QAPI_NET_HTTP_CLIENT_HEAD_E)
//    {
//		cmd = HTTP_METHOD_HEAD;
//	}
//	else
//	{
//		return QAPI_ERR_INVALID_PARAM;
//	}
    if ((handle == NULL) || (URL == NULL))
    {
		return QAPI_ERR_INVALID_PARAM;
	}
	//ql_httpc_setopt((http_client_t *)&handle, HTTP_CLIENT_OPT_METHOD, cmd);
	ql_httpc_setopt((http_client_t *)&handle, HTTP_CLIENT_OPT_URL, URL);
	if(ql_httpc_perform((http_client_t *)&handle) != HTTP_SUCCESS)
	{
		return QAPI_ERROR;
	}
    return QAPI_OK;
}

/**
 * @brief Start/Re-start HTTP client module
 *
 * @details Function is invoked to start or restart the HTTP client after it is
 *          stopped via call to qapi_Net_HTTPc_Stop().
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Start(void)
{
	return 0;
}

/**
 * @brief Stop HTTP client module
 *
 * @details Function is invoked to stop the HTTP client after it was started via 
            call to qapi_Net_HTTPc_Start().
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Stop(void)
{
	return QAPI_ERR_NOT_SUPPORTED;
}




