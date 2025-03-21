#ifndef __ANT_LOG_H__
#define __ANT_LOG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include "ant_typedef.h"

typedef enum {
	LOG_LEVEL_FTL = 0,
	LOG_LEVEL_ERR = 1,
	LOG_LEVEL_WRN = 2,
	LOG_LEVEL_INF = 3,
	LOG_LEVEL_VBS = 4,
	LOG_LEVEL_OFF = 5,
} ant_log_level_e;

#define MAXCHARS		   512

//#define ANT_CONFIG_DEBUG_OFF
//#define ANT_IOTSDK_LOG_NONE

#define ANT_CONFIG_IOTSDK_DEBUG_OFF
#ifdef DEBUG_KAL
#define ANT_CONFIG_USE_CATCHER_LOG
#endif

#ifndef ANT_CONFIG_DEBUG_OFF
#define PAYBOX_LOGI ant_logi
#define PAYBOX_LOGW ant_logw
#define PAYBOX_LOGE ant_loge
#define ANT_LOGD    ant_logd
#define ANT_LOGI    ant_logi
#define ANT_LOGW    ant_logw
#define ANT_LOGE    ant_loge
#define LOGD        ant_logd
#define LOGI        ant_logi
#define LOGW        ant_logw
#define LOGE        ant_loge
#else
#define ANT_KV_log(tag, format, ... )
#define PAYBOX_LOGW(tag, format, ... )
#define PAYBOX_LOGE(tag, format, ... )
#define ANT_LOGD(tag, format, ... )
#define ANT_LOGI(tag, format, ... )
#define ANT_LOGW(tag, format, ... )
#define ANT_LOGE(tag, format, ... )
#define LOGD(tag, format, ... )
#define LOGI(tag, format, ... )
#define LOGW(tag, format, ... )
#define LOGE(tag, format, ... )
#endif
void ant_print(char *s);
void ant_print_hex(ant_u8 *data, int len);
void ant_logd(const char *tag, const char *format, ...);
void ant_logi(const char *tag, const char *format, ...);
void ant_logw(const char *tag, const char *format, ...);
void ant_loge(const char *tag, const char *format, ...);
void ant_log_iot(const char *file, ant_u32 line, int log_level, const char *tag, const char *fmt, va_list ap);
void ant_log_cl(const char *file, ant_u32 line, int log_level, const char *tag, const char *fmt, va_list ap);
void _alipay_iot_log(const char *file_path, uint32_t line, ant_log_level_e level, const char *tag, const char *format, ...);

#define SHORT_FILE								quec_debug_get_short_file_name(__FILE__)
char * quec_debug_get_short_file_name(char * file_name);


/**
 * 设置打印级别
 *
 * @param level 打印级别,参考ant_log_level_e
 *
 * 说明: 1. SDK默认日志级别为LOG_LEVEL_INF
 *       2. 日志接口宏和级别对应关系
 *          LOGD -- LOG_LEVEL_VBS
 *          LOGI -- LOG_LEVEL_INF
 *          LOGW -- LOG_LEVEL_WRN
 *          LOGE -- LOG_LEVEL_ERR
 */
void ant_log_set_level(ant_s32 level);

/**
 * 打开或关闭sdk的日志
 *
 * param @on 1 : 打开sdk的日志(默认)
 *           0 : 关闭sdk的日志
 */
void ant_log_switch(int on);

/**
 * 打开或关闭iotsdk的日志
 *
 * @param enable 1 : 打开iotsdk的日志
 *               0 : 关闭iotsdk的日志(默认)
 */
void ant_log_set_iotlog(int enable);

/**
 * 打开或关闭cl_gui的日志
 *
 * @param enable 1 : 打开cl_gui的日志
 *               0 : 关闭cl_gui的日志(默认)
 */
void ant_log_set_cllog(int enable);

#ifdef __cplusplus
}
#endif
#endif

