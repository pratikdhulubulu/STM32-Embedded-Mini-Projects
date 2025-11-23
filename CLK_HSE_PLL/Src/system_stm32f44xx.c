/**
* @file    system_stm32f44xx.c
* @author  Pratik Dhulubulu
* @brief   STM32F446 System Clock Configuration
* @details Configures system clock to 70 MHz using external HSE oscillator and PLL.
*/

#include <stdint.h>
#include "stm32f446xx.h"

/**
* @brief   Initialize system clock for STM32F446 using HSE.
* @details Uses 8 MHz HSE crystal → PLL → 70 MHz SYSCLK.
* @param   None
* @retval  None
*/
void SystemInit(void){
  
    /* Starts external 8 MHz crystal and waits for HSERDY flag */
    RCC->CR |= RCC_CR_HSEON;
    while((RCC->CR & RCC_CR_HSERDY) == 0);

    /* Disable PLL and CSS before configuring PLL */
    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_CSSON);
    while(RCC->CR & RCC_CR_PLLRDY);

    /* Configure Flash latency and enable caches */
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;

    /* Configure PLL parameters */
    uint32_t pllm = 4;
    uint32_t plln = 140;
    uint32_t pllp = 2;

    /* Configure PLL registers */
    RCC->PLLCFGR = (pllm) |
                   (plln << 6) |
                   (((pllp/2)-1) << 16) |
                   (1 << 22);  /**< PLLSRC = HSE */

    /* Enable PLL and wait until ready */
    RCC->CR |= RCC_CR_PLLON;
    while((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Configure AHB and APB prescalers */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* Switch system clock to PLL */
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* Disable HSI */
    RCC->CR &= ~(RCC_CR_HSION);
}
