#ifndef RDA_MIPI_DSI_H
#define RDA_MIPI_DSI_H
#include "rda_lcdc.h"

struct rda_dsi_cmd
{
    int dtype;
    int delay;
    int dlen;
    const char *payload;
};

enum mipi_mode
{
    DSI_VIDEO_MODE,
    DSI_CMD_MODE,
};

enum dsi_tx_mode
{
    DSI_CMD,
    DSI_PULSE,
    DSI_EVENT,
    DSI_BURST
};

enum dsi_cmd_mode
{
    CMD_LP_MODE,
    CMD_HS_MODE
};

enum mipi_pixel_fmt
{
    DSI_FMT_RGB565 = 0,
    DSI_FMT_RGB666,
    DSI_FMT_RGB666L,
    DSI_FMT_RGB888
};

enum mipi_dsi_clk
{
    DSI_CLK_75MHz = 0,
    DSI_CLK_100MHz,
    DSI_CLK_125MHz,
    DSI_CLK_150MHz,
    DSI_CLK_200MHz,
    DSI_CLK_250MHz,
    DSI_CLK_300MHz,
    DSI_CLK_350MHz,
    DSI_CLK_400MHz
};

#define DSI_HDR_SHORT_PKT (0 << 5)  /*or 0x1*/
#define DSI_HDR_LONG_PKT (0x2 << 5) /*or 0x3*/
#define DSI_HDR_BTA (1 << 0)
#define DSI_HDR_HS (1 << 1)
#define DSI_HDR_CMD_REG (0x3 << 2)

#define DSI_HDR_VC(vc) (((vc)&0x03) << 14)
#define DSI_HDR_DTYPE(dtype) (((dtype)&0x03f) << 8)
#define DSI_HDR_DATA2(data) (((data)&0x0ff) << 24)
#define DSI_HDR_DATA1(data) (((data)&0x0ff) << 16)
#define DSI_HDR_WC(wc) (((wc)&0x0ffff) << 16)

#define DSI_LONG_PKT_DATA0(data) (((data)&0x0ff) << 0)
#define DSI_LONG_PKT_DATA1(data) (((data)&0x0ff) << 8)
#define DSI_LONG_PKT_DATA2(data) (((data)&0x0ff) << 16)
#define DSI_LONG_PKT_DATA3(data) (((data)&0x0ff) << 24)

/* dcs read/write */
#define DTYPE_DCS_SWRITE 0x05 /* short write,with 0 parameter  */
/* mark */
#define DTYPE_DCS_SWRITE_1_PARAM 0x15 /* short write,with 1 parameter  */

#define DTYPE_DCS_READ 0x06
#define DTYPE_MAX_PKTSIZE 0x37
#define DTYPE_DCS_LWRITE 0x39 /* long write */

/* dcs response */
#define DTYPE_ACK_ERR_RESP 0x02
#define DTYPE_EOT_RESP 0x08       /* end of tx */
#define DTYPE_GEN_READ1_RESP 0x11 /* 1 parameter, short */
#define DTYPE_GEN_READ2_RESP 0x12 /* 2 parameter, short */
#define DTYPE_GEN_LREAD_RESP 0x1a
#define DTYPE_DCS_LREAD_RESP 0x1c
#define DTYPE_DCS_READ1_RESP 0x21 /* 1 parameter, short */
#define DTYPE_DCS_READ2_RESP 0x22 /* 2 parameter, short */

void dsi_set_tx_mode(u8 mode);
u8 dsi_get_tx_mode(void);
void dsi_enable(bool on);
void dsi_pll_on(bool enable);
void dsi_op_mode(int mode);
void dsi_swrite(const struct rda_dsi_cmd *cmd);
void dsi_config(const struct lcd_panel_info *lcd);
void set_dsi_ap_pll(const struct rda_dsi_phy_ctrl *pll_phy);
void dsi_irq_clear(void);
void dsi_irq_mask(u32 mask);
u32 dsi_irq_status(void);
int rda_dsi_read_data(u8 *data, u32 len);
void dsi_lwrite(const struct rda_dsi_cmd *cmd);

#endif
