#include "pwm8.h"
// PWM 관련 설정과 선언이 포함된 헤더 파일

void pwm8Init(void)
{
    DDRB |= (1 << PORTB4); 
    // PB4 핀을 출력 모드로 설정 (PWM 신호 출력 핀)

    TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS02) | (1 << CS01); 
    // 타이머/카운터0 제어 레지스터 설정:
    // - WGM01, WGM00: 고속 PWM 모드 선택
    // - COM01: 비반전 출력 설정 (Clear OC0 on Compare Match, set OC0 at BOTTOM)
    // - CS02, CS01: 분주율 256 설정 (클럭 속도 조정)
}

void pwm8Run(uint8_t duty)
{
    OCR0 = duty; 
    // OCR0 레지스터에 Duty Cycle 설정
    // 입력 값(duty)에 따라 PWM 출력의 듀티비를 조정 (0~255)
}
