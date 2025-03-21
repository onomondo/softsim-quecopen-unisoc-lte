/**  @file
  ql_api_tp.h

  @brief
  This file is used to define touch penal api for different Quectel Project.

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
=================================================================*/


#ifndef QL_API_TP_H
#define QL_API_TP_H
#include "ql_api_common.h"
#include "ql_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_TP_CALIB_POINT_NUM       3
#define QL_TP_LCD_BLACK         0x0000      //LCD校准显示颜色，0x0000为黑色
#define QL_TP_LCD_WHITE         0xFFFF      //LCD校准显示颜色，0xffff为白色
#define QL_TP_ADC_VALUE_TIMES   1           //TP ADC采样次数
#define QL_TP_ADC_DIFF          10          //ADC采用在diff范围内认为是同一个触摸点


/****************************  error code about ql spi    ***************************/
typedef enum
{
	QL_TP_SUCCESS                  =   0,

    QL_TP_INIT_ERROR               =   1 | (QL_COMPONENT_BSP_TP << 16), //初始化失败
    QL_TP_PARAM_ERROR,                                                  //参数错误
    QL_TP_LCD_NOT_INIT,                                                 //LCD显示屏未初始化
    QL_TP_ADC_ERROR,                                                    //ADC采样失败
    QL_TP_NOT_INIT,                                                     //TP未初始化
    QL_TP_NOT_CALIB_ERR,                                                //TP未校准
    QL_TP_CALIB_ERR,                                                    //TP校准失败
    QL_TP_CFG_ERR,                                                      //配置参数错误
    QL_TP_SEMAPHORE_ERR,                                                //校准创建信号量失败
    QL_TP_TIMER_ERR,                                                    //创建timer失败
    QL_TP_ADC_CH_ERR,                                                   //ADC通道错误
    QL_TP_ADC_SAMPLE_ERR,                                               //ADC采样错误
    QL_TP_GPIO_ERR,                                                     //设置GPIO错误
}ql_errcode_tp_e;

typedef struct
{
    int xp_adc_channel;                         //XP或XN进行ADC采样，实际上读取LCD屏幕Y轴坐标的ADC值
    int yp_adc_channel;                         //YP或YN进行ADC采样，实际上读取LCD屏幕X轴坐标的ADC值
    ql_GpioNum xp;                              //控制XP导通或高阻
    ql_GpioNum xn;                              //控制XN导通或高阻
    ql_GpioNum yp;                              //控制YP导通或高阻
    ql_GpioNum yn;                              //控制YP导通或高阻
}ql_tp_ctg_t;

typedef enum
{
    QL_TP_NOT_CALIB = 0,                        //电阻触摸屏处于未校准状态
    QL_TP_IS_CALIB,                             //电阻触摸屏处于已校准状态
    QL_TP_CALIB_POINT_1,                        //正在校准中，等待点击第一个校准点
    QL_TP_CALIB_POINT_PRESS_1,                  //正在校准中，第一个校准点已经按下
    QL_TP_CALIB_POINT_2,                        //正在校准中，等待点击第二个校准点
    QL_TP_CALIB_POINT_PRESS_2,                  //正在校准中，第二个校准点已经按下
    QL_TP_CALIB_POINT_3,                        //正在校准中，等待点击第三个校准点
    QL_TP_CALIB_POINT_PRESS_3,                  //正在校准中，第三个校准点已经按下
} QL_TP_CALIB_STATUS_E;

//校准过程中的回调函数
typedef void (*ql_tp_calib_cb)(QL_TP_CALIB_STATUS_E calib_status);

typedef enum
{
    QL_TP_NOT_NEED_CALIB = 0,                   //电阻触摸屏不需要校准
    QL_TP_NEED_CALIB,                           //电阻触摸屏需要校准
} QL_TP_DO_CALIB_E;


typedef struct
{
    int x;                                      //LCD屏幕的X轴坐标
    int y;                                      //LCD屏幕的Y轴坐标
}ql_tp_point_t;

typedef enum
{
    QL_TP_NOT_PRESS = 0,                   //电阻触摸屏被按下
    QL_TP_PRESSED,                         //电阻触摸屏被松开
} QL_TP_PRESS_STATE_E;

typedef struct
{
    QL_TP_DO_CALIB_E calib;                     //QL_TP_NEED_CALIB重新校准，为QL_TP_NOT_NEED_CALIB时，执行校准，若触摸屏已校准，则不需要再校准
    ql_tp_point_t point[QL_TP_CALIB_POINT_NUM]; //校准的三个参考坐标，注意不能在同一条直线上
    int calib_width;                            //校准线+的横线长度
    int calib_height;                           //校准线+的竖线宽度
}ql_tp_calib_ctg_t;


typedef struct
{
    int timeout;                                //间隔时间进行TP检测，单位为us
    int adc_num;                                //每次TP检测，X轴和Y轴ADC采样的次数，为2*adc_num次
    int adc_diff;                               //ADC采样的误差，超过误差认为不是同一次采样
}ql_tp_touch_adc_ctg_t;

typedef void (*ql_tp_touch_cb)(ql_tp_point_t, QL_TP_PRESS_STATE_E);

/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_tp_init
*
* Description:
*   初始化电阻触摸屏，调用此API时，触摸屏必须处于未点击状态
* 
* Parameters:
*   tp_cfg      [in]    电阻触摸屏配置
*   adc_cfg     [in]    触摸屏中ADC配置
*
* Return:ql_errcode_tp_e
*
*****************************************************************/
ql_errcode_tp_e ql_tp_init(ql_tp_ctg_t *tp_cfg, ql_tp_touch_adc_ctg_t *adc_cfg);

/*****************************************************************
* Function: ql_tp_do_calib
*
* Description:
*   电阻触摸屏执行校准操作
* 
* Parameters:
*   calib_cfg       [in]    校准的配置信息
*   calib_cb        [in]    校准过程中的回调函数，回调函数可以为NULL
*
* Return:ql_errcode_tp_e
*
*****************************************************************/
ql_errcode_tp_e ql_tp_do_calib(ql_tp_calib_ctg_t *calib_cfg, ql_tp_calib_cb calib_cb);

/*****************************************************************
* Function: ql_tp_open
*
* Description:
*   电阻触摸屏开始进行检测是否按下
* 
* Parameters:
*   touch_cb        [in]    电阻触摸屏点击或者释放的回调函数，不能为NULL
*
* Return:ql_errcode_tp_e
*
*****************************************************************/
ql_errcode_tp_e ql_tp_open(ql_tp_touch_cb touch_cb);

/*****************************************************************
* Function: ql_tp_close
*
* Description:
*   关闭电阻触摸屏的触摸功能
* 
* Parameters:
*
* Return:ql_errcode_tp_e
*
*****************************************************************/
ql_errcode_tp_e ql_tp_close(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_TP_H */



