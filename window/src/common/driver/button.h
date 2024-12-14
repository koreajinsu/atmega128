#include "common/def.h" 
// 공통 정의 파일 포함 (def.h에 여러 공통 매크로 및 타입 정의가 있을 것으로 예상)

#define BUTTON_DDR      DDRD        // 버튼 데이터 방향 레지스터 (입출력 설정)
#define BUTTON_PIN      PIND        // 버튼 핀 상태 레지스터 (입력 상태 읽기)
#define BUTTON_ON       0           // 버튼 상태 매크로: 버튼이 눌림
#define BUTTON_OFF      1           // 버튼 상태 매크로: 버튼이 떼짐
#define BUTTON_TOGGLE   2           // 버튼 상태 매크로: 버튼 상태 토글

// 버튼 상태를 나타내는 열거형 정의
enum {PUSHED, RELEASED};            // PUSHED: 눌림 상태, RELEASED: 떼짐 상태
enum {NO_ACT, ACT_PUSH, ACT_RELEASED}; // NO_ACT: 동작 없음, ACT_PUSH: 눌림 동작, ACT_RELEASED: 떼짐 동작

// 버튼의 하드웨어 정보 및 상태를 저장하는 구조체 정의
typedef struct
{
    volatile uint8_t *ddr;          // 데이터 방향 레지스터 포인터
    volatile uint8_t *pin;          // 핀 상태 레지스터 포인터
    uint8_t           btnPin;       // 버튼이 연결된 핀 번호
    uint8_t           prevState;    // 이전 버튼 상태 (PUSHED/RELEASED)
} Button;

// 버튼 초기화 함수 프로토타입
void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNumber);

// 버튼 상태 감지 함수 프로토타입
uint8_t buttonGetState(Button *button);
