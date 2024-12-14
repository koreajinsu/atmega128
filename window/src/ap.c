#include "ap.h"            // 애플리케이션 관련 헤더 파일
#include "driver/led.h"    // LED 제어 함수 선언
#include "driver/button.h" // 버튼 제어 함수 선언
#include "driver/pwm8.h"   // PWM8 제어 함수 선언

void apInit(void)
{
    // 애플리케이션 초기화 함수 (현재는 구현 없음)
}

void apMain(void)
{
    LED led;
    led.port = &PORTB;  // LED가 연결된 포트 주소 지정
    led.pin = 0;        // LED가 연결된 핀 번호 지정
    ledInit(&led);      // LED 초기화 함수 호출

    Button btnOn;
    Button btnOff;
    Button btnTog;

    // 버튼 초기화 (버튼 핀 및 포트 지정)
    buttonInit(&btnOn, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
    buttonInit(&btnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
    buttonInit(&btnTog, &BUTTON_DDR, &BUTTON_PIN, BUTTON_TOGGLE);

    pwm8Init();  // PWM8 초기화 함수 호출

    while (1)  // 무한 루프
    {
        // 'btnOn' 버튼이 눌리면 LED를 켜고, PWM 듀티 사이클을 255로 설정
        if(buttonGetState(&btnOn) == ACT_RELEASED)
        {
            ledOn(&led);
            pwm8Run(255);  // PWM 듀티를 최대 값(255)으로 설정
        }

        // 'btnOff' 버튼이 눌리면 LED를 끄고, PWM 듀티 사이클을 0으로 설정
        if(buttonGetState(&btnOff) == ACT_RELEASED)
        {
            ledOff(&led);
            pwm8Run(0);  // PWM 듀티를 0으로 설정
        }
        if(buttonGetState(&btnTog) == ACT_RELEASED)
        {
            ledOff(&led);
            pwm8Run(200);  // PWM 듀티를 0으로 설정
        }
    }
}
