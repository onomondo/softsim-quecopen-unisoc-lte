/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "osi_api.h"
#include "osi_log.h"
#include "hwregs.h"
#include "drv_config.h"
#include "hal_gouda.h"
#include "hal_chip.h"
#include "drv_lcd.h"
#include "quec_proj_config.h"

#define LCD_DRV_ID_ST7567A 0x7567
#define LCD_WIDTH  128
#define LCD_HEIGHT 64

static uint16_t st7567a_line_mode;
static bool st7567a_farmk;



#define LCM_WR_REG(Addr, Data)                             \
    {                                                      \
        while (halGoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                              \
    }
#define LCD_DataWrite_ST7567A(Data)                    \
    {                                                 \
        while (halGoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                         \
    }
#define LCD_CtrlWrite_ST7567A(Cmd)                   \
    {                                               \
        while (halGoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                       \
    }

static void _st7567aClose(void);
static uint32_t _st7567aReadId(void);
static void _st7567aSleepIn(bool is_sleep);
static void _st7567aInit(void);
static void _st7567aInvalidate(void);
static void _st7567aInvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
);

static void _st7567aSetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t right, // end Horizon address
    uint16_t top,   // start Vertical address
    uint16_t bottom // end Vertical address
);
static void _lcdDelayMs(int ms_delay)
{
    osiDelayUS(ms_delay * 1000);
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
/**************************************************************************************/
static void _st7567aInit(void)
{
    OSI_LOGI(0, "lcd:    st7567aInit ");

    //hardware reset
    halGoudatResetLcdPin();
    _lcdDelayMs(20);
    
#ifndef __QUEC_OEM_VER_AC__
    //software reset
    LCD_CtrlWrite_ST7567A(0xe2);    //LCD_CMD_RESET
    _lcdDelayMs(5);

    //set power
    LCD_CtrlWrite_ST7567A(0x2f);    // follower on, LCD_CMD_PWR
    _lcdDelayMs(5);

    //set contrast
    LCD_CtrlWrite_ST7567A(0x24);    // !!!set regulation ratio=5, range is 0x20-0x27
    LCD_CtrlWrite_ST7567A(0x81);    // set EV
    LCD_CtrlWrite_ST7567A(0x28);    // !!!set EV=32, range is 0x00-0x3f

    //set bias mode
    LCD_CtrlWrite_ST7567A(0xa2);    // duty 1/65, bs=0 1/9

    //seg com set
    LCD_CtrlWrite_ST7567A(0xa0);    // 0xa0 is seg0->seg131, 0xa1 is seg131->seg0
    LCD_CtrlWrite_ST7567A(0xc8);    // 0xc0 is com0->com63 , 0xc8 is com63->com0

    //set back pressure???
    LCD_CtrlWrite_ST7567A(0xf8);    // back pressure ratio
    LCD_CtrlWrite_ST7567A(0x00);    // 00-10

    //set start line
    LCD_CtrlWrite_ST7567A(0x40);    // from line0

    //lcd display
    LCD_CtrlWrite_ST7567A(0xa6);    // 0xa6 is normal display, 0xa7 is inverse display white->black/black->white
    LCD_CtrlWrite_ST7567A(0xa4);    // 0xa4 is normal display, 0xa5 is all pixels ON
    LCD_CtrlWrite_ST7567A(0xaf);    // 0xaf is display on, 0xae is display off
#endif
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
/******************************************************************************/
static void _st7567aSetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t top,   // start Vertical address
    uint16_t right, // end Horizon address
    uint16_t bottom // end Vertical address
)
{
    OSI_LOGI(0, "lcd:    _st7567aSetDisplayWindow L = %d, top = %d, R = %d, bottom = %d", left, top, right, bottom);

    top &= 0x7f;
    top >>= 3;

    bottom &= 0x7f;
    bottom >>= 3;

    left &= 0x7f;
    right &= 0x7f;

    //设置col的高位
    LCD_CtrlWrite_ST7567A(0x10 | ((left>>4)&0x0f));
    LCD_CtrlWrite_ST7567A(0x00 | (left&0x0f));

    //设置page页
    LCD_CtrlWrite_ST7567A(0xb0 | top);

}

static void _st7567aSleepIn(bool is_sleep)
{
    OSI_LOGI(0, "lcd:  st7567a  _st7567aSleepIn, is_sleep = %d", is_sleep);

    if (is_sleep)
    {
        LCD_CtrlWrite_ST7567A(0xae); //display off
        _lcdDelayMs(120);
        LCD_CtrlWrite_ST7567A(0xa5); // enter sleep mode
    }
    else
    {
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
#if 0
        LCD_CtrlWrite_ST7567A(0xa4); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_ST7567A(0xaf); // display on
#endif
        _st7567aInit();
#else
        LCD_CtrlWrite_ST7567A(0xa4); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_ST7567A(0xaf); // display on
#endif
    }
}

static void _st7567aClose(void)
{
    OSI_LOGI(0, "lcd:  st7567a   in _st7567aClose");

    _st7567aSleepIn(true);
}

static void _st7567aInvalidate(void)
{
    OSI_LOGI(0, "lcd:  st7567a   in _st7567aInvalidate");

    _st7567aSetDisplayWindow(0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}

void _st7567aInvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
)
{

    OSI_LOGI(0, "lcd: st7567a _st7567aInvalidateRect");
    left = (left >= LCD_WIDTH) ? LCD_WIDTH - 1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH - 1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : bottom;

    if ((right < left) || (bottom < top))
    {

        OSI_ASSERT(false, "lcd: st7567a _st7567aInvalidateRect error"); /*assert verified*/
    }

    _st7567aSetDisplayWindow(left, top, right, bottom);
}

static void _st7567aRotationInvalidateRect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle)
{

    OSI_LOGI(0, "lcd: st7567a st7567aRotationInvalidateRect");
    switch (angle)
    {
    case LCD_ANGLE_0:
        _st7567aSetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_90:
        _st7567aSetDisplayWindow(left, top, bottom, right);
        break;
    case LCD_ANGLE_180:
        _st7567aSetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_270:
        _st7567aSetDisplayWindow(left, top, bottom, right);
        break;
    default:
        _st7567aSetDisplayWindow(left, top, right, bottom);
        break;
    }
}

static void _st7567aEsdCheck(void)
{

    OSI_LOGI(0, "lcd:  st7567a  st7567aEsdCheck");
}

static const lcdOperations_t st7567aOperations =
    {
        _st7567aInit,
        _st7567aSleepIn,
        _st7567aEsdCheck,
        _st7567aSetDisplayWindow,
        _st7567aInvalidateRect,
        _st7567aInvalidate,
        _st7567aClose,
        _st7567aRotationInvalidateRect,
        NULL,
        _st7567aReadId,
};

const lcdSpec_t g_lcd_st7567a =
    {
        LCD_DRV_ID_ST7567A,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
        (lcdOperations_t *)&st7567aOperations,
        false,
        0x2a000,
        20000000,
};

static uint32_t _st7567aReadId(void)
{
    uint32_t ret_id = 0;
    //HAL_ERR_T r_err;
    //uint8_t id = 0;
    OSI_LOGI(0, "lcd: rst and read st7567a Id \n");
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    halPmuSwitchPower(HAL_POWER_LCD, true, true);
#endif
    _lcdDelayMs(10);
    st7567a_farmk = g_lcd_st7567a.is_use_fmark;
    st7567a_line_mode = g_lcd_st7567a.bus_mode;

    halGoudaReadConfig(st7567a_line_mode, g_lcd_st7567a.is_use_fmark, g_lcd_st7567a.fmark_delay);
    _lcdDelayMs(140);
    //hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    //r_err = halGoudaReadData(0xfc, &id, 1);
    //hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    //_lcdDelayMs(10);
    //OSI_LOGI(0, "lcd:  st7567a  _st7567aReadId LCM: 0x%0x (read return : %d)\n", id, r_err);

    ret_id = 0x7567;

    if (LCD_DRV_ID_ST7567A == ret_id)
    {
        OSI_LOGI(0, "lcd:  st7567a is using !");
    }

    halGoudaClose();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    halPmuSwitchPower(HAL_POWER_LCD, false, false);
#endif
    return ret_id;
}
