/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file instance_runtime_stm32h5.cpp
 * @brief OS runtime timer for STM32H5.
 *
 * @details
 * A 32-bit timer (TIM2 or TIM5, YSS_RUNTIME_TIMER in config.h) counts at 1 MHz.
 * The update (overflow) interrupt extends it to 64 bits. One overflow takes
 * 2^32 us = 71.6 minutes, so the interrupt is rare.
 *
 * thread::delay() polls runtime::getUsec(), so this timer must be running before
 * any thread calls delay(). initializeYss() calls initializeSystemTime() first.
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <config.h>
#include <yss/instance.h>
#include <util/runtime.h>
#include <stm32h5xx_ll_bus.h>
#include <stm32h5xx_ll_tim.h>

#if YSS_RUNTIME_TIMER == RUNTIME_TIM2
#define ISR_RUNTIME         TIM2_IRQHandler
#define RUNTIME_DEV         TIM2
#define RUNTIME_IRQ         TIM2_IRQn
#define RUNTIME_PERIPH      LL_APB1_GRP1_PERIPH_TIM2
#define RUNTIME_DBG_STOP    DBGMCU_APB1FZR1_DBG_TIM2_STOP
#elif YSS_RUNTIME_TIMER == RUNTIME_TIM5
#define ISR_RUNTIME         TIM5_IRQHandler
#define RUNTIME_DEV         TIM5
#define RUNTIME_IRQ         TIM5_IRQn
#define RUNTIME_PERIPH      LL_APB1_GRP1_PERIPH_TIM5
#define RUNTIME_DBG_STOP    DBGMCU_APB1FZR1_DBG_TIM5_STOP
#else
#error "config.h: YSS_RUNTIME_TIMER must be RUNTIME_TIM2 or RUNTIME_TIM5"
#endif

static volatile uint32_t gOverflowCount;

extern "C"
{
	void ISR_RUNTIME(void)
	{
		if (LL_TIM_IsActiveFlag_UPDATE(RUNTIME_DEV))
		{
			LL_TIM_ClearFlag_UPDATE(RUNTIME_DEV);
			gOverflowCount++;
		}
	}
}

void initializeSystemTime(void)
{
	// SystemCoreClock was computed in SystemInit() before .data was initialized,
	// so the value was overwritten. Recompute it here, the first thing after main().
	SystemCoreClockUpdate();

	LL_APB1_GRP1_EnableClock(RUNTIME_PERIPH);
	LL_APB1_GRP1_ForceReset(RUNTIME_PERIPH);
	LL_APB1_GRP1_ReleaseReset(RUNTIME_PERIPH);

	uint32_t timclk = clock.getApb1TimerClockFrequency();

	LL_TIM_SetPrescaler(RUNTIME_DEV, timclk / 1000000 - 1);
	LL_TIM_SetAutoReload(RUNTIME_DEV, 0xFFFFFFFF);
	LL_TIM_SetCounterMode(RUNTIME_DEV, LL_TIM_COUNTERMODE_UP);
	// UIF only on counter overflow (URS = 1). Otherwise the UG event below sets
	// UIF and the first interrupt counts a false overflow (+71.6 minutes).
	LL_TIM_SetUpdateSource(RUNTIME_DEV, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_GenerateEvent_UPDATE(RUNTIME_DEV);   // load PSC now
	LL_TIM_ClearFlag_UPDATE(RUNTIME_DEV);
	LL_TIM_EnableIT_UPDATE(RUNTIME_DEV);

	// Stop the counter while the core is halted by the debugger.
	DBGMCU->APB1FZR1 |= RUNTIME_DBG_STOP;

	gOverflowCount = 0;
	NVIC_EnableIRQ(RUNTIME_IRQ);
	LL_TIM_EnableCounter(RUNTIME_DEV);
}

namespace runtime
{
uint64_t getUsec(void)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	uint32_t high = gOverflowCount;
	uint32_t cnt = LL_TIM_GetCounter(RUNTIME_DEV);

	// The counter wrapped but the interrupt has not run yet (we masked it).
	// If cnt is small the wrap happened before the read, so count it here.
	if (LL_TIM_IsActiveFlag_UPDATE(RUNTIME_DEV) && cnt < 0x80000000)
		high++;

	__set_PRIMASK(primask);

	return ((uint64_t)high << 32) | cnt;
}

uint64_t getMsec(void)
{
	return getUsec() / 1000;
}

uint32_t getSec(void)
{
	return getUsec() / 1000000;
}

void start(void)
{
	LL_TIM_EnableCounter(RUNTIME_DEV);
}

void stop(void)
{
	LL_TIM_DisableCounter(RUNTIME_DEV);
}
}

#endif
