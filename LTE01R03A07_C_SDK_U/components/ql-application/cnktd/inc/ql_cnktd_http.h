/**  @file
  ql_cnktd_http.h

  @brief
  This file is used to define cnktd http for different Quectel Project.

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


#ifndef QL_CNKTD_HTTP_H
#define QL_CNKTD_HTTP_H

#include "ql_cnktd.h"
#include "ql_cnktd_ssl.h"
#include "ql_http_client.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/**
 *  HTTP client handle used by most of qapi_Net_HTTPc_xxxx().
 */
typedef void* qapi_Net_HTTPc_handle_t;

/**
 *  HTTP response user callback registered during qapi_Net_HTTPc_New_sess().
 *
 * @param[in] arg	 User payload information.
 * @param[in] state  HTTP response state.
 * @param[in] value	 HTTP response information.
 */
typedef void (*qapi_HTTPc_CB_t)
(
  void* arg,
  int32_t state,
  void* value
);

/** @addtogroup qapi_networking_httpc
@{ */

/**
 * @brief HTTP request types supported by qapi_Net_HTTPc_Request().
 */
typedef enum 
{
  /* Supported http client methods */
  QAPI_NET_HTTP_CLIENT_GET_E = 1,
  /**< HTTP get request. */
  QAPI_NET_HTTP_CLIENT_POST_E,
  /**< HTTP post request. */
  QAPI_NET_HTTP_CLIENT_PUT_E,
  /**< HTTP put request. */
  QAPI_NET_HTTP_CLIENT_PATCH_E,
  /**< HTTP patch request. */
  QAPI_NET_HTTP_CLIENT_HEAD_E,
  /**< HTTP head request. */
  QAPI_NET_HTTP_CLIENT_CONNECT_E,
  /**< HTTP connect request. */
  QAPI_NET_HTTP_CLIENT_DELETE_E
  /**< HTTP delete request. */
} qapi_Net_HTTPc_Method_e;


/*========================================================================
 *  function Definition
 *========================================================================*/

/** @ingroup qapi_create_ssl_object
 * Creates a new SSL object (client).
 *
 * @param[in] role Client role. Server is not supported.
 *
 * @return
 * SSL object handle on success. \n
 * QAPI_NET_SSL_HDL_NULL on error (out of memory).
 *
 * @dependencies
 * This function must be called before using any other SSL function.
 */
 //unnecessary at EG915U,Just return true then.
qapi_Net_SSL_Obj_Hdl_t qapi_Net_SSL_Obj_New(qapi_Net_SSL_Role_t role);

/**
 * Creates a new HTTP client session. 
 *
 * @details To create a client session, the caller must invoke this function 
 *          and the handle to the newly created context is returned if successful. As part 
 *          of the function call, a user callback function is registered with the HTTP 
 *          client module that gets invoked for that particular session if there is some
 *          response data from the HTTP server. Passing in the SSL context information
 *          ensures that a secure session is created. Any HTTP connection or security 
 *          related configuration can be configured using the configuration QAPIs.
 *
 * @param[in] timeout   Timeout (in ms) of a session method (1s-65536s).
 * @param[in] uint32_t  SSL context for HTTPs connect (0-5).
 * @param[in] callback  Register a callback function; NULL for no support for a callback.
 * @param[in] arg       User data payload to be returned by the callback function.
 * @param[in] httpc_Max_Body_Length    Maximum body length for this session.(This parameter is invalid)
 * @param[in] httpc_Max_Header_Length  Maximum header length for this session.(This parameter is invalid)
 *
 * @return On success, #qapi_Net_HTTPc_handle_t is returned. NULL otherwise.
 */
qapi_Net_HTTPc_handle_t qapi_Net_HTTPc_New_sess(
        uint32_t timeout,
        qapi_Net_SSL_Obj_Hdl_t ssl_Object_Handle,
        http_client_event_cb_t callback,
        void* arg,
        uint32_t httpc_Max_Body_Length,
        uint32_t httpc_Max_Header_Length);

/**
 *  Configures an HTTP client session.
 *
 * @details Invocation of this function configures the HTTP client SSL session. 
 *
 * @param[in] handle    Handle to the HTTP client session.
 * @param[in] ssl_Cfg   SSL configuration information. 
 *
 * @return On success, 0 is returned. Other value on error.
 */
 //suggest to ignore this API, use ql_httpc_setopt directly.
qapi_Status_t qapi_Net_HTTPc_Configure_SSL(qapi_Net_HTTPc_handle_t handle, qapi_Net_SSL_Config_t *ssl_Cfg);

/*
 * Macro that Passes Byte Pool Pointer for HTTP Client 
 * Parameter 'a' : Handle
 * Parameter 'b' : Pointer to Byte Pool 
 * On success, QAPI_OK is returned. On error, QAPI_ERROR is returned.
 * (This Macro is only used in DAM Space)
 */
//unnecessary at EG915U,Just return true then.
int qapi_Net_HTTPc_Pass_Pool_Ptr(qapi_Net_HTTPc_handle_t hndl, void *httpc_byte_pool_ptr);

/**
 * Releases an HTTP client session.
 *
 * @details An HTTP client session that is connected to the HTTP server is disconnected 
            before releasing the resources associated with that session. 
 *
 * @param[in] handle  Handle to the HTTP client session.
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Free_sess(qapi_Net_HTTPc_handle_t handle);

/** @ingroup qapi_free_ssl_object
 * Frees the SSL object handle.
 *
 * @param[in] hdl SSL object handle.
 *
 * @return
 * QAPI_OK on success.
 *
 * @dependencies
 * All connections belonging to this handle must be closed before calling this
 * API.
 */
 //unnecessary at EG915U,Just return true then.
qapi_Status_t qapi_Net_SSL_Obj_Free(qapi_Net_SSL_Obj_Hdl_t hdl);

/**
 *  Connects an HTTP client session to the HTTP server.
 *
 * @details The HTTP client session is connected to the HTTP server in blocking mode. 
 *
 * @param[in] handle  Handle to the HTTP client session.
 * @param[in] URL     Server URL informtion.  
 * @param[in] port    Server port information. 
 *
 * @return On success, 0 is returned. Other value on error.
 */
 //suggest to ignore this API, use qapi_Net_HTTPc_Request directly.
qapi_Status_t qapi_Net_HTTPc_Connect(qapi_Net_HTTPc_handle_t handle, const char *URL, uint16_t port);

/**
 *  Disconnects an HTTP client session from the HTTP server.
 *
 * @details The HTTP client session that is connected to the HTTP server is disconnected 
            from the HTTP server. 
 *
 * @param[in] handle  Handle to the HTTP client session.
 *
 * @return On success, 0 is returned. Other value on error.
 */
//suggest to ignore this API, use qapi_Net_HTTPc_Free_sess directly.
qapi_Status_t qapi_Net_HTTPc_Disconnect(qapi_Net_HTTPc_handle_t handle);

/**
 *  Adds an HTTP client session header field.
 *
 * @details Multiple invocations of this function will result in appending the 
            header type-value pair information to the internal header buffer.
            Maximum allowed header size is configured at HTTP session creation.
 *
 * @param[in] handle    Handle to the HTTP client session.
 * @param[in] type      HTTP header type related information. 
 * @param[in] value     HTTP value associated with the header type. 
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Add_Header_Field(qapi_Net_HTTPc_handle_t handle, const char *type, const char *value);

/**
 *  Processes the HTTP client session requests.
 *
 * @details HTTP client session requests are processed and sent to the HTTP server. 
 *
 * @param[in] handle   Handle to the HTTP client session.
 * @param[in] cmd      HTTP request method information(Configure in advance, ignore this parameter). 
 * @param[in] URL      Server URL information.
 *
 * @return On success, 0 is returned. Other value on error.
 */
qapi_Status_t qapi_Net_HTTPc_Request(qapi_Net_HTTPc_handle_t handle, qapi_Net_HTTPc_Method_e cmd, const char *URL);

/**
 * Starts or restarts an HTTP client module.
 *
 * @details This function is invoked to start or restart the HTTP client after it is
 *          stopped via a call to qapi_Net_HTTPc_Stop().
 *
 * @return On success, 0 is returned. Other value on error.
 */
 //unnecessary at EG915U,Just return true then.
qapi_Status_t qapi_Net_HTTPc_Start(void);

/**
 * Stops an HTTP client module.
 *
 * @details This function is invoked to stop the HTTP client after it was started via 
 *          a call to qapi_Net_HTTPc_Start().
 *
 * @return On success, 0 is returned. Other value on error.
 */
 //unnecessary at EG915U,Just return NOT SUPPORTED then.
qapi_Status_t qapi_Net_HTTPc_Stop(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_HTTP_H */


