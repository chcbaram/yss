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

#include <__cross_studio_io.h>

#include <config.h>
#include <yss/instance.h>

#include <drv/peripheral.h>

#if defined(STM32F7) || defined(STM32F4) || defined(STM32F1) || defined(STM32G4) || defined(STM32L0) || defined(STM32L4) || defined(STM32F0)

static unsigned long long gYssTimeSum = (unsigned long long)-60000;
static unsigned int gOverFlowCnt = 60000;

#else

#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
static unsigned long long gYssTimeSum;
#else
static unsigned int gYssTimeSum;
#endif

static unsigned int gOverFlowCnt;

#endif

static bool gPreUpdateFlag;
static Mutex gMutex;

static void isr(void)
{
	if (!gPreUpdateFlag)
		gYssTimeSum += gOverFlowCnt;
	else
		gPreUpdateFlag = false;
}

void initSystemTime(void)
{
#ifndef YSS_DRV_TIMER_NOT_SUPPORT
	YSS_TIMER.setClockEn(true);
	YSS_TIMER.initSystemTime();
	gOverFlowCnt = YSS_TIMER.getOverFlowCount();
	YSS_TIMER.setUpdateIsr(isr);
	YSS_TIMER.setIntEn(true);
	YSS_TIMER.start();
#endif
}

namespace time
{
unsigned long long gLastRequestTime;

unsigned int getRunningSec(void)
{
#ifndef YSS_DRV_TIMER_NOT_SUPPORT
	__disable_irq();
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	unsigned long long time = gYssTimeSum + YSS_TIMER.getCounterValue();
#else
	unsigned int time = gYssTimeSum + YSS_TIMER.getCounterValue();
#endif

	// 타이머 인터럽트 지연으로 인한 시간 오류 발생 보완용
	if (time < gLastRequestTime)
	{
		gYssTimeSum += gOverFlowCnt;
		time += gOverFlowCnt;
		gPreUpdateFlag = true;
	}
	gLastRequestTime = time;
	__enable_irq();
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	return time / 1000000;
#else
	return time / 1000;
#endif
#else
	return 0;
#endif
}

unsigned int getRunningMsec(void)
{
#ifndef YSS_DRV_TIMER_NOT_SUPPORT

	__disable_irq();
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	unsigned long long time = gYssTimeSum + YSS_TIMER.getCounterValue();
#else
	unsigned int time = gYssTimeSum + YSS_TIMER.getCounterValue();
#endif

	// 타이머 인터럽트 지연으로 인한 시간 오류 발생 보완용
	if (time < gLastRequestTime)
	{
		gYssTimeSum += gOverFlowCnt;
		time += gOverFlowCnt;
		gPreUpdateFlag = true;
	}
	gLastRequestTime = time;
	__enable_irq();
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	return time / 1000;
#else
	return time;
#endif
#else
	return 0;
#endif
}

#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
unsigned long long getRunningUsec(void)
{
#ifndef YSS_DRV_TIMER_NOT_SUPPORT
	__disable_irq();
	unsigned long long time = gYssTimeSum + YSS_TIMER.getCounterValue();

	// 타이머 인터럽트 지연으로 인한 시간 오류 발생 보완용
	if (time < gLastRequestTime)
	{
		gYssTimeSum += gOverFlowCnt;
		time += gOverFlowCnt;
		gPreUpdateFlag = true;
	}
	gLastRequestTime = time;
	__enable_irq();
	return time;
#else
	return 0;
#endif
}
#endif
}