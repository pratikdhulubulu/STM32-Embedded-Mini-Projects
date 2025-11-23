/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   blocking LED toggle using SysTick Interrupt.
 * @details Uses global tick counter updated every 1 ms by SysTick interrupt.
 *          LED on PA5 toggles every 1000 ms without blocking CPU.
 */

#include "stm32f446xx.h"
#include "systick.h"

/**
 * @brief   Main application entry point.
 * @details Initializes GPIOA and SysTick. Toggles LED every 1 second using
 *          non-blocking time comparison.
 * @param   None
 * @retval  int (never returns)
 */
int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    /* Initialize 1 ms SysTick */
    systick_init();

    while (1)
    {
        GPIOA->ODR ^= (1U << 5);
        delay_ms(1000);
    }
}
