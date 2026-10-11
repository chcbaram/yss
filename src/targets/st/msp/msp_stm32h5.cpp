/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file msp_stm32h5.cpp
 * @brief MCU support package for STM32H5: early system init, SystemCoreClock.
 *
 * @details
 * Startup order (startup_stm32h563xx.s):
 *   Reset_Handler -> SystemInit() (msp_default.cpp) -> initializeSystem() (here)
 *   -> .data copy / .bss clear -> constructors -> main()
 *
 * initializeSystem() runs BEFORE .data/.bss are initialized. Code here and in
 * initializeClock() must only touch registers (LL), never global variables or
 * driver objects. SystemCoreClock is recomputed after main() starts by
 * initializeSystemTime() (instance_runtime_stm32h5.cpp).
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <drv/peripheral.h>
#include <stm32h5xx_ll_rcc.h>
#include <stm32h5xx_ll_icache.h>

extern "C"
{
	uint32_t SystemCoreClock = DEFAULT_CLOCK_SPEED;

	// Prescaler shift tables, normally defined in system_stm32h5xx.c (not linked,
	// see the file header). LL_RCC_GetSystemClocksFreq() uses them.
	const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
	const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};

	void SystemCoreClockUpdate(void)
	{
		LL_RCC_ClocksTypeDef clocks;

		LL_RCC_GetSystemClocksFreq(&clocks);
		SystemCoreClock = clocks.HCLK_Frequency;
	}
}

/**
 * @brief Moves Thread mode onto the process stack (PSP), like the SEGGER startup does.
 *
 * @details
 * The yss PendSV handler saves the interrupted context with "mrs r0, psp", so
 * Thread mode must already run on PSP when the first context switch happens.
 * SEGGER_THUMB_Startup.s sets PSP and CONTROL.SPSEL before main(); the vendor
 * GCC startup (startup_stm32h563xx.s) does not, and main() would run on MSP with
 * PSP = 0. The first PendSV then stores the context below address 0 and faults
 * (precise bus fault, BFAR = 0xFFFFFFC0).
 *
 * Thread mode keeps running on the same memory: PSP takes the current stack
 * pointer, so the frames of SystemInit() and its caller stay valid. MSP moves
 * to a separate handler stack, [__handler_stack_start__, __handler_stack_end__)
 * from the linker script.
 *
 * MSPLIM follows MSP. The ST startup sets MSPLIM = _sstack (bottom of the
 * startup stack); left as is, the first handler that pushes onto the new MSP
 * raises a UsageFault STKOF. MSPLIM is lowered before MSP moves, so the limit
 * never sits above the active stack pointer. PSPLIM stays 0: the scheduler does
 * not switch it per thread.
 *
 * Naked: must not touch the stack while switching.
 */
static void __attribute__((naked)) switchToProcessStack(void)
{
	__asm volatile(
		"mrs   r0, msp                     \n"
		"msr   psp, r0                     \n"
		"mrs   r0, control                 \n"
		"orr   r0, r0, #2                  \n" // SPSEL = 1: Thread mode uses PSP
		"msr   control, r0                 \n"
		"isb                               \n"
		"ldr   r0, =__handler_stack_start__\n"
		"msr   msplim, r0                  \n" // stack limit of the handler stack
		"ldr   r0, =__handler_stack_end__  \n"
		"msr   msp, r0                     \n" // handlers use the separate stack
		"bx    lr                          \n"
	);
}

/**
 * @brief Board clock configuration. Weak: the board overrides it.
 * @details The default keeps the reset clock (HSI 64 MHz / 2 = 32 MHz).
 *          Must not use global variables (runs before .data/.bss init).
 */
void __attribute__((weak)) initializeClock(void)
{
}

void initializeSystem(void)
{
	switchToProcessStack();

	// Enable the FPU (CP10, CP11). Needed before any float instruction and for
	// the FPU context save (vstmdb s16-s31) in PendSV.
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
	__DSB();
	__ISB();
#endif

	initializeClock();

	// Instruction cache (8 KB). Enabled after the flash wait states are set.
	LL_ICACHE_Enable();
}

// DMA (GPDMA) driver is not implemented yet.
void initializeDma(void)
{
}

#endif
