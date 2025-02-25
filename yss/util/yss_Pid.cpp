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
//  주담당자 : 아이구 (mymy49@nate.com) 2020.09.01 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <util/Pid.h>
#include <util/time.h>

Pid::Pid(void)
{
	mPgain = 1.0;
	mIgain = 1.0;
	mDgain = 1.0;

	mPLimitMax = 1.0;
	mPLimitMin = -1.0;
	mILimitMax = 1.0;
	mILimitMin = -1.0;
	mDLimitMax = 1.0;
	mDLimitMin = -1.0;
	mOutputLimitMax = 1.0;
	mOutputLimitMin = -1.0;

	mBeforeError = 0;
	mIsum = 0;
	mTarget = 0;
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	mLastTime = time::getRunningUsec();
#else
	mLastTime = time::getRunningMsec();
#endif
}

float Pid::calculate(float value)
{
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	unsigned long long thisTime = time::getRunningUsec();
#else
	unsigned long long thisTime = time::getRunningMsec();
#endif
	unsigned int lapse = (unsigned int)(thisTime - mLastTime);
	float p, err, d;

	err = mTarget - value;
	p = err * mPgain;
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	mIsum += err * mIgain * ((float)lapse / (float)1000000);
#else
	mIsum += err * mIgain * ((float)lapse / (float)1000);
#endif
	d = (err - mBeforeError) * mDgain;

	if (p > mPLimitMax)
		p = mPLimitMax;
	else if (p < mPLimitMin)
		p = mPLimitMin;

	if (mIsum > mILimitMax)
		mIsum = mILimitMax;
	else if (mIsum < mILimitMin)
		mIsum = mILimitMin;

	if (d > mDLimitMax)
		d = mDLimitMax;
	else if (d < mDLimitMin)
		d = mDLimitMin;

	p = p + mIsum + d;
	if (p > mOutputLimitMax)
		p = mOutputLimitMax;
	else if (p < mOutputLimitMin)
		p = mOutputLimitMin;

	mLastTime = thisTime;
	mBeforeError = err;

	return p;
}

float Pid::calculate(float target, float value)
{
	mTarget = target;
	return calculate(value);
}

void Pid::setPgain(float gain)
{
	mPgain = gain;
}

void Pid::setIgain(float gain)
{
	mIgain = gain;
}

void Pid::setDgain(float gain)
{
	mDgain = gain;
}

void Pid::setGain(float p, float i, float d)
{
	mPgain = p;
	mIgain = i;
	mDgain = d;
}

float Pid::getPgain(void)
{
	return mPgain;
}

float Pid::getIgain(void)
{
	return mIgain;
}

float Pid::getDgain(void)
{
	return mDgain;
}

void Pid::setLimit(float min, float max)
{
	mOutputLimitMin = min;
	mOutputLimitMax = max;
}

void Pid::setPLimit(float min, float max)
{
	mPLimitMin = min;
	mPLimitMax = max;
}

void Pid::setILimit(float min, float max)
{
	mILimitMin = min;
	mILimitMax = max;
}

void Pid::setDLimit(float min, float max)
{
	mDLimitMin = min;
	mDLimitMax = max;
}

float Pid::getPLimitMin(void)
{
	return mPLimitMin;
}

float Pid::getPLimitMax(void)
{
	return mPLimitMax;
}

float Pid::getILimitMin(void)
{
	return mILimitMin;
}

float Pid::getILimitMax(void)
{
	return mILimitMax;
}

float Pid::getDLimitMin(void)
{
	return mDLimitMin;
}

float Pid::getDLimitMax(void)
{
	return mDLimitMax;
}

float Pid::getCurrentP(void)
{
	return mP;
}

float Pid::getCurrentIsum(void)
{
	return mIsum;
}

float Pid::getCurrentD(void)
{
	return mD;
}

void Pid::setTarget(float target)
{
	mTarget = target;
}

float Pid::getTarget(void)
{
	return mTarget;
}

void Pid::reset(void)
{
	mIsum = 0;
#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))
	mLastTime = time::getRunningUsec();
#else
	mLastTime = time::getRunningMsec();
#endif
}

float Pid::getError(void)
{
	return mBeforeError;
}