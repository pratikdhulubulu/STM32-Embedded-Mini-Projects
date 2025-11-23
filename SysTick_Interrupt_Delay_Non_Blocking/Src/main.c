#include "stm32f446xx.h"

// Global millisecond counter incremented in SysTick interrupt
volatile uint32_t tick = 0;

//------------------------------------------------------------------
// SysTick Interrupt Handler
// Runs every 1ms because LOAD = 49999 (for 50 MHz CPU clock)
//------------------------------------------------------------------
void SysTick_Handler(void)
{
    tick++;    // 1 ms has passed
}

//------------------------------------------------------------------
// Initialize SysTick for 1 ms interrupt
//------------------------------------------------------------------
void systick_init(void)
{
    SysTick->LOAD = 50000 - 1;     // 50 MHz / 1000 = 50,000 ticks → 1ms
    SysTick->VAL  = 0;             // Reset current counter
    SysTick->CTRL = (1U<<2) |      // CLKSOURCE = CPU clock (HCLK)
                    (1U<<1) |      // TICKINT = Enable SysTick interrupt
                    (1U<<0);       // ENABLE = Start SysTick
}

//------------------------------------------------------------------
// MAIN (Non-blocking LED blink)
// LED toggles every 1000 ms using time comparison
// CPU never stops; loop always free to do other work
//------------------------------------------------------------------
int main(void)
{
    uint32_t next_toggle_time = 0;

    // Enable GPIOA clock (PA5 = LED)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure PA5 as output
    GPIOA->MODER &= ~(3U << (5 * 2));   // Clear mode bits
    GPIOA->MODER |=  (1U << (5 * 2));   // Set mode = 01 (output)

    // Start SysTick (1 ms interrupt)
    systick_init();

    // Set initial event time = current tick + 1000 ms
    next_toggle_time = tick + 1000;

    while (1)
    {
        //----------------------------------------------------------
        // NON-BLOCKING TIMER CHECK
        // If current time reached or passed next_toggle_time:
        //----------------------------------------------------------
        if ((int32_t)(tick - next_toggle_time) >= 0)
        {
            GPIOA->ODR ^= (1U << 5);    // Toggle LED

            // Schedule next toggle after 1000 ms
            next_toggle_time = tick + 1000;
        }

        //----------------------------------------------------------
        // CPU FREE → Place other tasks here:
        //
        //  read sensors
        //  communicate via UART/SPI/I2C
        //  run state machines
        //  handle buttons (non-blocking)
        //  do calculations
        //----------------------------------------------------------
    }
}
