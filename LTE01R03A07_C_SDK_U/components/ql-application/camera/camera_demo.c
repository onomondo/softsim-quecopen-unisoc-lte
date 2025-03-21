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
#include "ql_api_camera.h"
#include "camera_demo.h"
#include "ql_i2c.h"
#include "ql_lcd.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_CAMERADEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_CAMERADEMO_LOG(msg, ...)         QL_LOG(QL_CAMERADEMO_LOG_LEVEL, "ql_CAMERADEMO", msg, ##__VA_ARGS__)
#define QL_CAMERADEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_CAMERADEMO", msg, ##__VA_ARGS__)
    
#define QL_CAMERA_TASK_STACK_SIZE     		10240
#define QL_CAMERA_TASK_PRIO          	 	APP_PRIORITY_NORMAL
#define QL_CAMERA_TASK_EVENT_CNT      		5

#define SalveAddr_w (0x42 >> 1)
#define SalveAddr_r (0x43 >> 1)

#define TEST_CAPTURE_COUNTS (10)
#define TEST_PREVIEW_COUNTS (500)

/*===========================================================================
 * Struct
 ===========================================================================*/

typedef enum
{
    preview_TEST = 0,      //test preview
    capture_TEST = 1,      //test capture
}QL_CAM_TEST_E;

/*===========================================================================
 * Variate
 ===========================================================================*/

/*===========================================================================
 * Functions
 ===========================================================================*/
void ql_CamUYVY640to320(unsigned char* indata,unsigned char* outBuf)
{
    for(int y = 0; y < 240; y++)
    {
        for(int x = 0; x < 320*2; x++)
        {
            outBuf[320*2*y+x] = indata[640*2*2*y+x+(x/4)*4];
        }
    }
}

//if you need to rotate lcd,you could use this api    
int ql_camPrintExt(uint16_t *imageBuf)
{
    uint16_t *dataBUf = NULL;
	int offset = 0;
    //unsigned char* outBuf = NULL;
    //outBuf = (unsigned char*)malloc(320*240*2);//Space for a display screen
    ql_lcd_config_t lcd_config = {0};
    ql_lcd_get_config(&lcd_config);

    lcd_config.lcdrotation = 1;
    lcd_config.widthoriginal = 320;
    lcd_config.lcdcolorformat = QL_LCD_YUV422_YUYV;
    lcd_config.clean_screen = false;
    lcd_config.bus_mode = QL_LCD_SPI_LINE_4;
    lcd_config.freq = 0; // use default freq

    if(imageBuf == NULL)
    { 
       if(ql_CamPreviewDQBUF(&dataBUf) != QL_CAMERA_SUCCESS)
       {
            QL_CAMERADEMO_LOG("Cam not preview");
            return -1;
       }

       //Preview without rotation, use this section
       lcd_config.lcdrotation = 1;
       ql_lcd_set_config(&lcd_config);
       //ql_CamUYVY640to320((unsigned char*)dataBUf,outBuf);
	   offset = 320 * (240 - 1) * 2;//width of camera:320,height of camera:240,byte of a pixel:2
       ql_lcd_write((uint16_t *)((uint32_t)dataBUf + offset), 0, 0, 239, 319);
       ql_CamPreviewQBUF((uint16_t *)dataBUf);

       //Preview the rotation using this section
       //lcd_config.lcdrotation = 0;
       //ql_lcd_set_config(&lcd_config);
       //ql_lcd_write(dataBUf, 0, 0, 319, 239);
       //ql_CamPreviewQBUF((uint16_t *)dataBUf);
    }
    else
    {
        //ATTENTION:If you do not rotate the LCD, please set the LCD to landscape, i.e. 320 x 240
        lcd_config.lcdrotation = 0;
        ql_lcd_set_config(&lcd_config);
        //ql_CamUYVY640to320((unsigned char*)imageBuf,outBuf);
        ql_lcd_set_display_offset(0, 0, -80, 80);
        ql_spi_lcd_write_cmd(0x36);
        ql_spi_lcd_write_data(0x60);
        ql_lcd_write((uint16_t *)imageBuf, 0, 0, 319, 239);
    }
    //free(outBuf);
    return 0;
}


void ql_CamTest(QL_CAM_TEST_E TestMode)
{
    uint16_t *pCamDataBuffer = NULL;
    uint16_t i;
    if(TestMode == preview_TEST)
    {   
        for(i=0;i<TEST_PREVIEW_COUNTS;i++)
        {
            ql_camPrint(pCamDataBuffer);
            //ql_camPrintExt(pCamDataBuffer);
            //下面延时设置请根据cam的接口(spi\mipi)以及buf配置适当调节
            ql_rtos_task_sleep_ms(60);      
        }
    }
    else
    {
        for(i=0;i<TEST_CAPTURE_COUNTS;i++)
        {
            ql_CamCaptureImage(&pCamDataBuffer);
            ql_camPrint(pCamDataBuffer);
            //ql_camPrintExt(pCamDataBuffer);
            ql_rtos_task_sleep_ms(2000);
        }
    }
}

void ql_camera_demo_thread(void *param)
{
    int lcdstatus;
    //uint8_t read_data = 0;
    //uint8_t data = 0xaa;
    
    ql_CamInit(320, 240);
    //ql_CamInit(640, 480);
    //ql_CamInit0310SPI(320, 240);
    //ql_CamInit0310SPI(640, 480);
    ql_CamPowerOn();     
    
    lcdstatus = ql_lcd_init();
    if(lcdstatus == QL_SUCCESS)
        ql_lcd_display_on();
    
    //The api is used to read the register of sensor,after the camera is successfully powered on.
    //ql_CamI2CRead(0xf0, &read_data, 1);
    //The api is used to write the register of sensor,after the camera is successfully powered on.
    //ql_CamI2CWrite(0x55, data);

    /*Camera test*/
    ql_CamPreview();
    ql_CamTest(preview_TEST);
    ql_CamTest(capture_TEST);
    ql_CamStopPreview();  
    ql_CamPowerOff();
    
    ql_rtos_task_sleep_ms(2000);
    
    ql_CamPowerOn(); 
    ql_CamPreview();
    ql_CamTest(preview_TEST);
    ql_CamTest(capture_TEST);
    ql_CamStopPreview(); 
    ql_CamClose();
    
    if(lcdstatus == QL_SUCCESS)
        ql_lcd_display_off();
    ql_rtos_task_delete(NULL);
}

void ql_camera_app_init(void)
{
    QlCAMERAStatus err = QL_OSI_SUCCESS;
    ql_task_t camera_task = NULL;
        
    err = ql_rtos_task_create(&camera_task, QL_CAMERA_TASK_STACK_SIZE, QL_CAMERA_TASK_PRIO, "camera DEMO", ql_camera_demo_thread, NULL, QL_CAMERA_TASK_EVENT_CNT);
    if (err != QL_OSI_SUCCESS)
    {
        QL_CAMERADEMO_LOG("CAMERA demo task created failed");
    }
}


