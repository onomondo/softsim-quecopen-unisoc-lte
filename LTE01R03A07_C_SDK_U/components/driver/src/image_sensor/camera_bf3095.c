/**  @file
  camera_Bf3095.c

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

static const cameraReg_t RG_InitVgaMIPI[] =
{
 	{0x12, 0x80}, //Bit[7]:1-soft reset
	{0x15, 0x12}, //Bit[1]: VSYNC option; Bit[0]: HREF option
	{0x3e, 0x83},
	{0x09, 0x03}, //Bit[7]: Soft sleep mode; Bit[6:0]: Drive capability
	{0x12, 0x00},
	{0x3a, 0x02}, //YUV422 Sequence
	{0x1e, 0x00}, //Bit[5]: Mirror, 0: Normal image, 1: Mirror image;
	              //Bit[4]: Vertical Flip, 0: Normal image, 1: Vertically flip image;

	//clock, dummy, banding
	{0x1b, 0x0e}, //0x02:4倍频;  0x06:2倍频;  0x09:4/3倍频; 0x2e:2倍频;  0x4e:4倍频;  0x6e:8分频
	{0x2a, 0x00},
	{0x2b, 0x20}, //Dummy Pixel Insert LSB
	{0x92, 0x99},
	{0x93, 0x00},

	//banding
	{0x8a, 0x96}, //50 hz banding
	{0x8b, 0x7d}, //60 hz banding

	//initial AWB and AE
	{0x13, 0x00},  //Manual AWB & AE
	{0x01, 0x15},
	{0x02, 0x23},
	{0x9d, 0x20},
	{0x8c, 0x02},
	{0x8d, 0xee},
	{0x13, 0x07},

	//Contrast
	{0x5d, 0xb3},
	{0xbf, 0x08},
	{0xc3, 0x08},
	{0xca, 0x10},
	{0x62, 0x00},
	{0x63, 0x00},
	{0xb9, 0x00},
	{0x64, 0x00},

	{0x0e, 0x10},
	{0x22, 0x12},

	{0xbb, 0x10},
	{0x08, 0x02},
	{0x20, 0x09},
	{0x21, 0x4f},

	{0xd9, 0x25},
	{0xdf, 0x26},
	{0x2f, 0x84},
	{0x16, 0xa1},  
	{0x6c, 0xc2},  

	//black sun
	{0x71, 0x0f},
	{0x7e, 0x84},
	{0x7f, 0x3c},
	{0x60, 0xe5},
	{0x61, 0xf2},
	{0x6d, 0xc0},

	{0x17, 0x00},
	{0x18, 0xa0},
	{0x19, 0x00},
	{0x1a, 0x78},
	{0x03, 0xa0},
	{0x4a, 0x0c},  

	{0xda, 0x00},
	{0xdb, 0xa2},
	{0xdc, 0x00},
	{0xdd, 0x7a},
	{0xde, 0x00},
	{0x33, 0x10},
	{0x34, 0x08},
	{0x36, 0xc5},
	{0x6e, 0x20},
	{0xbc, 0x0d},

	//lens shading
	{0x34, 0x1d},
	{0x36, 0x45},
	{0x6e, 0x20},
	{0xbc, 0x0d},
	{0x35, 0x30}, //lens shading gain of R
	{0x65, 0x2a}, //lens shading gain of G
	{0x66, 0x2a}, //lens shading gain of B
	{0xbd, 0xf4},
	{0xbe, 0x44},
	{0x9b, 0xf4},
	{0x9c, 0x44},
	{0x37, 0xf4},
	{0x38, 0x44},

	//denoise and edge enhancement
	{0x70, 0x0b},
	{0x71, 0x0e},
	{0x72, 0x4c},
	{0x73, 0x2f}, //0x73[7:4]: The bigger, the smaller noise; 0x73[3:0]: The smaller, the smaller noise
	{0x79, 0x24},
 	{0x74, 0x6c},
	{0x7a, 0x01}, //0x7a[6:4]: Bright edge enhancement; 0x7a[2:0]: Dark edge enhancement
	//color fring correction
	{0x7b, 0x58},
	{0x7d, 0x00},
	//low light and outdoor denoise
	{0x75, 0xaa},
	{0x76, 0x28},
	{0x77, 0x2a},

	//gamma default
	{0x39, 0x9c},
	{0x3f, 0x9c},
	{0x90, 0x20},
	{0x91, 0xd0},
	{0x40, 0x3b},
	{0x41, 0x36},
	{0x42, 0x2b},
	{0x43, 0x1d},
	{0x44, 0x1a},
	{0x45, 0x14},
	{0x46, 0x11},
	{0x47, 0x0e},
	{0x48, 0x0d},
	{0x49, 0x0c},
	{0x4b, 0x0b},
	{0x4c, 0x09},
	{0x4e, 0x08},
	{0x4f, 0x07},
	{0x50, 0x07},

	//color default(outdoor)
	{0x5a, 0x56},
	{0x51, 0x13},
	{0x52, 0x05},
	{0x53, 0x91},
	{0x54, 0x72},
	{0x57, 0x96},
	{0x58, 0x35},

	//color default(normal)
	{0x5a, 0xd6},
	{0x51, 0x29},
	{0x52, 0x0d},
	{0x53, 0x91},
	{0x54, 0x81},
	{0x57, 0x56},
	{0x58, 0x09},
	{0x5b, 0x02},
	{0x5c, 0x30},

	//saturation and contrast
	{0x56, 0x40},
	{0x55, 0x00},
	{0xb0, 0xe0},
	{0xb3, 0x58},
	{0xb4, 0xa3}, //bit[7]: A light saturation
	{0xb1, 0xff},
	{0xb2, 0xa0},
	{0xb4, 0x63}, //bit[7]: non-A light saturation
	{0xb1, 0x90},
	{0xb2, 0x80},

	//AE
	{0x13, 0x07},
	{0x24, 0x48}, //AE Target
	{0x25, 0x88}, //Bit[7:4]: AE_LOC_INT; Bit[3:0]:AE_LOC_GLB
	{0x97, 0x3c}, //AE target value2.
	{0x98, 0xBF}, //AE 
	{0x80, 0x90}, //0x92 Bit[3:2]: the bigger, Y_AVER_MODIFY is smaller
	{0x81, 0xFF}, //AE speed
	{0x82, 0x2a}, //minimum global gain
	{0x83, 0x54},
	{0x84, 0x39},
	{0x85, 0x5d},
	{0x86, 0xff}, //maximum gain
	{0x89, 0x15}, //INT_MAX_MID 
	{0x94, 0xF2}, //Bit[7:4]: Threshold for over exposure pixels, the smaller, the more over exposure pixels; Bit[3:0]: Control the start of AE.
	{0x96, 0xF3},
	{0x9a, 0x50},
	{0x99, 0x10},
	{0x9f, 0xF4},

	//AWB
	{0x6a, 0x81},
	{0x23, 0x55}, //green gain
	{0xa1, 0x31},
	{0xa2, 0x0d}, //the low limit of blue gain for indoor scene
	{0xa3, 0x27}, //the upper limit of blue gain for indoor scene
	{0xa4, 0x0a}, //the low limit of red gain for indoor scene
	{0xa5, 0x2c}, //the upper limit of red gain for indoor scene
	{0xa6, 0x04},
	{0xa7, 0x1a}, //Base B gain
	{0xa8, 0x18}, //Base R gain
	{0xa9, 0x13},
	{0xaa, 0x18},
	{0xab, 0x24},
	{0xac, 0x3c},
	{0xad, 0xf0},
	{0xae, 0x59},
	{0xd0, 0xa3},
	{0xd1, 0x00},
	{0xd3, 0x09},
	{0xd4, 0x24},

	{0xd2, 0x58},
	{0xc5, 0x55},
	{0xc6, 0x88},
	{0xc7, 0x30},

	{0xc8, 0x0d},
	{0xc9, 0x10},
	{0xd3, 0x09},
	{0xd4, 0x24},

	 //Skin
	{0xee, 0x30},
};

sensorInfo_t Bf3095Info =
    {
        "Bf3095",         //		const char *name; // name of sensor
        DRV_I2C_BPS_100K, //		drvI2cBps_t baud;
        0xdc >> 1,        //		uint8_t salve_i2c_addr_w;	 // salve i2c write address
        0xdd >> 1,        //		uint8_t salve_i2c_addr_r;	 // salve i2c read address
        0,                //		uint8_t reg_addr_value_bits; // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
        {0x39, 0x05},     //		uint8_t sensorid[2];
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
        2,
        0,
        0,
        NULL, //		uint8_t *buffer;
        {NULL, NULL},
        false,
        true,
        true,
        false, //		bool isCamIfcStart;
        false, //		bool scaleEnable;
        false,  //		bool cropEnable;
        false, //		bool dropFrame;
        false, //		bool spi_href_inv;
        false, //		bool spi_little_endian_en;
        false,
        false,
        true,                  //       ddr_en
        SENSOR_VDD_2800MV,     //		cameraAVDD_t avdd_val; // voltage of avdd
        SENSOR_VDD_1800MV,     //		cameraAVDD_t iovdd_val;
        CAMA_CLK_OUT_FREQ_24M, //		cameraClk_t sensorClk;
        ROW_RATIO_1_1,         //		camRowRatio_t rowRatio;
        COL_RATIO_1_1,         //		camColRatio_t colRatio;
        CAM_FORMAT_YUV,        //		cameraImageFormat_t image_format; // define in SENSOR_IMAGE_FORMAT_E enum,
        SPI_MODE_NO,           //		camSpiMode_t camSpiMode;
        SPI_OUT_Y1_V0_Y0_U0,   //		camSpiYuv_t camYuvMode;
        camCaptureIdle,        //		camCapture_t camCapStatus;
        camCsi_In,             //		Only mipi interfaces are supported
        NULL, //		drvIfcChannel_t *camp_ipc;
        NULL, //		drvI2cMaster_t *i2c_p;
        NULL, //		CampCamptureCB captureCB;
        NULL,
};

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
void prvCamBf3095PowerOn(void)
{
    sensorInfo_t *p = &Bf3095Info;
    halPmuSetPowerLevel(HAL_POWER_CAMD, p->dvdd_val);
    halPmuSwitchPower(HAL_POWER_CAMD, true, false);
    osiDelayUS(1000);
    halPmuSetPowerLevel(HAL_POWER_CAMA, p->avdd_val);
    halPmuSwitchPower(HAL_POWER_CAMA, true, false);
    osiDelayUS(1000);
}

static void prvCamBf3095PowerOff(void)
{
    halPmuSwitchPower(HAL_POWER_CAMA, false, false);
    osiDelayUS(1000);
    halPmuSwitchPower(HAL_POWER_CAMD, false, false);
    osiDelayUS(1000);
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
bool prvCamBf3095I2cOpen(uint32_t name, drvI2cBps_t bps)
{
    sensorInfo_t *p = &Bf3095Info;
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

static void prvCamBf3095I2cClose()
{
    sensorInfo_t *p = &Bf3095Info;
    if (p->i2c_p != NULL)
        drvI2cMasterRelease(p->i2c_p);
    p->i2c_p = NULL;
}

/*static void prvCamWriteOneReg(uint8_t addr, uint8_t data)
{
    sensorInfo_t *p = &Bf3095Info;
    drvI2cSlave_t idAddress = {p->salve_i2c_addr_w, addr, 0, false};
    if (p->i2c_p != NULL)
    {
        drvI2cWrite(p->i2c_p, &idAddress, &data, 1);
    }
    else
    {
        OSI_LOGE(0, "i2c is not open");//quectel update
    }
}*/

static void prvCamReadReg(uint8_t addr, uint8_t *data, uint32_t len)
{
    sensorInfo_t *p = &Bf3095Info;
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
    sensorInfo_t *p = &Bf3095Info;
    uint16_t regCount;
    drvI2cSlave_t wirte_data = {p->salve_i2c_addr_w, 0, 0, false};
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
            //if(regList[regCount].addr == 0x12 && (regList[regCount].data == 0x80))
            //{
            //    osiDelayUS(1000*20);
            //    OSI_LOGI(0, "cam delay");//quectel update
            //}
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

static bool prvCamBf3095Rginit(sensorInfo_t *info)
{
    switch (info->sensorType)
    {
    case camCsi_In:
        osiDelayUS(10000);
        if (!prvCamWriteRegList(RG_InitVgaMIPI, sizeof(RG_InitVgaMIPI) / sizeof(cameraReg_t)))
            return false;
        break;
    default:
        return false;
    }
    return true;
}


static void prvCsiCamIsrCB(void *ctx)
{
    static uint8_t pictureDrop = 0;
    static uint8_t prev_ovf = 0;
    static uint8_t cap_ovf = 0;
    sensorInfo_t *p = &Bf3095Info;
    REG_CAMERA_IRQ_CAUSE_T cause;
    cameraIrqCause_t mask = {0, 0, 0, 0};
    cause.v = hwp_camera->irq_cause;
    hwp_camera->irq_clear = cause.v;
    OSI_LOGI(0, "cam bf3095 CamIsrCB %d,%d,%d,%d", cause.b.vsync_f, cause.b.ovfl, cause.b.dma_done, cause.b.vsync_r);
    OSI_LOGI(0, "p->camCapStatus %d", p->camCapStatus);
    switch (p->camCapStatus)
    {
    case camCaptureState1:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
            p->isCamIfcStart = true;
            mask.overflow = 1;
            mask.dma = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = camCaptureState2;
        }
        break;
    case camCaptureState2:
        if (cause.b.ovfl)
        {
            OSI_LOGI(0, "cam ovfl ");
            drvCameraControllerEnable(false);
            drvCamCmdSetFifoRst();
            drvCameraControllerEnable(true);
            cap_ovf = 1;
        }
        if (cause.b.dma_done)
        {
            drvCamClrIrqMask();
            if (cap_ovf == 1)
            {
                cap_ovf = 0;
                if (p->isCamIfcStart)
                    drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                p->camCapStatus = camCaptureState1;
                mask.fend = 1;
                drvCamSetIrqMask(mask);
                return;
            }
            if (p->isCamIfcStart == true)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->capturedata))
                {
                    OSI_LOGI(0, "cam p->captureDrops %d ", p->captureDrops);
                    OSI_LOGI(0, "cam pictureDrop %d ", pictureDrop);
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
                        osiSemaphoreRelease(p->cam_sem_capture);
                        if (pictureDrop >= p->captureDrops)
                            pictureDrop = 0;
                        p->isFirst = false;
                    }
                }
            }
        }
        break;
    case campPreviewState1:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            OSI_LOGI(0, "cam mask ovf %d, firstframe %d", prev_ovf, p->isFirstFrame);
            if (prev_ovf || p->isFirstFrame)
            {
                drvCamCmdSetFifoRst();
                drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                p->isCamIfcStart = true;
                p->isFirstFrame = false;
                prev_ovf = 0;
                hwp_camera->irq_clear |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
            }
            hwp_camera->irq_clear |= 1 << 0;
            mask.overflow = 1;
            mask.dma = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = campPreviewState2;
        }
        break;
    case campPreviewState2:
        if (cause.b.ovfl)
        {
            OSI_LOGI(0, "cam ovfl ");
            prev_ovf = 1;

            drvCamClrIrqMask();
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            //stop
            p->camCapStatus = campPreviewState1;
            return;
        }
        if (cause.b.dma_done)
        {
            drvCamClrIrqMask();
            if (prev_ovf == 1)
            {
                prev_ovf = 0;
                if (p->isCamIfcStart)
                    drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                p->camCapStatus = campPreviewState1;
                mask.fend = 1;
                drvCamSetIrqMask(mask);
                return;
            }
            if (p->isCamIfcStart == true)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]))
                {
                    OSI_LOGI(0, "cam 0310 preview release data %x", p->previewdata[p->page_turn]);
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
                    p->page_turn = 1 - p->page_turn;
#endif
                    osiSemaphoreRelease(p->cam_sem_preview);
                    p->isFirst = false;
                    OSI_LOGI(0, "cam 0310 preview_page data %d", p->preview_page);
                    if (--p->preview_page)
                    {
                        p->camCapStatus = campPreviewState1;
                        mask.fend = 1;
                        drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                        p->isCamIfcStart = true;
                        drvCamSetIrqMask(mask);
                    }
                    else
                    {
                        OSI_LOGI(0, "cam go to idle");
                        p->camCapStatus = camCaptureIdle;
                        drvCameraControllerEnable(false);
                    }
                }
                else
                {
                    drvCamClrIrqMask();
                    p->camCapStatus = campPreviewState1;
                    mask.fend = 1;
                    drvCamSetIrqMask(mask);
                    OSI_LOGI(0, "cam dma stop error");
                }
            }
        }
        break;
    default:
        break;
    }
}

//根据初始化函数(ql_CamInit(uint16_t width, uint16_t height))配置的宽、高,自动调整window大小
//NOTE:mipi camera初始化配置的宽、高一定需要和寄存器配置的window大小一致,否则会触发overflow中断,从而拍照失败.
//如果需动态调整其他参数,方法也类似
static void prvCamSetWindow(void)
{
    uint8_t col_h = 0, row_h = 0;
    uint8_t col_l = 0, row_l = 0, low = 0,subsample = 0;

    col_h = Bf3095Info.spi_pixels_per_column >>2;   //high 8 bits  height
    row_h = Bf3095Info.spi_pixels_per_line >> 2;    //high 8 bits  width 
    col_l = Bf3095Info.spi_pixels_per_column & 0x3; // low 2 bits
    row_l = Bf3095Info.spi_pixels_per_line & 0x3 ;  // low 2 bits
    low = 0xa0|row_l<<2 |col_l<<6;
    if(Bf3095Info.spi_pixels_per_line <= 328)
    {
        //subsample = 1; //置位会导致图片不全,如240*320的图片会变成240*(80(黑色)+240)
    }

    cameraReg_t RG_windowMIPI[] = 
    {
    	{0x17, 0x00},
    	{0x18, row_h},
    	{0x19, 0x00},
    	{0x1a, col_h},
    	{0x03, low},
    	{0x4a, 0x0c|subsample}, //BIT[0]: 0:648cols*488rows 1:328cols*488rows
    	{0x12, 0x00}, //Bit[7]:1-soft reset Bit[4]: 0:vga 1:qvga, ex:vga 640*480-->qvga:320*240
    };
    prvCamWriteRegList(RG_windowMIPI, sizeof(RG_windowMIPI) / sizeof(cameraReg_t));
    OSI_LOGI(0, "cam w=%d h=%d",Bf3095Info.spi_pixels_per_line,Bf3095Info.spi_pixels_per_column);
    OSI_LOGI(0, "cam col=%d %d row=%d %d low=%d",col_h,col_l,row_h,row_l,low);
}


bool CamBf3095Open(void)
{
    OSI_LOGI(0, "CamBf3095Open");//quectel update
    sensorInfo_t *p = &Bf3095Info;
    osiRequestSysClkActive(&gcCamCLK);
    drvCamSetPdn(false);
    osiDelayUS(1000);
    prvCamBf3095PowerOn();
    osiDelayUS(1000);
    drvCamSetMCLK(p->sensorClk);
    osiDelayUS(1000);
    drvCamSetPdn(true);
    osiDelayUS(1000);
    drvCamSetPdn(false);
    
    if (!prvCamBf3095I2cOpen(p->i2c_name, p->baud))
    {
        OSI_LOGE(0, "cam prvCamBf3095I2cOpen fail");//quectel update
        prvCamBf3095I2cClose();
        prvCamBf3095PowerOff();
        return false;
    }
    
    if (!prvCamBf3095Rginit(&Bf3095Info))
    {
        OSI_LOGE(0, "cam prvCamBf3095Rginit fail");//quectel update
        prvCamBf3095I2cClose();
        prvCamBf3095PowerOff();
        return false;
    }
    drvCampRegInit(&Bf3095Info);
    prvCamSetWindow();  //动态调整图片的大小
    drvCamSetIrqHandler(prvCsiCamIsrCB, NULL);
    p->isCamOpen = true;
    //drvCameraControllerEnable(true);
    return true;
}

void CamBf3095Close(void)
{
    sensorInfo_t *p = &Bf3095Info;
    if (p->isCamOpen)
    {
        
        osiReleaseClk(&gcCamCLK);
        drvCamSetPdn(true);
        osiDelayUS(1000);
        drvCamDisableMCLK();
        osiDelayUS(1000);
        prvCamBf3095PowerOff();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA         
        drvCamSetPdn(false);
#endif
        prvCamBf3095I2cClose();
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

void CamBf3095GetId(uint8_t *data, uint8_t len)
{
    sensorInfo_t *p = &Bf3095Info;
    if (p->isCamOpen)
    {
        if (data == NULL || len < 1)
            return;
        prvCamReadReg(0xfc, data, len);
    }
}

bool CamBf3095CheckId(void)
{
    sensorInfo_t *p = &Bf3095Info;
    uint8_t sensorID[2] = {0, 0};
    if (!p->isCamOpen)
    {
        drvCamSetPdn(false);
        osiDelayUS(5);
        prvCamBf3095PowerOn();
        drvCamSetMCLK(p->sensorClk);
        drvCamSetPdn(true);
        osiDelayUS(5);
        drvCamSetPdn(false);
        osiDelayUS(5);
        if (!prvCamBf3095I2cOpen(p->i2c_name, p->baud))
        {
            OSI_LOGE(0, "cam prvCamBf3095I2cOpen fail");//quectel update
            return false;
        }
        if (!p->isCamOpen)
        {
            p->isCamOpen = true;
        }
        CamBf3095GetId(sensorID, 2);
        OSI_LOGI(0x10007d67, "cam get id 0x%x,0x%x", sensorID[0], sensorID[1]);
        if ((p->sensorid[0] == sensorID[0]) && (p->sensorid[1] == sensorID[1]))
        {
            OSI_LOGI(0, "check id successful");//quectel update
            CamBf3095Close();
            return true;
        }
        else
        {
            CamBf3095Close();
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

void CamBf3095CaptureImage(uint32_t size)
{
    sensorInfo_t *p = &Bf3095Info;
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

void CamBf3095PrevStart(uint32_t size)
{
    sensorInfo_t *p = &Bf3095Info;
    OSI_LOGI(0, "CamBf3095PrevStart p->preview_page=%d preview size %d", p->preview_page, size);//quectel update
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

void CamBf3095PrevNotify(void)
{
    sensorInfo_t *p = &Bf3095Info;
    uint32_t critical = osiEnterCritical();
    if (p->isCamOpen && !p->isStopPrev)
    {
        if (p->preview_page == 0)
        {
            cameraIrqCause_t mask = {0, 0, 0, 0};

            p->camCapStatus = campPreviewState1;
            p->preview_page++;
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            drvCameraControllerEnable(true);
        }
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
        else
        {
            if (p->preview_page < 2)
                p->preview_page++;
        }
#endif
    }
    osiExitCritical(critical);
}

void CamBf3095StopPrev(void)
{
    sensorInfo_t *p = &Bf3095Info;
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

void CamBf3095SetFalsh(uint8_t level)
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

void CamBf3095Brightness(uint8_t level)
{
}

void CamBf3095Contrast(uint8_t level)
{
}

void CamBf3095ImageEffect(uint8_t effect_type)
{
}
void CamBf3095Ev(uint8_t level)
{
}

void CamBf3095AWB(uint8_t mode)
{
}

void CamBf3095GetSensorInfo(sensorInfo_t **pSensorInfo)
{
    OSI_LOGI(0, "CamBf3095GetSensorInfo %08x", &Bf3095Info);//quectel update
    *pSensorInfo = &Bf3095Info;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
sensorInfo_t *quec_getBf3095Info()
{
    sensorInfo_t *Info = &Bf3095Info;
    return Info;
}

bool quec_camBf3095CheckId(void)
 {
     uint32_t i2c_name[2] = {DRV_NAME_I2C1, DRV_NAME_I2C2};
     uint8_t i;
     uint8_t sensorID[2] = {0, 0};
     uint8_t i2c_ch_cnt = 2;
     sensorInfo_t *Bf3095Info;
     Bf3095Info = quec_getBf3095Info();
     if (!Bf3095Info->isCamOpen)
     {
         drvCamSetPdn(false);
         osiDelayUS(5);
         prvCamBf3095PowerOn();
         drvCamSetMCLK(Bf3095Info->sensorClk);
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
             if (!prvCamBf3095I2cOpen(i2c_name[i], Bf3095Info->baud))
                 continue;
             if (!Bf3095Info->isCamOpen)
             {
                 Bf3095Info->isCamOpen = true;
             }
             CamBf3095GetId(sensorID, 2);
             OSI_LOGI(0,"Bf3095 get id 0x%x,0x%x", sensorID[0], sensorID[1]);//quectel update
             if ((Bf3095Info->sensorid[0] == sensorID[0]) && (Bf3095Info->sensorid[1] == sensorID[1]))
             {
                 Bf3095Info->i2c_name = i2c_name[i];
                 CamBf3095Close();
                 return true;
             }
         }
         CamBf3095Close();
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


bool CamBf3095Reg(SensorOps_t *pSensorOpsCB)
{
    uint8_t ret;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
    extern uint32_t ql_cam_mclk_freq;
    ql_cam_mclk_freq=24000000;
    ret = quec_camBf3095CheckId();
#else
    ret = CamBf3095CheckId();
#endif
    if (ret)
    {
        pSensorOpsCB->cameraOpen = CamBf3095Open;//revise
        pSensorOpsCB->cameraClose = CamBf3095Close;//pass
        pSensorOpsCB->cameraGetID = CamBf3095GetId;//BF3905_MIPI_SENSOR_ID 0x3905 固定id
        pSensorOpsCB->cameraCaptureImage = CamBf3095CaptureImage;//pass
        pSensorOpsCB->cameraStartPrev = CamBf3095PrevStart;//pass
        pSensorOpsCB->cameraPrevNotify = CamBf3095PrevNotify;//pass
        pSensorOpsCB->cameraStopPrev = CamBf3095StopPrev;//pass
        pSensorOpsCB->cameraSetAWB = CamBf3095AWB;//null
        pSensorOpsCB->cameraSetBrightness = CamBf3095Brightness;//null
        pSensorOpsCB->cameraSetContrast = CamBf3095Contrast;//null
        pSensorOpsCB->cameraSetEv = CamBf3095Ev;//null
        pSensorOpsCB->cameraSetImageEffect = CamBf3095ImageEffect;//null
        pSensorOpsCB->cameraGetSensorInfo = CamBf3095GetSensorInfo;//pass
        pSensorOpsCB->cameraFlashSet = CamBf3095SetFalsh;//pass
        return true;
    }
    return false;
}

