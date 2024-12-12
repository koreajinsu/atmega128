#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "led.h"

int main(void)
{
	GPIO_LEDInit(); //ddrb = 0xff;
	uint8_t ledData = 0x80; //0b00000001; //변수 선언후 바로 초기화
	
	while (1)
	{
		for(int i=0; i<7; i++)
		{
			//ledleftshift(&ledData);  //메인문안에있는 leddata주소를 ledleftshift한테 전달한다.
			ledrightshift(&ledData);
			_delay_ms(300);
		}
	}
}