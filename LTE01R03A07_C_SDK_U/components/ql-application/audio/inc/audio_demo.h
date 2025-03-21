/**  @file
  audio_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef AUDIO_DEMO_H
#define AUDIO_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_audio.h"
#include "ql_api_common.h"
#include "ql_app_feature_config.h"

/*========================================================================
 *  General Definition
 *========================================================================*/
#define QL_AUDIO_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_AUDIO_LOG(msg, ...)			QL_LOG(QL_AUDIO_LOG_LEVEL, "ql_audio", msg, ##__VA_ARGS__)
#define QL_AUDIO_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_AUDIO", msg, ##__VA_ARGS__)

#if !defined(audio_demo_no_err)
	#define audio_demo_no_err(x, action, str)																	\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_AUDIO_LOG(str);																			\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define RECORD_BUFFER_MAX   (uint)100*1024
#define TEST_PLAY_VOLUME    AUDIOHAL_SPK_VOL_11

#define TEST_STREAM_PACKET_SIZE	(100 * 1024)

#define TEST_PLAY_FILE_NAME   "UFS:test.mp3"
#define TEST_STREAM_FILE_NAME "SD:test.mp3"
#define TEST_STREAM_FORMAT 	  QL_AUDIO_FORMAT_MP3

#define TEST_RECORD_WAV_NAME  "UFS:test_record_wav.wav"
#define TEST_RECORD_PCM_NAME  "UFS:test_record_pcm.pcm"

/* for headset_det */

#define QL_HEADSET_DETECT_PIN              QUEC_PIN_DNAME_GPIO_8
#define QL_HEADSET_DETECT_GPIO             QUEC_GPIO_DNAME_GPIO_8
#define QL_HEADSET_DETECT_FUNC_GPIO        0
#define QL_HEADSET_DETECT_DEBOUNCE_TIME    300    //unit: ms

#define DEMO_HEADSET_DETECT_PLUG_IN          1
#define DEMO_HEADSET_DETECT_PLUG_OUT         0

/*========================================================================
 *  Typedef
 *========================================================================*/
typedef struct
{
	PCM_HANDLE_T recorder;
	PCM_HANDLE_T player;
}ql_demo_poc_t;

/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_audio_app_init(void);

void test_record_play_pcm(void);
void test_play_file(char *file_name);
void test_record_file(void);
void test_record_stream(void);
void test_play_stream(void);
void test_poc_half_duplex(void);
void test_poc_full_duplex(void);
void test_ring_tone(void);
void config_internal_codec_gain(void);
void test_tone_dtmf(void);
int check_audio_format(char *fname);
void test_set_audio_quality(void);

#ifdef QL_APP_FEATURE_EXT_CODEC
void test_codec(void);
ql_audio_errcode_e ql_switch_ext_codec(bool enable);
ql_audio_errcode_e aud_codec_init(void);
#endif

void ql_headset_det_app_init(void);

// only for EC600U
void ql_headset_EC600U_app_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* AUDIO_DEMO_H */


