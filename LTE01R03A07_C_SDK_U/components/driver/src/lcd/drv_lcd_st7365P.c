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
#include "quec_customer_cfg.h"
#include "ql_gpio.h"

#define LCD_DRV_ID_st7365 0x7c89f0

#define LCD_WIDTH 480
#define LCD_HEIGHT 320

static uint16_t st7365_line_mode;
static bool st7365_farmk;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
lcdSpec_t g_lcd_st7365P;
#endif


#define LCM_WR_REG(Addr, Data)                             \
    {                                                      \
        while (halGoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                              \
    }
#define LCD_DataWrite_st7365(Data)                    \
    {                                                 \
        while (halGoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                         \
    }
#define LCD_CtrlWrite_st7365(Cmd)                   \
    {                                               \
        while (halGoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                       \
    }

static void _st7365Close(void);
static uint32_t _st7365ReadId(void);
static void _st7365SleepIn(bool is_sleep);
static void _st7365Init(void);
static void _st7365Invalidate(void);
static void _st7365InvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
);

static void _st7365SetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t right, // end Horizon address
    uint16_t top,   // start Vertical address
    uint16_t bottom // end Vertical address
);
static void _lcdDelayMs(int ms_delay)
{
    osiDelayUS(ms_delay * 1000);
}

OSI_UNUSED static void _st7365SetDirection(lcdDirect_t direct_type)
{

    OSI_LOGI(0, "lcd:    _st7365SetDirection = %d", direct_type);

    switch (direct_type)
    {
    case LCD_DIRECT_NORMAL:
        LCD_CtrlWrite_st7365(0x36);
        LCD_DataWrite_st7365(0x00);
        break;
    case LCD_DIRECT_ROT_90:
        LCD_CtrlWrite_st7365(0x36);
        LCD_DataWrite_st7365(0x60);
        break;
    }

    LCD_CtrlWrite_st7365(0x2c);
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
/******************************************************************************/
static void _st7365SetDisplayWindow(
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
    OSI_LOGI(0, "lcd:st7365SetDisplayWindow L = %d, top = %d, R = %d, bot = %d", left, top, right, bottom);

    LCD_CtrlWrite_st7365(0x2a);                   // set hori start , end (left, right)
    LCD_DataWrite_st7365((newleft >> 8) & 0xff);  // left high 8 b
    LCD_DataWrite_st7365(newleft & 0xff);         // left low 8 b
    LCD_DataWrite_st7365((newright >> 8) & 0xff); // right high 8 b
    LCD_DataWrite_st7365(newright & 0xff);        // right low 8 b

    LCD_CtrlWrite_st7365(0x2b);                    // set vert start , end (top, bot)
    LCD_DataWrite_st7365((newtop >> 8) & 0xff);    // top high 8 b
    LCD_DataWrite_st7365(newtop & 0xff);           // top low 8 b
    LCD_DataWrite_st7365((newbottom >> 8) & 0xff); // bot high 8 b
    LCD_DataWrite_st7365(newbottom & 0xff);        // bot low 8 b
    LCD_CtrlWrite_st7365(0x2c);                    // recover memory write mode
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
/**************************************************************************************/
static void _st7365Init(void)
{
    OSI_LOGI(0, "lcd:    _st480320nit ");

    // ql_gpio_set_level(GPIO_38,LVL_LOW);//拉低使能cs引脚
//************************************************///
	LCD_CtrlWrite_st7365( 0x11);
	_lcdDelayMs(120);

	LCD_CtrlWrite_st7365( 0x36);     // Memory Data Access Control MY,MX~~
//	LCD_DataWrite_st7365(0xC8);
	LCD_DataWrite_st7365(0xE8);

	LCD_CtrlWrite_st7365(0x3A);
	LCD_DataWrite_st7365(0x05);   //LCD_DataWrite_st7365(0x77);   //Write(Parameter , 0x66);

	LCD_CtrlWrite_st7365(0xF0);     // Command Set Control
	LCD_DataWrite_st7365(0xC3);

	LCD_CtrlWrite_st7365(0xF0);
	LCD_DataWrite_st7365( 0x96);

	LCD_CtrlWrite_st7365(0xB4);
	LCD_DataWrite_st7365(0x01);

	LCD_CtrlWrite_st7365(0xB7);
	LCD_DataWrite_st7365(0xC6);

	LCD_CtrlWrite_st7365(0xB9);
	LCD_DataWrite_st7365(0x02);
	LCD_DataWrite_st7365(0x01);

	LCD_CtrlWrite_st7365(0xC0);
	LCD_DataWrite_st7365(0x80);
	LCD_DataWrite_st7365(0x45);

	LCD_CtrlWrite_st7365(0xC1);
	LCD_DataWrite_st7365(0x13);	//LCD_DataWrite_st7365(0x00);

	LCD_CtrlWrite_st7365(0xC2);
	LCD_DataWrite_st7365(0xA7);

	LCD_CtrlWrite_st7365(0xC5);
	LCD_DataWrite_st7365(0x0A);	//LCD_DataWrite_st7365(0x0C);

	LCD_CtrlWrite_st7365(0xE8);
	LCD_DataWrite_st7365(0x40);
	LCD_DataWrite_st7365(0x8A);
	LCD_DataWrite_st7365(0x00);
	LCD_DataWrite_st7365(0x00);
	LCD_DataWrite_st7365(0x29);
	LCD_DataWrite_st7365(0x19);
	LCD_DataWrite_st7365(0xA5);
	LCD_DataWrite_st7365(0x33);

	LCD_CtrlWrite_st7365(0xE0);
	LCD_DataWrite_st7365(0xD0);
	LCD_DataWrite_st7365(0x08);
	LCD_DataWrite_st7365(0x0F);
	LCD_DataWrite_st7365(0x06);
	LCD_DataWrite_st7365(0x06);
	LCD_DataWrite_st7365(0x33);
	LCD_DataWrite_st7365(0x30);
	LCD_DataWrite_st7365(0x33);
	LCD_DataWrite_st7365(0x47);
	LCD_DataWrite_st7365(0x17);
	LCD_DataWrite_st7365(0x13);
	LCD_DataWrite_st7365(0x13);
	LCD_DataWrite_st7365(0x2B);
	LCD_DataWrite_st7365(0x31);

	LCD_CtrlWrite_st7365(0xE1);
	LCD_DataWrite_st7365(0xD0);
	LCD_DataWrite_st7365(0x0A);
	LCD_DataWrite_st7365(0x11);
	LCD_DataWrite_st7365(0x0B);
	LCD_DataWrite_st7365(0x09);
	LCD_DataWrite_st7365(0x07);
	LCD_DataWrite_st7365(0x2F);
	LCD_DataWrite_st7365(0x33);
	LCD_DataWrite_st7365(0x47);
	LCD_DataWrite_st7365(0x38);
	LCD_DataWrite_st7365(0x15);
	LCD_DataWrite_st7365(0x16);
	LCD_DataWrite_st7365(0x2C);
	LCD_DataWrite_st7365(0x32);

	LCD_CtrlWrite_st7365(0xF0);
	LCD_DataWrite_st7365(0x3C);

	LCD_CtrlWrite_st7365(0xF0);
	LCD_DataWrite_st7365(0x69);

	_lcdDelayMs(120);

	LCD_CtrlWrite_st7365(0x29);
	LCD_CtrlWrite_st7365(0x2c);

}

static void _st7365SleepIn(bool is_sleep)
{
    OSI_LOGI(0, "lcd:   _st7365SleepIn, is_sleep = %d", is_sleep);

    if (is_sleep)
    {
        LCD_CtrlWrite_st7365(0x28); //display off
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7365(0x10); // enter sleep mode
    }
    else
    {

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
#if 0
        LCD_CtrlWrite_st7365(0x11); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7365(0x29); // display on
#endif
        _st7365Init();
#else
        LCD_CtrlWrite_st7365(0x11); // sleep out mode
        _lcdDelayMs(120);
        LCD_CtrlWrite_st7365(0x29); // display on
#endif
    }
}

static void _st7365Close(void)
{
    OSI_LOGI(0, "lcd:     in GC9304_Close");

    _st7365SleepIn(true);
}
static void _st7365Invalidate(void)
{
    OSI_LOGI(0, "lcd:     in _st7365Invalidate");

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    _st7365SetDisplayWindow(0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
#else //no use
    _st7365SetDisplayWindow(0x0, 0x0, (g_lcd_st7365P.width) - 1, (g_lcd_st7365P.height) - 1);
#endif
}

void _st7365InvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
)
{

    OSI_LOGI(0, "lcd:  _st7365InvalidateRect");
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    left = (left >= LCD_WIDTH) ? LCD_WIDTH - 1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH - 1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : bottom;
#else //no use
    left = (left >= (g_lcd_st7365P.width)) ? (g_lcd_st7365P.width) - 1 : left;
    right = (right >= (g_lcd_st7365P.width)) ? (g_lcd_st7365P.width) - 1 : right;
    top = (top >= (g_lcd_st7365P.height)) ? (g_lcd_st7365P.height) - 1 : top;
    bottom = (bottom >= (g_lcd_st7365P.height)) ? (g_lcd_st7365P.height) - 1 : bottom;
#endif

    if ((right < left) || (bottom < top))
    {

        OSI_ASSERT(false, "lcd:  _st7365InvalidateRect error"); /*assert verified*/
    }

    _st7365SetDisplayWindow(left, top, right, bottom);
}

static void _st7365RotationInvalidateRect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle)
{

    OSI_LOGI(0, "lcd: gc9305 gc9305RotationInvalidateRect");
    switch (angle)
    {
    case LCD_ANGLE_0:
        _st7365SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_90:
        _st7365SetDisplayWindow(left, top, bottom, right);
        break;
    case LCD_ANGLE_180:
        _st7365SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_270:
        _st7365SetDisplayWindow(left, top, bottom, right);
        break;
    default:
        _st7365SetDisplayWindow(left, top, right, bottom);
        break;
    }
}

static void _st7365EsdCheck(void)
{

    OSI_LOGI(0, "lcd:  st7365sEsdCheck");
}

static const lcdOperations_t st7365sOperations =
    {
        _st7365Init,
        _st7365SleepIn,
        _st7365EsdCheck,
        _st7365SetDisplayWindow,
        _st7365InvalidateRect,
        _st7365Invalidate,
        _st7365Close,
        _st7365RotationInvalidateRect,
        NULL,
        _st7365ReadId,
};

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
const
#endif
lcdSpec_t g_lcd_st7365P =
    {
        LCD_DRV_ID_st7365,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
        (lcdOperations_t *)&st7365sOperations,
        false,
        0x18000,
        10000000, // 500000, //10000000, 10M
};

static uint32_t _st7365ReadId(void)
{
    uint32_t ret_id = 0;
    HAL_ERR_T r_err;
    uint8_t id[4] = {0};
    OSI_LOGI(0, "lcd:st7365ReadId \n");

    //   ql_pin_set_func(20, 0); 
    //ql_gpio_init(GPIO_1, GPIO_OUTPUT, PULL_NONE, LVL_LOW);


#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    halPmuSwitchPower(HAL_POWER_LCD, true, true);
#endif
    _lcdDelayMs(10);
    st7365_farmk = g_lcd_st7365P.is_use_fmark;
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
    st7365_line_mode = g_lcd_st7365P.bus_mode;
#else
    st7365_line_mode = ql_lcd_config.bus_mode;
#endif

    halGoudaReadConfig(st7365_line_mode, g_lcd_st7365P.is_use_fmark, g_lcd_st7365P.fmark_delay);
    _lcdDelayMs(140);
    //hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    _lcdDelayMs(10);

    r_err = halGoudaReadData(0x04, id, 4);
    //hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    OSI_LOGI(0, "lcd:st7365ReadId LCM: 0x%0x, 0x%x, 0x%0x (read return : %d)\n", id[2], id[1], id[3], r_err);
    ret_id = ((id[3] << 16) | (id[2] << 8) | id[1]);

    if (LCD_DRV_ID_st7365 == ret_id)
    {
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
