#define F_CPU 16000000UL // CPU 클럭 시간
#include <avr/io.h>
#include <util/delay.h>
#define LED_DDR     DDRB // LED 제어를 위한 포트의 데이터 방향 레지스터
#define LED_PORT    PORTB // LED를 켜고 끄기 위한 포트

int main(void)
{
	LED_DDR = 0xff; // 포트를 출력으로 설정
	
	while (1)
	{
		
		
 		for (uint8_t i = 0; i < 3; i++) // 0부터 3까지 반복                             좌 우측 사이드부터 한칸씩 안쪽으로 이동. 누적x
 		{
 			LED_PORT = ((0x80>>i) | (0x01<<i)); // LED 위치를 오른쪽으로 이동
 			_delay_ms(200);
 		}
 		
 		for (uint8_t i = 0; i < 3; i++) // 0부터 3까지 반복
 		{
 			LED_PORT = ((0x10<<i) | (0x08>>i)); // LED 위치를 오른쪽으로 이동
 			_delay_ms(200);
 		}

// 		for (uint8_t i = 0; i < 8; i++) // 0부터 7까지 반복                            한칸씩 누적으로 on, 다 켜지면 다시 한개씩 off.
// 		{
// 			int sum;
// 			sum = PORTB;
// 			PORTB = (0x01 << i) | sum;
// 			
// 			//PORTB += (0x01 << i) //위에랑 같은 의미 ㅇㅇㅇ 
// 			
// 			_delay_ms(200);
// 		}
// 		
// 		for (uint8_t i = 0; i < 9; i++)
// 		 		{
// 		 			LED_PORT = 0xff>>i;
// 		 			_delay_ms(200);
// 				}
	}
}
