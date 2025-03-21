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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "vfs.h"
#include "ql_ftp_client.h"

#include "ql_api_osi.h"
#include "ql_api_nw.h"

#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_ftp_client.h"
#include "ql_fs.h"
#include "ql_ssl.h"


#define QL_SFTP_LOG_LEVEL                QL_LOG_LEVEL_INFO
#define QL_SFTP_LOG(msg, ...)			QL_LOG(QL_SFTP_LOG_LEVEL, "ql_SFTP", msg, ##__VA_ARGS__)
#define QL_SFTP_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_SFTP", msg, ##__VA_ARGS__)

static ql_task_t sftp_task = NULL;

#define MAX_PATH_SIZE		256
static int lizhu_put_len;


static size_t quec_exec_sftp_get_cb(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t realsize = size * nmemb;
	char* printf_buff = NULL;
	printf_buff = (char*)malloc(realsize + 1);
	if (printf_buff == NULL)
		return 0;

	memset(printf_buff, 0, realsize + 1);
	memcpy(printf_buff, ptr, realsize + 1);
	printf_buff[realsize] = '\0';

	QL_SFTP_LOG("quec_exec_ftp_get_cb== %d  %s\n", realsize, printf_buff);
    free(printf_buff);
	return realsize;
}
static size_t quec_exec_sftp_put_cb(void* ptr, size_t size, size_t nmemb, void* stream)
{
	char buff[1000] = { 0 };
	memset(buff, 0x65, 1000);

	QL_SFTP_LOG("quec_exec_ftp_put_cb \n");
	if (lizhu_put_len < 1000)
	{
		lizhu_put_len = 1000;
		memcpy(ptr, buff, 1000);
		return 1000;
	}
	else
	{
		QL_SFTP_LOG("return 1000 success\n");
	}
	lizhu_put_len = 0;
	return 0;
}

static void sftp_app_thread(void *arg)
{
    int ret = 0;
	int i = 0;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	void * client = NULL;
	char buf[MAX_PATH_SIZE]={0};
    int run_num=1;
	char hostname[64]="220.180.239.252:22";
	char username[]="admin";
	char password[]="admin";
	QL_FTP_CLIENT_FILE_INFO_T ql_list_file[5]={0};
	uint8_t nSim = 0;
	uint16_t sim_cid;

	ql_rtos_task_sleep_s(10);
	QL_SFTP_LOG("========== ftp demo start ==========");
	QL_SFTP_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		QL_SFTP_LOG("====network registered!!!!====");
	}else{
		QL_SFTP_LOG("====network register failure!!!!!====");
		goto exit;
	}

	if(QL_DATACALL_SUCCESS != ql_bind_sim_and_profile(nSim, profile_idx, &sim_cid))
	{
		QL_SFTP_LOG("nSim or profile_idx is invalid!!!!");
		ret = -1;
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	QL_SFTP_LOG("===start data call====");
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
	QL_SFTP_LOG("===data call result:%d", ret);
	if(ret != 0){
		QL_SFTP_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0){
		QL_SFTP_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    QL_SFTP_LOG("info->profile_idx: %d", info.profile_idx);
	QL_SFTP_LOG("info->ip_version: %d", info.ip_version);
            
	QL_SFTP_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_SFTP_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_SFTP_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_SFTP_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);


	//ql_rtos_task_create(&ftp_task1, 16 * 1024, 23, "sftp_app1", sftp_app_thread, NULL, 5);

    run_num = 0;
	client = ql_ftp_client_new();
	if( client == NULL )
	{
		QL_SFTP_LOG("client init fail");
		goto exit;
	}
	ql_ftp_client_setopt(client, QL_FTP_CLIENT_SIM_CID, sim_cid);              /*set PDP cid,if not set,using default PDP*/
	ql_ftp_client_setopt(client, QL_FTP_CLIENT_SSL_ENABLE, QL_FTP_CLIENT_SSL_TYPE_SSH);

	ret=ql_ftp_client_open(client,hostname,username,password);
	QL_SFTP_LOG("\nql_ftp_client_open[%s] ret=%d;\n",hostname,ret);
	if(ret!=0)
	{
		QL_SFTP_LOG("client connect fail");
		goto exit;
	}
	
	do {
		
		ret=ql_ftp_client_pwd(client, buf, sizeof(buf));
		QL_SFTP_LOG("\nql_ftp_client_pwd ret=%d; remotedir:%s\n",ret,buf);
		ret=ql_ftp_client_cwd(client, "/temp");
		QL_SFTP_LOG("\nql_ftp_client_cwd[temp] ret=%d\n",ret);
	
		ret = ql_ftp_client_list(client, ".", NULL, ql_list_file, 5);
		if (ret < 0)
		{
			QL_SFTP_LOG("ql_ftp_client_list error ret = %d", ret);
			break;
		}
		QL_SFTP_LOG("read list number = %d", ret);
		for(i=0;i<ret;i++)
        {
            	QL_SFTP_LOG("file%d:%s\n   type:%d\n   size:%d\n   group:%s\n   permissions:%d\n   time:%04d-%02d-%02d %02d:%02d:%02d\n",i,
            		ql_list_file[i].filename,
            		ql_list_file[i].type, 
            		ql_list_file[i].size,
            		ql_list_file[i].group,
            		ql_list_file[i].permissions,
            		ql_list_file[i].timestamp.tm_year+1900,
            		ql_list_file[i].timestamp.tm_mon+1,
            		ql_list_file[i].timestamp.tm_mday,
            		ql_list_file[i].timestamp.tm_hour,
            		ql_list_file[i].timestamp.tm_min,
            		ql_list_file[i].timestamp.tm_sec
            		);
         }
		QL_SFTP_LOG("ql_ftp_client_list over remotdir\n");
	
		//ret = ql_ftp_client_put_ex(client, "UFS:20201210test.txt","/temp/1.txt", quec_exec_ftp_put_cb, NULL);
		ret = ql_ftp_client_put_ex(client, NULL,"/temp/1.txt", quec_exec_sftp_put_cb, NULL);
		if (ret != 0)
		{
			QL_SFTP_LOG("ql_ftp_client_put_ex error ret = %d", ret);
			break;
		}
		QL_SFTP_LOG("ql_ftp_client_put_ex over");
		ret = ql_ftp_client_get_ex(client, "/temp/1.txt", NULL, quec_exec_sftp_get_cb, NULL);
		if (ret != 0)
		{
			QL_SFTP_LOG("ql_ftp_client_get_ex error ret = %d", ret);
			break;
		}

		QL_SFTP_LOG("ql_ftp_client_get_ex over");
		ret = ql_ftp_client_delete(client, "/temp/1.txt");
		if (ret != 0)
		{
			QL_SFTP_LOG("ql_ftp_client_delete error ret = %d", ret);
			break;
		}
		QL_SFTP_LOG("ql_ftp_client_delete over");

		ret = ql_ftp_client_mkdir(client, "/temp/we");
		if (ret != 0)
		{
			QL_SFTP_LOG("ql_ftp_client_mkdir error ret = %d", ret);
			break;
		}
		QL_SFTP_LOG("ql_ftp_client_mkdir over");

		ret = ql_ftp_client_rmdir(client, "/temp/we");
		if (ret != 0)
		{
			QL_SFTP_LOG("ql_ftp_client_rmdir error ret = %d", ret);
			break;
		}
		QL_SFTP_LOG("ql_ftp_client_rmdir over");

		run_num++;
		QL_SFTP_LOG("==============sftp_client_test loop[%d]================\n",run_num);		
		ql_rtos_task_sleep_s(5); 
	}while(run_num < 10);
    do{
		QL_SFTP_LOG("==============sftp_client_test idle================\n");		
		ql_rtos_task_sleep_s(5);
	}while(1);
exit:
    if (client != NULL)
    {
        ql_ftp_client_release(client); /*release http resources*/
    }
   	ql_rtos_task_delete(sftp_task);	
	return;
}



int ql_sftp_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&sftp_task, 16 * 1024, APP_PRIORITY_ABOVE_NORMAL, "QsftpApp", sftp_app_thread, NULL, 5);
    if (err != QL_OSI_SUCCESS)
    {
        QL_SFTP_LOG("sftp_app init failed");
    }

    return 0;
}

