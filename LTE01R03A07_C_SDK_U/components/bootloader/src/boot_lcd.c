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

#include "boot_lcd.h"
#include "lcd_panel_desc.h"
#include "drv_names.h"
#include "hal_chip.h"
#include "hwregs.h"
#include "osi_api.h"
#include <string.h>

#define LCD_BACK_LIGHT_POWER (HAL_POWER_BACK_LIGHT)
#define LCD_CS (GOUDA_DESTINATION_V_LCD_CS_0)
#define LCD_CS0_POLARITY (0)
#define LCD_CS1_POLARITY (0)
#define LCD_LOW_FREQ (800000)

#define LCD_STATE_CLOSED (0)
#define LCD_STATE_PROBE_FAILD (1)
#define LCD_STATE_OPENED (2)
#define LCD_STATE_SLEEP (3)

struct bootLcd
{
    uint8_t state;                           // state
    drvLcdDirection_t app_dir;               // application direction
    const drvLcdPanelDesc_t *desc;           // panel description
    REG_GOUDA_GD_LCD_CTRL_T lcd_ctrl;        // GOUDA lcd_ctrl
    REG_GOUDA_GD_SPILCD_CONFIG_T spi_config; // GOUDA spi_config
    unsigned mem_address;                    // GOUDA mem_address
};

static bootLcd_t gLcd1;

static void prvSetPowerEnable(bool enabled)
{
    halPmuSwitchPower(HAL_POWER_LCD, enabled, false);
    if (enabled)
        osiDelayUS(200);
}

static void prvWaitGouda(void)
{
    OSI_LOOP_WAIT((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) == 0);
}

static void prvGoudaInit(void)
{
    // disable auto gating
    REG_SYS_CTRL_CLK_SYS_AHB_MODE_T clk_sys_ahb_mode = {hwp_sysCtrl->clk_sys_ahb_mode};
    clk_sys_ahb_mode.b.mode_sys_ahb_clk_id_gouda = 1; // manual
    hwp_sysCtrl->clk_sys_ahb_mode = clk_sys_ahb_mode.v;

    REG_SYS_CTRL_CLK_AP_APB_MODE_T clk_ap_apb_mode = {hwp_sysCtrl->clk_ap_apb_mode};
    clk_ap_apb_mode.b.mode_ap_apb_clk_id_gouda = 1; // manual
    hwp_sysCtrl->clk_ap_apb_mode = clk_ap_apb_mode.v;
}

static void prvGoudaDeinit(void)
{
    // enable auto gating
    REG_SYS_CTRL_CLK_SYS_AHB_MODE_T clk_sys_ahb_mode = {hwp_sysCtrl->clk_sys_ahb_mode};
    clk_sys_ahb_mode.b.mode_sys_ahb_clk_id_gouda = 0; // auto
    hwp_sysCtrl->clk_sys_ahb_mode = clk_sys_ahb_mode.v;

    REG_SYS_CTRL_CLK_AP_APB_MODE_T clk_ap_apb_mode = {hwp_sysCtrl->clk_ap_apb_mode};
    clk_ap_apb_mode.b.mode_ap_apb_clk_id_gouda = 0; // auto
    hwp_sysCtrl->clk_ap_apb_mode = clk_ap_apb_mode.v;

    hwp_gouda->gd_spilcd_config = 0;
    hwp_gouda->gd_lcd_ctrl = 0;
}

static void prvPanelConfig(bootLcd_t *d)
{
    d->lcd_ctrl.b.lcd_resetb = 0;
    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;

    osiDelayUS(d->desc->reset_us);

    d->lcd_ctrl.b.lcd_resetb = 1;
    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;

    hwp_gouda->gd_spilcd_config = d->spi_config.v;
    hwp_gouda->gd_lcd_mem_address = d->mem_address;

    hwp_gouda->tecon = 0;
    if (d->desc->fmark_enabled)
    {
        hwp_gouda->tecon = GOUDA_TE_EN | GOUDA_TE_EDGE_SEL;
        hwp_gouda->tecon2 = GOUDA_TE_COUNT1(d->desc->fmark_delay);
    }

    osiDelayUS(d->desc->init_delay_us);
}

bootLcd_t *bootLcdGetByname(unsigned name)
{
    if (name == DRV_NAME_LCD1)
        return &gLcd1;

    return NULL;
}

bool bootLcdGetPanelInfo(bootLcd_t *d, drvLcdPanelInfo_t *info)
{
    if (info == NULL)
        return false;
    if (d->state != LCD_STATE_OPENED)
        return false;

    unsigned spi_freq = CONFIG_DEFAULT_SYSAHB_FREQ / d->spi_config.b.spi_clk_divider;
    unsigned spi_bps = (d->desc->line_mode == DRV_LCD_SPI_3WIRE_2LANE) ? (spi_freq * 2) : spi_freq;
    info->name = d->desc->name;
    info->dev_id = d->desc->dev_id;
    info->width = d->desc->width;
    info->height = d->desc->height;
    info->intf = DRV_LCD_INTF_SPI;
    info->fmark_enabled = d->desc->fmark_enabled;
    info->speed = spi_bps / sizeof(uint16_t);
    info->frame_us = d->desc->frame_us;
    if (drvLcdDirSwapped(drvLcdDirCombine(d->app_dir, d->desc->dir)))
        OSI_SWAP(uint16_t, info->width, info->height);

    return true;
}

static void prvLcdSetDesc(bootLcd_t *d, const drvLcdPanelDesc_t *desc)
{
    d->desc = desc;

    d->lcd_ctrl.v = 0;
    d->lcd_ctrl.b.destination = LCD_CS;
    d->lcd_ctrl.b.output_format = desc->out_fmt;
    d->lcd_ctrl.b.high_byte = false;
    d->lcd_ctrl.b.cs0_polarity = LCD_CS0_POLARITY;
    d->lcd_ctrl.b.cs1_polarity = LCD_CS1_POLARITY;
    d->lcd_ctrl.b.rs_polarity = 0;
    d->lcd_ctrl.b.wr_polarity = 0;
    d->lcd_ctrl.b.rd_polarity = 0;
    d->lcd_ctrl.b.lcd_resetb = 0; // release reset

    d->spi_config.v = 0;
    d->spi_config.b.spi_lcd_select = 1;
    d->spi_config.b.spi_clk_divider = OSI_DIV_ROUND_UP(CONFIG_DEFAULT_SYSAHB_FREQ, desc->freq);
    d->spi_config.b.spi_dummy_cycle = 0;
    d->spi_config.b.spi_line = desc->line_mode;
    d->spi_config.b.spi_rx_byte = 0;
    d->spi_config.b.spi_rw = 0;

    prvPanelConfig(d);
}

bool bootLcdOpen(bootLcd_t *d)
{
    if (d->state == LCD_STATE_OPENED)
        return true;

    if (d->state == LCD_STATE_PROBE_FAILD)
        return false;

    prvGoudaInit();
    prvSetPowerEnable(true);

    d->state = LCD_STATE_PROBE_FAILD;
    for (unsigned n = 0;; n++)
    {
        if (gLcdPanels[n] == NULL)
            break;

        prvLcdSetDesc(d, gLcdPanels[n]);
        if (d->desc->ops.probe(d))
        {
            d->desc->ops.init(d);
            d->state = LCD_STATE_OPENED;
            break;
        }
    }

    if (d->state != LCD_STATE_OPENED)
    {
        prvGoudaDeinit();
        prvSetPowerEnable(false);
        return false;
    }

    return true;
}

void bootLcdClose(bootLcd_t *d)
{
    if (d->state == LCD_STATE_OPENED)
    {
        prvGoudaDeinit();
        prvSetPowerEnable(false);
        memset(d, 0, sizeof(bootLcd_t));
    }
}

void bootLcdSetDirection(bootLcd_t *d, drvLcdDirection_t dir)
{
    d->app_dir = dir;
}

void bootLcdSetBackLightEnable(bootLcd_t *d, bool enabled)
{
    halPmuSwitchPower(LCD_BACK_LIGHT_POWER, enabled, false);
}

void bootLcdWaitTransferDone(void)
{
    prvWaitGouda();
}

static int prvToVlInputFmt(uint8_t in_fmt)
{
    if (in_fmt == DRV_LCD_IN_FMT_RGB565)
        return 0;
    if (in_fmt == DRV_LCD_IN_FMT_UYVY)
        return 1;
    if (in_fmt == DRV_LCD_IN_FMT_YUYV)
        return 2;
    if (in_fmt == DRV_LCD_IN_FMT_IYUV)
        return 3;
    return -1;
}

static int prvToFrameInputFmt(uint8_t in_fmt)
{
    if (in_fmt == DRV_LCD_IN_FMT_RGB565)
        return 0;
    if (in_fmt == DRV_LCD_IN_FMT_ARGB8888)
        return 1;
    return -1;
}

static bool prvSetVideoLayer(const drvLcdVideoLayer_t *vl)
{
    if (vl == NULL || !vl->enabled)
    {
        hwp_gouda->gd_vl_input_fmt = 0;
        return true;
    }

    if (drvLcdAreaIsNul(&vl->out))
        return false;
    if (vl->buf_width == 0 || vl->buf_height == 0)
        return false;
    if (vl->stride < vl->buf_width)
        return false;

    int in_fmt = prvToVlInputFmt(vl->in_fmt);
    if (in_fmt < 0)
        return false;

    unsigned bpp = (vl->in_fmt == DRV_LCD_IN_FMT_IYUV) ? 1 : 2; // byte per pixel
    unsigned y_bl_offset = (vl->buf_height - 1) * vl->stride * bpp;
    unsigned uv_bl_offset = (vl->buf_height / 2 - 1) * vl->stride / 2;
    unsigned bwidth = vl->rotation ? vl->buf_height : vl->buf_width;
    unsigned bheight = vl->rotation ? vl->buf_width : vl->buf_height;
    unsigned xpitch = (bwidth << 8) / vl->out.w;
    unsigned ypitch = (bheight << 8) / vl->out.h;
    unsigned by = (uint32_t)vl->buf + (vl->rotation ? y_bl_offset : 0);
    unsigned bu = (vl->buf_u == NULL) ? 0 : (uint32_t)vl->buf_u + (vl->rotation ? uv_bl_offset : 0);
    unsigned bv = (vl->buf_v == NULL) ? 0 : (uint32_t)vl->buf_v + (vl->rotation ? uv_bl_offset : 0);

    if (vl->in_fmt == DRV_LCD_IN_FMT_IYUV)
    {
        if (!OSI_IS_ALIGNED(vl->buf, 16) || !OSI_IS_ALIGNED(vl->buf_u, 16) ||
            !OSI_IS_ALIGNED(vl->buf_v, 16) || !OSI_IS_ALIGNED(vl->stride, 16))
            return false;
    }
    else if (vl->in_fmt == DRV_LCD_IN_FMT_UYVY || vl->in_fmt == DRV_LCD_IN_FMT_YUYV)
    {
        if (!OSI_IS_ALIGNED(vl->buf, 4))
            return false;
    }
    else if (vl->in_fmt == DRV_LCD_IN_FMT_RGB565)
    {
        if (!OSI_IS_ALIGNED(vl->buf, 2))
            return false;
    }

    if (xpitch > (0x100 * 4) || xpitch < (0x100 / 4))
        return false;
    if (ypitch > (0x100 * 4) || ypitch < (0x100 / 4))
        return false;

    hwp_gouda->gd_vl_y_src = by;
    hwp_gouda->gd_vl_u_src = bu;
    hwp_gouda->gd_vl_v_src = bv;
    hwp_gouda->gd_vl_extents =
        GOUDA_MAX_COL(bwidth - 1) |
        GOUDA_MAX_LINE(bheight - 1);
    hwp_gouda->gd_vl_resc_ratio =
        GOUDA_XPITCH(xpitch) |
        GOUDA_YPITCH(ypitch) |
        GOUDA_YPITCH_SCALE_ENABLE;
    hwp_gouda->gd_vl_tl_ppos =
        GOUDA_X0(vl->out.x) |
        GOUDA_Y0(vl->out.y);
    hwp_gouda->gd_vl_br_ppos =
        GOUDA_X1(drvLcdAreaEndX(&vl->out)) |
        GOUDA_Y1(drvLcdAreaEndY(&vl->out));
    hwp_gouda->gd_vl_blend_opt =
        GOUDA_DEPTH(vl->depth) |
        GOUDA_ROTATION(vl->rotation) |
        GOUDA_ALPHA(vl->alpha) |
        GOUDA_CHROMA_KEY_COLOR(vl->key_color) |
        (vl->key_en ? GOUDA_CHROMA_KEY_ENABLE : 0) |
        GOUDA_CHROMA_KEY_MASK(vl->key_mask);
    hwp_gouda->gd_vl_input_fmt =
        GOUDA_FORMAT(in_fmt) |
        GOUDA_STRIDE(vl->stride * bpp) |
        GOUDA_ACTIVE;
    return true;
}

static bool prvSetOverlay(unsigned n, const drvLcdOverlay_t *ovl)
{
    if (ovl == NULL || !ovl->enabled)
    {
        hwp_gouda->overlay_layer[n].gd_ol_input_fmt = 0;
        return true;
    }

    if (drvLcdAreaIsNul(&ovl->out))
        return false;
    if (!OSI_IS_ALIGNED(ovl->buf, 2))
        return false;

    int in_fmt = prvToFrameInputFmt(ovl->in_fmt);
    if (in_fmt < 0)
        return false;

    hwp_gouda->overlay_layer[n].gd_ol_rgb_src = (uint32_t)ovl->buf;
    hwp_gouda->overlay_layer[n].gd_ol_tl_ppos =
        GOUDA_X0(ovl->out.x) |
        GOUDA_Y0(ovl->out.y);
    hwp_gouda->overlay_layer[n].gd_ol_br_ppos =
        GOUDA_X1(drvLcdAreaEndX(&ovl->out)) |
        GOUDA_Y1(drvLcdAreaEndY(&ovl->out));
    hwp_gouda->overlay_layer[n].gd_ol_blend_opt =
        GOUDA_ALPHA(ovl->alpha) |
        GOUDA_CHROMA_KEY_COLOR(ovl->key_color) |
        (ovl->key_en ? GOUDA_CHROMA_KEY_ENABLE : 0) |
        GOUDA_CHROMA_KEY_MASK(ovl->key_mask);
    hwp_gouda->overlay_layer[n].gd_ol_input_fmt =
        GOUDA_FORMAT(in_fmt) |
        GOUDA_STRIDE(ovl->stride) |
        GOUDA_PREFETCH |
        GOUDA_ACTIVE;
    return true;
}

bool bootLcdFlush(bootLcd_t *d, const drvLcdLayers_t *cfg, bool sync)
{
    if (d->state != LCD_STATE_OPENED)
        return false;

    if (drvLcdAreaIsNul(&cfg->layer_roi))
        return false;
    if (!drvLcdAreaShapeEqual(&cfg->layer_roi, &cfg->screen_roi))
        return false;

    prvWaitGouda();

    if (!prvSetVideoLayer(cfg->vl))
        return false;

    for (unsigned n = 0; n < DRV_LCD_OVERLAY_COUNT; n++)
    {
        if (!prvSetOverlay(n, cfg->ovl[n]))
            return false;
    }

    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;
    hwp_gouda->gd_lcd_mem_address = d->mem_address;
    hwp_gouda->gd_spilcd_config = d->spi_config.v;
    hwp_gouda->gd_roi_tl_ppos =
        GOUDA_X0(cfg->layer_roi.x) |
        GOUDA_Y0(cfg->layer_roi.y);
    hwp_gouda->gd_roi_br_ppos =
        GOUDA_X0(drvLcdAreaEndX(&cfg->layer_roi)) |
        GOUDA_Y0(drvLcdAreaEndY(&cfg->layer_roi));
    hwp_gouda->gd_roi_bg_color = cfg->bg_color;

    drvLcdDirection_t dir = drvLcdDirCombine(d->app_dir, d->desc->dir);
    d->desc->ops.blit_prepare(d, dir, &cfg->screen_roi);

    osiDCacheCleanAll(); // it is simpler than clean needed
    hwp_gouda->gd_eof_irq_mask = GOUDA_EOF_MASK;
    hwp_gouda->gd_command = GOUDA_START;

    if (sync)
        prvWaitGouda();

    return true;
}

bool bootLcdFill(bootLcd_t *d, uint16_t color, const drvLcdArea_t *screen_roi, bool sync)
{
    if (d->state != LCD_STATE_OPENED)
        return false;

    drvLcdArea_t roi;
    if (screen_roi != NULL)
    {
        roi = *screen_roi;
    }
    else
    {
        roi.x = 0;
        roi.y = 0;
        roi.w = d->desc->width;
        roi.h = d->desc->height;
        if (drvLcdDirSwapped(drvLcdDirCombine(d->app_dir, d->desc->dir)))
            OSI_SWAP(uint16_t, roi.w, roi.h);
    }

    drvLcdLayers_t layers = {
        .bg_color = color,
        .layer_roi = roi,
        .screen_roi = roi,
    };
    // dcache clean inside is not needed
    return bootLcdFlush(d, &layers, sync);
}

const drvLcdPanelDesc_t *drvLcdGetDesc(void *inst)
{
    bootLcd_t *d = (bootLcd_t *)inst;
    return d->desc;
}

void drvLcdWriteCmd(void *inst, uint16_t cmd)
{
    bootLcd_t *d = (bootLcd_t *)inst;
    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);
    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;
    hwp_gouda->gd_lcd_mem_address = d->mem_address;
    hwp_gouda->gd_lcd_single_access = GOUDA_START_WRITE | GOUDA_LCD_DATA(cmd);
    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);
}

void drvLcdWriteData(void *inst, uint16_t data)
{
    bootLcd_t *d = (bootLcd_t *)inst;
    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);
    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;
    hwp_gouda->gd_lcd_mem_address = d->mem_address;
    hwp_gouda->gd_lcd_single_access = GOUDA_START_WRITE | GOUDA_TYPE | GOUDA_LCD_DATA(data);
    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);
}

void drvLcdReadData(void *inst, uint16_t cmd, uint8_t *data, unsigned len)
{
    bootLcd_t *d = (bootLcd_t *)inst;
    bool spilcd = (d->spi_config.v & GOUDA_SPI_LCD_SELECT);
    if (spilcd && (len > GD_MAX_SLCD_READ_LEN))
        osiPanic();

    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);
    hwp_gouda->gd_lcd_ctrl = d->lcd_ctrl.v;
    hwp_gouda->gd_lcd_mem_address = d->mem_address;
    if (spilcd)
    {
        REG_GOUDA_GD_SPILCD_CONFIG_T spi_config = {d->spi_config.v};
        spi_config.b.spi_rx_byte = len;
        spi_config.b.spi_rw = GOUDA_SPI_RW_V_READ;
        spi_config.b.spi_dummy_cycle = 1;
        spi_config.b.spi_clk_divider = CONFIG_DEFAULT_SYSAHB_FREQ / LCD_LOW_FREQ;
        if (spi_config.b.spi_line == GOUDA_SPI_LINE_V_3_TWO_LANE)
            spi_config.b.spi_line = GOUDA_SPI_LINE_V_3;
        hwp_gouda->gd_spilcd_config = spi_config.v;
    }

    hwp_gouda->gd_lcd_single_access = GOUDA_START_WRITE | GOUDA_LCD_DATA(cmd);
    OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);

    if (spilcd)
    {
        unsigned rd = hwp_gouda->gd_spilcd_rd;
        for (unsigned n = 0; n < len; n++)
            data[n] = (rd >> (8 * n)) & 0xff;

        hwp_gouda->gd_spilcd_config = d->spi_config.v;
    }
    else
    {
        for (unsigned n = 0; n < len; n += 2)
        {
            hwp_gouda->gd_lcd_single_access = GOUDA_START_READ | GOUDA_TYPE;
            OSI_LOOP_WAIT((hwp_gouda->gd_status & GOUDA_LCD_BUSY) == 0);

            unsigned rd = hwp_gouda->gd_lcd_single_access;
            data[n] = rd & 0xff;
            data[n + 1] = (rd >> 8) & 0xff;
        }
    }
}
