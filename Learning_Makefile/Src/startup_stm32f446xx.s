.syntax unified
.cpu cortex-m4
.thumb

/* Interrupt vector table */
.section .isr_vector, "a", %progbits
.word _estack        /* Initial stack pointer */
.word Reset_Handler  /* Reset Handler */
.word 0              /* NMI_Handler */
.word 0              /* HardFault_Handler */
.word 0              /* MemManage_Handler */
.word 0              /* BusFault_Handler */
.word 0              /* UsageFault_Handler */
.word 0              /* Reserved */
.word 0              /* Reserved */
.word 0              /* Reserved */
.word 0              /* Reserved */
.word 0              /* SVC_Handler */
.word 0              /* DebugMon_Handler */
.word 0              /* Reserved */
.word 0              /* PendSV_Handler */
.word 0              /* SysTick_Handler */

.section .text.Reset_Handler
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    bl main
1:  b 1b  /* Infinite loop */
