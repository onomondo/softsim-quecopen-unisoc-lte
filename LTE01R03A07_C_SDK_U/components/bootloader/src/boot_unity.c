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

#define OSI_LOCAL_LOG_TAG LOG_TAG_UNITY

#include "hal_config.h"
#include "boot_config.h"
#include "boot_platform.h"
#include "boot_mem.h"
#include "boot_entry.h"
#include "cmsis_core.h"
#include "boot_mmu.h"
#include "boot_uart.h"
#include "boot_bsl_cmd.h"
#include "boot_fdl.h"
#include "hal_adi_bus.h"
#include "hal_chip.h"
#include "osi_log.h"
#include "osi_api.h"
#include "drv_names.h"
#include "calclib/crc32.h"
#include <alloca.h>
#include <string.h>
#include <sys/reent.h>
#ifdef CONFIG_SOC_8811
#include "8811/hal_rom_api.h"
#endif

extern int UnityMain(int argc, const char *argv[], void (*runAllTests)(void));
extern void UnityRunAllTests(void);

#ifdef CONFIG_UNITY_OUTPUT_UART
static bootUart_t *gUnityUart;
#else
#define UNITY_OUTPUT_LINE_SIZE (512)
static unsigned gUnityLineSize;
static char gUnityOutputLine[UNITY_OUTPUT_LINE_SIZE];
#endif

static void prvFdlIdentity(uint32_t param)
{
#ifdef CONFIG_SOC_8910
    uint32_t name = DRV_NAME_UART2;
#endif

#ifdef CONFIG_SOC_8811
    uint32_t name = DRV_NAME_UART3;
    if (param == ROM_LOADPAR_DOWNLOAD_UART1)
        name = DRV_NAME_UART1;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART2)
        name = DRV_NAME_UART2;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART3)
        name = DRV_NAME_UART3;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART4)
        name = DRV_NAME_UART4;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART5)
        name = DRV_NAME_UART5;
#endif

    bootUart_t *fdl_uart = bootUartOpen(name, 0, false);
    uint8_t sync;

    OSI_LOOP_WAIT(bootUartRead(fdl_uart, &sync, 1) > 0 && sync == 0x7e);

    static const char fdl_version_string[] = "Spreadtrum Boot Block version 1.2";
    void *pkt_buf = alloca(64);
    fdlPacket_t *pkt = (fdlPacket_t *)pkt_buf;
    unsigned ver_len = sizeof(fdl_version_string);
    pkt->type = OSI_TO_BE16(BSL_REP_VER);
    pkt->size = OSI_TO_BE16(ver_len);
    memcpy(&pkt->content[0], &fdl_version_string[0], ver_len);

    uint16_t crc = crc16FdlCalc((uint16_t *)pkt_buf, ver_len + 4);
    memcpy(&pkt->content[0] + ver_len, &crc, 2);
    bootUartWriteHdlc(fdl_uart, pkt, ver_len + 6);

    osiDelayUS(10000);

    pkt->type = OSI_TO_BE16(BSL_REP_ACK);
    pkt->size = 0;
    crc = crc16FdlCalc((uint16_t *)pkt_buf, 4);
    memcpy(&pkt->content[0], &crc, 2);
    bootUartWriteHdlc(fdl_uart, pkt, 6);
}

void bootUnityStart(uint32_t param)
{
#ifdef CONFIG_SOC_8910
    OSI_CLEAR_SECTION(bss);

    halClockInit();
    halRamInit();

    extern unsigned __mmu_ttbl1_start[];
    extern unsigned __mmu_ttbl2_start[];
    bootMmuEnable((uint32_t *)__mmu_ttbl1_start, (uint32_t *)__mmu_ttbl2_start);

    extern unsigned __sram_heap_start[];
    extern unsigned __sram_heap_end[];
    unsigned sram_heap_size = OSI_PTR_DIFF(__sram_heap_end, __sram_heap_start);
    bootHeapInit((uint32_t *)__sram_heap_start, sram_heap_size,
                 (uint32_t *)CONFIG_RAM_PHY_ADDRESS, CONFIG_RAM_SIZE);

    __FPU_Enable();
    _REENT_INIT_PTR_ZEROED(_impure_ptr);

    halAdiBusInit();
    bootResetPinEnable();
#endif

#ifdef CONFIG_SOC_8811
    osiICacheEnable();
    osiDCacheEnable();

    OSI_CLEAR_SECTION(bss);

    halClockInit(HAL_CLOCK_INIT_BOOTLOADER);

    SCB->CPACR |= (3 << 20) | (3 << 22);
    FPU->FPCCR |= (FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);
    _impure_ptr = _GLOBAL_REENT;

    extern unsigned __sram_heap_start[];
    extern unsigned __sram_heap_end[];
    unsigned sram_heap_size = OSI_PTR_DIFF(__sram_heap_end, __sram_heap_start);
    bootHeapInit((uint32_t *)__sram_heap_start, sram_heap_size, 0, 0);

    bootPlatformInit();
#endif

    halPmuExtFlashPowerOn();
    prvFdlIdentity(param);

    // Delay a while. In case fdl uart is the same as log or unity uart,
    // the delay may be helpful.
    osiDelayUS(200000);

    bootTraceInit(true);
    OSI_LOGI(0, "unity start");

#ifdef CONFIG_UNITY_OUTPUT_UART
    gUnityUart = bootUartOpen(CONFIG_UNITY_UART_NAME, CONFIG_UNITY_UART_BAUD, true);
#endif

    const char *argv[] = {"", "-v"};
    UnityMain(2, argv, UnityRunAllTests);

    OSI_DEAD_LOOP;
}

void UnityOutputChar(int c)
{
#ifdef CONFIG_UNITY_OUTPUT_UART
    uint8_t ch = c;
    bootUartWriteAll(gUnityUart, &ch, 1);
#else
    if (gUnityLineSize < UNITY_OUTPUT_LINE_SIZE - 1)
        gUnityOutputLine[gUnityLineSize++] = (char)c;

    if (c == 0 || c == '\n')
    {
        gUnityOutputLine[gUnityLineSize++] = '\0';
        gUnityLineSize = 0;
        OSI_LOGXI(OSI_LOGPAR_S, 0, "%s", gUnityOutputLine);
    }
#endif
}

void UnityOutputFlush(void)
{
}

void UnityOutputStart(void)
{
}

void UnityOutputComplete(void)
{
}
