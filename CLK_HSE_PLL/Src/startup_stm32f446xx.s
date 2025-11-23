/**
 * @file    startup_stm32f446xx.s
 * @brief   STM32F446 startup file: Vector table and reset handler.
 * @details This file contains the vector table, reset handler, default
 *          exception and interrupt handlers for STM32F446. It also
 *          performs .data initialization, .bss zeroing, calls SystemInit,
 *          and then calls main().
 */

.syntax unified
.cpu cortex-m4
.thumb

/**
 * @section Vector_Table
 * @brief   ARM Cortex-M4 vector table.
 * @details Located in Flash at 0x08000000 as defined by the linker script.
 *          Contains initial stack pointer and ISR entry addresses.
 */
.section .isr_vector, "a", %progbits
.align 2
.global __isr_vector
__isr_vector:
    .word   _estack                 /* Initial Stack Pointer (Top of RAM) */
    .word   Reset_Handler           /* Reset Handler (entry after reset) */
    .word   NMI_Handler             /* Non-Maskable Interrupt */
    .word   HardFault_Handler       /* Hard Fault */
    .word   MemManage_Handler       /* Memory Management Fault */
    .word   BusFault_Handler        /* Bus Fault */
    .word   UsageFault_Handler      /* Usage Fault */
    .word   0                       /* Reserved */
    .word   0                       /* Reserved */
    .word   0                       /* Reserved */
    .word   0                       /* Reserved */
    .word   SVC_Handler             /* SVCall */
    .word   DebugMon_Handler        /* Debug Monitor */
    .word   0                       /* Reserved */
    .word   PendSV_Handler          /* PendSV */
    .word   SysTick_Handler         /* SysTick */

    /* Peripheral interrupt vectors (update NUM_IRQS based on MCU) */
    .equ    NUM_IRQS, 82
    .rept   NUM_IRQS
        .word   Default_Handler
    .endr

/**
 * @section External_Symbols
 * @brief   Symbols imported from linker script.
 */
.extern _sidata    /* Start address for .data initialization values in Flash */
.extern _sdata     /* Start of .data in RAM */
.extern _edata     /* End of .data in RAM */
.extern _sbss      /* Start of .bss in RAM */
.extern _ebss      /* End of .bss in RAM */
.extern SystemInit /* System initialization function (clock, PLL, etc.) */
.extern main       /* Main application entry */

/**
 * @section Reset_Handler
 * @brief   Reset handler routine.
 * @details Initializes .data section, zeros .bss, calls SystemInit(),
 *          then jumps to main(). Enters infinite loop if main() returns.
 */
.section .text.Reset_Handler, "ax", %progbits
.global Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    /* Copy .data section from Flash (_sidata) to RAM (_sdata → _edata) */
    ldr   r0, =_sdata
    ldr   r1, =_edata
    ldr   r2, =_sidata
data_copy:
    cmp   r0, r1
    ittt  lt
    ldrlt r3, [r2], #4
    strlt r3, [r0], #4
    blt   data_copy

    /* Zero initialize the .bss section (_sbss → _ebss) */
    ldr   r0, =_sbss
    ldr   r1, =_ebss
    movs  r2, #0
bss_clear:
    cmp   r0, r1
    it    lt
    strlt r2, [r0], #4
    blt   bss_clear

    /* Call system initialization routine */
    bl SystemInit

    /* Call main application */
    bl    main

infinite_loop:
    b     infinite_loop

.size Reset_Handler, . - Reset_Handler

/**
 * @section Default_Handlers
 * @brief   Default exception and interrupt handlers.
 * @details Weakly linked handlers that can be overridden in user C code.
 */
.section .text.Default_Handler, "ax", %progbits
Default_Handler:
    b Default_Handler

/* Weak aliasing of all core handlers */
.weak NMI_Handler
.weak HardFault_Handler
.weak MemManage_Handler
.weak BusFault_Handler
.weak UsageFault_Handler
.weak SVC_Handler
.weak DebugMon_Handler
.weak PendSV_Handler
.weak SysTick_Handler

.set NMI_Handler,        Default_Handler
.set HardFault_Handler,  Default_Handler
.set MemManage_Handler,  Default_Handler
.set BusFault_Handler,   Default_Handler
.set UsageFault_Handler, Default_Handler
.set SVC_Handler,        Default_Handler
.set DebugMon_Handler,   Default_Handler
.set PendSV_Handler,     Default_Handler
.set SysTick_Handler,    Default_Handler
