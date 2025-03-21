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

#include "boot_config.h"

#define CPSR_M_USR          0x10U
#define CPSR_M_FIQ          0x11U
#define CPSR_M_IRQ          0x12U
#define CPSR_M_SVC          0x13U
#define CPSR_M_MON          0x16U
#define CPSR_M_ABT          0x17U
#define CPSR_M_HYP          0x1AU
#define CPSR_M_UND          0x1BU
#define CPSR_M_SYS          0x1FU
#define CPSR_T              (1UL << 5)

// Refer to halArmv7aRegs_t
#define OFFSET_R0           (0 * 4)
#define OFFSET_R1           (1 * 4)
#define OFFSET_R2           (2 * 4)
#define OFFSET_R8           (8 * 4)
#define OFFSET_R15          (15 * 4)
#define OFFSET_D0           (16 * 4)
#define OFFSET_CPSR         (80 * 4)
#define OFFSET_FPSCR        (81 * 4)
#define OFFSET_EPSR         (82 * 4)
#define OFFSET_SP_USR       (83 * 4)
#define OFFSET_LR_USR       (84 * 4)
#define OFFSET_SP_HYP       (85 * 4)
#define OFFSET_SPSR_HYP     (86 * 4)
#define OFFSET_ELR_HYP      (87 * 4)
#define OFFSET_SP_SVC       (88 * 4)
#define OFFSET_LR_SVC       (89 * 4)
#define OFFSET_SPSR_SVC     (90 * 4)
#define OFFSET_SP_ABT       (91 * 4)
#define OFFSET_LR_ABT       (92 * 4)
#define OFFSET_SPSR_ABT     (93 * 4)
#define OFFSET_SP_UND       (94 * 4)
#define OFFSET_LR_UND       (95 * 4)
#define OFFSET_SPSR_UND     (96 * 4)
#define OFFSET_SP_MON       (97 * 4)
#define OFFSET_LR_MON       (98 * 4)
#define OFFSET_SPSR_MON     (99 * 4)
#define OFFSET_SP_IRQ       (100 * 4)
#define OFFSET_LR_IRQ       (101 * 4)
#define OFFSET_SPSR_IRQ     (102 * 4)
#define OFFSET_R8_FIQ       (103 * 4)
#define OFFSET_SP_FIQ       (108 * 4)
#define OFFSET_LR_FIQ       (109 * 4)
#define OFFSET_SPSR_FIQ     (110 * 4)

    .text
    .arm

    .section BOOT_ENTRY, #alloc, #execinstr

// ====================================================================
//  vectors, can be used as entry
// ====================================================================
    .align 5
    .global Vectors
    .type   Vectors, %function
Vectors:
    LDR     pc, =bootEntry
    LDR     pc, =Undef_Handler
    LDR     pc, =SVC_Handler
    LDR     pc, =PAbt_Handler
    LDR     pc, =DAbt_Handler
    B       .
    LDR     pc, =IRQ_Handler
    LDR     pc, =FIQ_Handler

// ====================================================================
//  IRQ handler
// ====================================================================
    .type   IRQ_Handler, %function
IRQ_Handler:
    PUSH    {r0-r12, r14}

    BLX     bootIrqHandler

    POP     {r0-r12, r14}
    SUBS    pc, lr, #4

// ====================================================================
//  UND/SVC handler
// ====================================================================
    .type   Undef_Handler, %function
    .type   SVC_Handler, %function
Undef_Handler:
SVC_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    MRS     r0, spsr
    TST     r0, #CPSR_T
    SUBNE   lr, lr, #2              /* thumb elr: -2 bytes */
    SUBEQ   lr, lr, #4              /* arm elr: -4 bytes */
    B       .LAbortCommon

// ====================================================================
//  PAbt handler
// ====================================================================
    .type   PAbt_Handler, %function
PAbt_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #4              /* elr: -4 bytes */
    B       .LAbortCommon

// ====================================================================
//  DAbt handler
// ====================================================================
    .type   DAbt_Handler, %function
DAbt_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #8              /* elr: -8 bytes */
    B       .LAbortCommon

// ====================================================================
//  FIQ handler
// ====================================================================
    .type   FIQ_Handler, %function
FIQ_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #4              /* elr: -4 bytes */
    B       .LAbortCommon

// ====================================================================
//  abort common, jump to blue screen
// ====================================================================
.LAbortCommon:
    LDR     r0, =gBlueScreenRegs
    STR     r1, [r0, #OFFSET_R1]
    POP     {r1}
    STR     r1, [r0, #OFFSET_R0]
    STR     lr, [r0, #OFFSET_R15]

    ADD     r1, r0, #OFFSET_R2
    STM     r1, {r2-r7}             /* r2-r7 */

    MRS     r3, spsr
    STR     r3, [r0, #OFFSET_CPSR]  /* cpsr before exception */

    VMRS    r1, fpscr
    STR     r1, [r0, #OFFSET_FPSCR]

    MRS     r2, cpsr
    STR     r2, [r0, #OFFSET_EPSR]  /* cpsr of exception */

    ADD     r1, r0, #OFFSET_D0
    VSTMIA  r1!, {d0-d15}
    VSTMIA  r1!, {d16-d31}

    AND     r3, r3, #0x1f
    BIC     r1, r2, #0x1f
    ORR     r1, r3
    MSR     cpsr_c, r1              /* change M bit to spsr */
    ADD     r1, r0, #OFFSET_R8
    STM     r1, {r8-r14}            /* r8-r14 */

    MOV     r3, #0

    CPS     #CPSR_M_SYS
    STR     sp, [r0, #OFFSET_SP_USR]
    STR     lr, [r0, #OFFSET_LR_USR]

    STR     r3, [r0, #OFFSET_SP_HYP]
    STR     r3, [r0, #OFFSET_SPSR_HYP]
    STR     r3, [r0, #OFFSET_ELR_HYP]

    CPS     #CPSR_M_SVC
    STR     sp, [r0, #OFFSET_SP_SVC]
    STR     lr, [r0, #OFFSET_LR_SVC]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_SVC]

    CPS     #CPSR_M_ABT
    STR     sp, [r0, #OFFSET_SP_ABT]
    STR     lr, [r0, #OFFSET_LR_ABT]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_ABT]

    CPS     #CPSR_M_UND
    STR     sp, [r0, #OFFSET_SP_UND]
    STR     lr, [r0, #OFFSET_LR_UND]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_UND]

    STR     r3, [r0, #OFFSET_SP_MON]
    STR     r3, [r0, #OFFSET_LR_MON]
    STR     r3, [r0, #OFFSET_SPSR_MON]

    CPS     #CPSR_M_IRQ
    STR     sp, [r0, #OFFSET_SP_IRQ]
    STR     lr, [r0, #OFFSET_LR_IRQ]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_IRQ]

    CPS     #CPSR_M_FIQ
    ADD     r1, r0, #OFFSET_R8_FIQ
    STM     r1, {r8-r12}            /* r8-r12 (fiq) */
    STR     sp, [r0, #OFFSET_SP_FIQ]
    STR     lr, [r0, #OFFSET_LR_FIQ]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_FIQ]

    MSR     cpsr_c, r2
    LDR     sp, =__blue_screen_end
    SUB     sp, #(26*4)             /* reeserve space for halBsContextArmv7a_t */
    MOV     r0, #0
    BLX     bootBlueScreen

.LAbortEnd:
    B       .LAbortEnd

    .ltorg
    .size   Vectors, .-Vectors

// ====================================================================
//  Entry of boot.
// ====================================================================
    .global bootEntry
    .type   bootEntry, %function
bootEntry:
    CPSID   if

    // Reset SCTLR Settings
    MRC     p15, 0, r4, c1, c0, 0   // Read CP15 System Control register
    BIC     r4, r4, #(0x1 << 12)    // Clear I bit 12 to disable I Cache
    BIC     r4, r4, #(0x1 <<  2)    // Clear C bit  2 to disable D Cache
    BIC     r4, r4, #0x1            // Clear M bit  0 to disable MMU
    BIC     r4, r4, #(0x1 << 11)    // Clear Z bit 11 to disable branch prediction
    BIC     r4, r4, #(0x1 << 13)    // Clear V bit 13 to disable hivecs
    MCR     p15, 0, r4, c1, c0, 0   // Write value back to CP15 System Control register
    ISB

    ORR     r4, r4, #(0x1 << 12)    // Set I bit 12 to enable I Cache
    MCR     p15, 0, r4, c1, c0, 0   // Write value back to CP15 System Control register
    ISB

    LDR     r4, =Vectors
    MCR     p15, 0, r4, c12, c0, 0

    CPS     CPSR_M_UND
    LDR     sp, =__blue_screen_end
    CPS     CPSR_M_ABT
    LDR     sp, =__blue_screen_end
    CPS     CPSR_M_FIQ
    LDR     sp, =__blue_screen_end
    CPS     CPSR_M_SVC
    LDR     sp, =__blue_screen_end

    CPS     CPSR_M_IRQ
    LDR     sp, =__irq_stack_end
    CPS     CPSR_M_SYS
    LDR     sp, =__sys_stack_end

    BLX     bootStart

    .ltorg
    .size   bootEntry, .-bootEntry

// ====================================================================
//  void bootJumpImageEntry(unsigned param, bootJumpEntry_t entry)
// ====================================================================
    .global bootJumpImageEntry
    .type   bootJumpImageEntry, %function
bootJumpImageEntry:
    CPSID   if

    MOV     r4, #0
    MCR     p15, 0, r4, c7, c5, 0   // Invalidate I Cache All

    // Reset SCTLR Settings
    MRC     p15, 0, r4, c1, c0, 0   // Read CP15 System Control register
    BIC     r4, r4, #(0x1 << 12)    // Clear I bit 12 to disable I Cache
    BIC     r4, r4, #(0x1 <<  2)    // Clear C bit  2 to disable D Cache
    BIC     r4, r4, #0x1            // Clear M bit  0 to disable MMU
    BIC     r4, r4, #(0x1 << 11)    // Clear Z bit 11 to disable branch prediction
    BIC     r4, r4, #(0x1 << 13)    // Clear V bit 13 to disable hivecs
    MCR     p15, 0, r4, c1, c0, 0   // Write value back to CP15 System Control register
    ISB

    ORR     r4, r4, #(0x1 << 12)    // Set I bit 12 to enable I Cache
    MCR     p15, 0, r4, c1, c0, 0   // Write value back to CP15 System Control register
    ISB

    BLX     r1

    .ltorg
    .size   bootJumpImageEntry, .-bootJumpImageEntry
