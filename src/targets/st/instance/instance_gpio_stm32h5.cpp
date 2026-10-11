/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file instance_gpio_stm32h5.cpp
 * @brief GPIO driver instances for STM32H5 (GPIOA ~ GPIOI on AHB2).
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <yss/instance.h>
#include <stm32h5xx_ll_bus.h>

// Defines the clock / reset functions, setup tables and the Gpio object of one port.
//   x: port letter (A, B, ...)
#define YSS_STM32H5_GPIO_INSTANCE(x)                                        \
	static void enableGpio##x##Clock(bool en)                               \
	{                                                                       \
		uint32_t primask = __get_PRIMASK();                                 \
		__disable_irq();                                                    \
		if (en)                                                             \
			LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIO##x);          \
		else                                                                \
			LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIO##x);         \
		__set_PRIMASK(primask);                                             \
	}                                                                       \
                                                                            \
	static void resetGpio##x(void)                                          \
	{                                                                       \
		uint32_t primask = __get_PRIMASK();                                 \
		__disable_irq();                                                    \
		LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_GPIO##x);               \
		LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_GPIO##x);             \
		__set_PRIMASK(primask);                                             \
	}                                                                       \
                                                                            \
	static const Drv::setup_t gDrvGpio##x##Setup =                          \
	{                                                                       \
		enableGpio##x##Clock, /* clockFunc */                               \
		nullptr,              /* nvicFunc  */                               \
		resetGpio##x,         /* resetFunc */                               \
		nullptr               /* getClockFunc */                            \
	};                                                                      \
                                                                            \
	static const Gpio::setup_t gGpio##x##Setup =                            \
	{                                                                       \
		GPIO##x                                                             \
	};                                                                      \
                                                                            \
	Gpio gpio##x(gDrvGpio##x##Setup, gGpio##x##Setup);

#if defined(GPIOA)
YSS_STM32H5_GPIO_INSTANCE(A)
#endif
#if defined(GPIOB)
YSS_STM32H5_GPIO_INSTANCE(B)
#endif
#if defined(GPIOC)
YSS_STM32H5_GPIO_INSTANCE(C)
#endif
#if defined(GPIOD)
YSS_STM32H5_GPIO_INSTANCE(D)
#endif
#if defined(GPIOE)
YSS_STM32H5_GPIO_INSTANCE(E)
#endif
#if defined(GPIOF)
YSS_STM32H5_GPIO_INSTANCE(F)
#endif
#if defined(GPIOG)
YSS_STM32H5_GPIO_INSTANCE(G)
#endif
#if defined(GPIOH)
YSS_STM32H5_GPIO_INSTANCE(H)
#endif
#if defined(GPIOI)
YSS_STM32H5_GPIO_INSTANCE(I)
#endif

#endif
