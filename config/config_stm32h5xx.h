/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

// Template of config.h for STM32H5. Copy it into the application as config.h.

#ifndef YSS_CONFIG__H_
#define YSS_CONFIG__H_

// 32-bit timer used by runtime (RUNTIME_TIM2 or RUNTIME_TIM5). Counts at 1 MHz.
#define YSS_RUNTIME_TIMER   RUNTIME_TIM2

// Scheduler time slice in core clock ticks (SysTick reload).
// 250 MHz / 250000 = 1 ms.
#define THREAD_GIVEN_CLOCK  250000

// Maximum number of threads, including main.
#define MAX_THREAD          8

// Fill thread stacks with a pattern to measure stack usage.
#define FILL_THREAD_STACK   false

#define USE_GUI             false
#define NUM_OF_YSS_KEY      0

// Peripherals
#define USART1_ENABLE       false
#define USART2_ENABLE       false
#define USART3_ENABLE       false
#define UART4_ENABLE        false
#define UART5_ENABLE        false
#define USART6_ENABLE       false
#define UART7_ENABLE        false

#endif
