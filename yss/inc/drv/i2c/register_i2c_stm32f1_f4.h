////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_2.0
// 본 소스코드의 소유권은 yss Embedded Operating System 네이버 카페 관리자와 운영진에게 있습니다.
// 운영진이 임의로 코드의 권한을 타인에게 양도할 수 없습니다.
// 본 소스코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스코드의 상업적 또는 비상업적 이용이 가능합니다.
// 본 소스코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떤한 법적 책임을 지지 않습니다.
//
//  Home Page : http://cafe.naver.com/yssoperatingsystem
//  Copyright 2021. yss Embedded Operating System all right reserved.
//
//  주담당자 : 아이구 (mymy49@nate.com) 2016.04.30 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef YSS_DRV_I2C_ST_TYPE_B_REG__H_
#define YSS_DRV_I2C_ST_TYPE_B_REG__H_

#if defined(STM32F100xB) || defined(STM32F100xE) ||                                                 \
	defined(STM32F101x6) || defined(STM32F101xB) || defined(STM32F101xE) || defined(STM32F101xG) || \
	defined(STM32F102x6) || defined(STM32F102xB) ||                                                 \
	defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG) || \
	defined(STM32F105xC) ||                                                                         \
	defined(STM32F107xC) ||                                                                         \
	defined(STM32F405xx) || defined(STM32F415xx) ||                                                 \
	defined(STM32F407xx) || defined(STM32F417xx) ||                                                 \
	defined(STM32F427xx) || defined(STM32F437xx) ||                                                 \
	defined(STM32F429xx) || defined(STM32F439xx)

#include <yss/reg.h>

#define setI2cEn(addr, en) setRegBit(addr->CR1, en, 0)
#define getI2cEn(addr) getRegBit(addr->CR1, 0)
#define setI2cStart(addr) setRegBit(addr->CR1, 1, 8)
#define setI2cStop(addr) setRegBit(addr->CR1, 1, 9)
#define getI2cStop(addr) getRegBit(addr->CR1, 9)
#define setI2cAck(addr, x) setRegBit(addr->CR1, x, 10)
#define setI2cSoftReset(addr, x) setRegBit(addr->CR1, x, 15)

#define setI2cClockControl(addr, x) setRegField(addr->CCR, 0xFFFUL, x, 0)
#define setI2cFastModeDuty(addr, x) setRegBit(addr->CCR, x, 14)
#define setI2cSpeed(addr, x) setRegBit(addr->CCR, x, 15)

#define setI2cFreq(addr, freq) setRegField(addr->CR2, 0x3FUL, freq, 0)
#define setI2cIterrEn(addr, x) setRegBit(addr->CR2, x, 8)
#define setI2cItevtEn(addr, x) setRegBit(addr->CR2, x, 9)
#define setI2cItbufEn(addr, x) setRegBit(addr->CR2, x, 10)
#define setI2cDmaEn(addr, x) setRegBit(addr->CR2, x, 11)
#define setI2cLast(addr, x) setRegBit(addr->CR2, x, 12)

#define setI2cRiseTime(addr, x) setRegField(addr->TRISE, 0x3FUL, x, 0)

#define getI2cSr1(addr) addr->SR1
#define getI2cBtf(addr) getRegBit(addr->SR1, 2)
#define getI2cRxne(addr) getRegBit(addr->SR1, 6)
#define getI2cTxe(addr) getRegBit(addr->SR1, 7)
#define clrI2cSr1Af(addr) setRegBit(addr->SR1, 0, 10)
#define getI2cStartBit(addr) getRegBit(addr->SR1, 0)

#define getI2cSr2(addr) addr->SR2
#define getI2cBusy(addr) getRegBit(addr->SR2, 1)

#define reloadI2cDr(addr) addr->DR = addr->DR
#define setI2cDr(addr, data) addr->DR = data

#endif

#endif