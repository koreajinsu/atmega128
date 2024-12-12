#define F_CPU 16000000UL // CPU 클럭 속도
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRB  // LED 제어를 위한 포트의 데이터 방향 레지스터
#define LED_PORT PORTB // LED를 켜고 끄기 위한 포트

#define BUTTON_A PD2 // 스위치 1번
#define BUTTON_B PD3 // 스위치 2번
#define BUTTON_C PD4 // 스위치 3번

int main(void) {
	// LED와 버튼 초기화
	LED_DDR = 0xff;       // DDRB를 출력으로 설정 (LED용)
	DDRD &= ~((1 << BUTTON_A) | (1 << BUTTON_B) | (1 << BUTTON_C)); // PD2, PD3, PD4를 입력으로 설정
	PORTD |= (1 << BUTTON_A) | (1 << BUTTON_B) | (1 << BUTTON_C); // 풀업 저항 활성화

	uint8_t ledState = 0x00; // LED 상태 변수
	uint8_t ledOn = 0; // LED on/off 상태 변수

	while (1) {
		// 스위치 1번을 눌렀을 때 (ON/OFF 토글)
		if (!(PIND & (1 << BUTTON_A))) {
			_delay_ms(50); // 디바운싱 처리
			if (!(PIND & (1 << BUTTON_A))) {
				ledOn = !ledOn; // LED 상태 토글
				if (ledOn) {
					ledState = 0xFF; // LED 켜기 (모두 켬)
					} else {
					ledState = 0x00; // LED 끄기 (모두 끔)
				}
				LED_PORT = ledState;
				while (!(PIND & (1 << BUTTON_A))); // 버튼이 떼어질 때까지 대기
			}
		}

		// 스위치 2번을 눌렀을 때 (LED 왼쪽으로 쉬프트)
		if (!(PIND & (1 << BUTTON_B))) {
			_delay_ms(50); // 디바운싱 처리
			if (!(PIND & (1 << BUTTON_B))) {
				if (ledOn) {
					ledState = (ledState << 1) | (ledState >> 7); // 왼쪽으로 순환 쉬프트
					LED_PORT = ledState;
				}
				while (!(PIND & (1 << BUTTON_B))); // 버튼이 떼어질 때까지 대기
			}
		}

		// 스위치 3번을 눌렀을 때 (LED 오른쪽으로 쉬프트)
		if (!(PIND & (1 << BUTTON_C))) {
			_delay_ms(50); // 디바운싱 처리
			if (!(PIND & (1 << BUTTON_C))) {
				if (ledOn) {
					ledState = (ledState >> 1) | (ledState << 7); // 오른쪽으로 순환 쉬프트
					LED_PORT = ledState;
				}
				while (!(PIND & (1 << BUTTON_C))); // 버튼이 떼어질 때까지 대기
			}
		}
	}
}