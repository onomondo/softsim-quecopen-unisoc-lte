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

#include "mipi_lcd_demo.h"

#define MIPILCD_TEST_ST7701S      1    // Big Endian
#define MIPILCD_TEST_JD9365DA     2    // Big Endian
#define MIPILCD_TEST_SC7705       3    // Little Endian
#define MIPILCD_TEST_ST7365       4    // 1lane

#define MIPILCD_TEST_DRV_SEL      MIPILCD_TEST_ST7701S

/* 禁用dache方案，适用于高频率读取内存的情况，高频率读取内存会与cp侧带宽资源抢占，导致dump */
/* 1.使用禁用方案，需要修改target.config，将CONFIG_APP_MIPI_LCD_RAM_SIZE改为 大于 图片宽*图片高*2，并且必须能整除4096 */
/* 例：480*854图片，480 * 854 * 2 = 819840 = 0xC8280 CONFIG_APP_MIPI_LCD_RAM_SIZE可改为 0xD0000 */
/* 即在target.config中，增加CONFIG_APP_MIPI_LCD_RAM_SIZE=0xD0000 */
/* 2. 在quec_customer_cfg.c中修改成ql_model_mem_ctx.undache_enable = true，即启用禁用dache的方式 */
/* 使用方式，将数据拷贝到禁用dache的首地址，使用该地址 */
/* 3.demo里面需要将MIPI_LCD_UNDACHE改为1*/
#define MIPI_LCD_UNDACHE    0
#define MIPI_LCD_DISPALY_BUF_ADDRESS (CONFIG_RAM_PHY_ADDRESS + CONFIG_QUEC_APP_MIPI_LCD_RAM_OFFSET)

int16 *lcd_undcache = (int16*) MIPI_LCD_DISPALY_BUF_ADDRESS;//获取到禁用dache的首地址

/*** Image Array ***/
/*** Attention!!! ***/
/*** Please focus on the address of array! ***/
/*** The address can't be odd address. Must be align the address. ***/
/*** Add [OSI_CACHE_LINE_ALIGNED] after array name can ensure even address. ***/
/*** Like this: uint8_t mipi1[] OSI_CACHE_LINE_ALIGNED = { ***/
#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7701S )
#include "a_rabbit.h"        // Big Endian Image
#include "rabbit_mouse.h"    // Big Endian Image

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )
#include "mipi1.h"    // Little Endian Image
#include "mipi2.h"    // Little Endian Image

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_SC7705 )
#include "16bit.h"    // Little Endian Image

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )
#include "1lane_1.h"
#include "1lane_2.h"

#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_MIPILCDDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_MIPILCDDEMO_LOG(msg, ...)         QL_LOG(QL_MIPILCDDEMO_LOG_LEVEL, "ql_MIPILCDDEMO", msg, ##__VA_ARGS__)
#define QL_MIPILCDDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_MIPILCDDEMO", msg, ##__VA_ARGS__)

/*===========================================================================
 * Variate
 ===========================================================================*/
// cmd send example
// example is send sw reset cmd
#if 0
static const char test_cmd[] = {0x01};
static const ql_mipi_cmd_s cmd_test[] = {
    {QL_MIPI_LCD_CMD_SWRITE,0,sizeof(test_cmd),test_cmd},
};
#endif

#define LCD_RED        0xf800
#define LCD_GREEN      0x07e0
#define LCD_BLUE       0x001f

#define LCD_WHITE      0xffff
#define LCD_BLACK      0x0000

#define LCD_YELLOW     0xffe0
#define LCD_PURPLE     0xf81f


#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7701S )
#define QL_IMGW_LCDC   480
#define QL_IMGH_LCDC   854

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )
#define QL_IMGW_LCDC   600
#define QL_IMGH_LCDC   1024

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_SC7705 )
#define QL_IMGW_LCDC   800
#define QL_IMGH_LCDC   1280

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )
#define QL_IMGW_LCDC   320
#define QL_IMGH_LCDC   480

#endif

uint16_t Test_image1[30*50] = {};
uint16_t Test_image2[50*100] = {};
uint16_t Test_image3[100*200] = {};

/*===========================================================================
 * Functions
 ===========================================================================*/
void image_test_set(void)
{
    uint16_t count = 0;
    uint16_t image1_len = sizeof(Test_image1)/sizeof(Test_image1[0]);
    uint16_t image2_len = sizeof(Test_image2)/sizeof(Test_image2[0]);
    uint16_t image3_len = sizeof(Test_image3)/sizeof(Test_image3[0]);

    for( count = 0; count < image1_len; count++ )
    {
        Test_image1[count] = LCD_BLUE;
    }
    for( count = 0; count < image2_len; count++ )
    {
        Test_image2[count] = LCD_GREEN;
    }
    for( count = 0; count < image3_len; count++ )
    {
        Test_image3[count] = LCD_RED;
    }
}


void ql_mipi_lcd_write_area(void *buffer, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    if( buffer == NULL )
    {
        return;
    }

    if( (start_x >= QL_IMGW_LCDC)
     || (end_x >= QL_IMGW_LCDC)
     || (start_y >= QL_IMGH_LCDC)
     || (end_y >= QL_IMGH_LCDC) )
    {
        return;
    }

    uint16_t *area_data = (uint16_t *)buffer;
    uint16_t height, width;

    for( height = start_y; height < (end_y + 1); height++ )
    {
        for( width = start_x; width < (end_x + 1); width++ )
        {
#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7701S )

            a_rabbit[height * QL_IMGW_LCDC + width] = *area_data;

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )

            uint8_t low, high;
            low = (*area_data) & 0x00FF;
            high = ((*area_data) & 0xFF00) >> 8;

            mipi1[height * QL_IMGW_LCDC*2 + width*2]     = low;
            mipi1[height * QL_IMGW_LCDC*2 + width*2 + 1] = high;

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_SC7705 )

            uint8_t low, high;
            low = (*area_data) & 0x00FF;
            high = ((*area_data) & 0xFF00) >> 8;

            ac16bit[height * QL_IMGW_LCDC*2 + width*2]     = low;
            ac16bit[height * QL_IMGW_LCDC*2 + width*2 + 1] = high;

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )

            uint8_t low, high;
            low = (*area_data) & 0x00FF;
            high = ((*area_data) & 0xFF00) >> 8;

            mipi_1lane_1[height * QL_IMGW_LCDC*2 + width*2]     = low;
            mipi_1lane_1[height * QL_IMGW_LCDC*2 + width*2 + 1] = high;

#endif

            area_data++;
        }
    }

#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7701S )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,a_rabbit,sizeof(a_rabbit));
        ql_mipi_lcd_write_screen(lcd_undcache);
#else
        ql_mipi_lcd_write_screen(a_rabbit);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,mipi1,sizeof(mipi1));
        ql_mipi_lcd_write_screen(lcd_undcache);
#else
        ql_mipi_lcd_write_screen(mipi1);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_SC7705 )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,ac16bit,sizeof(ac16bit));
        ql_mipi_lcd_write_screen(lcd_undcache);
#else
        ql_mipi_lcd_write_screen(ac16bit);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,mipi_1lane_1,sizeof(mipi_1lane_1));
        ql_mipi_lcd_write_screen(lcd_undcache);
#else
        ql_mipi_lcd_write_screen(mipi_1lane_1);
#endif

#endif
}

/*********************************************************************************
* Attention:
*    1. To run mipi demo, you need to modify the app partition and app ram size. 
*       It is recommended to modify the app partition to 0x220000 and app ram to 0x200000
*    2. LCD backlight needs to be controlled by the customer
*    3. lcd_temp_buffer can be replaced by the customer's own malloc memory, customer 
*       can adjust the partition according to the actual use
**********************************************************************************/
static void ql_mipi_lcd_demo_thread(void *param)
{
    QL_MIPILCDDEMO_LOG("mipi_lcd demo thread enter, param 0x%x", param);

    //ql_event_t event;
    ql_mipi_lcd_info_t lcd_info = {QL_IMGW_LCDC, QL_IMGH_LCDC};

    int err = ql_mipi_lcd_init(&lcd_info);
    if( err < 0 )
    {
        QL_MIPILCDDEMO_LOG("MIPI LCD init failed");
        ql_rtos_task_delete(NULL);
    }

    /* In order to ensure the screen is not blurry when don't insert USB. */
    /* Please call ql_mipi_lcd_release_sys_clk when you don't use MIPI!!! */
    ql_mipi_lcd_request_sys_clk();
    image_test_set();

#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )
    uint16_t height, width;
    uint8_t test_temp;

    for( height = 0; height < QL_IMGH_LCDC; height++ )
    {
        for( width = 0; width < QL_IMGW_LCDC; width++ )
        {
            test_temp = mipi1[height * QL_IMGW_LCDC*2 + width*2];
            mipi1[height * QL_IMGW_LCDC*2 + width*2] = mipi1[height * QL_IMGW_LCDC*2 + width*2 + 1];
            mipi1[height * QL_IMGW_LCDC*2 + width*2 + 1] = test_temp;

            test_temp = mipi2[height * QL_IMGW_LCDC*2 + width*2];
            mipi2[height * QL_IMGW_LCDC*2 + width*2] = mipi2[height * QL_IMGW_LCDC*2 + width*2 + 1];
            mipi2[height * QL_IMGW_LCDC*2 + width*2 + 1] = test_temp;
        }
    }

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )
    ql_lcd_set_brightness(3);

#endif


    while(1)
    {
        //ql_event_wait(&event, 1);

#if ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7701S )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,a_rabbit,sizeof(a_rabbit));
        ql_mipi_lcd_write_screen(lcd_undcache);

        ql_rtos_task_sleep_ms(3000);

        memcpy(lcd_undcache,rabbit_mouse,sizeof(rabbit_mouse));
        ql_mipi_lcd_write_screen(lcd_undcache);

        ql_rtos_task_sleep_ms(3000);
#else
        ql_mipi_lcd_write_screen(a_rabbit);
        ql_rtos_task_sleep_ms(3000);

        ql_mipi_lcd_write_screen(rabbit_mouse);
        ql_rtos_task_sleep_ms(3000);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_JD9365DA )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,mipi1,sizeof(mipi1));
        ql_mipi_lcd_write_screen(lcd_undcache);
        ql_rtos_task_sleep_ms(3000);

        memcpy(lcd_undcache,mipi2,sizeof(mipi2));
        ql_mipi_lcd_write_screen(lcd_undcache);
        ql_rtos_task_sleep_ms(3000);
#else
        ql_mipi_lcd_write_screen(mipi1);
        ql_rtos_task_sleep_ms(3000);

        ql_mipi_lcd_write_screen(mipi2);
        ql_rtos_task_sleep_ms(3000);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_SC7705 )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,ac16bit,sizeof(ac16bit));
        ql_mipi_lcd_write_screen(lcd_undcache);
        ql_rtos_task_sleep_ms(3000);
#else
        ql_mipi_lcd_write_screen(ac16bit);
        ql_rtos_task_sleep_ms(3000);
#endif

#elif ( MIPILCD_TEST_DRV_SEL == MIPILCD_TEST_ST7365 )
#if MIPI_LCD_UNDACHE
        memcpy(lcd_undcache,mipi_1lane_1,sizeof(mipi_1lane_1));
        ql_mipi_lcd_write_screen(lcd_undcache);
        ql_rtos_task_sleep_ms(3000);

        memcpy(lcd_undcache,mipi_1lane_2,sizeof(mipi_1lane_2));
        ql_mipi_lcd_write_screen(lcd_undcache);
        ql_rtos_task_sleep_ms(3000);
#else
        ql_mipi_lcd_write_screen(mipi_1lane_1);
        ql_rtos_task_sleep_ms(3000);

        ql_mipi_lcd_write_screen(mipi_1lane_2);
        ql_rtos_task_sleep_ms(3000);
#endif

#endif

        /* area write */
        ql_mipi_lcd_write_area(Test_image1, 70, 100, 100-1, 150-1);
        ql_rtos_task_sleep_ms(3000);

        ql_mipi_lcd_write_area(Test_image2, 120, 150, 170-1, 250-1);
        ql_rtos_task_sleep_ms(3000);

        ql_mipi_lcd_write_area(Test_image3, 200, 200, 300-1, 400-1);
        ql_rtos_task_sleep_ms(3000);


        //ql_mipi_lcd_write_cmd((void *)cmd_test,sizeof(cmd_test) / sizeof(ql_mipi_cmd_s));    // mipi lcd sw reset
    }

    ql_rtos_task_delete(NULL);
}

void ql_mipi_lcd_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_task_t mipilcd_task = NULL;

    err = ql_rtos_task_create(&mipilcd_task, 5*1024, APP_PRIORITY_NORMAL, "ql_mipilcddemo", ql_mipi_lcd_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_MIPILCDDEMO_LOG("mipi_lcd demo task created failed");
    }
}


