/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_STM32H5_GPIO__H_
#define YSS_STM32H5_GPIO__H_

#include <yss/error.h>
#include <drv/peripheral.h>
#include <drv/Drv.h>

/**
 * @file Stm32h5Gpio.h
 * @brief GPIO driver for STM32H5 (STM32 LL).
 *
 * @details
 * The API follows the previous STM32 driver of yss (class_gpio_stm32.h, 2026.01).
 * Alternate functions are given as AF numbers (AF0 ~ AF15) from the datasheet
 * alternate function table, e.g. PA9 USART1_TX = AF7.
 *
 * @code
 * gpioC.setAsOutput(13);
 * gpioC.setOutput(13, true);
 *
 * gpioA.setAsAltFunc(9, Gpio::AF7);   // USART1_TX
 * @endcode
 */
class Gpio : public Drv
{
  public:
	enum ospeed_t : uint8_t
	{
		OSPEED_LOW = 0,
		OSPEED_MID,
		OSPEED_FAST,
		OSPEED_VERY_FAST
	};

	enum otype_t : uint8_t
	{
		OTYPE_PUSH_PULL = 0,
		OTYPE_OPEN_DRAIN
	};

	enum pupd_t : uint8_t
	{
		PUPD_NONE = 0,
		PUPD_PULL_UP,
		PUPD_PULL_DOWN
	};

	enum altFunc_t : uint8_t
	{
		AF0 = 0, AF1, AF2, AF3, AF4, AF5, AF6, AF7,
		AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15
	};

	/**
	 * @brief Configures a pin as a general purpose output.
	 */
	error_t setAsOutput(uint8_t pin, ospeed_t ospeed = OSPEED_MID, otype_t otype = OTYPE_PUSH_PULL);

	/**
	 * @brief Sets the output level of a pin (atomic, uses BSRR).
	 */
	void setOutput(uint8_t pin, bool data);

	/**
	 * @brief Configures a pin as an alternate function.
	 */
	error_t setAsAltFunc(uint8_t pin, altFunc_t altFunc, ospeed_t ospeed = OSPEED_MID, otype_t otype = OTYPE_PUSH_PULL);

	/**
	 * @brief Configures a pin as an input.
	 */
	void setAsInput(uint8_t pin, pupd_t pupd = PUPD_NONE);

	/**
	 * @brief Configures a pin as analog.
	 */
	void setAsAnalog(uint8_t pin);

	/**
	 * @brief Sets the pull-up / pull-down of a pin.
	 */
	void setPullUpDown(uint8_t pin, pupd_t pupd);

	/**
	 * @brief Reads the input level of a pin.
	 */
	bool getInputData(uint8_t pin);

	// The following are internal functions and must not be called by the user application.
	struct setup_t
	{
		GPIO_TypeDef *dev;
	};

	Gpio(const Drv::setup_t drvSetup, const setup_t setup);

  private:
	GPIO_TypeDef *mDev;
};

#endif
