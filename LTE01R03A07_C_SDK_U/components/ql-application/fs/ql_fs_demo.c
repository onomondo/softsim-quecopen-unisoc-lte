
/**  @file
  ql_fs_demo.c

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
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

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_fs_demo.h"
#include "ql_fs.h"
#include "ql_api_dev.h"

#define QL_FS_DEMO_LOG_LEVEL		QL_LOG_LEVEL_INFO
#define QL_FS_DEMO_LOG(msg, ...)	QL_LOG(QL_FS_DEMO_LOG_LEVEL, "ql_fs_demo", msg, ##__VA_ARGS__)

ql_task_t ql_fs_task = NULL;


#define QL_FS_COPY_FILE_BUFF_SIZE			(10240)
#define QL_FS_COPY_FILE_FEED_DOG_SIZE		(1024*110)		/* 写110K数据喂一次看门狗，避免写大文件看门狗复位 */
int ql_fs_copy_file(const char * sour_file_name, const char * dest_file_name)
{
	int sour_fd = 0, dest_fd = 0;
	int64 err = QL_FILE_OK;
	void * data_buff = NULL;

	sour_fd = ql_fopen(sour_file_name, "r");
	if(sour_fd < 0)
	{
		QL_FS_DEMO_LOG("open sour file failed");
		err = sour_fd;
		goto exit;
	}

	dest_fd = ql_fopen(dest_file_name, "w+");
	if(dest_fd < 0)
	{
		QL_FS_DEMO_LOG("open dest file failed");
		err = dest_fd;
		goto exit;
	}

	data_buff = malloc(QL_FS_COPY_FILE_BUFF_SIZE);
	while(TRUE)
	{
		err = ql_fread(data_buff, QL_FS_COPY_FILE_BUFF_SIZE, 1, sour_fd);
		if(err > 0)
		{
			static int i = 1;
			err = ql_fwrite(data_buff, err, 1, dest_fd);
			if(err < 0)
			{
				QL_FS_DEMO_LOG("write file failed");
				goto exit;
			}
			if(i * QL_FS_COPY_FILE_BUFF_SIZE >= QL_FS_COPY_FILE_FEED_DOG_SIZE)
			{
				ql_dev_feed_wdt();
				i = 0;
			}
			i++;
		}
		else if(err == 0)
		{
			goto exit;
		}
		else if(err < 0)
		{
			if(QL_FILE_READ_ZERO == err)
			{
				err = QL_FILE_OK;
			}
			else
			{
				QL_FS_DEMO_LOG("read file failed");
			}
			goto exit;
		}
	}

exit:
	if(sour_fd > 0)
	{
		ql_fclose(sour_fd);
	}
	if(dest_fd > 0)
	{
		ql_fclose(dest_fd);
	}
	if(NULL != data_buff)
	{
		free(data_buff);
	}
	return err;
}

void ql_fs_callback(void *ctx)
{
	int fd = 0;
	int err = QL_FILE_OK;
	char buffer[100] = {0};
	char *str = TEST_STR;

	ql_rtos_task_sleep_s(15);

	err = ql_mkdir(DIR_PATH, 0);
	if(err < 0)
	{
		if(err == QL_DIR_DIR_ALREADY_EXIST)
		{
			QL_FS_DEMO_LOG("exist");
		}
		else
		{
			QL_FS_DEMO_LOG("failed");
			goto exit;
		}
	}

	/* open encrypted file to write*/
	fd = ql_fopen(FILE_PATH, "ew");
	if(fd < 0)
	{
		QL_FS_DEMO_LOG("failed");
		err = fd;
		goto exit;
	}

	/* write encrypted file */
	err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;		
	}
	ql_fclose(fd);

	/* open encrypted file to read*/
	fd = ql_fopen(FILE_PATH, "er");
	if(fd < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;
	}

	/* read encrypted file */
	memset(buffer, 0, 100);
	err = ql_fread(buffer, ql_fsize(fd), 1, fd);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;					
	}

	QL_FS_DEMO_LOG("encrypted:%s", buffer);
	ql_fclose(fd);

	/* write common file */
	fd = ql_fopen(FILE_PATH, "wb+");
	if(fd < 0)
	{
		QL_FS_DEMO_LOG("failed");
		err = fd;
		goto exit;
	}

	err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;		
	}

	/* read common file */
	err = ql_frewind(fd);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;					
	}

	memset(buffer, 0, 100);
	err = ql_fread(buffer, ql_fsize(fd), 1, fd);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;					
	}

	QL_FS_DEMO_LOG("common:%s", buffer);
	ql_fclose(fd);

	err = ql_nvm_fwrite(TEST_NVM, str, strlen(str) + 1, 1);  //nvm 配置文件不需要输入路径
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}

	memset(buffer, 0, 100);
	err = ql_nvm_fread(TEST_NVM, buffer, strlen(str) + 1, 1);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}	
    
    QL_FS_DEMO_LOG("nvm:%s", buffer);

    err = ql_cust_nvm_fwrite(str, strlen(str) + 1, 1);  //客户nvm 配置文件不需要输入文件名，使用空间不大于1KB 
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}

	memset(buffer, 0, 100);
	err = ql_cust_nvm_fread(buffer, strlen(str) + 1, 1);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}
	QL_FS_DEMO_LOG("cust nvm:%s", buffer);   

	memset(buffer, 0, 100);
	err = ql_fac_cust_nvm_fread(0, buffer, 100, 1);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}
	QL_FS_DEMO_LOG("fac cust nvm:%s", buffer);
	err = ql_fs_copy_file(FILE_PATH, DEST_FILE_PATH);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		goto exit;
	}

	fd = ql_fopen(DEST_FILE_PATH, "r");
	if(fd < 0)
	{
		QL_FS_DEMO_LOG("failed");
		err = fd;
		goto exit;
	}

	memset(buffer, 0, 100);
	err = ql_fread(buffer, ql_fsize(fd), 1, fd);
	if(err < 0)
	{
		QL_FS_DEMO_LOG("failed");
		ql_fclose(fd);
		goto exit;					
	}

	QL_FS_DEMO_LOG("copyed:%s", buffer);
	ql_fclose(fd);

	ql_rmdir_ex(DIR_PATH); //删除DIR_PATH目录和目录里面的文件

exit:
	
	if(err < 0)
	{
		QL_FS_DEMO_LOG("errcode is %x", err);
	}
	
	QL_FS_DEMO_LOG("exit file demo");
	ql_rtos_task_delete(NULL);
}


int ql_fs_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;
	
	err = ql_rtos_task_create(&ql_fs_task, QL_FS_TASK_STACK_SIZE, QL_FS_TASK_PRIO, "ql_fs_task", ql_fs_callback, NULL, QL_FS_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_FS_DEMO_LOG("file_task created failed");
	}

	return err;
}


