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

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "hal_chip.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_profile.h"
#include "osi_byte_buf.h"
#include "drv_lcd.h"
#include "quec_proj_config.h"

extern const lcdSpec_t g_lcd_gc9305;
extern const lcdSpec_t g_lcd_st7735s;
extern const lcdSpec_t g_lcd_st7789h2;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
extern const lcdSpec_t g_lcd_st7789v;
extern const lcdSpec_t g_lcd_st7567a;
extern const lcdSpec_t g_lcd_st7796ss;
extern const lcdSpec_t g_lcd_st7365P;
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
static
#endif
uint8_t g_lcd_index = 0xff;

#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
static
#endif
lcdSpec_t *lcd_cfg_tab[] =
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_GC9305
        (lcdSpec_t *)&g_lcd_gc9305,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_ST7735S
        (lcdSpec_t *)&g_lcd_st7735s,
#endif
        (lcdSpec_t *)&g_lcd_st7789h2,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_ST7789V
        (lcdSpec_t *)&g_lcd_st7789v,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_ST7796S
        (lcdSpec_t *)&g_lcd_st7796ss,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_ST7365P
        (lcdSpec_t *)&g_lcd_st7365P,
#endif
};

#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
int8_t ql_lcd_bwscreen_index = -1;

lcdSpec_t *ql_lcd_bwscreen_tab[] =
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD_ST7567A
        (lcdSpec_t *)&g_lcd_st7567a,
#endif
};

uint32_t _drvLcdGetBWCfgCount(void)//quectel
{
    uint32_t count;
    if(sizeof(ql_lcd_bwscreen_tab[0]) == 0) // check table member whether existing
    {
        count = 0;
    }
    else
    {
        count = sizeof(ql_lcd_bwscreen_tab) / sizeof(ql_lcd_bwscreen_tab[0]);
    }
    
    OSI_LOGI(0, "lcd:  _drvLcdGetCfgCount count=%d \n", count);
    
    return count;
}
#endif

uint32_t _drvLcdGetCfgCount(void)
{
    uint32_t count;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
	if(sizeof(lcd_cfg_tab[0]) == 0)
	{
		count = 0;
	}
	else
	{
		count = sizeof(lcd_cfg_tab) / sizeof(lcd_cfg_tab[0]);
	}
#else
	count = sizeof(lcd_cfg_tab) / sizeof(lcd_cfg_tab[0]);
#endif
    OSI_LOGI(0, "lcd:  _drvLcdGetCfgCount count=%d \n", count);

    return count;
}

/*****************************************************************************/
//  Description:    This function is used to get LCD spec information
/*****************************************************************************/
lcdSpec_t *drvLcdGetSpecInfo(void)
{
    uint32_t count = 0;
    uint32_t i = 0;
    uint32_t lcm_dev_id = 0;
    OSI_LOGI(0, "lcd:  drvLcdGetSpecInfo \n");
	
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
    if( ql_lcd_bwscreen_index >= 0 )
    {
        count = _drvLcdGetBWCfgCount();
        if( (count - 1) < ql_lcd_bwscreen_index ) // check index range
        {
            return NULL;
        }

        return ql_lcd_bwscreen_tab[ql_lcd_bwscreen_index];
    }
#endif
    count = _drvLcdGetCfgCount();
    if ((count != 0) && (g_lcd_index == 0xff))
    {
        for (i = 0; i < count; i++)
        {
            if (lcd_cfg_tab[i]->operation->readId)
            {
                lcm_dev_id = lcd_cfg_tab[i]->operation->readId();
                OSI_LOGI(0, "lcd:  drvLcdGetSpecInfo id=0x%x \n", lcm_dev_id);
            }
            if (lcm_dev_id == lcd_cfg_tab[i]->dev_id)
            {
                g_lcd_index = i;
                OSI_LOGI(0, "lcd:  drvLcdGetSpecInfo g_lcd_index =0x%x \n", g_lcd_index);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LCD
                quec_lcd_set_offset(g_lcd_index);
                lcd_cfg_tab[g_lcd_index]->bus_mode = ql_lcd_config.bus_mode;
                if( ql_lcd_config.freq != 0 )
                {
                    lcd_cfg_tab[g_lcd_index]->freq = ql_lcd_config.freq;
                }
#endif
                return lcd_cfg_tab[g_lcd_index];
            }
        }
    }
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    else if ((count != 0) && (g_lcd_index != 0xff))
#else
	else if (g_lcd_index != 0xff)
#endif
    {
        return lcd_cfg_tab[g_lcd_index];
    }
    OSI_LOGI(0, "lcd:  drvLcdGetSpecInfo null ! \n");
    return NULL;
}

#ifdef __cplusplus
}

#endif // End of lcm_cfg_info.c
