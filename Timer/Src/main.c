/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   blocking LED toggle using Timer 2 Interrupt.
 * @details Uses ms counter updated every 1 ms by TIM2 interrupt.
 *          LED on PA5 toggles every 1000 ms.
 */
#include "stm32f446xx.h"
#include "timer.h"

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));
    
    /* Initialize TIM2*/
    timer_init();

    while (1) {
        GPIOA->ODR ^= (1U << 5);
        delay(1000);
    }
}
