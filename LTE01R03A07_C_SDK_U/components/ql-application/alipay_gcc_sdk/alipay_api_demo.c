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
Note: Don't delete BT firmware from the file system
=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_app_feature_config.h"

#include "ql_api_osi.h"
#include "ql_log.h"
#include "alipay_api_demo.h"
#include "aid_code.h"
#include "vendor_os.h"
#include "vendor_device_info.h"
#include "alipay_net_kal.h"
#include "vendor_file.h"
#include "hs_private.h"

//demo只是用来验证适配SDK的API功能，不属于ALIPAY SDK里面功能

#define QL_ALIPAY_API_LOG_LEVEL         	QL_LOG_LEVEL_INFO
#define QL_ALIPAY_API_LOG(msg, ...)			QL_LOG(QL_ALIPAY_API_LOG_LEVEL, "ql_alipay_api", msg, ##__VA_ARGS__)
#define QL_ALIPAY_API_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_alipay_api", msg, ##__VA_ARGS__)


ql_task_t ql_alipay_api_task = NULL;


void ql_alipay_api_test()
{
    char *ql_test_data = NULL;
    ql_test_data = (char *)alipay_malloc(20);
    if (ql_test_data)
    {
        alipay_log_ext("alipay_malloc %p", ql_test_data);

        alipay_free(ql_test_data);
        ql_test_data = NULL;
    }
    
    ql_test_data = (char *)alipay_calloc(1, 20);
    if (ql_test_data)
    {
        alipay_log_ext("alipay_calloc %p", ql_test_data);
        ql_test_data = alipay_realloc(ql_test_data, 50);
        if (ql_test_data)
        {
            alipay_log_ext("alipay_realloc %p", ql_test_data);
            alipay_free(ql_test_data);
            ql_test_data = NULL;
        }
        
    }

    uint32_t time_s = alipay_get_timestamp();
    alipay_log_ext("alipay_get_timestamp-[%d]",time_s);
    if(time_s < 1694598755)
    {
        alipay_log_ext("alipay_get_timestamp time is invalid");
    }

    alipay_set_system_time(time_s + 60*60);
    uint32_t time_adjust = alipay_get_timestamp();
    alipay_log_ext("alipay_get_timestamp-adjust[%d-%d]",time_s, time_adjust);
    if(time_adjust < (time_s+60*60))
    {
        alipay_log_ext("alipay_set_system_time is invalid");
    }

    uint32_t compile_timestamp = alipay_get_compile_timestamp();
    alipay_log_ext("alipay_get_compile_timestamp-[%d]",compile_timestamp);
    if(compile_timestamp < 1694598755)
    {
        alipay_log_ext("alipay_get_compile_timestamp time is invalid");
    }


    alipay_iot_local_time local_time;
    alipay_iot_get_local_time(&local_time);
    alipay_log_ext("%d %d %d %d %d %d", local_time.year, local_time.month, local_time.day, local_time.hour, local_time.minute, local_time.second);
    if (local_time.year < 2023)
    {
        alipay_log_ext("alipay_iot_get_local_time year is invalid");
    }
    if( local_time.month<1 || local_time.month>12)
    {
        alipay_log_ext("alipay_iot_get_local_time month is invalid");
    }
    if(local_time.day<1 || local_time.day>31)
    {
        alipay_log_ext("alipay_iot_get_local_time day is invalid");
    }
    if(local_time.hour > 23)
    {
        alipay_log_ext("alipay_iot_get_local_time hour is invalid");
    }
    if(local_time.minute>59)
    {
        alipay_log_ext("alipay_iot_get_local_time minute is invalid");
    }
    if(local_time.second>59)
    {
        alipay_log_ext("alipay_iot_get_local_time second is invalid");
    }

    uint8_t rang_data[5];
    get_Rng_number(rang_data, 5);

    retval_e ret = ALIPAY_RV_OK;
    uint8_t bt_mac[18]={0,};
    uint32_t len_mac = sizeof(bt_mac);
    if((ret = alipay_get_mac(bt_mac, &len_mac)) != ALIPAY_RV_OK)
    {
        alipay_log_ext("fail to get mac");
    }
    if(len_mac != 17)
    {
        alipay_log_ext("mac's len is not 17-[%d]", len_mac);
    }
    alipay_log_ext("mac[%d]:%s", len_mac, bt_mac);

    #define SN_MAX_LEN      32
    uint8_t buf_sn[40]={0,};
    uint32_t len_buf_sn = sizeof(buf_sn);
    if((ret = alipay_get_sn(buf_sn, &len_buf_sn)) != ALIPAY_RV_OK)
    {
        alipay_log_ext("fail to get sn");
    }
    if(len_buf_sn > SN_MAX_LEN)
    {
        alipay_log_ext("sn's len is longer than 32-[%d]", len_buf_sn);
    }
    alipay_log_ext("sn[%d]:%s", len_buf_sn, buf_sn);

    #define IMEI_MAX_LEN      48
    uint8_t buf_temp[56]={0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((ret = alipay_get_imei(buf_temp, &len_buf_temp)) != ALIPAY_RV_OK)
    {
        alipay_log_ext("fail to get imei");
    }
    if(len_buf_temp > IMEI_MAX_LEN)
    {
        alipay_log_ext("imei's len is longer than 48-[%d]", len_buf_temp);
    }
    alipay_log_ext("imei[%d]:%s", len_buf_temp, buf_temp);

    #define COMPANY_NAME_MAX_LEN      15
    uint8_t company_temp[56]={0,};
    uint32_t len_company_temp = sizeof(company_temp);
    if((ret = alipay_get_companyName(company_temp, &len_company_temp)) != ALIPAY_RV_OK)
    {
        alipay_log_ext("fail to get company name");
    }
    if(len_company_temp > COMPANY_NAME_MAX_LEN)
    {
        alipay_log_ext("company name's len is longer than 15-[%d]", len_company_temp);
    }
    alipay_log_ext("company name[%d]:%s", len_company_temp, company_temp);

    #define PRODUCT_MODEL_MAX_LEN      23
    uint8_t product_temp[56]={0,};
    uint32_t product_buf_temp = sizeof(product_temp);
    if((ret = alipay_get_productModel(product_temp, &product_buf_temp)) != ALIPAY_RV_OK)
    {
        alipay_log_ext("fail to get product model");
    }
    if(len_buf_temp > PRODUCT_MODEL_MAX_LEN)
    {
        alipay_log_ext("product model's len is longer than 23-[%d]", product_buf_temp);
    }
    alipay_log_ext("product model[%d]:%s", len_buf_temp, product_temp);


    alipay_iot_timeval now;
    alipay_iot_gettimeofday(&now);
    alipay_log_ext("alipay_iot_gettimeofday-[%d]-[%d]",now.tv_sec,now.tv_usec);
    if(now.tv_sec < 1694598755)
    {
        alipay_log_ext("alipay_iot_gettimeofday time is invalid");
    }

    alipay_iot_timeval now_t1, now_t2;
    uint32_t time_diff = 0;
    alipay_iot_gettimeofday(&now_t1);
    alipay_task_sleep(1100);
    alipay_iot_gettimeofday(&now_t2);
    time_diff = now_t2.tv_sec - now_t1.tv_sec;
    if((time_diff<1) || (time_diff>2))
    {
        alipay_log_ext("alipay_iot_gettimeofday is error[%d]", time_diff);
    }

    int mutex_ret = 0;
    alipay_iot_mutex mutex = alipay_iot_mutex_create("ALIPAY_M");
    if (mutex)
    {
        alipay_log_ext("alipay create lock");
        mutex_ret = alipay_iot_mutex_lock(mutex);
        if (mutex_ret < 0)
        {
            alipay_log_ext("alipay lock invalid");
        }
        alipay_task_sleep(100);
        mutex_ret = alipay_iot_mutex_unlock(mutex);
        if (mutex_ret < 0)
        {
            alipay_log_ext("alipay unlock invalid");
        }
        mutex_ret = alipay_iot_mutex_delete(mutex);
        if (mutex_ret < 0)
        {
            alipay_log_ext("alipay del lock invalid");
        }
        alipay_log_ext("alipay lock end");
    }
    else 
    {
        alipay_log_ext("alipay create lock invalid");
    }


    #define TEST_FILE_NAME      "test_file_name_long_long_long_long"
    #define TEST_FILE_DATA      "qwertyuiopasdfghjklzxcvbnm1234567890mnbvcxzlkjhgfdsaqwertyuiop"
    void* fd = NULL;
    alipay_log_ext("alipay file test start");
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0)
    {
        alipay_log_ext("fail to open file");
        goto exit;
    }
    if(alipay_write_rsvd_part(fd, TEST_FILE_DATA, strlen(TEST_FILE_DATA)+1) != 0)
    {
        alipay_log_ext("fail to write");
        goto exit;
    }
    if(alipay_close_rsvd_part(fd) != 0)
    {
        alipay_log_ext("fail to close");
        goto exit;
    }
    fd = NULL;

    uint8_t file_temp[128] = {0,};
    uint32_t len_file_temp = sizeof(file_temp);
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0)
    {
        alipay_log_ext("fail to open file");
        goto exit;
    }
    if((alipay_read_rsvd_part(fd, file_temp, &len_file_temp) != 0) || (len_file_temp == 0) || (len_file_temp != (strlen(TEST_FILE_DATA)+1)))
    {
        alipay_log_ext("fail to read[%d]", len_file_temp);
        goto exit;
    }

    if(alipay_close_rsvd_part(fd) != 0)
    {
        alipay_log_ext("fail to close");
        goto exit;
    }

    if(alipay_clear_rsvd_part() < 0)
    {
        alipay_log_ext("alipay_clear_rsvd_part error");
        goto exit;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 0)
    {
        alipay_log_ext("fail to clear file");
        goto exit;
    }

    alipay_log_ext("alipay file test ...");
    fd = NULL;
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0)
    {
        alipay_log_ext("fail to open file");
        goto exit;
    }
    if(alipay_write_rsvd_part(fd, TEST_FILE_DATA, strlen(TEST_FILE_DATA)+1) != 0)
    {
        alipay_log_ext("fail to write");
        goto exit;
    }
    if(alipay_close_rsvd_part(fd) != 0)
    {
        alipay_log_ext("fail to close");
        goto exit;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 1)
    {
        alipay_log_ext("alipay_access_rsvd_part error");
        goto exit;
    }

    if(alipay_remove_rsvd_part(TEST_FILE_NAME) < 0)
    {
        alipay_log_ext("fail to remove file");
        goto exit;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 0)
    {
        alipay_log_ext("fail to remove file");
        goto exit;
    }
    alipay_log_ext("alipay file test end");
    
    alipay_srand(0x5A);
    int num = alipay_rand();
    alipay_log_ext("alipay_rand num is %d", num);
    if(num == 0)
    {
        alipay_log_ext("alipay_rand num is invalid");
        goto exit;
    }
exit:
    return;
}

uint8_t csi_test_data[] = {
	0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x74, 0x68,
    0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x63, 0x73,
	0x69, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x73,
	0x74, 0x72, 0x69, 0x6E, 0x67, 0x0A, 0x20, 0x73};
char csi_name1[] = "asset_test_1";
uint32_t csi_temp;
uint8_t csi_readbuf[32];

void ql_alipay_csi_test()
{
#ifdef QL_APP_FEATURE_ALIPAY_GCC_SE_V1
    int ret = 0;
    unsigned int fd;
	csi_temp = sizeof(csi_readbuf);
#endif
    alipay_iot_timeval now_t1, now_t2;
    alipay_iot_gettimeofday(&now_t1);
    Delay_Ms(10);
    alipay_iot_gettimeofday(&now_t2);
    alipay_log_ext("now_t1 %ld %ld", now_t1.tv_sec, now_t1.tv_usec);
    alipay_log_ext("now_t2 %ld %ld", now_t2.tv_sec, now_t2.tv_usec);

    alipay_log_ext("=====================csi_asset_test_demo start!=====================");

    HS_IIC_Init();

#ifdef QL_APP_FEATURE_ALIPAY_GCC_SE_V1
    ret = csi_open_asset((void **)(&fd), csi_name1);
    if (ret) 
    {
        alipay_log_ext("%s is open failed!", csi_name1);
		goto ERROR;
    } 
    else 
    {
        alipay_log_ext("%s is open success!", csi_name1);
    }
    ret = csi_read_asset((void *)fd, 0, csi_readbuf, &csi_temp);
    if (ret) 
    {
		alipay_log_ext("%s is read failed! ", csi_name1);
        //goto ERROR;
    } 
    else 
    {
        alipay_log_ext("%s is read success! ", csi_name1);
    }

	ret = csi_close_asset((void *)fd);
    if (ret) 
    {
        alipay_log_ext("%s is close failed! ", csi_name1);
        goto ERROR;
    } 
    else 
    {
        alipay_log_ext("%s is close success! ", csi_name1);
    }

	alipay_log_ext("csi_asset_test_demo Fuzzing testing pass ");
	alipay_log_ext("=====================csi_asset_test_demo end!=====================");
#endif
    
    csi_enter_lpm(NULL);
    alipay_task_sleep(5000);
    csi_exit_lpm(NULL);
    alipay_log_ext("csi_lpm_test end");
    return ;
#ifdef QL_APP_FEATURE_ALIPAY_GCC_SE_V1
ERROR:
#endif
	alipay_log_ext("csi_asset_test_demo Fuzzing testing not pass ");
	alipay_log_ext("=====================csi_asset_test_demo end!=====================");
}

//测试编译链接
void ql_alipay_aid_code_test()
{
    char aid_code[256];
    uint32_t len_aid_code = 0;
    bool isSupportBle = false;

    alipay_get_aid_code(aid_code, &len_aid_code, isSupportBle);
    int len = strlen(aid_code);
    alipay_log_ext("len2=%d", len);
}

#define HTTP_GET "GET /index.html HTTP/1.1\nHost: www.baidu.com:80\nContent-Type: text/html\nContent-Length: 0\n\r\n"
bool alipay_socket_test()
{
    int ret = 0;
    char *host_name = "www.baidu.com";
    unsigned char ip[4];
    uint32_t port = 80;
    int fd = 0;
    struct alipay_iot_sockaddr sa;
    char send_data[256] = {
        0,
    };

    alipay_log_ext("=== alipay_socket_test:%d", ret);
    
    //通过dns服务获取www.baidu.com的ip地址
    if (0 != alipay_iot_dns(host_name, ip))
    {
        alipay_log_ext("alipay_iot_dns request www.baidu.com ip fail");
        return false;
    }
    //创建套接�?
    fd = alipay_iot_socket_create(ALIPAY_IOT_SOC_PF_INET, ALIPAY_IOT_SOC_SOCK_STREAM, 0);
    if (fd < 0)
    {
        alipay_log_ext("alipay_iot_socket_create fail");
        return false;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_family = ALIPAY_IOT_SOC_PF_INET;
    sa.data.sin_data.port = port;

    memcpy(sa.data.sin_data.ip, ip, sizeof(ip));
    //与目标建立连�?
    if (alipay_iot_socket_connect(fd, &sa, sizeof(sa) < 0))
    {
        alipay_log_ext("alipay_iot_socket_connect(net->fd, &sa, sizeof(sa) WRONG: alipay_iot_socket_connect fail");
        alipay_iot_socket_close(fd);
        return false;
    }
    strcpy(send_data, HTTP_GET);
    // CHECK_TRUE_MSG(alipay_iot_socket_write(fd,send_data,alipay_iot_strlen(send_data))>=0,"alipay_iot_socket_write fail", return false);
    //发http的get�?
    if (alipay_iot_socket_write(fd, send_data, strlen(send_data)) < 0)
    {
        alipay_log_ext("alipay_iot_socket_write(fd,send_data,alipay_iot_strlen(send_data)) WRONG: alipay_iot_socket_write fail");
        alipay_iot_socket_close(fd);
        return false;
    }

    //接收回的�?
    alipay_iot_timeval tv = {
        3,
        1
    };
    int selectRet = 0;
    alipay_iot_fd_set t_set1;
    alipay_iot_fd_zero(&t_set1);
    alipay_iot_fd_setbit(fd, &t_set1);
    selectRet = alipay_iot_select(fd + 1, &t_set1, NULL, NULL, &tv);
    if (selectRet < 0)
    {
        alipay_iot_socket_close(fd);
        alipay_log_ext("alipay_iot_select(sockfd + 1, &t_set1, NULL, NULL, &tv) WRONG: alipay_iot_select fail");
        return false;
    }
    if (selectRet > 0)
    {
        char buf[4096] = {0};
        int readLen = 0;
        memset(buf, 0, 4096);
        readLen = alipay_iot_socket_read(fd, buf, 4095);
        if (readLen <= 0)
        {
            alipay_iot_socket_close(fd);
            alipay_log_ext("alipay_iot_socket_read(fd, buf, 4095); WRONG: alipay_iot_socket_read fail\n");
            return false;      
        }
        alipay_log_ext("buf:%s", buf);
    }
    alipay_iot_socket_close(fd);
    return true;
}

void ql_alipay_api_task_handle(void *ctx)
{
    //ql_alipay_csi_test();
    ql_alipay_aid_code_test();  
    ql_alipay_api_test();
    alipay_socket_test();
    
    while(1)
    {
        ql_rtos_task_sleep_s(5);
    }
	ql_rtos_task_delete(NULL);
}


int ql_alipay_api_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;
	
	err = ql_rtos_task_create(&ql_alipay_api_task, ALIPAY_API_DEMO_TASK_STACK_SIZE, ALIPAY_API_DEMO_TASK_PRIO, "ql_alipay_api", ql_alipay_api_task_handle, NULL, ALIPAY_API_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_ALIPAY_API_LOG("ql_alipay_api_task created failed");
	}

	return err;
}


