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
#include <stdint.h>
#include <time.h>

#include "ql_api_osi.h"
#include "ql_api_nw.h"

#include "ql_log.h"
#include "ql_api_datacall.h"
#include "mxml.h"
#include "mxml_demo.h"

#define QL_MXML_DEMO_LOG_LEVEL		QL_LOG_LEVEL_INFO
#define QL_MXML_DEMO_LOG(msg, ...)	QL_LOG(QL_MXML_DEMO_LOG_LEVEL, "ql_mxml_demo", msg, ##__VA_ARGS__)

static ql_task_t mxml_app_task = NULL;


static int xml_parase_form_string(char *TEST_BUF)
{

	mxml_node_t *pHead,*pNode;

	/* 加载 */
	pHead = mxmlLoadString(NULL, TEST_BUF, MXML_NO_CALLBACK);

	/* 查找字段为root的节点 */
	for (pNode = mxmlFindElement(pHead, pHead, "people", NULL, NULL, MXML_DESCEND);
		 pNode != NULL;
		 pNode = mxmlFindElement(pNode, pHead, "people", NULL, NULL, MXML_DESCEND))
	{
		const char *name = mxmlElementGetAttr(pNode, (const char*)"name");
		const char *age = mxmlElementGetAttr(pNode, (const char*)"age");
		const char *jop = mxmlElementGetAttr(pNode, (const char*)"job");
		QL_MXML_DEMO_LOG("--->getname = :%s,getage = :%s,getjob = :%s\n", name, age, jop);
	}
	mxmlDelete(pHead);
    return 0;
}

static void save_xml_file(char *dir_path,char *file_path,char * buff)
{
    int err=0;
    int fd=0;

    err = ql_mkdir(dir_path, 0);
	if(err < 0)
	{
		if(err == QL_DIR_DIR_ALREADY_EXIST)
		{
			QL_MXML_DEMO_LOG("dir exist, not create");
		}
		else
		{
			QL_MXML_DEMO_LOG("make dir failed");
			goto exit;
		}
	}


    fd = ql_fopen(file_path, "wb+");
	if(fd < 0)
	{
		QL_MXML_DEMO_LOG("open file failed");
		err = fd;
		goto exit;
	}



	err = ql_fwrite(buff, strlen(buff) + 1, 1, fd);   //strlen not include '\0'
	if(err < 0)
	{
		QL_MXML_DEMO_LOG("write file failed");
		goto exit;		
	}

    QL_MXML_DEMO_LOG("save test xml file finished.");
    exit:
    if(fd)
    {
        ql_fclose(fd);
    }
    
    
}


static void read_xml_from_file(char *path,char *buff)
{

    int err=0;
    int fd=0;

    fd = ql_fopen(path, "r");
	if(fd < 0)
	{
		QL_MXML_DEMO_LOG("open file failed");
		err = fd;
		goto exit;
	}

    err=ql_fread((void *)buff, 1024, 1, fd);

	if(err == 0)
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
			QL_MXML_DEMO_LOG("read file failed");
		}
		goto exit;
	}
    
    exit:
    if(fd)
    {
        ql_fclose(fd);
    }

}



static void mxml_demo_thread(void * arg)
{

       QL_MXML_DEMO_LOG("============  xml demo test start===================\r\n");

       char s8Buff[1024] = {0};
       ql_rtos_task_sleep_s(5);
       mxml_node_t *pHead = NULL, *pBody = NULL, *pPeople = NULL, *pNode = NULL;
       /* 创建xml文件 */
       pHead = mxmlNewXML("1.0");
       /* 创建节点root */
       pBody = mxmlNewElement(pHead, "root");
       /* 在root节点下创建子节点num */
       pNode = mxmlNewElement(pBody, "num");
       mxmlNewText(pNode, 0, "3"); //num节点创建文本内容
       /* 在root节点下创建子节点people */
       pPeople = mxmlNewElement(pBody, "people");
       /* 在people节点下创建子节点name */
       //pNode = mxmlNewElement(pPeople, "name");
       mxmlElementSetAttrf(pPeople, "name", "zhangsan");
       mxmlElementSetAttrf(pPeople, "age", "23");
       mxmlElementSetAttrf(pPeople, "job", "staff");
       /* 在root节点下创建子节点people */
       pPeople = mxmlNewElement(pBody, "people");
       mxmlElementSetAttrf(pPeople, "name", "lisi");
       mxmlElementSetAttrf(pPeople, "age", "20");
       mxmlElementSetAttrf(pPeople, "job", "student");
       /* 在root节点下创建子节点people */
       pPeople = mxmlNewElement(pBody, "people");
       mxmlElementSetAttrf(pPeople, "name", "wangwu");
       mxmlElementSetAttrf(pPeople, "age", "40");
       mxmlElementSetAttrf(pPeople, "job", "teacher");
       /* 保存生成的xml数据 */
       mxmlSaveString(pHead, s8Buff, sizeof(s8Buff), MXML_NO_CALLBACK);
       
       
       QL_MXML_DEMO_LOG("xml data strlen:%d,content:%s", strlen(s8Buff),s8Buff);
      
  
       /* 删除节点，释放内存 */
       mxmlDelete(pHead);

       QL_MXML_DEMO_LOG("xml parase test start");

       QL_MXML_DEMO_LOG("save and read xml file test start");
       save_xml_file(XML_DIR_PATH,XML_FILE_PATH,s8Buff);
       
       memset(s8Buff,0,sizeof(s8Buff));

       read_xml_from_file(XML_FILE_PATH,s8Buff);
       QL_MXML_DEMO_LOG("xml data strlen:%d,content:%s", strlen(s8Buff),s8Buff);

       xml_parase_form_string(s8Buff);
       ql_rtos_task_delete(mxml_app_task);
       return;	

}


int ql_mxml_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
    err = ql_rtos_task_create(&mxml_app_task, 8*1024, 23, "mxml_demo_app", mxml_demo_thread, NULL, 1);
	if(err != QL_OSI_SUCCESS)
    {
		QL_MXML_DEMO_LOG("mxml demp task create failed");
	}

	return err;
}

