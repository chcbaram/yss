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

#ifndef YSS_DRV_DMA__H_
#define YSS_DRV_DMA__H_

#include "mcu.h"
#include "Drv.h"

#if defined(STM32F1)
typedef DMA_TypeDef				YSS_DMA_Peri;
typedef DMA_Channel_TypeDef		YSS_DMA_Channel_Peri;
#include "dma/define_dma_stm32f1.h"
#include "dma/map_dma_stm32f1.h"
#elif defined(STM32F4) || defined(STM32F7)
typedef DMA_TypeDef				YSS_DMA_Peri;
typedef DMA_Stream_TypeDef		YSS_DMA_Channel_Peri;
#include "dma/define_dma_stm32f4_f7.h"
#include "dma/map_dma_stm32f4_f7.h"
#elif defined(STM32G4)
typedef DMA_TypeDef				YSS_DMA_Peri;
typedef DMA_Channel_TypeDef		YSS_DMA_Channel_Peri;
#include "dma/define_dma_stm32g4.h"
#include "dma/map_dma_stm32g4.h"
#else
typedef void					YSS_DMA_Peri;
typedef void					YSS_DMA_Channel_Peri;
#endif

#include <drv/Drv.h>
#include <sac/Comm.h>
#include <yss/thread.h>

namespace drv
{
class Dma : public Drv
{
  public:
	Dma(void (*clockFunc)(bool en), void (*nvicFunc)(bool en));
	void init(void);
};

class Stream : public Drv
{
#warning "아래 mMutex 삭제하기 위해 전송 장치 mutex 사용 코드 필요"
	Mutex mMutex;
	bool mCompleteFlag, mErrorFlag;
#if defined(STM32F4) || defined(STM32F7)
	unsigned int mRemainSize, mAddr;
#elif defined(STM32G4)
	unsigned char mChNum;
#endif

  protected:
	YSS_DMA_Channel_Peri *mPeri;
	YSS_DMA_Peri *mDma;

  public:
	Stream(YSS_DMA_Peri *dma, YSS_DMA_Channel_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), unsigned char ch = 0);
	void init(void);
	bool send(sac::Comm *obj, void *src, unsigned int size, unsigned int timeout);
	void pendTx(sac::Comm *obj, void *src, unsigned int size);
	void pendRx(sac::Comm *obj, void *des, unsigned int size);

	bool wait(unsigned long long timeout);
	void stop(void);
	bool receive(sac::Comm *obj, void *des, unsigned int size, unsigned int timeout);
	void isr0(void);
	void isr1(void);
	void isr2(void);
	void isr3(void);
	void isr4(void);
	void isr5(void);
	void isr6(void);
	void isr7(void);
};
}

#endif