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


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_log.h"
#include "zip_demo.h"
#include "ql_zip.h"
#include "ql_fs.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_ZIPDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_ZIPDEMO_LOG(msg, ...)         QL_LOG(QL_ZIPDEMO_LOG_LEVEL, "ql_ZIPDEMO", msg, ##__VA_ARGS__)
#define QL_ZIPDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_ZIPDEMO", msg, ##__VA_ARGS__)
#define ZIP_FILE_LEN    1024
#define QL_ZIP_UNZIP_TYPE                0       //0:unzip to file 1:unzip to memery 2:unzip to memery ex
/*===========================================================================
 * Variate
 ===========================================================================*/
const char *unZipFilePath="UFS:test.zip";
char *unZip_path=NULL;
const char *TestFilePath[2]={"UFS:test01.txt","UFS:test02.txt"};
const char *TestFilezipname[2]={"test01.txt","test02.txt"};
int unzip_result=QL_FILE_OK;
int dl_size;
int upl_size;
ql_task_t zip_task = NULL;
/*===========================================================================
 * Functions
 ===========================================================================*/
static void ql_zip_demo_thread(void *param)
{
 	int ret = 0;
    int run_num=0;
    HZIP hz=0;
    QFILE fd[2] = {0};
    
    unsigned char *zip_fs_space=calloc(10,ZIP_FILE_LEN);
    if(NULL==zip_fs_space)
    {
        QL_ZIPDEMO_LOG("malloc err\r\n");
        return; 
    }
	ql_rtos_task_sleep_s(5);
    QL_ZIPDEMO_LOG("========== zip demo start ==========\r\n");
    QL_ZIPDEMO_LOG("========== start create Test Files:[%s][%s]==========\r\n",TestFilePath[0],TestFilePath[1]);
    fd[0] = ql_fopen(TestFilePath[0],"w+");
    fd[1] = ql_fopen(TestFilePath[1],"w+");
    if(fd[0] < 0||fd[1] < 0)
    {
        QL_ZIPDEMO_LOG("open file failed !\r\n");
        QL_ZIPDEMO_LOG("========== Create Test Files:[%s][%s] fail==========\r\n",TestFilePath[0],TestFilePath[1]);
        goto main_exit;
    }
    else 
    {
        QL_ZIPDEMO_LOG("open file:[%d][%d] success!\r\n",fd[0],fd[1]);
    }
    memset(zip_fs_space,'a',10*ZIP_FILE_LEN);
    ret = ql_fwrite(zip_fs_space, 10*ZIP_FILE_LEN, 1, fd[0]);
    if(ret > 0)
	{
		QL_ZIPDEMO_LOG("file:test01.txt write :%d bytes data\r\n", ret);
	}
    else
    {
        ql_fclose(fd[0]);
        ql_fclose(fd[1]);
        QL_ZIPDEMO_LOG("file:test01.txt write Failed\r\n");
        QL_ZIPDEMO_LOG("========== Create Test Files:[%s][%s] Failed==========\r\n",TestFilePath[0],TestFilePath[1]);
        goto main_exit;
    }
    memset(zip_fs_space,'b',5*ZIP_FILE_LEN);
    ret = ql_fwrite(zip_fs_space, 5*ZIP_FILE_LEN, 1, fd[1]);
    if(ret > 0)
	{
		QL_ZIPDEMO_LOG("file:test02.txt write :%d bytes data\r\n", ret);
	}
    else
    {
        ql_fclose(fd[0]);
        ql_fclose(fd[1]);
        QL_ZIPDEMO_LOG("file:test02.txt write Failed\r\n");
        QL_ZIPDEMO_LOG("========== Create Test Files:[%s][%s] Failed==========\r\n",TestFilePath[0],TestFilePath[1]);
        goto main_exit;
    }
    ql_fclose(fd[0]);
    ql_fclose(fd[1]);
    QL_ZIPDEMO_LOG("========== Create Test Files:[%s][%s] success==========\r\n",TestFilePath[0],TestFilePath[1]);
	ql_rtos_task_sleep_s(5); 
    
    for(run_num=0;run_num<50;run_num++)
    {
          QL_ZIPDEMO_LOG("========== loop_test[%d] start==========\r\n",run_num+1);
          QL_ZIPDEMO_LOG("========== start zip file[%d]==========\r\n",run_num+1);
          
          hz = ql_fs_zip_create(unZipFilePath,0,&ret);
          if (ret<0)
          {
               QL_ZIPDEMO_LOG("create empty.zip:[%s] Failed,errcode=%d\r\n",unZipFilePath,ret);
               goto main_exit;
          }
          else
          {
               QL_ZIPDEMO_LOG("create empty.zip:[%s] Success\r\n",unZipFilePath);
          }
         
          ret=ql_fs_zip_add_file(hz,TestFilezipname[0],TestFilePath[0]); 
          if (ret!=QL_FILE_OK) 
          {
               QL_ZIPDEMO_LOG("add [%s] to zip Failed\r\n",TestFilePath[0]);
               goto main_exit;
          }
          QL_ZIPDEMO_LOG("add [%s] to zip Success\r\n",TestFilePath[0]);

          ret=ql_fs_zip_add_file(hz,TestFilezipname[1],TestFilePath[1]); 
          if (ret!=QL_FILE_OK) 
          {
               QL_ZIPDEMO_LOG("add [%s] to zip Failed\r\n",TestFilePath[1]);
               goto main_exit;
          }
          QL_ZIPDEMO_LOG("add [%s] to zip Success\r\n",TestFilePath[1]);

          ql_fs_zip_close(hz);   
#if (QL_ZIP_UNZIP_TYPE == 1 || QL_ZIP_UNZIP_TYPE == 2)
          //解压到内存
          ql_rtos_task_sleep_s(5);
#if QL_ZIP_UNZIP_TYPE == 1
          ql_unzip_mem_data_s *ql_unzip_tmp = (ql_unzip_mem_data_s *)calloc(1, sizeof(ql_unzip_mem_data_s));
#else
          ql_unzip_mem_data_ex_s *ql_unzip_tmp = (ql_unzip_mem_data_ex_s *)calloc(1, sizeof(ql_unzip_mem_data_ex_s));
#endif
          if (ql_unzip_tmp == NULL)
          {
                QL_ZIPDEMO_LOG("malloc err");
                goto main_exit;
          }
          
          ql_unzip_tmp->data = (void *)calloc(1, 15*ZIP_FILE_LEN);
          if (ql_unzip_tmp->data == NULL)
          {
                QL_ZIPDEMO_LOG("malloc err");
                free(ql_unzip_tmp);
                goto main_exit;
          }

          ql_unzip_tmp->total_len = 15*ZIP_FILE_LEN;
#if QL_ZIP_UNZIP_TYPE == 1
          //type为1时，压缩包内文件数量最大32，超过则解压报错
          ret = ql_fs_unzip_ex(unZipFilePath, QL_UNZIP_TO_MEMERY, ql_unzip_tmp);
#else
          ql_zip_info_s zip_info = {0};
          ql_fs_zip_get_info(unZipFilePath, &zip_info);
          QL_ZIPDEMO_LOG("zip item:%d", zip_info.item_num);
          //压缩包内文件条目数量，包括压缩包本身，有多少数量则申请多少空间，申请的空间数必须大于等于实际条目，否则可能会造成dump
          ql_unzip_tmp->item = (ql_unzip_item_data_s *)calloc(zip_info.item_num, sizeof(ql_unzip_item_data_s));
          if (ql_unzip_tmp->item == NULL)
          {
              QL_ZIPDEMO_LOG("malloc err");
              free(ql_unzip_tmp->data);
              free(ql_unzip_tmp);
              goto main_exit;
          }
          //type为2时，压缩包内文件数量数量不限制，由客户侧决定，ql_unzip_tmp->item_num决定解压数量，为0时，默认全部解压
          ret = ql_fs_unzip_ex(unZipFilePath, QL_UNZIP_TO_MEMERY_EX, ql_unzip_tmp);
#endif
          if(ret!=QL_FILE_OK)
          {
                QL_ZIPDEMO_LOG("unzip file:[%s] Failed",unZipFilePath);
                free(ql_unzip_tmp->data);
                free(ql_unzip_tmp);
                goto main_exit;
          }
          QL_ZIPDEMO_LOG("used_len=%d,item_num=%d",ql_unzip_tmp->used_len, ql_unzip_tmp->item_num);
          
          int j=0;
          int i = 0;
          char *tmep = NULL;
          for (j=0; j<ql_unzip_tmp->item_num; j++)
          {
              QL_ZIPDEMO_LOG("file path=%s", ql_unzip_tmp->item[j].file_path);
              QL_ZIPDEMO_LOG("file len=%d,type=%d", ql_unzip_tmp->item[j].item_len, ql_unzip_tmp->item[j].file_type);
              tmep  = (char *)ql_unzip_tmp->item[j].item_data;
              if (tmep && ql_unzip_tmp->item[j].file_type == QL_UNZIP_FILE)
              {
                    if (0 == strcmp(ql_unzip_tmp->item[j].file_path, TestFilePath[0]))
                    {
                        for (i=0; i<ql_unzip_tmp->item[j].item_len; i++)
                        {
                             if (tmep[i] != 'a')
                             {
                                  QL_ZIPDEMO_LOG("not match -----------");
                                  break;
                             }
                        }
                    }

                    if (0 == strcmp(ql_unzip_tmp->item[j].file_path, TestFilePath[1]))
                    {
                        for (i=0; i<ql_unzip_tmp->item[j].item_len; i++)
                        {
                             if (tmep[i] != 'b')
                             {
                                  QL_ZIPDEMO_LOG("not match -----------");
                                  break;
                             }
                        }
                    }    
              }           
          }
#if QL_ZIP_UNZIP_TYPE == 2         
          free(ql_unzip_tmp->item);
#endif
          free(ql_unzip_tmp->data);
          free(ql_unzip_tmp);
          QL_ZIPDEMO_LOG("unzip file:[%s] Success\r\n",unZipFilePath);
#else 
          //解压到文件系统
          QL_ZIPDEMO_LOG("========== zip file[%d] success==========\r\n",run_num+1);
          ql_rtos_task_sleep_s(5);           
          QL_ZIPDEMO_LOG("========== start unzip file[%d]==========\r\n",run_num+1);
          
          ret=ql_fs_unzip(unZipFilePath,unZip_path,1);
          //ql_unzip_file_data_s ql_unzip_file_tmp = {0};
          //ql_unzip_file_tmp.mode = 1;
          //ql_unzip_file_tmp.path_name = unZip_path;
          //ret = ql_fs_unzip_ex(unZipFilePath, QL_UNZIP_TO_FILE, &ql_unzip_file_tmp);
          if(ret!=QL_FILE_OK)
          {
                QL_ZIPDEMO_LOG("unzip file:[%s] Failed\r\n",unZipFilePath);
                goto main_exit;
          }
          
          QL_ZIPDEMO_LOG("unzip file:[%s] Success\r\n",unZipFilePath);

#endif
          QL_ZIPDEMO_LOG("========== unzip file[%d] success==========\r\n",run_num+1);

          ql_rtos_task_sleep_s(5);
    }
main_exit:
    QL_ZIPDEMO_LOG("========== zip demo end ==========\r\n");
    ql_remove(TestFilePath[0]); 
    ql_remove(TestFilePath[1]); 
    if(zip_fs_space!=NULL)
    {
       free(zip_fs_space);
       zip_fs_space = NULL;
    } 
    ql_rtos_task_delete(NULL);	
    return;
}

void ql_zip_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&zip_task, 8*4096, APP_PRIORITY_NORMAL, "ql_zipdemo", ql_zip_demo_thread, NULL, 5);
    if( err != QL_OSI_SUCCESS )
    {
        QL_ZIPDEMO_LOG("zip demo task created failed");
    }
}




