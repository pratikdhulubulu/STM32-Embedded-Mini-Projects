#include "stm32f446xx.h"

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR & (1<<13))  // check pending for EXTI13
    {
        EXTI->PR = (1<<13);      // clear pending
        GPIOA->ODR ^= (1<<5);    // toggle LED
        for(volatile int i=0;i<100000;i++);
    }
}

int main(void)
{
    // 1. Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 2. Configure PA5 as output (LED)
    GPIOA->MODER &= ~(3U << (5*2));
    GPIOA->MODER |=  (1U << (5*2));     // 01 = output
    GPIOA->OTYPER &= ~(1<<5);           // push-pull
    GPIOA->OSPEEDR &= ~(3U << (5*2));
    GPIOA->OSPEEDR |=  (1U << (5*2));   // medium speed
    GPIOA->PUPDR &= ~(3U << (5*2));     // no pull-up/pull-down

    // 3. Configure PC13 as input pull-up (Button)
    GPIOC->MODER &= ~(3U << (13*2));    // input
    GPIOC->PUPDR &= ~(3U << (13*2));
    GPIOC->PUPDR |=  (1U << (13*2));    // pull-up

    // 4. Map PC13 to EXTI13
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |= (2 << 4);      // Port C

    // 5. Enable EXTI13 falling edge interrupt
    EXTI->IMR  |= (1 << 13);
    EXTI->RTSR |= (1 << 13);  // Rising edge trigger
    EXTI->FTSR &= ~(1 << 13); // Disable falling edge


    // 6. Enable NVIC for EXTI15_10
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);

    while(1)
    {
        // LED toggled inside ISR
    }
}
