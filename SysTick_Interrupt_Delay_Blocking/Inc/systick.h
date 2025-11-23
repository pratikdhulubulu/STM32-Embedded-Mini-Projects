/**
 * @file    systick.h
 * @author  Pratik Dhulubulu
 * @brief   SysTick timer interface
 * @details Provides initialization and global tick counter for timing operations.
 */

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * @brief   Global millisecond tick counter.
 * @details Incremented every 1 ms inside SysTick_Handler().
 */
extern volatile uint32_t tick;

void systick_init(void);
void delay_ms(uint32_t ms);

#endif
