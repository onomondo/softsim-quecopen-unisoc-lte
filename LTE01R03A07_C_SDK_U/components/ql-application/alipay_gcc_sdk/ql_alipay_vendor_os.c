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

#include "ql_api_osi.h"
#include "ql_api_dev.h"
#include "ql_api_rtc.h"
#include "ql_log.h"
#include "vendor_os.h"
#include "alipay_net_kal.h"

int32_t get_Rng_number(uint8_t *data, unsigned size) 
{
    for (int i = 0; i < size; i++) 
    {
        data[i] = (uint8_t)(ql_dev_get_sys_srand()%65536);
    }
    alipay_log_ext("random bytes[%d]: data[0]:0x%02x, data[1]:0x%02x - data[n-2]:0x%02x, data[n-1]0x%02x", size, data[0], data[1], data[size-2], data[size-1]);
    return 1;
}

extern int64_t osiEpochSecond(void);
uint32_t alipay_get_timestamp(void) 
{
    uint32_t ts_now = 0;
    ts_now = osiEpochSecond();
    return ts_now;
}

extern bool osiSetEpochTime(int64_t ms);
void alipay_set_system_time(PARAM_IN int32_t timestamp_s)
{
    int64_t ms = ((int64_t)timestamp_s)*1000;
    osiSetEpochTime(ms);
}

char ql_alipay_get_mount(char *date_str)
{
    if (strstr(date_str, "Jan"))
    {
        return 1;
    }

    if (strstr(date_str, "Feb"))
    {
        return 2;
    }

    if (strstr(date_str, "Mar"))
    {
        return 3;
    }

    if (strstr(date_str, "Apr"))
    {
        return 4;
    }

    if (strstr(date_str, "May"))
    {
        return 5;
    }

    if (strstr(date_str, "Jun"))
    {
        return 6;
    }

    if (strstr(date_str, "Jul"))
    {
        return 7;
    }

    if (strstr(date_str, "Aug"))
    {
        return 8;
    }

    if (strstr(date_str, "Sep"))
    {
        return 9;
    }

    if (strstr(date_str, "Oct"))
    {
        return 10;
    }

    if (strstr(date_str, "Nov"))
    {
        return 11;
    }

    if (strstr(date_str, "Dec"))
    {
        return 12;
    }

    return 1;
}

/*获取编译时候的时间戳，单位为秒
 * @return 编译时候的时间戳，单位为秒
 */
uint32_t alipay_get_compile_timestamp(void)
{
    int year, month, day;
    int hour, min, sec;
    int64_t days, hours, time_t;
    // 获取当前日期字符串
    //如：Mon Jul 08 15:53:12 2024
    char temp_data[64] = {0};
    char *temp = temp_data;
    char date_str[] =  __TIMESTAMP__;

    memcpy(temp_data, date_str, strlen(date_str));

    alipay_log_ext("date=%s", date_str);

    temp = date_str + 20;
    sscanf(temp, "%d", &year);
    
    month = ql_alipay_get_mount(date_str+4);
    
    temp = date_str + 8;
    temp[2] = '\0';
    sscanf(temp, "%d", &day);

    temp = date_str + 11;
    temp[2] = '\0';
    sscanf(temp, "%d", &hour);

    temp = date_str + 14;
    temp[2] = '\0';
    sscanf(temp, "%d", &min);

    temp = date_str + 17;
    temp[2] = '\0';
    sscanf(temp, "%d", &sec);

    month -= 2;
    if (0 >= (int)month) 
    {    
        // 1..12 -> 11,12,1..10
        month += 12;    
        
        //Puts Feb last since it has leap day
        year -= 1;
    }
    days = (unsigned long)(year / 4 - year / 100 + year / 400 +367 * month / 12 + day) + year * 365 - 719499;

    hours = (days * 24) + hour;

    time_t = (hours * 60 + min) * 60 + sec;
 
    return (uint32_t)time_t;
}

void alipay_iot_get_local_time(alipay_iot_local_time *time)
{
    ql_rtc_time_t tm = {0};
    ql_rtc_get_localtime(&tm);
    time->year = tm.tm_year;
    time->month = tm.tm_mon;
    time->wday = tm.tm_wday;
    time->day = tm.tm_mday;
    time->hour = tm.tm_hour;
    time->minute = tm.tm_min;
    time->second = tm.tm_sec;
}


void* alipay_malloc(uint32_t size) 
{
    if (size == 0)
    {
        return NULL;
    }
    
    void *ptr = malloc(size);
    if (!ptr) 
    {
        alipay_log_ext("malloc failed");
        return NULL;
    }
    memset(ptr, 0, size);
    return ptr;
}

void* alipay_calloc(uint32_t nblock,uint32_t size) 
{
    void *ptr = calloc(nblock, size);
    if (!ptr) 
    {
        alipay_log_ext("calloc failed");
        return NULL;
    }
    return ptr;
}

void alipay_free(void* pt) 
{
    if (pt) 
    {
        free(pt);
    }
}

void* alipay_realloc(void* ptr, uint32_t size) 
{
    return realloc(ptr, size);
}


void alipay_log_ext(const char *format, ...) 
{
    static char ALIPAY_log_buf[1024];
    memset(ALIPAY_log_buf, 0, sizeof(ALIPAY_log_buf));
    va_list arg;
    va_start(arg, format);
    vsnprintf(ALIPAY_log_buf, sizeof(ALIPAY_log_buf)-1, format, arg);    
    printf("%s\n", ALIPAY_log_buf);
    va_end(arg);
}

/**
* 设定随机数种子
*/
void alipay_srand(unsigned int seed)
{
    srand(seed);
}

/**
* 获取一个非0的随机数
*/
int alipay_rand(void)
{
    int ql_rand = 0;
    
    while(0 == (ql_rand = rand()))
    {
        ;
    }

    return ql_rand;
}

/*
* @brief: 使任务休眠milliseconds毫秒
* @input: milliseconds 休眠的时间，单位毫秒
*/
void alipay_task_sleep(uint32_t ms)
{
    ql_rtos_task_sleep_ms(ms);
}

/*
* @brief: 获取UTC时间
* note:  1.精确级别,微妙级别
*        2.受系统时间修改影响
*        3.返回的秒数是从1970年1月1日0时0分0秒开始
*/
void alipay_iot_gettimeofday(alipay_iot_timeval *now)
{
    ql_timeval_t ql_timeval = {0};
    
    ql_gettimeofday(&ql_timeval);
    now->tv_sec = ql_timeval.sec;
    now->tv_usec = ql_timeval.usec;
}

alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name)
{
    ql_mutex_t mutex = NULL;
    if (QL_OSI_SUCCESS != ql_rtos_mutex_create(&mutex))
    {
        return NULL;
    }

    return (alipay_iot_mutex)mutex;
}

int alipay_iot_mutex_lock(alipay_iot_mutex mutex)
{
    if (mutex == NULL)
    {
        return -1;
    }
    return (int)ql_rtos_mutex_try_lock(mutex);
}


int alipay_iot_mutex_unlock(alipay_iot_mutex mutex)
{
    if (mutex == NULL)
    {
        return -1;
    }
    return (int)ql_rtos_mutex_unlock(mutex);
}

int alipay_iot_mutex_delete(alipay_iot_mutex mutex)
{
    if (mutex == NULL)
    {
        return -1;
    }
    return (int)ql_rtos_mutex_delete(mutex);
}



