
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "psm_demo.h"

#include "ql_api_datacall.h"
#include "ql_power.h"
#include "ql_api_rtc.h"
#include "ql_api_dev.h"
#include "ql_api_nw.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_PSMDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_PSMDEMO_LOG(msg, ...)         QL_LOG(QL_PSMDEMO_LOG_LEVEL, "ql_PSM", msg, ##__VA_ARGS__)
#define QL_PSMDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_PSM", msg, ##__VA_ARGS__)

#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a)		(leapyear(a) ? 366 : 365)
#define	days_in_month(a)	(month_days[(a) - 1])

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L
#define SECYR			(SECDAY * 365)

int wake_lock = 0;

#if !defined(require_action)
	#define require_action(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_PSMDEMO_LOG(str);																		\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

#define QL_PSMDEMO_SIM_ID               0
#define QL_PSMDEMO_WAKE_UP_TIME         60          //unit:s
/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t psm_task = NULL;
ql_sem_t psm_rtc_update_sem =NULL;
ql_sem_t psm_data_reg_sem = NULL;

static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*===========================================================================
 * Functions
 ===========================================================================*/
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


void ql_psm_nw_notify_cb(uint8_t sim_id, unsigned int ind_type, void *ind_msg_buf)
{    
    switch(ind_type)
    {
        case QUEC_NW_NITZ_TIME_UPDATE_IND:
        {
            QL_PSMDEMO_LOG("time update!");
            ql_rtos_semaphore_release(psm_rtc_update_sem);
            break;
        }
        case QUEC_NW_DATA_REG_STATUS_IND:
        {
            ql_nw_common_reg_status_info_s  *data_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
            QL_PSMDEMO_LOG("Sim%d data reg status changed, current status is %d", sim_id, data_reg_status->state);
            if((QL_NW_REG_STATE_HOME_NETWORK == data_reg_status->state) || (QL_NW_REG_STATE_ROAMING == data_reg_status->state))
            {
                ql_rtos_semaphore_release(psm_data_reg_sem);
            }
            break;
        }
    }
}

void ql_psm_cb(void *ctx)
{
    return;
}

static bool ql_psm_set_cfun(void)
{
    uint8_t cfun = 0;
    int ret = 0;
    if(ql_dev_get_modem_fun(&cfun, QL_PSMDEMO_SIM_ID) != QL_DEV_SUCCESS)
    {
        return false;
    }
    QL_PSMDEMO_LOG("cfun: %d", cfun);
    if(QL_DEV_CFUN_MIN == cfun)
    {
        return true;
    }
    ret = ql_dev_set_modem_fun(QL_DEV_CFUN_AIR, 0, QL_PSMDEMO_SIM_ID);
    if(ret != QL_DEV_SUCCESS)
    {
        QL_PSMDEMO_LOG("set QL_DEV_CFUN_AIR, ret: 0x%x", ret);
        return false; 
    }

    ret = ql_dev_set_modem_fun(QL_DEV_CFUN_FULL, 0, QL_PSMDEMO_SIM_ID);
    if(ret != QL_DEV_SUCCESS)
    {
        QL_PSMDEMO_LOG("set QL_DEV_CFUN_FULL, ret: 0x%x", ret);
        return false; 
    }
    return true;
}

static bool ql_psm_set_alarm(void)
{
    int ret = 0;
    int64_t time_sec=0;
    ql_rtc_time_t tm={0};
    
    ret = ql_rtc_get_time(&tm);
    if(ret != QL_RTC_SUCCESS)
    {
        QL_PSMDEMO_LOG("get time err");
        return false; 
    }
    ql_rtc_print_time(tm);
    
    ql_rtc_conv_sec_time(&time_sec,&tm);
    time_sec += QL_PSMDEMO_WAKE_UP_TIME;
    ql_sec_conv_rtc_time(&time_sec,&tm);

    ret = ql_rtc_set_alarm(&tm);
    if(ret != QL_RTC_SUCCESS)
    {
        QL_PSMDEMO_LOG("set alarm err");
        return false; 
    }
    ql_rtc_print_time(tm);
    //Enable RTC alarm
    ret = ql_rtc_enable_alarm(1);
    if(ret != QL_RTC_SUCCESS)
    {
        QL_PSMDEMO_LOG("enable alarm err");
        return false; 
    }
    return true;
}

static void ql_psm_demo_thread(void *param)
{
    uint8_t source = 0;
    int ret = 0,retry_count = 0;
    int profile_idx = 1;   //range 1 to 7
    QlOSStatus err = QL_OSI_SUCCESS;

    ql_nw_reg_status_info_s* nw_info =  (ql_nw_reg_status_info_s*)calloc(1,sizeof(ql_nw_reg_status_info_s));
    ql_data_call_info_s* info =         (ql_data_call_info_s*)calloc(1,sizeof(ql_data_call_info_s));

    if(nw_info == NULL|| info ==NULL)
    {
        QL_PSMDEMO_LOG("calloc fail!");
        goto PSM_EXIT;
    }

    if(QL_SLEEP_SUCCESS != ql_psm_register_enter_cb(ql_psm_cb,NULL))
    {
        QL_PSMDEMO_LOG("register psm callback fail!");
        goto PSM_EXIT;
    }
    if(QL_NW_SUCCESS != ql_nw_register_cb(ql_psm_nw_notify_cb))
    {
        QL_PSMDEMO_LOG("register network callback fail");
        goto PSM_EXIT;
    }
    if(0 == ql_get_powerup_reason(&source) && QL_PWRUP_PSM_WAKEUP == source)
    {
        QL_PSMDEMO_LOG("PSM WAKEUP!");

        err = ql_lpm_wakelock_lock(wake_lock);
        require_action((err != 0), return, "Error in wakelock_lock");
        
        ret = ql_get_data_call_info(QL_PSMDEMO_SIM_ID, profile_idx, info);
        if(QL_DATACALL_SUCCESS == ret && info->v4.state)
        {
            QL_PSMDEMO_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info->v4.addr.ip)));
            
            //list the task that want to finish it before enter the PSM.
            //dspw_mqtt_boot_msg("timestamp"); 
               
            ql_lpm_wakelock_unlock(wake_lock);
            ql_lpm_wakelock_delete(wake_lock);
        }
        else{
            QL_PSMDEMO_LOG("ql_get_data_call_info fail!");
            ql_lpm_wakelock_unlock(wake_lock);
            ql_lpm_wakelock_delete(wake_lock);
            goto PSM_EXIT;
        }
        //Reset alarm for PSM wake up
        if(!ql_psm_set_alarm())
        {
            QL_PSMDEMO_LOG("alarm set fail!");
            goto PSM_EXIT;
        }
    }
    else{
        //Enable PSM when first power on
        char *periodic_TAU_time="00100001";     //T3412
        char *active_time="00000001";           //T3324
        if(QL_SLEEP_SUCCESS == ql_psm_sleep_enable(QL_PSMDEMO_SIM_ID, true, periodic_TAU_time, active_time))
        {
            ret = ql_rtos_semaphore_create(&psm_rtc_update_sem, 0);
            if(ret != QL_OSI_SUCCESS)
            {
                QL_PSMDEMO_LOG("psm_rtc_update_sem created failed, ret = 0x%x", ret);
                goto PSM_EXIT;

            }
            ret = ql_rtos_semaphore_create(&psm_data_reg_sem, 0);
            if(ret != QL_OSI_SUCCESS)
            {
                QL_PSMDEMO_LOG("ql_data_reg_sem created failed, ret = 0x%x", ret);
                goto PSM_EXIT;
            }

            //Call a necessary delay to avoid failure of getting cfun status when power on.
            ql_rtos_task_sleep_ms(2000);

            if(!ql_psm_set_cfun())
            {
                QL_PSMDEMO_LOG("cfun set fail!");
                goto PSM_EXIT;
            }
            if(ql_rtos_semaphore_wait(psm_rtc_update_sem, 10000))
            {
                QL_PSMDEMO_LOG("psm_rtc_update_sem time out");
            }
            //PDP active
            ret = ql_nw_get_reg_status(QL_PSMDEMO_SIM_ID, nw_info);
            if((QL_NW_REG_STATE_HOME_NETWORK != nw_info->data_reg.state) && (QL_NW_REG_STATE_ROAMING != nw_info->data_reg.state))
            {
                if(ql_rtos_semaphore_wait(psm_data_reg_sem, 10000))
                {
                    QL_PSMDEMO_LOG("ql_data_reg_sem time out");
                    goto PSM_EXIT;
                }
            }
            ret = ql_set_data_call_asyn_mode(QL_PSMDEMO_SIM_ID, profile_idx, 0);   //Set to sync mode
            //Try reconnect 10 times, time interval is 20 seconds
            while(((ret = ql_start_data_call(QL_PSMDEMO_SIM_ID, profile_idx, QL_PDP_TYPE_IP, NULL, NULL, NULL, 0)) != QL_DATACALL_SUCCESS) && (retry_count < 10))
            {
                retry_count++;
                QL_PSMDEMO_LOG("gprs reconnect fail, the retry count is %d", retry_count);
                ql_rtos_task_sleep_ms(20000);
            }
            if(QL_DATACALL_SUCCESS != ret)
            {
                goto PSM_EXIT;
            }
            ret = ql_get_data_call_info(QL_PSMDEMO_SIM_ID, profile_idx, info);
            if(QL_DATACALL_SUCCESS == ret && info->v4.state)
            {
                QL_PSMDEMO_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info->v4.addr.ip)));
            }
            else{
                QL_PSMDEMO_LOG("ql_get_data_call_info fail!");
                goto PSM_EXIT;
            }
            
            //set alarm
            if(!ql_psm_set_alarm())
            {
                QL_PSMDEMO_LOG("alarm set fail!");
                goto PSM_EXIT;
            }
            QL_PSMDEMO_LOG("psm enable!");
            ql_autosleep_enable(true);
        }
        else
        {
            QL_PSMDEMO_LOG("psm enable fail!");
            goto PSM_EXIT;
        }
    }
PSM_EXIT:
    if(nw_info)
    {
        free(nw_info);
        nw_info = NULL;
    }
    if(info)
    {
        free(info);
        info = NULL;
    }
    ql_rtos_semaphore_delete(psm_rtc_update_sem);
    ql_rtos_semaphore_delete(psm_data_reg_sem);
    psm_rtc_update_sem = NULL;
    psm_data_reg_sem = NULL;
    ql_nw_register_cb(NULL);
    ql_rtos_task_delete(NULL);
}

void ql_psm_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    wake_lock = ql_lpm_wakelock_create("my_lock", 10);
    err = ql_rtos_task_create(&psm_task, 4096, APP_PRIORITY_NORMAL, "ql_psmdemo", ql_psm_demo_thread, NULL, 3);
    if(err != QL_OSI_SUCCESS)
    {
        QL_PSMDEMO_LOG("psm demo task created failed", err);
        return;
    }
    return;
}


