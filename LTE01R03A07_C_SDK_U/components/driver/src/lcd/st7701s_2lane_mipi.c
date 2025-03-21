#include <string.h>
#include "osi_api.h"
#include "osi_log.h"
#include "hwregs.h"
#include "drv_config.h"
#include "hal_gouda.h"
#include "hal_chip.h"
#include "rda_lcdc.h"
#include "rda_mipi_dsi.h"

struct lcd_panel_info st7701s_mipi_info;

/*mipi dsi phy 250MHz*/
static const struct rda_dsi_phy_ctrl st7701s_pll_phy_250mhz = {
    {0x2676, 0x2762, 0x10, 0x2C00, 0x30A, 0x30B},
    {
#if defined(RDA8910M_FPGA)
        {0x6C, 0x84},
        {0x10C, 0x0},
        {0x108, 0x2},
        {0x118, 0x4},
        {0x11C, 0x0},
#else
        {0x6C, 0x84},
        {0x10C, 0x3},
        {0x108, 0x2},
        {0x118, 0x4},
        {0x11C, 0x0},
#endif
        {0x120, 0xC},
        {0x124, 0x2},
        {0x128, 0x3},
        {0x80, 0x15},
        {0x84, 0xC},
        {0x130, 0xC},
        {0x150, 0x12},
        {0x170, 0x872},
    },
    {
        {0x64, 0x3},
        {0x134, 0x3},
        {0x138, 0xB},
        {0x14C, 0xB},
        {0x13C, 0x7},
        {0x114, 0x26},
        {0x170, 0x872},
        {0x140, 0xFF},
    },
};
/*configure 1
static const char st7701s_cmd0[]  = 	{0x11};
static const char st7701s_cmd1[]  = 	{0xFF,0x77,0x01,0x00,0x00,0x10};
static const char st7701s_cmd2[]  = 	{0xC0,0xE9,0x03};
static const char st7701s_cmd3[]  = 	{0xC1,0x08,0x02};
static const char st7701s_cmd4[]  = 	{0xC2,0x31,0x08};
static const char st7701s_cmd5[]  = 	{0xCC,0x10};
static const char st7701s_cmd6[]  = 	{0xB0,0x00,0x0B,0x10,0x0D,0x11,0x06,0x01,0x08,0x08,0x1D,0x04,0x10,0x10,0x27,0x30,0x19};
static const char st7701s_cmd7[]  = 	{0xB1,0x00,0x0B,0x14,0x0C,0x11,0x05,0x03,0x08,0x08,0x20,0x04,0x13,0x10,0x28,0x30,0x19};
static const char st7701s_cmd8[]  = 	{0xFF,0x77,0x01,0x00,0x00,0x11};
static const char st7701s_cmd9[]  = 	{0xB0,0x35};
static const char st7701s_cmd10[]  = 	{0xB1,0x38};
static const char st7701s_cmd11[]  = 	{0xB2,0x02};
static const char st7701s_cmd12[]  = 	{0xB3,0x80};
static const char st7701s_cmd13[]  = 	{0xB5,0x4E};
static const char st7701s_cmd14[]  = 	{0xB7,0x85};
static const char st7701s_cmd15[]  = 	{0xB8,0x20};
static const char st7701s_cmd16[]  = 	{0xB9,0x10};
static const char st7701s_cmd17[]  = 	{0xC1,0x78};
static const char st7701s_cmd18[]  = 	{0xC2,0x78};
static const char st7701s_cmd19[]  = 	{0xD0,0x88};
static const char st7701s_cmd20[]  = 	{0xE0,0x00,0x00,0x02};
static const char st7701s_cmd21[]  = 	{0xE1,0x05,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x20,0x20};
static const char st7701s_cmd22[]  = 	{0xE2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const char st7701s_cmd23[]  = 	{0xE3,0x00,0x00,0x33,0x00};
static const char st7701s_cmd24[]  = 	{0xE4,0x22,0x00};
static const char st7701s_cmd25[]  = 	{0xE5,0x07,0x34,0xA0,0xA0,0x05,0x34,0xA0,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const char st7701s_cmd26[]  = 	{0xE6,0x00,0x00,0x33,0x00};
static const char st7701s_cmd27[]  = 	{0xE7,0x22,0x00};
static const char st7701s_cmd28[]  = 	{0xE8,0x06,0x34,0xA0,0xA0,0x04,0x34,0xA0,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const char st7701s_cmd29[]  = 	{0xEB,0x02,0x00,0x10,0x10,0x00,0x00,0x00};
static const char st7701s_cmd30[]  = 	{0xEC,0x02,0x00};
static const char st7701s_cmd31[]  = 	{0xED,0xAA,0x54,0x0B,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB,0xB0,0x45,0xAA};
static const char st7701s_cmd32[]  = 	{0xFF,0x77,0x01,0x00,0x00,0x00};
static const char st7701s_cmd33[]  = 	{0x36,0x00};
static const char st7701s_cmd34[]  = 	{0x29};
#ifdef ST7701_MIPI_DEFINE
static const char st7701s_cmd35[]  = 	{0x51,0xE0};
static const char st7701s_cmd36[]  = 	{0x53,0x24};
static const char st7701s_cmd37[]  = 	{0x55,0x01};
#endif
*/

//configure 2 id 0x880240
static const char st7701s_cmd0[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13};
static const char st7701s_cmd1[] = {0xEF, 0x08};
static const char st7701s_cmd2[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x10};
static const char st7701s_cmd3[] = {0xC0, 0xE9, 0x03};
static const char st7701s_cmd4[] = {0xC1, 0x11, 0x02};
static const char st7701s_cmd5[] = {0xC2, 0x07, 0x05};
static const char st7701s_cmd6[] = {0xCC, 0x18};
static const char st7701s_cmd7[] = {0xB0, 0x00, 0x0C, 0x17, 0x14, 0x19, 0x0B, 0x0F, 0x0A, 0x09, 0x22, 0x08, 0x15, 0x11, 0x10, 0x11, 0x07};
static const char st7701s_cmd8[] = {0xB1, 0x00, 0x0B, 0x17, 0x11, 0x16, 0x09, 0x10, 0x09, 0x09, 0x25, 0x08, 0x16, 0x13, 0x18, 0x1B, 0x07};
static const char st7701s_cmd9[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x11};
static const char st7701s_cmd10[] = {0xB0, 0x5D};
static const char st7701s_cmd11[] = {0xB1, 0x34};
static const char st7701s_cmd12[] = {0xB2, 0x87};
static const char st7701s_cmd13[] = {0xB3, 0x80};
static const char st7701s_cmd14[] = {0xB5, 0x47};
static const char st7701s_cmd15[] = {0xB7, 0x85};
static const char st7701s_cmd16[] = {0xB8, 0x20};
static const char st7701s_cmd17[] = {0xB9, 0x10};
static const char st7701s_cmd18[] = {0xC0, 0x07}; // Source EQ  to GND
static const char st7701s_cmd19[] = {0xC1, 0x78};
static const char st7701s_cmd20[] = {0xC2, 0x78};
static const char st7701s_cmd21[] = {0xD0, 0x88};

static const char st7701s_cmd22[] = {0xE0, 0x00, 0x00, 0x02};
static const char st7701s_cmd23[] = {0xE1, 0x08, 0x00, 0x0A, 0x00, 0x07, 0x00, 0x09, 0x00, 0x00, 0x33, 0x33};
static const char st7701s_cmd24[] = {0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const char st7701s_cmd25[] = {0xE3, 0x00, 0x00, 0x33, 0x33};
static const char st7701s_cmd26[] = {0xE4, 0x44, 0x44};
static const char st7701s_cmd27[] = {0xE5, 0x0E, 0x60, 0xA0, 0xA0, 0x10, 0x60, 0xA0, 0xA0, 0x0A, 0x60, 0xA0, 0xA0, 0x0C, 0x60, 0xA0, 0xA0};
static const char st7701s_cmd28[] = {0xE6, 0x00, 0x00, 0x33, 0x33};
static const char st7701s_cmd29[] = {0xE7, 0x44, 0x44};
static const char st7701s_cmd30[] = {0xE8, 0x0D, 0x60, 0xA0, 0xA0, 0x0F, 0x60, 0xA0, 0xA0, 0x09, 0x60, 0xA0, 0xA0, 0x0B, 0x60, 0xA0, 0xA0};
static const char st7701s_cmd31[] = {0xEB, 0x02, 0x01, 0xE4, 0xE4, 0x44, 0x00, 0x40};
static const char st7701s_cmd32[] = {0xEC, 0x02, 0x01};
static const char st7701s_cmd33[] = {0xED, 0xAB, 0x89, 0x76, 0x54, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x10, 0x45, 0x67, 0x98, 0xBA};
static const char st7701s_cmd34[] = {0xEF, 0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F};
static const char st7701s_cmd35[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13};
static const char st7701s_cmd36[] = {0xE8, 0x00, 0x0E};
static const char st7701s_cmd37[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x00};
static const char st7701s_cmd38[] = {0x11};

static const char st7701s_cmd39[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13};
static const char st7701s_cmd40[] = {0xE8, 0x00, 0x0C};
static const char st7701s_cmd41[] = {0xE8, 0x00, 0x00};
static const char st7701s_cmd42[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x00};
static const char st7701s_cmd43[] = {0x35, 0x00};
static const char st7701s_cmd44[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x10};
static const char st7701s_cmd45[] = {0xBC, 0x18};
static const char st7701s_cmd46[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x00};
static const char st7701s_cmd47[] = {0x29, 0x00};
// delay 50mss

/*
static const char st7701s_exit_sleep[] = {0x11, 0x00};
static const char st7701s_display_on[] = {0x29, 0x00};
static const char st7701s_enter_sleep[] = {0x10, 0x00};
static const char st7701s_display_off[] = {0x28, 0x00};
*/
static const char st7701s_read_id[] = {0xa1, 0x00};
static const char st7701s_max_pkt[] = {0x5, 0x00};

/* configure 1
static const struct rda_dsi_cmd st7701s_init_cmd_part[] = {
    {DTYPE_DCS_SWRITE, 0, sizeof(st7701s_cmd0), st7701s_cmd0},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd1), st7701s_cmd1},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd2), st7701s_cmd2},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd3), st7701s_cmd3},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd4), st7701s_cmd4},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd5), st7701s_cmd5},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd6), st7701s_cmd6},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd7), st7701s_cmd7},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd8), st7701s_cmd8},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd9), st7701s_cmd9},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd10), st7701s_cmd10},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd11), st7701s_cmd11},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd12), st7701s_cmd12},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd13), st7701s_cmd13},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd14), st7701s_cmd14},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd15), st7701s_cmd15},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd16), st7701s_cmd16},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd17), st7701s_cmd17},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd18), st7701s_cmd18},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd19), st7701s_cmd19},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd20), st7701s_cmd20},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd21), st7701s_cmd21},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd22), st7701s_cmd22},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd23), st7701s_cmd23},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd24), st7701s_cmd24},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd25), st7701s_cmd25},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd26), st7701s_cmd26},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd27), st7701s_cmd27},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd28), st7701s_cmd28},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd29), st7701s_cmd29},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd30), st7701s_cmd30},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd31), st7701s_cmd31},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd32), st7701s_cmd32},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd33), st7701s_cmd33},
#ifdef ST7701_MIPI_DEFINE
    {DTYPE_DCS_SWRITE,0,sizeof(st7701s_cmd35),st7701s_cmd35},
    {DTYPE_DCS_SWRITE,0,sizeof(st7701s_cmd36),st7701s_cmd36},
    {DTYPE_DCS_SWRITE,0,sizeof(st7701s_cmd37),st7701s_cmd37},
#endif
    {DTYPE_DCS_SWRITE, 0, sizeof(st7701s_cmd34), st7701s_cmd34},
};
*/

//configure2  id 0x880240
static const struct rda_dsi_cmd st7701s_init_cmd_part[] = {
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd0), st7701s_cmd0},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd1), st7701s_cmd1},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd2), st7701s_cmd2},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd3), st7701s_cmd3},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd4), st7701s_cmd4},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd5), st7701s_cmd5},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd6), st7701s_cmd6},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd7), st7701s_cmd7},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd8), st7701s_cmd8},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd9), st7701s_cmd9},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd10), st7701s_cmd10},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd11), st7701s_cmd11},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd12), st7701s_cmd12},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd13), st7701s_cmd13},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd14), st7701s_cmd14},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd15), st7701s_cmd15},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd16), st7701s_cmd16},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd17), st7701s_cmd17},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd18), st7701s_cmd18},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd19), st7701s_cmd19},
    {DTYPE_DCS_SWRITE_1_PARAM, 120, sizeof(st7701s_cmd20), st7701s_cmd20},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd21), st7701s_cmd21},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd22), st7701s_cmd22},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd23), st7701s_cmd23},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd24), st7701s_cmd24},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd25), st7701s_cmd25},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd26), st7701s_cmd26},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd27), st7701s_cmd27},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd28), st7701s_cmd28},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd29), st7701s_cmd29},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd30), st7701s_cmd30},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd31), st7701s_cmd31},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd32), st7701s_cmd32},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd33), st7701s_cmd33},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd34), st7701s_cmd34},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd35), st7701s_cmd35},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd36), st7701s_cmd36},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd37), st7701s_cmd37},
    {DTYPE_DCS_SWRITE, 120, sizeof(st7701s_cmd38), st7701s_cmd38},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd39), st7701s_cmd39},
    {DTYPE_DCS_LWRITE, 10, sizeof(st7701s_cmd40), st7701s_cmd40},
    {DTYPE_DCS_LWRITE, 10, sizeof(st7701s_cmd41), st7701s_cmd41},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd42), st7701s_cmd42},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd43), st7701s_cmd43},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd44), st7701s_cmd44},
    {DTYPE_DCS_SWRITE_1_PARAM, 0, sizeof(st7701s_cmd45), st7701s_cmd45},
    {DTYPE_DCS_LWRITE, 0, sizeof(st7701s_cmd46), st7701s_cmd46},
    {DTYPE_DCS_SWRITE, 0, sizeof(st7701s_cmd47), st7701s_cmd47},
};
/*
static const struct rda_dsi_cmd st7701s_exit_sleep_cmd[] = {
	{DTYPE_DCS_SWRITE,120,sizeof(st7701s_exit_sleep),st7701s_exit_sleep},
};

static const struct rda_dsi_cmd st7701s_display_on_cmd[] = {
	{DTYPE_DCS_SWRITE,20,sizeof(st7701s_display_on),st7701s_display_on},
};


static const struct rda_dsi_cmd st7701s_enter_sleep_cmd[] = {
	{DTYPE_DCS_SWRITE,120,sizeof(st7701s_enter_sleep),st7701s_enter_sleep},
};

static const struct rda_dsi_cmd st7701s_display_off_cmd[] = {
	{DTYPE_DCS_SWRITE,20,sizeof(st7701s_display_off),st7701s_display_off},
};
*/
static const struct rda_dsi_cmd st7701s_max_pkt_cmd[] = {
    {DTYPE_MAX_PKTSIZE, 5, sizeof(st7701s_max_pkt), st7701s_max_pkt},
};

static const struct rda_dsi_cmd st7701s_read_id_cmd[] = {
    {DTYPE_DCS_READ, 25, sizeof(st7701s_read_id), st7701s_read_id},
};

void st7701s_mipi_init_lcd(void)
{
    OSI_LOGI(0, "mipi lcd:  st7701s_mipi_init_lcd");
    int cmds_cnt;
    int i;

    cmds_cnt = ARRAY_SIZE(st7701s_init_cmd_part);

    for (i = 0; i < cmds_cnt; i++)
    {

        if (st7701s_init_cmd_part[i].delay)
            _lcdDelayMs(st7701s_init_cmd_part[i].delay);
        if (st7701s_init_cmd_part[i].dtype == DTYPE_DCS_LWRITE)
        {
            dsi_lwrite(&st7701s_init_cmd_part[i]);
        }
        else if (st7701s_init_cmd_part[i].dtype == DTYPE_DCS_SWRITE || st7701s_init_cmd_part[i].dtype == DTYPE_DCS_SWRITE_1_PARAM)
        {
            dsi_swrite(&st7701s_init_cmd_part[i]);
        }
        else
        {
            OSI_LOGI(0, "dtype error \n");
        }
    }

    OSI_LOGI(0, "mipi lcd:  st7701s_mipi_init_lcd end \n");
}

static bool st7701s_mipi_match_id(void)
{
    u8 id[5] = {};

    OSI_LOGI(0, "%s\n", __func__);
    dsi_swrite(st7701s_max_pkt_cmd);
    dsi_swrite(st7701s_read_id_cmd);
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    rda_dsi_read_data(id, 4);
    OSI_LOGI(0, "mipi lcd: get id 0x%02x%02x%02x%02x\n", id[0], id[1], id[2], id[3]);
#else
    rda_dsi_read_data(id, 5);
    OSI_LOGI(0, "mipi lcd id 0x%02x%02x%02x%02x%02x", id[0], id[1], id[2], id[3], id[4]);
#endif

    if (id[0] == 0x88 && id[1] == 0x02)
        return TRUE;

    return FALSE;
}

struct lcd_panel_info st7701s_mipi_info = {
    .width = 480,
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
	.height = 854,
#else
    .height = 960,
#endif
    .bpp = 16,
    .lcd_interface = LCD_IF_DSI,
    .use_pwm = FALSE,
    .mipi_pinfo = {
        .data_lane = 2,
        .mipi_mode = DSI_VIDEO_MODE,
        .pixel_format = RGB_PIX_FMT_RGB565,
        .dsi_format = DSI_FMT_RGB565,
        //        .pixel_format = RGB_PIX_FMT_RGB888,
        //        .dsi_format = DSI_FMT_RGB888,

        .rgb_order = RGB_ORDER_BGR,
        .trans_mode = DSI_BURST,
        .bllp_enable = TRUE,
        .h_sync_active = 50,
        .h_back_porch = 100,
        .h_front_porch = 100,
        .v_sync_active = 16,
        .v_back_porch = 100,
        .v_front_porch = 100,
        .frame_rate = 60,
        .te_sel = FALSE,
// Add by Quectel
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    // Htotal = width + h_sync_active + h_back_porch + h_front_porch
    // Vtotal = height + v_sync_active + v_back_porch + v_front_porch
    // DSI_CLK = (Htotal * Vtatal * FPS * bpp) / data_lane / 2
    // customer can get FPS by setting DSI CLK
    // unit: MHz, range is  1. 75MHz ~ 125MHz, step is 25MHz  2. 150MHz ~ 400MHz, step is 50MHz
    // current value is based on width[480] & height[854], FPS is 55.8Hz
#endif
        .dsi_pclk_rate = DSI_CLK_100MHz,
        .dsi_phy_db = &st7701s_pll_phy_250mhz,
    },
    .open = st7701s_mipi_init_lcd,
    .match_id = st7701s_mipi_match_id,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    .id_len = 2,
    .id = {0x88,0x02},
#endif
};
