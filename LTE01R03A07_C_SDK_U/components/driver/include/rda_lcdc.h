#ifndef __RDA_LCDC_H__
#define __RDA_LCDC_H__

#include "hwregs.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
#include <string.h>
#endif


#ifndef FALSE // quectel add
#define FALSE (1==0)
#endif

#ifndef TRUE // quectel add
#define TRUE (1==1)
#endif

typedef unsigned char uint8_t;
typedef uint8_t u8;
typedef unsigned int u32;
typedef unsigned short u16;
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define ALT_MUX_SELECT 0x11a09018
#define RDA_LCDC_BASE 0x8807000

// 2 lane mipi
#define ST7701_MIPI_DEFINE
// 1 lane mipi
#define ST7365_MIPI_DEFINE // quectel need it

#ifndef bool
#define bool char
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
#define QUEC_MIPI_LCD_ID_MAX_LEN        8
#endif

/*LCDC reg offset start*/
#define LCDC_COMMAND            0x0
#define LCDC_STATUS	            0x04
#define LCDC_EOF_IRQ            0x08
#define LCDC_EOF_IRQ_MASK       0xC
#define LCD_CTRL                0x10
#define LCD_TIMING              0x14
#define LCDC_MEM_ADDRESS        0x18
#define LCD_STRIDE_OFFSET       0x1C
#define LCDC_SINGLE_ACCESS      0x20
#define LCDC_SPILCD_CONFIG      0x24
#define LCDC_SPILCD_RD          0x28
#define LCDC_DCT_SHIFT_UV_REG1  0x2C
#define DPI_CONFIG              0x30
#define DPI_FRAME0_ADDR         0x34
#define DPI_FRAME0_CON          0x38
#define DPI_FRAME1_ADRR         0x3C
#define DPI_FRAME1_CON          0x40
#define DPI_FRAME2_ADRR         0x44
#define DPI_FRAME2_CON          0x48
#define DPI_SIZE                0x4C
#define DPI_FIFO_CTRL           0x50
#define DPI_THROT               0x54
#define DPI_POL                 0x58
#define DPI_TIME0               0x5C
#define DPI_TIME1               0x60
#define DPI_TIME2               0x64
#define DPI_TIME3               0x68
#define DPI_STATUS              0x6C
#define LCDC_DITHER_CTRL        0x70
#define LCDC_DITHER_MATRIX0_0   0x74
#define LCDC_DITHER_MATRIX0_1   0x78
#define LCDC_DITHER_MATRIX1     0x7C
#define TECON                   0x80
#define TECON2                  0x84
#define DSI_REG_OFFSET          0x400


/* rgb order */
#define RGB_ORDER_RGB           (0<<3)
#define RGB_ORDER_BGR           (1<<3)

/* pixel format */
#define RGB_PIX_FMT_RGB565      (0<<4)
#define RGB_PIX_FMT_RGB888      (1<<4)
#define RGB_PIX_FMT_XRGB888     (2<<4)
#define RGB_PIX_FMT_RGBX888     (3<<4)

#define MIPI_DSI_ENABLE         (1<<9)
#define MIPI_DSI_DISABLE        (0<<9)

//DPI_CONFIG
#define LCDC_REG_PEND_REQ(n)       (((n)&31)<<21)
#define LCDC_MIPI_CMD_SEL          (1<<20)
#define LCDC_REG_EMPTY_CTRL(n)     (((n)&3)<<14)
#define LCDC_R_RGB_FORMAT(n)       (((n)&3)<<12)
#define LCDC_R_DSI_ENABLE          (1<<9)
#define LCDC_R_OUTOFF_DATA         (1<<8)
#define LCDC_R_OUTOFF_CLK          (1<<7)
#define LCDC_R_OUTOFF_ALL          (1<<6)
#define LCDC_R_PIX_FMT(n)          (((n)&3)<<4)
#define LCDC_R_RGB_ORDER           (1<<3)
#define LCDC_R_FRAME2_ENABLE       (1<<2)
#define LCDC_R_FRAME1_ENABLE       (1<<1)
#define LCDC_R_RGB_ENABLE          (1<<0)
#define MIPI_CMD_SEL_CMD           (1<<20)
#define REG_PEND_REQ(n)            (((n)&0x1F)<<21)

#define DPI_FRAME_LINE_STEP(n)  (((n)&0x1FFF)<<16)
#define DPI_FRAME_VALID         (1<<0)

//dpi size
#define VERTICAL_PIX_NUM(n)     (((n)&0x7FF)<<16)
#define HORIZONTAL_PIX_NUM(n)   (((n)&0x7FF)<<0)

//dct_shift_uv_reg1
#define DCT_SHIFT_UV_ENABLE     (1<<30)
#define LCDC_RGB_WAIT           (1<<30)

/*dpi_fifo_ctrl*/
#define DPI_DATA_FIFO_LOWTHRES(n)   (((n)&0x3FF)<<16)
#define DPI_DATA_FIFO_RST_AUTO      (1<<1)
#define DPI_DATA_FIFO_RST           (1<<0)

#define MIPI_DSI_LANE(n)            (((n)&0x3)<<0)

//gd_eof_irq
#define LCDC_EOF_CAUSE          (1 << 0)
#define LCDC_VSYNC_RISE         (1 << 2)
#define LCDC_VSYNC_FALL         (1 << 3)
#define LCDC_DPI_OVERFLOW       (1 << 4)
#define LCDC_DPI_FRAMEOVER      (1 << 5)
#define LCDC_MIPI_INT           (1 << 6)
#define LCDC_EOF_STATUS         (1 << 16)
#define LCDC_IRQ_CLEAR_ALL      (~0)

//gd_eof_irq_mask
#define LCDC_EOF_MASK           (1<<0)
#define LCDC_VSYNC_RISE_MASK    (1<<1)
#define LCDC_VSYNC_FALL_MASK    (1<<2)
#define LCDC_DPI_OVERFLOW_MASK  (1<<3)
#define LCDC_DPI_FRAMEOVER_MAS  (1<<4)
#define LCDC_MIPI_INT_MASK      (1<<5)
#define LCDC_IRQ_MASK_ALL       (0x3f)

/*dsi irq status*/
#define DSI_TX_END_FLAG         (1 << 0)
#define DSI_RX_END_FLAG         (1 << 1)
#define DSI_CMD_Q_END_FLAG      (1 << 5)

/* lcd mode */
#define LCD_DSI_MODE            (0x1)

//DPI_STATUS
#define LCDC_CURRENT_FRAME_SHIFT   (4)
#define LCDC_CURRENT_FRAME(n)      (((n)&3)<<4)
#define LCDC_FRAME_RUNING          (1<<3)
#define LCDC_FRAME2_OVER           (1<<2)
#define LCDC_FRAME1_OVER           (1<<1)
#define LCDC_FRAME0_OVER           (1<<0)

//DPI_FRAM0_CON
typedef union {
    REG32 v;
    struct {
        REG32 r_frame0_valid : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_frame0_line_step : 13; // [28:16]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DPI_FRAM0_CON_T;

//DPI_FRAM1_CON
typedef union {
    REG32 v;
    struct {
        REG32 r_frame1_valid : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_frame1_line_step : 13; // [28:16]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DPI_FRAM1_CON_T;

enum rda_lcd_interface {
    LCD_IF_DBI = 0, // 8080
    LCD_IF_DPI = 1, // RGB
    LCD_IF_DSI = 2  // MIPI
};

struct dsi_timing {
    u32 off;
    u32 value;
};

struct rda_dsi_phy_ctrl {
    u32 pll[6];
    struct dsi_timing video_timing[13];
    struct dsi_timing cmd_timing[8];
};

struct mipi_panel_info {
    u8 data_lane;
    u8 mipi_mode;
    u8 pixel_format;
    u8 dsi_format;
    u8 trans_mode;
    u8 rgb_order;
    bool bllp_enable;
    u32 h_sync_active;
    u32 h_back_porch;
    u32 h_front_porch;
    u32 v_sync_active;
    u32 v_back_porch;
    u32 v_front_porch;
    u8 frame_rate;
    u8 te_sel;
    u32 dsi_pclk_rate;
    const struct rda_dsi_phy_ctrl *dsi_phy_db;
};

struct lcd_panel_info {
    char name[32];
    u16 width;
    u16 height;
    u16 bpp;
    u16 lcd_interface;
    u32 lcd_base;
    u32 lcd_base1;
    bool use_pwm;
    int reset_gpio;
    struct mipi_panel_info mipi_pinfo;
    void (*open)(void);
    bool (*match_id)(void);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
    //boot里保存这个ID，跑到APP时，直接取对应lcd_panel_info信息
    char id_len;                        //id长度。
    char id[QUEC_MIPI_LCD_ID_MAX_LEN];  //实际ID
#endif
};

//#define LCDC_OUTL(addr, data) do {*(volatile u32*)(addr) = (data);flush_dcache_range((addr), (addr) + sizeof(u32));} while(0)
#define LCDC_OUTL(addr, data) do {*(volatile u32*)(addr) = (data);} while(0)
#define LCDC_INL(addr) *(volatile u32*)(addr)



#define LCDC_OR_WITH_REG(addr, data)	\
            do{ \
                 LCDC_OUTL(addr,data | LCDC_INL(addr));	\
              }while(0);

#define LCDC_AND_WITH_REG(addr, data)	\
            do{ \
                 LCDC_OUTL(addr,data & LCDC_INL(addr));	\
              }while(0);

 void _lcdDelayMs(int ms_delay);
 void rda_write_dsi_reg(u32 offset, u32 value);
 u32 rda_read_dsi_reg(u32 offset);
 void rda_write_dsi_reg1(u32 offset, u32 value);
 u32 rda_read_dsi_reg1(u32 offset);
 void set_lcdc_for_cmd(u32 addr, int group_num);
 void set_lcdc_for_video(const struct lcd_panel_info *lcd);
 void rda_lcdc_set(void);
 int  rda_lcdc_irq_status(void);
 void reset_lcdc_fifo(void);
 void rda_lcdc_reset(void);
 void lcdc_irq_mask_set(u32 mask);
 void lcdc_irq_mask_clear(u32 mask);
 void rda_lcdc_pre_enable(const struct lcd_panel_info *lcd, int on);
 void enable_lcdc_clk(int on);
 void rda_mipi_lcd_color_bar_test(void);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
 bool select_panel(void);
 void lcd_enable(void);
 void dma_chanel0 (void* dest, const void * src, size_t len);
 u32 dma_status(void);
#endif

#endif /*__RDA_LCDC_H*/

