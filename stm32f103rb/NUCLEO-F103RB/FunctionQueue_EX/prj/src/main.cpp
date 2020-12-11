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
//  주담당자 : 아이구 (mymy49@nate.com) 2020.12.01 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <__cross_studio_io.h>
#include <string.h>
#include <yss/yss.h>
#include <util/time.h>

void thread_uart1Rx(void)
{
	unsigned char data;
	while (1)
	{
		thread::yield();
		data = uart2.getWaitUntilReceive();
		debug_printf("0x%02x\n", data);
	}
}

int main(void)
{
	yss::init();

	using namespace define::gpio;

	// LED init
	gpioA.setToOutput(5);

	////UART init
	gpioA.setToAltFunc(2, altfunc::PA2_USART2_TX, ospeed::LOW, otype::PUSH_PULL);
	gpioA.setToAltFunc(3, altfunc::PA3_USART2_RX, ospeed::LOW, otype::PUSH_PULL);

	uart2.setClockEn(true);
	uart2.init(9600, 512);
	uart2.setIntEn(true);

	thread::add(thread_uart1Rx, 256);

	const char *str = "hello world!!\n\r";

	while (1)
	{
//		debug_printf("%d\r", (int)time::getRunningMsec());
		gpioA.setOutput(5, true);
		thread::delay(500);
		gpioA.setOutput(5, false);
		thread::delay(500);
		uart2.send(str, strlen(str), 1000);
	}
	return 0;
}