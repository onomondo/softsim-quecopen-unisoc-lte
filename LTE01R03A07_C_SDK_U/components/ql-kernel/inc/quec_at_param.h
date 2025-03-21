/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#ifndef _QUEC_AT_PARAM_H_
#define _QUEC_AT_PARAM_H_

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#include "ql_api_common.h"
#include "ql_uart_internal.h"
#include "quec_atc_common.h"

/*************************************************************************
*  							Define
**************************************************************************/
#define AT_BUFFER_SIZE			(3*1024)
#define AT_NAME_LEN_MAX			24		//at名称的最大长度,如"cfun"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HW_IOT_DEVICE_SDK_AT
#define AT_PARMA_LEN_MAX 		2048*2	//at命令的最大总长度
#else
#define AT_PARMA_LEN_MAX 		2048	//at命令的最大总长度
#endif
#define AT_MAX_LEN				(AT_PARMA_LEN_MAX+AT_NAME_LEN_MAX)
#define AT_MAX_PARAM_CNT 		24		//at命令的参数个数
#define AT_LF_TIMEOUT			10		//CR后若无数据,则等 AT_LF_TIMEOUT ms

/*************************************************************************
*  							Enum
**************************************************************************/
typedef enum
{
    AT_PARAM_TYPE_EMPTY,
    AT_PARAM_TYPE_RAW,
    AT_PARAM_TYPE_STRING,
} ql_at_param_type_e;

typedef enum
{
	QUEC_AT_CONSTRAIN_MIN	= 100,
	QUEC_AT_CONSTRAIN_CJSON,

	QUEC_AT_CONSTRAIN_MAX
} QUEC_AT_CONSTRAIN_E;

/*************************************************************************
*  							Struct
**************************************************************************/
typedef struct ql_at_cmd ql_at_cmd_t;
typedef void (*async_ctx_destory)(struct ql_at_cmd *cmd);
typedef void (*quec_at_timeout_cb_t)(ql_at_cmd_t *cmd);
typedef void (*quec_at_cb_t)(void *ctx);

typedef struct ql_at_desc
{
    char *name;                    		 	///< AT command name, such as "+IPR"
    void (*handler)(struct ql_at_cmd*); 	///< At command handler
    uint32_t constrains;                 	///< AT command constrains
} ql_at_desc_t;

typedef struct
{
    uint8_t type;    	///< parameter type, used by AT engine internally
    uint16_t length; 	///< value length
    char *value;   		///< value, the real size is variable
} ql_at_param_t;

typedef struct
{
	quec_at_cb_t  cb;
	void		 *ctx;
}ql_at_cb_param_t;

typedef struct ql_at_cmd
{
	ql_dispatch_t		*engine;	 //当前AT端口的dispatch(等同于dev,作用为兼容8910)
	quec_dev_e			dev;         //当前端口的端口号
	
	uint8				param_count; //当前AT的参数个数
	atCmdType_t			type;        //AT命令的类型
	ql_at_desc_t		*desc;		 //AT命令信息
	bool 				resp_flag;   //当前AT是否已经进行过AT回复
	bool 				err_flag;	 //当前AT在执行过程中是否遇到错误
	ql_at_param_t		*params[AT_MAX_PARAM_CNT]; //参数
	uint16_t 			uti;
    void 				*async_ctx;
    async_ctx_destory 	async_ctx_destroy;
} ql_at_cmd_t;

typedef struct
{
    uint32_t minval; ///< minimal value
    uint32_t maxval; ///< maximum value
} quec_UintRange_t;

typedef struct
{
    int minval; ///< minimal value
    int maxval; ///< maximum value
} quec_IntRange_t;

typedef struct
{
    uint64_t minval; ///< minimal value
    uint64_t maxval; ///< maximum value
} quec_Uint64Range_t;

/**
 * data structure to define a signed 64bits integer range
 */
typedef struct
{
    int64_t minval; ///< minimal value
    int64_t maxval; ///< maximum value
} quec_Int64Range_t;

typedef struct
{
    uint32_t value;  ///< integer value
    const char *str; ///< string value
} quec_ValueStrMap_t;


/*************************************************************************
*  							Function
**************************************************************************/
/**
 * extract uint parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 */
uint32_t quec_atParamUint(ql_at_param_t *param, bool *paramok);

/**
 * extract optional uint parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUint.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 */

uint32_t quec_atParamDefUint(ql_at_param_t *param, uint32_t defval, bool *paramok);

/**
 * extract uint parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in range
 */
uint32_t quec_atParamUintInRange(ql_at_param_t *param, uint32_t minval, uint32_t maxval, bool *paramok);

/**
 * extract optional uint parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInRange. The \a defval is not required in the
 * range.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in range
 */
uint32_t quec_atParamDefUintInRange(ql_at_param_t *param, uint32_t defval, uint32_t minval, uint32_t maxval, bool *paramok);

/**
 * extract uint parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in list
 */
uint32_t quec_atParamUintInList(ql_at_param_t *param, const uint32_t *list, unsigned count, bool *paramok);

/**
 * extract optional uint parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInList. The \a defval is not required in the
 * list.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in list
 */
uint32_t quec_atParamDefUintInList(ql_at_param_t *param, uint32_t defval, const uint32_t *list, unsigned count, bool *paramok);

/**
 * extract int parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 */
int quec_atParamInt(ql_at_param_t *param, bool *paramok);

/**
 * extract optional int parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUint.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 */
int quec_atParamDefInt(ql_at_param_t *param, int defval, bool *paramok);

/**
 * extract int parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in range
 */
int quec_atParamIntInRange(ql_at_param_t *param, int minval, int maxval, bool *paramok);

/**
 * extract optional int parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInRange. The \a defval is not required in the
 * range.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in range
 */
int quec_atParamDefIntInRange(ql_at_param_t *param, int defval, int minval, int maxval, bool *paramok);

/**
 * extract int parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in list
 */
int quec_atParamIntInList(ql_at_param_t *param, const int *list, unsigned count, bool *paramok);

/**
 * extract optional int parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInList. The \a defval is not required in the
 * list.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in list
 */
int quec_atParamDefIntInList(ql_at_param_t *param, int defval, const int *list, unsigned count, bool *paramok);

/**
 * extract uint64 parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 */
uint64_t quec_atParamUint64(ql_at_param_t *param, bool *paramok);

/**
 * extract optional uint64 parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUint.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 */
uint64_t quec_atParamDefUint64(ql_at_param_t *param, uint64_t defval, bool *paramok);

/**
 * extract uint64 parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in range
 */
uint64_t quec_atParamUint64InRange(ql_at_param_t *param, uint64_t minval, uint64_t maxval, bool *paramok);

/**
 * extract optional uint64 parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInRange. The \a defval is not required in the
 * range.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in range
 */
uint64_t quec_atParamDefUint64InRange(ql_at_param_t *param, uint64_t defval, uint64_t minval, uint64_t maxval, bool *paramok);

/**
 * extract uint64 parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in list
 */
uint64_t quec_atParamUint64InList(ql_at_param_t *param, const uint64_t *list, unsigned count, bool *paramok);

/**
 * extract optional uint64 parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInList. The \a defval is not required in the
 * list.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in list
 */
uint64_t quec_atParamDefUint64InList(ql_at_param_t *param, uint64_t defval, const uint64_t *list, unsigned count, bool *paramok);

/**
 * extract int64 parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 */
int64_t quec_atParamInt64(ql_at_param_t *param, bool *paramok);

/**
 * extract optional int64 parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUint.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 */
int64_t quec_atParamDefInt64(ql_at_param_t *param, int64_t defval, bool *paramok);

/**
 * extract int64 parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in range
 */
int64_t quec_atParamInt64InRange(ql_at_param_t *param, int64_t minval, int64_t maxval, bool *paramok);

/**
 * extract optional int64 parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInRange. The \a defval is not required in the
 * range.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in range
 */
int64_t quec_atParamDefInt64InRange(ql_at_param_t *param, int64_t defval, int64_t minval, int64_t maxval, bool *paramok);

/**
 * extract int64 parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in list
 */
int64_t quec_atParamInt64InList(ql_at_param_t *param, const int64_t *list, unsigned count, bool *paramok);

/**
 * extract optional int64 parameter, and check in list
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintInList. The \a defval is not required in the
 * list.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param list      array of valid values
 * @param count     valid value count
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - int parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is not integer number
 *          - \a param is not in list
 */
int64_t quec_atParamDefInt64InList(ql_at_param_t *param, int64_t defval, const int64_t *list, unsigned count, bool *paramok);

/**
 * extract string parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * String parameter is parameter started and ended with double quotation.
 * The escape defined in V.250 is \\HH. For example, \30 for '0'.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * The output pointer will be valid till the parameter itself is deleted.
 *
 * The output string is ended with null byte for end of string.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not string (start and end with double quotation)
 */
const char *quec_atParamStr(ql_at_param_t *param, bool *paramok);

/**
 * extract hex or str parameter from raw text
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * Raw text parameter is parameter started and ended without double quotation.
 * The escape defined in V.250 is \\HH. For example, \30 for '0'.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * The output pointer will be valid till the parameter itself is deleted.
 *
 * The output string is ended with null byte for end of string.
 *
 * @param param     parameter pointer
 * @param len     the length of the parameter
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is string (start and end with double quotation)
 */
const char *quec_atParamHexData(ql_at_param_t *param, uint32_t len, bool *paramok);

/**
 * extract hex parameter from raw text
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * Raw text parameter is parameter started and ended without double quotation.
 * The escape defined in V.250 is \\HH. For example, \30 for '0'.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * The output pointer will be valid till the parameter itself is deleted.
 *
 * The output string is ended with null byte for end of string.
 *
 * @param param     parameter pointer
 * @param len     the length of the parameter
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is string (start and end with double quotation)
 */
const char *quec_atParamHexRawText(ql_at_param_t *param, uint32_t len, bool *paramok);
/**
 * extract string parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * String parameter is parameter started and ended with double quotation.
 * The escape defined in V.250 is \\HH. For example, \30 for '0'.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * The output pointer will be valid till the parameter itself is deleted.
 *
 * The output string is ended with null byte for end of string.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not string (start and end with double quotation)
 */
const char *quec_atParamOptStr(ql_at_param_t *param, bool *paramok);
/**
 * extract optional string parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamStr.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is not string (start and end with double quotation)
 */
const char *quec_atParamDefStr(ql_at_param_t *param, const char *defval, bool *paramok);

/**
 * extract raw parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * The raw parameter is not parsed. For example, the double quotation of
 * string parameter will be kept, and the escape in string parameter is
 * untouched.
 *
 * The output string is ended with null byte as end of string.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 */
const char *quec_atParamRawText(ql_at_param_t *param, bool *paramok);

/**
 * extract floating point parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - floating point parameter
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not double
 */
double quec_atParamDouble(ql_at_param_t *param, bool *paramok);

/**
 * extract DTMF parameter
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * There are 2 kinds of DTMF parameter:
 * - one DTMF character without double quotation
 * - multiple DTMF chaeacters with double quotation
 * In both case, the return value is DTMF character string with null byte
 * as the string end.
 *
 * It will not check whether DTMF characters inside the string is valid
 * DTMF character. Application should validate them.
 *
 * It is possible that the internal storage will be changed during this
 * function call. So, don't call other parameter APIs for the same
 * parameter after it is called. However, this function itself can be
 * called again.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - parsed DTMF string
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not DTMF
 */
const char *quec_atParamDtmf(ql_at_param_t *param, bool *paramok);

/**
 * extract string parameter and return mapped uint
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * It is just a wrapper to:
 * - extract string parameter
 * - map to uint
 *
 * @param param     parameter pointer
 * @param vsmap     integer/string map, ended with NULL string value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not in map
 */
uint32_t quec_atParamUintByStrMap(ql_at_param_t *param, const quec_ValueStrMap_t *vsmap, bool *paramok);

/**
 * extract optional string parameter and return mapped uint
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * When \a param is empty, \a defval is returned. Otherwise, it is the
 * same as \a quec_atParamUintByStrMap. The \a defval is not required in the
 * map.
 *
 * @param param     parameter pointer
 * @param defval    default value
 * @param vsmap     integer/string map, ended with NULL string value
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is not in map
 */
uint32_t quec_atParamDefUintByStrMap(ql_at_param_t *param, uint32_t defval, const quec_ValueStrMap_t *vsmap, bool *paramok);

/**
 * extract uint parameter by hex string
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * The valid parameter is the output of:
 * \code{.cpp}
 * printf("\"%x\"", value)
 * \endcode
 *
 * For example, parameter "12345" will be parsed as 0x12345.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - NULL on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not the needed format
 */
uint32_t quec_atParamHexStrUint(ql_at_param_t *param, bool *paramok);

/**
 * trim tailing characters at the end of parameter
 *
 * This shall be called before the parameter isn't parsed. \p quec_atParamRawText
 * won't parse the parameter, so it is safe to call it after \p quec_atParamRawText.
 *
 * It will change the internal storage of parameter directly. So, only call
 * it when absolutely needed, and you know what you are doing.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - true on success
 *      - false on failed
 *          - \p param type is not raw text
 *          - the length of parameter is shorter than \p len
 */
bool quec_atParamTrimTail(ql_at_param_t *param, uint32_t len);

/**
 * check whether the parameter is empty
 *
 * Both not specified and skipped parameter are empty. For example:
 * - AT+CMD=123
 * - AT+CMD=123,
 * - AT+CMD=123,,456
 *
 * In all of the above cases, params[1] is empty.
 *
 * @param param     parameter pointer
 * @return
 *      - true if the parameter is empty
 *      - false otherwise
 */
bool quec_atParamIsEmpty(ql_at_param_t *param);

/**
 * extract hex int64_t parameter, and DO NOT check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 */
int64_t quec_atParamHexInt64(ql_at_param_t *param, bool *paramok);

/**
 * extract hex int64_t parameter, and check range
 *
 * When \a *paramok is false, it will do nothing. On failure, \a *param
 * will be set to false on return.
 *
 * @param param     parameter pointer
 * @param minval    minimum valid value, inclusive
 * @param maxval    maximum valid value, inclusive
 * @param paramok   in/out parameter parsing ok flag
 * @return
 *      - uint parameter
 *      - 0 on failed
 *          - \a *paramok is false at input
 *          - \a param is empty
 *          - \a param is not integer number
 *          - \a param is not in range
 */
int64_t quec_atParamHexInt64InRange(ql_at_param_t *param, int64_t minval, int64_t maxval, bool *paramok);

#endif

