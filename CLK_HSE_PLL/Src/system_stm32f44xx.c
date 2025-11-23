#include <stdint.h>
#include "stm32f446xx.h"

void SystemInit(void){
  
    RCC->CR |= RCC_CR_HSEON; // Enable HSE
    while((RCC->CR & RCC_CR_HSERDY) == 0); // Wait for start of HSE

    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_CSSON); // Disabled PLL,CSS
    while(RCC->CR & RCC_CR_PLLRDY);// Wait for PLL Disable

    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                 FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS; // Flash accelerations

    uint32_t pllm = 4;   /* 2 MHz VCO input */
    uint32_t plln = 140;  /* 280 MHz VCO output */
    uint32_t pllp = 2;    /* => 70 MHz SYSCLK */

    RCC->PLLCFGR = (pllm) |
                   (plln << 6) |
                   (((pllp/2)-1) << 16) |
                   (1 << 22); // Configurating PLL

    RCC->CR |= RCC_CR_PLLON; // Enable PLL
    while((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for start PLL

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // Set AHB - DIV1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // Set APB1 - DIV2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // Set APB2 - DIV1

    RCC->CFGR &= ~(RCC_CFGR_SW); // Clear System CLK switch bits 00
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Set PLL output as System CLK
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait for System CLK is switch to PLL

    RCC->CR &= ~(RCC_CR_HSION);
}