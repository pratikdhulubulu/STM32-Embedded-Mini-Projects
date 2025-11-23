/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   LED toggle using SysTick delay on STM32F446RE.
 * @details Demonstrates SysTick polling-based delay
 */

#include "stm32f446xx.h"

/**
 * @brief   Blocking millisecond delay using SysTick.
 * @details Configures SysTick for a 1 ms tick and generates a blocking delay of ms.
 * @param   ms  Number of milliseconds to delay.
 * @retval  None
 */
void delay_ms(uint32_t ms)
{
    /* Load value for 1 ms: 50 MHz clock → 50,000 ticks */
    SysTick->LOAD = 49999;

    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0);

    /* Wait for 'ms' milliseconds */
    for(uint32_t i = 0; i < ms; i++)
    {
        while(!(SysTick->CTRL & (1 << 16)));
    }

    SysTick->CTRL = 0;
}

/**
 * @brief   Main program entry point.
 * @details Enables GPIOA clock and toggles PA5 LED every 1 second.
 * @param   None
 * @retval  int (never returns).
 */
int main(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    while (1) {
        GPIOA->ODR ^= (1U << 5);
        delay_ms(1000);
    }
}
