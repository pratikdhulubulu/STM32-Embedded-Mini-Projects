/**
 * @file    main.c
 * @author  Pratik
 * @brief   External interrupt example using EXTI13.
 * @details PC13 button triggers a rising-edge interrupt that toggles
 *          LED on PA5 inside EXTI IRQ handler.
 */

#include "stm32f446xx.h"
#include "exti.h"

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    /* PC13 - Button input with pull-up */
    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->PUPDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR |=  (1U << (13 * 2));

    /* Initialize EXTI13 */
    exti13_init();

    while (1)
    {
        /* LED toggled inside EXTI ISR */
    }
}
