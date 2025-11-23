/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   Toggle onboard LED on STM32F446.
 * @details This program toggles the onboard LED connected to GPIOA pin 5
 *          in an infinite loop using a software delay.
 */

#include "stm32f446xx.h"

/**
 * @brief   Simple software delay.
 * @details Uses a volatile for-loop to create a rough delay. Duration is approximate.
 * @param   None
 * @retval  None
 */
static void delay(void) {
    volatile int i;
    
    for (i = 0; i < 1000000; i++);
}

/**
 * @brief   Main program entry point.
 * @details Enables GPIOA clock, configures PA5 as output, and toggles LED
 *          in an infinite loop with a software delay.
 * @param   None
 * @retval  None This function does not return in embedded applications.
 */
int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5U * 2U));
    GPIOA->MODER |=  (1U << (5U * 2U));

    while (1) {
        GPIOA->ODR ^= (1U << 5U);
        delay();
    }
}
