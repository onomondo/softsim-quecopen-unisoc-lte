/**  @file
  audio_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef TTS_DEMO_H
#define TTS_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#include "ql_audio.h"
#include "ql_api_tts.h"

#define QL_TTS_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_TTS_LOG(msg, ...)			QL_LOG(QL_TTS_LOG_LEVEL, "ql_app_tts", msg, ##__VA_ARGS__)
#define QL_TTS_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_app_tts", msg, ##__VA_ARGS__)

#if !defined(tts_demo_no_err)
	#define tts_demo_no_err(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_TTS_LOG(str);																			\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_TTS_TASK_PRIO     25
#define QL_TTS_TASK_STACK    8*1024

#define QUEC_16K_FRAME_LEN 640 
#define QUEC_8K_FRAME_LEN  320
#define QUEC_BEFORE_SAMPLING  16000
#define QUEC_AFTER_SAMPLING   8000

typedef struct osi_buff
{
    unsigned size;
    unsigned rd;
    unsigned wr;
    char data[];
}osi_buff_t;

typedef struct
{
	char  	   	   	*input_buffer;
	char  	   	   	*output_buffer;
	osi_buff_t  	*pipe;	
	ql_resampler    *filter;
	PCM_HANDLE_T 	pcm;
}tts_loacl_info_t;
/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_tts_demo_init(void);
int ql_tts_init(pUserCallback mCallback);//TTS 初始化
int ql_tts_deinit(void);//TTS去初始化，释放资源
int ql_tts_play(ql_tts_encoding_e encoding, uint8 digit_type, bool interrupt, const char *string, uint len);
void ql_tts_stop_all(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* AUDIO_DEMO_H */


