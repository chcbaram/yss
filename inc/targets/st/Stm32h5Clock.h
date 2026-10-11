/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_STM32H5_CLOCK__H_
#define YSS_STM32H5_CLOCK__H_

#include <drv/Drv.h>

/**
 * @file Stm32h5Clock.h
 * @brief Clock query driver for STM32H5 (STM32 LL).
 *
 * @details
 * The clock tree itself (HSE, PLL1, voltage scaling, flash wait states) is set up
 * once in initializeClock() before main(), see msp_stm32h5.cpp. That function
 * runs before .data/.bss initialization, so it must not use this object.
 * This class only reports the resulting frequencies after main() starts.
 */
class Clock : public Drv
{
  public:
	/**
	 * @brief Returns the system clock (SYSCLK) frequency in Hz.
	 */
	uint32_t getSystemClockFrequency(void);

	/**
	 * @brief Returns the core / AHB clock (HCLK) frequency in Hz.
	 */
	uint32_t getCoreClockFrequency(void);

	/**
	 * @brief Returns the APB1 bus clock (PCLK1) frequency in Hz.
	 */
	uint32_t getApb1ClockFrequency(void);

	/**
	 * @brief Returns the APB2 bus clock (PCLK2) frequency in Hz.
	 */
	uint32_t getApb2ClockFrequency(void);

	/**
	 * @brief Returns the APB3 bus clock (PCLK3) frequency in Hz.
	 */
	uint32_t getApb3ClockFrequency(void);

	/**
	 * @brief Returns the kernel clock of timers on APB1 (TIM2~7, TIM12~14) in Hz.
	 * @details Timers run at PCLK1 when the APB1 prescaler is 1, otherwise at 2 x PCLK1 (TIMPRE = 0).
	 */
	uint32_t getApb1TimerClockFrequency(void);
};

#endif
