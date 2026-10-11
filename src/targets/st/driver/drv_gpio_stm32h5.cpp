/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file drv_gpio_stm32h5.cpp
 * @brief GPIO driver for STM32H5 (STM32 LL).
 *
 * @details
 * MODER/OTYPER/OSPEEDR/PUPDR/AFR are read-modify-write registers shared by all
 * pins of a port. Interrupts are masked around each update so that two threads
 * configuring different pins of the same port cannot corrupt each other.
 * Output uses BSRR, which is atomic.
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <targets/st/Stm32h5Gpio.h>
#include <stm32h5xx_ll_gpio.h>

static const uint32_t gSpeed[] = {LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_SPEED_FREQ_MEDIUM, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_SPEED_FREQ_VERY_HIGH};
static const uint32_t gOtype[] = {LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_OUTPUT_OPENDRAIN};
static const uint32_t gPupd[] = {LL_GPIO_PULL_NO, LL_GPIO_PULL_UP, LL_GPIO_PULL_DOWN};

Gpio::Gpio(const Drv::setup_t drvSetup, const setup_t setup) : Drv(drvSetup)
{
	mDev = setup.dev;
}

error_t Gpio::setAsOutput(uint8_t pin, ospeed_t ospeed, otype_t otype)
{
	if (pin > 15)
		return error_t::WRONG_CONFIG;

	uint32_t mask = 1UL << pin;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	LL_GPIO_SetPinSpeed(mDev, mask, gSpeed[ospeed]);
	LL_GPIO_SetPinOutputType(mDev, mask, gOtype[otype]);
	LL_GPIO_SetPinMode(mDev, mask, LL_GPIO_MODE_OUTPUT);
	__set_PRIMASK(primask);

	return error_t::ERROR_NONE;
}

void Gpio::setOutput(uint8_t pin, bool data)
{
	if (data)
		LL_GPIO_SetOutputPin(mDev, 1UL << pin);
	else
		LL_GPIO_ResetOutputPin(mDev, 1UL << pin);
}

error_t Gpio::setAsAltFunc(uint8_t pin, altFunc_t altFunc, ospeed_t ospeed, otype_t otype)
{
	if (pin > 15)
		return error_t::WRONG_CONFIG;

	uint32_t mask = 1UL << pin;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	if (pin < 8)
		LL_GPIO_SetAFPin_0_7(mDev, mask, altFunc);
	else
		LL_GPIO_SetAFPin_8_15(mDev, mask, altFunc);
	LL_GPIO_SetPinSpeed(mDev, mask, gSpeed[ospeed]);
	LL_GPIO_SetPinOutputType(mDev, mask, gOtype[otype]);
	LL_GPIO_SetPinMode(mDev, mask, LL_GPIO_MODE_ALTERNATE);
	__set_PRIMASK(primask);

	return error_t::ERROR_NONE;
}

void Gpio::setAsInput(uint8_t pin, pupd_t pupd)
{
	uint32_t mask = 1UL << pin;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	LL_GPIO_SetPinPull(mDev, mask, gPupd[pupd]);
	LL_GPIO_SetPinMode(mDev, mask, LL_GPIO_MODE_INPUT);
	__set_PRIMASK(primask);
}

void Gpio::setAsAnalog(uint8_t pin)
{
	uint32_t mask = 1UL << pin;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	LL_GPIO_SetPinPull(mDev, mask, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(mDev, mask, LL_GPIO_MODE_ANALOG);
	__set_PRIMASK(primask);
}

void Gpio::setPullUpDown(uint8_t pin, pupd_t pupd)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	LL_GPIO_SetPinPull(mDev, 1UL << pin, gPupd[pupd]);
	__set_PRIMASK(primask);
}

bool Gpio::getInputData(uint8_t pin)
{
	return LL_GPIO_IsInputPinSet(mDev, 1UL << pin);
}

#endif
