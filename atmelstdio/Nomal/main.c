#define F_CPU 16000000UL //CPU클럭 시간
#include <avr/io.h>
#include <util/delay.h>


//64분주
//0.0000000625(1/16Mhz) * 64분주 = 0.000004
//64 세면 4uS 걸린다.
//0.000004 * 주기/2 = 125
//      256개 - 125묶음 = 130묶음. 근데 0부터 숫자를 세니까 131 개가 된다.
//한 주기 = 2uS
int main(void)
{
	DDRD |= (1<<7);		//7번핀 출력설정
	PORTD &= ~(1<<7);	//LOW출발
	TCCR0 |= (1<<CS02) | (1<<CS00);
	TCNT0 = 130; //6부터 시작 255까지 가면 토글
	while (1)
	{
		while((TIFR & 0x01) == 0); //TOV0 check
		PORTD = ~PORTD; //포트 반전
		TCNT0 = 130;
		TIFR =0x01;
		
	}
}

//
// #define F_CPU 16000000UL //CPU클럭 시간
// #include <avr/io.h>
// #include <util/delay.h>
//
// int main(void)
// {
// 	DDRD |= (1<<7);		//7번핀 출력설정
// 	PORTD &= ~(1<<7);	//LOW출발
// 	TCCR0 |= (1<<CS02) | (1<<CS00);
// 	TCNT0 = 6; //6부터 시작 255까지 가면 토글
// 	while (1)
// 	{
// 		while((TIFR & 0x01) == 0); //TOV0 check
// 		PORTD = ~PORTD; //포트 반전
// 		TCNT0 = 6;
// 		TIFR =0x01;
//
// 	}
// }
//


