/**  @file
  ql_cnktd_ssl.h

  @brief
  This file is used to define cnktd ssl for different Quectel Project.

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


#ifndef QL_CNKTD_SSL_H
#define QL_CNKTD_SSL_H

#include "ql_cnktd.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

    /** Maximum number of cipher suites that can be configured. */
#define QAPI_NET_SSL_CIPHERSUITE_LIST_DEPTH 8
    /** Maximum number of characters in a certificate or CA list name. */
#define QAPI_NET_SSL_MAX_CERT_NAME_LEN (64)
    
/**
 * Structure to specify the certificate verification policy.
 */
typedef struct /** @cond */__qapi_Net_SSL_Verify_Policy_s/** @endcond */ {
    uint8_t domain ;
    /**< TRUE to verify certificate commonName against the peer's domain name. */

    uint8_t time_Validity ;
    /**< TRUE to verify certificate time validity. */

    uint8_t send_Alert ;
    /**< TRUE to immediately send a fatal alert on detection of an untrusted certificate. */

    char match_Name[QAPI_NET_SSL_MAX_CERT_NAME_LEN];
    /**< Name to match against the common name or altDNSNames of the certificate. See
     #QAPI_NET_SSL_MAX_CERT_NAME_LEN. */
}  qapi_Net_SSL_Verify_Policy_t;


/**
 * Structure to configure an SSL connection.
 */
typedef  struct /** @cond */__qapi_Net_SSL_Config_s/** @endcond */ {
    uint16_t protocol ;
    /**< Protocol to use. See QAPI_NET_SSL_PROTOCOL_*. */

    uint16_t cipher[QAPI_NET_SSL_CIPHERSUITE_LIST_DEPTH] ;
    /**< Cipher to use. See SSL cipher suites QAPI_NET_TLS* and
     #QAPI_NET_SSL_CIPHERSUITE_LIST_DEPTH. */

    qapi_Net_SSL_Verify_Policy_t verify ;
    /**< Certificate verification policy. */

    uint16_t max_Frag_Len;
    /**< Maximum fragment length in bytes. */

    uint16_t max_Frag_Len_Neg_Disable;
    /**< Whether maximum fragment length negotiation is allowed. See RFC 6066. */

    uint16_t sni_Name_Size;
    /**< Length of the SNI server name. */

    char *sni_Name;
    /**< Server name for SNI. */

}  qapi_Net_SSL_Config_t;

/**
 * Internal certificate format. The certificate is in a binary
 * format optimized for speed and size. The *.bin foramt
 * certificate can be created using the command line tool
 * [SharkSslParseCert].
 *
 * @usage
 * SharkSslParseCert \<cert file\> \<privkey file\> [-p
 * \<passkey\>] [-b \<binary output file\>]
 */
typedef const void * qapi_Net_SSL_Cert_t;

/**
 * Internal CA list format. The CA list is in a binary format
 * optimized for speed and size. The list can be created
 * using the command line tool [SharkSSLParseCAList].
 *
 * @usage
 * SharkSSLParseCAList [-b \<binary output file\>]
 *     \<certfile\> [certfile...]
 *     where certfile is a .PEM file containing
 *     one or more certificates
 *
 *
 */
typedef const void * qapi_Net_SSL_CAList_t;

/**
 * Internal psk_table format.PSK table is in an optimized binary format.
 * The table can be created by using the command line tool
 * [SharkSslParsePSKTable]. Set the PSK file
 * format before using the tool. \n
 * Identity_1: psk_key1 \n
 * Identity_2: psk_key2
 *
 * @usage
 * SharkSslParsePSKTable \<PSK file\> [-b \<binary output
 * file\>]
 *
 */
typedef const void * qapi_Net_SSL_PSKTable_t;

/**
 * Internal DI certificate format. The certificate is in a binary
 * format optimized for speed and size.
 */
typedef const void * qapi_Net_SSL_DICERT_t;

/**
 * Handle to an SSL object.
 *
 * This is obtained from a call to qapi_Net_SSL_Obj_New(). The handle is
 * freed with a call to qapi_Net_SSL_Obj_Free().
 */
typedef uint32_t qapi_Net_SSL_Obj_Hdl_t;

/**
 * SSL object role.
 */
typedef enum
{
  QAPI_NET_SSL_SERVER_E = 1,
  /**< Server role. Not supported. */

  QAPI_NET_SSL_CLIENT_E = 2
  /**< Client role. */
} qapi_Net_SSL_Role_t;

/*========================================================================
 *  function Definition
 *========================================================================*/


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_SSL_H */


