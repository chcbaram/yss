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

#ifndef YSS_DRV_ADC__H_
#define YSS_DRV_ADC__H_

#include "peripheral.h"
#include <drv/Drv.h>

#if defined(STM32F1)
#define YSS_DRV_ADC_MAX_CH	18
#include "adc/define_adc_stm32f1.h"
typedef ADC_TypeDef		YSS_ADC_Peri;
#elif defined(STM32F4) || defined(STM32F7)
#define YSS_DRV_ADC_MAX_CH	18
#include "adc/define_adc_stm32f4_f7.h"
typedef ADC_TypeDef		YSS_ADC_Peri;
#elif defined(STM32G4)
#define YSS_DRV_ADC_MAX_CH	18
#include "adc/define_adc_stm32g4.h"
typedef ADC_TypeDef		YSS_ADC_Peri;
#else
#define YSS_DRV_ADC_MAX_CH	1
typedef void			YSS_ADC_Peri;
#endif

namespace drv
{
class Adc : public Drv
{
	YSS_ADC_Peri *mPeri;
	signed int mResult[YSS_DRV_ADC_MAX_CH];
	unsigned char mIndex;
	unsigned char mLpfLv[YSS_DRV_ADC_MAX_CH];
	unsigned char mChannel[YSS_DRV_ADC_MAX_CH];
	unsigned char mBit[YSS_DRV_ADC_MAX_CH];
	unsigned char mNumOfCh;

  public:
	Adc(YSS_ADC_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void));
	bool init(void);
	void isr(void);
	void add(unsigned char pin, unsigned char lpfLv = define::adc::lpfLv::LV0, unsigned char bit = define::adc::bit::BIT12);
	unsigned short get(unsigned char pin);
	void setSampleTime(unsigned char pin, unsigned char sampleTime);
};
}

#endif