/**
 * @file    systick.c
 * @author  Pratik Dhulubulu
 * @brief   SysTick timer configuration and interrupt handler.
 * @details Implements 1 ms system tick and provides global tick counter.
 */

#include "stm32f446xx.h"
#include "systick.h"

volatile uint32_t tick = 0;

/**
 * @brief   SysTick Interrupt Handler.
 * @details Executes every 1 ms. Increments the global tick counter.
 * @param   None
 * @retval  None
 */
void SysTick_Handler(void)
{
    tick++;
}

/**
 * @brief   Initialize SysTick for 1 ms periodic interrupts.
 * @details LOAD = (SystemCoreClock/1000) - 1 → 1 ms period at 50 MHz.
 *          Enables SysTick interrupt and counter.
 * @param   None
 * @retval  None
 */
void systick_init(void)
{
    SysTick->LOAD = 50000U - 1U;
    SysTick->VAL  = 0U;
    SysTick->CTRL = (1U << 2) |
                    (1U << 1) |
                    (1U << 0);
}
