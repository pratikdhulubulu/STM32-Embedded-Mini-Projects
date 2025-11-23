#include "stm32f446xx.h"
volatile int ms = 0;
void delay(int ms_delay) {
    int start = ms;
    while((ms - start) < ms_delay);
}

void TIM2_IRQHandler(void)
{
    if(TIM2->SR & 1)
    {
        TIM2->SR = 0;
        ms++;
    }
}

int main(void)
{
    /* 1) Enable clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* 2) Configure PA5 as output
       MODER5 = 01 (Output mode)
    */
    GPIOA->MODER &= ~(3U << (5 * 2));   // clear mode bits
    GPIOA->MODER |=  (1U << (5 * 2));   // set to output mode

    TIM2->PSC = 24; // Configure Prescaler
    TIM2->ARR = 999; 
    TIM2->DIER |= 1; // enable update interrupt
    TIM2->CR1 |= 1; // Start Timer

    NVIC_EnableIRQ(TIM2_IRQn); // Enable NVIC

    while (1) {
        GPIOA->ODR ^= (1U << 5);        // Toggle PA5
        delay(1000);
    }
}
