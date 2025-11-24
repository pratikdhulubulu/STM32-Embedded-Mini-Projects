/**
 * @file    exti.c
 * @author  Pratik
 * @brief   EXTI13 configuration and interrupt handler.
 * @details Implements mapping of PC13 to EXTI13, rising-edge trigger setup,
 *          and NVIC interrupt enabling. ISR toggles LED connected to PA5.
 */

#include "exti.h"

/**
 * @brief   Initialize EXTI line 13 for PC13 interrupt.
 * @details Sets up PC13 as EXTI13 with rising-edge trigger and enables
 *          its NVIC interrupt line.  
 * @param   None
 * @retval  None
 */
void exti13_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |=  (2  << 4);    // Port C = 2

    EXTI->RTSR |=  (1 << 13);
    EXTI->FTSR &= ~(1 << 13);

    EXTI->IMR |= (1 << 13);

    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief   EXTI Interrupt Handler for lines 10–15.
 * @details Handles EXTI13 pending interrupt. Clears pending flag
 *          and toggles LED on PA5. Includes small software delay.
 * @param   None
 * @retval  None
 */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 13))
    {
        EXTI->PR = (1 << 13);

        GPIOA->ODR ^= (1 << 5);

        for (volatile int i = 0; i < 100000; i++);
    }
}
