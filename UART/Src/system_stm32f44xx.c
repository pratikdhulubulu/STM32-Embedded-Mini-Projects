#include <stdint.h>
#include "stm32f446xx.h"

void SystemInit(void)
{
    RCC->CR |= RCC_CR_HSION;
    while((RCC->CR & RCC_CR_HSIRDY) == 0);

    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON | RCC_CR_CSSON);
    while(RCC->CR & RCC_CR_PLLRDY);

    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                 FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_1WS;

    uint32_t pllm = 16;
    uint32_t plln = 200;
    uint32_t pllp = 4;

    RCC->PLLCFGR = (pllm) |
                   (plln << 6) |
                   (((pllp/2)-1) << 16);

    RCC->CR |= RCC_CR_PLLON;
    while((RCC->CR & RCC_CR_PLLRDY) == 0);

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // SysTick --> 1ms
    SysTick->LOAD = 50000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}
