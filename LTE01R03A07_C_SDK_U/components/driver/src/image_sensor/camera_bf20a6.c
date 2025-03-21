/**  @file
  camera_bf20a6.c

  @brief
  This file is the driver of camera bf30a2 .

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

//#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_INFO

#include "osi_log.h"
#include "osi_api.h"
#include "hwregs.h"
#include <stdlib.h>
#include "hal_chip.h"
#include "drv_i2c.h"
#include "image_sensor.h"
#include "drv_camera.h"
#include "drv_names.h"
#include "osi_clock.h"
#include "quec_cust_feature.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
#include "ql_i2c.h"
#include "quec_customer_cfg.h"
#endif

static osiClockConstrainRegistry_t gcCamCLK = {.tag = HAL_NAME_CAM};

const cameraReg_t RG_BFInitVgaSPI[] =
    {
        {0xf2,0x01},
        {0x12,0xc0},  

        {0x3a,0x02},
        {0xe1,0x92},
        {0xe3,0x12},    
        {0xe0,0x00},
        {0x2a,0x98},
        {0xcd,0x17},
        {0xc0,0x10},
        {0xc6,0x1d},
        {0x10,0x35},
        {0xe2,0x09},
        {0xe4,0x72},
        {0xe5,0x22},
        {0xe6,0x24},
        {0xe7,0x64},
        {0xe8,0xf2},
        {0x4a,0x00},
        {0x00,0x03},
        {0x1f,0x02},
        {0x22,0x02},
        {0x0c,0x31}, 
		
		{0x00,0x00},
		{0x60,0x81},
		{0x61,0x81},
		
        {0xa0,0x08},
        {0x01,0x1a},
		{0x01,0x1a},
		{0x01,0x1a},
        {0x02,0x15},
		{0x02,0x15},
		{0x02,0x15},
        {0x13,0x08},
		{0x8a,0x96},
		{0x8b,0x06},
        {0x87,0x18},	
        
        {0x34,0x48},
		{0x35,0x40},
		{0x36,0x40},
		
        {0x71,0x44},
        {0x72,0x48},
        {0x74,0xa2},
        {0x75,0xa9},
		{0x78,0x12},
		{0x79,0xa0},
		{0x7a,0x94},
		{0x7c,0x97},
        {0x40,0x30},
        {0x41,0x30},
        {0x42,0x28},
        {0x43,0x1f},
        {0x44,0x1c},
        {0x45,0x16},
        {0x46,0x13},
        {0x47,0x10},
        {0x48,0x0d},
        {0x49,0x0c},
        {0x4B,0x0a},
        {0x4C,0x0b},
        {0x4E,0x09},
        {0x4F,0x08},
        {0x50,0x08},
        
		{0x5f,0x29},
		{0x23,0x33},
		{0xa1,0x10},
		{0xa2,0x0d},
		{0xa3,0x30},
		{0xa4,0x06},
		{0xa5,0x22},
		{0xa6,0x56},
		{0xa7,0x18},
		{0xa8,0x1a},
		{0xa9,0x12},
		{0xaa,0x12},
		{0xab,0x16},
		{0xac,0xb1},
		{0xba,0x12},
		{0xbb,0x12},
		{0xad,0x12},
		{0xae,0x56},
		{0xaf,0x0a},
		{0x3b,0x30},
		{0x3c,0x12},
		{0x3d,0x22},
		{0x3e,0x3f},
		{0x3f,0x28},
		{0xb8,0xc3},
		{0xb9,0xA3},
		{0x39,0x47},
		{0x26,0x13},
		{0x27,0x16},
		{0x28,0x14},
		{0x29,0x18},
		{0xee,0x0d},
		
        {0x13,0x05},
		{0x24,0x3C},
		{0x81,0x20},
		{0x82,0x40},
		{0x83,0x30},
		{0x84,0x58},
		{0x85,0x30},
		{0x92,0x08},
		{0x86,0xA0},
		{0x8a,0x96},
		{0x91,0xff},
		{0x94,0x62},
		{0x9a,0x18},//outdoor threshold
		{0xf0,0x4e},
		{0x51,0x17},//color normal
		{0x52,0x03},
		{0x53,0x5F},
		{0x54,0x47},
		{0x55,0x66},
		{0x56,0x0F},
		{0x7e,0x14},
		{0x57,0x36},//A光color
		{0x58,0x2A},
		{0x59,0xAA},
		{0x5a,0xA8},
		{0x5b,0x43},
		{0x5c,0x10},
		{0x5d,0x00},
		{0x7d,0x36},
		{0x5e,0x10},

		{0xd6,0x88},//contrast
		{0xd5,0x20},//低光加亮度
		{0xb0,0x84},//灰色区域降饱和度
		{0xb5,0x08},//低光降饱和度阈值
		{0xb1,0xc8},//saturation
		{0xb2,0xc0},
		{0xb3,0xd0},
		{0xb4,0xB0},

		{0x32,0x10},
		{0xa0,0x09},

		{0x00,0x03},

		{0x0b,0x00},
        //{0x17,0x28},//160
		//{0x18,0x78},//480
		//{0x19,0x1e},//120
		//{0x1a,0x5a},//360
};

sensorInfo_t bf20a6Info =
    {
        "bf20a6",         //		const char *name; // name of sensor
        DRV_I2C_BPS_100K, //		drvI2cBps_t baud;
        0xdc >> 1,        //		uint8_t salve_i2c_addr_w;	 // salve i2c write address
        0xdd >> 1,        //		uint8_t salve_i2c_addr_r;	 // salve i2c read address
        0,                //		uint8_t reg_addr_value_bits; // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
        {0x20, 0xa6},     //		uint8_t sensorid[2];

        640,              //        uint16_t spi_pixels_per_line;   // max width of source image
        480,              //        uint16_t spi_pixels_per_column; // max height of source image

        1,                //		uint16_t rstActiveH;	// 1: high level valid; 0: low level valid
        100,              //		uint16_t rstPulseWidth; // Unit: ms. Less than 200ms
        1,                //		uint16_t pdnActiveH;	// 1: high level valid; 0: low level valid
        0,                //		uint16_t dstWinColStart;
        640,              //        uint16_t dstWinColEnd;
        0,                //		uint16_t dstWinRowStart;
        480,              //		uint16_t dstWinRowEnd;
        1,                //		uint16_t spi_ctrl_clk_div;
        DRV_NAME_I2C1,    //		uint32_t i2c_name;
        0,                //		uint32_t nPixcels;
        1,                //		uint8_t captureDrops;
        0,
        0,
        NULL, //		uint8_t *buffer;
        {NULL, NULL},
        false,
        true,
        true,
        false, //		bool isCamIfcStart;
        false, //		bool scaleEnable;
        true,  //		bool cropEnable;
        false, //		bool dropFrame;
        false, //		bool spi_href_inv;
        false, //		bool spi_little_endian_en;
        false,
        false,
        false,                 //       ddr_en
        SENSOR_VDD_2800MV,     //		cameraAVDD_t avdd_val; // voltage of avdd
        SENSOR_VDD_1800MV,     //		cameraAVDD_t iovdd_val;
        CAMA_CLK_OUT_FREQ_12M, //		cameraClk_t sensorClk;
        ROW_RATIO_1_1,         //		camRowRatio_t rowRatio;
        COL_RATIO_1_1,         //		camColRatio_t colRatio;
        CAM_FORMAT_YUV,        //		cameraImageFormat_t image_format; // define in SENSOR_IMAGE_FORMAT_E enum,
        SPI_MODE_MASTER2_2,    //		camSpiMode_t camSpiMode;
        SPI_OUT_Y1_V0_Y0_U0,   //		camSpiYuv_t camYuvMode;
        camCaptureIdle,        //		camCapture_t camCapStatus;
        camSpi_In,
        NULL, //		drvIfcChannel_t *camp_ipc;
        NULL, //		drvI2cMaster_t *i2c_p;
        NULL, //		CampCamptureCB captureCB;
        NULL,
};

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
void prvCamBf20a6PowerOn(void)
{
    sensorInfo_t *p = &bf20a6Info;
    halPmuSetPowerLevel(HAL_POWER_CAMD, p->dvdd_val);
    halPmuSwitchPower(HAL_POWER_CAMD, true, false);
    osiDelayUS(1000);
    halPmuSetPowerLevel(HAL_POWER_CAMA, p->avdd_val);
    halPmuSwitchPower(HAL_POWER_CAMA, true, false);
    osiDelayUS(1000);
}

static void prvCamBf20a6PowerOff(void)
{
    halPmuSwitchPower(HAL_POWER_CAMA, false, false);
    osiDelayUS(1000);
    halPmuSwitchPower(HAL_POWER_CAMD, false, false);
    osiDelayUS(1000);
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
bool prvCamBf20a6I2cOpen(uint32_t name, drvI2cBps_t bps)
{
    sensorInfo_t *p = &bf20a6Info;
    if (name == 0 || bps != DRV_I2C_BPS_100K || p->i2c_p != NULL)
    {
        return false;
    }
    p->i2c_p = drvI2cMasterAcquire(name, bps);
    if (p->i2c_p == NULL)
    {
        OSI_LOGE(0, "cam i2c open fail");//quectel update
        return false;
    }
    return true;
}

static void prvCamBf20a6I2cClose()
{
    sensorInfo_t *p = &bf20a6Info;
    if (p->i2c_p != NULL)
        drvI2cMasterRelease(p->i2c_p);
    p->i2c_p = NULL;
}

static void prvCamWriteOneReg(uint8_t addr, uint8_t data)
{
    sensorInfo_t *p = &bf20a6Info;
    drvI2cSlave_t idAddress = {p->salve_i2c_addr_w, addr, 0, false};
    if (p->i2c_p != NULL)
    {
        drvI2cWrite(p->i2c_p, &idAddress, &data, 1);
    }
    else
    {
        OSI_LOGE(0, "i2c is not open");//quectel update
    }
}

static void prvCamReadReg(uint8_t addr, uint8_t *data, uint32_t len)
{
    sensorInfo_t *p = &bf20a6Info;
    drvI2cSlave_t idAddress = {p->salve_i2c_addr_w, addr, 0, false};
    if (p->i2c_p != NULL)
    {
        drvI2cRead(p->i2c_p, &idAddress, data, len);
    }
    else
    {
        OSI_LOGE(0, "i2c is not open");//quectel update
    }
}

static bool prvCamWriteRegList(const cameraReg_t *regList, uint16_t len)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
    uint8_t retry_cnt = 0;
#endif
    sensorInfo_t *p = &bf20a6Info;
    uint16_t regCount;
    drvI2cSlave_t wirte_data = {p->salve_i2c_addr_w, 0, 0, false};
    prvCamWriteOneReg(0xf4, 0x1c);
    osiDelayUS(1000);
    for (regCount = 0; regCount < len; regCount++)
    {
        OSI_LOGI(0, "cam write reg %x,%x", regList[regCount].addr, regList[regCount].data);//quectel update
        wirte_data.addr_data = regList[regCount].addr;
#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        if (drvI2cWrite(p->i2c_p, &wirte_data, &regList[regCount].data, 1))
            osiDelayUS(5);
        else
            return false;
#else
retry:
        if (drvI2cWrite(p->i2c_p, &wirte_data, &regList[regCount].data, 1))
        {
            retry_cnt = 0;
            osiDelayUS(5);
        }
        else
        {
            if(retry_cnt++ > 3)
            {
                return false;
            }
            else
            {
                goto retry;
            }
        }
#endif
    }
    return true;
}

static bool prvCamBf20a6Rginit(sensorInfo_t *info)
{
    switch (info->sensorType)
    {
    case camSpi_In:
        if (!prvCamWriteRegList(RG_BFInitVgaSPI, sizeof(RG_BFInitVgaSPI) / sizeof(cameraReg_t)))
            return false;
        break;
    default:
        return false;
    }
    return true;
}

static void prvCamIsrCB(void *ctx)
{
    sensorInfo_t *p = &bf20a6Info;
    static uint8_t pictureDrop = 0;
    REG_CAMERA_IRQ_CAUSE_T cause;
    cameraIrqCause_t mask = {0, 0, 0, 0};
    cause.v = hwp_camera->irq_cause;
    hwp_camera->irq_clear = cause.v;
    OSI_LOGI(0, "cam bf20a6 prvCsiCamIsrCB %d,%d,%d,%d", cause.b.vsync_f, cause.b.ovfl, cause.b.dma_done, cause.b.vsync_r);//quectel update
    switch (p->camCapStatus)
    {
    case camCaptureState1:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart == true)
            {
                drvCampStopTransfer(p->nPixcels, p->capturedata);
                p->isCamIfcStart = false;
            }
            drvCampStartTransfer(p->nPixcels, p->capturedata);
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = camCaptureState2;
            p->isCamIfcStart = true;
        }
        break;
    case camCaptureState2:
        if (cause.b.vsync_f)
        {
            if (p->isCamIfcStart)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->capturedata))
                {
                    if (pictureDrop < p->captureDrops)
                    {
                        mask.fend = 1;
                        drvCamSetIrqMask(mask);
                        p->camCapStatus = camCaptureState1;
                        pictureDrop++;
                    }
                    else
                    {
                        p->camCapStatus = camCaptureIdle;
                        OSI_LOGD(0, "cam bf20a6 campture release %x", p->capturedata);//quectel update
                        osiSemaphoreRelease(p->cam_sem_capture);
                        if (pictureDrop >= p->captureDrops)
                            pictureDrop = 0;
                        p->isFirst = false;
                    }
                }
                else
                {
                    drvCampStartTransfer(p->nPixcels, p->capturedata);
                    mask.fend = 1;
                    p->isCamIfcStart = true;
                    drvCamSetIrqMask(mask);
                }
            }
        }
        break;
    case campPreviewState1:
        if (cause.b.vsync_f)
        {   
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart)
            {
                drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                p->isCamIfcStart = false;
            }
            drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = campPreviewState2;
            p->isCamIfcStart = true;
        }
        break;
    case campPreviewState2:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart == true)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]))
                {
                    OSI_LOGD(0, "cam bf20a6 preview release %x", p->previewdata[p->page_turn]);//quectel update
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
                    p->page_turn = 1 - p->page_turn;
#endif
                    osiSemaphoreRelease(p->cam_sem_preview);
                    p->isFirst = false;
                    if (--p->preview_page)
                    {
                        drvCamClrIrqMask();
                        drvCamCmdSetFifoRst();
                        drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                        p->isCamIfcStart = true;
                        mask.fend = 1;
                        drvCamSetIrqMask(mask);
                    }
                    else
                    {
                        drvCamClrIrqMask();
                        p->camCapStatus = camCaptureIdle;
                    }
                }
                else
                {
                    p->camCapStatus = campPreviewState1;
                    mask.fend = 1;
                    drvCamSetIrqMask(mask);
                }
            }
        }
        break;
    default:
        break;
    }
}

bool CamBf20a6Open(void)
{
    OSI_LOGI(0, "CamBf20a6Open");//quectel update
    sensorInfo_t *p = &bf20a6Info;
    osiRequestSysClkActive(&gcCamCLK);
    drvCamSetPdn(false);
    osiDelayUS(1000);
    prvCamBf20a6PowerOn();
    osiDelayUS(1000);
    drvCamSetMCLK(p->sensorClk);
    osiDelayUS(1000);
    drvCamSetPdn(true);
    osiDelayUS(1000);
    drvCamSetPdn(false);

    if (!prvCamBf20a6I2cOpen(p->i2c_name, p->baud))
    {
        OSI_LOGE(0, "cam prvCamBf20a6I2cOpen fail");//quectel update
        prvCamBf20a6I2cClose();
        prvCamBf20a6PowerOff();
        return false;
    }
    if (!prvCamBf20a6Rginit(&bf20a6Info))
    {
        OSI_LOGE(0, "cam prvCamBf20a6Rginit fail");//quectel update
        prvCamBf20a6I2cClose();
        prvCamBf20a6PowerOff();
        return false;
    }
    drvCampRegInit(&bf20a6Info);
    drvCamSetIrqHandler(prvCamIsrCB, NULL);
    p->isCamOpen = true;
    drvCameraControllerEnable(true);
    return true;
}

void CamBf20a6Close(void)
{
    sensorInfo_t *p = &bf20a6Info;
    if (p->isCamOpen)
    {
        
        osiReleaseClk(&gcCamCLK);
        drvCamSetPdn(true);
        osiDelayUS(1000);
        drvCamDisableMCLK();
        osiDelayUS(1000);
        prvCamBf20a6PowerOff();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA         
        drvCamSetPdn(false);
#endif
        prvCamBf20a6I2cClose();
        drvCampRegDeInit();
        drvCamDisableIrq();
        p->isFirst = true;
        p->isCamOpen = false;
    }
    else
    {
        p->isCamOpen = false;
    }
}

void CamBf20a6GetId(uint8_t *data, uint8_t len)
{
    sensorInfo_t *p = &bf20a6Info;
    if (p->isCamOpen)
    {
        if (data == NULL || len < 1)
            return;
        prvCamReadReg(0xfc, data, len);
    }
}

bool CamBf20a6CheckId(void)
{
    sensorInfo_t *p = &bf20a6Info;
    uint8_t sensorID[2] = {0, 0};
    if (!p->isCamOpen)
    {
        drvCamSetPdn(false);
        osiDelayUS(5);
        prvCamBf20a6PowerOn();
        drvCamSetMCLK(p->sensorClk);
        drvCamSetPdn(true);
        osiDelayUS(5);
        drvCamSetPdn(false);
        osiDelayUS(5);
        if (!prvCamBf20a6I2cOpen(p->i2c_name, p->baud))
        {
            OSI_LOGE(0, "cam prvCamBf20a6I2cOpen fail");//quectel update
            return false;
        }
        if (!p->isCamOpen)
        {
            p->isCamOpen = true;
        }
        CamBf20a6GetId(sensorID, 2);
        OSI_LOGI(0x10007d67, "cam get id 0x%x,0x%x", sensorID[0], sensorID[1]);
        if ((p->sensorid[0] == sensorID[0]) && (p->sensorid[1] == sensorID[1]))
        {
            OSI_LOGI(0, "check id successful");//quectel update
            CamBf20a6Close();
            return true;
        }
        else
        {
            CamBf20a6Close();
            OSI_LOGE(0, "check id error");//quectel update
            return false;
        }
    }
    else
    {
        OSI_LOGE(0, "camera already opened !");//quectel update
        return false;
    }
}

void CamBf20a6CaptureImage(uint32_t size)
{
    sensorInfo_t *p = &bf20a6Info;
    if (size != 0)
    {
        cameraIrqCause_t mask = {0, 0, 0, 0};
        drvCamSetIrqMask(mask);
        if (p->isCamIfcStart == true)
        {
            drvCampStopTransfer(p->nPixcels, p->capturedata);
            p->isCamIfcStart = false;
        }
        p->nPixcels = size;
        p->camCapStatus = camCaptureState1;

        mask.fend = 1;
        drvCamSetIrqMask(mask);
        drvCameraControllerEnable(true);
    }
}

void CamBf20a6PrevStart(uint32_t size)
{
    sensorInfo_t *p = &bf20a6Info;
    OSI_LOGI(0, "CamBf20a6PrevStart p->preview_page=%d preview size %d", p->preview_page, size);//quectel update
    if (p->preview_page == 0)
    {
        cameraIrqCause_t mask = {0, 0, 0, 0};
        drvCamSetIrqMask(mask);
        if (p->isCamIfcStart == true)
        {
            drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
            p->isCamIfcStart = false;
        }
        p->nPixcels = size;
        p->camCapStatus = campPreviewState1;
        p->isStopPrev = false;
#ifdef CONFIG_CAMERA_SINGLE_BUFFER
        p->preview_page = 1;
#else
        p->preview_page = 2;
#endif
        mask.fend = 1;
        drvCamSetIrqMask(mask);
        drvCameraControllerEnable(true);
    }
}

void CamBf20a6PrevNotify(void)
{
    sensorInfo_t *p = &bf20a6Info;
    if (p->isCamOpen && !p->isStopPrev)
    {
        if (p->preview_page == 0)
        {
            cameraIrqCause_t mask = {0, 0, 0, 0};

            p->camCapStatus = campPreviewState1;
            p->preview_page++;
            mask.fend = 1;
            drvCamSetIrqMask(mask);
        }
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
        else
        {
            if (p->preview_page < 2)
                p->preview_page++;
        }
#endif
    }
}

void CamBf20a6StopPrev(void)
{
    sensorInfo_t *p = &bf20a6Info;
    drvCamClrIrqMask();
    if (p->isCamIfcStart == true)
    {
        drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
        p->isCamIfcStart = false;
    }
    drvCameraControllerEnable(false);
    p->camCapStatus = camCaptureIdle;
    p->isStopPrev = true;
    p->preview_page = 0;
}

void CamBf20a6SetFalsh(uint8_t level)
{
    if (level >= 0 && level < 16)
    {
        if (level == 0)
        {
            halPmuSwitchPower(HAL_POWER_CAMFLASH, false, false);
        }
        else
        {
            halPmuSetCamFlashLevel(level);
            halPmuSwitchPower(HAL_POWER_CAMFLASH, true, false);
        }
    }
}

void CamBf20a6Brightness(uint8_t level)
{
}

void CamBf20a6Contrast(uint8_t level)
{
}

void CamBf20a6ImageEffect(uint8_t effect_type)
{
}
void CamBf20a6Ev(uint8_t level)
{
}

void CamBf20a6AWB(uint8_t mode)
{
}

void CamBf20a6GetSensorInfo(sensorInfo_t **pSensorInfo)
{
    OSI_LOGI(0, "CamBf20a6GetSensorInfo %08x", &bf20a6Info);//quectel update
    *pSensorInfo = &bf20a6Info;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
sensorInfo_t *quec_getBf20a6Info()
{
    sensorInfo_t *Info = &bf20a6Info;
    return Info;
}

bool quec_camBf20a6CheckId(void)
 {
     uint32_t i2c_name[2] = {DRV_NAME_I2C1, DRV_NAME_I2C2};
     uint8_t i;
     uint8_t sensorID[2] = {0, 0};
     uint8_t i2c_ch_cnt = 2;
     sensorInfo_t *bf20a6Info;
     bf20a6Info = quec_getBf20a6Info();
     if (!bf20a6Info->isCamOpen)
     {
         drvCamSetPdn(false);
         osiDelayUS(5);
         prvCamBf20a6PowerOn();
         drvCamSetMCLK(bf20a6Info->sensorClk);
         drvCamSetPdn(true);
         osiDelayUS(5);
         drvCamSetPdn(false);
         osiDelayUS(5);

         extern uint8_t  ql_cam_iic_ch;
         if(ql_cam_iic_ch != CAM_IIC_CH_AUTO)
         {
             i2c_ch_cnt = 1;
             i2c_name[0] = (ql_cam_iic_ch == i2c_1) ? DRV_NAME_I2C1 : ((ql_cam_iic_ch == i2c_2) ? DRV_NAME_I2C2 : DRV_NAME_I2C3);
         }
         for(i=0;i<i2c_ch_cnt;i++)
         {
             sensorID[0] = 0;
             sensorID[1] = 0;
             if (!prvCamBf20a6I2cOpen(i2c_name[i], bf20a6Info->baud))
                 continue;
             if (!bf20a6Info->isCamOpen)
             {
                 bf20a6Info->isCamOpen = true;
             }
             CamBf20a6GetId(sensorID, 2);
             OSI_LOGI(0,"bf20a6 get id 0x%x,0x%x", sensorID[0], sensorID[1]);//quectel update
             if ((bf20a6Info->sensorid[0] == sensorID[0]) && (bf20a6Info->sensorid[1] == sensorID[1]))
             {
                 bf20a6Info->i2c_name = i2c_name[i];
                 CamBf20a6Close();
                 return true;
             }
         }
         CamBf20a6Close();
         OSI_LOGI(0,"check id error");//quectel update
         return false;
     }
     else
     {
         OSI_LOGI(0,"camera is opened");//quectel update
         return false;
     }
 }
#endif


bool CamBf20a6Reg(SensorOps_t *pSensorOpsCB)
{
    
    uint8_t ret;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
    ret = quec_camBf20a6CheckId();

#else
    ret = CamBf20a6CheckId();
#endif
    if (ret)
    {
        pSensorOpsCB->cameraOpen = CamBf20a6Open;
        pSensorOpsCB->cameraClose = CamBf20a6Close;
        pSensorOpsCB->cameraGetID = CamBf20a6GetId;
        pSensorOpsCB->cameraCaptureImage = CamBf20a6CaptureImage;
        pSensorOpsCB->cameraStartPrev = CamBf20a6PrevStart;
        pSensorOpsCB->cameraPrevNotify = CamBf20a6PrevNotify;
        pSensorOpsCB->cameraStopPrev = CamBf20a6StopPrev;
        pSensorOpsCB->cameraSetAWB = CamBf20a6AWB;
        pSensorOpsCB->cameraSetBrightness = CamBf20a6Brightness;
        pSensorOpsCB->cameraSetContrast = CamBf20a6Contrast;
        pSensorOpsCB->cameraSetEv = CamBf20a6Ev;
        pSensorOpsCB->cameraSetImageEffect = CamBf20a6ImageEffect;
        pSensorOpsCB->cameraGetSensorInfo = CamBf20a6GetSensorInfo;
        pSensorOpsCB->cameraFlashSet = CamBf20a6SetFalsh;
        return true;
    }
    return false;
}

