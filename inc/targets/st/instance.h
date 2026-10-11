/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_INSTANCE_STM32H5__H_
#define YSS_INSTANCE_STM32H5__H_

#include <drv/peripheral.h>
#include <config.h>

/**
 * @file instance.h
 * @brief Global driver instances for STM32H5 (drivers implemented with STM32 LL).
 */

// 32-bit general purpose timers usable as the runtime timer (YSS_RUNTIME_TIMER in config.h)
#define RUNTIME_TIM2		2
#define RUNTIME_TIM5		5

// EXTI and Flash drivers are not implemented yet. These headers define
// YSS_DRV_EXTI_UNSUPPORTED / YSS_DRV_FLASH_UNSUPPORTED for STM32H5, which makes
// initializeYss() and instance_common.cpp skip them.
#include <drv/Exti.h>
#include <drv/Flash.h>

#include "Stm32h5Clock.h"
extern Clock clock;

#include "Stm32h5Gpio.h"
#if defined(GPIOA)
extern Gpio gpioA;
#endif
#if defined(GPIOB)
extern Gpio gpioB;
#endif
#if defined(GPIOC)
extern Gpio gpioC;
#endif
#if defined(GPIOD)
extern Gpio gpioD;
#endif
#if defined(GPIOE)
extern Gpio gpioE;
#endif
#if defined(GPIOF)
extern Gpio gpioF;
#endif
#if defined(GPIOG)
extern Gpio gpioG;
#endif
#if defined(GPIOH)
extern Gpio gpioH;
#endif
#if defined(GPIOI)
extern Gpio gpioI;
#endif

#include "Stm32h5Uart.h"
#if defined(USART1) && USART1_ENABLE
extern Stm32h5Uart usart1;
#endif
#if defined(USART2) && USART2_ENABLE
extern Stm32h5Uart usart2;
#endif
#if defined(USART3) && USART3_ENABLE
extern Stm32h5Uart usart3;
#endif
#if defined(UART4) && UART4_ENABLE
extern Stm32h5Uart uart4;
#endif
#if defined(UART5) && UART5_ENABLE
extern Stm32h5Uart uart5;
#endif
#if defined(USART6) && USART6_ENABLE
extern Stm32h5Uart usart6;
#endif
#if defined(UART7) && UART7_ENABLE
extern Stm32h5Uart uart7;
#endif

#endif
