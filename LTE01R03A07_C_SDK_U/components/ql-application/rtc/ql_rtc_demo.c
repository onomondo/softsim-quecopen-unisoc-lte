/**  
  @file
  ql_rtc_demo.c

  @brief
  quectel rtc demo.

*/
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
16/12/2020        Neo         Init version
=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_rtc.h"
#include "ql_rtc_demo.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_RTCDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_RTCDEMO_LOG(msg, ...)         QL_LOG(QL_RTCDEMO_LOG_LEVEL, "ql_RTCDEMO", msg, ##__VA_ARGS__)
#define QL_RTCDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_RTCDEMO", msg, ##__VA_ARGS__)

#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a)		(leapyear(a) ? 366 : 365)
#define	days_in_month(a)	(month_days[(a) - 1])

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L
#define SECYR			(SECDAY * 365)

#if !defined(rtc_demo_err_exit)
	#define rtc_demo_err_exit(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x)                                                                        				\
				{                                                                                               \
					QL_RTCDEMO_LOG(str);																			\
					{goto action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

/*========================================================================
 *  Global Variable
 *========================================================================*/   
static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*========================================================================
 *  function Definition
 *========================================================================*/
static int ql_rtc_fix_weekday(ql_rtc_time_t *rtc_time)
{
    int leapsToDate;
    int lastYear;
    int day;
    int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

    if (rtc_time->tm_year < 1753)
        return -1;
    lastYear=rtc_time->tm_year-1;

    //Number of leap corrections to apply up to end of last year
    leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;

    //This year is a leap year if it is divisible by 4 except when it is divisible by 100 unless it is divisible by 400
    //e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 will be
    if((rtc_time->tm_year%4==0) && ((rtc_time->tm_year%100!=0) || (rtc_time->tm_year%400==0)) && (rtc_time->tm_mon>2)) 
    {       
        //We are past Feb. 29 in a leap year
        day=1;
    } 
    else 
    {
        day=0;
    }

    day += lastYear*365 + leapsToDate + MonthOffset[rtc_time->tm_mon-1] + rtc_time->tm_mday;

    rtc_time->tm_wday=day%7;

    return 0;
}

static int ql_sec_conv_rtc_time(int64_t* time_t, ql_rtc_time_t *rtc_time)
{
    int    i;
    long   hms, day;

    day = *time_t / SECDAY;
    hms = *time_t % SECDAY;

    //Hours, minutes, seconds are easy
    rtc_time->tm_hour = hms / 3600;
    rtc_time->tm_min = (hms % 3600) / 60;
    rtc_time->tm_sec = (hms % 3600) % 60;

    //Number of years in days
    for (i = STARTOFTIME; day >= days_in_year(i); i++) 
    {
        day -= days_in_year(i);
    }
    rtc_time->tm_year = i;

    //Number of months in days left
    if (leapyear(rtc_time->tm_year)) 
    {
        days_in_month(FEBRUARY) = 29;
    }
    for (i = 1; day >= days_in_month(i); i++) 
    {
        day -= days_in_month(i);
    }
    days_in_month(FEBRUARY) = 28;
    rtc_time->tm_mon = i;

    //Days are what is left over (+1) from all that.
    rtc_time->tm_mday = day + 1;

    //Determine the day of week
    return ql_rtc_fix_weekday(rtc_time);
}

static int ql_rtc_conv_sec_time(int64_t* time_t, ql_rtc_time_t* rtc_time)
{
    int mon = rtc_time->tm_mon;
    int year = rtc_time->tm_year;
    int64_t days, hours;

    mon -= 2;
    if (0 >= (int)mon) 
    {    
        // 1..12 -> 11,12,1..10
        mon += 12;    
        
        //Puts Feb last since it has leap day
        year -= 1;
    }
    days = (unsigned long)(year / 4 - year / 100 + year / 400 +367 * mon / 12 + rtc_time->tm_mday) + year * 365 - 719499;

    hours = (days * 24) + rtc_time->tm_hour;

    *time_t = (hours * 60 + rtc_time->tm_min) * 60 + rtc_time->tm_sec;

    return 0;
}

void ql_rtc_test_callback(void)
{
    QL_RTCDEMO_LOG("ql rtc test callback come in!");
    
    ql_rtc_time_t test_tm = {0}; 
    
    //disable RTC alarm
    //ql_rtc_enable_alarm(0);
    
    //get alarm time
    QL_RTCDEMO_LOG("=========callback print alarm time===========\r\n");
    ql_rtc_get_alarm(&test_tm);
    ql_rtc_print_time(test_tm);
}

static void ql_rtc_demo_thread()
{
    ql_rtc_time_t tm;
    int64_t time_t;
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_errcode_rtc_e ret;
    int timezone_value = 0;

    //get current time
    ret = ql_rtc_get_time(&tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "get time err");
    
    ret =ql_rtc_print_time(tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "print time err");

    ql_rtc_conv_sec_time(&time_t, &tm);
    QL_RTCDEMO_LOG("getting time convert %ld sec.\r\n",time_t);

    ql_sec_conv_rtc_time(&time_t, &tm);
    ret =ql_rtc_print_time(tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "print time err");
    
    //2021-6-15 16:50:30 [TUS]
    tm.tm_year = 2021;
    tm.tm_mon  = 6;
    tm.tm_mday = 22;
    tm.tm_wday = 4;
    tm.tm_hour = 16;
    tm.tm_min  = 50;
    tm.tm_sec  = 30;
    ret = ql_rtc_print_time(tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "print time err");

    //set time
    ret = ql_rtc_set_time(&tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "set time err");

    ret = ql_rtc_get_time(&tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "get time err");
    
    ret = ql_rtc_print_time(tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "print time err");

    //set & enable alarm
 	tm.tm_sec += 20;
	ret = ql_rtc_set_alarm(&tm);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "set alarm err");
    
    ret = ql_rtc_register_cb(ql_rtc_test_callback);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "reg cb err");
    
	ret = ql_rtc_enable_alarm(1);
    rtc_demo_err_exit(ret != QL_RTC_SUCCESS, QL_RTC_DEMO_EXIT, "enable alarm err");
    
	while (1) 
    { 
		ql_rtc_get_time(&tm);
		QL_RTCDEMO_LOG("=========print current time===========\r\n");
		ql_rtc_print_time(tm);
        
        ql_rtc_set_timezone(32);    //UTC+32
        ql_rtc_get_localtime(&tm);
        QL_RTCDEMO_LOG("=========print local time===========\r\n");
		ql_rtc_print_time(tm);
        
        ql_rtc_get_timezone(&timezone_value);
        QL_RTCDEMO_LOG("timezone_value: %+03d\r\n", timezone_value);
		ql_rtos_task_sleep_s(5);
	}
QL_RTC_DEMO_EXIT:
    err = ql_rtos_task_delete(NULL);
    if(err != QL_OSI_SUCCESS)
    {
        QL_RTCDEMO_LOG("task deleted failed");
    }

}

void ql_rtc_app_init()
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_task_t rtc_task = NULL;

    err = ql_rtos_task_create(&rtc_task, 1024, APP_PRIORITY_NORMAL, "ql_rtcdemo", ql_rtc_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_RTCDEMO_LOG("rtc demo task created failed");
    }
}



