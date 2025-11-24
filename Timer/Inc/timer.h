/**
 * @file    timer.h
 * @author  Pratik Dhulubulu
 * @brief   Timer interface
 * @details Provides initialization timer operations.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief   Global millisecond tick counter.
 */
extern volatile uint32_t ms;

void timer_init(void);
void delay(uint32_t ms_delay);

#endif