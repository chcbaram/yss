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

#ifndef REG__H_
#define REG__H_

// 이전 사용 방식(나중에 제거할 예정)
#define setRegBit(des, bit, shift) \
	if (bit)                       \
		des |= 1 << shift;         \
	else                           \
		des &= ~(1 << shift)

#define getRegBit(src, shift) ((src >> (shift)) & 0x1)

template <class setRegFieldVar>
__attribute__((always_inline)) __STATIC_INLINE void setRegField(setRegFieldVar &des, unsigned long mask, unsigned long data, unsigned char sh)
{
	des = (des & ~(mask << sh)) | ((data & mask) << sh);
}
#define getRegField(addr, mask, sh) ((addr >> sh) & mask)

// 앞으로 사용할 방식
#define setBitData(des, bit, shift) \
	if (bit)                       \
		des |= 1 << shift;         \
	else                           \
		des &= ~(1 << shift)

#define getBitData(src, shift) ((src >> (shift)) & 0x1)

#define setFieldData(des, mask, data, shift) des = (des & ~mask) | ((data << shift) & mask)
#define getFieldData(src, mask, shift) ((src & mask) >> shift)


#endif