/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file drv_clock_stm32h5.cpp
 * @brief Clock query driver for STM32H5 (STM32 LL).
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <targets/st/Stm32h5Clock.h>
#include <stm32h5xx_ll_rcc.h>

static LL_RCC_ClocksTypeDef getClocks(void)
{
	LL_RCC_ClocksTypeDef clocks;

	LL_RCC_GetSystemClocksFreq(&clocks);
	return clocks;
}

uint32_t Clock::getSystemClockFrequency(void)
{
	return getClocks().SYSCLK_Frequency;
}

uint32_t Clock::getCoreClockFrequency(void)
{
	return getClocks().HCLK_Frequency;
}

uint32_t Clock::getApb1ClockFrequency(void)
{
	return getClocks().PCLK1_Frequency;
}

uint32_t Clock::getApb2ClockFrequency(void)
{
	return getClocks().PCLK2_Frequency;
}

uint32_t Clock::getApb3ClockFrequency(void)
{
	return getClocks().PCLK3_Frequency;
}

uint32_t Clock::getApb1TimerClockFrequency(void)
{
	uint32_t pclk1 = getApb1ClockFrequency();

	// With TIMPRE = 0 (reset value), timer clock = PCLK1 if APB1 prescaler is 1, else 2 x PCLK1
	if (LL_RCC_GetAPB1Prescaler() == LL_RCC_APB1_DIV_1)
		return pclk1;
	else
		return pclk1 * 2;
}

#endif
