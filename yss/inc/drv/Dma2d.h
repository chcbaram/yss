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

#ifndef YSS_DRV_DMA2D__H_
#define YSS_DRV_DMA2D__H_

#include "peripheral.h"

#if defined(DMA2D)

#if defined(STM32F7) || defined(STM32F4)
#include "dma2d/define_dma2d_stm32f4_f7.h"
#endif

#include <drv/Drv.h>
#include <yss/gui.h>
#include <yss/thread.h>

class Font;

struct FontSize
{
	unsigned char width;
	unsigned char height;
};

struct FontInfo
{
	FontSize *size;
	signed char *yPos;
	unsigned long *pointer;
	unsigned char *base;
};

namespace drv
{
class Dma2d : public Drv
{
	FontInfo mFontInfo;
#warning "mMutex 삭제 위해서는 호출하는 곳에서 lock 사용해야 함"
	Mutex mMutex;

  public:
	Dma2d(DMA2D_TypeDef *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en));
	void init(void);
	void setFontColor(unsigned char red, unsigned char green, unsigned char blue);
	void draw(Object &des, Object &src);
	void drawArea(Object &des, Pos areaPos, Size areaSize, Object &src);
	Size calculateStringSize(char *str);
	unsigned short convertUtf8ToUtf16(char *ch);
	unsigned char getFontSize(void);

	unsigned char drawChar(Rgb565 &des, Font *font, unsigned int utf8, Pos pos, unsigned int color, unsigned char alpha = 0xff);
	void fill(Rgb565 &obj, RGB565_union color);
	void fillRectangle(Rgb565 &obj, Pos pos, Size size, RGB565_union color);
	void fillRectangle(Rgb565 &obj, Pos sp, Pos ep, RGB565_union color);
	void draw(Rgb565 &des, Rgb565 &src, Pos pos);
	void draw(Rgb565 &des, const Bmp565 *bmp, Pos pos);
	void drawArea(Rgb565 &des, Pos areaPos, Size areaSize, Rgb565 &src, Pos srcPos);

	unsigned char drawChar(Rgb888 &des, Font *font, unsigned int utf8, Pos pos, unsigned int color, unsigned char alpha = 0xff);
	void fill(Rgb888 &obj, RGB888_union color);
	void fillRectangle(Rgb888 &obj, Pos pos, Size size, RGB888_union color);
	void fillRectangle(Rgb888 &obj, Pos sp, Pos ep, RGB888_union color);
	void draw(Rgb888 &des, Rgb888 &src, Pos pos);
	void draw(Rgb888 &des, const Bmp565 *bmp, Pos pos);
	void drawArea(Rgb888 &des, Pos areaPos, Size areaSize, Rgb888 &src, Pos srcPos);
};
}

#endif

#endif


