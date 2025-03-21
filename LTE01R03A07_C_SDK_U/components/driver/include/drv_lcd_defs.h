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

#ifndef _DRV_LCD_DEFS_H_
#define _DRV_LCD_DEFS_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief maximum overlay count
 */
#define DRV_LCD_OVERLAY_COUNT (3)

/**
 * \brief combine 8bits RGB to RGB565
 *
 * [15:11] red
 * [10:5] green
 * [4:0] blue
 */
#define DRV_LCD_RGB565(r, g, b) ((((uint8_t)(b) >> 3) & 0x1f) | (((uint8_t)(g) << 3) & 0x7e0) | (((uint8_t)(r) << 8) & 0xf800))

/**
 * \brief 8bits R from RGB565
 */
#define DRV_LCD_RGB565_R(c) (((uint16_t)(c)&0xf800) >> 8)

/**
 * \brief 8bits G from RGB565
 */
#define DRV_LCD_RGB565_G(c) (((uint16_t)(c)&0x7e0) >> 3)

/**
 * \brief 8bits B from RGB565
 */
#define DRV_LCD_RGB565_B(c) (((uint16_t)(c)&0x1f) << 3)

/**
 * \brief input buffer color format
 */
typedef enum
{
    /** RGB565 in little endian, [15:11] R, [10:5] G, [4:0] B */
    DRV_LCD_IN_FMT_RGB565,
    /** ARGB8888 in little endian, [31:24] A, [23:16] R, [15:8] G, [7:0] B */
    DRV_LCD_IN_FMT_ARGB8888,
    /** UYVY pixel-packed format */
    DRV_LCD_IN_FMT_UYVY,
    /** YUYV pixel-packed format */
    DRV_LCD_IN_FMT_YUYV,
    /** IYUV planar format */
    DRV_LCD_IN_FMT_IYUV,
} drvLcdInputFormat_t;

/**
 * \brief video layer blending depth
 */
typedef enum
{
    /** at the bottom of all overlays */
    DRV_LCD_VIDEO_BLEND_BOTTOM,
    /** above overlay 0, under overlay 1 */
    DRV_LCD_VIDEO_BLEND_ABOVE_0,
    /** above overlay 1, under overlay 2 */
    DRV_LCD_VIDEO_BLEND_ABOVE_1,
    /** above overlay 2 */
    DRV_LCD_VIDEO_BLEND_ABOVE_2,
    /** at the top of all overlays */
    DRV_LCD_VIDEO_BLEND_TOP = DRV_LCD_VIDEO_BLEND_ABOVE_2,
} drvLcdVideoBlendDepth_t;

/**
 * \brief LCD panel interface type
 */
typedef enum
{
    DRV_LCD_INTF_SPI, ///< SPI
    DRV_LCD_INTF_DSI, ///< MIPI DSI
} drvLcdInterface_t;

/**
 * \brief direction transform
 */
typedef enum
{
    /** normal direction */
    DRV_LCD_DIR_NORMAL,
    /** Y is inverted */
    DRV_LCD_DIR_YINV,
    /** X is inverted */
    DRV_LCD_DIR_XINV,
    /** both Y and X are inverted */
    DRV_LCD_DIR_YINV_XINV,
    /** X-Y are swapped */
    DRV_LCD_DIR_EXCHG,
    /** X-Y are swapped, Y is inverted */
    DRV_LCD_DIR_EXCHG_YINV,
    /** X-Y are swapped, X is inverted */
    DRV_LCD_DIR_EXCHG_XINV,
    /** X-Y are swapped, both X and Y are inverted */
    DRV_LCD_DIR_EXCHG_XINV_YINV,
    /** alias for rotate 0 degree */
    DRV_LCD_DIR_ROTATE_0 = DRV_LCD_DIR_NORMAL,
    /** alias for rotate 90 degree */
    DRV_LCD_DIR_ROTATE_90 = DRV_LCD_DIR_EXCHG_XINV,
    /** alias for rotate 180 degree */
    DRV_LCD_DIR_ROTATE_180 = DRV_LCD_DIR_YINV_XINV,
    /** alias for rotate 270 degree */
    DRV_LCD_DIR_ROTATE_270 = DRV_LCD_DIR_EXCHG_YINV,
} drvLcdDirection_t;

/**
 * \brief combine two transform direction
 *
 * \param first     the 1st transform direction
 * \param second    the 2nd transform direction
 * \return
 *      - the combined transform direction
 */
static inline drvLcdDirection_t drvLcdDirCombine(drvLcdDirection_t first, drvLcdDirection_t second) { return first ^ second; }

/**
 * \brief whether the transform direction swap X and Y
 *
 * For example, rotate 90 and 270 degree will swap X and Y
 *
 * \param dir       the transform direction
 * \return
 *      - true if X and Y is swapped
 */
static inline bool drvLcdDirSwapped(drvLcdDirection_t dir) { return (dir & 4) != 0; }

/**
 * \brief panel information
 */
typedef struct
{
    /** string name */
    const char *name;
    /** panel ID */
    uint32_t dev_id;
    /** virtual panel width, direction transform considered */
    uint16_t width;
    /** virtual panel height, direction transform considered */
    uint16_t height;
    /** interface type */
    drvLcdInterface_t intf;
    /** whether fmark enabled */
    bool fmark_enabled;
    /** data transfer speed, in pixel per second */
    unsigned speed;
    /** frame duration, in us */
    unsigned frame_us;
} drvLcdPanelInfo_t;

/**
 * \brief an area
 *
 * Area with (w == 0 || h == 0) is nul or invalid area.
 */
typedef struct
{
    uint16_t x; ///< starting x
    uint16_t y; ///< starting y
    uint16_t w; ///< width
    uint16_t h; ///< height
} drvLcdArea_t;

/**
 * \brief helper to check whether area are the same
 * \param a     first area
 * \param b     second area
 * \return
 *      - true if matches, false otherwise
 */
static inline bool drvLcdAreaEqual(const drvLcdArea_t *a, const drvLcdArea_t *b)
{
    return (a->x == b->x && a->y == b->y && a->w == b->w && a->h == b->h);
}

/**
 * \brief helper to check whether area shape are the same
 * \param a     first area
 * \param b     second area
 * \return
 *      - true if matches, false otherwise
 */
static inline bool drvLcdAreaShapeEqual(const drvLcdArea_t *a, const drvLcdArea_t *b)
{
    return a->w == b->w && a->h == b->h;
}

/**
 * \brief helper to check whether area is nul
 * \param a     area
 * \return
 *      - true if either width or height is 0
 *      - false if not
 */
static inline bool drvLcdAreaIsNul(const drvLcdArea_t *a) { return (a->w == 0 || a->h == 0); }

/**
 * \brief helper for end X of srea
 * \param a     area
 * \return
 *      - end X
 */
static inline uint16_t drvLcdAreaEndX(const drvLcdArea_t *a) { return a->x + a->w - 1; }

/**
 * \brief helper for end Y of srea
 * \param a     area
 * \return
 *      - end X
 */
static inline uint16_t drvLcdAreaEndY(const drvLcdArea_t *a) { return a->y + a->h - 1; }

/**
 * \brief video layer description
 */
typedef struct
{
    /** buffer at top-left */
    void *buf;
    /** U buffer at top-left for IYUV */
    void *buf_u;
    /** V buffer at top-left for IYUV */
    void *buf_v;
    /** whether this layer is enabled */
    bool enabled;
    /** video blending depth */
    uint8_t depth;
    /** input format */
    uint8_t in_fmt;
    /** blending alpha, 0 for transparent */
    uint8_t alpha;
    /** whether to rotate 90 degree clockwise */
    bool rotation;
    /** whether to enable color key */
    bool key_en;
    /** key color mask */
    uint8_t key_mask;
    /** key color in RGB565 */
    uint16_t key_color;
    /** input buffer stride in pixel */
    uint16_t stride;
    /** input buffer width in pixel */
    uint16_t buf_width;
    /** input buffer height in pixel */
    uint16_t buf_height;
    /** layer ROI */
    drvLcdArea_t out;
} drvLcdVideoLayer_t;

/**
 * \brief overlay definition
 */
typedef struct
{
    /** buffer at top-left */
    void *buf;
    /** whether this layer is enabled */
    bool enabled;
    /** input format */
    uint8_t in_fmt;
    /** blending alpha, 0 for transparent */
    uint8_t alpha;
    /** whether to enable color key */
    bool key_en;
    /** key color mask */
    uint8_t key_mask;
    /** key color in RGB565 */
    uint16_t key_color;
    /** input buffer stride in pixel */
    uint16_t stride;
    /** layer ROI */
    drvLcdArea_t out;
} drvLcdOverlay_t;

/**
 * \brief LCD output layers
 *
 * The configuration will be used for screen update.
 */
typedef struct
{
    /** video layer, NULL for not enabled */
    drvLcdVideoLayer_t *vl;
    /** overlay layers, NULL for not enabled */
    drvLcdOverlay_t *ovl[DRV_LCD_OVERLAY_COUNT];
    /** background color */
    uint16_t bg_color;
    /** blended layers ROI */
    drvLcdArea_t layer_roi;
    /** screen ROI */
    drvLcdArea_t screen_roi;
} drvLcdLayers_t;

/**
 * \brief panel spi interface type
 */
typedef enum
{
    DRV_LCD_SPI_4WIRE,
    DRV_LCD_SPI_3WIRE,
    DRV_LCD_SPI_4WIRE_START_BYTE,
    DRV_LCD_SPI_3WIRE_2LANE,
} drvLcdSpiLineMode_t;

/**
 * \brief panel data format
 */
typedef enum
{
    DRV_LCD_OUT_FMT_8BIT_RGB332 = 0,
    DRV_LCD_OUT_FMT_8BIT_RGB444 = 1,
    DRV_LCD_OUT_FMT_8BIT_RGB565 = 2,
    DRV_LCD_OUT_FMT_16BIT_RGB332 = 4,
    DRV_LCD_OUT_FMT_16BIT_RGB444 = 5,
    DRV_LCD_OUT_FMT_16BIT_RGB565 = 6,
} drvLcdOutputFormat_t;

/**
 * \brief LCD panel operations
 *
 * All functions can't be NULL. User won't check NULL pointer before calling
 * panel operations. Also, caller will take care thread safe.
 */
typedef struct
{
    /**
     * \brief probe the panel
     * \param d         LCD instance
     * \return
     *      - true if the panel matches the driver
     *      - false if not match
     */
    bool (*probe)(void *d);
    /**
     * \brief initialization
     * \param d         LCD instance
     */
    void (*init)(void *d);
    /**
     * \brief prepare data transfer, set direction and ROI
     * \param d         LCD instance
     * \param dir       direction
     * \param roi       region of interest
     */
    void (*blit_prepare)(void *d, drvLcdDirection_t dir, const drvLcdArea_t *roi);
} drvLcdPanelOps_t;

/**
 * \brief panel configuration description
 */
typedef struct
{
    /** panel operation */
    drvLcdPanelOps_t ops;
    /** name string */
    const char *name;
    /** device ID */
    unsigned dev_id;
    /** reset time in us */
    unsigned reset_us;
    /** delay time after reset in us */
    unsigned init_delay_us;
    /** width in normal direction */
    uint16_t width;
    /** height in normal direction */
    uint16_t height;
    /** output format */
    drvLcdOutputFormat_t out_fmt;
    /** panel initial direction */
    drvLcdDirection_t dir;
    /** SPI line mode */
    drvLcdSpiLineMode_t line_mode;
    /** fmark enabled */
    bool fmark_enabled;
    /** fmark delay */
    unsigned fmark_delay;
    /** SPI frequency */
    unsigned freq;
    /** frmae period in us */
    unsigned frame_us;
} drvLcdPanelDesc_t;

/**
 * \brief get panel description from LCD instance
 *
 * This should be called only in panel driver, and implemented in lcd driver.
 *
 * \param d         LCD instance
 * \return
 *      - panel description
 */
const drvLcdPanelDesc_t *drvLcdGetDesc(void *d);

/**
 * \brief write command to panel
 *
 * This should be called only in panel driver, and implemented in lcd driver.
 *
 * \param d         LCD instance
 * \param cmd       command
 */
void drvLcdWriteCmd(void *d, uint16_t cmd);

/**
 * \brief write data to panel
 *
 * This should be called only in panel driver, and implemented in lcd driver.
 *
 * \param d         LCD instance
 * \param data      data
 */
void drvLcdWriteData(void *d, uint16_t data);

/**
 * \brief read data from panel by command
 *
 * This should be called only in panel driver, and implemented in lcd driver.
 *
 * \param d         LCD instance
 * \param cmd       command
 * \param data      data
 * \param len       data length
 */
void drvLcdReadData(void *d, uint16_t cmd, uint8_t *data, unsigned len);

OSI_EXTERN_C_END
#endif
