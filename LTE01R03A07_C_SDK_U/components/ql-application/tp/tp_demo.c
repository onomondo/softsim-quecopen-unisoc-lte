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
#include "ql_api_tp.h"
#include "ql_lcd.h"
#include "ql_gpio.h"
#include "ql_adc.h"
#include "ql_pwm.h"

#include "ql_log.h"
#include "tp_demo.h"


#define QL_TP_DEMO_LOG_LEVEL       	            QL_LOG_LEVEL_INFO
#define QL_TP_DEMO_LOG(msg, ...)			    QL_LOG(QL_TP_DEMO_LOG_LEVEL, "ql_TP_DEMO", msg, ##__VA_ARGS__)
#define QL_TP_DEMO_LOG_PUSH(msg, ...)	        QL_LOG_PUSH("ql_TP_DEMO", msg, ##__VA_ARGS__)


ql_task_t tp_demo_task = NULL;

#define QL_TP_LCD_CALIB_WIDTH       20          //在LCD屏幕坐标显示校准+的宽度
#define QL_TP_LCD_CALIB_HEIGHT       4          //在LCD屏幕坐标显示校准+的高度
#define QL_TP_LCD_WIDTH             240         //LCD屏幕宽度,需要改成实际屏的宽度
#define QL_TP_LCD_HIGH              320         //LCD屏幕高度,需要改成实际屏的高度

//注意，3个校准点不能在一条直线上
#define QL_TP_CALIB_POINT_X1        (0)                                                                     //左上角
#define QL_TP_CALIB_POINT_Y1        (0)
#define QL_TP_CALIB_POINT_X2        (QL_TP_LCD_WIDTH-QL_TP_LCD_CALIB_WIDTH-QL_TP_LCD_CALIB_HEIGHT)          //右上角
#define QL_TP_CALIB_POINT_Y2        (0)
#define QL_TP_CALIB_POINT_X3        ((QL_TP_LCD_WIDTH-QL_TP_LCD_CALIB_WIDTH-QL_TP_LCD_CALIB_HEIGHT)/2)      //屏幕中间
#define QL_TP_CALIB_POINT_Y3        ((QL_TP_LCD_HIGH-QL_TP_LCD_CALIB_WIDTH-QL_TP_LCD_CALIB_HEIGHT)/2)

void ql_tp_demo_calib_cb(QL_TP_CALIB_STATUS_E calib_status)
{
    QL_TP_DEMO_LOG("calib_status=%d", calib_status);
}

static void ql_tp_demo_touch_cb(ql_tp_point_t point, QL_TP_PRESS_STATE_E state)
{
    uint16_t draw_data[64];
    int i;
    for (i=0; i<64; i++)
    {
        draw_data[i] = QL_TP_LCD_BLACK;
    }
    QL_TP_DEMO_LOG("LCD x=%d,y=%d,state=%d", point.x, point.y, state);
     
    if (point.x > QL_TP_LCD_WIDTH - 3)
    {
        point.x = QL_TP_LCD_WIDTH - 3;
    }
    
    if (point.y > QL_TP_LCD_HIGH - 3)
    {
        point.y = QL_TP_LCD_HIGH - 3;
    }
    ql_lcd_write(draw_data, point.x, point.y, point.x+2, point.y+2);
}


static void ql_tp_demo_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    ql_tp_ctg_t tp_cfg;
    ql_errcode_tp_e ret;
    ql_tp_calib_ctg_t calib_cfg = {0};
    ql_tp_touch_adc_ctg_t adc_cfg = {0};

    //控制背光，根据实际样机修改
    ql_pin_set_func(122, 2);
    ql_gpio_deinit(GPIO_29);
    ql_gpio_init(GPIO_29, GPIO_OUTPUT, PULL_NONE, LVL_HIGH);
    ql_rtos_task_sleep_s(2);

    if( ql_lcd_init() != QL_LCD_SUCCESS )
    {
        QL_TP_DEMO_LOG("LCD init failed");
        goto exit;
    }
    //控制背光，根据实际样机修改
    ql_pwm_open(PWM_PWT);
    ql_pwm_pwt_enable(50, 2000, 1000);

    //必须要先将对应的引脚设置为GPIO功能
    //XP使能脚 GPIO16
    ql_pin_set_func(29, 4);
    //XN使能脚 GPIO22
    ql_pin_set_func(32, 0);
    //YP使能脚 GPIO13
    ql_pin_set_func(53, 0);
    //YN使能脚 GPIO5
    ql_pin_set_func(27, 1);

    tp_cfg.xp_adc_channel = QL_ADC2_CHANNEL;
    tp_cfg.yp_adc_channel = QL_ADC1_CHANNEL;
    tp_cfg.xp = GPIO_16;
    tp_cfg.xn = GPIO_22;
    tp_cfg.yp = GPIO_13;
    tp_cfg.yn = GPIO_5;

    adc_cfg.timeout = 3000;
    adc_cfg.adc_diff = QL_TP_ADC_DIFF;
    adc_cfg.adc_num = QL_TP_ADC_VALUE_TIMES;
    
    ret = ql_tp_init(&tp_cfg, &adc_cfg);
    if (ret != QL_TP_SUCCESS)
    {
        QL_TP_DEMO_LOG("tp init failed");
        goto exit;
    }

    ql_tp_point_t point[QL_TP_CALIB_POINT_NUM] = {{QL_TP_CALIB_POINT_X1, QL_TP_CALIB_POINT_Y1},{QL_TP_CALIB_POINT_X2, QL_TP_CALIB_POINT_Y2},{QL_TP_CALIB_POINT_X3, QL_TP_CALIB_POINT_Y3}};
    calib_cfg.calib = QL_TP_NEED_CALIB;
    memcpy(&calib_cfg.point, point, sizeof(point));
    calib_cfg.calib_width = QL_TP_LCD_CALIB_WIDTH;
    calib_cfg.calib_height = QL_TP_LCD_CALIB_HEIGHT;
    ret = ql_tp_do_calib(&calib_cfg, ql_tp_demo_calib_cb);
    if (ret != QL_TP_SUCCESS)
    {
        QL_TP_DEMO_LOG("tp init failed");
        goto exit;
    }
    
    ret = ql_tp_open(ql_tp_demo_touch_cb);
    if (ret != QL_TP_SUCCESS)
    {
        QL_TP_DEMO_LOG("tp touch failed");
        goto exit;
    }
    //只是为了测试ql_tp_close，实际不需要关闭TP功能
    ql_rtos_task_sleep_s(30);
    ql_tp_close();
    ql_rtos_task_sleep_s(5);

    ret = ql_tp_open(ql_tp_demo_touch_cb);
    if (ret != QL_TP_SUCCESS)
    {
        QL_TP_DEMO_LOG("tp touch failed");
        goto exit;
    }
      
    while(1)
    {
        ql_rtos_task_sleep_s(10);
    }   


    QL_TP_DEMO_LOG("ql_rtos_task_delete");
exit:
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_TP_DEMO_LOG("task deleted failed");
	}
}

QlOSStatus ql_tp_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;
	err = ql_rtos_task_create(&tp_demo_task, TP_DEMO_TASK_STACK_SIZE, TP_DEMO_TASK_PRIO, "ql_tp_demo", ql_tp_demo_task_pthread, NULL, TP_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_TP_DEMO_LOG("demo_task created failed");
        return err;
	}
    
    return err;
}

