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
//	Home Page : http://cafe.naver.com/yssoperatingsystem
//	Copyright 2020.	yss Embedded Operating System all right reserved.
//  
//  주담당자 : 아이구 (mymy49@nate.com) 2016.04.30 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef YSS_DRV_SDRAM_ST_TYPE_A_DEFINE__H_
#define YSS_DRV_SDRAM_ST_TYPE_A_DEFINE__H_

#include <drv/peripheral.h>

#if defined(STM32F4) || defined(STM32F7)

#include <drv/Gpio.h>

namespace define
{
namespace sdram
{
	namespace sdclk
	{
		enum
		{
			HCLKx2 = 2,
			HCLKx3 = 3,
		};
	}

	namespace bank
	{
		enum
		{
			BANK1 = 0,
			BANK2 = 1
		};
	}

	namespace columnAddress
	{
		enum
		{
			BIT8 = 0,
			BIT9 = 1,
			BIT10 = 2,
			BIT11 = 3
		};
	}

	namespace rowAddress
	{
		enum
		{
			BIT11 = 0,
			BIT12 = 1,
			BIT13 = 2
		};
	}

	namespace dbusWidth
	{
		enum
		{
			BIT8 = 0,
			BIT16 = 1,
			BIT32 = 2
		};
	}

	namespace internalBank
	{
		enum
		{
			TWO = 0,
			FOUR = 1
		};
	}

	namespace casLatency
	{
		enum
		{
			ONE = 1,
			TWO = 2,
			THREE = 3
		};
	}

	namespace rpipe
	{
		enum
		{
			NO_DELAY = 0,
			ONE_DELAY = 1,
			TWO_DELAY = 2
		};
	}

	namespace apbDivisionFactor
	{
		enum
		{
			NO_DIV = 0,
			DIV2 = 0x4,
			DIV4 = 0x5,
			DIV8 = 0x6,
			DIV16 = 0x7,
		};
	}
}
}

#endif

#endif
