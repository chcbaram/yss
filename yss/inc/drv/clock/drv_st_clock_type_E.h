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

#ifndef YSS_DRV_CLOCK_ST_TYPE_E__H_
#define YSS_DRV_CLOCK_ST_TYPE_E__H_

#if defined(STM32L010x4) || defined(STM32L010x6) || defined(STM32L010x8) || defined(STM32L010xB) || \
    defined(STM32L011xx) || defined(STM32L021xx) ||                                                 \
    defined(STM32L031xx) || defined(STM32L041xx) ||                                                 \
    defined(STM32L051xx) || defined(STM32L052xx) || defined(STM32L053xx) ||                         \
    defined(STM32L061xx) || defined(STM32L062xx) || defined(STM32L063xx) ||                         \
    defined(STM32L071xx) || defined(STM32L072xx) || defined(STM32L073xx) ||                         \
    defined(STM32L081xx) || defined(STM32L082xx) || defined(STM32L083xx)

#include "drv_st_clock_type_E_config.h"
#include "drv_st_clock_type_E_define.h"
#include "drv_st_clock_type_E_ec.h"
#include <config.h>
#include <yss/mcu.h>

namespace drv
{
class Mainpll
{
  public:
    bool enable(unsigned char src, unsigned char mul, unsigned char div);
    unsigned int getFreq(void);
};

class Peripheral
{
  public:
#if defined(DMA1)
    void setDmaEn(bool en);
#endif

#if defined(GPIOA)
    void setGpioAEn(bool en);
#endif
#if defined(GPIOB)
    void setGpioBEn(bool en);
#endif
#if defined(GPIOC)
    void setGpioCEn(bool en);
#endif
#if defined(GPIOD)
    void setGpioDEn(bool en);
#endif
#if defined(GPIOE)
    void setGpioEEn(bool en);
#endif
#if defined(GPIOH)
    void setGpioHEn(bool en);
#endif
#if defined(TIM2)
    void setTimer2En(bool en);
#endif
#if defined(TIM3)
    void setTimer3En(bool en);
#endif
#if defined(TIM6)
    void setTimer6En(bool en);
#endif
#if defined(TIM7)
    void setTimer7En(bool en);
#endif
#if defined(TIM21)
    void setTimer21En(bool en);
#endif
#if defined(TIM22)
    void setTimer22En(bool en);
#endif

#if defined(I2C1)
    void setI2c1En(bool en);
    void resetI2c1(void);
#endif
#if defined(I2C2)
    void setI2c2En(bool en);
    void resetI2c2(void);
#endif

#if defined(USART1)
    void setUart1En(bool en);
#endif
#if defined(USART2)
    void setUart2En(bool en);
#endif
#if defined(USART4)
    void setUart4En(bool en);
#endif
#if defined(USART5)
    void setUart5En(bool en);
#endif

#if defined(ADC1)
    void setAdc1En(bool en);
#endif

#if defined(SPI1)
    void setSpi1En(bool en);
#endif

#if defined(SPI2)
    void setSpi2En(bool en);
#endif

#if defined(DAC)
    void setDacEn(bool en);
#endif

#if defined(SYSCFG)
    void setSyscfgEn(bool en);
#endif
};

class Clock
{
  public:
    Mainpll pll;
    Peripheral peripheral;

    bool enableHse(unsigned char hseMhz);
    bool enableHsi(bool div = define::clock::hsi::div::NO_DIV, bool en = true);
    bool enableLsi(bool en = true);
    bool enableLse(bool en = true);
    bool setUsbClkSrc(unsigned char src);
    bool setSysclk(unsigned char sysclkSrc, unsigned char ahb, unsigned char apb1, unsigned char apb2);
    bool setVosRange(unsigned char range);

    unsigned int getSysClkFreq(void);
    unsigned int getApb1ClkFreq(void);
    unsigned int getApb2ClkFreq(void);
    unsigned int getTimerApb1ClkFreq(void);
    unsigned int getTimerApb2ClkFreq(void);
};
}

#if defined(RCC)
extern drv::Clock clock;
#endif

#endif

#endif