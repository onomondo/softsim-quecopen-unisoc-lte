/**  
  @file
  charger_demo.c

  @brief
  quectel charger_demo.

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
15/01/2021        Neo         Init version
=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_power.h"
#include "ql_adc.h"
#include "charge_demo.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_CHARGERDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_CHARGERDEMO_LOG(msg, ...)         QL_LOG(QL_CHARGERDEMO_LOG_LEVEL, "ql_CHARGER", msg, ##__VA_ARGS__)
#define QL_CHARGERDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("qlCHARGER", msg, ##__VA_ARGS__)

/*
** CONFIG_CHARGER_POWERUP宏支持关机充电和充电自动开机功能，可以添加到对应项目的target.config开启这项功能
** CONFIG_SRV_POWER_ON_VOLTAGE 为开机电压，默认为3300mV
** CONFIG_SRV_POWER_OFF_VOLTAGE为关机电压，默认为3000mV
*/

#define QL_ENABLE_BATTERY_DETECT    0     //只有外接ADC检测电池，才可以打开这个功能

#define BATTERY_CONNECT_THRESHOLD   1720  //电池在位阈值，需要根据根据NTC电阻温度对照表确定在位阈值电压！！！！！！
#define BATTERY_NO_CONNECT		    0     //电池未连接
#define BATTERY_CONNECT		        1     //电池已连接


/*===========================================================================
 * Functions
 ===========================================================================*/
#if QL_ENABLE_BATTERY_DETECT
/*	
** Description: 获取电池在位状态。根据读取的NTC电压判断是否处于在位状态，需要根据根据NTC电阻温度对照表确定在位阈值电压！！！！！！
*/
void ql_get_battery_state(int ql_adc_channel_id, uint8_t* bat_state)
{
	int ntc_vol = 0;
	ql_adc_get_volt(ql_adc_channel_id, &ntc_vol);
	QL_CHARGERDEMO_LOG("ntc_vol=%d", ntc_vol);

	if(ntc_vol < BATTERY_CONNECT_THRESHOLD)
	{
		*bat_state = BATTERY_CONNECT;
	}
	else
	{
		*bat_state = BATTERY_NO_CONNECT;
	}
}

/*
** Description: 获取电池温度,根据电池电压与温度对应关系计算,映射关系需要参考NTC电阻温度对照表！！！！！！
*/
void ql_get_battery_temperature(int ql_adc_channel_id, int *bat_temperature)
{
	int ntc_vol = 0, i = 0;
	uint8_t bat_state = 0;
	uint32_t table_size = 0;

	ql_get_battery_state(ql_adc_channel_id, &bat_state);
	if(BATTERY_NO_CONNECT == bat_state)
	{
		return;
	}
	table_size = sizeof(ql_vbat_temp_table) / sizeof(ql_vbat_temp_table[0]);
	ql_adc_get_volt(ql_adc_channel_id, &ntc_vol);

    for (i = 0; i < table_size; i++)
    {
        if (ntc_vol >= ql_vbat_temp_table[i][0])
        {
            break;
        }
    }

    if (i < table_size)
    {
        *bat_temperature = ql_vbat_temp_table[i][1];
    }
    else
    {
        *bat_temperature = ql_vbat_temp_table[table_size - 1][1];
    }

    QL_CHARGERDEMO_LOG("bat_temperature=%d, ntc_vol=%d", *bat_temperature, ntc_vol);
}
#endif

static void ql_charge_demo_thread(void *param)
{
    QL_CHARGERDEMO_LOG("charger demo thread enter, param 0x%x", param);

    uint32_t chg_vol = 0;
    uint32_t vbus_state = 0;
	uint32_t battery_vol = 0;
	uint32_t battery_level = 0;
	uint32_t charger_cur = 0;   
    ql_charge_status_e charge_status;
	ql_charge_config_t charge_config = 
		{
			4150,  //rechg_vol
			4210,  //chg_end_vol
			QL_CHARGER_CURRENT_700MA,  //standard_chg_current
			18000,  //chg_timeout, unit:s
			9000,  //chg_ontime_ms
			1000,  //chg_offtime_ms
		}; //the above is the original default value of unisoc
	ql_charge_config_t charge_config_obtain = {0};

#if QL_ENABLE_BATTERY_DETECT
    int bat_temperature = 0;
    uint8_t bat_state = 0;
#endif

	ql_charge_set_config(&charge_config);
	ql_charge_get_config(&charge_config_obtain);
	QL_CHARGERDEMO_LOG("rechg_vol=%d, chg_end_vol=%d, standard_chg_current=%d, chg_timeout=%d\r\n",
		               charge_config_obtain.rechg_vol, charge_config_obtain.chg_end_vol,  
	                   charge_config_obtain.standard_chg_current, charge_config_obtain.chg_timeout);
	QL_CHARGERDEMO_LOG("chg_ontime_ms=%d,chg_offtime_ms=%d\r\n", charge_config_obtain.chg_ontime_ms,charge_config_obtain.chg_offtime_ms);

	while(1)
	{
        ql_get_vchg_vol(&chg_vol);
        ql_get_vbus_state(&vbus_state);
        ql_get_battery_vol(&battery_vol);
        ql_get_battery_level(&battery_level);
        ql_get_charge_cur(&charger_cur);
        ql_get_charge_status(&charge_status);
		
		QL_CHARGERDEMO_LOG("battery_vol:%d, battery_level:%d\r\n", battery_vol, battery_level);
		QL_CHARGERDEMO_LOG("charger_status:%d, charger_cur:%d\r\n",charge_status,charger_cur);
		QL_CHARGERDEMO_LOG("chg_vol:%d, vbus_state:%d\r\n",chg_vol, vbus_state);

#if QL_ENABLE_BATTERY_DETECT
		bat_state = BATTERY_NO_CONNECT;
		bat_temperature = 0;
        ql_get_battery_state(QL_ADC0_CHANNEL, &bat_state);             //此处使用ADC0检测，客户可以替换为其它ADC通道
		ql_get_battery_temperature(QL_ADC0_CHANNEL, &bat_temperature); //此处使用ADC0检测，客户可以替换为其它ADC通道
		QL_CHARGERDEMO_LOG("battery_state:%d, battery_temperature:%d\r\n",bat_state,bat_temperature);
		if(bat_temperature > 45 || bat_temperature < -5)
		{
			if(QL_CHG_CHARGING == charge_status)
			{
				ql_charge_set_over_temp(true);  //温度过高/过低保护，停止充电
			}
		}
		else
		{
			if(QL_CHG_IDLE == charge_status)
			{
				ql_charge_set_over_temp(false); //取消过高/过低保护，恢复充电
			}
		}
#endif

		if(charge_status==QL_CHG_IDLE)
		{
            QL_CHARGERDEMO_LOG("battery idle");
		}	
		else
		{
            QL_CHARGERDEMO_LOG("battery charging");
		}

		ql_rtos_task_sleep_s(5);
	}

    ql_rtos_task_delete(NULL);
}

void ql_charge_app_init(void)
{
    QlOSStatus err = QL_SUCCESS;
    ql_task_t charge_task = NULL;
    err = ql_rtos_task_create(&charge_task, 1024, APP_PRIORITY_NORMAL, "ql_chargedemo", ql_charge_demo_thread, NULL, 1);
	if( err != QL_SUCCESS )
    {
        QL_CHARGERDEMO_LOG("charger demo task created failed");
    }
}


