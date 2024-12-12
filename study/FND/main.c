#define F_CPU 16000000UL //CPU클럭 시간
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA =0xff; //0b11111111 모두 출력으로 설정한다.
	uint8_t FND_Number[]
	
	={0x3F, 0x06, 0x5B, 0x4F,0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
		
	int count =0; //지역변수 count, 초기값 0
	DDRA = 0xff;  //A 포트 출력 설정
	
	while (1)
	{
		PORTA = FND_Number[count];
		count = (count+1)%10; //나머지를 구해서
		_delay_ms(200);
	}
}




