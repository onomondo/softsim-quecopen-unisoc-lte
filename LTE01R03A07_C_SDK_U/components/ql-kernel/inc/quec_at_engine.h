/**  @file
  quec_at_engine.h

  @brief
  This file is used to define at command engine functions for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.
  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------



------------     -------     -------------------------------------------------------------------------------
=================================================================*/

#ifndef QUEC_AT_ENGINE_H
#define QUEC_AT_ENGINE_H

#include "at_cfg.h"
#include "quec_proj_config.h"
#include "ql_uart_internal.h"
#include "quec_at_param.h"
#include "quec_atresp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
*  							Macro
**************************************************************************/
#define DECLARE_CMD_HANDLER(h)  	 extern void h(ql_at_cmd_t *)
#define QUEC_AT_CONFIG_ALL_CHANNEL   1

#define QUEC_AT_VIRT_PORT_CNT		 10
#define QUEC_VIRT_AT_PIPE_LEN  		 1024

extern atSetting_t gAtSetting;
/*========================================================================
 *  general Definition
 *========================================================================*/
#define QUEC_ATC_S3		gAtSetting.chsetting.s3
#define QUEC_ATC_S4		gAtSetting.chsetting.s4
#define QUEC_ATC_ATV	gAtSetting.chsetting.atv
#define QUEC_ATC_ATQ	gAtSetting.chsetting.atq
#define QUEC_ATC_ATE	gAtSetting.chsetting.ate
#define QUEC_ATC_CMEE	gAtSetting.chsetting.cmee
#define QUEC_ATC_CRC	gAtSetting.chsetting.crc


#define QUEC_CHAR_CR 				QUEC_ATC_S3
#define QUEC_CHAR_LF 				QUEC_ATC_S4
#define QUEC_CHAR_BACKSPACE  		0x8
#define QUEC_CHAR_DOT				','
#define QUEC_CHAR_QUOT				'"'
#define QUEC_CHAR_EQUA				'='
#define QUEC_CHAR_QUES				'?'
#define QUEC_CHAR_AND				'&'
#define QUEC_CHAR_END				'\0'
#define QUEC_CHAR_SEMI				';'
#define QUEC_CHAR_SPACE				' '
#define QUEC_CHAR_BRACE_LEFT		'{'
#define QUEC_CHAR_BRACE_RIGHT		'}'
#define AT_EXTENDED_PREFIXES 		"+*$%^"
#define AT_EXTENDED_PREFIXES_LETTER "s"             //这些字符开头，当成AT指令，不当成参数


#define AT_BASIC_PREFIXES 	 		"adehilmoqvwxz"	//这些AT均只有一个字节的名称,且只有一个参数,参数为AT_BASIC_PARAM_SUPPORT中支持的值
#define AT_BASIC_PARAM_RANGE		"0123456789*#"	//基础型AT所支持的非字母参数
#define AT_BASIC_PARAM_CK(chr)		(strchr(AT_BASIC_PARAM_RANGE, chr) || (isalpha(chr) && !strchr(AT_BASIC_PREFIXES,tolower(chr)) && !strchr(AT_EXTENDED_PREFIXES_LETTER ,tolower(chr))))

typedef enum
{
	QUEC_AT_REAL_PORT_MIN	= 0,
	QUEC_AT_REAL_PORT_MAX 	= DEV_MAX,

	QUEC_AT_VIRT_PORT_MIN,
	QUEC_AT_VIRT_PORT_MAX 	= QUEC_AT_VIRT_PORT_MIN + QUEC_AT_VIRT_PORT_CNT, //虚拟通道

	QUEC_AT_PORT_MAX
}quec_at_port_e;

typedef enum
{
    QUEC_CHSETTING_CRC = 0,
    QUEC_CHSETTING_CHSETTING, 
    QUEC_CHSETTING_CMEE,
    QUEC_CHSETTING_ATE,
    QUEC_CHSETTING_ATV,
    QUEC_CHSETTING_ATQ,
    QUEC_CHSETTING_ATX,
    QUEC_CHSETTING_S3,
    QUEC_CHSETTING_S4,
    QUEC_CHSETTING_S5,
    QUEC_CHSETTING_ANDD,
    QUEC_CHSETTING_ANDC,
}quec_at_chsetting_e; 

enum
{
    AT_CONSTRAINT_NONE = 0,           ///< no constrain
    AT_CON_CALIB_MODE = (1 << 0),     ///< only in calibration mode, not in normal mode
    AT_CON_NOT_CALIB_MODE = (1 << 1), ///< only in normal mode, not in calibration mode
    AT_CON_NOT_CLAC = (1 << 2),       ///< ignore in CLAC
};

/*************************************************************************
*  							Enum
**************************************************************************/


/*************************************************************************
*  							Struct
**************************************************************************/
void quec_at_set_all_chsetting(quec_at_chsetting_e type, void *data, int len);

void quec_app_at_add(const ql_at_desc_t *desc, size_t list_length);

atChannelSetting_t *quec_at_get_chsetting(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_AT_ENGINE_H */






