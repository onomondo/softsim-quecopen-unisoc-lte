/**  @file
  ql_cnktd_coap.h

  @brief
  This file is used to define cnktd coap for different Quectel Project.

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


#ifndef QL_CNKTD_COAP_H
#define QL_CNKTD_COAP_H

#include "ql_cnktd.h"
#include "ql_cnktd_ssl.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/** QAPI CoAP message types. */
typedef enum {
  QAPI_COAP_TYPE_CON, /**< Confirmables. */
  QAPI_COAP_TYPE_NON, /**< Non-confirmables. */
  QAPI_COAP_TYPE_ACK, /**< Acknowledgements. */
  QAPI_COAP_TYPE_RST  /**< Reset. */
}qapi_Coap_Message_Type_t;

/** CoAP header options. */
typedef enum {
  QAPI_COAP_OPTION_IF_MATCH = 1,       /**< 0-8 B. */
  QAPI_COAP_OPTION_URI_HOST = 3,       /**< 1-255 B. */
  QAPI_COAP_OPTION_ETAG = 4,           /**< 1-8 B. */
  QAPI_COAP_OPTION_IF_NONE_MATCH = 5,  /**< 0 B. */
  QAPI_COAP_OPTION_OBSERVE = 6,        /**< 0-3 B. */
  QAPI_COAP_OPTION_URI_PORT = 7,       /**< 0-2 B. */
  QAPI_COAP_OPTION_LOCATION_PATH = 8,  /**< 0-255 B. */
  QAPI_COAP_OPTION_URI_PATH = 11,      /**< 0-255 B. */
  QAPI_COAP_OPTION_CONTENT_TYPE = 12,  /**< 0-2 B. */
  QAPI_COAP_OPTION_MAX_AGE = 14,       /**< 0-4 B. */
  QAPI_COAP_OPTION_URI_QUERY = 15,     /**< 0-270 B. */
  QAPI_COAP_OPTION_ACCEPT = 17,        /**< 0-2 B. */
  QAPI_COAP_OPTION_TOKEN = 19,         /**< 1-8 B. */
  QAPI_COAP_OPTION_LOCATION_QUERY = 20, /**< 1-270 B. */
  QAPI_COAP_OPTION_BLOCK2 = 23,        /**< 1-3 B. */
  QAPI_COAP_OPTION_BLOCK1 = 27,        /**< 1-3 B. */
  QAPI_COAP_OPTION_SIZE = 28,          /**< 0-4 B. */
  QAPI_COAP_OPTION_PROXY_URI = 35,     /**< 1-270 B. */
  QAPI_COAP_OPTION_SIZE1 = 60,          /**< 0-4 B. */
  QAPI_COAP_OPTION_MAX                 /**< MAX COAP OPTION VALUE. */
}qapi_Coap_Option_t;

enum { QAPI_OPTION_MAP_SIZE = sizeof(uint8_t) * 8 };
#define QAPI_COAP_ETAG_LEN                        8      /**< Maximum number of bytes for ETag. */
#define QAPI_COAP_TOKEN_LEN                       8      /**< Maximum number of bytes for token. */
#define QAPI_COAP_MAX_ACCEPT_NUM                  2      /**< Maximum number of accept preferences to parse/store. */


/** CoAP content types. */
typedef enum {
  QAPI_TEXT_PLAIN = 0,                      /**< Plain text. */
  QAPI_TEXT_XML = 1,                        /**< XML text. */
  QAPI_TEXT_CSV = 2,                        /**< CSV text. */
  QAPI_TEXT_HTML = 3,                       /**< HTML text. */
  QAPI_IMAGE_GIF = 21,                      /**< GIF image. */
  QAPI_IMAGE_JPEG = 22,                     /**< JPEG image. */
  QAPI_IMAGE_PNG = 23,                      /**< PNG image. */
  QAPI_IMAGE_TIFF = 24,                     /**< TIFF image format. */
  QAPI_AUDIO_RAW = 25,                      /**< RAW audio. */
  QAPI_VIDEO_RAW = 26,                      /**< RAW video. */
  QAPI_APPLICATION_LINK_FORMAT = 40,        /**< Application link format. */
  QAPI_APPLICATION_XML = 41,                /**< Application XML. */
  QAPI_APPLICATION_OCTET_STREAM = 42,       /**< Application octet stream. */
  QAPI_APPLICATION_RDF_XML = 43,            /**< Application RDF XML. */
  QAPI_APPLICATION_SOAP_XML = 44,           /**< Application SOAP XML. */
  QAPI_APPLICATION_ATOM_XML = 45,           /**< Application ATOM XML. */
  QAPI_APPLICATION_XMPP_XML = 46,           /**< Application XMPP XML. */
  QAPI_APPLICATION_EXI = 47,                /**< Application EXI. */
  QAPI_APPLICATION_FASTINFOSET = 48,        /**< Application FastInfoSet */
  QAPI_APPLICATION_SOAP_FASTINFOSET = 49,   /**< Application SOAP FastInfoSet. */
  QAPI_APPLICATION_JSON = 50,               /**< Application JSON . */
  QAPI_APPLICATION_X_OBIX_BINARY = 51,      /**< Application X OBIX binary. */
  QAPI_M2M_TLV = 11542,                     /**< M2M TLV. */
  QAPI_M2M_JSON = 11543,                    /**< M2M JSON. */
}qapi_Coap_Content_Type_t;

/** CoAP multi option information. */
typedef struct _qapi_Multi_Option_t {
  struct _qapi_Multi_Option_t *next;   /**< Pointer to next option in multi-option list. */
  uint8_t is_static;                   /**< Option static or not.  */
  uint8_t len;                         /**< Option length. */
  uint8_t *data;                       /**< Pointer to option value. */
}qapi_Multi_Option_t;


/** Parsed message structure. */
typedef struct {
  uint8_t *buffer; /**< Pointer to CoAP header / Incoming packet buffer / Memory to serialize packet. */
  uint8_t version;                        /**< CoAP version number. */
  qapi_Coap_Message_Type_t type;          /**< CoAP message type e.g. CON|NON|ACK|RST. */
  uint8_t code;                           /**< CoAP message code. */
  uint16_t mid;                           /**< CoAP message ID. */
  
  uint8_t options[(QAPI_COAP_OPTION_MAX -1) / QAPI_OPTION_MAP_SIZE + 1]; /**< Bitmap to check if option is set. */
  
  qapi_Coap_Content_Type_t content_type; /**< Parse options once and store; allows setting options in random order. */
  uint32_t max_age;                       /**< Max-Age Option indicates the maximum time a response may be cached . */
  size_t proxy_uri_len;                   /**< Proxy-Uri length.*/
  uint8_t *proxy_uri;                     /**< Proxy-Uri indicates URI to be used to make a request to forward-proxy .*/
  uint8_t etag_len;                       /**< ETAG option length.*/
  uint8_t etag[QAPI_COAP_ETAG_LEN];       /**< Option for differentiating between representations of the same resource that vary over time.*/  
  size_t uri_host_len;                    /**< Length of Uri-Host.*/
  uint8_t * uri_host;                     /**< Pointer to Uri-Host that specify the target server or client .*/
  qapi_Multi_Option_t *location_path;     /**< Length of location query.*/
  uint16_t uri_port;                      /**< Port of the target coap server/client to connect with .*/
  size_t location_query_len;              /**< Pointer to CoAP server/client to connect with .*/
  uint8_t *location_query;                /**< Pointer to query that results to a specific target resource  .*/
  qapi_Multi_Option_t *uri_path;          /**< Path that specify the target resource.*/
  uint32_t observe;                       /**< Option for implementing CoAP observe functionality  .*/
  uint8_t token_len;                      /**< Length of the Token ID .*/
  uint8_t token[QAPI_COAP_TOKEN_LEN];     /**< Token ID of the CoAP message .*/
  uint8_t accept_num;                     /**< Length of the accept field .*/
  
  qapi_Coap_Content_Type_t accept[QAPI_COAP_MAX_ACCEPT_NUM]; /**< The option used to indicate Content-Format is acceptable to the target server or client .*/
  
  uint8_t if_match_len;                   /**< Length of the accept if match field.*/
  uint8_t if_match[QAPI_COAP_ETAG_LEN];   /**< Option to make a request for one or more ETAG representations of the target resource .*/
  uint32_t block2_num;                    /**< Block number of the CoAP message with BLOCK2 option.*/
  uint8_t block2_more;                    /**< More blocks followed or not during BLockwise transfer  .*/
  uint16_t block2_size;                   /**< Size of the payload during BLockwise transfer .*/
  uint32_t block2_offset;                 /**< Block2 offset number.*/
  uint32_t block1_num;                    /**< Block number of the CoAP message with BLOCK1 option .*/
  uint8_t block1_more;                    /**< More blocks followed or not during blockwise transfer .*/
  uint16_t block1_size;                   /**< Size of the payload during blockwise transfer .*/
  uint32_t block1_offset;                 /**< Block1 offset number .*/
  uint32_t size;                          /**< Specifies the CoAP Message size.*/
  uint32_t size1;                         /**< Size1 option provides size information about the resource representation in a request.*/
  qapi_Multi_Option_t * uri_query;        /**< Query path that specices target resource .*/
  uint8_t if_none_match;                  /**< Option used to request conditional on the non existence of the target resource .*/
  uint16_t payload_len;                   /**< Length of the payload contained in CoAP message .*/
  uint8_t *payload;                       /**< Payload contained in the CoAP message .*/
  uint16_t buffer_len;                    /**< Length of the buffer .*/
}qapi_Coap_Packet_t;

/**  Callback to be invoked when CoAP DL request is received
   This callback will be called in response to a DL message from the server to the CoAP 
   Client. The application needs to set this callback in qapi_Coap_Create_Session() via 
   qapi_Coap_Session_Info_t. 
   */
typedef  int32_t (*qapi_Coap_Client_Session_Cb)(qapi_Coap_Session_Hdl_t  hdl, qapi_Coap_Packet_t * message , void * usr_data);

/** CoAP client configuration information to be set while creating a CoAP Session via
    qapi_Coap_Create_Session(). Use the default values for the configuration parameters.
*/
typedef struct qapi_Coap_Session_Info_s {
  void *   clientData ;                   /**< IN PARAM : Usr_data. This usr_data will be passed in the DL Callback. */
  uint32_t coap_max_retransmits;          /**< Optional IN PARAM : Max Number of COAP Rxmts for CON Messages DEF VAL =  4. */
  uint32_t coap_transaction_timeout;      /**< Optional IN PARAM :Transaction Timeout for CON Messages DEF VAL = 2 Seconds. */
  double   coap_ack_random_factor;        /**< Optional IN PARAM :DEF VAL = 1.5. */
  uint32_t coap_max_latency;              /**< Optional IN PARAM :DEF VAL = 100. */
  uint32_t coap_default_maxage;           /**< Optional IN PARAM :DEF VAL = 60. */
  qapi_Coap_Client_Session_Cb cb;         /**< IN PARAM : Callback to be called as a part of any DL Requests ex GET/PUT/POST?DELETE. */

}qapi_Coap_Session_Info_t;

/** CoAP security mode Type. */
typedef enum {
  QAPI_COAP_MODE_PSK = 0,             /**< PSK. */
  QAPI_COAP_MODE_RAW_PUBLIC_KEY,      /**< RAW_PUBLIC_KEY. */
  QAPI_COAP_MODE_CERTIFICATE,         /**< CERTIFICATE. */
  QAPI_COAP_MODE_NONE,                /**< NON SEC. */
}qapi_Sec_Mode;

typedef enum {
        QAPI_COAP_PROTOCOL_UDP = 0x1,  /**< PROTOCOL_UDP. */
        QAPI_COAP_PROTOCOL_NIDD = 0x2, /**< PROTOCOL_NIDD. */
}qapi_Coap_Protocol_Type;


/** CoAP structure to hold the information related to 
   CERTIFICATES/PSK which will be used by CoAP to create a secure connection client that 
   can provide information for any one of qapi_Sec_Modes. Security type to be used by the 
   application when calling qapi_Coap_Create_Connection(). 
*/
typedef struct qapi_Coap_Sec_Info_s {
   qapi_Net_SSL_Cert_t cert;             /**< IN PARAM:Certificate location. */ 
   qapi_Net_SSL_CAList_t ca_list;        /**< IN PARAM:CA List location. */
   qapi_Net_SSL_PSKTable_t psk;          /**< IN PARAM:PSK location. */
   qapi_Net_SSL_DICERT_t di_cert;        /**< IN PARAM DI_CERY location. */
}qapi_Coap_Sec_Info_t;

/** Connection establishment configuration parameters
   qapi_Coap_Create_Connection().
*/
typedef struct qapi_Coap_Connection_Cfg_s {
   qapi_Sec_Mode  sec_Mode;              /**< SEC or NON SEC. */
   qapi_Coap_Protocol_Type proto;        /**< UDP or NIDD. */
   uint32_t   callHndl ;                 /**< Data call handle ,to be used for NIDD.*/
   char *   src_host;                    /**< Local IP. */
   uint16_t src_port;                    /**< Local port. */
   char *   dst_host;                    /**< Remote IP. */
   uint16_t dst_port;                    /**< Remote port. */
   uint16_t family_type;                 /**< IPv4 or IPv6. */
   boolean  resumption_enabled;          /**< Enble DTLS resumption. */ 
   int      session_resumption_timeout;  /**< Session timeout. */
   qapi_Net_SSL_Config_t * ssl_cfg;      /**< SSL configuration to be associated with the connection. */
   qapi_Coap_Sec_Info_t *  sec_info;     /**< Certificates/ PSK location information
                                         */
   boolean  disable_close_notify;        /**< Disable Close-Notify when shutting down the connection. */ 
}qapi_Coap_Connection_Cfg_t;

/** CoAP transaction info for the request/response received from network. */
struct _qapi_Coap_Transaction_ 
{
   uint16_t            mID;              /**< MID of the message whose response this CB is called. */
  qapi_Coap_Packet_t * message;          /**< Message in whose reply this CB is called. */
  uint8_t *            buffer;           /**< RAW Buffer Payload of message whose response is this CB. */
  void *               userData;         /**< Usr Data Passed in the message in whose reply this CB is called. */
  uint16_t             buffer_len;       /**< RAW Buffer Length of message whose response is this CB. */
  uint8_t              ack_received;     /**< If set, implies a response to our message, else a timeout has happened. */
  time_t               response_timeout; /**< Timeout associated with this message. */
  uint8_t              retrans_counter;  /**< Number for times this message was re-transmitted. */
  time_t               retrans_time;     /**< Re-transmit time associated with this message. */
};

typedef struct _qapi_Coap_Transaction_  qapi_Coap_Transaction_t;

/**  
* This callback will be called in response to a CON message sent by the CoAP client via qapi_Coap_Send_Message().
* Application needs to set this callback in qapi_Coap_Send_Message() via qapi_Coap_Message_Params_t
*/

typedef void (*qapi_Coap_Transaction_Callback_t) (qapi_Coap_Session_Hdl_t hdl,qapi_Coap_Transaction_t * transacP, qapi_Coap_Packet_t* message);


/** CoAP configuration structure to be passed for creating a message. */
typedef struct qapi_Coap_Message_s {
  uint16_t * lastmid;                     /**< OUTPARAM: MID used to send the message. */
  uint8_t  * token;                       /**< INPARAM: Token value to be used to send the message. */
  uint8_t token_len;                      /**< INPARAM: Token length. */
  qapi_Coap_Transaction_Callback_t msg_cb;/**< INPARAM: This callback will be called in response to a CON message 
                                                        sent by the application. The application needs to set this callback in 
                                                        qapi_Coap_Send_Message()
                                          */
  void * msgUsrData;                      /**< INPARAM: Application user data associated with this message
                                                      same will be returned as a part of msg_cb set
                                                      above for CON messages. */
}qapi_Coap_Message_Params_t;

/** Enumerations added for get and set header parameters for CoAP packet.
 */
typedef enum {
  QAPI_COAP_URI_PATH,          /**< URI PATH. */
  QAPI_COAP_URI_QUERY,         /**< URI_QUERY. */
  QAPI_COAP_CONTENT_TYPE,      /**< CONTENT_TYPE. */
  QAPI_COAP_TOKEN,             /**< TOKEN. */
  QAPI_COAP_OBSERVE,           /**< OBSERVE. */
  QAPI_COAP_LOCATION_PATH ,    /**< LOCATION_PATH. */
  QAPI_COAP_SET_STATUS,        /**< SET_STATUS. */
  QAPI_COAP_MULTI_OPTION,      /**< MULTI_OPTION. */
  QAPI_COAP_BLOCK1,            /**< BLOCK1. */
  QAPI_COAP_BLOCK2,            /**< BLOCK2. */
  QAPI_COAP_ETAG,              /**< ETAG. */
  QAPI_COAP_LOCATION_QUERY,    /**< LOCATION_QUERY. */
  QAPI_COAP_MAXAGE,            /**< MAXAGE. */
  QAPI_COAP_ACCEPT,            /**< ACCEPT. */
  QAPI_COAP_URI_HOST,          /**< URI_HOST. */
  QAPI_COAP_SIZE1,             /**< BLOCK SIZE1. */
  QAPI_COAP_SIZE,              /**< BLOCK SIZE. */
}qapi_Coap_Header_type;

/*========================================================================
 *  function Definition
 *========================================================================*/
/**
 * 
 * Data\_Services 1.2.0  &  Introduced. 
 * }
 *   This function is used to create a client CoAP Session.
 *  
 * @param [out] session            Pointer to a QAPI coap client session context that 
 *                                 will be returned and is to be used for all other
 *                                 further CoAPQAPIs associated with this session.
 * @param [in] coap_session_config Configuration parameters associated with 
 *           this session.
 * @return on success            -  QAPI_OK 
 *         on failure            -  QAPI_ERROR
 */
qapi_Status_t qapi_Coap_Create_Session (qapi_Coap_Session_Hdl_t  * session, qapi_Coap_Session_Info_t * coap_session_config);

/**
 * Macro that passes a byte pool pointer for the CoAP client.
 *
 * Parameter a -- Handle. \n
 * Parameter b -- Pointer to byte pool.
 *
 * On success, QAPI_OK is returned. On error, QAPI_ERROR is returned.
 *
 * @note1hang This macro is only used in the DAM space.
 */

qapi_Status_t qapi_Coap_Pass_Pool_Ptr(qapi_Coap_Session_Hdl_t hndl, void *coap_byte_pool_ptr);

/** 
 *
 *         This function is used to create a COAP connection with the remote server.

 *  
 * @param  sessionHandle     CoAP Session Handle returned by Create Session.
 * @param  coap_conn_config  Connection related config Parameters.
 * @return on success - QAPI_OK 
 *         on failure - QAPI_ERROR
 */
qapi_Status_t qapi_Coap_Create_Connection(qapi_Coap_Session_Hdl_t sessionHandle,qapi_Coap_Connection_Cfg_t * coap_conn_config );


/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *   This function is used to close a CoAP aonnection associated with the session.
 *  
 * @param  sessionHandle  CoAP session handle returned by Create Session.
 * @return on success            -  QAPI_OK 
 *         on failure            -  QAPI_ERROR
 */
qapi_Status_t qapi_Coap_Close_Connection(qapi_Coap_Session_Hdl_t sessionHandle);

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *    This function is used to destroy a CoAP session.
 *  
 * @param  sessionHandle  CoAP Session Handle returned by Create Session.
 * @return on success - QAPI_OK
 *         on failure            -  QAPI_ERROR
 */
qapi_Status_t qapi_Coap_Destroy_Session(qapi_Coap_Session_Hdl_t sessionHandle);

/** 
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *    This function is used to create a new request message.
 * @param sessionHandle   CoAP Session Handle returned by Create Session .
 * @param message         Pointer to qapi_Coap_Packet_t. The memory for this packet will
 *                        be allocated by this API.
 * @param coap_msg_type   CON/NON-CON/ACK/RESET.
 * @param msg_code        GET/POST/PUT/DELETE.
 * @return on success - QAPI_OK 
 *         on failure - QAPI_ERROR
 */
qapi_Status_t qapi_Coap_Init_Message(qapi_Coap_Session_Hdl_t sessionHandle , qapi_Coap_Packet_t ** message , qapi_Coap_Message_Type_t coap_msg_type ,uint8_t msg_code );

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *          This function is used to set coap payload.
 *
 * @param session      CoAP Session Handle returned by Create Session .
 * @param   packet     Pointer to qapi coap packet.
 * @param   payload    Pointer to payload.
 * @param   length     Length of payload. 
 * @return  on success - 1 
 *          on error   - 0
 */
qapi_Status_t qapi_Coap_Set_Payload(qapi_Coap_Session_Hdl_t  session ,qapi_Coap_Packet_t *packet, const void *payload, size_t length);

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *         This function is used to send the message to coap. Failures would internally
 *   release the allocated resources.
 *  
 * @param sessionHandle    CoAP Session Handle returned by Create Session .
 * @param message          Pointer to qapi CoAP message.
 * @param msg_conf         Pointer to CoAP Message config structure
 * return  on success   -  0
 *         on error     - -1 
 */
qapi_Status_t qapi_Coap_Send_Message(qapi_Coap_Session_Hdl_t  sessionHandle , qapi_Coap_Packet_t * message, qapi_Coap_Message_Params_t * msg_conf);

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * } 
 *          This function is used to free the coap message.
 * @param sessionHandle   CoAP Session Handle returned by Create Session .
 * @param message         Pointer to qapi coap message 
 * @return  void
 */
qapi_Status_t qapi_Coap_Free_Message(qapi_Coap_Session_Hdl_t  sessionHandle ,qapi_Coap_Packet_t *message);

/** 
* @versiontable{2.0,2.45,
* Data\_Services 1.8.0  &  Introduced. @tblendline
* }
*     This function is used to set extended header parameters.
*  
* @param session      CoAP Session Handle returned by Create Session .
* @param message      Pointer to coap message header to be filled.
* @param header_type  Option number of header field to be filled.
*                     This API needs to be called, if Option number is not present 
*                     in qapi_Coap_Header_type enum.
* @param header_val   Pointer to header value.
* @param val_len      Length of header value.
*
* @return See Section @xref{hdr:QAPIStatust}. \n
* On success, QAPI_OK (0) is returned. Other value on error.
*             QAPI_NET_COAP_EINVALID_PARAM   - Invalid parameters 
*             QAPI_NET_COAP_HANDLE_NOT_FOUND - CoAP session handle not found 
*             QAPI_ERROR                     - For generic failures.
*/
qapi_Status_t qapi_Coap_Set_Header_Ext(qapi_Coap_Session_Hdl_t sessionhandle ,qapi_Coap_Packet_t *message , uint16_t header_type , const void *header_val , size_t val_len);

/** 
* @versiontable{2.0,2.45,
* Data\_Services 1.2.0  &  Introduced. @tblendline
* }
*     This function is used to set header parameters.
*  
* @param session      CoAP Session Handle returned by Create Session .
* @param message      Pointer to coap message header to be filled.
* @param header_type  Type of header field to be filled.
*                     This API needs to be called, if Option number is present 
*                      in qapi_Coap_Header_type enum.
* @param header_val   Pointer to header value.
* @param val_len      Length of header value.
* @return int value 
*/
qapi_Status_t qapi_Coap_Set_Header(qapi_Coap_Session_Hdl_t  session ,qapi_Coap_Packet_t*message , qapi_Coap_Header_type header_type , const void *header_val , size_t val_len); 

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_COAP_H */


