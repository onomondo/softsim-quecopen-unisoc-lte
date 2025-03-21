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

#ifndef _DRV_LCD_V2_H_
#define _DRV_LCD_V2_H_

#include "drv_lcd_defs.h"
#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief opaque data struct for LCD
 */
typedef struct drvLcd drvLcd_t;

/**
 * \brief initialize LCD module
 */
void drvLcdInitV2(void);

/**
 * \brief get LCD driver instance by name
 *
 * \param name      LCD driver name
 * \return
 *      - LCD instance
 *      - NULL on invalid parameter
 */
drvLcd_t *drvLcdGetByname(unsigned name);

/**
 * \brief open LCD instance
 *
 * When it is the first time to open the LCD, it will probe the panel.
 *
 * After open, pixel data can be send to panel. However, unless back light
 * id enabled, picture can't be seen on panel.
 *
 * \param d         LCD driver instance
 * \return
 *      - true on success
 *      - false on invalid parameter, or invalid panel
 */
bool drvLcdOpenV2(drvLcd_t *d);

/**
 * \brief open LCD instance
 *
 * Usually, it is not needed to be called. For power saving, \p drvLcdSleep
 * and \p drvLcdWakeup should be called.
 *
 * \param d         LCD driver instance
 */
void drvLcdCloseV2(drvLcd_t *d);

/**
 * \brief LCD enter sleep mode
 *
 * When LCD is not in sleep, system can't sleep also.
 *
 * \param d         LCD driver instance
 * \return
 *      - true on success
 *      - false on invalid parameter
 */
bool drvLcdSleep(drvLcd_t *d);

/**
 * \brief LCD exit sleep mode
 *
 * \param d         LCD driver instance
 * \return
 *      - true on success
 *      - false on invalid parameter
 */
bool drvLcdWakeup(drvLcd_t *d);

/**
 * \brief turn on or off back light of LCD
 *
 * \param d         LCD instance
 * \param enabled   true to turn on back light, false to turn off
 */
void drvLcdSetBackLightEnable(drvLcd_t *d, bool enabled);

/**
 * \brief get panel information
 *
 * When the panel is invalid, it will return false.
 *
 * After direction transform is changed, the width and height of panel
 * may be swapped.
 *
 * \param d         LCD driver instance
 * \param info      output panel information
 * \return
 *      - true on success
 *      - false on invalid parameter or panel invalid
 */
bool drvLcdGetPanelInfo(drvLcd_t *d, drvLcdPanelInfo_t *info);

/**
 * \brief set screen direction
 *
 * \param d         LCD driver instance
 * \param dir       direction
 */
void drvLcdSetDirection(drvLcd_t *d, drvLcdDirection_t dir);

/**
 * \brief flush LCD display
 *
 * When \p sync is false, this will return immediate after data transfer is
 * started. Otherwise, this will wait data transfer is finished.
 *
 * When previous data transfer is on-going, it will wait previous data
 * transfer finish anyway.
 *
 * Dcache clean will be performed inside.
 *
 * \param d         LCD driver instance
 * \param layers    layers definition
 * \param sync      true to wait data transfer done
 * \return
 *      - true on success
 *      - false on invalid parameter
 */
bool drvLcdFlush(drvLcd_t *lcd, const drvLcdLayers_t *layers, bool sync);

/**
 * \brief fill solid color in screen ROI
 *
 *  When \p screen_roi is NULL, fill full screen.
 *
 * \param d         LCD driver instance
 * \param color     color in RGB565
 * \param screen_roi    screen ROI, NULL for full screen
 * \param sync      true to wait data transfer done
 * \return
 *      - true on success
 *      - false on invalid parameter
 */
bool drvLcdFill(drvLcd_t *lcd, uint16_t color, const drvLcdArea_t *screen_roi, bool sync);

/**
 * \brief blend layers to memory
 *
 * It will reuse the hardware resource, and not bind to LCD. It will wait
 * previous data transfer done.
 *
 * There is only "sync" version, that it, it will always wait data transfer
 * done before return.
 *
 * Dcache clean for layer buffers and dcache invalidate for outptu will be
 * performed inside. Caller should take care alignment of \p buf, to avoid
 * invalidate other varaiables.
 *
 * The output format is fixed to RGB565. \p buf should point to the original
 * pixel of blended layer ROI. The unit of \p stride is pixel, not bytes.
 *
 * \param layers    layers definition
 * \param buf       output buffer
 * \param stride    output buffer stride in pixel
 * \return
 *      - true on success
 *      - false on invalid parameter
 *
 */
bool drvLcdBlend(const drvLcdLayers_t *layers, void *buf, unsigned stride);

/**
 * \brief wait on-going data transfer done
 */
void drvLcdWaitTransferDone(void);

OSI_EXTERN_C_END
#endif
