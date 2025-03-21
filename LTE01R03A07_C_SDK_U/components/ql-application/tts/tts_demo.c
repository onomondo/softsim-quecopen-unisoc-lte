/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

/************************************************************************************************************
  用户如需将TTS库放到内核，则在target.config中将 CONFIG_QUEC_PROJECT_FEATURE_TTS_IN_KERNEL 置为 y,
  并修改分区，将内核分区增大250k左右，其余的内容不变
*************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_app_feature_config.h"
#include "tts_demo.h"
#include "ql_api_osi.h"
#include "ql_api_tts.h"
#include "ql_log.h"
#include "ql_osi_def.h"
#include "ql_fs.h"
#include "ql_api_dev.h"

/* 
	1. 不同的TTS资源文件，对应的播放效果不同。其中中文资源不能用来播英文单词，单词会以字母的方式播出; 英文资源也不能用来播中文。默认使用
	   中文16k TTS资源

	2. 若使用16k中文TTS资源，且TTS资源文件预置到内置flash中，则不需要修改json脚本(脚本默认已选择预置16k中文资源，且预置到内置flash)，只需要调用
	   ql_tts_engine_init函数即可完成初始化，不需要关注以下描述
	
	3. 所有的资源文件均在components\ql-config\download\prepack下，其中:
	   英文16k资源文件名为: "quectel_tts_resource_english_16k.bin"
	   中文8k资源文件为："quectel_tts_resource_chinese_8k.bin"
	   中文16k资源文件为："quectel_pcm_resource.bin"

	4. 预置文件时，请将json脚本中的"file"固定为"/qsfs/quectel_pcm_resource.bin"(预置资源文件到内置flash), 或
	   "/ext/qsfs/quectel_pcm_resource.bin"(预置到外置6线spi flash中), 并修改"local_file"来选择上传哪个资源文件，如下述示例.
	   若不使用中文16k资源，则需要使用"ql_tts_engine_init_ex"函数，将配置结构体中的"resource"变量设置为需要使用的资源;
	   若将资源文件预置到外置6线spi flash，需要将"position"变量设置为 POSIT_EFS

	   当TTS资源文件预置在内置Flash时，针对需要FOTA升级的情况，新版本SDK中默认将该文件进行拆分为多个子文件进行预置！
	   外置存储时可以不用拆分。
	   
	5. 使用英文16k TTS资源播放时，需要1.45M的RAM空间，因此要注意RAM空间是否充足； 选择中文16k TTS资源文件时，需要620k的RAM空间； 选择中文
	   8k资源时，需要570kRAM空间


预置文件示例：
	1. 预置16k中文TTS资源文件到内部flash(默认):
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_pcm_resource.bin"
			}
		]
	2. 预置16k英文TTS资源文件到内部flash(以"/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_english_16k.bin"
			}
		]
	3. 预置8K中文TTS资源文件到内部flash(以"/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_chinese_8k.bin"
			}
		]
	4.  (1)预置16k英文TTS资源到外置6线spi flash(以/ext/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/ext/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_english_16k.bin"
			}
		]
		(2) 8910: 把boot_fdl_dnld.c(8850上为fdl.c)文件的fdlDnldStart函数中, spi6部分的 #if 0改为 #if 1
			8850: 把fdl.c(8850上为fdl.c)文件的prvDnldStart函数中, spi6部分的 #if 0改为 #if 1
		(3)在target.config中，CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR打开，CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS关闭
		
	5.  (1)预置16k中文TTS资源文件到外置4线flash
		"files": [
			{
				"file": "/ext4n/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_pcm_resource.bin"
			}
		]
		(2)需要把boot_fdl_dnld.c文件的bool fdlDnldStart(fdlEngine_t *fdl, unsigned devtype)，
			4线flash部分的#if 0打开为1（CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS部分）;
		(3)在target.config中，CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS打开，CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR关闭
	6.  不要随意删除文件系统里的预置TTS资源
*/

//QL_TTS_RESAMPLE_TEST宏控开启用于展示如何使用重采样相关函数
//重采样就是把采样率进行重新调整，比如tts为16k资源，由于语音通话中只支持8k采样率播放，因此，如果需要
//在通话过程中给对端播TTS必须将采样率进行重采样成8k的，才能正常播放
#define QL_TTS_RESAMPLE_TEST				0

//使用英文TTS资源时，需要同步修改预置文件，如上述预置文件示例第4条
#define	QL_TTS_LANGUAGE_USE_ENGLISH			0

/*0:tts库在内置flash，1:tts库在六线flash，2：tts库在四线flash*/
#define QL_TTS_LOCATION   0

typedef struct
{
	ql_tts_encoding_e encode;		//字符编码
	uint8			  digit_type;
	char 			  *str;			//播放的内容
	uint			  len;			//播放的长度
	bool			  interrupt;	//如果正在播放TTS,是否打断播放当前的TTS
}tts_demo_play_info_t;

typedef enum
{
	DIGIT_AS_VALUE,	//按数值发音(如一百二十三)
	DIGIT_AS_NUMBER	//按数字发音(如一二三)
}digit_play_e;

/*===========================================================================
 * Variate
 ===========================================================================*/
#ifdef QL_APP_FEATURE_AUDIO
PCM_HANDLE_T ql_player = NULL;
ql_task_t  ql_tts_demo_task = NULL;
ql_queue_t ql_tts_demo_queue = NULL;

#if QL_TTS_RESAMPLE_TEST
static tts_loacl_info_t tts_loacl_info = {0};
#endif

/*===========================================================================
 * Functions
 ===========================================================================*/
#if QL_TTS_RESAMPLE_TEST
osi_buff_t *osibuffCreate(unsigned size)
{
    if (size == 0)
        return NULL;

    osi_buff_t *pipe = (osi_buff_t *)calloc(1, sizeof(osi_buff_t) + size);
    if (pipe == NULL)
        return NULL;

    pipe->size = size;
	pipe->rd = 0;
	pipe->wr = 0;
    return pipe;
}

int osibuffWrite(osi_buff_t *pipe, const void *buf, unsigned size)
{
    if (size == 0)
        return 0;
    if (pipe == NULL || buf == NULL)
        return -1;

    uint32_t critical = ql_rtos_enter_critical();
    unsigned space = pipe->size - (pipe->wr - pipe->rd);
    unsigned len = OSI_MIN(unsigned, size, space);
    unsigned wr = pipe->wr;

    if (len == 0)
    {
        ql_rtos_exit_critical(critical);
        return 0;
    }

    unsigned offset = wr % pipe->size;
    unsigned tail = pipe->size - offset;
    if (tail >= len)
    {
        memcpy(&pipe->data[offset], buf, len);
    }
    else
    {
        memcpy(&pipe->data[offset], buf, tail);
        memcpy(pipe->data, (const char *)buf + tail, len - tail);
    }

    pipe->wr += len;
    ql_rtos_exit_critical(critical);

    return len;
}

int osibuffWriteAll(osi_buff_t *pipe, const void *buf, unsigned size, unsigned timeout)
{
    if (size == 0)
        return 0;
    if (pipe == NULL || buf == NULL)
        return -1;

    unsigned len = 0;
    for (;;)
    {
        int bytes = osibuffWrite(pipe, buf, size);
        if (bytes < 0)
            return -1;

        len += bytes;
        size -= bytes;
        buf = (const char *)buf + bytes;
        if (size == 0 )
            break;
    }
    return len;
}

int osibuffReadAvail(osi_buff_t *pipe)
{
    if (pipe == NULL)
        return -1;

    uint32_t critical = ql_rtos_enter_critical();
    unsigned bytes = pipe->wr - pipe->rd;
    ql_rtos_exit_critical(critical);
    return bytes;
}

int osibuffRead(osi_buff_t *pipe, void *buf, unsigned size)
{
    if (size == 0)
        return 0;
    if (pipe == NULL || buf == NULL)
        return -1;

    uint32_t critical = ql_rtos_enter_critical();
    unsigned bytes = pipe->wr - pipe->rd;
    unsigned len = OSI_MIN(unsigned, size, bytes);
    unsigned rd = pipe->rd;

    if (len == 0)
    {
        ql_rtos_exit_critical(critical);
        return 0;
    }

    unsigned offset = rd % pipe->size;
    unsigned tail = pipe->size - offset;
    if (tail >= len)
    {
        memcpy(buf, &pipe->data[offset], len);
    }
    else
    {
        memcpy(buf, &pipe->data[offset], tail);
        memcpy((char *)buf + tail, pipe->data, len - tail);
    }

    pipe->rd += len;
    ql_rtos_exit_critical(critical);

    return len;
}

int osibuffReadAll(osi_buff_t *pipe, void *buf, unsigned size, unsigned timeout)
{
    if (size == 0)
        return 0;
    if (pipe == NULL || buf == NULL)
        return -1;

    unsigned len = 0;
    for (;;)
    {
        int bytes = osibuffRead(pipe, buf, size);
        if (bytes < 0)
            return -1;

        len += bytes;
        size -= bytes;
        buf = (char *)buf + bytes;
        if (size == 0 )
            break;
    }

    return len;
}
#endif

int userCallback(void *param, int param1, int param2, int param3, int data_len, const void *pcm_data)
{
#if (QL_TTS_RESAMPLE_TEST == 0)
	int err;

	err = ql_pcm_write(ql_player, (void *)pcm_data, data_len);
	if(err <= 0)
	{
		QL_TTS_LOG("write data to PCM player failed");
		return -1;
	}	
    return 0;
#else
	int err,cnt = 0;
	tts_loacl_info_t *tts = &tts_loacl_info;

	err = osibuffWriteAll(tts->pipe, pcm_data, data_len, QL_WAIT_FOREVER); //降采样需要每次输入一包完整的PCM帧640字节,因此先缓存
	tts_demo_no_err((err<0), goto exit, "cache data fail");
	tts_demo_no_err(!((uint)tts->input_buffer & (uint)tts->output_buffer), goto exit, "invalid buffer");
	
	while(1)
	{
		cnt = osibuffReadAvail(tts->pipe);
		tts_demo_no_err((err<0), goto exit, "read avil fail");

		if(cnt >= QUEC_16K_FRAME_LEN)	//降采样需要每次输入一包完整的PCM帧640字节,因此先缓存,再每次读取640字节 
		{
			cnt = osibuffReadAll(tts->pipe, (void *)tts->input_buffer, QUEC_16K_FRAME_LEN, QL_WAIT_FOREVER);
			tts_demo_no_err((cnt!=QUEC_16K_FRAME_LEN), goto exit, "pipe read fail");
			ql_aud_resampler_run(tts->filter, (short *)tts->input_buffer, (short *)tts->output_buffer); //开始降采样	
			err = ql_pcm_write(ql_player, (void *)tts->output_buffer, QUEC_8K_FRAME_LEN);
			tts_demo_no_err((err!=QUEC_8K_FRAME_LEN), goto exit, "pcm write fail");
		}
		else
		{
			break;
		}
	}
	return 0;
exit:
	QL_TTS_LOG("tts callback failed");
	return -1;
#endif
}

#if QL_TTS_RESAMPLE_TEST
void TTS_RESAM_Init(void)
{
	tts_loacl_info_t *tts = &tts_loacl_info;
	tts->filter = (ql_resampler *)calloc(1, sizeof(ql_resampler));
	if(!(tts->filter))
	{
		QL_TTS_LOG("no mem");
		goto exit;	
	}	
	ql_aud_resampler_create(QUEC_BEFORE_SAMPLING, QUEC_AFTER_SAMPLING, QUEC_BEFORE_SAMPLING*20/1000, tts->filter);

	tts->pipe = osibuffCreate(4*1024);
	tts_demo_no_err(!tts->pipe, goto exit, "no mem");

	tts->input_buffer = (char *)calloc(1, 640);
	tts_demo_no_err(!tts->input_buffer, goto exit, "no mem");
	
	tts->output_buffer = (char *)calloc(1, 640);
	tts_demo_no_err(!tts->output_buffer, goto exit, "no mem");
	return;

exit:
	if(tts->filter)
	{
		ql_aud_resampler_destroy(tts->filter);
		free(tts->filter);
		tts->filter = NULL;
	}
	if(tts->pipe)
	{
		free(tts->pipe);
		tts->pipe = NULL;
	}
	if(tts->input_buffer)
	{
		free(tts->input_buffer);
		tts->input_buffer = NULL;
	}
	if(tts->output_buffer){
		free(tts->output_buffer);
		tts->output_buffer = NULL;
	}
	return;
}
#endif

void ql_tts_thread_demo(void *param)
{
    int err = 0;
	tts_demo_play_info_t info = {0};
	QL_PCM_CONFIG_T config = {0};

	config.samplerate = 16000; //TTS采样率为16000
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_LOCAL, AUDIOHAL_SPK_VOL_11);

#if QL_TTS_RESAMPLE_TEST
	TTS_RESAM_Init();
#endif
    
	while(1)
	{	
		err = ql_rtos_queue_wait(ql_tts_demo_queue, (uint8 *)&info, sizeof(tts_demo_play_info_t), QL_WAIT_FOREVER);
		tts_demo_no_err(err, continue, "invalid queue");
		
		if(!info.str || !info.len){
			QL_TTS_LOG("invalid tts string");
			continue;
		}

		//ql_tts_init成功后,ql_tts_is_running查询值为true
		//ql_tts_end后,ql_tts_is_running查询值为false
		if(false == ql_tts_is_running())
		{
			//初始化TTS比较耗费RAM: 中文版600k左右, 英文版1.2M左右
			err = ql_tts_init(userCallback);
			tts_demo_no_err(err, goto exit, "tts init failed"); 
		}

		ql_player = ql_pcm_open(&config, QL_PCM_WRITE_FLAG | QL_PCM_BLOCK_FLAG);
		tts_demo_no_err(!ql_player, goto exit, "create player failed");

#if !QL_TTS_LANGUAGE_USE_ENGLISH //使用英文TTS库不需要设置编码
		err = ql_tts_set_config_param(QL_TTS_CONFIG_ENCODING, info.encode);
		tts_demo_no_err(err, goto exit, "config tts failed");

		err = ql_tts_set_config_param(QL_TTS_CONFIG_DIGITS, info.digit_type);
		tts_demo_no_err(err, goto exit, "config tts failed");
#endif
        
		err = ql_tts_start(info.str, info.len);
		tts_demo_no_err(err, goto exit, "tts start failed");
        
		ql_aud_data_done();
		ql_aud_wait_play_finish(QL_WAIT_FOREVER);
		
exit:	 
		//也可以不释放tts引擎(同时也就不需要再ql_tts_init), 这样下次播TTS就可以少300多ms延迟, 
		//不过这样TTS占用的ram无法被释放,用户如果ram充足可以不考虑每次都init --- deinit
		//ql_tts_deinit(); 
		
		ql_pcm_close(ql_player);
		free(info.str);   
	}
}

int ql_tts_init(pUserCallback mCallback)
{	
	tts_param_t tts_param = {0};
	
	if(!mCallback){
		return QL_TTS_INVALID_PARAM;
	}

	if(!ql_tts_is_running())
	{
#if  !QL_TTS_LANGUAGE_USE_ENGLISH
		tts_param.resource = TTS_RESOURCE_16K_CN;  //使用中文16k资源
#else
		tts_param.resource = TTS_RESOURCE_16K_EN;  //使用英文16k资源
#endif


#if		QL_TTS_LOCATION==1   //使用的tts库在外部6线flash
		tts_param.position = POSIT_EFS;      
#elif	QL_TTS_LOCATION==2   //使用的tts库在外部4线flash
		tts_param.position = POSIT_EXNSFFS;
#else                        //默认使用的tts库在内置flash
		tts_param.position = POSIT_INTERNAL_FS;   
#endif

		//int err = ql_tts_engine_init(userCallback); //若使用默认的中文16k资源，且资源文件预置到内置flash, 则直接调用ql_tts_engine_init即可
		int	err = ql_tts_engine_init_ex(userCallback, &tts_param);
		tts_demo_no_err(err, return err, "tts session begain failed");
	}
	else
	{
		QL_TTS_LOG("tts is running");
		return QL_TTS_DEVICE_BUSY;
	}
	return QL_TTS_SUCCESS;
}

int ql_tts_deinit(void)
{
	int err = 0;

	err = ql_tts_end();
	tts_demo_no_err(err, return err, "tts end failed");

	return err;
}

/*
	encoding:  		输入的TTS字符编码
	digit_type: 	要播报的内容中,数字的播报方式(如播报成一二三还是一百二十三),取值范围: TTS_DIGIT_AUTO, TTS_DIGIT_NUMBER, TTS_DIGIT_VALUE 
	interrupt:		如果当前正在播放其他TTS, 是否打断当前正在播放的TTS
	string:			要播报的内容
	len:			要播报的长度
*/
int ql_tts_play(ql_tts_encoding_e encoding, uint8 digit_type, bool interrupt, const char *string, uint len)
{
	tts_demo_play_info_t param = {0};
	int err = 0;
	
	if(encoding < QL_TTS_GBK || encoding > QL_TTS_UCS2 || !string || !len)
	{
		QL_TTS_LOG("invalid param");
		return QL_TTS_INVALID_PARAM;
	}

	param.digit_type = digit_type;
	param.encode = encoding;
	param.len = len;
	param.str = calloc(1, len);
	if(!param.str)
	{
		QL_TTS_LOG("tts no memory");
		return QL_TTS_NO_MEMORY;
	}
	
	memcpy(param.str, string, len);

	if(ql_tts_is_running() && interrupt)
	{
		ql_tts_stop_all();
	}
	
	err = ql_rtos_queue_release(ql_tts_demo_queue, sizeof(tts_demo_play_info_t), (uint8 *)&param, 0);
	if(err)
	{
		free(param.str);
	}

	return err;
}

void ql_tts_stop_all(void)
{
	ql_rtos_queue_reset(ql_tts_demo_queue);	//清空播放队列
	ql_tts_exit();							//TTS异步打断
	ql_pcm_close(ql_player);				//关闭播放器
}

#ifdef QL_APP_FEATURE_VOICE_CALL
static int ql_tts_ringtone_callback(bool start, void *ctx)
{
	if(true == start)
	{
		//此处仅演示收到来电时播放,如果需要播报真实的来电号码,请结合voice_call_demo.c
		char *str = "收到来电,请接通电话";
		ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_VALUE, false, str, strlen(str));
	}
	else
	{
		ql_tts_stop_all(); //电话接通,或者对方挂断电话,或者本地挂断电话
	}

	return 0;
}
#endif

void ql_tts_demo_init(void)
{
    uint8_t err = QL_OSI_SUCCESS;

#ifdef QL_APP_FEATURE_VOICE_CALL
	ql_aud_set_ringtone_type(QL_AUD_RING_CUSTOMER_DEF);
	ql_bind_ring_tone_cb(ql_tts_ringtone_callback);
#endif

	err = ql_rtos_queue_create(&ql_tts_demo_queue, sizeof(tts_demo_play_info_t), 10);
	if (err != QL_OSI_SUCCESS)
    {
        QL_TTS_LOG("TTS queue created failed");
		return;
    }
	
    err = ql_rtos_task_create(&ql_tts_demo_task, QL_TTS_TASK_STACK, QL_TTS_TASK_PRIO-1, "ql_tts_task", ql_tts_thread_demo, NULL, 1);
    if (err != QL_OSI_SUCCESS)
    {
    	ql_rtos_queue_delete(ql_tts_demo_queue);
		ql_tts_demo_queue = NULL;
        QL_TTS_LOG("TTS demo task2 created failed");
		return;
    }

#if !QL_TTS_LANGUAGE_USE_ENGLISH	//播放中文TTS

	char *str1 = "支付宝收款 12345元";
	char *str2 = "条形码为: 22198336452388";
	char *str3 = "一共100条来电: 来电号码为 1 2 3 4 5 6 7 8 9"; //也可以用空格将数字隔开,这样能实现同一句中既播数值也播数字
	uint16 ucs2_str[8] = {0x6B22, 0x8FCE, 0x4F7F, 0x7528, 0x79FB, 0x8FDC, 0x6A21, 0x5757}; //UCS2编码: 欢迎使用移远模块
	
	ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_VALUE, false, str1, strlen(str1));
/*	
	//模拟播放过程中打断当前播放, 开始播放下一条
	ql_rtos_task_sleep_ms(1000);
	ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_NUMBER, true, str2, strlen(str2));
*/
	ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_NUMBER, false, str2, strlen(str2));
	ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_AUTO, false, str3, strlen(str3));
	ql_tts_play(QL_TTS_UCS2, TTS_DIGIT_AUTO, false, (const char *)ucs2_str, sizeof(ucs2_str));
	
#else //播放英文TTS
	char *str_eng = "The price of the shirt is $50, and the price of the computer is $1200";
	ql_tts_play(QL_TTS_UTF8, TTS_DIGIT_AUTO, false, str_eng, strlen(str_eng));
#endif
}

#endif
