/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_STM32H5_UART__H_
#define YSS_STM32H5_UART__H_

#include <drv/Uart.h>
#include <drv/peripheral.h>

/**
 * @file Stm32h5Uart.h
 * @brief USART / UART driver for STM32H5 (STM32 LL).
 *
 * @details
 * Reception: RXNE interrupt pushes each byte into the Uart ring buffer.
 * Transmission: polling. The thread yields while the TX register is full,
 * so other threads keep running. DMA (GPDMA) is not used yet.
 * FIFO mode is disabled. Data is always 8 bits; with parity the frame is 9 bits.
 */
class Stm32h5Uart : public Uart
{
  public:
	// Internal system configurations. Do not call from user application.
	struct setup_t
	{
		USART_TypeDef *dev;
	};

	Stm32h5Uart(const Drv::setup_t drvSetup, const setup_t setup);

	/**
	 * @brief Initializes the UART (baudrate, stop bits, parity, receive buffer).
	 * @details MODE_ONE_WIRE and PARITY_MARK / PARITY_SPACE are not supported.
	 */
	error_t initialize(config_t config);

	virtual error_t changeBaudrate(int32_t baudrate);

	virtual error_t send(void *src, int32_t size, uint32_t timeout = 1000);

	virtual error_t send(int8_t data, uint32_t timeout = 1000);

	// Internal system interrupt routine. Do not call from user application.
	void isr(void);

  private:
	USART_TypeDef *mDev;
};

#endif
