#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define FND_DATA_DDR	DDRC// 데이터 방향
#define FND_SELECT_DDR	DDRG// 디지트 방향
#define FND_DATA_PORT	PORTC// 데이터 출력
#define FND_SELECT_PORT	PORTG// 디지트 출력

void FND_Display(uint16_t data);// 출력함수, 4자리수 -> 16비트임

int main(void)
{
	FND_DATA_DDR = 0xff;// 데이터 출력 설정
	FND_SELECT_DDR = 0xff;// 디지트 출력 설정
	FND_SELECT_PORT = 0x00;// 디지트 출력 0V 로 시작

	uint16_t count = 0;// 카운트 갯수 증가
	uint32_t timeTick = 0;// 시간을 셀수 있는 변수 (틱)
	uint32_t prevTime = 0;// 이전 시간을 저장하는 변수

	while (1)
	{
		FND_Display(count);//출력 함수 호출 -> 카운트값이 증가
		if(timeTick - prevTime > 100)// 100ms 지날때마다 카운트 1증가
		{
			prevTime = timeTick;// 현재시간을 저장
			count++;
		}
		_delay_ms(1);// 딜레이 1ms
		timeTick++;// 시간을 세는 변수 1증가
	}
}


void FND_Display(uint16_t data)
{
	static uint8_t position = 0;// 디지트 선택 변수
	uint8_t fndData[]=
	{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};

	switch (position)
	{
		case 0:
		// 첫번째 자리수를 표기 하기 위해
		// 1번핀 LOW, 2,3,4번핀 HIGH
		FND_SELECT_PORT &= ~(1<<0);
		// FND_SELECT_PORT = FND_SELECT_PORT & ~(1<<0);
		FND_SELECT_PORT |= (1<<1) | (1<<2) | (1<<3);
		// FND_SELECT_PORT = FND_SELECT_PORT |(1<<1) | (1<<2) | (1<<3);
		// 입력된 데이터의 천의 자리를 구하기 위해
		FND_DATA_PORT = fndData[data/1000];//천의 자리수 를 구하는 산수
		break;

		case 1:
		FND_SELECT_PORT &= ~(1<<1); //0으로 만든다.
		FND_SELECT_PORT |= (1<<0) | (1<<2) | (1<<3); // 
		FND_DATA_PORT = fndData[data/100%10]; //100의자리수를 구한다
		break;

		case  2:
		FND_SELECT_PORT &= ~(1<<2);
		FND_SELECT_PORT |= (1<<0) | (1<<1) | (1<<3);
		FND_DATA_PORT = fndData[data/10%10]; //10의 자리를 구한다.
		break;

		case 3:
		FND_SELECT_PORT &= ~(1<<3);
		FND_SELECT_PORT |= (1<<0) | (1<<1) | (1<<2);
		FND_DATA_PORT = fndData[data%10];
		break;
	}
	position++;//다음 자리수 이동
	position = position % 4;
}