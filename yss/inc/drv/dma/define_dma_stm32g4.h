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

#ifndef YSS_DRV_DMA_ST_TYPE_C_DEFINE__H_
#define YSS_DRV_DMA_ST_TYPE_C_DEFINE__H_

#include <drv/mcu.h>

#if defined(STM32L0) || defined (STM32G4) || defined (STM32F0)

namespace define
{
namespace dma
{
namespace priorityLevel
{
enum
{
	LOW = 0,
	MEDIUM = 1,
	HIGH = 2,
	VERY_HIGH = 3
};
}

namespace dir
{
enum
{
	PERI_TO_MEM = 0,
	MEM_TO_PERI = 1,
	MEM_TO_MEM = 2
};
}

namespace addr
{
enum
{
	FIXED = 0,
	ADDR_INC = 1
};
}

namespace burst
{
enum
{
	SINGLE = 0,
	INCR4 = 1,
	INCR8 = 2,
	INCR16 = 3
};
}

namespace size
{
enum
{
	BYTE = 0,
	HALF_WORD = 1,
	WORD = 2
};
}
}

namespace dma1
{
namespace stream1
{
enum
{
	ADC_CH1 = 0,
	TIM2_CH3 = 8,
	AES_IN = 11,
};
}

namespace stream2
{
enum
{
	ADC_CH1 = 0,
	SPI1_RX = 1,
	USART1_TX = 3,
	LPUART1_TX = 5,
	I2C1_TX = 6,
	TIM2_UP = 8,
	TIM6_UP = 9,
	DAC_CH1 = 9,
	TIM3_CH3 = 10,
	AES_OUT = 11,
	USART4_RX = 12,
	USART5_RX = 13,
	I2C3_TX = 14
};
}

namespace stream3
{
enum
{
	SPI1_TX = 1,
	USART1_RX = 3,
	LPUART1_RX = 5,
	I2C1_RX = 6,
	TIM2_CH2 = 8,
	TIM3_CH4 = 10,
	TIM3_UP = 10,
	AES_OUT = 11,
	USART4_TX = 12,
	USART5_TX = 13,
	I2C3_RX = 14
};
}

namespace stream4
{
enum
{
	SPI2_RX = 2,
	USART1_TX = 3,
	USART2_TX = 4,
	I2C2_TX = 7,
	TIM2_CH4 = 8,
	I2C3_TX = 14,
	TIM7_UP = 15,
	DAC_CH2 = 15
};
}

namespace stream5
{
enum
{
	SPI2_TX = 2,
	USART1_RX = 3,
	USART2_RX = 4,
	I2C2_RX = 7,
	TIM2_CH1 = 8,
	TIM3_CH1 = 10,
	AES_IN = 11,
	I2C3_RX = 14
};
}

namespace stream6
{
enum
{
	SPI2_RX = 2,
	USART2_RX = 4,
	LPUART1_RX = 5,
	I2C1_TX = 6,
	TIM3_TRIG = 10,
	USART4_RX = 12,
	USART5_RX = 13
};
}

namespace stream7
{
enum
{
	SPI2_TX = 2,
	USART2_TX = 4,
	LPUART1_TX = 5,
	I2C1_RX = 6,
	TIM2_CH2 = 8,
	TIM2_CH4 = 8,
	USART4_TX = 12,
	USART5_TX = 13
};
}
}
}

#endif

#endif

