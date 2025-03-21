/**  
  @file
  ql_api_qr_code.h

  @brief
  This file provides the definitions for qr code API functions.

*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_API_DECODER_H
#define QL_API_DECODER_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_DECODER_ERRCODE_BASE (QL_COMPONENT_BSP_DECODER<<16)
    
#define DECODER_INITIAL_FAILED  0
#define DECODER_INITIAL  		1
    
#define DECODER_ERROR			0
#define DECODER_SUCCESS			1
#define DECODER_ERROR_NONE		1

#define QUEC_IMAGE_DECODE_DEBUG  0
/*===========================================================================
 * Enum
===========================================================================*/
typedef enum
{
    QL_DECODER_SUCCESS = QL_SUCCESS,

    QL_DECODER_INIT_ERR                 = 1|QL_COMPONENT_BSP_DECODER,
	QL_DECODER_REPEAT_INIT_ERR,
	QL_DECODER_COPYRIGHT_ERR,
    QL_DECODER_SETMIRROR_ERR,

	QL_DECODER_ERR						= 5|QL_COMPONENT_BSP_DECODER,
    QL_DECODER_MEM_ERR,					
    QL_DECODER_PREVIEW_ERR,
    QL_DECODER_GET_RESULT_ERR,
    QL_DECODER_GET_RESULT_LENGTH_ERR,

    QL_DECODER_DESTROY_ERR				= 10|QL_COMPONENT_BSP_DECODER,
}ql_errcode_decoder_e;

typedef enum
{
    QL_DECODER_TYPE_CODE39 = 0,
    QL_DECODER_TYPE_CODE128,
    QL_DECODER_TYPE_QR_CODE,

    QL_DECODER_TYPE_NONE = 0xff,
}ql_decoder_type_e;

typedef enum
{
    QL_DECODE_NORMAL_MODE = 0,   //正常码解析
    QL_DECODE_MIRROR_MODE,       //镜像码解析
    QL_DECODE_AUTO_MODE,         //正常解析，镜像解析都可以
    
    QL_DECODE_MODE_MAX,
}QL_DECODE_MODE_E;

/*===========================================================================
 * Variate
 ===========================================================================*/

/*===========================================================================
 * Functions
 ===========================================================================*/

/*****************************************************************
* Function: ql_qr_decoder_init
*
* Description: Initialize the decoder
* 
* Parameters:
*
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_qr_decoder_init (void);

/*****************************************************************
* Function: ql_qr_get_decoder_result
*
* Description: get the decode result
* 
* Parameters:
*	*type 		[out]	the type of the code
*   *result     [out]   the result of the code
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_qr_get_decoder_result(ql_decoder_type_e* type, unsigned char * result);

/*****************************************************************
* Function: ql_qr_get_decoder_result_len
*
* Description: get decoder result len
* 
* Parameters:
*   
* Return:
*
*****************************************************************/
unsigned int ql_qr_get_decoder_result_len(void);

/*****************************************************************
* Function: ql_qr_image_decoder
*
* Description: decode the image
* 
* Parameters:
*   *img_buffer     [in]    the image need to be decoded
*   whidth          [in]    the width of the image
*   height          [in]    the height of the image
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_qr_image_decoder (uint16_t *img_buffer, uint32_t width, uint32_t height);

/*****************************************************************
* Function: ql_destroy_decoder
*
* Description: destroy the deocder
* 
* Parameters:
*
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_destroy_decoder(void);

/*****************************************************************
* Function: ql_get_decoder_version
*
* Description: get version of the deocder
* 
* Parameters:
*   *version        [out]   the data of the version
*
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_get_decoder_version(unsigned char* version);

/*****************************************************************
* Function: ql_set_decode_mirror
*
* Description: set decode mode
* 
* Parameters:
*   decode_mode:  QL_DECODE_NORMAL_MODE QL_DECODE_MIRROR_MODE  AUTO   
*
* Return:
* 	ql_errcode_decoder_e 
*
*****************************************************************/
ql_errcode_decoder_e ql_set_decode_mirror(QL_DECODE_MODE_E decode_mode);


#ifdef __cplusplus
    } /*"C" */
#endif

#endif /* QL_API_DECODER_H*/

