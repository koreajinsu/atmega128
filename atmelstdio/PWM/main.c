#define F_CPU 16000000UL //CPU클럭 시간
#include <avr/io.h>
#include <util/delay.h>
//256주기 
int main(void)
{

   DDRB |= (1<<4); //PB4번 출력으로 설정 0001 0000
   
   TCCR0 |= (1<<WGM01) | (1<<WGM00) | (1<<COM01) |(1<<CS02) |(1<<CS01); 
   //OCR0 = 128; //976hz
   
    while (1) 
    {
		for (uint8_t i=0; i<256; i++) 
		{
			OCR0 =i;
			_delay_ms(5);
		}
	}
}
// 동작 영상 동영상 참고.
