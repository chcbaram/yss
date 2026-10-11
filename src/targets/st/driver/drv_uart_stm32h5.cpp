/*
 * Copyright (c) 2026 Hancheol Cho
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file drv_uart_stm32h5.cpp
 * @brief USART / UART driver for STM32H5 (STM32 LL).
 */

#include <drv/mcu.h>

#if defined(STM32H5)

#include <targets/st/Stm32h5Uart.h>
#include <yss/scheduler.h>
#include <util/Timeout.h>
#include <stm32h5xx_ll_usart.h>

Stm32h5Uart::Stm32h5Uart(const Drv::setup_t drvSetup, const setup_t setup) : Uart(drvSetup)
{
	mDev = setup.dev;
	mRcvBufSize = 0;
	mHead = mTail = 0;
}

error_t Stm32h5Uart::initialize(config_t config)
{
	uint32_t dataWidth, parity, stopbit, direction;

	if (config.mode == MODE_ONE_WIRE)
		return error_t::NOT_SUPPORTED_YET;

	switch (config.parity)
	{
	case PARITY_NONE:
		dataWidth = LL_USART_DATAWIDTH_8B;
		parity = LL_USART_PARITY_NONE;
		break;

	// The parity bit takes the MSB of the frame, so 8 data bits + parity = 9-bit frame.
	case PARITY_ODD:
		dataWidth = LL_USART_DATAWIDTH_9B;
		parity = LL_USART_PARITY_ODD;
		break;

	case PARITY_EVEN:
		dataWidth = LL_USART_DATAWIDTH_9B;
		parity = LL_USART_PARITY_EVEN;
		break;

	default:
		return error_t::NOT_SUPPORTED_YET;
	}

	stopbit = (config.stopbit == STOP_2BIT) ? LL_USART_STOPBITS_2 : LL_USART_STOPBITS_1;

	switch (config.mode)
	{
	case MODE_TX_ONLY:
		direction = LL_USART_DIRECTION_TX;
		break;

	case MODE_RX_ONLY:
		direction = LL_USART_DIRECTION_RX;
		break;

	default:
		direction = LL_USART_DIRECTION_TX_RX;
		break;
	}

	// Configure the receive buffer before enabling the RX interrupt.
	if (config.mode != MODE_TX_ONLY)
	{
		if (config.rcvBuf == nullptr)
		{
			if (mRcvBuf != nullptr && mRcvBufSize != (int32_t)config.rcvBufSize)
			{
				delete[] mRcvBuf;
				mRcvBuf = nullptr;
			}
			if (mRcvBuf == nullptr)
				mRcvBuf = new int8_t[config.rcvBufSize];
		}
		else
			mRcvBuf = (int8_t *)config.rcvBuf;

		mRcvBufSize = config.rcvBufSize;
		mHead = mTail = 0;
	}

	// CR1/CR2/CR3/BRR/PRESC can only be written while UE = 0.
	LL_USART_Disable(mDev);

	LL_USART_ConfigCharacter(mDev, dataWidth, parity, stopbit);
	LL_USART_SetTransferDirection(mDev, direction);
	LL_USART_SetOverSampling(mDev, LL_USART_OVERSAMPLING_16);
	LL_USART_SetPrescaler(mDev, LL_USART_PRESCALER_DIV1);
	LL_USART_DisableFIFO(mDev);
	LL_USART_SetHWFlowCtrl(mDev, config.hwfc ? LL_USART_HWCONTROL_RTS_CTS : LL_USART_HWCONTROL_NONE);
	LL_USART_ConfigAsyncMode(mDev);

	int32_t clk = Drv::getClockFrequency();
	if (clk <= 0 || config.baudrate == 0 || (uint32_t)clk / 16 < config.baudrate)
		return error_t::WRONG_CONFIG;
	LL_USART_SetBaudRate(mDev, clk, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, config.baudrate);

	LL_USART_ClearFlag_ORE(mDev);
	LL_USART_ClearFlag_FE(mDev);
	LL_USART_ClearFlag_PE(mDev);
	LL_USART_ClearFlag_NE(mDev);

	if (config.mode != MODE_TX_ONLY)
	{
		LL_USART_EnableIT_RXNE_RXFNE(mDev);
		LL_USART_EnableIT_ERROR(mDev);   // ORE / FE / NE
		if (parity != LL_USART_PARITY_NONE)
			LL_USART_EnableIT_PE(mDev);
	}

	LL_USART_Enable(mDev);

	mMode = config.mode;

	return error_t::ERROR_NONE;
}

error_t Stm32h5Uart::changeBaudrate(int32_t baudrate)
{
	int32_t clk = Drv::getClockFrequency();

	if (clk <= 0 || baudrate <= 0 || clk / 16 < baudrate)
		return error_t::WRONG_CONFIG;

	// Wait for the last frame to leave the shifter before touching BRR.
	while (LL_USART_IsEnabled(mDev) && (mDev->CR1 & USART_CR1_TE) && !LL_USART_IsActiveFlag_TC(mDev))
		thread::yield();

	bool enabled = LL_USART_IsEnabled(mDev);

	LL_USART_Disable(mDev);
	LL_USART_SetBaudRate(mDev, clk, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, baudrate);
	if (enabled)
		LL_USART_Enable(mDev);

	return error_t::ERROR_NONE;
}

error_t Stm32h5Uart::send(void *src, int32_t size, uint32_t timeout)
{
	uint8_t *data = (uint8_t *)src;
	Timeout tout(timeout);

	for (int32_t i = 0; i < size; i++)
	{
		while (!LL_USART_IsActiveFlag_TXE_TXFNF(mDev))
		{
			if (tout.isTimeout())
				return error_t::TIMEOUT;
			thread::yield();
		}
		LL_USART_TransmitData8(mDev, data[i]);
	}

	while (!LL_USART_IsActiveFlag_TC(mDev))
	{
		if (tout.isTimeout())
			return error_t::TIMEOUT;
		thread::yield();
	}

	return error_t::ERROR_NONE;
}

error_t Stm32h5Uart::send(int8_t data, uint32_t timeout)
{
	return send(&data, 1, timeout);
}

void Stm32h5Uart::isr(void)
{
	uint32_t isr = mDev->ISR;

	if (isr & (USART_ISR_PE | USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE))
	{
		if (isr & USART_ISR_FE && mIsrHandler.frameError)
			mIsrHandler.frameError();
		if (isr & USART_ISR_PE && mIsrHandler.parityError)
			mIsrHandler.parityError();

		mDev->ICR = USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_ORECF;
	}

	// RDR is read even after an error so that RXNE is cleared.
	if (isr & USART_ISR_RXNE_RXFNE)
	{
		int8_t data = LL_USART_ReceiveData8(mDev);

		if (mIsrHandler.dataRx)
			mIsrHandler.dataRx(data);
		else if (mRcvBufSize > 0)
			push(data);
	}
}

#endif
