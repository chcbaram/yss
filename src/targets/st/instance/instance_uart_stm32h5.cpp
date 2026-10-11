/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file instance_uart_stm32h5.cpp
 * @brief USART / UART driver instances for STM32H5.
 *
 * @details
 * Each instance is compiled only when <NAME>_ENABLE is true in config.h.
 * The interrupt handler names follow startup_stm32h563xx.s.
 * USART1 is on APB2, the others on APB1.
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <yss/instance.h>
#include <config.h>
#include <stm32h5xx_ll_bus.h>
#include <stm32h5xx_ll_rcc.h>

// Defines the clock / NVIC / reset functions, setup tables, the object and the IRQ handler.
//   obj : object name (usart1)
//   DEV : peripheral name (USART1)
//   BUS : LL bus function group (APB2_GRP1)
//   KIND: USART or UART, selects LL_RCC_Get<KIND>ClockFreq()
#define YSS_STM32H5_UART_INSTANCE(obj, DEV, BUS, KIND)                      \
	static void enable_##obj##_clock(bool en)                               \
	{                                                                       \
		uint32_t primask = __get_PRIMASK();                                 \
		__disable_irq();                                                    \
		if (en)                                                             \
			LL_##BUS##_EnableClock(LL_##BUS##_PERIPH_##DEV);                \
		else                                                                \
			LL_##BUS##_DisableClock(LL_##BUS##_PERIPH_##DEV);               \
		__set_PRIMASK(primask);                                             \
	}                                                                       \
                                                                            \
	static void enable_##obj##_interrupt(bool en)                           \
	{                                                                       \
		if (en)                                                             \
			NVIC_EnableIRQ(DEV##_IRQn);                                     \
		else                                                                \
			NVIC_DisableIRQ(DEV##_IRQn);                                    \
	}                                                                       \
                                                                            \
	static void reset_##obj(void)                                           \
	{                                                                       \
		uint32_t primask = __get_PRIMASK();                                 \
		__disable_irq();                                                    \
		LL_##BUS##_ForceReset(LL_##BUS##_PERIPH_##DEV);                     \
		LL_##BUS##_ReleaseReset(LL_##BUS##_PERIPH_##DEV);                   \
		__set_PRIMASK(primask);                                             \
	}                                                                       \
                                                                            \
	static uint32_t get_##obj##_clock_frequency(void)                       \
	{                                                                       \
		return LL_RCC_Get##KIND##ClockFreq(LL_RCC_##DEV##_CLKSOURCE);       \
	}                                                                       \
                                                                            \
	static const Drv::setup_t gDrv_##obj##_setup =                          \
	{                                                                       \
		enable_##obj##_clock,                                               \
		enable_##obj##_interrupt,                                           \
		reset_##obj,                                                        \
		get_##obj##_clock_frequency                                         \
	};                                                                      \
                                                                            \
	static const Stm32h5Uart::setup_t g_##obj##_setup =                     \
	{                                                                       \
		DEV                                                                 \
	};                                                                      \
                                                                            \
	Stm32h5Uart obj(gDrv_##obj##_setup, g_##obj##_setup);                   \
                                                                            \
	extern "C" void DEV##_IRQHandler(void)                                  \
	{                                                                       \
		obj.isr();                                                          \
	}

#if defined(USART1) && USART1_ENABLE
YSS_STM32H5_UART_INSTANCE(usart1, USART1, APB2_GRP1, USART)
#endif
#if defined(USART2) && USART2_ENABLE
YSS_STM32H5_UART_INSTANCE(usart2, USART2, APB1_GRP1, USART)
#endif
#if defined(USART3) && USART3_ENABLE
YSS_STM32H5_UART_INSTANCE(usart3, USART3, APB1_GRP1, USART)
#endif
#if defined(UART4) && UART4_ENABLE
YSS_STM32H5_UART_INSTANCE(uart4, UART4, APB1_GRP1, UART)
#endif
#if defined(UART5) && UART5_ENABLE
YSS_STM32H5_UART_INSTANCE(uart5, UART5, APB1_GRP1, UART)
#endif
#if defined(USART6) && USART6_ENABLE
YSS_STM32H5_UART_INSTANCE(usart6, USART6, APB1_GRP1, USART)
#endif
#if defined(UART7) && UART7_ENABLE
YSS_STM32H5_UART_INSTANCE(uart7, UART7, APB1_GRP1, UART)
#endif

#endif
