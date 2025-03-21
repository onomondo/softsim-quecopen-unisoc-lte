/**  @file
  quec_log.h

  @brief
  This file is used to define version information for different Quectel Project.

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

	
#ifndef QUEC_LOG_H
#define QUEC_LOG_H
	
#include "osi_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QUEC_LOG_LEVEL OSI_LOG_LEVEL_INFO
#define QUEC_LOG_TAG                  OSI_MAKE_LOG_TAG('Q', 'U', 'E', 'C')
#define QUEC_LOG_TAG_QVOICECALL	      OSI_MAKE_LOG_TAG('Q', 'V', 'C', 'A')
#define QUEC_LOG_TAG_QATNW		        OSI_MAKE_LOG_TAG('Q', 'A', 'N', 'W')
#define QUEC_LOG_TAG_QPBK		          OSI_MAKE_LOG_TAG('Q', 'P', 'B', 'K')
#define QUEC_LOG_TAG_QCFG		          OSI_MAKE_LOG_TAG('Q', 'C', 'F', 'G')
#define QUEC_LOG_TAG_QATSIM		        OSI_MAKE_LOG_TAG('Q', 'S', 'I', 'M')
#define QUEC_LOG_TAG_MODEM	          OSI_MAKE_LOG_TAG('Q', 'M', 'D', 'M')
#define QUEC_LOG_TAG_SDMMC		        OSI_MAKE_LOG_TAG('Q', 'S', 'D', 'M')
#define QUEC_LOG_TAG_AUDIO		        OSI_MAKE_LOG_TAG('Q', 'A', 'D', 'O')

#define QUEC_LOG_TAG_GNSS		          OSI_MAKE_LOG_TAG('Q', 'G', 'N', 'S')
#define QUEC_LOG_TAG_OSI		          OSI_MAKE_LOG_TAG('Q', 'O', 'S', 'I')
#define QUEC_LOG_TAG_QXRTK		        OSI_MAKE_LOG_TAG('Q', 'X', 'R', 'T')
#define QUEC_LOG_TAG_CAMERA		        OSI_MAKE_LOG_TAG('Q', 'C', 'A', 'M')

#define QUEC_LOG_TAG_FTP		     OSI_MAKE_LOG_TAG('Q', 'F', 'T', 'P')
#define QUEC_LOG_TAG_HTTP		     OSI_MAKE_LOG_TAG('Q', 'H', 'T', 'T')
#define QUEC_LOG_TAG_MQTT		     OSI_MAKE_LOG_TAG('Q', 'M', 'Q', 'T')
#define QUEC_LOG_TAG_SMTP		     OSI_MAKE_LOG_TAG('Q', 'S', 'M', 'T')
#define QUEC_LOG_TAG_ZIP		     OSI_MAKE_LOG_TAG('Q', 'Z', 'I', 'P')
#define QUEC_LOG_TAG_MMS		     OSI_MAKE_LOG_TAG('Q', 'M', 'M', 'S')
#define QUEC_LOG_TAG_NTP		     OSI_MAKE_LOG_TAG('Q', 'N', 'T', 'P')
#define QUEC_LOG_TAG_PING		     OSI_MAKE_LOG_TAG('Q', 'P', 'I', 'N')
#define QUEC_LOG_TAG_SNMP		     OSI_MAKE_LOG_TAG('Q', 'S', 'N', 'M')
#define QUEC_LOG_TAG_FILE		     OSI_MAKE_LOG_TAG('Q', 'F', 'I', 'L')
#define QUEC_LOG_TAG_SECURTY		 OSI_MAKE_LOG_TAG('Q', 'S', 'E', 'C')
#define QUEC_LOG_TAG_XLAT		     OSI_MAKE_LOG_TAG('Q', 'L', 'A', 'T')

#define QUEC_LOG_TAG_ALIYUN_IOT_SMARTCARD     OSI_MAKE_LOG_TAG('Q', 'A', 'L', 'S')
#define QUEC_LOG_TAG_ALIPAY_IOT               OSI_MAKE_LOG_TAG('Q', 'A', 'L', 'P')
#define QUEC_LOG_TAG_FIREWALL                 OSI_MAKE_LOG_TAG('Q', 'F', 'I', 'W')
#define QUEC_LOG_TAG_HW_IOT_DEVICE            OSI_MAKE_LOG_TAG('Q', 'H', 'W', 'D')
#define QUEC_LOG_TAG_OHOS                     OSI_MAKE_LOG_TAG('Q', 'O', 'H', 'S')
#define QUEC_LOG_TAG_TCPIP                    OSI_MAKE_LOG_TAG('Q', 'T', 'I', 'P')
#define QUEC_LOG_TAG_LWM2M                    OSI_MAKE_LOG_TAG('Q', 'M', '2', 'M')
#define QUEC_LOG_TAG_CTSREG                   OSI_MAKE_LOG_TAG('Q', 'C', 'T', 'R')
#define QUEC_LOG_TAG_LBS                      OSI_MAKE_LOG_TAG('Q', 'L', 'B', 'S')
#define QUEC_LOG_TAG_BT_BLE		     OSI_MAKE_LOG_TAG('Q', 'B', 'T', 'E')
#define QUEC_LOG_TAG_BLE_GATT		 OSI_MAKE_LOG_TAG('Q', 'A', 'T', 'T')
#define QUEC_LOG_TAG_BT_HFP		     OSI_MAKE_LOG_TAG('Q', 'H', 'F', 'P')
#define QUEC_LOG_TAG_BT_SPP		     OSI_MAKE_LOG_TAG('Q', 'S', 'P', 'P')
#define QUEC_LOG_TAG_BT_A2DP_AVRCP   OSI_MAKE_LOG_TAG('Q', 'A', 'A', 'C')

#define QUEC_LOG_TAG_POWER		        OSI_MAKE_LOG_TAG('Q', 'P', 'W', 'R')
#define QUEC_LOG_TAG_URC		        OSI_MAKE_LOG_TAG('Q', 'U', 'R', 'C')
#define QUEC_LOG_TAG_NOR_FLASH		    OSI_MAKE_LOG_TAG('Q', 'N', 'O', 'F')
#define QUEC_LOG_TAG_NAND_FLASH		    OSI_MAKE_LOG_TAG('Q', 'N', 'A', 'F')
#define QUEC_LOG_TAG_PWM		        OSI_MAKE_LOG_TAG('Q', 'P', 'W', 'M')
#define QUEC_LOG_TAG_CMUX		        OSI_MAKE_LOG_TAG('Q', 'C', 'M', 'X')
#define QUEC_LOG_TAG_GPIO	        	OSI_MAKE_LOG_TAG('Q', 'G', 'I', 'O')
#define QUEC_LOG_TAG_LED	        	OSI_MAKE_LOG_TAG('Q', 'L', 'E', 'D')
#define QUEC_LOG_TAG_TTS		        OSI_MAKE_LOG_TAG('Q', 'T', 'T', 'S')


#ifndef _THIS_IP_
//获取当前函数的地址
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
#endif
extern bool ql_quec_trace_is_enabled(void);

#define QUEC_LOG_PRINTF_TAG(level, tag, fmt, ...)                                        \
    do                                                                          \
    {                                                                           \
        if (QUEC_LOG_LEVEL >= level && ql_quec_trace_is_enabled()) 				\
            osiTracePrintf((level << 28) | (tag), fmt, ##__VA_ARGS__); \
    } while (0)

#define QUEC_LOG_PRINTF(level, fmt, ...)     QUEC_LOG_PRINTF_TAG(level, QUEC_LOG_TAG, fmt, ##__VA_ARGS__)


//log打印加上函数名和行号
//#define custom_log(user_str, msg, ...)				QUEC_LOG_PRINTF(QUEC_LOG_LEVEL, "%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
//#define custom_log_level(level, user_str, msg, ...)	QUEC_LOG_PRINTF(level, "%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)

//log打印加上函数地址和行号
#define custom_log(user_str, msg, ...)							QUEC_LOG_PRINTF_TAG(QUEC_LOG_LEVEL, QUEC_LOG_TAG, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#define custom_log_level(level, user_str, msg, ...)				QUEC_LOG_PRINTF_TAG(level, QUEC_LOG_TAG, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)

#define custom_log_tag(user_str, tag, msg, ...)					QUEC_LOG_PRINTF_TAG(QUEC_LOG_LEVEL, tag, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#define custom_log_level_tag(level, tag, user_str, msg, ...)	QUEC_LOG_PRINTF_TAG(level, tag, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)

//log打印加上函数地址
//#define custom_log(user_str, msg, ...)				QUEC_LOG_PRINTF(QUEC_LOG_LEVEL, "%p "msg"", _THIS_IP_, ##__VA_ARGS__)
//#define custom_log_level(level, user_str, msg, ...)	QUEC_LOG_PRINTF(level, "%p "msg"", _THIS_IP_, ##__VA_ARGS__)

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_LOG_H */

