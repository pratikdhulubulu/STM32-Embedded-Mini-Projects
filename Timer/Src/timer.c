/**
 * @file    timer.c
 * @author  Pratik Dhulubulu
 * @brief   Timer configuration and interrupt handler.
 * @details Configure TIM2 for generating 1 ms delay.
 */

#include "stm32f446xx.h"
#include "timer.h"

volatile uint32_t ms = 0;

/**
 * @brief   TIM2 Interrupt Handler.
 * @details Executes when 1 ms delay completed and increments ms counter.
 * @param   None
 * @retval  None
 */
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & 1)
    {
        TIM2->SR = 0;
        ms++;
    }
}

/**
 * @brief   Initialize Timer 2.
 * @details Initialize TIM2 for 1 ms periodic interrupts.
 * @param   None
 * @retval  None
 */
void timer_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 49;
    TIM2->ARR = 999; 
    TIM2->DIER |= 1;
    TIM2->CR1 |= 1;

    NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief   Blocking delay in milliseconds.
 * @details Relies on ms counter updated by TIM2 interrupt.
 * @param   ms  Duration of delay in milliseconds.
 * @retval  None
 */
void delay(uint32_t ms_delay) {
    uint32_t start = ms;
    while((ms - start) < ms_delay);
}
