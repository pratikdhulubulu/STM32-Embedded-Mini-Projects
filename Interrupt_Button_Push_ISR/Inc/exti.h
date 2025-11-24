/**
 * @file    exti.h
 * @author  Pratik Dhulubulu
 * @brief   EXTI13 driver interface.
 * @details Provides initialization for EXTI line 13 mapped to PC13.
 *          Supports rising-edge interrupt generation and NVIC configuration.
 */

#ifndef EXTI_H
#define EXTI_H

#include "stm32f446xx.h"

void exti13_init(void);

#endif
