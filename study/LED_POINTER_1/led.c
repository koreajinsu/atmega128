
//함수의 정의를 모아놓은다.

#include "led.h"

//led포트 레지스터에 출력방향 설정 함수
void GPIO_LEDInit()
{
	LED_DDR = 0xff;
}

//led 포트 레지스터에 데이터를 출력하는 함수.
void GPIO_output(uint8_t data)
{
	LED_PORT = data; //data값을 portb에 출력
	
}

//led를 좌측으로 이동하는 함수
void ledleftshift(uint8_t *data)
{
	*data =(*data >> 7) | (*data << 1);
	GPIO_output(*data); //위에서 저장한 값을 GPIO_output에 던져준다.
}

//LED를 우측으로 이동하는 함수
void ledrightshift(uint8_t *data)
{
	*data = (*data << 7) | (*data >>1); //   <<7 을 이용해서 00으로 초기화 시킨다음에 >>1 을 이용해서 다시 1칸씩 shift 한다.
	GPIO_output(*data);
}
