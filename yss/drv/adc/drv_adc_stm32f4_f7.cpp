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
//  주담당자 : 아이구 (mymy49@nate.com) 2020.07.01 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <drv/mcu.h>

#if defined(STM32F7) || defined(STM32F4)

#include <drv/Adc.h>
#include <drv/adc/register_adc_stm32f4_f7.h>

namespace drv
{
Adc::Adc(YSS_ADC_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void)) : Drv(clockFunc, nvicFunc, resetFunc)
{
	mPeri = peri;
	mIndex = 0;
	mNumOfCh = 0;

	for (int i = 0; i < 18; i++)
	{
		mChannel[i] = 0;
		mResult[i] = 0;
		mLpfLv[i] = define::adc::lpfLv::LV0;
		mBit[i] = define::adc::bit::BIT12;
	}
}

bool Adc::init(void)
{
#if defined(ADC123_COMMON)
	ADC123_COMMON->CCR |= ADC_CCR_ADCPRE_Msk;
#endif
	// ADC on
	mPeri->CR2 |= ADC_CR2_ADON_Msk;

	// 샘플 타임 설정
	mPeri->SMPR1 = ADC_SMPR1_SMP10_Msk | ADC_SMPR1_SMP11_Msk | ADC_SMPR1_SMP12_Msk | ADC_SMPR1_SMP13_Msk | ADC_SMPR1_SMP14_Msk | ADC_SMPR1_SMP15_Msk | ADC_SMPR1_SMP16_Msk | ADC_SMPR1_SMP17_Msk | ADC_SMPR1_SMP18_Msk;
	mPeri->SMPR2 = ADC_SMPR2_SMP0_Msk | ADC_SMPR2_SMP1_Msk | ADC_SMPR2_SMP2_Msk | ADC_SMPR2_SMP3_Msk | ADC_SMPR2_SMP4_Msk | ADC_SMPR2_SMP5_Msk | ADC_SMPR2_SMP6_Msk | ADC_SMPR2_SMP7_Msk | ADC_SMPR2_SMP8_Msk | ADC_SMPR2_SMP9_Msk;

	mPeri->CR1 |= ADC_CR1_EOCIE_Msk;
	mPeri->CR2 |= ADC_CR2_SWSTART_Msk;
	return true;
}

void Adc::isr(void)
{
	if (mPeri->CR1 & ADC_CR1_EOCIE_Msk && mPeri->SR & ADC_SR_EOC_Msk)
	{
		signed int dr = mPeri->DR << 19, temp, abs;
		unsigned char index = mChannel[mIndex];

		mPeri->SR = 0;

		temp = dr - mResult[index];
		temp >>= mLpfLv[mIndex];
		mResult[index] += temp;

		mIndex++;
		if (mIndex >= mNumOfCh)
			mIndex = 0;

		mPeri->SQR3 &= ~ADC_SQR3_SQ1_Msk;
		mPeri->SQR3 |= mChannel[mIndex];
		mPeri->CR2 |= ADC_CR2_SWSTART_Msk;
	}
}

void Adc::add(unsigned char pin, unsigned char lpfLv, unsigned char bit)
{
	if (mNumOfCh >= 18)
		return;
	mChannel[mNumOfCh] = pin;
	mLpfLv[mNumOfCh] = lpfLv;
	mBit[pin] = bit;
	mNumOfCh++;
}

unsigned short Adc::get(unsigned char pin)
{
	return mResult[pin] >> mBit[pin];
}
}
#endif