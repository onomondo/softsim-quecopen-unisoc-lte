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

#define LCD_DRV_ID_ST7796S 0x7796f0
#define LCD_WIDTH 320
#define LCD_HEIGHT 480

static uint16_t st7796s_line_mode;
static bool st7796s_farmk;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
lcdSpec_t g_lcd_st7796ss;
#endif

#define LCM_WR_REG(Addr, Data)                             \
    {                                                      \
        while (halGoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                              \
    }
#define LCD_DataWrite_st7796s(Data)                   \
    {                                                 \
        while (halGoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                         \
    }
#define LCD_CtrlWrite_st7796s(Cmd)                  \
    {                                               \
        while (halGoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                       \
    }

static void _st7796sClose(void);
static uint32_t _st7796sReadId(void);
static void _st7796sSleepIn(bool is_sleep);
static void _st7796sInit(void);
static void _st7796sInvalidate(void);
static void _st7796sInvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
);

static void _st7796sSetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t right, // end Horizon address
    uint16_t top,   // start Vertical address
    uint16_t bottom // end Vertical address
);
static void _lcdDelayMs(int ms_delay)
{
    osiDelayUS(ms_delay * 1000);
}

OSI_UNUSED static void _st7796sSetDirection(lcdDirect_t direct_type)
{

    OSI_LOGI(0x1000aa35, "lcd:    _st7796sSetDirection = %d", direct_type);

    switch (direct_type)
    {
    case LCD_DIRECT_NORMAL:
        LCD_CtrlWrite_st7796s(0x36);
        LCD_DataWrite_st7796s(0x00);
        break;
    case LCD_DIRECT_ROT_90:
        LCD_CtrlWrite_st7796s(0x36);
        LCD_DataWrite_st7796s(0x60);
        break;
    }

    LCD_CtrlWrite_st7796s(0x2c);
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
/******************************************************************************/
static void _st7796sSetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t top,   // start Vertical address
    uint16_t right, // end Horizon address
    uint16_t bottom // end Vertical address
)
{
    uint16_t newleft = left;
    uint16_t newtop = top;
    uint16_t newright = right;
    uint16_t newbottom = bottom;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
    newleft   = newleft   + ql_lcd_offset_cfg.start_x_offset;
    newtop    = newtop    + ql_lcd_offset_cfg.start_y_offset;
    newright  = newright  + ql_lcd_offset_cfg.start_x_offset;
    newbottom = newbottom + ql_lcd_offset_cfg.start_y_offset;
#endif
    OSI_LOGI(0x1000aa36, "lcd:st7796sSetDisplayWindow L = %d, top = %d, R = %d, bot = %d", left, top, right, bottom);

    LCD_CtrlWrite_st7796s(0x2a);                   // set hori start , end (left, right)
    LCD_DataWrite_st7796s((newleft >> 8) & 0xff);  // left high 8 b
    LCD_DataWrite_st7796s(newleft & 0xff);         // left low 8 b
    LCD_DataWrite_st7796s((newright >> 8) & 0xff); // right high 8 b
    LCD_DataWrite_st7796s(newright & 0xff);        // right low 8 b

    LCD_CtrlWrite_st7796s(0x2b);                    // set vert start , end (top, bot)
    LCD_DataWrite_st7796s((newtop >> 8) & 0xff);    // top high 8 b
    LCD_DataWrite_st7796s(newtop & 0xff);           // top low 8 b
    LCD_DataWrite_st7796s((newbottom >> 8) & 0xff); // bot high 8 b
    LCD_DataWrite_st7796s(newbottom & 0xff);        // bot low 8 b
    LCD_CtrlWrite_st7796s(0x2c);                    // recover memory write mode
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
/**************************************************************************************/
static void _st7796sInit(void)
{
    OSI_LOGI(0x1000aa37, "lcd:    _st7796sInit ");
    LCD_CtrlWrite_st7796s(0x11); //Sleep out
    _lcdDelayMs(100);            //Delay 120ms
                                 //------------------------------------st7796sS Frame Rate-----------------------------------------//
    LCD_CtrlWrite_st7796s(0Xf0);
    LCD_DataWrite_st7796s(0xc3);
    LCD_CtrlWrite_st7796s(0Xf0);
    LCD_DataWrite_st7796s(0x96);
    LCD_CtrlWrite_st7796s(0X36);
    LCD_DataWrite_st7796s(0x48);
    LCD_CtrlWrite_st7796s(0X3a);
    LCD_DataWrite_st7796s(0x05);
    LCD_CtrlWrite_st7796s(0Xe6);
    LCD_DataWrite_st7796s(0x0f);
    LCD_DataWrite_st7796s(0xf2);
    LCD_DataWrite_st7796s(0x3f);
    LCD_DataWrite_st7796s(0x4f);
    LCD_DataWrite_st7796s(0x4f);
    LCD_DataWrite_st7796s(0x28);
    LCD_DataWrite_st7796s(0x0e);
    LCD_DataWrite_st7796s(0x00);
    LCD_CtrlWrite_st7796s(0Xc5);
    LCD_DataWrite_st7796s(0x2a);
    LCD_CtrlWrite_st7796s(0Xe0);
    LCD_DataWrite_st7796s(0xf0);
    LCD_DataWrite_st7796s(0x03);
    LCD_DataWrite_st7796s(0x0a);
    LCD_DataWrite_st7796s(0x11);
    LCD_DataWrite_st7796s(0x14);
    LCD_DataWrite_st7796s(0x1c);
    LCD_DataWrite_st7796s(0x3b);
    LCD_DataWrite_st7796s(0x55);
    LCD_DataWrite_st7796s(0x4a);
    LCD_DataWrite_st7796s(0x0a);
    LCD_DataWrite_st7796s(0x13);
    LCD_DataWrite_st7796s(0x14);
    LCD_DataWrite_st7796s(0x1c);
    LCD_DataWrite_st7796s(0x1f);
    LCD_CtrlWrite_st7796s(0Xe1);
    LCD_DataWrite_st7796s(0xf0);
    LCD_DataWrite_st7796s(0x03);
    LCD_DataWrite_st7796s(0x0a);
    LCD_DataWrite_st7796s(0x0c);
    LCD_DataWrite_st7796s(0x0c);
    LCD_DataWrite_st7796s(0x09);
    LCD_DataWrite_st7796s(0x36);
    LCD_DataWrite_st7796s(0x54);
    LCD_DataWrite_st7796s(0x49);
    LCD_DataWrite_st7796s(0x0f);
    LCD_DataWrite_st7796s(0x1b);
    LCD_DataWrite_st7796s(0x18);
    LCD_DataWrite_st7796s(0x1b);
    LCD_DataWrite_st7796s(0x1f);
    LCD_CtrlWrite_st7796s(0Xf0);
    LCD_DataWrite_st7796s(0x3c);
    LCD_CtrlWrite_st7796s(0Xf0);
    LCD_DataWrite_st7796s(0x69);

    LCD_CtrlWrite_st7796s(0x29); //Display on

    _lcdDelayMs(20);
    LCD_CtrlWrite_st7796s(0x2c);
}

static void _st7796sSleepIn(bool is_sleep)
{
    OSI_LOGI(0x1000aa38, "lcd:   _st7796sSleepIn, is_sleep = %d", is_sleep);

    if (is_sleep)
    {
        LCD_CtrlWrite_st7796s(0x28); //display off
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7796s(0x10); // enter sleep mode
    }
    else
    {

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
#if 0
        LCD_CtrlWrite_st7796s(0x11); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7796s(0x29); // display on
#endif
        _st7796sInit();
#else
        LCD_CtrlWrite_st7796s(0x11); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7796s(0x29); // display on
#endif
    }
}

static void _st7796sClose(void)
{
    OSI_LOGI(0x10007dd8, "lcd:     in GC9304_Close");

    _st7796sSleepIn(true);
}
static void _st7796sInvalidate(void)
{
    OSI_LOGI(0x1000aa39, "lcd:     in _st7796sInvalidate");

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    _st7796sSetDisplayWindow(0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
#else //no use
    _st7796sSetDisplayWindow(0x0, 0x0, (g_lcd_st7796ss.width) - 1, (g_lcd_st7796ss.height) - 1);
#endif
}

void _st7796sInvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
)
{

    OSI_LOGI(0x1000aa3a, "lcd:  _st7796sInvalidateRect");
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    left = (left >= LCD_WIDTH) ? LCD_WIDTH - 1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH - 1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : bottom;
#else //no use
    left = (left >= (g_lcd_st7796ss.width)) ? (g_lcd_st7796ss.width) - 1 : left;
    right = (right >= (g_lcd_st7796ss.width)) ? (g_lcd_st7796ss.width) - 1 : right;
    top = (top >= (g_lcd_st7796ss.height)) ? (g_lcd_st7796ss.height) - 1 : top;
    bottom = (bottom >= (g_lcd_st7796ss.height)) ? (g_lcd_st7796ss.height) - 1 : bottom;
#endif

    if ((right < left) || (bottom < top))
    {

        OSI_ASSERT(false, "lcd:  _st7796sInvalidateRect error"); /*assert verified*/
    }

    _st7796sSetDisplayWindow(left, top, right, bottom);
}

static void _st7796sRotationInvalidateRect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle)
{

    OSI_LOGI(0x10007dcf, "lcd: gc9305 gc9305RotationInvalidateRect");
    switch (angle)
    {
    case LCD_ANGLE_0:
        _st7796sSetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_90:
        _st7796sSetDisplayWindow(left, top, bottom, right);
        break;
    case LCD_ANGLE_180:
        _st7796sSetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_270:
        _st7796sSetDisplayWindow(left, top, bottom, right);
        break;
    default:
        _st7796sSetDisplayWindow(left, top, right, bottom);
        break;
    }
}

static void _st7796sEsdCheck(void)
{

    OSI_LOGI(0x1000aa3b, "lcd:  st7796ssEsdCheck");
}

static const lcdOperations_t st7796ssOperations =
    {
        _st7796sInit,
        _st7796sSleepIn,
        _st7796sEsdCheck,
        _st7796sSetDisplayWindow,
        _st7796sInvalidateRect,
        _st7796sInvalidate,
        _st7796sClose,
        _st7796sRotationInvalidateRect,
        NULL,
        _st7796sReadId,
};

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
const
#endif
lcdSpec_t g_lcd_st7796ss =
    {
        LCD_DRV_ID_ST7796S,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
        (lcdOperations_t *)&st7796ssOperations,
        false,
        0x18000,
        10000000, // 500000, //10000000, 10M
};

static uint32_t _st7796sReadId(void)
{
    uint32_t ret_id = 0;
    HAL_ERR_T r_err;
    uint8_t id[4] = {0};
    //OSI_LOGI(0x1000aa3c, "lcd:st7796sReadId \n");  //打印不出来 quectel update
    OSI_LOGI(0, "lcd:st7796sReadId \n");
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    halPmuSwitchPower(HAL_POWER_LCD, true, true);
#endif
    _lcdDelayMs(10);
    st7796s_farmk = g_lcd_st7796ss.is_use_fmark;
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    st7796s_line_mode = g_lcd_st7796ss.bus_mode;
#else
    st7796s_line_mode = ql_lcd_config.bus_mode;
#endif

    halGoudaReadConfig(st7796s_line_mode, g_lcd_st7796ss.is_use_fmark, g_lcd_st7796ss.fmark_delay);
    _lcdDelayMs(140);
    //hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    _lcdDelayMs(10);
    r_err = halGoudaReadData(0x04, id, 4);
    //hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    //OSI_LOGI(0x1000aa3d, "lcd:st7796sReadId LCM: 0x%0x, 0x%x, 0x%0x (read return : %d)\n", id[2], id[1], id[3], r_err);//quectel update
    OSI_LOGI(0, "lcd:st7796sReadId LCM: 0x%0x, 0x%x, 0x%0x (read return : %d)\n", id[2], id[1], id[3], r_err);
    ret_id = ((id[3] << 16) | (id[2] << 8) | id[1]);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
    //NOTE:If the screen does not support read id
    //ret_id = LCD_DRV_ID_ST7796S;
#endif
    if (LCD_DRV_ID_ST7796S == ret_id)
    {
        //OSI_LOGI(0x10007dde, "lcd:   is using !");//quectel update
        OSI_LOGI(0, "lcd:   is using !");
    }
    halGoudaClose();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    halPmuSwitchPower(HAL_POWER_LCD, false, false);
#endif
    osiDebugEvent(0x666661d);
    osiDebugEvent(ret_id);
    return ret_id;
}
