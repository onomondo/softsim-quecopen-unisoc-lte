
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
#include "ql_api_fota.h"
#include "ql_ftp_client.h"
#include "ql_app_feature_config.h"

#define QL_FOTA_FTP_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_FOTA_FTP_LOG(msg, ...) QL_LOG(QL_FOTA_FTP_LOG_LEVEL, "ql_FOTA_ftp", msg, ##__VA_ARGS__)
#define QL_FOTA_FTP_LOG_PUSH(msg, ...) QL_LOG_PUSH("ql_FOTA_ftp", msg, ##__VA_ARGS__)

#ifdef QL_APP_FEATURE_FTP_FOTA
#define QL_VERSION_MAX 256

float approximate_zero = 1e-6;    //近似0值,用于两个浮点型变量判相等


/* 默认配置,用户可自行修改 */
bool file_progress_show = true;                   //默认显示下载进度
char local_filename[] = "UFS:fota.pac";           //本地默认升级包地址


int profile_idx = 1;    //cid
uint8_t nSim = 0;     //sid

//FTP服务器相关信息
char hostname[] = "220.180.239.212:8309";         //FTP服务器 IP:端口号
char username[] = "test";                         //FTP服务器用户名
char password[] = "test";                         //FTP服务器用户密码
char file_full_path[] = "kruskal/output.pack";    //FTP服务器文件路径


ql_task_t fota_ftp_task = NULL;

typedef enum{
    QL_FTP_FOTA_SUCCESS    =  0,
    QL_ERROR_INVALID_PARAM = -1,
}ql_fota_ftp_error_code_e;

typedef enum{
    DOWN_INIT,           //初始阶段
    DOWN_DOWNING,        //下载中
    DOWN_INTR,           //下载被中断
    DOWN_DOWNED,         //下载完成
    DOWN_NOSPACE,        //没有空间
    DOWN_OTHER_ERROR,    //执行下载时其他错误
}fota_down_state;

typedef struct{
    bool show;                             //是否显示文件下载进度
    unsigned long total_size;              //从FTP服务器所要下载的文件总大小
    unsigned long downloaded_size;         //已下载到本地的FOTA文件大小
    unsigned long last_downloaded_size;    //上次下载完成后文件的大小
}fota_file_progress;

typedef struct{
    void *ftp_handle;                                  //用于与FTP服务器进行文件交互的句柄
    bool poweroutage_resume;                           //0-不支持断点续传,1-支持断点续传
    int profile_idx;                                   //cid
    uint8_t nSim;                                    //sid
    fota_down_state down_state;                        //当前从FTP服务器下载文件的下载状态
    fota_file_progress file_progress;                  //当前从FTP服务器文件信息和已下载文件的具体进度信息
    char local_filename[QL_FOTA_PACK_NAME_MAX_LEN];    //下载到本地的升级文件的位置

    //FTP服务器相关
    char hostname[256];          //FTP服务器: IP地址+端口号 (IP地址:端口号)
    char username[256];          //FTP服务器用户名
    char password[256];          //FTP服务器用户密码
    char file_full_path[256];    //所要从FTP服务器上获取的文件的具体路径信息,包括文件名. (文件路径/文件名)
}fota_ftp_client_t;

fota_ftp_client_t fota_ftp_cli = {.down_state = DOWN_INIT};

int fota_ftp_client_init(fota_ftp_client_t *fota_ftp_cli_p)
{
    int ret = QL_FTP_FOTA_SUCCESS;
	QFILE local_file_fd = -1;	 //用来保存本地FTP FOTA文件的文件描述符
	
	fota_ftp_cli_p->poweroutage_resume = true;
	fota_ftp_cli_p->profile_idx = profile_idx;
	fota_ftp_cli_p->nSim = nSim;

	fota_ftp_cli_p->down_state = DOWN_INIT;

	fota_ftp_cli_p->file_progress.show = file_progress_show;
	fota_ftp_cli_p->file_progress.total_size = 0;	 //等连接到FTP服务器,获得服务器文件大小后才赋值
	fota_ftp_cli_p->file_progress.downloaded_size = 0;
	fota_ftp_cli_p->file_progress.last_downloaded_size = 0;

	memcpy(fota_ftp_cli_p->local_filename, local_filename, strlen(local_filename) );	//默认升级包地址

	//FTP服务器相关信息
	memcpy(fota_ftp_cli_p->hostname, hostname, strlen(hostname) );
	memcpy(fota_ftp_cli_p->username, username, strlen(username) );
	memcpy(fota_ftp_cli_p->password, password, strlen(password) );
	memcpy(fota_ftp_cli_p->file_full_path, file_full_path, strlen(file_full_path) );

	
	//本次对FTP服务器的文件进行操作时,文件句柄都需要重新创建
	fota_ftp_cli_p->ftp_handle = ql_ftp_client_new();
	if (NULL == fota_ftp_cli_p->ftp_handle)
	{
		//未成功创建句柄时,返回错误,并且退出线程
		ret = QL_ERROR_INVALID_PARAM;
	}	
	local_file_fd = ql_fopen(fota_ftp_cli_p->local_filename,"wb+");
	if(local_file_fd <= 0)
	{
		QL_FOTA_FTP_LOG("ftp fota loc file open failed  ret : %d!",local_file_fd);
		ret = QL_ERROR_INVALID_PARAM;
	}
	ql_fclose(local_file_fd);
    return ret;
}

int fota_ftp_net_register(uint8_t nSim,int profile_idx)
{
    int ret = QL_FTP_FOTA_SUCCESS;
    ql_data_call_info_s info;
    memset(&info, 0x00, sizeof(ql_data_call_info_s));

    for (int i = 0; i < 10; i++)
    {
        ret = ql_network_register_wait(nSim, 120);
        if (QL_FTP_FOTA_SUCCESS == ret)
        {
            break;
        }
        ql_rtos_task_sleep_s(1);
    }

    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        QL_FOTA_FTP_LOG("network register failure!");
        return QL_ERROR_INVALID_PARAM;
    }

    ret = ql_set_data_call_asyn_mode(nSim, profile_idx, 0);
    QL_FOTA_FTP_LOG("network datacall asyn mode ret = %d",ret);

    ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
    QL_FOTA_FTP_LOG("network datacall result ret = %d",ret);
    if (0 != ret)
    {
        QL_FOTA_FTP_LOG("data call failure!");
    }

    ret = ql_get_data_call_info(nSim, profile_idx, &info);
    if (0 != ret)
    {
        ql_stop_data_call(nSim, profile_idx);
        return QL_ERROR_INVALID_PARAM;
    }

    return ret;
}

int fota_ftp_server_connect(fota_ftp_client_t *fota_ftp_cli_p)
{
    int ret = QL_FTP_FOTA_SUCCESS;
    uint16_t sim_cid;

    ret = ql_bind_sim_and_profile(fota_ftp_cli_p->nSim, fota_ftp_cli_p->profile_idx, &sim_cid);
    if (ret != QL_DATACALL_SUCCESS)
    {
        return QL_ERROR_INVALID_PARAM;
    }
    ql_ftp_client_setopt(fota_ftp_cli_p->ftp_handle, QL_FTP_CLIENT_SIM_CID, sim_cid);
    ql_ftp_client_setopt(fota_ftp_cli_p->ftp_handle, QL_FTP_CLIENT_OPT_PDP_CID, fota_ftp_cli_p->profile_idx);

    ret = ql_ftp_client_open(fota_ftp_cli_p->ftp_handle, fota_ftp_cli_p->hostname, fota_ftp_cli_p->username, fota_ftp_cli_p->password);
    QL_FOTA_FTP_LOG("********** ql_ftp_client_open ret:%d **********",ret);

    return ret;
}

int fota_ftp_get_server_file_size(fota_ftp_client_t *fota_ftp_cli_p)
{
    QL_FOTA_FTP_LOG("********** fota_ftp_get_server_file_size ***********");
    int ret = QL_FTP_FOTA_SUCCESS;
    double file_size = 0;

    ret = ql_ftp_client_size(fota_ftp_cli_p->ftp_handle, fota_ftp_cli_p->file_full_path, &file_size);
    fota_ftp_cli_p->file_progress.total_size = file_size;
    QL_FOTA_FTP_LOG("********** fota_ftp_cli_p->file_progress.total_size : %d **********", fota_ftp_cli_p->file_progress.total_size);
	return ret;
}

int fota_ftp_net_connect_ready(fota_ftp_client_t *fota_ftp_cli_p)
{
    int ret = QL_FTP_FOTA_SUCCESS;
    int residual_space_size = 0;

    //网络注册,连接
    ret = fota_ftp_net_register(fota_ftp_cli_p->nSim, fota_ftp_cli_p->profile_idx);
    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        QL_FOTA_FTP_LOG("********** Network registration failed **********");
        ret = QL_ERROR_INVALID_PARAM;
        goto exit;
    }

    //向FTP服务器发起连接请求
    ret = fota_ftp_server_connect(fota_ftp_cli_p);
    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        //未成功与FTP服务器建立连接
        QL_FOTA_FTP_LOG("********** The connection to the FTP server was unsuccessful **********");
        ret = QL_ERROR_INVALID_PARAM;
        goto exit;
    }

    //获取FTP服务器所要下载的文件大小
   
    if (0 != fota_ftp_get_server_file_size(fota_ftp_cli_p))
    {
        //文件不存在于FTP服务器上
        QL_FOTA_FTP_LOG("********** The file to be downloaded does not exist  or failed on the FTP server **********");
        //断开与FTP服务器的连接
        ql_ftp_client_close(fota_ftp_cli_p->ftp_handle);
        ret = QL_ERROR_INVALID_PARAM;
        goto exit;
    }

    //获取模块剩余空间大小
    residual_space_size = ql_fs_free_size(fota_ftp_cli_p->local_filename);
    QL_FOTA_FTP_LOG("********** residual_space_size:%d   ftp file total_size:%d**********", residual_space_size,fota_ftp_cli_p->file_progress.total_size);

    //判断文件空间是否足够
    //当 total_size - downloaded_size > residual_space_size 时 空间不足
    if (fota_ftp_cli_p->file_progress.total_size > residual_space_size)
    {
        //剩余空间不足,无法下载FOTA文件
        QL_FOTA_FTP_LOG("********** There is not enough free space to download FOTA files **********");
		fota_ftp_cli_p->down_state = DOWN_NOSPACE;
		
		ql_remove(fota_ftp_cli_p->local_filename);
        //断开与FTP服务器的连接
        ql_ftp_client_close(fota_ftp_cli_p->ftp_handle);
        ret = QL_ERROR_INVALID_PARAM;
        goto exit;
    }
exit:
    return ret;
}

void fota_ftp_show_file_progress(fota_ftp_client_t *fota_ftp_cli_p)
{
    float last_download_progress = 0;    //上次下载完成后的进度信息
    float download_progress = 0;         //本次下载完成后的进度信息

    //避免除0中断
    if (0 == fota_ftp_cli_p->file_progress.total_size)
    {
        return;
    }

    last_download_progress = (float)(fota_ftp_cli_p->file_progress.last_downloaded_size)/(fota_ftp_cli_p->file_progress.total_size);
    download_progress = (float)(fota_ftp_cli_p->file_progress.downloaded_size)/(fota_ftp_cli_p->file_progress.total_size);

    //相同的进度不予显示
    if ( (download_progress - last_download_progress) < approximate_zero )
    {
        return;
    }

    download_progress = download_progress * 100;
    QL_FOTA_FTP_LOG("*************************************************************");
    QL_FOTA_FTP_LOG("********** Current file download progress : %.1f%% **********", download_progress);
    QL_FOTA_FTP_LOG("*************************************************************");

}

size_t fota_ftp_write_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    QL_FOTA_FTP_LOG("********** fota_ftp_write_cb **********");

    int ret = 0;
    QFILE fd = -1;
    fota_ftp_client_t *user_data;

    user_data = (fota_ftp_client_t *)stream;

    //"ab" 打开或新建一个二进制文件,只允许在文件末尾追写    
    fd = ql_fopen(user_data->local_filename, "ab");
    //回写数据前将状态置为中断,ql_fwrite出现异常无法继续执行下去时,将会一直保持此状态,再次调用FTP FOTA线程时,将使用本次的断点信息继续下载
    user_data->down_state = DOWN_INTR;
    ret = ql_fwrite(ptr, size, nmemb, fd);
    QL_FOTA_FTP_LOG("ql_fwrite ret = %d",ret);

    ql_fclose(fd);

 if (size*nmemb == ret)
    {
        user_data->file_progress.downloaded_size += size*nmemb;

        if (true == user_data->file_progress.show)
        {
            //显示本次调用回调fota_ftp_write_cb,回写文件当前下载进度
            fota_ftp_show_file_progress(user_data);
        }

        user_data->file_progress.last_downloaded_size = user_data->file_progress.downloaded_size;

        //当前状态还视为下载中,是否下载完成在 fota_ftp_get_file 通过本地文件与服务器文件大小进行判断
        user_data->down_state = DOWN_DOWNING;
    }
	else 
	{
		if (QL_FILE_NO_SPACE == ret)
		{
			//空间不足,无法写入数据
			user_data->down_state = DOWN_NOSPACE;
		}
		else 
		{
			user_data->down_state = DOWN_OTHER_ERROR;
		}
		return 0;
	}
    //ret的其余错误码均视为回写过程中的异常中断
    QL_FOTA_FTP_LOG();
    return size*nmemb;
}

void fota_ftp_get_file(fota_ftp_client_t *fota_ftp_cli_p)
{
    QL_FOTA_FTP_LOG("********** fota_ftp_get_file **********");

    int ret;

    //设置文件下载起始偏移为上次文件下载之后的位置,初始偏移量为0
    QL_FOTA_FTP_LOG("********** downloaded_size : %ld**********", fota_ftp_cli_p->file_progress.downloaded_size);
    ql_ftp_client_setopt(fota_ftp_cli_p->ftp_handle, QL_FTP_CLIENT_OPT_START_POS, fota_ftp_cli_p->file_progress.downloaded_size);

    ret = ql_ftp_client_get_ex(fota_ftp_cli_p->ftp_handle, fota_ftp_cli_p->file_full_path, NULL, fota_ftp_write_cb, (void *)fota_ftp_cli_p);
    QL_FOTA_FTP_LOG("********** ql_ftp_client_get_ex : %d **********", ret);

    if ( (QL_FTP_FOTA_SUCCESS != ret) && (DOWN_DOWNING == fota_ftp_cli_p->down_state) )
    {
        //fota_ftp_cli_p->down_state 被置为 DOWN_DOWNING,说明已经执行了回调函数fota_ftp_write_cb,且最后一次执行回调时还在下载中
        //由于网络异常,导致回写无法继续进行,ql_ftp_client_get_ex返回错误,fota_ftp_cli_p->down_state为上次正常执行时的状态
        //下载状态设置为阻塞状态,并尝试下一次FTP文件获取请求

        //下载过程中网络异常
        fota_ftp_cli_p->down_state = DOWN_INTR;
        return;
    }

    if (DOWN_INTR == fota_ftp_cli_p->down_state)
    {
        //下载中断的状态,还未正确执行fota_ftp_write_cb而修改
        return;
    }

    if (DOWN_NOSPACE == fota_ftp_cli_p->down_state)
    {
        //由于空间不足导致ql_ftp_client_get_ex返回值异常,直接退出
        return;
    }

    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        //ql_ftp_client_get_ex 配置参数错误,导致无法正确调用写回调函数
        fota_ftp_cli_p->down_state = DOWN_OTHER_ERROR;
        return;
    }

    if (fota_ftp_cli_p->file_progress.downloaded_size >= fota_ftp_cli_p->file_progress.total_size)
    {
        //文件已下载大小和FTP 服务器文件大小一致,完成文件下载
        fota_ftp_cli_p->down_state = DOWN_DOWNED;
    }
}

void fota_ftp_file_download(fota_ftp_client_t *fota_ftp_cli_p)
{
	//向FTP服务器请求最多10次FTP FOTA文件下载
	for (int i = 0; i < 10; i++)
	{
		QL_FOTA_FTP_LOG("*************** down_state : %d ******************", fota_ftp_cli_p->down_state);
		if (DOWN_DOWNED == fota_ftp_cli_p->down_state)
		{
			//下载完成
			QL_FOTA_FTP_LOG("FTP FOTA file download complete");
			break;
		}
		else if (DOWN_NOSPACE == fota_ftp_cli_p->down_state)
		{
			//在当前file_download阶段发生了空间不足的情况,表明net_connect_ready阶段时候获取的目标文件大小 和 当前file_download阶段下载的文件不是同一内容的文件,仅仅同名
			//在net_connect_ready阶段获取了文件大小后 到 file_download阶段 开始真正文件下载前,这一时间间隔当中服务器的文件可能被同名替换,或者在下载文件过程中,文件被同名替换
			//这样的文件和初始目标文件不同,应该被删除

			//目标文件被替换,导致下载FOTA包的空间不足
			QL_FOTA_FTP_LOG("********** The object file on the server has been replaced and there is not enough space left in the FOTA package after downloading the replacement **********");
			//删除文件
			ql_remove(fota_ftp_cli_p->local_filename);
			//下载状态置为初始状态,等待下一次的FTP FOTA下载
			fota_ftp_cli_p->down_state = DOWN_INIT;
			break;
		}
		else if (DOWN_OTHER_ERROR == fota_ftp_cli_p->down_state)
		{
			//ql_ftp_client_get_ex 函数参数配置错误,导致下载FTP文件时发生错误
			QL_FOTA_FTP_LOG("********** The ql_ftp_client_get_ex function parameter configuration error, resulting in an error when downloading FTP files **********");
			break;
		}
		//FTP FOTA文件下载
		fota_ftp_get_file(fota_ftp_cli_p);
		//等待10s继续下载
		ql_rtos_task_sleep_s(10);
	}

	//断开与FTP服务器的连接
	ql_ftp_client_close(fota_ftp_cli_p->ftp_handle);
}

void fota_ftp_file_check(fota_ftp_client_t *fota_ftp_cli_p)
{
    int ret = QL_FTP_FOTA_SUCCESS;
    char *old_version = NULL;
    char *new_version = NULL;
    //文件下载完成,FOTA文件本地校验
    if (DOWN_DOWNED == fota_ftp_cli_p->down_state)
    {
        ret = ql_fota_image_verify(fota_ftp_cli_p->local_filename);
        if (QL_FTP_FOTA_SUCCESS != ret)
        {
            QL_FOTA_FTP_LOG("********** [%s]package is invalid FOTA **********", fota_ftp_cli_p->local_filename);
            //删除无效的FOTA文件
            ret = ql_remove(fota_ftp_cli_p->local_filename);
            fota_ftp_cli_p->down_state = DOWN_INIT;
            if (QL_FTP_FOTA_SUCCESS == ret)
            {
                QL_FOTA_FTP_LOG("********** delete file success **********");
            }
        }
        else
        {
            //校验成功
            QL_FOTA_FTP_LOG("********** download is sucess ,system will reset power! **********");
            
            ret = ql_fota_get_image_version(&old_version,&new_version);
            if(ret)
            {
                QL_FOTA_FTP_LOG("********** verion error **********");
                //do someting
            }
            QL_FOTA_FTP_LOG("old :%s --> new: %s",old_version,new_version);
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
            
            //设置升级
            ql_rtos_task_sleep_s(5);
            ql_power_reset(RESET_NORMAL);
        }
    }
	else 
	{

		QL_FOTA_FTP_LOG("********** download is failed ,remove file! **********");
		ret = ql_remove(fota_ftp_cli_p->local_filename);
		fota_ftp_cli_p->down_state = DOWN_INIT;
		if (QL_FTP_FOTA_SUCCESS == ret)
		{
			QL_FOTA_FTP_LOG("********** delete file success **********");
		}
	}
    return;
}


ql_fota_result_e  fota_ftp_result_process(void)
{
	ql_fota_result_e 	p_fota_result = 0;
	
	//获取升级结果
	if ( ql_fota_get_result(&p_fota_result) != QL_FOTA_SUCCESS )
	{
		QL_FOTA_FTP_LOG("ql_fota_get_result failed ");
		return QL_FOTA_STATUS_INVALID;
	}

	if ( p_fota_result == QL_FOTA_FINISHED )
	{
		QL_FOTA_FTP_LOG("update finished");
		ql_fota_file_reset(TRUE);
		return QL_FOTA_FINISHED;
	}
	else if(p_fota_result == QL_FOTA_READY)
	{
		QL_FOTA_FTP_LOG("fota ready bigen power reset ");
		ql_rtos_task_sleep_s(5);
		ql_power_reset(RESET_NORMAL);
	}
	else if(p_fota_result == QL_FOTA_NOT_EXIST)
	{
		QL_FOTA_FTP_LOG("fota file not exist");
		ql_fota_file_reset(TRUE);
		return QL_FOTA_NOT_EXIST;
	}
	QL_FOTA_FTP_LOG("fota  result status invalid");
	return QL_FOTA_STATUS_INVALID;
}


void fota_ftp_app_thread()
{
    //延迟启动30S
    ql_rtos_task_sleep_s(30);

    int ret = QL_FTP_FOTA_SUCCESS;
	char	version_buf[QL_VERSION_MAX] = {0};
    
	ql_dev_get_firmware_version(version_buf, sizeof(version_buf));

	QL_FOTA_FTP_LOG("current version:  %s", version_buf);

    fota_ftp_client_t *fota_ftp_cli_p = &fota_ftp_cli;

	if(fota_ftp_result_process() == QL_FOTA_FINISHED)
	{
		goto init_error_exit;
	}

    ret = fota_ftp_client_init(fota_ftp_cli_p);
    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        goto init_error_exit;
    }

    ret = fota_ftp_net_connect_ready(fota_ftp_cli_p);
    if (QL_FTP_FOTA_SUCCESS != ret)
    {
        goto exit;
    }

    //向FTP服务器进行FTP FOTA文件下载请求
    fota_ftp_file_download(fota_ftp_cli_p);

    //文件下载完成,FOTA文件本地校验
    fota_ftp_file_check(fota_ftp_cli_p);

exit:
    ql_ftp_client_release(fota_ftp_cli_p->ftp_handle);

init_error_exit:
    QL_FOTA_FTP_LOG("********** exit ql_ftp_fota_demo **********");
    ql_rtos_task_delete(fota_ftp_task);
}

#endif

void ql_fota_ftp_app_init()
{
#ifdef QL_APP_FEATURE_FTP_FOTA

    QL_FOTA_FTP_LOG("ftp fota demo support!");
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&fota_ftp_task, 1024*16, APP_PRIORITY_HIGH, "QfotaFtp", fota_ftp_app_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_FOTA_FTP_LOG("created task failed");
    }

#endif
}
