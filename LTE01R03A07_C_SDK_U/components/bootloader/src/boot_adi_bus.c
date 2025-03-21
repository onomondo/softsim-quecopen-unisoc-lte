/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "boot_adi_bus.h"
#include "boot_platform.h"
#include "hwregs.h"
#include "osi_api.h"
#include <stdarg.h>
#include "quec_proj_config.h"

#define VALUE_MASK (0xffff)
#define ADDRESS_MASK (0xfff)
#define ADDRESS_START (REG_RDA2720M_GLOBAL_BASE & ~0xfff)

//代码剪裁
#define QUEC_BOOT_ADIBUS_CODE_CUT 0

void bootAdiBusInit(void)
{
    REG_ADI_MST_ADI_CFG0_T adi_cfg0 = {
        .b = {
            .rf_gssi_frame_len = 29,
            .rf_gssi_cmd_len = 13,
            .rf_gssi_data_len = 16,
            .rf_gssi_wr_pos = 16,
            .rf_gssi_wr_pol = 0,
            .rf_gssi_sync_sel = 0,
            .rf_gssi_sync_mode = 1,
            .rf_gssi_sync = 0,
            .rf_gssi_sck_rev = 0,
            .rf_gssi_oe_cfg = 0,
            .rf_gssi_ie_cfg = 0,
            .rf_gssi_fast_mode = 1,
            .rf_gssi_dummy_clk_en = 1,
            .rf_gssi_sck_all_on = 1,
            .rf_gssi_wr_disable = 0,
        }};
    REG_ADI_MST_ADI_CFG1_T adi_cfg1 = {
        .b = {
            .rf_gssi_ng_tx = 1,
            .rf_gssi_ng_rx = 1,
            .rf_gssi_clk_div = 10,
            .rf_gssi_sync_head_len = 0,
            .rf_gssi_sync_end_len = 0,
            .rf_gssi_dummy_len = 2,
            .rf_gssi_sample_delay = 0,
            .rf_gssi_scc_len = 0,
            .rf_gssi_wbp_len = 0,
            .rf_gssi_rbp_len = 0,
            .rf_gssi_strtbit_mode = 1,
        }};

    hwp_adiMst->adi_cfg0 = adi_cfg0.v;
    hwp_adiMst->adi_cfg1 = adi_cfg1.v;
}

#if QUEC_BOOT_ADIBUS_CODE_CUT
void boot_reg_wait_field_eqz(volatile uint32_t *reg,uint8_t ofs,uint8_t var)
{
    while(((*reg >> ofs)& var)!=0);
}
#endif

uint32_t bootAdiBusRead(volatile uint32_t *reg)
{
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_ADI_MST_ARM_CMD_STATUS_T arm_cmd_status;
    REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif
    hwp_adiMst->arm_rd_cmd = (uint32_t)reg;

#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_ADI_MST_ARM_RD_DATA_T arm_rd_data;
    REG_WAIT_FIELD_EQZ(arm_rd_data, hwp_adiMst->arm_rd_data, arm_rd_cmd_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_rd_data,31,0x01);
    REG_ADI_MST_ARM_RD_DATA_T arm_rd_data = {hwp_adiMst->arm_rd_data};
#endif

    if (arm_rd_data.b.arm_rd_addr != (((uint32_t)reg & 0xffff) >> 2))
        osiPanic();

    return arm_rd_data.b.arm_rd_cmd;
}

void bootAdiBusWrite(volatile uint32_t *reg, uint32_t value)
{
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_ADI_MST_ARM_CMD_STATUS_T arm_cmd_status;
    REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif

    *reg = value;
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_rd_data,31,0x01);
#endif
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
uint32_t quec_bootAdiBusBatchChange(volatile uint32_t *reg, uint32_t write_mask, uint32_t write_value)
{
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_ADI_MST_ARM_CMD_STATUS_T arm_cmd_status;
#endif
    if ((write_mask & VALUE_MASK) != VALUE_MASK)
    {
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
        REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
        boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif
        hwp_adiMst->arm_rd_cmd = (uint32_t)reg;

#if QUEC_BOOT_ADIBUS_CODE_CUT==0
        REG_ADI_MST_ARM_RD_DATA_T arm_rd_data;
        REG_WAIT_FIELD_EQZ(arm_rd_data, hwp_adiMst->arm_rd_data, arm_rd_cmd_busy);
#else
        boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_rd_data,31,0x01);
        REG_ADI_MST_ARM_RD_DATA_T arm_rd_data = {hwp_adiMst->arm_rd_data};
#endif

        if (arm_rd_data.b.arm_rd_addr != (((uint32_t)reg & 0xffff) >> 2))
            osiPanic();

        uint32_t value = arm_rd_data.b.arm_rd_cmd;
        write_value = (value & ~write_mask) | (write_value & write_mask);
    }
#if QUEC_BOOT_ADIBUS_CODE_CUT==0
    REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif
    *reg = write_value;
#if QUEC_CUT_CODE==0
    REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
#else
    boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif
    return write_value;
}
#endif

void bootAdiBusBatchChange(volatile uint32_t *reg, uint32_t write_mask, uint32_t write_value, ...)
{
    va_list ap;
    va_start(ap, write_value);

    bool first = true;
    while ((uint32_t)reg != 0)
    {
        if (!first)
        {
            write_mask = va_arg(ap, uint32_t);
            write_value = va_arg(ap, uint32_t);
        }
#ifdef CONFIG_QUEC_PROJECT_FEATURE
        quec_bootAdiBusBatchChange(reg,write_mask,write_value);
#else
        //REG_ADI_MST_ARM_CMD_STATUS_T arm_cmd_status;
        if ((write_mask & VALUE_MASK) != VALUE_MASK)
        {
            //REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
            boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
            hwp_adiMst->arm_rd_cmd = (uint32_t)reg;

            //REG_ADI_MST_ARM_RD_DATA_T arm_rd_data;
            //REG_WAIT_FIELD_EQZ(arm_rd_data, hwp_adiMst->arm_rd_data, arm_rd_cmd_busy);
            boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
            REG_ADI_MST_ARM_RD_DATA_T arm_rd_data = {hwp_adiMst->arm_rd_data};

            if (arm_rd_data.b.arm_rd_addr != (((uint32_t)reg & 0xffff) >> 2))
                osiPanic();

            uint32_t value = arm_rd_data.b.arm_rd_cmd;
            write_value = (value & ~write_mask) | (write_value & write_mask);
        }

        //REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
        boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
        *reg = write_value;
        //REG_WAIT_FIELD_EQZ(arm_cmd_status, hwp_adiMst->arm_cmd_status, adi_busy);
        boot_reg_wait_field_eqz((volatile uint32_t *)&hwp_adiMst->arm_cmd_status,4,0x01);
#endif
        reg = va_arg(ap, volatile uint32_t *);
        first = false;
    }

    va_end(ap);
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
uint32_t bootAdiBusChange(volatile uint32_t *reg, uint32_t mask, uint32_t value)
{
    return quec_bootAdiBusBatchChange(reg, mask, value);
}
#else
void bootAdiBusChange(volatile uint32_t *reg, uint32_t mask, uint32_t value)
{
    bootAdiBusBatchChange(reg, mask, value, HAL_ADI_BUS_CHANGE_END);
}
#endif

OSI_DECL_STRONG_ALIAS(bootAdiBusInit, OSI_NO_RETURN void halAdiBusInit(void));
OSI_DECL_STRONG_ALIAS(bootAdiBusWrite, void halAdiBusWrite(volatile uint32_t *reg, uint32_t value));
OSI_DECL_STRONG_ALIAS(bootAdiBusRead, uint32_t halAdiBusRead(volatile uint32_t *reg));
OSI_DECL_STRONG_ALIAS(bootAdiBusBatchChange, void halAdiBusBatchChange(volatile uint32_t *reg, uint32_t write_mask, uint32_t write_value, ...));
OSI_DECL_STRONG_ALIAS(bootAdiBusChange, uint32_t halAdiBusChange(volatile uint32_t *reg, uint32_t mask, uint32_t value));


