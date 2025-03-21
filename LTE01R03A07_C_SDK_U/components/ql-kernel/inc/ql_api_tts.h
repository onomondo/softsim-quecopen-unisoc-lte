
/**  @file
  ql_api_tts.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
/**  
  @file
  ql_api_tts.h

  @brief
  This file provides the definitions for tts API functions.

*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/
#ifndef QL_API_TTS_H
#define QL_API_TTS_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS

#define TTS_DIGIT_AUTO      0	//自动检测发音(不推荐,可能不准)
#define TTS_DIGIT_NUMBER    1	//数字数字本身发音(如一二三)
#define TTS_DIGIT_VALUE     2	//数字按照数值发音(如一百二十三)

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Enum
===========================================================================*/
typedef enum
{
    QL_TTS_SUCCESS 						   = QL_SUCCESS,

	QL_TTS_UNKNOWN_ERROR				   = 901 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_INVALID_PARAM				   = 902 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_OPERATION_NOT_SUPPORT		   = 903 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_DEVICE_BUSY					   = 904 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_NO_MEMORY					   = 905 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_RESOURCE_WAIT_MERGE			   = 906 | (QL_COMPONENT_AUDIO_TTS << 16),


    QL_TTS_INIT_ENGINE_ERR                 = 2001 | (QL_COMPONENT_AUDIO_TTS << 16),
	QL_TTS_INIT_SOURCE_ERR				   = 2002 | (QL_COMPONENT_AUDIO_TTS << 16),	
    QL_TTS_START_ERR					   = 2003 | (QL_COMPONENT_AUDIO_TTS << 16),
    QL_TTS_STOP_ERR						   = 2004 | (QL_COMPONENT_AUDIO_TTS << 16),
    QL_TTS_EXIT_ERR						   = 2005 | (QL_COMPONENT_AUDIO_TTS << 16),
    QL_TTS_SOURCE_NOT_EXIST			   	   = 2006 | (QL_COMPONENT_AUDIO_TTS << 16), //tts source not exist, please re-import the TTS resource file: "SFS:quectel_pcm_resource.bin"
    QL_TTS_SOURCE_ERROR				   	   = 2007 |	(QL_COMPONENT_AUDIO_TTS << 16),	//tts source error, please re-import the TTS resource file
    QL_TTS_LIB_EXPIRED					   = 2008 |	(QL_COMPONENT_AUDIO_TTS << 16),	//tts lib too old, please contact us to change the library
    QL_TTS_DEVICE_ID_ERROR				   = 2009 |	(QL_COMPONENT_AUDIO_TTS << 16), //chip id is error  
}ql_errcode_tts_e;

typedef enum
{
	QL_TTS_CONFIG_SPEED = 1,
	QL_TTS_CONFIG_VOLUME,
    QL_TTS_CONFIG_ENCODING,
    QL_TTS_CONFIG_DIGITS,
    QL_TTS_CONFIG_DGAIN,

	QL_TTS_CONFIG_MAX,
}ql_tts_config_e;

typedef enum
{
    QL_TTS_GBK=0,
	QL_TTS_UTF8,
	QL_TTS_UCS2,

	QL_TTS_ENCODE_MAX
}ql_tts_encoding_e;

typedef enum
{
	TTS_RESOURCE_16K_CN = 1,
	TTS_RESOURCE_16K_EN,
	TTS_RESOURCE_8K_CN
}TTS_RESOURCE_E;

typedef enum
{
/*
	tts resource file is in internal flash, should be prepacked as /qsfs/quectel_pcm_resource.bin
	after prepacked done, users can use ql_stat("SFS:quectel_pcm_resource.bin",st) 
	to check if the tts resource file is exist 
*/
	POSIT_INTERNAL_FS = 0,

/*
	tts resource file is in external spi6 flash, should be prepacked as /ext/qsfs/quectel_pcm_resource.bin
	after prepacked done, users can use ql_stat("EFS:qsfs/quectel_pcm_resource.bin",st) 
	to check if the tts resource file is exist 
*/
	POSIT_EFS,

/*
	tts resource file is in external spi4 flash, should be prepacked as /ext4n/qsfs/quectel_pcm_resource.bin
	after prepacked done, users can use ql_stat("EXNSFFS:qsfs/quectel_pcm_resource.bin",st) 
	to check if the tts resource file is exist 
*/
	POSIT_EXNSFFS
}TTS_POSITION_E;

/*===========================================================================
 * Variate
 ===========================================================================*/
typedef struct
{
    int  			ql_tts_volume;  // play volume
    int  			ql_tts_speed; // play speed
    int  			ql_tts_encoding_type;   // utf8 , gbk,ucs2
    int  			ql_tts_running_flag;
    int  			ql_tts_readdigit;//0:TTS_DIGIT_AUTO,	1:TTS_DIGIT_NUMBER, 2:TTS_DIGIT_VALUE
	uint8			language; //TTS_LANGUAGE_CHINESE or TTS_LANGUAGE_ENGLISH
	char 			*prepack_path;
    int             ql_tts_dgain;  // volume gain
}ql_tts_typeconfig_t;

typedef struct
{
	TTS_RESOURCE_E resource;	//chose to use 16k_chinese, 16k english, or 8k chinese reource
	TTS_POSITION_E position;    //chose to prepack tts resource to internal flash or external flash 
	uint8 		   reserve[1];
}tts_param_t;
/*===========================================================================
 * Functions
 ===========================================================================*/
/*****************************************************************
* Function: ql_utf8_to_gbk_str
*
* Description: tts data utf-8 to gbk
* 
* Parameters:

* Return:
* 	0: 		success
    others: ql_errcode_tts_e
*****************************************************************/

ql_errcode_tts_e ql_utf8_to_gbk_str(void * utf8, int inputlen, int * outputlen,void * gbk);

/*****************************************************************
* Function: pUserCallback
*
* Description: tts data callback, pcm data will be sent to this callback 
* 
* Parameters:
* param4: PCM data buffer

* Return:
* 	config value
*****************************************************************/
typedef int (*pUserCallback)(void *param, int param1, int param2, int param3, int data_len, const void *pcm_data);


/*****************************************************************
* Function: ql_tts_get_config_param
*
* Description: get the tts param value
* 
* Parameters:
* type: speed, volume, volume dgain, or encoding, 
		volume range is -32768~32767, and 0 is default; 
		speed range is -32768~32767, and 0 is default
		encoding : QL_TTS_GBK, QL_TTS_UTF8, QL_TTS_UCS2, and QL_TTS_GBK is default
		readdigit : 0:TTS_DIGIT_AUTO,	1:TTS_DIGIT_NUMBER, 2:TTS_DIGIT_VALUE, and TTS_DIGIT_VALUE is default
		volume dgain range is 0~4,and 0 is default
* Return:
* 	config value
*****************************************************************/
int ql_tts_get_config_param(ql_tts_config_e type);

/*****************************************************************
* Function: ql_tts_is_running
*
* Description: get the tts state
* 
* Parameters:
* Return:
* 	0: tts is not running
    1: tts is running
*****************************************************************/
int ql_tts_is_running(void);

/*****************************************************************
* Function: ql_tts_set_config_param
*
* Description: set the tts param value, it will take effect in next play duration
* 
* Parameters:
* type: speed, volume, volume dgain, or encoding, 
		volume range is -32768~32767, and 0 is default; 
		speed range is -32768~32767, and 0 is default
		encoding : QL_TTS_GBK, QL_TTS_UTF8, QL_TTS_UCS2, and QL_TTS_GBK is default
		readdigit : 0:TTS_DIGIT_AUTO,	1:TTS_DIGIT_NUMBER, 2:TTS_DIGIT_VALUE, and TTS_DIGIT_VALUE is default
		volume dgain range is 0~4,and 0 is default
  value: the config value need to be set
* Return:
* 	0: 		success
    others: ql_errcode_tts_e
*****************************************************************/
ql_errcode_tts_e ql_tts_set_config_param(ql_tts_config_e type, int value)
;

/*****************************************************************
* Function: ql_tts_engine_init
*
* Description: init tts, use Chinese 16k tts resource, and tts resource prepacked in internal flash
* 
* Parameters:
* mCallback: play callback, text will be convert to 16-bit pcm data, and the pcm data
			 will be sent to this callback, see descreption of pUserCallback in detail
* Return:
* 	0: 		success
    others: ql_errcode_tts_e

*note: chinese resource need be prepacked in file system firstly
	   if return QL_TTS_INIT_ENGINE_ERR, see if the file "SFS:quectel_pcm_resource.bin"
	   is exist in file system, and if the ram is enough; tts playing need at least 620k ram
*****************************************************************/
ql_errcode_tts_e ql_tts_engine_init(pUserCallback mCallback);

/*****************************************************************
* Function: ql_tts_engine_init_ex
*
* Description: init tts, user can choose tts resource by this api. If not use Chinses 16k reource, or
			   prepacked tts resource in external flash, users need to aujust the prepack json, can see 
			   it in tts_demo.c or tts_demo2.c for detail
* 
* Parameters:
* mCallback: play callback, text will be convert to 16-bit pcm data, and the pcm data
			 will be sent to this callback, see descreption of pUserCallback in detail
* param->resource: choose to use chinese-16k, chinese-8k, or english-16k tts resource, default use chinese-16k 
* param->position: tell the prepack path to kernel, kernel will use the prepack path to initial tts, default 
				   path is internal flash
*	 
* Return:
* 	0: 		success
    others: ql_errcode_tts_e

*note: chinese resource or english resource need be prepacked in file system firstly if not return 0, see 
	   1. if the file "SFS:quectel_pcm_resource.bin"(prepacked to internal flash),or 
	      "EFS:qsfs/quectel_pcm_resource.bin"(prepacked to external spi6 flash)is exist in file system 
	      
	   2. if the ram is enough; tts playing need at least 620k ram if use chinese-16k resource, 
	      need at least 1.45M ram if use english resource , and need at least 570k ram if use chinese-8k resource 	
*****************************************************************/
ql_errcode_tts_e ql_tts_engine_init_ex(pUserCallback mCallback, tts_param_t *param);


/*****************************************************************
* Function: ql_tts_start
*
* Description: start tts convert,encoding support GBK, UCS2, UTF8 now, default GBK
			   before play tts, you should set encoding first
* 
* Parameters:
* textString: the text need to be convert, format should be GBK, UCS2 or UTF8
* textLen: the length of text
* Return:
* 	0: 		success
    others: ql_errcode_tts_e
*****************************************************************/
ql_errcode_tts_e ql_tts_start(const char* textString, unsigned int textLen);

/*****************************************************************
* Function: ql_tts_end
*
* Description: stop tts convert, and release some tts resource
* 
* Parameters:
* Return:
* 	0: 		success
    others: ql_errcode_tts_e
*****************************************************************/
ql_errcode_tts_e ql_tts_end();

/*****************************************************************
* Function: ql_tts_exit
*
* Description: interrupt tts convert
* 
* Parameters:
* Return:
* 	0: 		success
    others: ql_errcode_tts_e
*****************************************************************/
ql_errcode_tts_e ql_tts_exit();


#endif

#ifdef __cplusplus
    } /*"C" */
#endif

#endif /* QL_API_TTS_H*/


