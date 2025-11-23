#include "stm32f4xx.h"

void delay(volatile uint32_t time)
{
    while(time--) __NOP();
}

int main(void)
{
    // Enable clock for GPIOA and GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // Configure PA5 as general purpose output
    GPIOA->MODER &= ~(3UL << (5 * 2));   // clear mode
    GPIOA->MODER |=  (1UL << (5 * 2));   // set as output
	
		// Configure PA5 as general purpose output
		GPIOA->MODER &= ~(3UL << (13 * 2));
	
    // Optional: push-pull, no pull-up/down, low speed (default)
    GPIOA->OTYPER &= ~(1UL << 5);
    GPIOA->PUPDR  &= ~(3UL << (5 * 2));

    while (1)
    {
			if( !(GPIOC->IDR & (1 << 13)))
			{
				GPIOA->ODR |= (1UL << 5);   // Toggle LED
        delay(5000000);             // crude visible delay
			}
			GPIOA->ODR ^= (1UL << 5);
			delay(1000000);
    }
}
