#include "stm32f446xx.h"

void delay(void) {
    for (volatile int i = 0; i < 1000000; i++);
}

void delay_ms(uint32_t ms)
{
    // Step 1: Set reload value for 1ms delay
    // SysTick is 24-bit timer, counts down from RVR to 0
    // CPU clock = 50 MHz → 1ms = 50,000 ticks
    // We subtract 1 because counting includes 0
    SysTick->LOAD = 49999;
    SysTick->VAL = 0;
    SysTick->CTRL = (1<<2) | (1<<0); 
    for(uint32_t i = 0; i < ms; i++)
    {
        while(!(SysTick->CTRL & (1 << 16)));  // Poll COUNTFLAG
    }
    SysTick->CTRL = 0;  
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
        delay_ms(1000);
    }
}
