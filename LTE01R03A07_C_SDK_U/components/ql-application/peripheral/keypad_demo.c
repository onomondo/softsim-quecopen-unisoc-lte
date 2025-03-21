/**  
  @file
  keypad_demo.c

  @brief
  quectel keypad_demo.

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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_keypad.h"
#include "ql_api_osi.h"
#include "ql_log.h"
#include "keypad_demo.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_KEYPADDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_KEYPADDEMO_LOG(msg, ...)         QL_LOG(QL_KEYPADDEMO_LOG_LEVEL, "ql_KEYPADDEMO", msg, ##__VA_ARGS__)
#define QL_KEYPADDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_KEYPADDEMO", msg, ##__VA_ARGS__)

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


/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_keypad_callback(ql_keymatrix_t keymatrix)
{   
    QL_KEYPADDEMO_LOG("keymap:%d keyout:%d keyin:%d pressd:%d", keymatrix.keymap, keymatrix.keyout, keymatrix.keyin, keymatrix.keystate);
}

void ql_keypad_demo_thread(void *param)
{
    QlOSStatus err = 0;
    uint32_t pressed;
    uint32_t press_last[QL_KEY_MAP_MAX_COUNT] = {0};
    int i=0;

    ql_keypad_out_e row[QL_KEYPAD_ROW_LENGTH] = {QL_KP_OUT0, QL_KP_OUT1, QL_KP_OUT2, QL_KP_OUT3, QL_KP_OUT4, QL_KP_OUT5};
    ql_keypad_in_e  col[QL_KEYPAD_COL_LENGTH] = {QL_KP_IN0, QL_KP_IN1,QL_KP_IN2, QL_KP_IN3, QL_KP_IN_NO_VALID, QL_KP_IN_NO_VALID};

    err = ql_keypad_init(ql_keypad_callback, row, col);
    if(err == QL_KEYPAD_SUCCESS)
    {
        while(1)
        { 
            for (i=QL_KEY_MAP_0; i<QL_KEY_MAP_MAX_COUNT; i++)
            {
                err = ql_keypad_state(&pressed, (ql_keymap_e)i);
                
                if(err == QL_KEYPAD_SUCCESS)
                {
                    if (press_last[i] != pressed)
                    {
                        press_last[i] = pressed;
                        QL_KEYPADDEMO_LOG("keypad%d pressed: %d", i, pressed);
                    }
                }
                else 
                {
                    QL_KEYPADDEMO_LOG("keymap id read error!");
                    continue;
                }
            }
            
            ql_rtos_task_sleep_ms(100);
        }  
    }  
    err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_KEYPADDEMO_LOG("task deleted failed");
	}

}

void ql_keypad_app_init()
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_task_t keypad_task = NULL;

    err = ql_rtos_task_create(&keypad_task, 1024, APP_PRIORITY_NORMAL, "ql_keypaddemo", ql_keypad_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_KEYPADDEMO_LOG("keypad demo task created failed");
    }
}


