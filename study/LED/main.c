#define F_CPU 16000000UL //CPU클럭 시간
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB =0xff; //0b11111111 모두 출력으로 설정한다.
	
    
    while (1) 
    {
		PORTB = 0xff; // 0b11111111 => 모두 출력(on)
		_delay_ms(500); //0.5초
		//_delay_us()//마이크로 sec
		PORTB = 0x00; //출력 off
		_delay_ms(500); //유지시간
	}
}

