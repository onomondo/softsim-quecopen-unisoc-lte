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

#ifndef _HAL_SPI_NAND_H_
#define _HAL_SPI_NAND_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "quec_proj_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief read manufacture/device id
 *
 * \param hwp       hardware register base
 * \return
 *      - 2 bytes manufacture id and device id
 */
uint32_t halSpiNandReadId(uintptr_t hwp);

/**
 * \brief get device status
 *
 * \param hwp       hardware register base
 * \param addr      status address (A0H/B0H/C0H)
 * \return
 *      - device status
 */
uint8_t halSpiNandGetFeature(uintptr_t hwp, uint8_t addr);

/**
 * \brief set device status
 *
 * \param hwp       hardware register base
 * \param addr      status address (A0H/B0H/C0H)
 * \param val       device status
 */
void halSpiNandSetFeature(uintptr_t hwp, uint8_t addr, uint8_t val);

/**
 * \brief write enable
 *
 * \param hwp       hardware register base
 */
void halSpiNandWriteEnable(uintptr_t hwp);

/**
 * \brief write disable
 *
 * \param hwp       hardware register base
 */
void halSpiNandWriteDisable(uintptr_t hwp);

/**
 * \brief read flash array to device cache
 *
 * \param hwp       hardware register base
 * \param row_addr  row address
 */
void halSpiNandPageReadToCache(uintptr_t hwp, uint32_t row_addr);

/**
 * \brief read from device cache
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      memory of read data
 * \param size      read size
 */
void halSpiNandPageRead(uintptr_t hwp, uint32_t col_addr, void *data, unsigned size);

/**
 * \brief read from device cache
 *
 * Command and address are single, data are quad.
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      memory of read data
 * \param size      read size
 */
void halSpiNandPageReadX4(uintptr_t hwp, uint32_t col_addr, void *data, unsigned size);

/**
 * \brief read from device cache
 *
 * Command is single, address and data are quad.
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      memory of read data
 * \param size      read size
 */
void halSpiNandPageReadQuad(uintptr_t hwp, uint32_t col_addr, void *data, unsigned size);

#ifdef CONFIG_QUEC_PROJECT_FEATURE
/**
 * \brief load data to device cache,unused bytes are reset to 0xff by default
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      data to be load
 * \param size      read size
 */
void halSpiNandProgramLoad(uintptr_t hwp, uint32_t col_addr, const void *data, unsigned size);

/**
 * \brief load data to device cache,unused bytes are reset to 0xff by default
 *
 * Command and address are single, data are quad.
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      data to be load
 * \param size      read size
 */
void halSpiNandProgramLoadX4(uintptr_t hwp, uint32_t col_addr, const void *data, unsigned size);
#endif

/**
 * \brief load data to device cache
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      data to be load
 * \param size      read size
 */
void halSpiNandProgramLoadRandom(uintptr_t hwp, uint32_t col_addr, const void *data, unsigned size);

/**
 * \brief load data to device cache
 *
 * Command and address are single, data are quad.
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      data to be load
 * \param size      read size
 */
void halSpiNandProgramLoadRandomX4(uintptr_t hwp, uint32_t col_addr, const void *data, unsigned size);

/**
 * \brief load data to device cache
 *
 * Command is single, address and data are quad.
 *
 * \param hwp       hardware register base
 * \param col_addr  address inside page
 * \param data      data to be load
 * \param size      read size
 */
void halSpiNandProgramLoadRandomQuad(uintptr_t hwp, uint32_t col_addr, const void *data, unsigned size);

/**
 * \brief program execute
 *
 * \param hwp       hardware register base
 * \param row_addr  row address
 */
void halSpiNandProgramExec(uintptr_t hwp, uint32_t row_addr);

/**
 * \brief erase block
 *
 * \param hwp       hardware register base
 * \param row_addr  row address
 */
void halSpiNandBlockErase(uintptr_t hwp, uint32_t row_addr);

/**
 * \brief reset (stop all operations)
 *
 * \param hwp       hardware register base
 */
void halSpiNandReset(uintptr_t hwp);

/**
 * \brief whether operation is finished
 *
 * It will check OIP bit in device status.
 *
 * \param hwp       hardware register base
 */
bool halSpiNandIsOpFinished(uintptr_t hwp);

/**
 * \brief wait operation finished
 *
 * \param hwp       hardware register base
 */
void halSpiNandWaitOpFinished(uintptr_t hwp);

#ifdef __cplusplus
}
#endif
#endif
