/**
 * @file    system_stm32f44xx.c
 * @author  Pratik Dhulubulu
 * @brief   STM32F446 System Clock Configuration
 * @details   Configures system clock to 50 MHz using HSI and PLL.
 */

#include <stdint.h>
#include "stm32f446xx.h"

/**
 * @brief   Initialize system clock for STM32F446.
 * @details Configures system to run at 50 MHz using internal HSI oscillator.
 *          Flash latency, AHB/APB prescalers configured. PLL used as SYSCLK.
 * @param   None
 * @retval  None
 * @warning None
 */
void SystemInit(void) {
    /* Enable HSI oscillator */
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    /* Disable PLL, HSE, and Clock Security System */
    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON | RCC_CR_CSSON);
    while (RCC->CR & RCC_CR_PLLRDY);

    /* Configure Flash latency and enable caches */
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_1WS;

    /* Configure PLL parameters */
    uint32_t pllm = 16; 
    uint32_t plln = 200;
    uint32_t pllp = 4;

    /* Configure PLL registers */
    RCC->PLLCFGR = (pllm) |
                   (plln << 6) |
                   (((pllp / 2) - 1) << 16) |
                   (0 << 22);

    /* Enable PLL and wait until ready */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Configure AHB and APB prescalers */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* Switch system clock to PLL */
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
