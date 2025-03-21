/**  @file
  ql_keypad.h

  @brief
  This file is used to define keypad interface for different Quectel Project.

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
16/11/2020        Neo         Init version
=================================================================*/

#ifndef QL_KEYPAD_H
#define QL_KEYPAD_H

#include "ql_api_common.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_KEYPAD_ERRCODE_BASE (QL_COMPONENT_BSP_KEYPAD<<16)
#define QL_KEYPAD_ROW_LENGTH  6
#define QL_KEYPAD_COL_LENGTH  6  

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
/**
 * keypad errcode
 */
typedef enum
{
    QL_KEYPAD_SUCCESS = QL_SUCCESS,

    QL_KEYPAD_INVALID_PARAM_ERR                 = 1|QL_KEYPAD_ERRCODE_BASE
}ql_errcode_keypad_e;

/**
 * keyout pin
 * if the module has GNSS function, keyout4 and keyout5 cannot be used
 */
typedef enum
{
	QL_KP_OUT0 = 0,
	QL_KP_OUT1,
	QL_KP_OUT2,
	QL_KP_OUT3,
	QL_KP_OUT4,
	QL_KP_OUT5,
	QL_KP_OUT_NO_VALID
}ql_keypad_out_e;

/**
 * keyin pin 
 * keyin4 and keyin5 can be reused as UART2 RXD/TXD and UART3 CTS/RTS. 
 * if it is enabled here, other functions will not be available
 * if it is not reused for other functions, please ignore this sentence
 */
typedef enum 
{
    QL_KP_IN0 = 0,
	QL_KP_IN1,
	QL_KP_IN2,
	QL_KP_IN3,
	QL_KP_IN4,
	QL_KP_IN5,
	QL_KP_IN_NO_VALID
}ql_keypad_in_e;

/**
 * keypad position,support 6*6 keypad，Compatible with the old 6*5 keypad

          keyin0            keyin1         keyin2         keyin3          keyin4         keyin5

keyout0   QL_KEY_MAP_0    QL_KEY_MAP_1   QL_KEY_MAP_2    QL_KEY_MAP_3   QL_KEY_MAP_4   QL_KEY_MAP_5

keyout1   QL_KEY_MAP_31   QL_KEY_MAP_6   QL_KEY_MAP_7    QL_KEY_MAP_8   QL_KEY_MAP_9   QL_KEY_MAP_10

keyout2   QL_KEY_MAP_32   QL_KEY_MAP_11  QL_KEY_MAP_12   QL_KEY_MAP_13  QL_KEY_MAP_14  QL_KEY_MAP_15

keyout3   QL_KEY_MAP_33   QL_KEY_MAP_16  QL_KEY_MAP_17   QL_KEY_MAP_18  QL_KEY_MAP_19  QL_KEY_MAP_20

keyout4   QL_KEY_MAP_34   QL_KEY_MAP_21  QL_KEY_MAP_22   QL_KEY_MAP_23  QL_KEY_MAP_24  QL_KEY_MAP_25

keyout5   QL_KEY_MAP_35  QL_KEY_MAP_26  QL_KEY_MAP_27   QL_KEY_MAP_28  QL_KEY_MAP_29  QL_KEY_MAP_30

*/
typedef enum
{
    QL_KEY_MAP_0 = 0,
    QL_KEY_MAP_1,         
    QL_KEY_MAP_2,         
    QL_KEY_MAP_3,         
    QL_KEY_MAP_4,         
    QL_KEY_MAP_5,         
    QL_KEY_MAP_6,         
    QL_KEY_MAP_7,         
    QL_KEY_MAP_8,         
    QL_KEY_MAP_9,         
    QL_KEY_MAP_10,     
    QL_KEY_MAP_11,     
    QL_KEY_MAP_12,      
    QL_KEY_MAP_13,     
    QL_KEY_MAP_14,        
    QL_KEY_MAP_15,      
    QL_KEY_MAP_16,       
    QL_KEY_MAP_17,    
    QL_KEY_MAP_18,    
    QL_KEY_MAP_19,    
    QL_KEY_MAP_20,      
    QL_KEY_MAP_21,      
    QL_KEY_MAP_22,        
    QL_KEY_MAP_23,        
    QL_KEY_MAP_24,        
    QL_KEY_MAP_25,        
    QL_KEY_MAP_26,        
    QL_KEY_MAP_27,        
    QL_KEY_MAP_28,
    QL_KEY_MAP_29,
    QL_KEY_MAP_30,
    QL_KEY_MAP_31,
    QL_KEY_MAP_32,
    QL_KEY_MAP_33,
    QL_KEY_MAP_34,
    QL_KEY_MAP_35,
    QL_KEY_MAP_MAX_COUNT  ///< total count
} ql_keymap_e;

typedef enum
{
    QL_KEY_STATE_PRESS = (1 << 0),
    QL_KEY_STATE_RELEASE = (1 << 1),
} ql_keystate_e;


/*========================================================================
*  Type Definition
*========================================================================*/
typedef struct
{
   ql_keymap_e keymap;      //按键id
   uint32_t keyout;         //按键对应的输出通道
   uint32_t keyin;          //按键对应的输入通道
   ql_keystate_e keystate;  //按键状态,2表示按键按下, 1表示按键未被按下
}ql_keymatrix_t;


typedef struct
{
    uint8_t debunceTime;    //消抖时间
    uint8_t kpItvTime;      //中断间隔时间
}ql_keypad_config_t;
/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Description: keypad 回调函数，用于按键触发通知
* 
* Parameters:
*   key         [in]    按键状态结构体 
*
*
*****************************************************************/
typedef void (*ql_keyeventcb_t)(ql_keymatrix_t key);

/*****************************************************************
* Function: ql_keypad_state
*
* Description: 获取按键状态
* 
* Parameters:
*   pressed	    [out] 	按键状态,1表示按键处于按下状态, 0表示按键未被按下 
* 	id  	    [in] 	按键id, 取值范围见ql_keymap_e枚举 
*
* Return:
*   0           获取按键状态成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_keypad_e ql_keypad_state(uint32_t *pressed, ql_keymap_e id);

/*****************************************************************
* Function: ql_keypad_init

* Attention:
   1. the callback will be run in interrupt, so it is forbidden to block the callback;
   2. it is forbidden to call Audio start/stop/close， file write/read，CFW（releated to RPC）in interrupt;
   3. it is forbidden to enter critical in interrupt
   4. it is suggested for users to  perform simple operations , or send event(no timeout) to inform your task in interrupt
   
* Parameters:
*
* Description: 矩阵按键初始化
* 
* Parameters:
*   cb	        [in] 	回调函数 
* 	keyrow  	[in] 	输出通道数据,取值范围见ql_keypad_out_e枚举
*   keycol  	[in] 	输入通道数据,取值范围见ql_keypad_in_e枚举
*
* Return:
*   0           获取按键状态成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_keypad_e ql_keypad_init(ql_keyeventcb_t cb, ql_keypad_out_e keyrow[QL_KEYPAD_ROW_LENGTH], ql_keypad_in_e keycol[QL_KEYPAD_COL_LENGTH]);

/*****************************************************************
* Function: ql_keypad_set_config

* Attention:
   1. Need to be called before ql_keypad_init function
*
* Description: 矩阵按键配置，一般不需要使用，对于按键反应速度极快需求的客户才需要使用到
* 
* Parameters:
*    cfg	                      [in] 	结构体指针 
* 	->debunceTime  	            [in] 	消抖时间设置，计算公式(debunceTime+1)*0.3125*6=xx ms，取值范围（0-30）
*   ->kpItvTime  	              [in] 	中断间隔设置，计算公式（kpItvTime+1）*(debunceTime+1)*0.3125*6=xx ms，取值范围（0-63）
*
* Return:
*   0           获取按键状态成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_keypad_e ql_keypad_set_config(ql_keypad_config_t *cfg);
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_KEYPAD_H */

