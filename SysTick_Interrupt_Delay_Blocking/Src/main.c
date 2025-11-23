#include "stm32f446xx.h"

// Global millisecond counter updated inside SysTick interrupt
volatile uint32_t tick = 0;

//------------------------------------------------------------------
// SysTick ISR
// Fires every 1ms (because LOAD = 50000 - 1 for 50 MHz CPU clock)
// Increments the global tick counter
//------------------------------------------------------------------
void SysTick_Handler(void)
{
    tick++;
}

//------------------------------------------------------------------
// SysTick Initialization
// Generates interrupt every 1ms using CPU clock (50 MHz)
// LOAD = (50 MHz / 1000) - 1 = 50000 - 1
//------------------------------------------------------------------
void systick_init(void)
{
    SysTick->LOAD = 50000 - 1;     // Reload value for 1 ms
    SysTick->VAL  = 0;             // Clear current counter
    SysTick->CTRL = (1U<<2) |      // CLKSOURCE = Processor clock (HCLK)
                    (1U<<1) |      // TICKINT   = Enable SysTick interrupt
                    (1U<<0);       // ENABLE    = Start SysTick timer
}

//------------------------------------------------------------------
// Delay in milliseconds (blocking)
// Uses tick counter updated by SysTick interrupt
// No polling of hardware COUNTFLAG — timing from ISR
//------------------------------------------------------------------
void delay_ms(uint32_t ms)
{
    uint32_t start = tick;         // Capture current tick
    while ((tick - start) < ms);   // Wait until required ms passed
}

//------------------------------------------------------------------
// MAIN
// PA5 = LED (Nucleo board)
// Toggle LED every 1000 ms using SysTick interrupt based delay
//------------------------------------------------------------------
int main(void)
{
    // Enable clock for GPIOA peripheral
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure PA5 as General-Purpose Output
    GPIOA->MODER &= ~(3U << (5 * 2));   // Clear mode bits
    GPIOA->MODER |=  (1U << (5 * 2));   // Set mode = 01 (output)

    // Initialize SysTick to generate 1 ms interrupts
    systick_init();

    while (1)
    {
        GPIOA->ODR ^= (1U << 5);        // Toggle LED on PA5
        delay_ms(4000);                 // Delay 1000 ms (1 second)
    }
}
