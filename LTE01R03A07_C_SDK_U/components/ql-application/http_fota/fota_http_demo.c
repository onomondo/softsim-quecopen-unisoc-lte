
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_type.h"
#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_fs.h"
#include "ql_power.h"
#include "ql_api_dev.h"
#include "ql_http_client.h"
#include "ql_api_fota.h"
#include "ql_app_feature_config.h"
#ifdef QL_APP_FEATURE_SDMMC
#include "ql_sdmmc.h"
#endif
#define QL_FOTA_HTTP_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_FOTA_HTTP_LOG(msg, ...) QL_LOG(QL_FOTA_HTTP_LOG_LEVEL, "ql_FOTA_http", msg, ##__VA_ARGS__)
#define QL_FOTA_HTTP_LOG_PUSH(msg, ...) QL_LOG_PUSH("ql_FOTA_http", msg, ##__VA_ARGS__)

#ifdef QL_APP_FEATURE_HTTP_FOTA

#define TRY_DOWN_TIMES		10
#define WRITE_TO_FILESIZE	(1024*5)		
#define QL_VERSION_MAX 256
#define HTTP_HEAD_RANGE_LENGTH_MAX  50
#define HTTP_DLOAD_URL                      "http://49.235.127.120/output.pack"

ql_task_t fota_http_task = NULL;
ql_sem_t fota_http_semp = NULL;
typedef enum 
{
	FOTA_HTTP_DOWN_INIT,		//初始阶段
	FOTA_HTTP_DOWN_DOWNING,		//下载中
	FOTA_HTTP_DOWN_INTR,		//下载被中断
	FOTA_HTTP_DOWN_DOWNED,		//下载完成
	FOTA_HTTP_DOWN_NOSPACE,		//没有空间
}e_fota_down_stage;

typedef struct 
{
    bool is_show;			//是否显示进度
    uint total_size;		//文件总共大小
    uint dload_size;		//已经下载大小
    uint file_size;			//上次升级中断保存的文件大小
}fota_http_progress_t;

typedef struct 
{
    http_client_t http_cli;				//和http交互创建的struct http_client_s类型的指针
    bool b_is_http_range;			//是否发送http get range 报文
    int profile_idx;					//cid
    uint8_t sim_id;						//simid
    char fota_packname[QL_FOTA_PACK_NAME_MAX_LEN];	//下载到本地的升级文件的位置
    fota_http_progress_t http_progress;				//http进度
    e_fota_down_stage e_stage;				//http下载固件包的阶段
	QFILE			fd;						//写文件的文件描述符
	int		i_save_size;					//在一次下载的过程中，分为多次写入，保存的是上次的写入大小，用于控制满多少字节写一次
	uint   last_precent;					//下载最后一次百分比
	bool	b_is_have_space;				//存储空间是否可用
    int    chunk_encode;                    //Transfer-Encoding:chunked 传输方式
}fota_http_client_t;

static int fota_http_get_fileszie(char *filename);
static void fota_http_close_fd(fota_http_client_t* fota_http_cli_p);

int fota_dload_file_clran(fota_http_client_t* fota_http_cli_p)
{
    if ( fota_http_cli_p->fd < 0 )
    {
        QL_FOTA_HTTP_LOG("clran write file [%s] %d",fota_http_cli_p->fota_packname,fota_http_cli_p->fd);
        fota_http_cli_p->fd = -1;
    }
    else 
    {
        ql_fclose(fota_http_cli_p->fd);
        ql_rtos_task_sleep_ms(10);
    }
    fota_http_cli_p->fd = ql_fopen(fota_http_cli_p->fota_packname, "wb+");
    if(fota_http_cli_p->fd < 0)
    {
        QL_FOTA_HTTP_LOG("clran open write file [%s]  failed %d",fota_http_cli_p->fota_packname,fota_http_cli_p->fd);
        return -1;
    }
    fota_http_cli_p->http_progress.file_size = 0;
    fota_http_cli_p->http_progress.dload_size = 0;
    fota_http_cli_p->http_progress.total_size = 0;
    fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNING;
    fota_http_cli_p->i_save_size = 0;
    QL_FOTA_HTTP_LOG("clran write file [%s] open fd %d",fota_http_cli_p->fota_packname,fota_http_cli_p->fd);
    return 0;
}

static void fota_http_info_cfg(fota_http_client_t* fota_http_cli_p)
{
	if ( fota_http_cli_p == NULL )
	{	
		QL_FOTA_HTTP_LOG("fota_http_cli_p is null");
		return;
	}
	QL_FOTA_HTTP_LOG("init file name:[%s]",fota_http_cli_p->fota_packname);
	QL_FOTA_HTTP_LOG("init file stage:[%d]",fota_http_cli_p->e_stage);
	QL_FOTA_HTTP_LOG("init file download:[%d]",fota_http_cli_p->http_progress.dload_size);
	QL_FOTA_HTTP_LOG("init file file_size:[%d]",fota_http_cli_p->http_progress.file_size);
	QL_FOTA_HTTP_LOG("init file real file_size:[%d]",fota_http_get_fileszie(fota_http_cli_p->fota_packname));
	QL_FOTA_HTTP_LOG("init file is_show:[%d]",fota_http_cli_p->http_progress.is_show);
	QL_FOTA_HTTP_LOG("init file last_percent:[%d]",fota_http_cli_p->last_precent);
	QL_FOTA_HTTP_LOG("init file space:[%d]",fota_http_cli_p->b_is_have_space);
}

/***********************************************************
* funcname		:fota_http_event_cb
* description	:
*	http响应报文的回调函数，当http请求返回的时候调用 
*	client		[in]	[http_client_t *] http句柄
*	event		[in]    [http_event_id_e] http事件类型
*	event_code  [in]    [http_error_code_e] http处理结果
*	argv        [in]    [void *]		 ql_httpc_news传进来的参数
*   infomation
*
************************************************************/

static void fota_http_event_cb(http_client_t *client, int event, int event_code, void *argv)
{
	if ( argv == NULL )
	{
		QL_FOTA_HTTP_LOG("fota_http_event_cb argv is null");
		return;
	}
	fota_http_client_t* fota_http_cli_p = (fota_http_client_t*)argv;

    if (*client != fota_http_cli_p->http_cli)
        return;
	
    switch (event)
    {
	    case HTTP_EVENT_SESSION_ESTABLISH: {
	        if (event_code != HTTP_SUCCESS)
	        {
				QL_FOTA_HTTP_LOG("HTTP session create failed!!!!! ");
				//保存下载信息,如果不是无存储空间所致，设置为下载被中断状态
				if ( fota_http_cli_p->e_stage != FOTA_HTTP_DOWN_NOSPACE && fota_http_cli_p->e_stage != FOTA_HTTP_DOWN_DOWNED )
				{
					fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_INTR;
				}
	            ql_rtos_semaphore_release(fota_http_semp);
	        }
	    }
	    break;
	    case HTTP_EVENT_RESPONE_STATE_LINE: {
	        if (event_code == HTTP_SUCCESS)
	        {
	            int resp_code = 0;
	            int content_length = 0;
	            int chunk_encode = 0;
                int accept_ranges = 0;
	            char *location = NULL;
	            ql_httpc_getinfo(client, HTTP_INFO_RESPONSE_CODE, &resp_code);
	            ql_httpc_getinfo(client, HTTP_INFO_CHUNK_ENCODE, &chunk_encode);
				QL_FOTA_HTTP_LOG("response code:%d chunk_encode %d", resp_code,chunk_encode);
				fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNING;
                if(resp_code == 200 || resp_code == 206)
                {
    	            if (chunk_encode == 0)
    	            {
                        ql_httpc_getinfo(client, HTTP_INFO_ACCEPT_RANGES, &accept_ranges);
                        ql_httpc_getinfo(client, HTTP_INFO_CONTENT_LEN, &content_length);
                        if(accept_ranges == 1 &&  fota_http_cli_p->b_is_http_range == true)
                        {
                            fota_http_cli_p->http_progress.total_size  += content_length;
                        }
                        else 
                        {
                            if(fota_dload_file_clran(fota_http_cli_p) == 0)
                            {
                                fota_http_cli_p->http_progress.total_size = content_length;
                            }
                            else
                            {
				                fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
                            }
                        }
                        QL_FOTA_HTTP_LOG("content_length:[%d] totalsize=[%d]",content_length ,fota_http_cli_p->http_progress.total_size);
    	            }
                    else if (1 == chunk_encode)
                    {
                        QL_FOTA_HTTP_LOG("http chunk encode!");
                        fota_http_cli_p->chunk_encode = 1;
                    }
                }
                else
                {
				    fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
                }
				//返回码416提示416 Requested Range Not Satisfiable
				if ( resp_code == 416 )
				{
					//发送已经最大了
					fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
				}
	            if (resp_code >= 300 && resp_code < 400)
	            {
                    fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
                    ql_httpc_getinfo(client, HTTP_INFO_LOCATION, &location);
	                QL_FOTA_HTTP_LOG("redirect location:%s", location);
	                free(location);
	            }
	        }
	    }
	    break;
	    case HTTP_EVENT_SESSION_DISCONNECT: {
	        if (event_code == HTTP_SUCCESS)
	        {
	        	//下载完成将配置文件设置为初始状态,只允许此种情况才能恢复为初始状态
	        	fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
	            QL_FOTA_HTTP_LOG("===http transfer end!!!!");
	        }
	        else
	        {
				//保存下载信息,如果不是无存储空间所致，设置为下载被中断状态
				if ( fota_http_cli_p->e_stage != FOTA_HTTP_DOWN_NOSPACE &&  fota_http_cli_p->e_stage != FOTA_HTTP_DOWN_DOWNED )
				{
					fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_INTR;
				}
	            QL_FOTA_HTTP_LOG("===http transfer occur exception!!!!!");
	        }
	        ql_rtos_semaphore_release(fota_http_semp);
	    }
	    break;
    }
}
/***********************************************************
* funcname		:fota_http_write_file
* description	:
*	将收到的数据写入文件中
************************************************************/

static int fota_http_write_file(fota_http_client_t* fota_cli_p ,char *data, int size,  QFILE fd)
{
    int ret = -1;
	uint temp=0;
    if(fota_cli_p->fd != fd)
    {
        QL_FOTA_HTTP_LOG("file fd error");
        fota_http_close_fd(fota_cli_p);
        return 0;
    }
	//写文件前休息1ms,以防永久阻塞
	ql_rtos_task_sleep_ms(1);
	QL_FOTA_HTTP_LOG("write [%d] size",size);
    ret = ql_fwrite(data, size,1,fd);
	QL_FOTA_HTTP_LOG("write ret=[%d]",ret);
    if (ret > 0)
    {
        fota_cli_p->http_progress.dload_size += (uint)ret;
		fota_cli_p->http_progress.file_size = ql_fsize(fd);
		if ( fota_cli_p->http_progress.is_show == true )
		{
            if (1 != fota_cli_p->chunk_encode)
            {
                //计算进度，如果开启进度显示，那么会计算本次进度和上次进度是否相同，进度不同才会展示进度
                temp = 100UL*fota_cli_p->http_progress.dload_size/fota_cli_p->http_progress.total_size;
                if ( fota_cli_p->last_precent != temp || temp == 100  )
                {
                    fota_cli_p->last_precent = temp;
                    QL_FOTA_HTTP_LOG("dload progress:===[%u%%]===total size[%d] file_size[%d] dload size[%d]",temp,fota_cli_p->http_progress.total_size,ql_fsize(fd),fota_cli_p->http_progress.dload_size );
                }
            }
            else
            {
                QL_FOTA_HTTP_LOG("dload progress:=== file_size[%d] dload size[%d] ===", ql_fsize(fd), fota_cli_p->http_progress.dload_size);
            }
		}

		//保存文件，每一次满5k,保存一次写入的文件
		if ( (fota_cli_p->i_save_size <= fota_cli_p->http_progress.dload_size)
          || ( (1 != fota_cli_p->chunk_encode) && (fota_cli_p->i_save_size >= fota_cli_p->http_progress.total_size) ) )
		{
			//满WRITE_TO_FILESIZE个字节保存一次
			if ( (1 != fota_cli_p->chunk_encode) && (fota_cli_p->i_save_size >= fota_cli_p->http_progress.total_size) )
			{
				fota_cli_p->i_save_size = fota_cli_p->http_progress.total_size;
			}
			else
			{
				fota_cli_p->i_save_size=fota_cli_p->http_progress.dload_size+WRITE_TO_FILESIZE;
			}
		}
		if ( (1 != fota_cli_p->chunk_encode) && (fota_cli_p->http_progress.dload_size >= fota_cli_p->http_progress.total_size) )
		{
			fota_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
		}
		if ( ret != size )
		{
			//关闭固件升级包的文件描述符
			fota_http_close_fd(fota_cli_p);
		}
    }
    else
    {
        QL_FOTA_HTTP_LOG("error: ret:%d",ret);
		//关闭固件升级包的文件描述符
		fota_http_close_fd(fota_cli_p);
    }
	return ret;
}

/***********************************************************
* funcname		:fota_http_write_response_data
* description	:
*	http响应报文，处理报文体的回调函数。
*	client		[in]  [int]   http请求的句柄
*	argv		[in]  [void *] ql_httpc_setopt用HTTP_CLIENT_OPT_WRITE_DATA传递的参数
*	data		[in ] [char *] http响应的报文体数据
*	size		[in]  [int]    http响应报文体数据大小
*	end         [in]  [int]   1 表示当前为最后一包数据 0 非最后一包数据
*   返回值: 实际处理的数据长度
*
************************************************************/
static int fota_http_write_response_data(http_client_t *client, void *argv, char *data, int size, unsigned char end)
{

    int ret = -1;
	int write_size = size ;
	char * p_write_data= data;
	int    i_deal_size = WRITE_TO_FILESIZE;
	int64 file_free_size=0;
	if ( argv == NULL )
	{
		QL_FOTA_HTTP_LOG("fota_http_write_response_data argv is invalied NULL ");
		return -2;
	}
	fota_http_client_t* fota_cli_p = (fota_http_client_t*)argv;

	if ( (fota_cli_p->e_stage == FOTA_HTTP_DOWN_DOWNED)
     ||  (fota_cli_p->chunk_encode == 1 && end == 1) )
	{
        fota_cli_p->e_stage = FOTA_HTTP_DOWN_DOWNED;
		//416提示资源已经超出长度，多余的314个字节的固件包数据不用写入文件。
		QL_FOTA_HTTP_LOG("go on dload file finished",fota_cli_p->fota_packname);
		//关闭本地固件升级包文件描述符
		fota_http_close_fd(fota_cli_p);
		return 0;
	}

    //获取当前剩余空间，如果不够直接不写文件
    file_free_size = ql_fs_free_size(fota_cli_p->fota_packname);
    if ( (1 != fota_cli_p->chunk_encode && file_free_size < (fota_cli_p->http_progress.total_size - fota_cli_p->http_progress.dload_size))
      || (1 == fota_cli_p->chunk_encode &&  file_free_size < size) )
    {
        if (1 != fota_cli_p->chunk_encode)
        {
	        QL_FOTA_HTTP_LOG("free_space[%d] total_size [%d] dload_size[%d]",file_free_size,fota_cli_p->http_progress.total_size\
		        ,fota_cli_p->http_progress.dload_size);
        }
        else
        {
            QL_FOTA_HTTP_LOG("free_space[%d] dload_size[%d]",file_free_size,fota_cli_p->http_progress.dload_size);
        }
	    fota_cli_p->e_stage=FOTA_HTTP_DOWN_NOSPACE;
	    fota_cli_p->b_is_have_space = false;
	    QL_FOTA_HTTP_LOG("file free_size not enough");
	    fota_http_close_fd(fota_cli_p);
	    return 0;
    }

	if ( size <=0 )
	{
		QL_FOTA_HTTP_LOG("write 0 size to file [%s]",fota_cli_p->fota_packname);
		//关闭本地固件升级包文件描述符
		fota_http_close_fd(fota_cli_p);
		return -1;
	}
	//在这儿也可以添加一个缓存池，将数据先写入缓存池(需要判断数据量是否大于缓存池的情况)，当缓存池满了在写入文件，
	//以防大量的小数据频繁写文件操作
	//每次写1k
	do {
		if ( write_size < i_deal_size )
		{
			i_deal_size = write_size;
		}
		ret = fota_http_write_file(fota_cli_p ,p_write_data, i_deal_size,  fota_cli_p->fd);
		if ( ret < 0)
		{
			QL_FOTA_HTTP_LOG("write file error");
			return size-write_size;
		}
		write_size -= ret;
		p_write_data +=ret;
	}while(write_size > 0);
	return size-write_size;
}

int fota_http_init(fota_http_client_t* fota_http_cli_p)
{
	QFILE fd = -1;
	ql_rtos_semaphore_create(&fota_http_semp, 0);
	//获取上次未完成下载信息
	memset(fota_http_cli_p,0x00,sizeof(fota_http_client_t));

	fota_http_cli_p->http_cli = 0;					//http连接句柄
	fota_http_cli_p->profile_idx = 1;				//cid
	fota_http_cli_p->sim_id = 0;					//simid
	fota_http_cli_p->e_stage =FOTA_HTTP_DOWN_INIT;
	fota_http_cli_p->i_save_size = 0;
	memcpy(fota_http_cli_p->fota_packname,"UFS:fota.pack",strlen("UFS:fota.pack"));	//默认升级包地址

	//存储空间开始默认为足够
	fota_http_cli_p->b_is_have_space = true;
	fota_http_cli_p->http_progress.is_show = true;	//设置展示进度条
	fota_http_cli_p->last_precent = 0;
    fota_http_cli_p->chunk_encode = 0;
	fd = ql_fopen(fota_http_cli_p->fota_packname, "wb+");
	if(fd < 0)
	{
		QL_FOTA_HTTP_LOG("init file name:[%s] ret: %d",fota_http_cli_p->fota_packname,fd);
		return -1;
	}
	ql_fclose(fd);
	QL_FOTA_HTTP_LOG("init file name:[%s]",fota_http_cli_p->fota_packname);
	QL_FOTA_HTTP_LOG("init file size:[%d]",fota_http_get_fileszie(fota_http_cli_p->fota_packname));
	QL_FOTA_HTTP_LOG("init file stage:[%d]",fota_http_cli_p->e_stage);
	return 0;
}

static void fota_http_release()
{
	ql_rtos_semaphore_delete(fota_http_semp);
	//不设置成NULL 会导致restart
	fota_http_semp = NULL ;
}

static int fota_http_get_fileszie(char *filename)
{
	int file_size= 0;
	QFILE fd = ql_fopen(filename,"rb");
	if ( fd < 0 )
	{
		QL_FOTA_HTTP_LOG("open file name:[%s] ret = %d",filename,fd);
		return -1;
	}
	file_size = ql_fsize(fd);
	ql_fclose(fd);
	return file_size;
}

/***********************************************************
* funcname		:fota_http_active
* description	:
*	注册网络，拨号，初始化网络环境
* return:
*  QL_DATACALL_SUCCESS  -- sucess 
*  other 	-- failed 
************************************************************/
static ql_datacall_errcode_e fota_http_active(uint8_t sim,int cid)
{
	ql_datacall_errcode_e ret = QL_DATACALL_ACTIVE_FAIL_ERR;
	//判断网络是否正常
    if (ql_datacall_get_sim_profile_is_active(sim, cid) == false)
	{
		unsigned char num=0;
		
		for ( num = 0 ; num < 10 ; num++ )
		{
			if ( (ret = ql_network_register_wait(sim, 120)) == QL_DATACALL_SUCCESS )
			{	
				//注网成功跳出循环
				break;
			}
		}
		
		if ( ret != QL_DATACALL_SUCCESS )
		{
			return ret;
		}
		//设置拨号同步
		if ( (ret = ql_set_data_call_asyn_mode(sim,cid,0)) != QL_DATACALL_SUCCESS )
		{
			return ret;
		}
		
		num=0;
		for (  num=0 ; num < 3 ; num++ )
		{
			if ( (ret = ql_start_data_call(sim, cid, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0))  == QL_DATACALL_SUCCESS )
			{
				//拨号成功跳出
				break;
			}
		}
	}
	return ret;
}

/********************************************************************
* funcname		:fota_http_get_fd
* description	:
*	打开升级包文件描述符，用于写入http下载升级包数据，
*	如果读取的上次保留的下载信息配置文件中的中断状态是初始状态、下载
*	已完成状态、或者是由于存储空间不够导致的下载失败状态，则删除上级包
*	重新下载重新覆盖写本地上次下载的升级包文件，其他的中断情况则启用追
*	加的方式重新请求升级包，追加升级包文件。
*  fota_http_cli_p [in]   [fota_http_client_t *] fota http客户端结构体
* return:
*  0  -- sucess 
*  other 	-- failed 
**********************************************************************/
static QFILE fota_http_get_fd(fota_http_client_t* fota_http_cli_p)
{
	if ( fota_http_cli_p->e_stage == FOTA_HTTP_DOWN_INIT || fota_http_cli_p->e_stage ==FOTA_HTTP_DOWN_DOWNED \
		||fota_http_cli_p->e_stage == FOTA_HTTP_DOWN_NOSPACE )
	{
		//已经完成、初始化、或者是没有空间导致的下载中断以覆盖写来打开文件
		fota_http_cli_p->fd = ql_fopen(fota_http_cli_p->fota_packname, "wb+");
		fota_http_cli_p->http_progress.file_size = 0;
		fota_http_cli_p->http_progress.dload_size = 0;
		fota_http_cli_p->http_progress.total_size = 0;
		fota_http_cli_p->e_stage = FOTA_HTTP_DOWN_INIT;
		fota_http_cli_p->i_save_size = 0;
		QL_FOTA_HTTP_LOG("over write file [%s]",fota_http_cli_p->fota_packname);
	}
	else
	{
		//其他的情况是追加方式打开文件
		fota_http_cli_p->fd = ql_fopen(fota_http_cli_p->fota_packname, "ab+");
		QL_FOTA_HTTP_LOG("add write file [%s]",fota_http_cli_p->fota_packname);
	}
	if ( fota_http_cli_p->fd < 0 )
	{	
		QL_FOTA_HTTP_LOG("ql_fopen failed");
	}
	return fota_http_cli_p->fd;
}
/********************************************************************
* funcname		:fota_http_close_fd
* description	:
*	关闭下载升级包的文件描述符
*  fota_http_cli_p [in]   [fota_http_client_t *] fota http客户端结构体
* return:
*  0  -- sucess 
*  other 	-- failed 
**********************************************************************/

static void fota_http_close_fd(fota_http_client_t* fota_http_cli_p)
{
	if ( fota_http_cli_p->fd > 0 )
	{
		ql_fclose(fota_http_cli_p->fd);
		fota_http_cli_p->fd = -1;
	}
}
/***********************************************************
* funcname		:fota_http_active
* description	:
*	初始化http请求的网络环境，组建http请求报文，发起http请求
* return:
*  0  -- sucess 
*  other 	-- failed 
************************************************************/
static int fota_http_evn_request(fota_http_client_t* fota_http_cli_p)
{
	char dload_range[HTTP_HEAD_RANGE_LENGTH_MAX] = {0};
	http_method_e e_http_method;
	uint8_t sim = fota_http_cli_p->sim_id;
	int cid = fota_http_cli_p->profile_idx;
	//发送http请求前创建存储升级包文件的文件描述符，别忘关闭
	if ( fota_http_get_fd(fota_http_cli_p) < 0 )
	{
		QL_FOTA_HTTP_LOG("range_request http data done ,file_size[%d]",fota_http_cli_p->http_progress.file_size);
		return -1;
	}
	//注网拨号
	if ( QL_DATACALL_SUCCESS != fota_http_active(sim,cid) )
	{
		QL_FOTA_HTTP_LOG("http net is failed ");
		fota_http_close_fd(fota_http_cli_p);
		return -1;
	}
	
	//创建http请求句柄
	if (ql_httpc_new(&(fota_http_cli_p->http_cli), fota_http_event_cb, fota_http_cli_p) != HTTP_SUCCESS)
    {
        QL_FOTA_HTTP_LOG("http create failed");
		ql_httpc_release(&(fota_http_cli_p->http_cli));
		fota_http_close_fd(fota_http_cli_p);
		return -2;
    }
	//设置http请求方式为HTTP_METHOD_GET 
	e_http_method = HTTP_METHOD_GET;
	ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_METHOD,e_http_method);
	
	//文件大小不为0，fota_http_get_fd已经限制了只有在下载被中断的情况下发生
	if ( fota_http_cli_p->b_is_http_range == true )
	{
		//设置使用断电续传功能，使用上次下载未完成的最后下载信息
		fota_http_cli_p->http_progress.dload_size = fota_http_cli_p->http_progress.file_size;
		sprintf(dload_range, "Range: bytes=%d-",fota_http_cli_p->http_progress.file_size); 
		ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_REQUEST_HEADER,dload_range);
		QL_FOTA_HTTP_LOG("Get http %s",dload_range);
	}
	else
	{
		//不设置范围下载字段
	}
	//设置url下载地址
	ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_URL, HTTP_DLOAD_URL);
	//设置sim_id
	ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_SIM_ID, fota_http_cli_p->sim_id);
	//设置cid
	ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_PDPCID, fota_http_cli_p->profile_idx);
	//接收报体中的文件内容
    ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_WRITE_FUNC, fota_http_write_response_data);
	//设置fota_http_write_response_data 第二参数为fota_http_cli
    ql_httpc_setopt(&(fota_http_cli_p->http_cli), HTTP_CLIENT_OPT_WRITE_DATA, fota_http_cli_p);
	//发送http请求
	if (ql_httpc_perform(&fota_http_cli_p->http_cli) == HTTP_SUCCESS)
	{
		 //阻塞等待信号量
		 if ( ql_rtos_semaphore_wait(fota_http_semp, QL_WAIT_FOREVER) != QL_OSI_SUCCESS )
		 {
		 	//获取信号量失败
		 	ql_httpc_release(&(fota_http_cli_p->http_cli));
		 	fota_http_close_fd(fota_http_cli_p);
		 	return -1;
		 }
		 QL_FOTA_HTTP_LOG("fota http dload size %d=====End,\n",fota_http_cli_p->http_progress.dload_size);
		 ql_httpc_release(&(fota_http_cli_p->http_cli));
		 fota_http_close_fd(fota_http_cli_p);
		 return 0;
	}
	ql_httpc_release(&(fota_http_cli_p->http_cli));
	fota_http_close_fd(fota_http_cli_p);
	return -3;
}

/***********************************************************
* funcname		:fota_http_download_pacfile
* description	:
*	下载升级包主体函数
* return:
*  0 		-- sucess  下载成功并且校验成功
*
*  other 	-- failed 
************************************************************/
static int 	fota_http_download_pacfile(fota_http_client_t* fota_http_cli_p)
{

	char *old_version = NULL;
    char *new_version = NULL;
	fota_http_info_cfg(fota_http_cli_p);
	//初始化http网络环境，组http请求报文,发送http请求，阻塞到下载完成或异常
	if (fota_http_evn_request(fota_http_cli_p) != 0)
	{
		int file_size = fota_http_get_fileszie(fota_http_cli_p->fota_packname);
		QL_FOTA_HTTP_LOG("failed [%s] size[%d]",fota_http_cli_p->fota_packname,file_size);
		return -1;
	}
	fota_http_info_cfg(fota_http_cli_p);
	//校验下载完成文件是否有效
	if ( fota_http_cli_p->e_stage == FOTA_HTTP_DOWN_DOWNED )
	{
		ql_errcode_fota_e ret = ql_fota_image_verify(fota_http_cli_p->fota_packname);
		if ( ret != QL_FOTA_SUCCESS )
		{
			//下载完成校验不成功删除文件
			ql_remove(fota_http_cli_p->fota_packname);
			QL_FOTA_HTTP_LOG("[%s]package is invalid",fota_http_cli_p->fota_packname);
			return -3;
		}
		else
		{
			//校验成功
            if(ql_fota_get_image_version(&old_version,&new_version))
            {
                QL_FOTA_HTTP_LOG("********** verion error **********");
                //do someting
            }
            QL_FOTA_HTTP_LOG("old :%s --> new: %s",old_version,new_version);
#if 0
            if(strcmp(new_version,$your_version$) == 0)
            {
                //version check ok
            }
            else
            {
                //version check fail,delete fota img and reset fota status
                ql_fota_file_reset(true);
            }
#endif
            if(old_version)
            {
                free(old_version);
            }
            if(new_version)
            {
                free(new_version);
            }
			QL_FOTA_HTTP_LOG("download is sucess ,system will reset power!");
			ql_rtos_task_sleep_s(5);
	        ql_power_reset(RESET_NORMAL);
		}
	}
	return 0;
}


ql_fota_result_e  fota_http_result_process(void)
{
	ql_fota_result_e 	p_fota_result = 0;
	
	//获取升级结果
	if ( ql_fota_get_result(&p_fota_result) != QL_FOTA_SUCCESS )
	{
		QL_FOTA_HTTP_LOG("ql_fota_get_result failed ");
		return QL_FOTA_STATUS_INVALID;
	}

	if ( p_fota_result == QL_FOTA_FINISHED )
	{
		QL_FOTA_HTTP_LOG("update finished");
		ql_fota_file_reset(TRUE);
		return QL_FOTA_FINISHED;
	}
	else if(p_fota_result == QL_FOTA_READY)
	{
		QL_FOTA_HTTP_LOG("fota ready bigen power reset ");
		ql_rtos_task_sleep_s(5);
		ql_power_reset(RESET_NORMAL);
	}
	else if(p_fota_result == QL_FOTA_NOT_EXIST)
	{
		QL_FOTA_HTTP_LOG("fota file not exist");
		ql_fota_file_reset(TRUE);
		return QL_FOTA_NOT_EXIST;
	}
	QL_FOTA_HTTP_LOG("fota  result status invalid");
	return QL_FOTA_STATUS_INVALID;
}


void fota_http_app_thread()
{

	ql_rtos_task_sleep_s(5);
#ifdef QL_APP_FEATURE_SDMMC
    QL_FOTA_HTTP_LOG("ql sdmmc mount : %d",ql_sdmmc_mount());
#endif
	fota_http_client_t  fota_http_cli;
	uint8				ui_down_times = TRY_DOWN_TIMES;
	char 				version_buf[QL_VERSION_MAX] = {0};
	//获取升级结果
	if(fota_http_result_process() == QL_FOTA_FINISHED)
	{
		goto exit;
	}

	ql_dev_get_firmware_version(version_buf, sizeof(version_buf));
	QL_FOTA_HTTP_LOG("current version:  %s", version_buf);
	
	//下载前初始化
	if(fota_http_init(&fota_http_cli) != 0)
	{
		QL_FOTA_HTTP_LOG("fota http init failed");
		goto exit1;
	}

	//尝试下载最多十次
	while( ui_down_times-- )
	{
		QL_FOTA_HTTP_LOG("start [%d] times download fota packge",TRY_DOWN_TIMES-ui_down_times);
		
		if ( fota_http_download_pacfile(&fota_http_cli) == 0 )
		{
			//下载完成
			break;
		}
		
		if ( fota_http_cli.b_is_have_space != true )
		{
			//空间不够，删除文件
			ql_remove(fota_http_cli.fota_packname);
			QL_FOTA_HTTP_LOG("have no space");
			break;
		}
		//下载失败等待10s重新开始下载

		ql_rtos_task_sleep_s(5);
	}
		//下载失败等待10s重新开始下载
exit1:
		fota_http_release();
		ql_rtos_task_sleep_s(5);
exit:		
		QL_FOTA_HTTP_LOG("exit ql_http_fota_demo");
		ql_rtos_task_delete(fota_http_task);
}
#endif

void ql_fota_http_app_init()
{
#ifdef QL_APP_FEATURE_HTTP_FOTA
	QL_FOTA_HTTP_LOG("http fota demo support!");
	QlOSStatus err = QL_OSI_SUCCESS;
	err = ql_rtos_task_create(&fota_http_task, 4096*32, APP_PRIORITY_NORMAL, "QfotaHttp", fota_http_app_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_FOTA_HTTP_LOG("created task failed");
    }
#else
    QL_FOTA_HTTP_LOG("http fota demo not support!");
#endif
}
