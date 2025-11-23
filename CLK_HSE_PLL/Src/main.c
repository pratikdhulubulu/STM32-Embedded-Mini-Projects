#include "stm32f446xx.h"

void delay(void) {
    for (volatile int i = 0; i < 1000000; i++);
}

int main(void)
{
    /* 1) Enable clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* 2) Configure PA5 as output
       MODER5 = 01 (Output mode)
    */
    GPIOA->MODER &= ~(3U << (5 * 2));   // clear mode bits
    GPIOA->MODER |=  (1U << (5 * 2));   // set to output mode

    while (1) {
        GPIOA->ODR ^= (1U << 5);        // Toggle PA5
        delay();
    }
}
