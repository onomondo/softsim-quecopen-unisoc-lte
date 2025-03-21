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
#include "ql_gpio.h"

#include "lcd_demo.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_LCDDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_LCDDEMO_LOG(msg, ...)         QL_LOG(QL_LCDDEMO_LOG_LEVEL, "ql_LCDDEMO", msg, ##__VA_ARGS__)
#define QL_LCDDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_LCDDEMO", msg, ##__VA_ARGS__)

#define BWSCREEN_TEST   0 //open test when set 1, use black white screen

#define TEST_BLACK      0
#define TEST_COLOR      0
ql_mutex_t customer_lcd_lock = NULL;

/*===========================================================================
 * Variate
 ===========================================================================*/
//uint16_t rotation_flg = 0;    // for rotation test

#if !BWSCREEN_TEST
uint16_t Test_image1[30*50] = {};
uint16_t Test_image2[50*100] = {};
uint16_t Test_image3[100*200] = {};
#else
uint8_t bmp[]={
// 请
0x00,
0X20,0X22,0Xec,0X00,0X20,0X22,0Xaa,0Xaa,0Xaa,0Xbf,0Xaa,0Xaa,0Xeb,0Xa2,0X20,0X00,
// 插
0X10,0X10,0X10,0Xff,0X90,0X24,0X24,0Xa4,0X24,0Xfc,0X22,0X23,0X22,0Xb0,0X20,0X00,
// 入
0X00,0X00,0X00,0X00,0X00,0X01,0Xe2,0X1c,0Xe0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//'U'
0Xfc,0Xfc,0X00,0X00,0X00,0Xfc,0Xfc,0X00,
//'I'
0X00,0X00,0X04,0Xfc,0Xfc,0X04,0X00,0X00,
//'M'
0Xfc,0Xfc,0X38,0X70,0X38,0Xfc,0Xfc,0X00,
// 卡
0X40,0X40,0X40,0X40,0X40,0X40,0Xff,0X44,0X44,0X44,0X46,0X44,0X40,0X60,0X40,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//请2
//0x00,
0X00,0X00,0X7f,0X20,0X10,0X00,0Xff,0X0a,0X0a,0X0a,0X4a,0X8a,0X7f,0X00,0X00,0X00,
//插2
0X02,0X42,0X81,0X7f,0X00,0Xff,0X49,0X48,0X40,0X7f,0X40,0X49,0X49,0Xff,0X01,0X00,
//入2
0X80,0X40,0X20,0X10,0X0c,0X03,0X00,0X00,0X00,0X03,0X0c,0X30,0X40,0Xc0,0X40,0X00,
//'U'2
0X07,0X0f,0X08,0X08,0X08,0X0f,0X07,0X00,
//'I'2
0X00,0X00,0X08,0X0f,0X0f,0X08,0X00,0X00,
//'M'2
0X0f,0X0f,0X00,0X00,0X00,0X0f,0X0f,0X00,
// 卡2
0X00,0X00,0X00,0X00,0X00,0X00,0Xff,0X00,0X02,0X04,0X0c,0X18,0X00,0X00,0X00,0X00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

uint8_t gImage[] =  { /* 0X43,0X01,0X71,0X00,0X40,0X00, */
0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

#endif

/*===========================================================================
 * Functions
 ===========================================================================*/
#if !BWSCREEN_TEST
void image_test_set(void)
{
    uint16_t count = 0;
    uint16_t image1_len = sizeof(Test_image1)/sizeof(Test_image1[0]);
    uint16_t image2_len = sizeof(Test_image2)/sizeof(Test_image2[0]);
    uint16_t image3_len = sizeof(Test_image3)/sizeof(Test_image3[0]);

    for( count = 0; count < image1_len; count++ )
    {
        Test_image1[count] = LCD_BLACK;
    }
    for( count = 0; count < image2_len; count++ )
    {
        Test_image2[count] = LCD_YELLOW;
    }
    for( count = 0; count < image3_len; count++ )
    {
        Test_image3[count] = LCD_PURPLE;
    }
}
#else
void ql_lcd_write_bmp(uint8_t *buffer)
{
    uint8_t page, column;

    for( page = 0; page < 8; page++ )
    {
        ql_spi_lcd_write_cmd(0xb0+page);  // from page0
        ql_spi_lcd_write_cmd(0x10);       // from column0_high
        ql_spi_lcd_write_cmd(0x00);       // from column0_low

        for( column = 0; column < 128; column++ )
        {
            ql_spi_lcd_write_data(*buffer++);
        }
    }
}
#endif

#if !(TEST_COLOR&TEST_BLACK)
static void ql_lcd_demo_thread(void *param)
{
    QL_LCDDEMO_LOG("lcd demo thread enter, param 0x%x", param);

    ql_event_t event;
    ql_lcd_info_t lcd_info = {0};
#if BWSCREEN_TEST
    ql_lcd_bw_info_t bw_info = {0};
#endif

    // if you don't want clean screen in the initialization,you can use this API
    // ql_lcd_config_t config = { 0 };
    // ql_lcd_get_config(&config);
    // config.clean_screen = true;
    // ql_lcd_set_config(&config);

    // befor/after lcd_init all OK!!!
    // 1. set the first two parameters when customer's LCD start pixel isn't (0,0)
    // 2. set the last two parameters when customer's LCD driver width&height is inconsistent with QL_driver
    //                                                           (less/greater than QL_driver, is positive/negative value)
    //ql_lcd_set_display_offset(2, 1, 10, 20);

    //before lcd_init set this API!!!
    //if busmode isn't SPI_LINE_4, set other busmode by this API.
    //ql_lcd_set_busmode(QL_LCD_SPI_LINE_3);


#if !BWSCREEN_TEST
    if( ql_lcd_init() != QL_LCD_SUCCESS )
#else
    bw_info.index = QL_LCD_BW_ST7567A;
    if( ql_lcd_init_bwscreen(bw_info) != QL_LCD_SUCCESS )
#endif
    //if( ql_lcd_init_ext(LCD_BLACK, false) != QL_LCD_SUCCESS )
    {
        QL_LCDDEMO_LOG("LCD init failed");
        ql_rtos_task_delete(NULL);
    }

    // befor/after lcd_init all OK!!!
    //ql_lcd_set_display_offset(0, 0, -80, 80);

    ql_lcd_get_info(&lcd_info);
    QL_LCDDEMO_LOG("LCD ID[0x%x] w[%d] h[%d] bus[%d] freq[%d]", lcd_info.drv_id, lcd_info.width, lcd_info.height, lcd_info.bus_mode, lcd_info.freq);

    ql_lcd_config_t get_config = {0};
    ql_lcd_get_config(&get_config);
    QL_LCDDEMO_LOG("LCD get_config lcdrotation[%d] widthoriginal[%d] lcdcolorformat[%d] clean_screen[%d] bus[%d] freq[%d]", get_config.lcdrotation, get_config.widthoriginal, get_config.lcdcolorformat, get_config.clean_screen, get_config.bus_mode, get_config.freq);

#if !BWSCREEN_TEST
    image_test_set();

    /* set backlight brightness, from level 1 to level 5 */
    ql_lcd_set_level_brightness(1, 0);
    ql_lcd_set_level_brightness(2, 7);
    ql_lcd_set_level_brightness(3, 15);
    ql_lcd_set_level_brightness(4, 31);
    ql_lcd_set_level_brightness(5, 63);

    //ql_lcd_set_brightness(5);    //if ql_lcd_init_ext's second argument is false, need to set ql_lcd_set_brightness.
#endif
    
    while(1)
    {
        ql_event_wait(&event, 1);

#if !BWSCREEN_TEST
#if 0   // for rotation test
        // 需根据具体的屏幕驱动的MY MX配置选择指令0x36配置值
        ql_lcd_display_on();
        QL_LCDDEMO_LOG("LCD display on");
        ql_rtos_task_sleep_s(1);

        if( rotation_flg == 0)
        {
            ql_lcd_clear_screen(LCD_RED);
            ql_rtos_task_sleep_s(5);

            ql_lcd_set_display_offset(0, 0, 0, 0);
            ql_spi_lcd_write_cmd(0x36);
            ql_spi_lcd_write_data(0x00);

            ql_lcd_clear_screen(LCD_GREEN);
            ql_rtos_task_sleep_s(5);

            rotation_flg = 1;
        }
        else
        {
            ql_lcd_clear_screen(LCD_YELLOW);
            ql_rtos_task_sleep_s(5);

            ql_lcd_set_display_offset(0, 0, -80, 80);
            ql_spi_lcd_write_cmd(0x36);
            ql_spi_lcd_write_data(0xC0);

            ql_lcd_clear_screen(LCD_BLUE);
            ql_rtos_task_sleep_s(5);

            rotation_flg = 0;
        }

        /* block display BLACK */
        ql_lcd_write(Test_image1, 20, 20, 50-1, 70-1);
        ql_rtos_task_sleep_s(5);
#else
        /* display on */
        ql_lcd_display_on();
        QL_LCDDEMO_LOG("LCD display on");
        ql_rtos_task_sleep_s(1);

        /* full screen RED */
        ql_lcd_clear_screen(LCD_RED);
        ql_rtos_task_sleep_s(5);
        /* block display BLACK */
        ql_lcd_write(Test_image1, 20, 20, 50-1, 70-1);
        ql_rtos_task_sleep_s(5);

        /* full screen GREEN */
        ql_lcd_clear_screen(LCD_GREEN);
        ql_rtos_task_sleep_s(5);
        /* block display YELLOW */
        ql_lcd_write(Test_image2, 50, 50, 100-1, 150-1);
        ql_rtos_task_sleep_s(5);

        /* LCD enter sleep*/
        /* if the lcd has been initialized, the function must be called before enter sleep*/
        ql_lcd_enter_sleep(true);
        QL_LCDDEMO_LOG("LCD enter sleep");
        ql_rtos_task_sleep_s(10);

        /* LCD exit sleep*/
        ql_lcd_enter_sleep(false);
        QL_LCDDEMO_LOG("LCD exit sleep");

        /* full screen BLUE */
        ql_lcd_clear_screen(LCD_BLUE);
        ql_rtos_task_sleep_s(5);
        /* block display PURPLE */
        ql_lcd_write(Test_image3, 100, 100, 200-1, 300-1);
        ql_rtos_task_sleep_s(5);

        /* rotation 90 display */
        ql_spi_lcd_write_cmd(0x36);
        ql_spi_lcd_write_data(0x60);
        ql_lcd_write(Test_image1, 70, 100, 100-1, 150-1);
        ql_rtos_task_sleep_s(3);

        /* normal display */
        ql_spi_lcd_write_cmd(0x36);
        ql_spi_lcd_write_data(0x00);
        ql_lcd_write(Test_image2, 120, 150, 170-1, 250-1);
        ql_rtos_task_sleep_s(3);

        /* full screen WHITE */
        ql_lcd_clear_screen(LCD_WHITE);

        /* brightness adjust */
        ql_lcd_set_brightness(0);
        QL_LCDDEMO_LOG("LCD backlight 0");
        ql_rtos_task_sleep_s(3);

        ql_lcd_set_brightness(1);
        QL_LCDDEMO_LOG("LCD backlight 1");
        ql_rtos_task_sleep_s(3);

        ql_lcd_set_brightness(2);
        QL_LCDDEMO_LOG("LCD backlight 2");
        ql_rtos_task_sleep_s(3);

        ql_lcd_set_brightness(3);
        QL_LCDDEMO_LOG("LCD backlight 3");
        ql_rtos_task_sleep_s(3);

        ql_lcd_set_brightness(4);
        QL_LCDDEMO_LOG("LCD backlight 4");
        ql_rtos_task_sleep_s(3);

        ql_lcd_set_brightness(5);
        QL_LCDDEMO_LOG("LCD backlight 5");
        ql_rtos_task_sleep_s(3);

        /* display off */
        ql_lcd_display_off();
        QL_LCDDEMO_LOG("LCD display off");
        ql_rtos_task_sleep_s(1);
#endif
#else
        ql_lcd_write_bmp(bmp);
        ql_rtos_task_sleep_s(10);
#endif
    }

    ql_rtos_task_delete(NULL);
}
#endif

//双屏，共用专用LCDSPI接口，模拟cs，应用示例
//模拟的cs引脚需要在驱动drv_lcd_xxxx.c中的init函数中，cs拉低

ql_errcode_gpio ql_lcd_pin_gpio_high_init(uint8_t pin_num,ql_GpioNum gpio_num)
{
    ql_pin_set_gpio(pin_num);
    return ql_gpio_init(gpio_num,GPIO_OUTPUT,PULL_NONE,LVL_HIGH);
}

#if TEST_COLOR
static void ql_colorlcd_thread(void *param)
{
    static int a = 1;

    //客户根据自己的方案去配置自己控制的引脚
    ql_lcd_pin_gpio_high_init(COLOR_SCREEN_BACK_LIGHT_PIN,COLOR_SCREEN_BACK_LIGHT_GPIO);//背光控制
    ql_lcd_pin_gpio_high_init(COLOR_SCREEN_SIMULATE_CS_PIN,COLOR_SCREEN_SIMULATE_CS_GPIO);//彩屏cs

    //初始化时或者操作时，需要保持与另一个屏幕互斥
    ql_rtos_mutex_lock(customer_lcd_lock, 0xffffffffUL);

    //客户根据自己想要初始化的lcd类型配置
    ql_lcd_special_t ql_lcd_special = {
        .screen_type = QL_LCD_COLOR_SCREEN,
        .dev_id = COLOR_SCREEN_DEVICE_ID,
        .backlight_sw = false,
    };

    //驱动init加入模拟cs控制
    if( ql_lcd_special_init(&ql_lcd_special) != QL_LCD_SUCCESS )
    {
        QL_LCDDEMO_LOG("LCD init failed");
        ql_rtos_mutex_unlock(customer_lcd_lock);
        ql_rtos_task_delete(NULL);
    }

    //控制lcd屏幕开启显示
    ql_lcd_display_on();

    //刷屏
    ql_lcd_clear_screen(LCD_RED);

    //cs拉高，取消选中
    ql_gpio_set_level(COLOR_SCREEN_SIMULATE_CS_GPIO,LVL_HIGH);
        
    //解锁   
    ql_rtos_mutex_unlock(customer_lcd_lock);

    while(1)
    {
        //初始化时或者操作时，需要保持与另一个屏幕互斥
        ql_rtos_mutex_lock(customer_lcd_lock, 0xffffffffUL);

        //切换屏幕配置为当前的这个屏幕的配置
        ql_lcd_special_change(&ql_lcd_special);

        //控制lcd屏幕开启显示
        ql_lcd_display_on();

        //刷屏
        a++;
        if(a%2)
        {
            ql_lcd_set_special_display_offset(0, 0, 160, -160,&ql_lcd_special);
            ql_spi_lcd_write_cmd(LCD_DISPLAY_INSTRUCTION);
            ql_spi_lcd_write_data(LCD_DISPLAY_ROTATE_90);

            ql_lcd_write(Test_image1, 20, 40, 50-1, 90-1);
            ql_rtos_task_sleep_s(3);
            ql_lcd_clear_screen(LCD_GREEN);
        }  
        else 
        {
            ql_lcd_set_special_display_offset(0, 0, 0, 0,&ql_lcd_special);
            ql_spi_lcd_write_cmd(LCD_DISPLAY_INSTRUCTION);
            ql_spi_lcd_write_data(LCD_DISPLAY_ROTATE_0);

            ql_lcd_write(Test_image1, 20, 20, 50-1, 70-1);
            ql_rtos_task_sleep_s(3);

            ql_lcd_clear_screen(LCD_BLUE);
        }
        
        //cs拉高，取消选中
        ql_gpio_set_level(COLOR_SCREEN_SIMULATE_CS_GPIO,LVL_HIGH);

        //解锁
        ql_rtos_mutex_unlock(customer_lcd_lock);

        //释放cpu资源
        ql_rtos_task_sleep_s(2);
    }

}
#endif

#if TEST_BLACK
static void ql_blacklcd_thread(void *param)
{
    static int a = 1;

    //客户根据自己的方案去配置自己控制的引脚
    ql_lcd_pin_gpio_high_init(BLACK_SCREEN_SIMULATE_CS_PIN,BLACK_SCREEN_SIMULATE_CS_GPIO);//黑白屏cs

    //初始化时或者操作时，需要保持与另一个屏幕互斥
    ql_rtos_mutex_lock(customer_lcd_lock, 0xffffffffUL);

    //客户根据自己想要初始化的lcd类型配置
    ql_lcd_special_t ql_lcd_special = {
        .screen_type = QL_LCD_BLACK_SCREEN,
        .dev_id = BLACK_SCREEN_DEVICE_ID,
        .backlight_sw = true,
    };

    //驱动init加入模拟cs控制
    if( ql_lcd_special_init(&ql_lcd_special) != QL_LCD_SUCCESS )
    {
        QL_LCDDEMO_LOG("LCD init failed");
        ql_rtos_mutex_unlock(customer_lcd_lock);
        ql_rtos_task_delete(NULL);
    }

    //刷屏
    ql_lcd_write_bmp(bmp);

    //cs拉高，取消选中
    ql_gpio_set_level(BLACK_SCREEN_SIMULATE_CS_GPIO,LVL_HIGH);

    //解锁
    ql_rtos_mutex_unlock(customer_lcd_lock);

    while(1)
    {
        //初始化时或者操作时，需要保持与另一个屏幕互斥
        ql_rtos_mutex_lock(customer_lcd_lock, 0xffffffffUL);

        //切换屏幕配置为当前的这个屏幕的配置
        ql_lcd_special_change(&ql_lcd_special);

        //刷屏
        a++;
        if(a%2)
            ql_lcd_write_bmp(gImage);
        else 
            ql_lcd_write_bmp(bmp);

        //cs拉高，取消选中
        ql_gpio_set_level(BLACK_SCREEN_SIMULATE_CS_GPIO,LVL_HIGH);

        //解锁
        ql_rtos_mutex_unlock(customer_lcd_lock);

        //释放cpu资源
        ql_rtos_task_sleep_s(2);
    }
}
#endif

void ql_lcd_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
#if !(TEST_COLOR&TEST_BLACK)
    ql_task_t lcd_task = NULL;
#endif

#if TEST_COLOR
    ql_task_t colorlcd_task = NULL;
#endif
#if TEST_BLACK
    ql_task_t blacklcd_task = NULL;
#endif

// 单屏示例
#if !(TEST_COLOR&TEST_BLACK)
    err = ql_rtos_task_create(&lcd_task, 5*1024, APP_PRIORITY_NORMAL, "ql_lcddemo", ql_lcd_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_LCDDEMO_LOG("lcd demo task created failed");
    }
#endif

// 双屏示例
// 两个屏间操作需要上互斥锁
#if (TEST_COLOR|TEST_BLACK)
    QlOSStatus ret = ql_rtos_mutex_create(&customer_lcd_lock);
	if (QL_OSI_MUTEX_CREATE_FAIL == ret)
	{
		QL_LCDDEMO_LOG("mutex init failed");
		return;
	}

    //相应的屏引脚自行配置，各个型号引脚不同，需要根据型号来修改 PIN_LCD_SPI_SIO，LCD_SPI_SDC，LCD_SPI_CLK
    ql_pin_set_func(LCD_SPI_SIO_PIN, LCD_FUNCTION);
    ql_pin_set_func(LCD_SPI_SDC_PIN, LCD_FUNCTION);
    ql_pin_set_func(LCD_SPI_CLK_PIN, LCD_FUNCTION);

    //必须加，否则切换屏幕时会有白屏现象
    ql_lcd_reset_config(false);
#endif

#if TEST_COLOR
    err = ql_rtos_task_create(&colorlcd_task, 5*1024, APP_PRIORITY_NORMAL, "ql_colorlcddemo", ql_colorlcd_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_LCDDEMO_LOG("lcd demo task created failed");
    }
#endif

#if TEST_BLACK
    err = ql_rtos_task_create(&blacklcd_task, 5*1024, APP_PRIORITY_NORMAL, "ql_blacklcddemo", ql_blacklcd_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_LCDDEMO_LOG("lcd demo task created failed");
    }
#endif
}


