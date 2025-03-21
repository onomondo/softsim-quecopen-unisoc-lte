/**  @file
  ql_cnktd.h

  @brief
  This file is used to define cnktd for different Quectel Project.

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


#ifndef QL_CNKTD_H
#define QL_CNKTD_H

#include "ql_api_common.h"


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/*
 * The following definitions are used to format error codes based on their
 * module. Error codes that use these macros will be a negative value of
 * the format -((0x9F000000) + <Status Code>).
 */
#define __QAPI_ERR_MOD_OFFSET                (QL_COMPONENT_APP_START)
#define __QAPI_ERR_ENCAP_MOD_ID(__mod_id__)  (__QAPI_ERR_MOD_OFFSET << 16)
#define __QAPI_ERROR(__mod_id__, __err__)    (__QAPI_ERR_ENCAP_MOD_ID(__mod_id__) + (__err__))
#define QAPI_MOD_BASE                        0          //

#define QAPI_OK                              ((qapi_Status_t)(0))                               /**< Success                   */
#define QAPI_ERROR                           ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  1))) /**< General error             */
#define QAPI_ERR_INVALID_PARAM               ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  2))) /**< Invalid parameter         */
#define QAPI_ERR_NO_MEMORY                   ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  3))) /**< Memory allocation error   */
#define QAPI_ERR_NO_RESOURCE                 ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  4))) /**< Resource allocation error */
#define QAPI_ERR_BUSY                        ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  6))) /**< Opertion busy             */
#define QAPI_ERR_NO_ENTRY                    ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  7))) /**< Entry not found           */
#define QAPI_ERR_NOT_SUPPORTED               ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  8))) /**< Feature not supported     */
#define QAPI_ERR_TIMEOUT                     ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE,  9))) /**< Operation timed out       */
#define QAPI_ERR_BOUNDS                      ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 10))) /**< Out of bounds             */
#define QAPI_ERR_BAD_PAYLOAD                 ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 11))) /**< Bad Payload               */
#define QAPI_ERR_EXISTS                      ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 12))) /**< Entry already exists      */
#define QAPI_ERR_NOT_INITIALIZED             ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 13))) /**< Unintialized              */
#define QAPI_ERR_INVALID_STATE               ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 15))) /**< Invalid state             */
#define QAPI_ERR_API_DEPRACATED              ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, 14))) /**< Qapi function is deprecated*/

typedef int32_t qapi_Status_t;


/********************* Quectel OEM error number Definitions **********************/
typedef enum{
	QAPI_QT_ERR_OK = __QAPI_ERROR(QAPI_MOD_BASE, 100),
	QAPI_QT_ERR_NORMAL_FAIL,
	QAPI_QT_ERR_INVALID_PARAM,
	QAPI_QT_ERR_OPERATE_FAILED,
	QAPI_QT_ERR_NO_MEM,
	QAPI_QT_ERR_PORT_ID,
	QAPI_QT_ERR_STREAM_ID,
	QAPI_QT_ERR_LOC_STOPPED,
	QAPI_QT_ERR_LOC_STARTED,
	QAPI_QT_ERR_GEOFENCE_ID_NOT_EXIST,
	QAPI_QT_ERR_FOTA_INVALID_PKG,
	QAPI_QT_ERR_FOTA_NOT_IN_DOWNLOADING,
	
    QAPI_QT_ERR_FTP_ERR_HDL,            /* ftp client handle is null or invalid */
    QAPI_QT_ERR_FTP_OCCUPIED,           /* ftp client already startup */
    QAPI_QT_ERR_FTP_OPEN_CTRL_CON_FAIL, /* ftp connect failed */
    QAPI_QT_ERR_FTP_OPEN_DATA_CON_FAIL, 
    QAPI_QT_ERR_FTP_CLOSED_BY_PEER,
    QAPI_QT_ERR_FTP_NOT_ALLOWED,
    QAPI_QT_ERR_FTP_ERR_RESP,
    QAPI_QT_ERR_FTP_NO_SESS,
    QAPI_QT_ERR_FTP_NO_FILE,
    QAPI_QT_ERR_FTP_READ_FILE,
    QAPI_QT_ERR_FTP_SEND_DATA,

    QAPI_QT_ERR_OPER_NOT_SUPPORT,           /*Operation not supported*/
    QAPI_QT_ERR_GNSS_SUB_BUSY,              /*GNSS subsystem busy.*/
    QAPI_QT_ERR_FUNC_NOT_EN,                /* Function not enable */
    QAPI_QT_ERR_TIME_INFO_ERR,              /*Time info error*/
    QAPI_QT_ERR_GNSS_XTRA_NOT_ENABLE,       /*   XTRA not enable*/
    QAPI_QT_ERR_GNSS_XTRA_FILE_OPEN_ERR,    /*XTRA file open failed.*/
    QAPI_QT_ERR_GNSS_NO_TIME,               /*Not get time now*/
    QAPI_QT_ERR_GNSS_XTRA_FILE_NOT_EXIST,   /*XTRA file is not exist*/
    QAPI_QT_ERR_GNSS_XTRA_ON_DOWNLOADING,   /*xtra downloading */
    QAPI_QT_ERR_GNSS_XTRA_DATA_VALID,       /*xtra data is valid*/
    QAPI_QT_ERR_GNSS_IS_ACTIVE,             /*GNSS is working*/
    QAPI_QT_ERR_GNSS_TIME_INJECT_ERR,       /*Time info error*/
    QAPI_QT_ERR_GNSS_XTRA_DATA_INVALID,     /*xtra data is bad*/
    QAPI_QT_ERR_GNSS_UNKNOWN_ERR,           /*	Unknown*/
  
    QAPI_QT_ERR_API_NOT_SUPPORT = __QAPI_ERROR(QAPI_MOD_BASE, 150),
    QAPI_QT_ERR_IVNALID_QT_API,
	/*  end  */
	QAPI_QT_ERR_END,
}QAPI_QT_ERR_NUM;


typedef void * qapi_Device_Info_Hndl_t;
typedef void * qapi_DSS_Hndl_t;
typedef void * qapi_Coap_Session_Hdl_t; /**< CoAP Session Handle. */
typedef  unsigned char      boolean;     /* Boolean value type. */
typedef uint32_t qbool_t;


/*========================================================================
 *  function Definition
 *========================================================================*/
#define CNKTD_MIN(a,b) ((a) < (b) ? (a) : (b))


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_H */


