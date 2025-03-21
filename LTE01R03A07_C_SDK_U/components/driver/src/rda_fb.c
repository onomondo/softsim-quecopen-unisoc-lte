#include "hwregs.h"
#include <string.h>

#include "osi_api.h"
#include "osi_log.h"
#include "rda_mipi_io.h"
#include "rda_lcdc.h"
#include "rda_mipi_dsi.h"
#include "hal_chip.h"


#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD    // no use
void *lcd_base;				/* Start of framebuffer memory	*/
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
static struct lcd_panel_info *lcd = NULL;
#else
struct lcd_panel_info *lcd = NULL;
#endif

typedef struct vidinfo
{
    u16 vl_col; /* Number of columns (i.e. 160) */
    u16 vl_row; /* Number of rows (i.e. 100) */
    u16 vl_rot; /* Rotation of Display (0, 1, 2, 3) */
    u8 vl_bpix; /* Bits per pixel, 0 = 1 */
    u16 *cmap;  /* Pointer to the colormap */
    void *priv; /* Pointer to driver-specific data */
} vidinfo_t;

vidinfo_t panel_info = {
    .vl_col = 0,
    .vl_row = 0,
    .vl_bpix = 0,
};

//#ifdef ST7701_MIPI_DEFINE
extern struct lcd_panel_info st7701s_mipi_info;
//#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
extern  struct lcd_panel_info jd9365da_mipi_info;
extern  struct lcd_panel_info sc7705_mipi_info;
//#ifdef ST7365_MIPI_DEFINE
extern struct lcd_panel_info st7365_mipi_info;
//#endif
#endif

/* external function */
extern bool halPmuSwitchPower(uint32_t id, bool enabled, bool lp_enabled);

static struct lcd_panel_info *panel_list[] = {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD_ST7701S
//#ifdef ST7701_MIPI_DEFINE
    &st7701s_mipi_info,
//#endif
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD_JD9365DA
    &jd9365da_mipi_info,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD_SC7705
    &sc7705_mipi_info,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD_ST7365
//#ifdef ST7365_MIPI_DEFINE
    &st7365_mipi_info,
//#endif
#endif
};

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
//根据ID，从panel_list找到对应的屏，并将penal info赋值给lcd
//注意该函数请不要改动
bool quec_mipi_lcd_get_panel_info(bool set_flag, char id_len, char *id)
{    
    int panel_count = 0;
    struct lcd_panel_info * lcd_info = NULL;
  
    if(id_len == 0 || id == NULL)
    {
       return false;
    }

    panel_count = ARRAY_SIZE(panel_list);
    if (!panel_count)
    {
        OSI_LOGI(0, "lcd empty");
		return false;		
    }
    
    for (int i = 0; i < panel_count; i++)
    {
        lcd_info = panel_list[i];
        if ((id_len == lcd_info->id_len) && (0 == memcmp(id, lcd_info->id, lcd_info->id_len)))
        {
            if (set_flag)
            {
                lcd = lcd_info;
            }
            return true;
        }
    }

    return false;
}
#endif

static int lcd_pannel_reset(void)
{
    /*	hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
	_lcdDelayMs(5);
	hwp_gouda->gd_lcd_ctrl &= ~GOUDA_LCD_RESETB;
	_lcdDelayMs(10);
	hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
	_lcdDelayMs(120);


    */
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
#ifdef ST7701_MIPI_DEFINE
    //OSI_LOGI(0, "mipi lcd_pannel_reset  ST7701_MIPI_DEFINE ok \n"); //quectel simple log
    hwp_iomux->pad_keyout_0_cfg_reg = 0x11100000; // output v1.8
    _lcdDelayMs(5);
    hwp_iomux->pad_keyout_0_cfg_reg = 0x11000000; // output v0
    _lcdDelayMs(10);
    hwp_iomux->pad_keyout_0_cfg_reg = 0x11100000; // output v1.8
    _lcdDelayMs(120);
#else
#ifdef ST7365_MIPI_DEFINE
    OSI_LOGI(0, "mipi lcd_pannel_reset  ST7365_MIPI_DEFINE ok \n");
    hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
    _lcdDelayMs(10);
    hwp_gouda->gd_lcd_ctrl &= ~GOUDA_LCD_RESETB;
    _lcdDelayMs(10);
    hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
    _lcdDelayMs(120);

#else
    hwp_iomux->pad_gpio_13_cfg_reg = IOMUX_PAD_GPIO_13_PULL_FRC | IOMUX_PAD_GPIO_13_PULL_UP;
    _lcdDelayMs(5);
    hwp_iomux->pad_gpio_13_cfg_reg &= ~(IOMUX_PAD_GPIO_13_PULL_FRC | IOMUX_PAD_GPIO_13_PULL_UP);
    _lcdDelayMs(10);
    hwp_iomux->pad_gpio_13_cfg_reg |= IOMUX_PAD_GPIO_13_PULL_FRC | IOMUX_PAD_GPIO_13_PULL_UP;
    _lcdDelayMs(120);
#endif

#endif

#else
    hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
	_lcdDelayMs(5);
	hwp_gouda->gd_lcd_ctrl &= ~GOUDA_LCD_RESETB;
	_lcdDelayMs(10);
	hwp_gouda->gd_lcd_ctrl |= GOUDA_LCD_RESETB;
	_lcdDelayMs(120);
#endif
    return 0;
}

static void set_pinmux(void)
{
    /* pinmux for lcd */
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
#ifdef ST7365_MIPI_DEFINE
    OSI_LOGI(0, "set_pinmux  ST7365_MIPI_DEFINE ok \n");
    hwp_iomux->pad_gpio_0_cfg_reg = IOMUX_PAD_GPIO_0_OEN_FRC | IOMUX_PAD_GPIO_0_OEN_REG;
    hwp_iomux->pad_gpio_1_cfg_reg = IOMUX_PAD_GPIO_1_OEN_FRC | IOMUX_PAD_GPIO_1_OEN_REG;
    hwp_iomux->pad_gpio_2_cfg_reg = IOMUX_PAD_GPIO_2_OEN_FRC | IOMUX_PAD_GPIO_2_OEN_REG;
    hwp_iomux->pad_gpio_3_cfg_reg = IOMUX_PAD_GPIO_3_OEN_FRC | IOMUX_PAD_GPIO_3_OEN_REG;
    hwp_iomux->pad_gpio_4_cfg_reg = IOMUX_PAD_GPIO_4_OEN_FRC | IOMUX_PAD_GPIO_4_OEN_REG;
    hwp_iomux->pad_gpio_5_cfg_reg = IOMUX_PAD_GPIO_5_OEN_FRC | IOMUX_PAD_GPIO_5_OEN_REG;
#else
    hwp_iomux->pad_gpio_0_cfg_reg = IOMUX_PAD_GPIO_0_OEN_FRC | IOMUX_PAD_GPIO_0_OEN_REG;
    hwp_iomux->pad_gpio_1_cfg_reg = IOMUX_PAD_GPIO_1_OEN_FRC | IOMUX_PAD_GPIO_1_OEN_REG;
    hwp_iomux->pad_gpio_2_cfg_reg = IOMUX_PAD_GPIO_2_OEN_FRC | IOMUX_PAD_GPIO_2_OEN_REG;
    hwp_iomux->pad_gpio_3_cfg_reg = IOMUX_PAD_GPIO_3_OEN_FRC | IOMUX_PAD_GPIO_3_OEN_REG;
    hwp_iomux->pad_gpio_4_cfg_reg = IOMUX_PAD_GPIO_4_OEN_FRC | IOMUX_PAD_GPIO_4_OEN_REG;
    hwp_iomux->pad_gpio_5_cfg_reg = IOMUX_PAD_GPIO_5_OEN_FRC | IOMUX_PAD_GPIO_5_OEN_REG;

    hwp_iomux->pad_camera_rst_l_cfg_reg = IOMUX_PAD_CAMERA_RST_L_OEN_REG | IOMUX_PAD_CAMERA_RST_L_OEN_FRC;
    hwp_iomux->pad_spi_camera_sck_cfg_reg = IOMUX_PAD_SPI_CAMERA_SCK_OEN_REG | IOMUX_PAD_SPI_CAMERA_SCK_OEN_FRC;
    hwp_iomux->pad_spi_camera_si_1_cfg_reg = IOMUX_PAD_SPI_CAMERA_SI_1_OEN_REG | IOMUX_PAD_SPI_CAMERA_SI_1_OEN_FRC;
    hwp_iomux->pad_spi_camera_si_0_cfg_reg = IOMUX_PAD_SPI_CAMERA_SI_0_OEN_REG | IOMUX_PAD_SPI_CAMERA_SI_0_OEN_FRC;
    hwp_iomux->pad_gpio_22_cfg_reg = IOMUX_PAD_GPIO_22_OEN_REG | IOMUX_PAD_GPIO_22_OEN_FRC;
    hwp_iomux->pad_gpio_23_cfg_reg = IOMUX_PAD_GPIO_23_OEN_REG | IOMUX_PAD_GPIO_23_OEN_FRC;

    hwp_iomux->pad_gpio_13_cfg_reg = IOMUX_PAD_GPIO_13_SEL_FUN_GPIO_13_SEL; // lcd reset pin
    hwp_iomux->pad_gpio_7_cfg_reg = IOMUX_PAD_GPIO_7_SEL_FUN_GPIO_7_SEL | IOMUX_PAD_GPIO_7_PULL_FRC | IOMUX_PAD_GPIO_7_PULL_UP;
#endif

#else  // no use this function
	hwp_iomux->pad_gpio_0_cfg_reg = IOMUX_PAD_GPIO_0_OEN_FRC | IOMUX_PAD_GPIO_0_OEN_REG;
	hwp_iomux->pad_gpio_1_cfg_reg = IOMUX_PAD_GPIO_1_OEN_FRC | IOMUX_PAD_GPIO_1_OEN_REG;
	hwp_iomux->pad_gpio_2_cfg_reg = IOMUX_PAD_GPIO_2_OEN_FRC | IOMUX_PAD_GPIO_2_OEN_REG;
	hwp_iomux->pad_gpio_3_cfg_reg = IOMUX_PAD_GPIO_3_OEN_FRC | IOMUX_PAD_GPIO_3_OEN_REG;
	hwp_iomux->pad_gpio_4_cfg_reg = IOMUX_PAD_GPIO_4_OEN_FRC | IOMUX_PAD_GPIO_4_OEN_REG;
	hwp_iomux->pad_gpio_5_cfg_reg = IOMUX_PAD_GPIO_5_OEN_FRC | IOMUX_PAD_GPIO_5_OEN_REG;
#endif
    OSI_LOGI(0, "set_pinmux  ok \n");
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
static void select_panel(void)
#else
bool select_panel(void)
#endif
{
    int i, panel_count;
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    bool ret;
#else
    bool ret = 0;
#endif

    panel_count = ARRAY_SIZE(panel_list);
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    panel_count = 1;
#endif
    if (!panel_count)
    {
        OSI_LOGI(0, "lcd panel list empty, check your board configuration\n");
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
        return;
#else
		return ret;
#endif		
    }
    for (i = 0; i < panel_count; i++)
    {
        lcd = panel_list[i];
        rda_lcdc_pre_enable(lcd, 1);
        lcd_pannel_reset();
        ret = lcd->match_id();
        rda_lcdc_pre_enable(lcd, 0);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
        // matched ID, directly return
        if(ret)
        {
            return ret;
        }
#endif
    }
    if (!ret)
        OSI_LOGI(0, "lcd panel auto match failed\n");

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    return ret;
#endif
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
#ifdef ST7365_MIPI_DEFINE
extern bool drvBackLightOpen(uint32_t light_level);
#endif
#endif

void lcd_ctrl_init(void *lcdbase, void *lcdbase1)
{

    //	power_on();
    halPmuSwitchPower(HAL_POWER_LCD, true, true);
    halPmuSwitchPower(HAL_POWER_VIBR, true, true);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD 
	halPmuSetPowerLevel(HAL_POWER_LCD, POWER_LEVEL_1800MV);
    halPmuSetPowerLevel(HAL_POWER_VIBR, POWER_LEVEL_2800MV);
#else
//#ifdef ST7365_MIPI_DEFINE
    halPmuSetPowerLevel(HAL_POWER_LCD, 1800);
    halPmuSetPowerLevel(HAL_POWER_VIBR, 2800);
    drvBackLightOpen(251);
//#endif
#endif

    set_pinmux();
    select_panel();

    lcd->lcd_base = (u32)lcdbase;
    lcd->lcd_base1 = (u32)lcdbase1;
    panel_info.vl_col = lcd->width;
    panel_info.vl_row = lcd->height;
    panel_info.vl_bpix = lcd->bpp;
}

void lcd_enable(void)
{
    rda_lcdc_reset();
    if (lcd->lcd_interface == LCD_IF_DSI)
    {
        enable_lcdc_clk(1);
        dsi_config(lcd);
        lcd_pannel_reset();
        lcd->open();
        //lcd_fill_color_bar((unsigned short *)(lcd->lcd_base), lcd->width, lcd->height);
        //uint16_t dsiIntMask = rda_read_dsi_reg(0x4c);
        rda_write_dsi_reg(0x4c, 0xffff);

        set_lcdc_for_video(lcd);
        _lcdDelayMs(50);
        //backlight_on();
        OSI_LOGI(0, "rda lcd enable done\n");
    }
}
