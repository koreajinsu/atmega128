#include "button.h"

// 버튼 초기화 함수
void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNumber)
{
    button->ddr = ddr;            // DDR 레지스터 주소 저장 (입출력 설정용)
    button->pin = pin;            // PIN 레지스터 주소 저장 (입력 상태 읽기용)
    button->btnPin = pinNumber;   // 버튼이 연결된 핀 번호 저장
    button->prevState = RELEASED; // 버튼의 초기 상태를 RELEASED(눌리지 않은 상태)로 설정
    *button->ddr &= ~(1 << button->btnPin); // 해당 핀을 입력 모드로 설정
}

// 버튼 상태 읽기 함수
uint8_t buttonGetState(Button *button)
{
    uint8_t curState = *button->pin & (1 << button->btnPin); // 현재 버튼 상태 읽기

    // 버튼이 눌림(PUSHED) 상태로 변했을 때
    if ((curState == PUSHED) && (button->prevState == RELEASED))
    {
        _delay_ms(50);            // 디바운싱을 위한 짧은 지연
        button->prevState = PUSHED; // 이전 상태를 PUSHED로 업데이트
        return ACT_PUSH;          // 버튼이 눌렸음을 반환
    }
    // 버튼이 눌림 상태에서 뗀(RELEASED) 상태로 변했을 때
    else if ((curState != PUSHED) && (button->prevState == PUSHED))
    {
        _delay_ms(50);            // 디바운싱을 위한 짧은 지연
        button->prevState = RELEASED; // 이전 상태를 RELEASED로 업데이트
        return ACT_RELEASED;      // 버튼이 떼졌음을 반환
    }

    // 버튼 상태에 변화가 없으면 NO_ACT 반환
    return NO_ACT;
}
