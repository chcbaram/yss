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
//  주담당자 : 아이구 (mymy49@nate.com) 2021.10.17 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <__cross_studio_io.h>
#include <string.h>
#include <yss/yss.h>


// PA2 USART2_TX 핀이 PA9 USART1_TX와 연결
// USART1에서 TX 라인에 송신한 데이터가 USART2의 TX 라인을 통해 수신되고 그것을 debug_printf로 출력하는 예제 코드

void thread_uart2Rx(void)
{
    unsigned char data;
    while (1)
    {
        // uart2에 데이터 수신이 있을 때까지 대기했다가 수신이 발생하면 값을 리턴 받음
        data = uart2.getWaitUntilReceive();
        debug_printf("0x%02x(%c)\n", data, data);
    }
}

int main(void)
{
    yss::init();

    using namespace define::gpio;

    //UART1 초기화 9600 baudrate, 수신 링버퍼 크기는 512 바이트
    gpioA.setAsAltFunc(9, altfunc::PA9_USART1_TX);

    uart1.setClockEn(true);
    uart1.initOneWire(9600, 512);
    uart1.setIntEn(true);

    //UART2 초기화 9600 baudrate, 수신 링버퍼 크기는 512 바이트
    gpioA.setAsAltFunc(2, altfunc::PA2_USART2_TX);

    uart2.setClockEn(true);
    uart2.initOneWire(9600, 512);
    uart2.setIntEn(true);

    // thread_uart2Rx 쓰레드 등록
    thread::add(thread_uart2Rx, 256);

    const char *str = "hello world!!";

    while (1)
    {
        // uart1로 str 전송
        uart1.send(str, strlen(str), 1000);
    }
    return 0;
}